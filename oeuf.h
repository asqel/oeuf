#ifndef OEUF_H
#define OEUF_H

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

char	*oe_read_file(const char *path);
char	*oe_read_file_bin(const char *path, size_t *len);

// return th number of digits without the sign
int		oe_i64_len(int64_t num);
int		oe_u64_len(uint64_t num);

int		oe_write_file(const char *path, const char *content, int64_t len);

char	*oe_str_replace(const char *str, const char *from, const char *to);

char	*oe_fstring(const char *fmt, ...);


// if len == NULL: len is guessed with NULL terminated array
char	**oe_strarr_append(char **arr, char *str, int *len, int free_error);
int		oe_strarr_len(char **arr);
void	oe_strarr_free(char **arr, int len);

#endif
