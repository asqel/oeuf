#ifndef OEUF_H
#define OEUF_H

/*

if OEUF32 is defined 64bits function will be disabled


*/


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

typedef struct {
	char *flag;
	char *width;
	char *precision;
	char *length;
	int current_count;
}oe_format_arg;


typedef struct {
	int (*func)(FILE *, va_list *, oe_format_arg);
	char *specifier;
}oe_format_t;



extern i16 oe_errno;

// read a file at `path` and returns its content as null terminated string
char *uti_read_file(char *path);


// read a file at `path` and returns its content and put its length  in *len
char *uti_read_file_bin(char *path, int *len);


// return a new string which is the concatenation of `s1` and `s2` as null terminated string 
char *uti_str_cat_new(char *s1, char *s2);

// return a new string which is the concatenation of `s1` `s2` and `s3` as null terminated string 
char *uti_str_cat_new3(char *s1, char *s2);

// return a new string which is the concatenation of `s1` `s2` `s3`and `s4` as null terminated string 
char *uti_str_cat_new4(char *s1, char *s2);


/*
split a string by a separator
strings can be empty but not null
null pointer indicate end
every pointer of result may be free and result may be freed
ex:
	split("abc :! df :!:!uu", ":!")
	-> ["abc", " df ", "", "uu", NULL]
*/
char **uit_str_split(char *str, char* sep);


/*
	apply the selection sort on an array `arr` of integers of length `len`
*/
void uti_selection_sort_i(int *arr, int len);


/*
apply selection sort on arr using comp
comp is a function that takes 2 void * a and b and return a < b
*/
void uti_selection_sort_c(void *arr, int len, size_t elem_size, int (*comp)(void *, void *));



//return the length of a number `n` in base 10
// if the number return len(-n) + 1
// -2**31 will be correctly handled and will not overflow
int uti_number_len(uint32_t n);


//return the length of a number `n` in base `b`
// if the number is negative the length of its opposite will be returned + 1
// -2**31 will be correctly handled and will not overflow
int uti_number_len_b(uint32_t n, uint32_t b);


/*	write to a file at `path` the content of `content` of length `len`
	if an error occured during writting or opening the file 1 will be returned

*/
int uti_write_file(char *path, char *content, int len);


/*	return a copy of `str` where sequence of `from` are replaced by `to`
	ex:
		replace("bonjour salut", "salut", "hi")
	if `from` or `to` is NULL a copy of `str` is returned
	if str is NULL, NULL is retured
*/
char *uti_str_replace(char *str, char *from, char *to);

extern oe_format_t blt_formats[];

extern int blt_formats_len;

extern oe_format_t *custom_formats;
extern int custom_formats_len;

int oe_fprintf(FILE *fd, char *format, ...);

#endif