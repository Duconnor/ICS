#include "cpu/instr.h"

static void instr_execute_1op() {
	operand_read(&opr_src);
	cpu.esp -= (data_size / 8);
	*(cpu.esp) = opr_src.val;
}

make_instr_impl_1op(push, r, v);
