#include "cpu/instr.h"

make_instr_func(lgdt) {
	OPERAND gdtaddr;
	gdtaddr.data_size = 32;
	gdtaddr.type = OPR_IMM;
	gdtaddr.addr = eip + 2;

	operand_read(&gdtaddr);

	cpu.gdtr.limit = paddr_read(gdtaddr.val, 2);
	cpu.gdtr.base = paddr_read(gdtaddr.val + 2, 4);


	return 6;
}
