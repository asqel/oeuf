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
	return 0;
}

