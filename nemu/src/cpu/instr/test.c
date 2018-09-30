#include "cpu/instr.h"

static void instr_execute_2op() {
	operand_read(&opr_src);
	operand_read(&opr_dest);
	opr_dest.val = opr_src.val & opr_dest.val;
	operand_write(&opr_dest);

	// two eflags need to be set
	cpu.eflags.CF = 0;
	cpu.eflags.OF = 0;
}

make_instr_impl_2op(test, r, rm, v)
