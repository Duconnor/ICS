#include "intr.h"
#include "cpu/instr.h"

make_instr_func(int_) {
	OPERAND int_no;
	int_no.type = OPR_IMM;
	int_no.addr = eip + 1;
	int_no.data_size = 8;
	int_no.sreg = SREG_CS;
	operand_read(&int_no);
	raise_sw_intr(int_no.val);
	return 0;
}
