#include "cpu/instr.h"

static void instr_execute_2op() {
	operand_read(&opr_src);
	operand_read(&opr_dest);
	opr_dest.val = opr_dest.val & opr_src.val;

	cpu.eflags.CF = cpu.eflags.OF = 0;
	cpu.eflags.SF = sf(opr_dest.val, opr_dest.data_size);
	cpu.eflags.ZF = zf(opr_dest.val);
	cpu.eflags.PF = pf(opr_dest.val);
}

make_instr_impl_2op(and, i, rm, bv)
