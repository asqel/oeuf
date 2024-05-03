#include <stdint.h>
#include <stdio.h>
#include <limits.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include "../oeuf.h"


#ifndef elif
#define elif else if
#endif


/*


printf format

%M use array for print
    int *r = malloc(sizeof(int) * 2);
    r[0] = 1;
    r[1] = 2;
    printf("%M%d %d", 2, r);
    %M linked to the 2 (length of the array)
    and will take the content of   r as argument for print
*/


//%[flags][width][.precision][length]specifier
/*
flags: '-', '+', ' ', '#', '0'
width: number, *
.precision: .number, .*
length: h, l, L

*/

oe_format_t *custom_formats = NULL;
int custom_formats_len = 0;

void oe_register_format(oe_format_t fm) {
    custom_formats_len++;
    custom_formats = realloc(custom_formats, sizeof(oe_format_t) * (custom_formats_len));
    custom_formats[custom_formats_len - 1] = fm;
}

int does_is_char_good_format(char c) {
    if (c <= 'Z' && 'A' <= c)
        return 1;
    if (c <= 'z' && 'a' <= c)
        return 1;
    return 0;
}

static int do_format(FILE *fd, char *format, va_list *args, oe_format_arg fm_arg, int *fm_len) {
    int end = 0;
    while (does_is_char_good_format(format[end])) end++;

    char *to_search = malloc(sizeof(char) * (end + 1));
    memcpy(to_search, format, end);
    to_search[end] = '\0';

    *fm_len = end;
    //find blt format
    for(int i = 0; i < blt_formats_len; i++) {
        if (!strcmp(to_search, blt_formats[i].specifier))
            return blt_formats[i].func(fd, args, fm_arg);
    }

    for(int i = 0; i < custom_formats_len; i++) {
        if (!strcmp(to_search, custom_formats[i].specifier))
            return custom_formats[i].func(fd, args, fm_arg);
    }
    return 0;
}

static char* parse_flag_fm(char **format) {
    char *flag = NULL;
    int flag_len = 0;
    while (1) {
        char current = **format;
        if (current != '-' && current != '+' && current != ' ' && current != '0' && current != '\'' && current != '#' )
            break;
        else {
            flag_len++;
            flag = realloc(flag, sizeof(char) * (flag_len + 1));
            flag[flag_len - 1] = current;
            flag[flag_len] = '\0';
            (*format)++;
        }
    }
    return flag;
}

int oe_fprintf(FILE *fd, char *format, ...) {
    int res = 0;
    va_list args;
    va_start(args, format);
    while (*format) {
        if (*format != '%') {
            fputc(*format, fd);
            res++;
            format++;
        }
        else if (*(format + 1) == '%') {
            fputc('%', fd);
            format++;
            format++;
            res++;
        }
        else {
            //parse format
            char *flag = NULL;
            char *width = NULL;
            char *precision = NULL;
            char *length = NULL;

            format++;
            flag = parse_flag_fm(&format);

            if (*format == '*') {
                width = strdup("*");
                format++;
            }
            elif (*format <= 9 && 0 <= *format) {
                char *start = format;
                while(*format <= 9 && *format >= 0) 
                    format++;
                width = malloc(sizeof(char) * (format - start + 1));
                int p = 0;
                while(start != format)
                    width[p++] = *(start++);
                width[p] = '\0';
            }
            if (*format == '.') {
                format++;
                if (*format == '*') {
                    precision = strdup("*");
                    format++;
                }
                elif (*format <= 9 && 0 <= *format) {
                    char *start = format;
                    while(*format <= 9 && *format >= 0) 
                        format++;
                    precision = malloc(sizeof(char) * (format - start + 1));
                    int p = 0;
                    while(start != format)
                        precision[p++] = *(start++);
                    precision[p] = '\0';
                }
            }

            if (*format == 'h') {
                length = "h";
                format++;
            }
            elif (*format == 'l') {
                length = "l";
                format++;
            }
            elif (*format == 'L') {
                length = "L";
                format++;
            }
            int fm_len;
            res += do_format(
                fd,
                format,
                &args,
                (oe_format_arg) {
                    .current_count = res,
                    .flag= flag,
                    .length = length,
                    .precision = precision,
                    .width = width
                },
                &fm_len
            );
            format = &(format[fm_len]);
            free(width);
            free(flag);
            free(precision);
        }
    }
    return res;
}
