#include "cpu/instr.h"
#include <stdio.h>

make_instr_func(ret_near) {
	OPERAND ret; // hold the return address
	ret.type = OPR_MEM;
	ret.addr = cpu.esp;
	ret.data_size = data_size;

	operand_read(&ret);
	
	cpu.esp += (data_size / 8); // increment esp
	printf("eip:%x", cpu.eip);
	cpu.eip = ret.val;
	print_asm_0("ret", "", 2);
	
	return 0;
}
