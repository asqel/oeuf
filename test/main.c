#include "oeuf.h"

int main() {
	oe_bigint_t num;
	if (oe_bigint_init(&num)) {
		perror("malloc");
		return 1;
	}

	oe_bigint_add2(&num, 0xABCDEF00);
	oe_bigint_add2(&num, 0xFFFFFFFF);

	oe_bigint_print_hex(&num);
	printf("\n");
	oe_bigint_sub2(&num, 0xABCD);
	oe_bigint_print_hex(&num);
	printf("\n");
	oe_bigint_print_bin(&num);
	printf("\n");
	oe_bigint_lshift_bit(&num, 2);
	oe_bigint_print_bin(&num);
	printf("\n");
	oe_bigint_free(&num);
}
