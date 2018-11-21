#include "cpu/instr.h"

make_instr_func(jmp_near) {
    OPERAND rel;
    rel.type = OPR_IMM;
	rel.sreg = SREG_CS;
    rel.data_size = data_size;
    rel.addr = eip + 1;

    operand_read(&rel);

	int offset = sign_ext(rel.val, data_size);
	print_asm_1("jmp", "", 2, &rel);

	cpu.eip += offset;
	if (data_size == 16)
		cpu.eip &= 0xffff;

    return 1 + data_size / 8;
}

make_instr_func(jmp_short) {
	OPERAND rel; // rel8
	rel.type = OPR_IMM;
	rel.sreg = SREG_CS;
	rel.data_size = 8;
	rel.addr = eip + 1;

	operand_read(&rel);

	int offset = sign_ext(rel.val, 8);
	print_asm_1("jmp", "", 2, &rel);

	cpu.eip += offset;
	if (data_size == 16)
		cpu.eip &= 0xffff;

	return 2; // 1 + 8 / 8
}

make_instr_func(jmp_near_indirect) {
	OPERAND rm;
	modrm_rm(eip + 1, &rm);
	rm.data_size = data_size;
	operand_read(&rm);
	
	if (data_size == 16)
		cpu.eip = rm.val & 0xffff;
	else
		cpu.eip = rm.val;

	return 0;
}

make_instr_func(jmp_far_imm) {
	OPERAND ptr16, ptr32;
	ptr16.type = ptr32.type = OPR_IMM;
	ptr16.data_size = 16;
	ptr32.data_size = 32;

	ptr16.addr = eip + 1;
	ptr32.addr = eip + 3;

	operand_read(&ptr16);
	operand_read(&ptr32);

	cpu.cs.val = ptr16.val;
	printf("base: %x", ptr16.val);
	cpu.eip = ptr32.val;

	load_sreg(1); // ljmp must be cs

	return 0;
}
