#include "oeuf.h"
#include <string.h>

int oe_bigint_init(oe_bigint_t *self) {
	self->parts = malloc(sizeof(int64_t) * OE_BIGINT_ALLOC_INC);
	if (!self->parts)
		return 1;
	memset(self->parts, 0, sizeof(int64_t) * OE_BIGINT_ALLOC_INC);
	self->len = 0;
	self->alloc_len = OE_BIGINT_ALLOC_INC;
	return 0;
}

void oe_bigint_free(oe_bigint_t *self) {
	free(self->parts);
	self->parts = NULL;
}

int oe_bigint_copy(oe_bigint_t *dest, oe_bigint_t *src) {
	dest->parts = malloc(sizeof(int64_t) * src->alloc_len);
	if (!dest->parts)
		return 1;
	dest->len = src->len;
	dest->alloc_len = src->alloc_len;
	memcpy(dest->parts, src->parts, sizeof(int64_t) * dest->len);
	return 0;
}
