#include <stdint.h>
#include <stdio.h>
#include <limits.h>
#include <stdarg.h>
#include "../oeuf.h"


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

typedef struct {
	int (*func)(FILE*, char *, va_list);
	char *format;
	char type;
	/*
	0: nothing juste %format
	1: integer %Iformat ex: %34format
	2: float   %Fformat ex: %2.1format
	*/
}oe_format_t;


oe_format_t blt_formats[] = {
	(oe_format_t){.func = &putnbr_i32},
	
};

int blt_formats_len = sizeof(blt_formats)/sizeof(blt_formats[0]);

oe_format_t *custom_formats = NULL;
int custom_formats_len = 0;

void oe_register_format() {

}

static int do_format(FILE *fd, char *format, va_list args) {
	(void)fd;
	(void)format;
	(void)args;
	return 0;
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
		else
			res += do_format(fd, ++format, args);
	}
	return res;
}