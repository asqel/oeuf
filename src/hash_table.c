#include <stdlib.h>
#include <string.h>
#include "../oeuf.h"


typedef struct oe_node_t{
	char *key; // if key is NULL node is not set / empty
	void *data;
	struct oe_node_t *next; //last node is NULL
}oe_node_t;


typedef struct {
	oe_node_t *values;
	int len;
}oe_hash_map_t;

i32 oe_hash(char *str) {
	i32 res = 0;
	while (*str)
		res += ((*str) << 5) + *(str++);
	return res;
}

oe_hash_map_t oe_new_hash_map(int len) {
	return (oe_hash_map_t){
		.len = len,
		.values = calloc(len, sizeof(oe_node_t))
	};
}

void *oe_hash_map_get(oe_hash_map_t map, char *key, i32 hash) {
	oe_errno = 0;
	oe_node_t *current_node = &(map.values[hash % map.len]);
	while (current_node->next != NULL) {
		if (!strcmp(current_node->key, key))
			return current_node->data;
		current_node = current_node->next;
	}
	oe_errno = 1;
	return NULL;
}

oe_node_t *oe_hash_map_get_node(oe_hash_map_t map, char *key, i32 hash) {
	oe_errno = 0;
	oe_node_t *current_node = &(map.values[hash % map.len]);
	while (current_node->next != NULL) {
		if (!strcmp(current_node->key, key))
			return current_node;
		current_node = current_node->next;
	}
	oe_errno = 1;
	return NULL;
}

void *oe_hash_map_set(oe_hash_map_t map, char *key, i32 hash, void *data) {
	oe_errno = 0;
	oe_node_t *current_node = &(map.values[hash % map.len]);
	while (current_node->next != NULL) {
		if (!strcmp(current_node->key, key)) {
			void *old_data = current_node->data;
			current_node->data = data;
			return old_data;
		}
		current_node = current_node->next;
	}
	oe_errno = 1;
	return NULL;
}
