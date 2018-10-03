#include "cpu/instr.h"

make_instr_func(ret_near) {
	OPERAND ret; // hold the return address
	ret.type = OPR_MEM;
	ret.sreg = SREG_CS;
	ret.addr = cpu.esp;
	ret.data_size = 32;

	operand_read(&ret);
	
	cpu.esp += (data_size / 8); // increment esp
	cpu.eip = ret.val;
	print_asm_0("ret", "", 2);
	
	return 0;
}
