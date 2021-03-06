#include "cpu/cpu.h"
#include "memory/memory.h"

// return the linear address from the virtual address and segment selector
uint32_t segment_translate(uint32_t offset, uint8_t sreg) {
	/* TODO: perform segment translation from virtual address to linear address
	 * by reading the invisible part of the segment register 'sreg'
	 */
	assert(cpu.segReg[sreg].invisible.base == 0);
	/*
	if (cpu.segReg[sreg].invisible.base != 0)
		printf("%x\n", cpu.eip);
		*/
	return offset + cpu.segReg[sreg].invisible.base;
}

// load the invisible part of a segment register
void load_sreg(uint8_t sreg) {
	/* TODO: load the invisibile part of the segment register 'sreg' by reading the GDT.
	 * The visible part of 'sreg' should be assigned by mov or ljmp already.
	 */
	
	// ignore ti and rpl here
	// first, get the index and the gdt
	uint32_t index = cpu.segReg[sreg].selector.index;
	//printf("%x\n", cpu.segReg[sreg].selector.index);
	uint32_t gdt = cpu.gdtr.base;

	// find the corresponding segment descriptor
	SegDesc segDesc;
	segDesc.val[0] = paddr_read(gdt + index * 8, 4);
	//printf("%x\n", cpu.gdtr.base);
	segDesc.val[1] = paddr_read(gdt + index * 8 + 4, 4);
	//printf("%x\n", segDesc.val[1]);

	// load
	cpu.segReg[sreg].invisible.base = segDesc.base_31_24 << 24 | segDesc.base_23_16 << 16 | segDesc.base_15_0;
	cpu.segReg[sreg].invisible.limit = segDesc.limit_19_16 << 16 | segDesc.limit_15_0;
	//printf("%x\n", segDesc.base_15_0);
	cpu.segReg[sreg].invisible.granularity = segDesc.granularity;
	cpu.segReg[sreg].invisible.present = segDesc.present;
	cpu.segReg[sreg].invisible.privilege_level = segDesc.privilege_level;
	//printf("gdt: %x\n", cpu.segReg[sreg].invisible.base);
	assert(cpu.segReg[sreg].invisible.base == 0 && cpu.segReg[sreg].invisible.limit == 0xFFFFF && cpu.segReg[sreg].invisible.granularity == 1 && cpu.segReg[sreg].invisible.present == 1);
}
