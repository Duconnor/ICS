#include "cpu/instr.h"

make_instr_func(lgdt) {
	OPERAND gdtaddr;
	gdtaddr.data_size = 32;
	gdtaddr.type = OPR_IMM;
	gdtaddr.addr = eip + 2;

	operand_read(&gdtaddr);
	uint32_t gdt = paddr_read(gdtaddr.val, 4);
	printf("val: %x", gdt);

	cpu.gdtr.limit = gdt & 0xFFFF;
	cpu.gdtr.base = gdtaddr.val;

	return 6;
}
