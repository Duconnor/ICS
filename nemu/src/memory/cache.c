#include "memory/cache.h"
#include "memory/memory.h"
#include <stdlib.h>
#include <string.h>

// 8-way set associative
#define WAYNUM 8

struct CacheLine cache[1024]; // Cache has 64KB

void init_cache() {
	memset(cache, 0, sizeof(struct CacheLine));
}

uint32_t cache_read(paddr_t paddr, size_t len) {
	// b = 6, q = 7, rest = 19
	printf("addr: %x\tlen: %d!\n", paddr, len);
	assert(len == 1 || len == 2 || len == 4 || len == 3);
	uint32_t flag = (paddr >> 13) & 0x7FFFF;
	uint32_t group_index = ((paddr >> 6) & 0x7F) * WAYNUM;
	uint32_t address_inside_group = paddr & 0x3F;
	uint32_t data = 0;
	int hit = 0, empty_line = -1;

	//printf("group_index: %d", group_index);
	// begin scan and read
	for (int i = 0; i < WAYNUM; i++) {
		int line_num = group_index + i;
		if (cache[line_num].valid_bit == 1) {
			// valid!
			if (cache[line_num].flag_bits == flag) {
				// flag bits meet
				hit = 1;
				for (int j = 0; j < len; j++) {
					data <<= 8;
					if (address_inside_group + j >= 64) {
						// cases when read cross the line
						// recursively call cache_read function
						uint32_t new_address = paddr + j; // j bytes have been read, so increase address by j
						uint32_t data_rest = cache_read(new_address, len - j);
						data <<= (len - j - 1) * 8; // because we have already shift left before, so remember minus 1 here
						data |= data_rest;
						return data;
					}
					data |= cache[line_num].slot[address_inside_group + j];
				}
				return data;
			}
		} else {
			empty_line = line_num;
			break;
		}
	}

	if (hit == 0) {
		// the data we are looking for is not in the cache
		// 1. get the data from memory
		for (int i = 0; i < len; i++) {
			data <<= 8;
			data |= hw_mem[paddr + i];
		}
		// 2. write the whole chunck of data into the cache
		uint32_t start_address = paddr & 0xC0;
		if (empty_line == -1) {
			// randomly replace one
			int replace_line = (rand() % WAYNUM) + group_index;
			memcpy(cache[replace_line].slot, hw_mem + start_address, 64);
			cache[replace_line].flag_bits = flag;
		} else {
			// there is an empty cache line
			memcpy(cache[empty_line].slot, hw_mem + start_address, 64);
			cache[empty_line].valid_bit = 1;
			cache[empty_line].flag_bits = flag;
		}
	}
	return data;
}

void cache_write(paddr_t paddr, size_t len, uint32_t data) {
	assert(len == 1 || len == 2 || len == 4 || len == 3);
	uint32_t flag = (paddr >> 13) & 0x7FFFF;
	uint32_t group_index = ((paddr >> 6) & 0x7F) * WAYNUM;
	uint32_t address_inside_group = paddr & 0x3F;
	uint32_t data_temp = data;
	int hit = 0, empty_line = -1;
	
	// printf("write here\n");

	// since we will access the memory anyway, let's write data back first
	for (int i = 0; i < len; i++) {
		hw_mem[paddr + i] = data_temp & 0xFF;
		data_temp >>= 8;
	}
	
	// now, scan the cache to see if we can find it in cache
	for (int i = 0; i < WAYNUM; i++) {
		int line_num = group_index + i;
		if (cache[line_num].valid_bit == 1) {
			// valid!
			if (cache[line_num].flag_bits == flag) {
				// flag bits meet
				hit = 1;
				for (int j = 0; j < len; j++) {
					if (address_inside_group + j > 64) {
						// cache write cross the line
						uint32_t new_address = paddr + j;
						cache_write(new_address, len - j, data);
						return;
					}
					cache[line_num].slot[address_inside_group + j] = data & 0xFF;
					data >>= 8;
				}
				return;
			}
		} else {
			empty_line = line_num;
			break;
		}
	}

	if (hit == 0) {
		// cache miss
		if (empty_line == -1) {
			// cache full
			empty_line = group_index + (rand() % WAYNUM);
		}
		uint32_t start_address = paddr & 0xC0;
		memcpy(cache[empty_line].slot, hw_mem + start_address, 64);
		cache[empty_line].valid_bit = 1;
		cache[empty_line].flag_bits = flag;
	}
}
