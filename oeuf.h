#ifndef OEUF_H
#define OEUF_H
/*
if OEUF64 is defined 64bits functionalities will be enabled
*/

#if defined(_WIN32)
    #define OE_OS_WIN
    #define OE_OS "windows"
#elif defined(__linux__)
    #define OE_OS_LINUX
    #define OE_OS "linux"
#elif defined(__APPLE__)
    #define OE_OS_MACOS
    #define OE_OS "macOS"
#elif defined(__profanOS__)
    #define OE_OS_PROFAN
    #define OE_OS "profanOS"
#else
    #define OE_OS_UNKNOWN
    #define OE_OS "unknown"
#endif

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#undef i8
#undef i16
#undef i32
#undef i64
#undef u8
#undef u16
#undef u32
#undef u64
#undef u0

#undef I8
#undef I16
#undef I32
#undef I64
#undef U8
#undef U16
#undef U32
#undef U64
#undef U0

#define i8 		int8_t
#define i16 	int16_t
#define i32 	int32_t
#define i64 	int64_t

#define u8 		uint8_t
#define u16 	uint16_t
#define u32 	uint32_t
#define u64 	uint64_t
#define u0 		void

#define I8 		int8_t
#define I16 	int16_t
#define I32 	int32_t
#define I64 	int64_t

#define U8 		uint8_t
#define U16 	uint16_t
#define U32 	uint32_t
#define U64 	uint64_t
#define U0 		void

#ifdef OEUF64
    typedef i64 oe_arch_int;
    typedef u64 oe_arch_uint;
#else
    typedef i32 oe_arch_int;
    typedef u32 oe_arch_uint;
#endif

typedef unsigned int uint;

typedef struct {
    char minus;
    char plus;
    char space;
    char hash;
    char zero;
} oe_format_flag;

extern oe_format_flag null_flag;

#define FLAG_CMP(X, Y)  (X.minus == Y.minus && X.plus == Y.plus && X.space == Y.space&& X.hash == Y.hash && X.zero == Y.zero &&)

typedef struct {
    oe_format_flag flag;
    i32 width; // if -1 then it's *
    i32 precision; // same
    char *length;
    int current_count;
} oe_format_arg;


typedef struct {
    int (*func)(FILE *, va_list *, oe_format_arg);
    char *specifier;
} oe_format_t;

extern i16 oe_errno;

// read a file at `path` and returns its content as null terminated string
char *oe_read_file(const char *path);

// read a file at `path` and returns its content and put its length  in *len
char *oe_read_file_bin(const char *path, size_t *len);


// return a new string which is the concatenation of `s1` and `s2` as null terminated string
char *oe_str_cat_new(const char *s1, const char *s2);

// return a new string which is the concatenation of `s1` `s2` and `s3` as null terminated string
char *oe_str_cat_new3(const char *s1, const char *s2, const char *s3);

// return a new string which is the concatenation of `s1` `s2` `s3`and `s4` as null terminated string
char *oe_str_cat_new4(const char *s1, const char *s2, const char *s3, const char *s4);

/*
    apply the selection sort on an array `arr` of integers of length `len`
*/
void oe_selection_sort_i(int *arr, int len);


/*
apply selection sort on arr using comp
comp is a function that takes 2 void * a and b and return a < b
*/
void oe_selection_sort_c(void *arr, int len, size_t elem_size, int (*comp)(void *, void *));



//return the length of a number `n` in base 10
// if the number return len(-n) + 1
// -2**31 will be correctly handled and will not overflow
int oe_number_len(uint32_t n);


//return the length of a number `n` in base `b`
// if the number is negative the length of its opposite will be returned + 1
// -2**31 will be correctly handled and will not overflow
int oe_number_len_b(uint32_t n, uint32_t b);


/*	write to a file at `path` the content of `content` of length `len`
    if an error occured during writting or opening the file 1 will be returned

*/
int oe_write_file(char *path, char *content, int len);


/*	return a copy of `str` where sequence of `from` are replaced by `to`
    ex:
        replace("bonjour salut", "salut", "hi")
    if `from` or `to` is NULL a copy of `str` is returned
    if str is NULL, NULL is retured
*/
char *oe_str_replace(char *str, char *from, char *to);

extern oe_format_t blt_formats[];

extern int blt_formats_len;

extern oe_format_t *custom_formats;
extern int custom_formats_len;

int oe_fprintf(FILE *fd, char *format, ...);

enum oe_errno_values {
    oe_err_none,
    oe_err_no_mem,
    oe_err_free_invalid_pointer,
    oe_err_realloc_invalid_pointer,
    oe_err_hash_map_key_not_found

};

void *oe_malloc(size_t size);
void oe_free(void *ptr);


// allocate a new string using fmt and the args using vsnprintf
char *oe_fstring(const char *fmt, ...);


#endif
