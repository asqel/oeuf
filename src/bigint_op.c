#include "oeuf.h"

int oe_bigint_add(oe_bigint_t *self, oe_bigint_t *other) {
	size_t new_len = self->len;
	if (new_len < other->len)
		new_len = other->len;
	new_len++;
	if (new_len % OE_BIGINT_ALLOC_INC)
		new_len = (new_len / OE_BIGINT_ALLOC_INC + 1)
				* OE_BIGINT_ALLOC_INC;
	
	if (new_len > self->alloc_len) {
		int64_t *tmp = realloc(self->parts, sizeof(int64_t) * new_len);
		if (!tmp)
			return 1;
		self->alloc_len = new_len;
		self->parts = tmp;
	}
	for (size_t i = self->len; i < self->alloc_len; i++)
		self->parts[i] = 0;
	self->len = new_len;
	for (size_t i = 0; i < other->len; i++)
		self->parts[i] += other->parts[i];
	for (size_t i = 0; i < self->len - 1; i++) {
		self->parts[i + 1] += self->parts[i] >> 32;
		self->parts[i] &= 0xFFFFFFFF;
	}
	while (self->len && !self->parts[self->len - 1])
		self->len--;	
	return 0;
}

int oe_bigint_add2(oe_bigint_t *self, uint32_t other) {
	int need_resize = 0;

	if (self->len && self->parts[self->len - 1] >> 31)
		need_resize = 1;
	if (!self->len)
		need_resize = 1;
	if (need_resize && self->alloc_len < self->len + 1) {
		size_t new_len = (self->alloc_len / OE_BIGINT_ALLOC_INC + 1)
				* OE_BIGINT_ALLOC_INC;
		int64_t *tmp = realloc(self->parts, sizeof(int64_t) * new_len);
		if (!tmp)
			return 1;
		self->parts = tmp;
		for (size_t i = self->alloc_len; i < new_len; i++)
			self->parts[i] = 0;
		self->alloc_len = new_len;
	}
	if (need_resize)
		self->len++;
	self->parts[0] += other;

	for (size_t i = 0; i < self->len - 1; i++) {
		self->parts[i + 1] += self->parts[i] >> 32;
		self->parts[i] &= 0xFFFFFFFF;
	}
	while (self->len && !self->parts[self->len - 1])
		self->len--;	
	return 0;
}

int oe_bigint_sub(oe_bigint_t *self, oe_bigint_t *other) {
	if (other->len > self->len)
		return 1;
	if (other->len == self->len &&
			self->parts[self->len - 1] < other->parts[other->len - 1]) {
		return 1;
	}
	size_t i = 0;
	for (i = 0; i < other->len; i++)
		self->parts[i] -= other->parts[i];

	i = 0;
	while (i < self->len - 1 && self->parts[i] < 0) {
		self->parts[i + 1]--;
		self->parts[i] = UINT32_MAX - (-self->parts[i]) + 1;
		i++;
	}

	while (self->len && !self->parts[self->len - 1])
		self->len--;	

	
	return 0;
}

int oe_bigint_sub2(oe_bigint_t *self, uint32_t other) {
	if (self->len == 1 && self->parts[0] < other)
		return 1;
	self->parts[0] -= (int64_t)other;

	size_t i = 0;
	while (i < self->len - 1 && self->parts[i] < 0) {
		self->parts[i + 1]--;
		self->parts[i] = UINT32_MAX - (-self->parts[i]) + 1;
		i++;
	}

	while (self->len && !self->parts[self->len - 1])
		self->len--;	

	return 0;
}

int oe_bigint_lshift_bit(oe_bigint_t *self, size_t n) {
	size_t parts_to_add = (n + 31) / 32;

	if (self->len + parts_to_add > self->alloc_len) {
		size_t new_len = self->len + parts_to_add;
		new_len = (new_len + OE_BIGINT_ALLOC_INC - 1) / OE_BIGINT_ALLOC_INC;
			
		int64_t *tmp = realloc(self->parts, sizeof(int64_t) * new_len);
		if (!tmp)
			return 1;
		self->parts = tmp;
		for (size_t i = self->alloc_len; i < new_len; i++)
			self->parts[i] = 0;
		self->alloc_len = new_len;
	}
	oe_bigint_lshift_4byte(self, n / 32); 

	n = n % 32;
	if (!n)
		return 0;
	self->len++;
	
	for (size_t i = self->len; i > 1; i--) {
		uint32_t left = self->parts[i - 1];
		uint32_t right = self->parts[i - 2];

		left |= right >> (32 - n - 1);

		self->parts[i - 1] = left;
		self->parts[i - 2] = right;
	}

	while (self->len && !self->parts[self->len - 1])
		self->len--;	
	return 1;
}

int oe_bigint_lshift_4byte(oe_bigint_t *self, size_t n) {
	
}

int oe_bigint_rshift_bit(oe_bigint_t *self, size_t n);
int oe_bigint_rshift_byte(oe_bigint_t *self, size_t n);
