#include "cpu/instr.h"
#include <stdio.h>

make_instr_func(call_near) {
	OPERAND rel;
	rel.type = OPR_IMM;
	rel.sreg = SREG_CS;
	rel.data_size = data_size;
	rel.addr = eip + 1;

	operand_read(&rel);

	int offset = sign_ext(rel.val, data_size);

	// use opr_dest to implement push eip
	opr_dest.type = OPR_MEM;
	cpu.esp -= (32 / 8);
	opr_dest.addr = cpu.esp;
	eip = eip + 1 + (data_size / 8); // set eip to the next instr
	opr_dest.val = eip;
	operand_write(&opr_dest); // push old eip
	printf("eip:%x", eip);

	// change eip to the address of the target procedure
	cpu.eip = eip + offset; // change the global var "cpu.eip"
	print_asm_0("call" , "", 2);

	return 0; // global eip get set so return 0
}
