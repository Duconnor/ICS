#include "cpu/instr.h"

static void instr_execute_2op() {
	operand_read(&opr_src);
	operand_read(&opr_dest);
	opr_dest.val = opr_dest.val | opr_src.val;
	operand_write(&opr_dest);

	// set eflags
	cpu.eflags.CF = cpu.eflags.OF = 0;
	cpu.eflags.ZF = zf(opr_dest.val);
	cpu.eflags.PF = pf(opr_dest.val);
	cpu.eflags.SF = sf(opr_dest.val, opr_dest.data_size);
}

make_instr_impl_2op(or, r, rm, v)
make_instr_impl_2op(or, rm, r, b)
