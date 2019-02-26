#include "cpu/instr.h"

static void instr_execute_2op() {
	operand_read(&opr_src);
	operand_read(&opr_dest);
	if (opr_src.data_size == 8 && opr_dest.data_size > 8)
		opr_src.val = sign_ext(opr_src.val, opr_src.data_size);
	opr_dest = alu_sbb(opr_src.val, opr_dest.val, data_size);
	operand_write(&opr_dest);
}

make_instr_impl_2op(sbb, rm, r, v)
