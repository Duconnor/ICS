#include "cpu/instr.h"

make_instr_func(lgdt) {
	OPERAND gdt;
	gdt.type = OPR_MEM;
	gdt.data_size = 32;

	gdt.addr = eip + 2;

	operand_read(&gdt);

	cpu.gdtr.limit = gdt.val & 0xFFFF;
	cpu.gdtr.base = gdt.val;

	return 6;
}
