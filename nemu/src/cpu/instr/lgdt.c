#include "cpu/instr.h"

make_instr_func(lgdt) {
	OPERAND limit, base;
	limit.type = OPR_MEM;
	base.type = OPR_MEM;

	limit.data_size = 16;
	if (data_size == 16)
		base.data_size = 24;
	else
		base.data_size = 32;

	limit.addr = eip + 1;
	// TODO: determine the proper address of base
} 
