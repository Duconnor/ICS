#include "cpu/instr.h"

static void instr_execute_1op() {
	OPERAND ele;
	operand_read(&opr_src);
	ele.type = OPR_MEM;
	cpu.esp -= (data_size / 8);
	ele.addr = cpu.esp;
	ele.val = sign_ext(opr_src.val, opr_src.data_size);
	ele.data_size = data_size;
	ele.sreg = SREG_CS;
	operand_write(&ele);
}

make_instr_impl_1op(push, r, v)
make_instr_impl_1op(push, rm, v)
make_instr_impl_1op(push, i, b)
make_instr_impl_1op(push, i, v)
