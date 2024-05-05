#include "../oeuf.h"
#include <stdarg.h>
#include <limits.h>



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
    if (n == -9223372036854775807LL - 1){
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

static int put_format_d(FILE *fd, va_list *args, oe_format_arg) {
    //char left_just = fm_arg.flag.minus;
	return putnbr_i32(va_arg(*args, i32), fd);
}

static int put_format_n(FILE *, va_list *args, oe_format_arg fm_arg) {
    int *ptr = va_arg(*args, int *);
    *ptr = fm_arg.current_count;
    return 0;
}


oe_format_t blt_formats[] = {
    (oe_format_t){.func = &put_format_d, .specifier = "d"},
    (oe_format_t){.func = &put_format_n, .specifier = "n"},
};

int blt_formats_len = 2;