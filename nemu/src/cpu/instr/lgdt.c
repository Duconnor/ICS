#include "cpu/instr.h"

make_instr_func(lgdt) {
	OPERAND gdtaddr;
	gdtaddr.data_size = 32;
	gdtaddr.type = OPR_IMM;
	gdtaddr.addr = eip + 2;

	operand_read(&gdtaddr);
	uint32_t gdt = paddr_read(gdtaddr.val, 4);
	printf("%x\n",gdt.val);

	cpu.gdtr.limit = gdt & 0xFFFF;
	cpu.gdtr.base = (gdt && 0xFFFF0000) >> 16;

	return 6;
}
