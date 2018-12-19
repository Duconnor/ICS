#include "cpu/instr.h"

static void push(uint32_t val, uint8_t len) {
	if (len == 16)
		val &= 0xFFFF;
	OPERAND ele;
	ele.type = OPR_MEM;
	cpu.esp -= (len / 8);
	ele.addr = cpu.esp;
	ele.val = sign_ext(val, len);
	ele.data_size = len;
	ele.sreg = SREG_CS;
	operand_write(&ele);
}

make_instr_func(pusha) {
	uint8_t len = data_size;
	uint32_t temp = cpu.esp;
	push(cpu.eax, len);
	push(cpu.ecx, len);
	push(cpu.edx, len);
	push(cpu.ebx, len);
	push(temp, len);
	push(cpu.ebp, len);
	push(cpu.esi, len);
	push(cpu.edi, len);
}
