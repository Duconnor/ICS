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

make_instr_func(popa) {
	uint8_t len = data_size;
	cpu.edi = pop(len);
	cpu.esi = pop(len);
	cpu.ebp = pop(len);
	pop(len);
	cpu.ebx = pop(len);
	cpu.edx = pop(len);
	cpu.ecx = pop(len);
	cpu.eax = pop(len);
	return 1;
}
