#include "cpu/cpu.h"
#include "memory/memory.h"

// translate from linear address to physical address
paddr_t page_translate(laddr_t laddr) {
#ifndef TLB_ENABLED
	/*
	printf("\nPlease implement page_translate()\n");
	assert(0);
	*/
#ifdef IA32_PAGE
	uint32_t dir = laddr >> 22; // 10-bit
	uint32_t page = (laddr >> 12) & 0x3FF; // 10-bit
	uint32_t offset = laddr & 0xFFF; // 12-bit
	uint32_t dirbase = cpu.cr3.page_directory_base;
	// read the page directory entry
	PDE dir_entry;
	dir_entry.val = paddr_read((dirbase << 12) + 4 * dir, 4);	
	// read the page table entry
	printf("%x\n", laddr);
	assert(dir_entry.present == 1); // assert first
	PTE table_entry;
	table_entry.val = paddr_read((dir_entry.page_frame << 12) + 4 * page, 4);
	assert(table_entry.present == 1); // assert again
	//paddr_t paddr = (table_entry.page_frame << 12) | offset;
	//printf("%x\n", paddr);
	return (table_entry.page_frame << 12) | offset;
#else
	assert(0);
#endif
#else	
	return tlb_read(laddr) | (laddr & PAGE_MASK);;
#endif
}
