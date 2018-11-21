#include "cpu/instr.h"

make_instr_func(lgdt) {
	OPERAND gdtaddr;
	gdtaddr.data_size = 32;
	gdtaddr.type = OPR_IMM;
	gdtaddr.addr = eip + 2;

	operand_read(&gdt);
	printf("val: %x", gdt.val);

	cpu.gdtr.limit = gdt.val & 0xFFFF;
	cpu.gdtr.base = gdt.val;

	return 6;
}
