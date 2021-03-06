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
	 PLUS = 0, SUB, MULTIPLY, DIVIDE, NEG, EQUAL, NOTEQUAL, LOGICALAND, LOGICALOR, DEREFERRENCE, NOT, NOTYPE, NUMBER, LEFTBRACKET, RIGHTBRACKET, REGISTER, SYMBOL, HEX

	/* Add more token types */

};

// the lower, the higher priority
int operator_priority[11] = {4, 4, 3, 3, 2, 7, 7, 11, 12, 2, 2};
// LEFT TO RIGHT: PLUS, SUB, MULTIPLY, DIVIDE, NEG, EQUAL, NOTEQUAL, LOGICALAND, LOGICALOR, DEREFERRENCE, NOT


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
	{"==", EQUAL},
	{"!=", NOTEQUAL},
	{"&&", LOGICALAND},
	{"\\|\\|", LOGICALOR},
	{"!", NOT},
	{"0x[a-fA-F0-9]+", HEX}, // hex has to be test before number
	{"[0-9]+", NUMBER},     // match integers and decimal numbers
	{"\\(", LEFTBRACKET},
	{"\\)", RIGHTBRACKET},
	{"\\*", MULTIPLY},
	{"/", DIVIDE},
	{"\\$e([abcd]x|[sbi]p|[sd]i)", REGISTER}, // register has to be test before symbol
	{"[a-zA-Z_][a-zA-Z_0-9]*", SYMBOL}
};

#define NR_REGEX (sizeof(rules) / sizeof(rules[0]) )

static regex_t re[NR_REGEX];

uint32_t look_up_symtab(char *sym, bool *success);

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

bool is_arithmatic_operator(int token_type) {
	return token_type == LOGICALOR || token_type == LOGICALAND || token_type == NOTEQUAL || token_type == EQUAL || token_type == NEG || token_type == PLUS || token_type == SUB || token_type == MULTIPLY || token_type == DIVIDE || token_type == NOT || token_type == DEREFERRENCE;
}

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

				printf("match regex[%d] at position %d with len %d: %.*s\n", i, position, substr_len, substr_len, substr_start);
				position += substr_len;

				/* TODO: Now a new token is recognized with rules[i]. 
				 * Add codes to perform some actions with this token.
				 */
				if (rules[i].token_type == NOTYPE)
					break;
				for (int i = 0; i < substr_len; i++)
					tokens[nr_token].str[i] = *(substr_start + i);
				tokens[nr_token].str[substr_len] = '\0';

				switch(rules[i].token_type) {
					// do special case handling only, else we could just use default case
					case MULTIPLY: {
						if (substr_start == e || is_arithmatic_operator(tokens[nr_token - 1].type) == true
							|| tokens[nr_token - 1].type == LEFTBRACKET || tokens[nr_token - 1].type == RIGHTBRACKET)
							tokens[nr_token].type = DEREFERRENCE;
						else
							tokens[nr_token].type = MULTIPLY;
						nr_token++;
					} break;
					case SUB: {
						if (substr_start == e || is_arithmatic_operator(tokens[nr_token - 1].type) == true || tokens[nr_token - 1].type == LEFTBRACKET
								|| tokens[nr_token - 1].type == RIGHTBRACKET)
							tokens[nr_token].type = NEG;
						else
							tokens[nr_token].type = SUB;
						nr_token++;
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

bool check_parentheses(int start, int end, bool *real_bad) {
	int top = 0; // top pointer pointing to the next place to push (there is a stack here, but only clever guy can see it :) )
	*real_bad = false;
	bool ret = false, match_before = false;
	for (int i = start; i <= end; i++) {
		if (tokens[i].type == LEFTBRACKET)
			top++; // push in, increase top
		else if (tokens[i].type == RIGHTBRACKET) {
			if (top <= 0) {
				// empty stack, bad expression
				*real_bad = true; // no need for further calculation
				return false;
			} else {
				top--;
				if (top == 0) {
					if (i == end && tokens[start].type == LEFTBRACKET && match_before == false) {
						// the right most is a right bracket, the left most is a left bracket and they match!!!!
						ret = true;
					} else {
						// can't be true anymore because we run out of left bracket first
						ret = false;
						match_before = true;
					}
				}
			}
		}
	}
	*real_bad = false;
	if (top != 0)
		// not match, left bracket more
		*real_bad = true;
	return ret;
}


bool is_single_operand(int token_type) {
	return token_type == NEG || token_type == NOT || token_type == DEREFERRENCE;
}

void preprocess_tokens(bool *success) {
	*success = true;
	// pre-process the tokens array
	/*-------------------------------------------*/
	for (int i = 0; i < nr_token; i++) {
		if (tokens[i].type == NEG) {
			// replace all NEG by NOTYPE and neg the number behind it
			/*
			tokens[i].type = NOTYPE;
			char neg_str[32] = "-";
			strcat(neg_str, tokens[i + 1].str);
			strcpy(tokens[i + 1].str, neg_str);
			*/
		} else if (tokens[i].type == REGISTER) {
			// replace register by their value
			tokens[i].type = NUMBER;
			uint32_t value = 0;
			bool temp_succ = false;
			value = get_reg_val(tokens[i].str + 1, &temp_succ);
			sprintf(tokens[i].str, "%d", value);
		} else if (tokens[i].type == HEX) {
			// replace hex number by their corresponding decimal number
			tokens[i].type = NUMBER;
			uint32_t value = 0;
			sscanf(tokens[i].str, "%x", &value);
			sprintf(tokens[i].str, "%d", value);
		} else if (tokens[i].type == DEREFERRENCE) {
			// replace * with NOTYPR and change the address behind it by its value in the memory
			/*
			tokens[i].type = NOTYPE;
			tokens[i + 1].type = NUMBER;
			uint32_t address = 0;
			sscanf(tokens[i + 1].str, "%x", &address);
			uint32_t value = vaddr_read(address, SREG_CS, 4);
			sprintf(tokens[i + 1].str, "%d", value);
			*/
		} else if (tokens[i].type == NOT) {
			// replace ! with NOTYPE and change the value of the number behind it
			/*
			tokens[i].type = NOTYPE;
			uint32_t value = atoi(tokens[i + 1].str);
			value = !value;
			sprintf(tokens[i + 1].str, "%d", value);
			*/
		} else if (tokens[i].type == SYMBOL) {
			// replce symbol by their value
			tokens[i].type = NUMBER;
			bool temp_success = false;
			uint32_t value = look_up_symtab(tokens[i].str, &temp_success);
			if (temp_success == false) {
				*success = false;
			   	return;
			}
			sprintf(tokens[i].str, "%d", value); // change the value back to string
		}
	}
	/*-------------------------------------------*/
	// LAST STEP
	// eliminate all NOTYPE
	Token tokens_aux[32];
	for (int i = 0; i < nr_token; i++) {
		tokens_aux[i].type = tokens[i].type;
		strcpy(tokens_aux[i].str, tokens[i].str);
	}
	// copy back and skip all NOTYPE
	int j = 0;
	for (int i = 0; i < nr_token; i++) {
		if (tokens_aux[i].type == NOTYPE)
			continue;
		else {
			tokens[j].type = tokens_aux[i].type;
			strcpy(tokens[j].str, tokens_aux[i].str);
			j++;
		}
	}
	nr_token = j;
}

uint32_t eval(int start, int end, bool *success) {
	bool real_bad = false;
	if (start > end) {
		*success = false;
		return 0;
	} else if (start == end) {
		// single token
		// now only consider number
		if (tokens[start].type != NUMBER) {
			*success = false;
			return 0;
		}
		*success = true;
		//printf("here!\n");
		return atoi(tokens[start].str);
	} else if (check_parentheses(start, end, &real_bad) == true) {
		// just throw away the parenthese
		//printf("here\n");
		return eval(start + 1, end - 1, success);
	} else {
		// find dominant operator, and split the expression there
		// recursively calculate the left part and the right part and combine them together
		
		// since parenthese mismatched, let's now consider whether it's valid or not
		if (real_bad) {
			// it's a invalid expression
			*success = false;
			return 0;
		}
		// let's first find the dominant operator
		// the order of the arithmatic operator is the same as their priority in the enum type declaration
		bool flag_continue = false;
		int position = start - 1; // the position of the dominant operator
		for (int i = start; i <= end; i++) {
			if (flag_continue == true) {
				if (tokens[i].type == RIGHTBRACKET)
					flag_continue = false; // matched right bracket find
				continue;
			}
			if (tokens[i].type == LEFTBRACKET) {
				flag_continue = true;
				// begin continue until we see a matched rightbracket
				continue;
			}
			if (is_arithmatic_operator(tokens[i].type) == true) {
				if (position == start - 1)
					position = i;
				else {
					// already a valid candidate
					if (operator_priority[tokens[position].type] <= operator_priority[tokens[i].type])
						// whether operator at i has higher priority or has the same priority but it appears later
						position = i;
				}
			}
		}
		//printf("position:%d\n", position);
		if (!is_single_operand(tokens[position].type)) {
			// two operand type
			bool success_left = false, success_right = false;
			uint32_t val_left = eval(start, position - 1, &success_left);
			if (success_left == false) {
				*success = false;
				return 0;
			}
			uint32_t val_right = eval(position + 1, end, &success_right);
			if (success_right == false) {
				*success = false;
				return 0;
			}
			*success = true;
			switch(tokens[position].type) {
				case PLUS: return val_left + val_right;
				case SUB: return val_left - val_right;
				case MULTIPLY: return val_left * val_right;
				case DIVIDE: return val_left / val_right;
				case EQUAL: return val_left == val_right;
				case NOTEQUAL: return val_left != val_right;
				case LOGICALAND: return val_left && val_right;
				case LOGICALOR: return val_left || val_right;
				default: {
					*success = false;
					return 0;
				}
			}
		} else {
			bool success_right = false;
			uint32_t val_right = eval(position + 1, end, &success_right);
			if (success_right == false) {
				*success = false;
				return 0;
			}
			*success = true;
			// printf("val: %d\n", val_right);
			switch(tokens[position].type) {
				case NEG: return -val_right;
				case NOT: return !val_right;
				case DEREFERRENCE: return vaddr_read(val_right, SREG_CS, 4);
				default: {
					*success = false;
					return 0;
				}
			}
		}
	}
}

uint32_t expr(char *e, bool *success) {
	if(!make_token(e)) {
		*success = false;
		return 0;
	}

	/*
	printf("\nPlease implement expr at expr.c\n");
	assert(0);
	*/

	preprocess_tokens(success);
	if (*success == false)
		return 0;
	return eval(0, nr_token - 1, success);
}

