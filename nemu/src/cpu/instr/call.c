#include "cpu/instr.h"

make_instr_func(call_near) {
	OPERAND rel;
	rel.type = OPR_IMM;
	// rel.sreg = SREG_CS;
	rel.data_size = data_size;
	rel.addr = eip + 1;

	operand_read(&rel);

	int offset = sign_ext(rel.val, data_size);

	// use opr_dest to implement push %eip
	opr_dest.type = OPR_MEM;
	cpu.esp -= (data_size / 8);
	opr_dest.addr = cpu.esp;
	opr_dest.val = eip;
	operand_write(&opr_dest); // push old eip
	
	// change eip to the address of the target procedure
	cpu.eip += offset;

	printf("%x\n", offset);

	return 1 + data_size / 8;
}
