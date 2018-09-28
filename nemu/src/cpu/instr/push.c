#include "cpu/instr.h"

static void instr_execute_1op() {
	operand_read(&opr_src);
	opr_dest.type = MEM;
	cpu.esp -= (data_size / 8);
	opr_dest.addr = cpu.esp;
	operand_write(&opr_dest);
}

make_instr_impl_1op(push, r, v);
