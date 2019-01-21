#include "cpu/instr.h"

make_instr_func(ret_near) {
	printf("here!\n");
	OPERAND ret; // hold the return address
	ret.type = OPR_MEM;
	ret.sreg = SREG_CS;
	ret.addr = cpu.esp;
	ret.data_size = data_size;

	operand_read(&ret);
	
	cpu.esp += (data_size / 8); // increment esp
	cpu.eip = ret.val;
	if (data_size == 16)
		cpu.eip &= 0xffff;
	print_asm_0("ret", "", 1);
	
	return 0;
}

make_instr_func(ret_near_imm16) {
	OPERAND ret;
	ret.type = OPR_MEM;
	ret.sreg = SREG_CS;
	ret.addr = cpu.esp;
	ret.data_size = data_size;

	operand_read(&ret);

	cpu.esp += (data_size / 8);
	cpu.eip = ret.val;
	if (data_size == 16)
		cpu.eip &= 0xffff;
	print_asm_0("ret", "", 1);

	// deal with the imm16
	OPERAND imm16;
	imm16.type = OPR_MEM;
	imm16.sreg = SREG_CS;
	imm16.addr = eip + 1;
	imm16.data_size = 16;

	operand_read(&imm16);

	cpu.esp += imm16.val;

	return 0;
}
