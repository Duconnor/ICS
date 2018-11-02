struct CacheLine {
	unsigned char valid_bit:1; // 1 valid bit
	unsigned int flag_bit:19; // q = 7, b = 6, so 32 - 7 - 6 = 19
	unsigned char slot[64]; // cache slot
};
