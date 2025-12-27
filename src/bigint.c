#include "oeuf.h"

int oe_bigint_init(oe_bigint_t *self) {
	self->parts = malloc(sizeof(uint64_t) * OE_BIGINT_ALLOC_INC);
	if (!self->parts)
		return 1;
	self->len = 0;
	self->alloc_len = OE_BIGINT_ALLOC_INC;
}

void oe_bigint_free(oe_bigint_t *self) {
	free(self->parts);
	self->parts = NULL;
}
