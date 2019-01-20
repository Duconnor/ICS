#include "cpu/instr.h"
#include "device/port_io.h"

make_instr_func(in_b) {
	//printf("here!\n");
	//operand_read(&opr_src);
	//opr_src.val &= 0xFFFF;
	//printf("port:%x\n", opr_src.val);
	uint32_t data = pio_read(cpu.edx & 0xFFFF, 1);
	//operand_write(&opr_dest);
	cpu.eax = data;
	print_asm_0("in (%%dx), (%%al)", "", 1);
	return 1;
}

