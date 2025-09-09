#include "oeuf.h"

int oe_strarr_len(char **arr) {
	if (!arr)
		return 0;

	int len = 0;
	while (arr[len])
		len++;
	return len;
}

char **oe_strarr_append(char **arr, char *str, int *len, int free_error) {
	int real_len = 0;

	if (len == NULL)
		real_len = oe_strarr_len(arr);
	else
		real_len = *len;

	char **res = realloc(arr, sizeof(char *) * (2 + real_len));
	if (res == NULL) {
		if (free_error)
			oe_strarr_free(arr, real_len);
		return NULL;
	}

	res[real_len] = str;
	res[real_len + 1] = NULL;
	return res;
}

void oe_strarr_free(char **arr, int len) {
	if (len < 0)
		len = oe_strarr_len(arr);

	for (int i = 0; i < len; i++)
		free(arr[i]);
	free(arr);
}