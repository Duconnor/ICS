#include "common.h"
#include "memory.h"
#include "string.h"

#include <elf.h>

#ifdef HAS_DEVICE_IDE
#define ELF_OFFSET_IN_DISK 0
#endif

#define STACK_SIZE (1 << 20)

void ide_read(uint8_t *, uint32_t, uint32_t);
void create_video_mapping();
uint32_t get_ucr3();

uint32_t loader() {	
	//Log("Here!\n");
	//BREAK_POINT;
	Elf32_Ehdr *elf;
	Elf32_Phdr *ph, *eph;

#ifdef HAS_DEVICE_IDE
	uint8_t buf[4096];
	ide_read(buf, ELF_OFFSET_IN_DISK, 4096);
	//Log("here!\n");
	elf = (void*)buf;
	Log("ELF loading from hard disk.");
#else
	elf = (void *)0x0;
	Log("ELF loading from ram disk.");
#endif

	/* Load each program segment */
	ph = (void *)elf + elf->e_phoff;
	eph = ph + elf->e_phnum;
	for(; ph < eph; ph ++) {
		if(ph->p_type == PT_LOAD) {

			// panic("Please implement the loader");

			/* copy the segment from the ELF file to its proper memory area */
#ifndef IA32_PAGE
			char* mem = (char*)(0x0 + ph->p_vaddr);
#else
			char* mem = (char*)mm_malloc(ph->p_vaddr, ph->p_memsz);
			// Log("men:%x, vaddr:%x, memsz:%x", mem, ph->p_vaddr, ph->p_memsz);
#endif

#ifndef HAS_DEVICE_IDE
			memcpy(mem, (char*)ph->p_offset, ph->p_filesz);
#else
			ide_read((uint8_t*)mem, ELF_OFFSET_IN_DISK + ph->p_offset, ph->p_filesz);
#endif

			/* zero the memory area [vaddr + file_sz, vaddr + mem_sz) */
			memcpy(mem + ph->p_filesz, 0, ph->p_memsz - ph->p_filesz);

#ifdef IA32_PAGE
			/* Record the program break for future use */
			extern uint32_t brk;
			uint32_t new_brk = ph->p_vaddr + ph->p_memsz - 1;
			if(brk < new_brk) { brk = new_brk; }
#endif
		}
	}

	volatile uint32_t entry = elf->e_entry;

#ifdef IA32_PAGE
	mm_malloc(KOFFSET - STACK_SIZE, STACK_SIZE);
#ifdef HAS_DEVICE_VGA
	create_video_mapping();
#endif
	write_cr3(get_ucr3());
#endif
	return entry;
}
