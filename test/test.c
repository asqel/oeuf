#include "../oeuf.h"



int main() {
	int r;
	oe_fprintf(stdout, "%d%n\n", 42, &r);
	oe_fprintf(stdout, "%d\n", r);
	return 0;
}