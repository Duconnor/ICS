#include "cpu/instr.h"
#include "device/port_io.h"

make_instr_func(out_b) {
	/*
	operand_read(&opr_src);
	opr_src.val &= 0xFF;
	operand_read(&opr_dest);
	opr_dest.val &= 0xFFFF;
	*/
	//printf("val:%x", opr_dest.val);
	pio_write(cpu.dx, 1, cpu.al);
	print_asm_0("out (%%al), (%%dx)", "", 1);
	return 1;
}
