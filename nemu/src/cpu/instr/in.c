#include "cpu/instr.h"

static void instr_execute_1op() {
	operand_read(&opr_src);
	opr_src.val &= 0xFFFF;
	opr_dest.val = pio_read(opr_src.val, data_size);
	operand_wrire(&opr_dest);
}
