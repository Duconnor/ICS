#include "cpu/instr.h"

static void instr_execute_1op() {
	operand_read(&opr_src);
	opr_dest.type = OPR_MEM;
	cpu.esp -= (opr_src.data_size / 8);
	opr_dest.addr = cpu.esp;
	printf("%x\n", cpu.esp);
	opr_dest.val = opr_src.val;
	operand_write(&opr_dest);
}

make_instr_impl_1op(push, r, v);
