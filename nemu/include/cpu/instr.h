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
make_instr_func(jmp_short); // 0xeb
make_instr_func(jmp_near_indirect); // 0xff 100

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
make_instr_func(jo_short_); // 0x70
make_instr_func(jno_short_); // 0x71
make_instr_func(jb_short_); // 0x72
make_instr_func(jae_short_); // 0x73
make_instr_func(ja_short_); // 0x77
make_instr_func(js_short_); // 0x78
make_instr_func(jns_short_); // 0x79
make_instr_func(jp_short_); // 0x7a
make_instr_func(jnp_short_); // 0x7b
make_instr_func(jl_short_); // 0x7c
make_instr_func(jge_short_); // 0x7d
make_instr_func(jle_short_); // 0x7e
make_instr_func(jg_short_); // 0x7f

// setcc
make_instr_func(setne_b); // 0x0f 0x95

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
make_instr_func(sub_r2rm_v); // 0x29
make_instr_func(sub_i2rm_v); // 0x81 101

// sbb
make_instr_func(sbb_rm2r_v); // 0x1b
make_instr_func(sbb_i2rm_v); // 0x81 011

// mul
make_instr_func(mul_rm2a_v); // 0xf7 100 pre-defined

// imul
make_instr_func(imul_rm2r_v); // 0x0f 0xaf pre-defined
make_instr_func(imul_rm2a_v); // 0xf7 101 pre-defined

// idiv
make_instr_func(idiv_rm2a_v); // 0xf7 111 pre-defined

// cmp
make_instr_func(cmp_i2rm_bv);
make_instr_func(cmp_r2rm_v);
make_instr_func(cmp_i2rm_v);
make_instr_func(cmp_rm2r_v); // 0x3b
make_instr_func(cmp_i2a_b); // 0x3c
make_instr_func(cmp_i2rm_b); // 0x80 111
make_instr_func(cmp_rm2r_b); // 0x8a
make_instr_func(cmp_i2a_v); // 0x3d

// cmps
make_instr_func(cmps_v); // 0xa7
make_instr_func(cmps_b); // 0xa6

// push
make_instr_func(push_r_v);
make_instr_func(push_rm_v);
make_instr_func(push_i_b); // 0x6a
make_instr_func(push_i_v); // 0x68

// pop
make_instr_func(pop_r_v); // 0x58 ~ 0x5f

// leave
make_instr_func(leave); // 0xc9

// call
make_instr_func(call_near);
make_instr_func(call_near_indirect); //0xff 010

// ret
make_instr_func(ret_near);
make_instr_func(ret_near_imm16); // 0xc2

// test
make_instr_func(test_r2rm_v); // 0x85
make_instr_func(test_r2rm_b); // 0x84
make_instr_func(test_i2rm_v); // 0xf7 000
make_instr_func(test_i2a_v); // 0xa9
make_instr_func(test_i2a_b); // 0xa8
make_instr_func(test_i2rm_b); // 0xf6 000

// and
make_instr_func(and_i2rm_bv); // 0x83 100
make_instr_func(and_rm2r_b); // 0x22
make_instr_func(and_r2rm_v); // 0x21
make_instr_func(and_r2rm_b); // 0x20
make_instr_func(and_i2rm_v); // 0x82 100
make_instr_func(and_i2rm_b); // 0x80 100
make_instr_func(and_i2a_v); // 0x25

// or
make_instr_func(or_r2rm_v); // 0x09
make_instr_func(or_rm2r_b); // 0x0a
make_instr_func(or_i2rm_bv);

// xor
make_instr_func(xor_r2rm_v); // 0x31

// not
make_instr_func(not_rm_v); // 0xf7 010

// lea
make_instr_func(lea); // 0x8d

// inc
make_instr_func(inc_rm_v); // 0xff 000
make_instr_func(inc_r_v); // 0x40 ~ 0x47

// dec
make_instr_func(dec_r_v); // 0x48 ~ 0x4f 
make_instr_func(dec_rm_v); // 0xff 001

// stos
make_instr_func(stos_b); // 0xaa
make_instr_func(stos_v); // 0xab

// lgdt
make_instr_func(lgdt);

// mov (special)
make_instr_func(mov_c2r_l);
make_instr_func(mov_r2c_l);
make_instr_func(mov_rm2s_w);

// jmp (special)
make_instr_func(jmp_far_imm);

// cli
make_instr_func(cli);

// lidt
make_instr_func(lidt);
#endif
