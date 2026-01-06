#include "oeuf.h"

static inline void print_byte(uint8_t byte, int has_printed) {
	char c1 = "0123456789abcdef"[byte >> 4];
	char c2 = "0123456789abcdef"[byte & 0x0f];
	if (c1 != '0' || has_printed)
		printf("%c", c1);
	printf("%c", c2);
}

void oe_bigint_print_hex(oe_bigint_t *self) {
	int has_printed = 0;
	for (size_t i = self->len; i > 0; i--) {
		for (int k = 3; k >= 0; k--) {
			uint8_t byte = ((uint32_t)self->parts[i - 1]) >> (k * 8);
			if (byte || has_printed) {
				print_byte(byte, has_printed);
				has_printed = 1;
			}
		}
	}
}
