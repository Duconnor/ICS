#include "cpu/instr.h"
#include <stdio.h>

static void instr_execute_2op() {
	operand_read(&opr_src);
	operand_read(&opr_dest);
	alu_sub(opr_src.val, opr_dest.val, data_size);
	printf("%x\t%x\t%x\n", opr_src.val, opr_dest.val, data_size);
}

make_instr_impl_2op(cmp, i, rm, bv)
make_instr_impl_2op(cmp, r, rm, v)
make_instr_impl_2op(cmp, i, rm, v)
make_instr_impl_2op(cmp, rm, r, v)
make_instr_impl_2op(cmp, i, a, b)
make_instr_impl_2op(cmp, i, rm, b)
