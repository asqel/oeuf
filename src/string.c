#include "oeuf.h"

char *oe_fstring(const char *fmt, ...) {
	va_list	args;
	va_list	args_copy;
	int		len;

	va_start(args, fmt);
	va_copy(args_copy, args);
	len = vsnprintf(NULL, 0, fmt, args);

	char *res = malloc(sizeof(char) * (len + 1));
	if (res)
		vsnprintf(res, len + 1, fmt, args_copy);

	va_end(args_copy);
	va_end(args);
	return res;
}
