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


#ifndef OEUF32
static int putnbr_u64(u64 n, FILE *fd) {
    if (n == 0) {
        fputc('0', fd);
        return 1;
    }
    if (n < 10) {
        fputc('0' + n, fd);
        return 1;
    }
    int res = putnbr_u64(n / 10, fd) + 1;
    fputc('0' + (n % 10), fd);
    return res + 1;
}
#endif

static int putnbr_u32(u32 n, FILE *fd) {
    #ifndef OEUF32
        return putnbr_u64((u64)n, fd);
    #else
        if (n == 0) {
        fputc('0', fd);
        return 1;
        }
        if (n < 10) {
            fputc('0' + n, fd);
            return 1;
        }
        int res = putnbr_u64(n / 10, fd) + 1;
        fputc('0' + (n % 10), fd);
        return res + 1;
    #endif
}

static int putnbr_u16(u16 n, FILE *fd) {
    return putnbr_u32((u32)n, fd);
}

static int putnbr_u8(u8 n, FILE *fd) {
    return putnbr_u32((u32)n, fd);
}

#ifndef OEUF32
static int putnbr_i64(i64 n, FILE *fd) {
    if (n >= 0) 
        return putnbr_u64((u64) n, fd);
    if (n == LLONG_MIN){
        fwrite("-9223372036854775808", 1, 20, fd);
        return 20;
    }
    else {
        fputc('-', fd);
        int v = putnbr_u64((u64)(-n), fd);
        return v + 1;
    }
}
#endif

static int putnbr_i32(i32 n, FILE *fd) {
    if (n >= 0) 
        return putnbr_u32((u32) n, fd);
    if (n == -2147483648) {
        fwrite("-2147483648", 1, 11, fd);
        return 11;
    }
    else  {
        fputc('-', fd);
        int v = putnbr_u32((u32)-n, fd);
        return v + 1;
    }

}

static int putnbr_i16(i16 n, FILE *fd) {
    if (n >= 0) 
        return putnbr_u16((u16) n, fd);
    if (n == -32768) {
        fwrite("-32768", 1, 6, fd);
        return 6;
    }
    else  {
        fputc('-', fd);
        int v = putnbr_u16((u16)-n, fd);
        return v + 1;
    }
}


static int putnbr_i8(i16 n, FILE *fd) {
    if (n >= 0) 
        return putnbr_u8((u8) n, fd);
    if (n == 128) {
        fwrite("-128", 1, 4, fd);
        return 4;
    }
    else  {
        fputc('-', fd);
        int v = putnbr_u8((u8)-n, fd);
        return v + 1;
    }
}

static int put_format_d(FILE *, va_list *, oe_format_arg fm_arg) {
    printf("char printed :%d\n", fm_arg.current_count);
    printf("flag :%s\n", fm_arg.flag);
    printf("length :%s\n", fm_arg.length);
    printf("precision :%s\n", fm_arg.precision);
    printf("width :%s\n", fm_arg.width);
    return 0;
}
//%[flags][width][.precision][length]specifier
/*
flags: '-', '+', ' ', '#', '0'
width: number, *
.precision: .number, .*
length: h, l, L

*/


oe_format_t blt_formats[] = {
    (oe_format_t){.func = &put_format_d, .specifier = "d"},
    
};

int blt_formats_len = 1;

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