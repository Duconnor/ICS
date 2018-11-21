#include "cpu/instr.h"

make_instr_func(lgdt) {
	OPERAND limit, base;
	limit.type = OPR_IMM;
	base.type = OPR_IMM;
	limit.data_size = 16;
	base.data_size = 32;

	limit.addr = eip + 2;
	base.addr = eip + 2;

	operand_read(&limit);
	operand_read(&base);

	cpu.gdtr.base = base.val;
	cpu.gdtr.limit = limit.val;

	return 6;
}
