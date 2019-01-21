#include "cpu/instr.h"

make_instr_func(call_near) {
	OPERAND rel;
	rel.type = OPR_IMM;
	rel.sreg = SREG_CS;
	rel.data_size = data_size;
	rel.addr = eip + 1;

	operand_read(&rel);

	int offset = sign_ext(rel.val, data_size);

	// use opr_dest to implement push eip
	opr_dest.type = OPR_MEM;
	opr_dest.sreg = SREG_CS;
	opr_dest.data_size = 32; // eip is always 32
	cpu.esp -= (32 / 8);
	opr_dest.addr = cpu.esp;
	// may be some bug for code below where i don't know for sure whether to add this 1 or not
	eip = eip + 1 + (data_size / 8); // set eip to the next instr
	opr_dest.val = eip;
	operand_write(&opr_dest); // push old eip

	// change eip to the address of the target procedure
	cpu.eip = eip + offset; // change the global var "cpu.eip"
	print_asm_0("call" , "", 2);

	return 0; // global eip get set so return 0
}

make_instr_func(call_near_indirect) {
	int len = 1;
	OPERAND addr;
	addr.sreg = SREG_CS;
	addr.data_size = data_size;
	len += modrm_rm(eip + 1, &addr);
	operand_read(&addr);
	
	OPERAND ret_addr; // store old return address
	ret_addr.type = OPR_MEM;
	ret_addr.sreg = SREG_CS;
	ret_addr.data_size = 32;
	cpu.esp -= 4;
	ret_addr.addr = cpu.esp;
	ret_addr.val = eip + len;
	operand_write(&ret_addr);

	if (data_size == 16)
		cpu.eip = addr.val & 0xffff;
	else
		cpu.eip = addr.val;
	print_asm_0("call", "", 1);

	return 0;
}
