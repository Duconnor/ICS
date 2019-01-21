#include "nemu.h"
#include "cpu/cpu.h"
#include "memory/memory.h"
#include "device/mm_io.h"
#include "memory/cache.h"
#include <memory.h>
#include <stdio.h>


uint8_t hw_mem[MEM_SIZE_B];

uint32_t hw_mem_read(paddr_t paddr, size_t len) {
	//printf("%x %x\n", paddr, cpu.eip);
	assert(paddr < MEM_SIZE_B - len);
	uint32_t ret = 0;
	memcpy(&ret, hw_mem + paddr, len);
	return ret;
}

void hw_mem_write(paddr_t paddr, size_t len, uint32_t data) {
	//printf("%x\n", paddr);
	assert(paddr < MEM_SIZE_B - len);
	memcpy(hw_mem + paddr, &data, len);
}

uint32_t paddr_read(paddr_t paddr, size_t len) {
	uint32_t ret = 0;
#ifdef CACHE_ENABLED
	ret = cache_read(paddr, len);
#else
	ret = hw_mem_read(paddr, len);
#endif
	return ret;
}

void paddr_write(paddr_t paddr, size_t len, uint32_t data) {
#ifdef CACHE_ENABLED
	cache_write(paddr, len, data);
#else
	hw_mem_write(paddr, len, data);
#endif
}


uint32_t laddr_read(laddr_t laddr, size_t len) {
	assert(len == 1 || len == 2 || len == 4);
#ifndef IA32_PAGE
	return paddr_read(laddr, len);
#else
	//printf("%x\n", cpu.cr0.val);
	if (cpu.cr0.PE == 1 && cpu.cr0.paging == 1) {
		uint8_t start = (laddr >> 12) & 0x1;
		uint8_t end = ((laddr + (len - 1)) >> 12) & 0x1;
		if (start != end) {
			//printf("here\n");
			uint8_t low12 = laddr & 0xFFF;
			uint8_t len_frist_page = 0xFFF - low12 + 1;
			uint8_t len_second_page = len - len_frist_page;
			paddr_t paddr = page_translate(laddr);
			uint32_t result = paddr_read(paddr, len_frist_page);
			paddr = page_translate(laddr + len_frist_page);
			result |= paddr_read(paddr, len_second_page) << (len_frist_page * 8);
			return result;
		} else {
			//printf("%x\n", laddr);
			paddr_t paddr = page_translate(laddr);
			//printf("%x\n", paddr);
			return paddr_read(paddr, len);
		}
	} else {
		return paddr_read(laddr, len);
	}
#endif
}

void laddr_write(laddr_t laddr, size_t len, uint32_t data) {
#ifndef IA32_PAGE
	paddr_write(laddr, len, data);
#else
	if (cpu.cr0.PE == 1 && cpu.cr0.paging == 1) {
		// the following code is used to tell if we cross the page
		uint8_t start = (laddr >> 12) & 0x1;
		uint8_t end = ((laddr + (len - 1)) >> 12) & 0x1;
		if (start != end) {
			//assert(0);
			uint8_t low12 = laddr & 0xFFF;
			uint8_t len_first_page = 0xFFF - low12 + 1;
			uint8_t len_second_page = len - len_first_page;
			uint32_t data1 = data & (0xFFFFFFFF >> ((4 - len_first_page) * 8));
			uint32_t data2 = (data >> (len_first_page * 8)) & (0xFFFFFFFF >> ((4 - len_second_page) * 8));
			paddr_t paddr = page_translate(laddr);
			paddr_write(paddr, len_frist_page, data1);
			paddr = page_translate(laddr + len_first_page);
			paddr_write(paddr, len_second_page, data2);
			return result;
		} else {
			paddr_t paddr = page_translate(laddr);
			paddr_write(paddr, len, data);
		}
	} else {
		paddr_write(laddr, len, data);
	}
#endif
}


uint32_t vaddr_read(vaddr_t vaddr, uint8_t sreg, size_t len) {
	assert(len == 1 || len == 2 || len == 4);
#ifndef IA32_SEG
	return laddr_read(vaddr, len);
#else
	uint32_t laddr = vaddr;
	if (cpu.cr0.PE == 1) {
		laddr = segment_translate(vaddr, sreg);
	}
	//printf("read: %x\n", laddr);
	return laddr_read(laddr, len);
#endif
}

void vaddr_write(vaddr_t vaddr, uint8_t sreg, size_t len, uint32_t data) {
	assert(len == 1 || len == 2 || len == 4);
#ifndef IA32_SEG
	laddr_write(vaddr, len, data);
#else
	uint32_t laddr = vaddr;
	if (cpu.cr0.PE == 1) {
		laddr = segment_translate(vaddr, sreg);
	}
	return laddr_write(laddr, len, data);
#endif
}

void init_mem() {
	// clear the memory on initiation
	memset(hw_mem, 0, MEM_SIZE_B);

#ifdef TLB_ENABLED
	make_all_tlb();
	init_all_tlb();
#endif

#ifdef CACHE_ENABLED
	init_cache();
#endif
}

uint32_t instr_fetch(vaddr_t vaddr, size_t len) {
	assert(len == 1 || len == 2 || len == 4);
	return vaddr_read(vaddr, SREG_CS, len);
}

uint8_t * get_mem_addr() {
	return hw_mem;
}
