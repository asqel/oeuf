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
