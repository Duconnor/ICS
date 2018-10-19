#include "nemu.h"
#include "cpu/reg.h"
#include "memory/memory.h"

#include <stdlib.h>

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>

enum {
	NOTYPE = 256, NUMBER, LEFTBRACKET, RIGHTBRACKET, MULTIPLY, DOLLAR, REGISTER, SYMBOL, DEREFERRENCE, NEG, PLUS, SUB

	/* Add more token types */

};

static struct rule {
	char *regex;
	int token_type;
} rules[] = {

	/* Add more rules.
	 * Pay attention to the precedence level of different rules.
	 */

	{" +",	NOTYPE},				// white space
	{"\\+", PLUS},
	{"\\-", SUB},
	{"\\d+(\\.\\d+)?", NUMBER},     // match integers and decimal numbers
	{"\\(", LEFTBRACKET},
	{"\\)", RIGHTBRACKET},
	{"\\*", MULTIPLY},
	{"\\$", DOLLAR}, // dollar symbol means using the value inside the register
	{"e([abcd]x|[sbi]p|[sd]i)", REGISTER}, // register has to be test before symbol
	{"[a-zA-Z_][a-zA-Z_0-9]*", SYMBOL}
};

#define NR_REGEX (sizeof(rules) / sizeof(rules[0]) )

static regex_t re[NR_REGEX];

/* Rules are used for more times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
	int i;
	char error_msg[128];
	int ret;

	for(i = 0; i < NR_REGEX; i ++) {
		ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
		if(ret != 0) {
			regerror(ret, &re[i], error_msg, 128);
			assert(ret != 0);
		}
	}
}

typedef struct token {
	int type;
	char str[32];
} Token;

Token tokens[32];
int nr_token;

static bool make_token(char *e) {
	int position = 0;
	int i;
	regmatch_t pmatch;
	
	nr_token = 0;

	while(e[position] != '\0') {
		/* Try all rules one by one. */
		for(i = 0; i < NR_REGEX; i ++) {
			if(regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
				char *substr_start = e + position;
				int substr_len = pmatch.rm_eo;

				printf("match regex[%d] at position %d with len %d: %.*s", i, position, substr_len, substr_len, substr_start);
				position += substr_len;

				/* TODO: Now a new token is recognized with rules[i]. 
				 * Add codes to perform some actions with this token.
				 */
				for (int i = 0; i < substr_len; i++)
					tokens[nr_token].str[i] = *(substr_start + i);
				token_type[nr_token].str[substr_len] = '\0';

				switch(rules[i].token_type) {
					// do special case handling only, else we could just use default case
					case STAR: {
						if (*(substr_start + 1) != '\0' && *(substr_start + 2) != '\0' && 
								*(substr_start + 1) == '0' && (*(substr_start + 2) == 'x' || *(substr_start + 2) == 'X')
								tokens[nr_token].type = DEREFERRENCE;
						nr_token++;
					} break;
					case SUB: {
						if (substr_start == e || tokens[nr_token - 1].type == PLUS || tokens[nr_token].type == SUB 
								|| tokens[nr_token].type == MULTIPLY
					} break;
					default: tokens[nr_token].type = rules[i].token_type;
							 nr_token ++;
				}

				break;
			}
		}

		if(i == NR_REGEX) {
			printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
			return false;
		}
	}

	return true;
}

uint32_t expr(char *e, bool *success) {
	if(!make_token(e)) {
		*success = false;
		return 0;
	}

	printf("\nPlease implement expr at expr.c\n");
	assert(0);

	return 0;
}

