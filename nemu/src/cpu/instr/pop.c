#include "cpu/instr.h"

static void instr_execute_1op() {
	opr_dest.type = OPR_MEM;
	opr_dest.sreg = SREG_CS;
	opr_dest.data_size = data_size;
	opr_dest.addr = cpu.esp;
	
	// pop out and increment
	operand_read(&opr_dest);
	cpu.esp += (data_size / 8);

	opr_src.val = opr_dest.val;
	operand_write(&opr_src);
}
