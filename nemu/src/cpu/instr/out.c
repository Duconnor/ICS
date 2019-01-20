#include "cpu/instr.h"
#include "device/port_io.h"

make_instr_func(out_b) {
	operand_read(&opr_src);
	opr_src.val &= 0xFF;
	operand_read(&opr_dest);
	opr_dest.val &= 0xFFFF;
	pio_write(opr_dest.val, data_size / 8, opr_src.val);
	printf_asm_0("out (%%al), (%%dx)", "", 1);
	return 1;
}
