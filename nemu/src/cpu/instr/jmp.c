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
