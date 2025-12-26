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


// if len == NULL or < 0: len is guessed with NULL terminated array
char	**oe_strarr_append(char **arr, char *str, int *len, int free_error);
int		oe_strarr_len(char **arr);
void	oe_strarr_free(char **arr, int len);
char 	**oe_strarr_dup(char **arr, int len);

typedef struct oe_node_t {
	char *key;
	void *data;
	struct oe_node_t *next;
} oe_node_t;

typedef struct oe_hashmap_t {
	uint32_t len;
	oe_node_t **nodes;
} oe_hashmap_t;

uint32_t oe_hash_str(char *str);
int oe_hashmap_init(oe_hashmap_t *map, uint32_t len);
int oe_hashmap_set(oe_hashmap_t *map, char *key, void *value);
int oe_hashmap_set2(oe_hashmap_t *map, char *key, uint32_t hash, void *value);
void *oe_hashmap_get(oe_hashmap_t *map, char *key);
void **oe_hashmap_get2(oe_hashmap_t *map, char *key, uint32_t hash);

void oe_hashmap_free(oe_hashmap_t *map, void (*free_func)(char *, void *));

size_t oe_hashmap_len(oe_hashmap_t *map);
char **oe_hashmap_get_keys(oe_hashmap_t *map); // free only return value not content (adds NULL at end)
void **oe_hashmap_get_values(oe_hashmap_t *map); // free only return value not content (adds NULL at end)

void oe_hashmap_remove(oe_hashmap_t *map, char *key, void (*free_func)(char *, void *));
void oe_hashmap_remove2(oe_hashmap_t *map, char *key, uint32_t hash, void (*free_func)(char *, void *));

void oe_hashmap_free_keys(char **keys);
#define oe_hashmap_free_value free

#define OE_BIGINT_ALLOC_INC 8

typedef struct {
	size_t alloc_len; // % OE_BIGINT_ALLOC_INC && > 0
	size_t len;
	uint64_t *parts;
} oe_bigint_t; // unsigned

int oe_bigint_init(oe_bigint_t *self);
void oe_bigint_free(oe_bigint_t *self);

// returns 1 for alloc errors
int oe_bigint_add(oe_bigint_t *self, oe_bigint_t *other);
int oe_bigint_add2(oe_bigint_t *self, uint32_t other); // cannot fail technicaly
int oe_bigint_add3(oe_bigint_t *res, oe_bigint_t *a, oe_bigint_t *b);

#endif
