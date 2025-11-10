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
	map->len = 0;
	map->nodes = malloc(sizeof(oe_node_t *) * len);
	if (!map->nodes)
		return 1;
	map->len = len;
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
		}
		node->data = value;
		return 0;
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
		if (!strcmp(node->key, key))
			return &node->data;
		node = node->next;
	}
	return NULL;
}

void oe_hashmap_free(oe_hashmap_t *map, void (*free_func)(char *, void *)) {
	for (size_t i = 0; i < map->len; i++) {
		oe_node_t *node = map->nodes[i];
		while (node) {
			if (free_func)
				free_func(node->key, node->data);
			node = node->next;
		}

		node = map->nodes[i];
		while (node) {
			oe_node_t *next = node->next;
			free(node);
			node = next;
		}
	}
	free(map->nodes);
	map->nodes = NULL;
	map->len = 0;
}

size_t oe_hashmap_len(oe_hashmap_t *map) {
	size_t len = 0;
	for (size_t i = 0; i < map->len; i++) {
		oe_node_t *node = map->nodes[i];
		while (node) {
			len++;
			node = node->next;
		}
	}

	return len;
}
char **oe_hashmap_get_keys(oe_hashmap_t *map) {
	char **res = malloc(sizeof(char *) * (1 + oe_hashmap_len(map)));
	if (!res)
		return NULL;

	int k = 0;
	for (size_t i = 0; i < map->len; i++) {
		oe_node_t *node = map->nodes[i];
		while (node) {
			res[k++] = strdup(node->key);
			node = node->next;
		}
	}
	res[k] = NULL;
	return res;
}

void **oe_hashmap_get_values(oe_hashmap_t *map) {
	void **res = malloc(sizeof(void *) * (1 + oe_hashmap_len(map)));
	if (!res)
		return NULL;

	int k = 0;
	for (size_t i = 0; i < map->len; i++) {
		oe_node_t *node = map->nodes[i];
		while (node) {
			res[k++] = node->data;
			node = node->next;
		}
	}
	res[k] = NULL;
	return res;
}

void oe_hashmap_remove(oe_hashmap_t *map, char *key, void (*free_func)(char *, void *)) {
	oe_hashmap_remove2(map, key, oe_hash_str(key), free_func);
}

void oe_hashmap_remove2(oe_hashmap_t *map, char *key, uint32_t hash, void (*free_func)(char *, void *)) {
	hash %= map->len;
	
	oe_node_t *node = map->nodes[hash];
	if (!strcmp(node->key, key)) {
		oe_node_t *next = node->next;
		if (free_func)
			free_func(key, node->data);
		free(node->key);
		free(node);
		map->nodes[hash] = next;
		return ;
	}
	while (node->next) {
		if (strcmp(node->next->key, key)) {
			node = node->next;
			continue;
		}
		if (free_func)
			free_func(key, node->next->data);
		oe_node_t *next = node->next->next;
		free(node->next->key);
		free(node->next);
		node->next = next;
		break;
	}
}

void oe_hashmap_free_keys(char **keys) {
	if (!keys)
		return ;
	for (int i = 0; keys[i]; i++)
		free(keys[i]);
	free(keys);
}
