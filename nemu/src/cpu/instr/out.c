#include "cpu/instr.h"
#include "device/port_io.h"

make_instr_func(out_b) {
	operand_read(&opr_src);
	opr_src.val &= 0xFF;
	opr_dest.val = opr_src.val;
	pio_write(opr_dest.val, data_size / 8);
	printf_asm_0("out (%%al), (%%dx)", "", 1);
	return 1;
}
