#include "cpu/instr.h"

make_instr_func(lidt) {
	OPERAND idtaddr;
	idtaddr.data_size = data_size;
	idtaddr.type = OPR_IMM;
	idtaddr.addr = eip + 2;

	operand_read(&idtaddr);

	cpu.idtr.limit = paddr_read(idtaddr.val, 2);
	cpu.idtr.base = paddr_read(idtaddr.val + 2, 4);
}
