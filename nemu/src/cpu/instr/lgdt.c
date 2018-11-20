#include "cpu/instr.h"

make_instr_func(lgdt) {
	uint32_t address = 0;
	address |= paddr_read(eip + 2, 1);
	address |= paddr_read(eip + 2, 1) << 8;
	address |= paddr_read(eip + 2, 1) << 16;
	address |= paddr_read(eip + 2, 1) << 24;
}
