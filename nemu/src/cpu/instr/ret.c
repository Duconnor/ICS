#include "cpu/instr.h"

make_instr_func(ret_near) {
	OPERAND ret; // hold the return address
	ret.type = OPR_MEM;
	ret.addr = cpu.esp;
	ret.data_size = data_size;

	operand_read(&ret);
	
	cpu.esp += (data_size / 8); // decrement esp
	cpu.eip = ret.val;
	
	return 1;
}
