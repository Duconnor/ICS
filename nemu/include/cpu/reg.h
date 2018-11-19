#ifndef __REG_H__
#define __REG_H__

#include "nemu.h"

// define the structure of registers
typedef struct {

	// general purpose registers
	union {
		union {
			union {
				uint32_t _32;
				uint16_t _16;
				uint8_t _8[2];
			};
			uint32_t val;
		} gpr[8];
		struct { // do not change the order of the registers
			uint32_t eax, ecx, edx, ebx, esp, ebp, esi, edi;
		};
	};
	
	// EIP
	uint32_t eip;

	// EFLAGS
	union {
		struct {
			uint32_t CF :1;
			uint32_t dummy0 :1;
			uint32_t PF :1;
			uint32_t dummy1 :1;
			uint32_t AF :1;
			uint32_t dummy2: 1;
			uint32_t ZF :1;
			uint32_t SF :1;
			uint32_t TF :1;
			uint32_t IF :1;
			uint32_t DF :1;
			uint32_t OF :1;
			uint32_t OLIP :2;
			uint32_t NT :1;
			uint32_t dummy3 :1;
			uint32_t RF :1;
			uint32_t VM :1;
			uint32_t dummy4 :14;
		};
		uint32_t val;	
	} eflags;

#ifdef IA32_SEG
	struct GDTR {
		uint32_t base; // 32-bit address
		uint16_t limit; // 16-bit limit
	} gdtr;
	union {
		struct SegReg {
			uint32_t invisible_low;
			uint32_t invisible_high;
			uint16_t visible; // 16-bit visible selector
		} segReg[6];
		struct { SegReg es, cs, ss, ds, fs, gs; };
	};
	union CR0 {
		struct {
			uint32_t PE :1;
			uint32_t MP :1;
			uint32_t EM :1;
			uint32_t TS :1;
			uint32_t ET :1;
			uint32_t undefined :23;
			uint32_t PG :1;
		};
		uint32_t val;
	} cr0;
#endif
#ifdef IA32_PAGE
	// control registers, TODO: define type CR3
	CR3 cr3;
#endif

#ifdef IA32_INTR
	// interrupt
	IDTR idtr; // IDTR, todo: define type IDTR
	uint8_t intr;
#endif
} CPU_STATE;

enum {REG_AL, REG_CL, REG_DL, REG_BL, REG_AH, REG_CH, REG_DH, REG_BH};
enum {REG_AX, REG_CX, REG_DX, REG_BX, REG_SP, REG_BP, REG_SI, REG_DI};
enum {REG_EAX, REG_ECX, REG_EDX, REG_EBX, REG_ESP, REG_EBP, REG_ESI, REG_EDI};

enum {SREG_ES, SREG_CS, SREG_SS, SREG_DS, SREG_FS, SREG_GS};

extern char * reg_names_l[];
extern char * reg_names_w[];
extern char * reg_names_b[];
extern char * sreg_names[];

// print registers
void print_reg();
// get register values
int get_reg_val(const char *reg, bool *success);

#endif
