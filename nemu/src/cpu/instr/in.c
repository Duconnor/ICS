#include "cpu/instr.h"

make_instr_func(in_b) {
	operand_read(&opr_src);
	opr_src.val &= 0xFFFF;
	opr_dest.val = pio_read(opr_src.val, data_size);
	operand_write(&opr_dest);
	print_asm_0("in (%%dx), (%%al)", "", 1);
	return 1;
}

