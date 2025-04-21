#include "oeuf.h"

#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

char *oe_fstring(const char *fmt, ...) {
    va_list args;
    va_list args_copy;
    va_start(args, fmt);
    va_copy(args_copy, args);
    int len = vsnprintf(NULL, 0, fmt, args);
    va_end(args);
    char *res = malloc(len + 1);
    vsnprintf(res, len + 1, fmt, args_copy);
    va_end(args_copy);
    return res;
}

int oe_strarr_len(char **arr) {
	int len = 0;

	if (arr == NULL)
		return 0;
	while (arr[len] != NULL)
		len++;
	return len;
}

char **oe_strarr_append(char **arr, char *str, int *len) {
	char **res = NULL;
	int real_len = 0;

	if (len == NULL)
		real_len = oe_strarr_len(arr);
	else
		real_len =*len;
	

	res = malloc(sizeof(char *) * (real_len + 2));
	res[real_len + 1] = NULL;
	res[real_len] = strdup(str);
	for (int i = 0; i < real_len; i++)
		res[i] = arr[i];
	free(arr);

	if (len != NULL)
		*len = real_len;
	return res;
}
