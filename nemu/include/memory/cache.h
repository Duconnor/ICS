#include "nemu/nemu.h"

struct CacheLine {
	unsigned char valid_bit:1; // 1 valid bit
	uint32_t flag_bits:19; // q = 7, b = 6, so 32 - 7 - 6 = 19
	unsigned char slot[64]; // cache slot
};

void init_cache(); // initial cache

uint32_t cache_read(paddr_t paddr, size_t len); // read from cache

void cache_write(paddr_t paddr, size_t len, uint32_t data); // write to cache
