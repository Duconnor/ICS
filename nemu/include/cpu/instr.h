#ifndef __INSTRUCTIONS_H__
#define __INSTRUCTIONS_H__

#include "nemu.h"
#include "cpu/cpu.h"
#include "cpu/fpu.h"
#include "cpu/modrm.h"
#include "cpu/operand.h"
#include "cpu/instr_helper.h"
#include "memory/memory.h"
#include <cpu-ref/instr_ref.h>

#include <stdio.h>
 
extern uint8_t data_size;

#include "cpu/instr/mov.h"
#include "cpu/instr/jmp.h"
#include "cpu/instr/shift.h"
#include "cpu/instr/flags.h"
#include "cpu/instr/group.h"
#include "cpu/instr/special.h"
#include "cpu/instr/x87.h"

/* TODO: add more instructions here */

// jmp
make_instr_func(jmp_near); // pre-defined
make_instr_func(jmp_short); // 0xeb

// jcc
make_instr_func(je_short_); // 0x74 pre-defined
make_instr_func(jg_short_); // 0x7f pre-defined
make_instr_func(jle_short_); // 0x7e pre-defined
make_instr_func(jne_short_); // 0x75 pre-defined
make_instr_func(jna_short_); // 0x76 pre-defined
make_instr_func(jna_near); // 0x0f 86 2byte pre-defined
make_instr_func(jo_near); // 0x0f 80 2byte pre-defined
make_instr_func(jno_near); // 0x0f 81 2byte pre-defined
make_instr_func(jb_near); // 0x0f 82 2byte pre-defined
make_instr_func(jae_near); // 0x0f 83 2byte pre-defined
make_instr_func(je_near); // 0x0f 84 2byte pre-defined
make_instr_func(jne_near); // 0x0f 85 2byte pre-defined
make_instr_func(ja_near); // 0x0f 87
make_instr_func(js_near); // 0x0f 88
make_instr_func(jns_near); // 0x0f 89
make_instr_func(jp_near); // 0x0f 8a
make_instr_func(jnp_near); // 0x0f 9b
make_instr_func(jge_short_); // 0x7d
make_instr_func(jl_near); // 0x0f 8c
make_instr_func(jge_near); // 0x0f 8d
make_instr_func(jle_near); // 0x0f 8e
make_instr_func(jg_near); // 0xf 8f

// add
make_instr_func(add_i2rm_bv);
make_instr_func(add_r2rm_b);
make_instr_func(add_i2rm_v); // 0x81 000
make_instr_func(add_i2a_v); // 0x05
make_instr_func(add_r2rm_v); // 0x01

// adc
make_instr_func(adc_r2rm_v); // 0x11

// sub
make_instr_func(sub_i2rm_bv); // 0x83 101
make_instr_func(sub_rm2r_v); // 0x2b

// cmp
make_instr_func(cmp_i2rm_bv);
make_instr_func(cmp_r2rm_v);
make_instr_func(cmp_i2rm_v);
make_instr_func(cmp_rm2r_v); // 0x3b

// push
make_instr_func(push_r_v);
make_instr_func(push_rm_v);
make_instr_func(push_i_b); // 0x6a

// pop
make_instr_func(pop_r_v); // 0x58 ~ 0x5f

// leave
make_instr_func(leave); // 0xc9

// call
make_instr_func(call_near);

// ret
make_instr_func(ret_near);

// test
make_instr_func(test_r2rm_v); // 0x85

// and
make_instr_func(and_i2rm_bv); // 0x83 100

// or
make_instr_func(or_r2rm_v); // 0x09

// xor
make_instr_func(xor_r2rm_v); // 0x31

// lea
make_instr_func(lea); // 0x8d

// inc
make_instr_func(inc_rm_v); // 0xff 000
make_instr_func(inc_r_v); // 0x40 ~ 0x47

// dec
make_instr_func(dec_r_v); // 0x48 ~ 0x4f 
#endif
