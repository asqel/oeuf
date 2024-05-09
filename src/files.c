#include "../oeuf.h"

#include <stdlib.h>
#include <stdio.h>

char *oe_read_file_bin(const char *path, size_t *lp) {
    size_t len;

    FILE *f = fopen(path, "r");

    if (!f)
        return NULL;

    // get file length
    fseek(f, 0, SEEK_END);
    len = ftell(f);
    fseek(f, 0, SEEK_SET);

    if (lp)
        *lp = len;

    char *res = malloc(len * sizeof(char));

    if (!res || fread(res, 1, len, f) != len) {
        free(res);
        return NULL;
    }

    return res;
}

char *oe_read_file(const char *path) {
    size_t len;
    char *data = oe_read_file_bin(path, &len);

    if (!data) {
        return NULL;
    }

    // remove trailing null bytes
    for (size_t i = 0; i < len; i++) {
        if (data[i]) 
            continue;
        return realloc(data, i + 1);
    }

    data = realloc(data, len + 1);
    data[len] = 0;

    return data;
}
