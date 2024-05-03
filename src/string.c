#include "../oeuf.h"

#include <stdlib.h>
#include <string.h>

char *uti_str_cat_new(const char *s1, const char *s2) {
    int len1 = strlen(s1);
    int len2 = strlen(s2);

    char *res = malloc(len1 + len2 + 1);
    if (!res)
        return NULL;

    memcpy(res, s1, len1);
    memcpy(res + len1, s2, len2);
    res[len1 + len2] = 0;

    return res;
}

char *uti_str_cat_new3(const char *s1, const char *s2, const char *s3) {
    int len1 = strlen(s1);
    int len2 = strlen(s2);
    int len3 = strlen(s3);

    char *res = malloc(len1 + len2 + len3 + 1);
    if (!res)
        return NULL;

    memcpy(res, s1, len1);
    memcpy(res + len1, s2, len2);
    memcpy(res + len1 + len2, s3, len3);
    res[len1 + len2 + len3] = 0;

    return res;

}

char *uti_str_cat_new4(const char *s1, const char *s2, const char *s3, const char *s4) {
    int len1 = strlen(s1);
    int len2 = strlen(s2);
    int len3 = strlen(s3);
    int len4 = strlen(s4);

    char *res = malloc(len1 + len2 + len3 + len4 + 1);
    if (!res)
        return NULL;

    memcpy(res, s1, len1);
    memcpy(res + len1, s2, len2);
    memcpy(res + len1 + len2, s3, len3);
    memcpy(res + len1 + len2 + len3, s4, len4);
    res[len1 + len2 + len3 + len4] = 0;

    return res;
}
