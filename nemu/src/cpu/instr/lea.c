#include "cpu/instr.h"

make_instr_func(lea) {
	int len = 1;
	opr_src.data_size = 32;
	opr_dest.data_size = data_size;
	// processing modrm byte to get src and dest operand
	len += modrm_r_rm(eip + 1, &opr_dest, &opr_src);

	if (data_size < 32)
		opr_dest.val = opr_src.addr & 0xFFFF;
	else
		opr_dest.val = opr_src.addr;

	opr_dest.sreg = SREG_DS;
	operand_write(&opr_dest);
	print_asm_2("lea", "", 7, &opr_src, &opr_dest);

	return len; 
}
