#include "oeuf.h"

int oe_bitset_init(oe_bitset_t *bitset, size_t size_bit) {
	if (size_bit <= 32) {
		bitset->size = 32;
		bitset->is_alloc = 0;
		bitset->fixed = 0;
		return 0;
	}

	size_t size_byte = size_bit / 8 + 1;
	bitset->dynamic = malloc(size_bit);
	if (!bitset->dynamic)
		return 1;
	bitset->is_alloc = 1;
	bitset->size = size_bit;
	for (size_t i = 0; i < size_byte; i++)
		bitset->dynamic[i] = 0;
	return 0;
}

void oe_bitset_set(oe_bitset_t *bitset, size_t idx) {
	if (bitset->is_alloc)
		bitset->dynamic[((size_t)idx) >> 3] |= ((size_t)1) << (idx & 7);
	else
		bitset->fixed |= ((size_t)1) << (((size_t)idx) & 7);
}

void oe_bitset_clear(oe_bitset_t *bitset, size_t idx) {
	if (bitset->is_alloc)
		bitset->dynamic[((size_t)idx) >> 3] &= ~(((size_t)1) << (idx & 7));
	else
		bitset->fixed &= ~(((size_t)1) << (((size_t)idx) & 7));
}

size_t oe_bitset_test(oe_bitset_t *bitset, size_t idx) {
	if (bitset->is_alloc)
		return (bitset->dynamic[((size_t)idx) >> 3] & (((size_t)1) << (idx & 7))) != 0;
	return (bitset->fixed & (((size_t)1) << (((size_t)idx) & 7))) != 0;
}
