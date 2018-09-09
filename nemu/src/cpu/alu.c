#include "cpu/cpu.h"

// create a mask where lower data_size bits are 1 while other bits are 0
uint32_t create_mask(size_t data_size) {
	switch(data_size) {
		case 8:
			return 0x000000FF;
		case 16:
			return 0x0000FFFF;
		case 32:
			return 0xFFFFFFFF;
		default:
			return 0x00000000;
	}
}

inline uint32_t cf(uint32_t src, uint32_t dest, uint32_t result) {
	return result < src || result < dest;
}

uint32_t pf(uint32_t result) {
	uint32_t mask = create_mask(8);
	result = result & mask;
	uint32_t count = 0;
	for (int i = 0; i < 8; i++) {
		count += result & 1;
		result = result >> 1;
	}
	return count % 2 == 0;
}

inline uint32_t zf(uint32_t result) { return result == 0; }

inline uint32_t sf(uint32_t result, size_t data_size) {
	return (result >> (data_size - 1)) & 1;
}

inline uint32_t of(uint32_t src, uint32_t dest, uint32_t result, size_t data_size) {
	src = (src >> (data_size - 1)) & 1;
	dest = (dest >> (data_size - 1)) & 1;
	result = (result >> (data_size - 1)) & 1;
	return (src && dest && !result) || (!src && !dest && result);
}

uint32_t alu_add(uint32_t src, uint32_t dest, size_t data_size) {
#ifdef NEMU_REF_ALU
	return __ref_alu_add(src, dest, data_size);
#else
	/*
	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	assert(0);
	return 0;
	*/
	uint32_t mask = create_mask(data_size);
	src = src & mask;
	dest = dest & mask;
	uint32_t result = (src + dest) & mask; // cut out
	// set EFLAGS
	cpu.eflags.CF = cf(src, dest, result);
	cpu.eflags.PF = pf(result);
	cpu.eflags.ZF = zf(result);
	cpu.eflags.SF = sf(result, data_size);
	cpu.eflags.OF = of(src, dest, result, data_size);
	return result;
#endif
}

uint32_t alu_adc(uint32_t src, uint32_t dest, size_t data_size) {
#ifdef NEMU_REF_ALU
	return __ref_alu_adc(src, dest, data_size);
#else
	/*
	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	assert(0);
	return 0;
	*/
	uint32_t CF = cpu.eflags.CF; // save old CF
	uint32_t result = alu_add(src, dest, data_size);
	uint32_t CF_temp = cpu.eflags.CF; // save temp CF
	if (!CF)
		return result; // adc = add if CF = 1
	result = alu_add(result, CF, data_size);
	if (CF_temp)
		cpu.eflags.CF = 1; // if carried out already, set CF
	// overflow must be determined globally
	cpu.eflags.OF = of(src, dest, result, data_size);
	return result;
#endif
}


uint32_t alu_sub(uint32_t src, uint32_t dest, size_t data_size) {
#ifdef NEMU_REF_ALU
	return __ref_alu_sub(src, dest, data_size);
#else
	/*
	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	assert(0);
	return 0;
	*/
	uint32_t neg_src = ~src + 1, mask = create_mask(data_size);
	dest = dest & mask;
	src = src & mask;
	if (((neg_src & mask) != src) || src == 0) {
		// sub can be converted to add at this situation
		uint32_t result = alu_add(neg_src, dest, data_size);
		if ((src & mask) > (dest & mask))
			cpu.eflags.CF = 1;
		else
			cpu.eflags.CF = 0;
		return result;
	}
	// situation where src is NEG_MAX
	uint32_t result = (dest - src) & mask;
	// set EFLAGS
	cpu.eflags.PF = pf(result);
	cpu.eflags.ZF = zf(result);
	cpu.eflags.SF = sf(result, data_size);
	if (src > dest)
		cpu.eflags.CF = 1;
	else
		cpu.eflags.CF = 0;
	dest = dest >> (data_size - 1);
	if (dest)
		cpu.eflags.OF = 0;
	else
		cpu.eflags.OF = 1;
	return result;
#endif
}

uint32_t alu_sbb(uint32_t src, uint32_t dest, size_t data_size) {
#ifdef NEMU_REF_ALU
	return __ref_alu_sbb(src, dest, data_size);
#else
	/*
	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	assert(0);
	return 0;
	*/
	uint32_t CF = cpu.eflags.CF, result = alu_sub(src, dest, data_size), mask = create_mask(data_size);
	src &= mask;
	dest &= mask;
	if (!CF)
		return result; // if CF = 0, sbb = sub
	CF = cpu.eflags.CF; // save temp CF
	result = alu_sub(1, result, data_size);
	if (CF)
		cpu.eflags.CF = 1; // if carried out already, set CF
	uint32_t src_msb = (src >> (data_size - 1)) & 1;
	uint32_t dest_msb = (dest >> (data_size - 1)) & 1;
	uint32_t result_msb = (result >> (data_size - 1)) & 1;
	// detect overflow
	if ((dest_msb && !src_msb && !result_msb) || ((!dest_msb && src_msb && result_msb) && src && dest))
		cpu.eflags.OF = 1;
	else
		cpu.eflags.OF = 0;
	return result;
#endif
}


uint64_t alu_mul(uint32_t src, uint32_t dest, size_t data_size) {
#ifdef NEMU_REF_ALU
	return __ref_alu_mul(src, dest, data_size);
#else
	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	assert(0);
	return 0;
#endif
}

int64_t alu_imul(int32_t src, int32_t dest, size_t data_size) {
#ifdef NEMU_REF_ALU
	return __ref_alu_imul(src, dest, data_size);
#else
	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	assert(0);
	return 0;
#endif
}

uint32_t alu_div(uint64_t src, uint64_t dest, size_t data_size) {
#ifdef NEMU_REF_ALU
	return __ref_alu_div(src, dest, data_size);
#else
	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	assert(0);
	return 0;
#endif
}

int32_t alu_idiv(int64_t src, int64_t dest, size_t data_size) {
#ifdef NEMU_REF_ALU
	return __ref_alu_idiv(src, dest, data_size);
#else
	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	assert(0);
	return 0;
#endif
}

uint32_t alu_mod(uint64_t src, uint64_t dest) {
#ifdef NEMU_REF_ALU
	return __ref_alu_mod(src, dest);
#else
	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	assert(0);
	return 0;
#endif
}

int32_t alu_imod(int64_t src, int64_t dest) {
#ifdef NEMU_REF_ALU
	return __ref_alu_imod(src, dest);
#else
	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	assert(0);
	return 0;
#endif
}

uint32_t alu_and(uint32_t src, uint32_t dest, size_t data_size) {
#ifdef NEMU_REF_ALU
	return __ref_alu_and(src, dest, data_size);
#else
	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	assert(0);
	return 0;
#endif
}

uint32_t alu_xor(uint32_t src, uint32_t dest, size_t data_size) {
#ifdef NEMU_REF_ALU
	return __ref_alu_xor(src, dest, data_size);
#else
	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	assert(0);
	return 0;
#endif
}

uint32_t alu_or(uint32_t src, uint32_t dest, size_t data_size) {
#ifdef NEMU_REF_ALU
	return __ref_alu_or(src, dest, data_size);
#else
	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	assert(0);
	return 0;
#endif
}

uint32_t alu_shl(uint32_t src, uint32_t dest, size_t data_size) {
#ifdef NEMU_REF_ALU
	return __ref_alu_shl(src, dest, data_size);
#else
	/*
	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	assert(0);
	return 0;
	*/
	uint32_t mask = create_mask(data_size);
	if (src > data_size)
		cpu.eflags.CF = 0;
	else
		cpu.eflags.CF = (dest >> (data_size - src)) & 1;
	dest = (unsigned)dest << src;
	dest &= mask; // cut out the higher bits
	cpu.eflags.PF = pf(dest);
	cpu.eflags.ZF = zf(dest);
	cpu.eflags.SF = sf(dest, data_size);
	return dest;
#endif
}

uint32_t alu_shr(uint32_t src, uint32_t dest, size_t data_size) {
#ifdef NEMU_REF_ALU
	return __ref_alu_shr(src, dest, data_size);
#else
	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	assert(0);
	return 0;
#endif
}

uint32_t alu_sar(uint32_t src, uint32_t dest, size_t data_size) {
#ifdef NEMU_REF_ALU
	return __ref_alu_sar(src, dest, data_size);	
#else
	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	assert(0);
	return 0;
#endif
}

uint32_t alu_sal(uint32_t src, uint32_t dest, size_t data_size) {
#ifdef NEMU_REF_ALU
	return __ref_alu_sal(src, dest, data_size);
#else
	/*
	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	assert(0);
	return 0;
	*/
	return alu_shl(src, dest, data_size);
#endif
}
