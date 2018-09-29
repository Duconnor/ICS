#include "cpu/instr.h"

make_instr_func(call_near) {
	opr_src.type = OPR_IMM;
	opr_src.data_size = data_size;
	opr_src.addr = eip + 1;

	operand_read(&opr_src);

	opr_dest.type = OPR_MEM;
	cpu.esp -= (data_size / 8);
	opr_dest.addr = cpu.esp;

	operand.write(&opr_dest); // push old eip
}
