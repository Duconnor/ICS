#include "cpu/instr.h"

make_instr_func(call_near) {
	src.type = OPR_IMM;
	src.data_size = data_size;
	src.addr = eip + 1;

	operand_read(&src);

	dest.type = OPR_MEM;
	cpu.esp -= (data_size / 8);
	dest.addr = cpu.esp;

	operand.write(&dest); // push old eip
}
