#include "cpu/instr.h"

make_instr_func(leave) {
	cpu.esp = cpu.ebp;
	opr_src.type = OPR_MEM;
	opr_src.sreg = SREG_CS;
	opr_src.data_size = data_size;
	opr_src.addr = cpu.esp;

	// pop out to ebp
	operand_read(&opr_src);
	cpu.esp += (data_size / 8);
	cpu.ebp = opr_src.val;

	return 1;
}
