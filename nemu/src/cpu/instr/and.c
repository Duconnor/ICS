#include "cpu/instr.h"

static void instr_execute_2op() {
	operand_read(&opr_src);
	operand_read(&opr_dest);
	if (opr_src.data_size == 8)
		opr_src.val = sign_ext(opr_src.val, opr_src.data_size);
	opr_dest.val = opr_dest.val & opr_src.val;
	operand_write(&opr_dest); // magic code that destroy quick-sort

	cpu.eflags.CF = cpu.eflags.OF = 0;
	cpu.eflags.SF = sf(opr_dest.val, opr_dest.data_size);
	cpu.eflags.ZF = zf(opr_dest.val);
	cpu.eflags.PF = pf(opr_dest.val);
}

make_instr_impl_2op(and, i, rm, bv)
make_instr_impl_2op(and, rm, r, b)
make_instr_impl_2op(and, r, rm, v)
make_instr_impl_2op(and, r, rm, b)
