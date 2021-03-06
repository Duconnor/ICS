#include "cpu/instr.h"
#include <stdio.h>

make_instr_func(lgdt) {
	OPERAND gdtaddr;
	//gdtaddr.data_size = 32;
	uint32_t len = modrm_rm(eip + 1, &gdtaddr);
	//printf("%x\n", gdtaddr.type);
	//gdtaddr.type = OPR_IMM;
	//gdtaddr.addr = eip + 2;

	//operand_read(&gdtaddr);
	//printf("%x\n", gdtaddr.val);

	//cpu.gdtr.limit = paddr_read(gdtaddr.val, 2);
	//cpu.gdtr.base = paddr_read(gdtaddr.val + 2, 4);
	gdtaddr.data_size = 16;
	operand_read(&gdtaddr);
	cpu.gdtr.limit = gdtaddr.val;
	gdtaddr.data_size = 32;
	gdtaddr.addr += 2;
	operand_read(&gdtaddr);
	cpu.gdtr.base = gdtaddr.val;


	return len + 2;
}
