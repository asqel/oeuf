#include "oeuf.h"

static inline void print_byte_hex(uint8_t byte, int *has_printed) {
	if (!byte && !*has_printed)
		return ;
	char c1 = "0123456789abcdef"[byte >> 4];
	char c2 = "0123456789abcdef"[byte & 0x0f];
	if (c1 != '0' || *has_printed)
		printf("%c", c1);
	printf("%c", c2);
	*has_printed = 1;
}

static inline void print_byte_bin(uint8_t byte, int *has_printed) {
	if (!byte && !*has_printed)
		return ;

	for (int i = 7; i >= 0; i--) {
		if (((byte >> i) & 1) == 0 && !*has_printed)
			continue;
		printf(((byte >> i) & 1) ? "1" : "0");
		*has_printed = 1;
	}
}

static inline void fore_each_byte(oe_bigint_t *self, void (*func)(uint8_t, int *), int *data) {
	for (size_t i = self->len; i > 0; i--) {
		for (int k = 3; k >= 0; k--)
			(*func)(((uint32_t)self->parts[i - 1]) >> (k * 8), data);
	}
		
}

void oe_bigint_print_hex(oe_bigint_t *self) {
	if (self->len == 0) {
		printf("0");
		return ;
	}
	int has_printed = 0;
	fore_each_byte(self, print_byte_hex, &has_printed);
}

void oe_bigint_print_bin(oe_bigint_t *self) {
	if (self->len == 0) {
		printf("0");
		return ;
	}
	int has_printed = 0;
	fore_each_byte(self, print_byte_bin, &has_printed);
}
