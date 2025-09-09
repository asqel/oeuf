#include "oeuf.h"

int oe_i64_len(int64_t n) {
	if (n == -0x7FFFFFFFFFFFFFFFLL - 1) return 19;
	if (n < 0) n = -n;
	if (n < 10LL) return 1;
	if (n < 100LL) return 2;
	if (n < 1000LL) return 3;
	if (n < 10000LL) return 4;
	if (n < 100000LL) return 5;
	if (n < 1000000LL) return 6;
	if (n < 10000000LL) return 7;
	if (n < 100000000LL) return 8;
	if (n < 1000000000LL) return 9;
	if (n < 10000000000LL) return 10;
	if (n < 100000000000LL) return 11;
	if (n < 1000000000000LL) return 12;
	if (n < 10000000000000LL) return 13;
	if (n < 100000000000000LL) return 14;
	if (n < 1000000000000000LL) return 15;
	if (n < 10000000000000000LL) return 16;
	if (n < 100000000000000000LL) return 17;
	if (n < 1000000000000000000LL) return 18;
	return 19;
}

int oe_u64_len(uint64_t n) {
	if (n < 10ULL) return 1;
	if (n < 100ULL) return 2;
	if (n < 1000ULL) return 3;
	if (n < 10000ULL) return 4;
	if (n < 100000ULL) return 5;
	if (n < 1000000ULL) return 6;
	if (n < 10000000ULL) return 7;
	if (n < 100000000ULL) return 8;
	if (n < 1000000000ULL) return 9;
	if (n < 10000000000ULL) return 10;
	if (n < 100000000000ULL) return 11;
	if (n < 1000000000000ULL) return 12;
	if (n < 10000000000000ULL) return 13;
	if (n < 100000000000000ULL) return 14;
	if (n < 1000000000000000ULL) return 15;
	if (n < 10000000000000000ULL) return 16;
	if (n < 100000000000000000ULL) return 17;
	if (n < 1000000000000000000ULL) return 18;
	return 19;
}
