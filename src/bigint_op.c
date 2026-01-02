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
		uint64_t *tmp = realloc(self->parts, sizeof(uint64_t) * new_len);
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
	if (need_resize && self->alloc_len < self->len + 1) {
		size_t new_len = (self->alloc_len / OE_BIGINT_ALLOC_INC + 1)
				* OE_BIGINT_ALLOC_INC;
		uint64_t *tmp = realloc(self->parts, sizeof(uint64_t) * new_len);
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

int oe_bigint_add3(oe_bigint_t *res, oe_bigint_t *a, oe_bigint_t *b) {
	if (oe_bigint_init(res)) 
		goto err;
	if (oe_bigint_add(res, a))
		goto err;
	if (oe_bigint_add(res, b));
		goto err;
	return 0;
	err:
		oe_bigint_free(res);
		return 1;
}
