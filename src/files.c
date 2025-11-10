#include "oeuf.h"

#include <stdlib.h>
#include <stdio.h>

char *oe_read_file_bin(const char *path, size_t *lp) {
	size_t len;

	FILE *f = fopen(path, "rb");

	if (!f)
		return NULL;

	fseek(f, 0, SEEK_END);
	len = ftell(f);
	fseek(f, 0, SEEK_SET);

	if (lp)
		*lp = len;

	char *res = malloc(len * sizeof(char));

	if (!res || fread(res, 1, len, f) != len) {
		free(res);
		fclose(f);
		return NULL;
	}
	
	fclose(f);
	return res;
}

char *oe_read_file(const char *path) {
	size_t len;
	char *data = oe_read_file_bin(path, &len);

	if (!data)
		return NULL;

	size_t i = 0;
	while (i < len && data[i])
		i++;

	data = realloc(data, sizeof(char) * (i + 1));
	data[len] = 0;
	return data;
}
