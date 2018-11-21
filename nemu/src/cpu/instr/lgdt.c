#include "cpu/instr.h"

make_instr_func(lgdt) {
	OPERAND gdtaddr;
	gdtaddr.data_size = 32;
	gdtaddr.type = OPR_IMM;
	gdtaddr.addr = eip + 2;

	operand_read(&gdtaddr);
	printf("val: %x", gdtaddr.val);

	cpu.gdtr.limit = gdtaddr.val & 0xFFFF;
	cpu.gdtr.base = gdtaddr.val;

	return 6;
}
