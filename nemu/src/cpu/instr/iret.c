#include "cpu/instr.h"

static uint32_t pop(uint8_t len) {
	OPERAND ele;
	ele.type = OPR_MEM;
	ele.sreg = SREG_CS;
	ele.data_size = len;
	ele.addr = cpu.esp;

	operand_read(&ele);
	cpu.esp += (len / 8);

	return ele.val;
}

make_instr_func(iret) {
	cpu.eip = pop(32);
	cpu.cs.val = pop(16);
	cpu.cs.invi_val2 = pop(32);
	cpu.cs.invi_val1 = pop(32);
	cpu.eflags.val = pop(32);
	printf("cpu.eip: %x\n", cpu.eip);
	return 0;	
}
