#include "cpu/instr.h"

make_instr_func(lgdt) {
	OPERAND gdt;
	gdt.data_size = 32;

	modrm_rm(eip + 2, &gdt);
	operand_read(&gdt);
	printf("val: %x", gdt.val);

	cpu.gdtr.limit = gdt.val & 0xFFFF;
	cpu.gdtr.base = gdt.val;

	return 6;
}
