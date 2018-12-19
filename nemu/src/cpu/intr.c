#include "cpu/intr.h"
#include "cpu/instr.h"
#include "memory/memory.h"

static void push(uint32_t val, uint8_t len) {
	if (len == 16)
		val &= 0xFFFF;
	OPERAND ele;
	ele.type = OPR_MEM;
	cpu.esp -= (len / 8);
	ele.addr = cpu.esp;
	ele.val = sign_ext(val, len);
	ele.data_size = len;
	ele.sreg = SREG_CS;
	operand_write(&ele);
}

void raise_intr(uint8_t intr_no) {
#ifdef IA32_INTR
	/*
	printf("Please implement raise_intr()");
	assert(0);
	*/
	
	// push EFLAGS, CS and EIP
	push(cpu.eflags.val, 32);
	push(cpu.cs.invi_val1, 32);
	push(cpu.cs.invi_val2, 32);
	push(cpu.cs.val, 16);
	push(cpu.eip, 32);

	// find the IDT entry
	uint32_t base = cpu.idtr.base;
	GateDesc desc;
	desc.val[0] = vaddr_read(base + intr_no * 8, SREG_DS, 4);
	desc.val[1] = vaddr_read(base + intr_no * 8 + 4, SREG_CS, 4);
	uint32_t offset = desc.offset_31_16 << 16 | desc.offset_15_0;
	uint32_t addr = vaddr_read(offset, SREG_CS, 4);
	printf("offset: %x\n", offset);

	// clear IF if it is an interrupt
	if (intr_no != 0x80)
		cpu.eflags.IF = 0;

	// set eip to the interrupt handler
	cpu.eip = addr;

#endif
}

void raise_sw_intr(uint8_t intr_no) {
	// return address is the next instruction
	cpu.eip += 2;
	raise_intr(intr_no);
}
