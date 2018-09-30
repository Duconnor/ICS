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

    return 1 + data_size / 8;
}

/*
make_instr_func(je_short_) {
	OPERAND rel; // rel8
	rel.type = OPR_IMM;
	rel.sreg = SREG_CS; // sreg here!
	rel.data_size = data_size;
	rel.addr = eip + 1;

	operand_read(&rel);

	int offset = sign_ext(rel.val, data_size);

	if (cpu.eflags.ZF)
		cpu.eip += offset;

	return 1 + data_size / 8;
}
*/
