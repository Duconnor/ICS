#include "cpu/instr.h"

static void instr_execute_1op() {
	operand_read(&opr_src);
	CPU_STATE.esp -= (data_size / 8);
	*CPU_STATE.esp = opr_src.val;
}

make_instr_impl_1op(push, r, v);
