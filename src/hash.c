#include "oeuf.h"
#include <string.h>

uint32_t oe_hash_str(char *str) {
	uint32_t res = 5381;

	uint32_t c;
	while ((c = *(str++)))
		res = (res << 5) + res + c;
	return res;
}

int oe_hashmap_init(oe_hashmap_t *map, uint32_t len) {
	if (!map)
		return 1;
	if (!len)
		len = 1024;
	map->len = len;
	map->nodes = malloc(sizeof(oe_node_t *) * len);
	if (!map->nodes)
		return 1;
	for (uint32_t i = 0; i < len; i++)
		map->nodes[i] = NULL;
	return 0;
}

int oe_hashmap_set(oe_hashmap_t *map, char *key, void *value) {
	if (!key)
		return 1;
	return oe_hashmap_set2(map, key, oe_hash_str(key), value);
}

int oe_hashmap_set2(oe_hashmap_t *map, char *key, uint32_t hash, void *value) {
	if (!key || !map || !map->nodes)
		return 1;
	hash = hash % map->len;
	if (!map->nodes[hash]) {
		map->nodes[hash] = malloc(sizeof(oe_node_t));
		if (!map->nodes[hash])
			return 1;
		map->nodes[hash]->key = strdup(key);
		if (!map->nodes[hash]->key) {
			free(map->nodes[hash]);
			map->nodes[hash] = NULL;
			return 1;
		}
		map->nodes[hash]->data = value;
		map->nodes[hash]->next = NULL;
		return 0;
	}
	oe_node_t *node = map->nodes[hash];
	while (1) {
		if (strcmp(node->key, key)) {
			if (node->next) {
				node = node->next;
				continue;
			}
			node->next = malloc(sizeof(oe_node_t));
			if (!node->next)
				return 1;
			node->next->key = strdup(key);
			if (!node->next->key) {
				free(node->next);
				node->next = NULL;
				return 1;
			}
			node->next->data = value;
			node->next->next = NULL;
			return 1;
		} // TODO support if fiirst
	}
	return 0;
}

void *oe_hashmap_get(oe_hashmap_t *map, char *key) {
	return *oe_hashmap_get2(map, key, oe_hash_str(key));
}

void **oe_hashmap_get2(oe_hashmap_t *map, char *key, uint32_t hash) {
	hash %= map->len;

	oe_node_t *node = map->nodes[hash];
	while (node) {
		if (!strcmp(node->key))
			return &node->data;
		node = node->next;
	}
	return NULL;
}
