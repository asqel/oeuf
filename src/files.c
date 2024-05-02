#include "../oeuf.h"

#include <malloc.h>
#include <stdio.h>

char *oe_read_file_bin(char *path, int *len) {
	FILE *f = fopen(path, "r");
	fseek(f, 0, SEEK_END);
	*len = ftell(f);
	fseek(f, 0, SEEK_SET);
	char *res = malloc(sizeof(char) * (*len));
	fread(res, 1, *len, f);
	return res;

}