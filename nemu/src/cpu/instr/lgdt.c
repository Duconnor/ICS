#include "cpu/instr.h"

make_instr_func(lgdt) {
	OPERAND limit, base;
	limit.data_size = 16;
	base.data_size = 32;

	limit.addr
}
