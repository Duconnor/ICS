#include "cache.h"

CacheLine[1024]; // Cache has 64KB

void init_cache() {
	memset(CacheLine, 0, sizeof(CacheLine));
}

uint32_t cache_read(paddr_t paddr, size_t len) {
	
}
