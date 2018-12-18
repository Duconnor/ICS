#include "cpu/instr.h"
#include <stdio.h>

make_instr_func(lgdt) {
	OPERAND gdtaddr;
	gdtaddr.data_size = 32;
	uint32_t len = modrm_rm(eip + 2, &gdtaddr);
	printf("here\n");
	if (gdtaddr.type == OPR_IMM)
		printf("IMM\n");
	//gdtaddr.type = OPR_IMM;
	//gdtaddr.addr = eip + 2;

	operand_read(&gdtaddr);

	cpu.gdtr.limit = paddr_read(gdtaddr.val, 2);
	cpu.gdtr.base = paddr_read(gdtaddr.val + 2, 4);


	return len + 2;
}
