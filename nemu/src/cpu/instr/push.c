#include "cpu/instr.h"

static void instr_execute_1op() {
	operand_read(&opr_src);
	OPERAND ele;
	ele.type = OPR_MEM;
	cpu.esp -= (opr_src.data_size / 8);
	ele.addr = cpu.esp;
	ele.val = opr_src.val;
	ele.data_size = opr_src.data_size;
	operand_write(&opr_dest);
}

make_instr_impl_1op(push, r, v)
make_instr_impl_1op(push, rm, v)
make_instr_impl_1op(push, i, b)
