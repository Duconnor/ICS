#include "cpu/instr.h"

static void instr_execute_1op() {
	operand_read(&opr_src);
	opr_dest.type = OPR_MEM;
	cpu.esp -= (opr_src.data_size / 8);
	opr_dest.addr = cpu.esp;
	opr_dest.val = opr_src.val;
	opr_dest.data_size = opr_src.data_size;
	operand_write(&opr_dest);
}

make_instr_impl_1op(push, r, v);
