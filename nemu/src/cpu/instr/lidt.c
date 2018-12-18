#include "cpu/instr.h"

make_instr_func(lidt) {
	OPERAND idtaddr;
	uint32_t len = modrm_rm(eip + 1, &idtaddr);

	idtaddr.data_size = 16;
	operand_read(&idtaddr);
	cpu.idtr.limit = idtaddr.val;
	idtaddr.data_size = 32;
	idtaddr.addr += 2;
	operand_read(&idtaddr);
	cpu.idtr.base = idtaddr.val;

	return len + 2;
}
