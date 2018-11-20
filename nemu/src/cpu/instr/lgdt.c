#include "cpu/instr.h"

make_instr_func(lgdt) {
	uint32_t address = 0;
	address |= paddr_read(eip + 2, 1);
	address |= paddr_read(eip + 3, 1) << 8;
	address |= paddr_read(eip + 4, 1) << 16;
	address |= paddr_read(eip + 5, 1) << 24;

	cpu.gdtr.limit |= paddr_read(address, 1);
	cpu.gdtr.limit |= paddr_read(address + 1, 1) << 8;

	cpu.gdtr.base |= paddr_read(address + 2, 1);
	cpu.gdtr.base |= paddr_read(address + 3, 1) << 8;
	cpu.gdtr.base |= paddr_read(address + 4, 1) << 16;
	cpu.gdtr.base |= paddr_read(address + 5, 1) << 24;

	return 6; // 6-byte long
}
