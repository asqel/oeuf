#include "oeuf.h"

#define HEAP_SIZE 0x10000

u8 heap_info[HEAP_SIZE / 4] = {0}; //1 == ocupied ; 2 == start of block (2bit each)
u8 heap[HEAP_SIZE];

#define is_occupied_or_start(x) (heap_info[(size_t)((x) - heap) / 4] & ((u8)0b11 << (2 * ((size_t)((x) - heap) % 4))))
#define is_occupied(x) (heap_info[(size_t)((x) - heap) / 4] & ((u8)0b01 << (2 * ((size_t)((x) - heap) % 4))))
#define is_start(x) (heap_info[(size_t)((x) - heap) / 4] & ((u8)0b10 << (2 * ((size_t)((x) - heap) % 4))))
#define set_start(x) (heap_info[(size_t)((x) - heap) / 4] |= ((u8)0b10 << (2 * ((size_t)((x) - heap) % 4))))
#define set_occupied(x) (heap_info[(size_t)((x) - heap) / 4] |= ((u8)0b01 << (2 * ((size_t)((x) - heap) % 4))))
#define clear_info(x) (heap_info[(size_t)((x) - heap) / 4] &= ~((u8)0b11 << (2 * ((size_t)((x) - heap) % 4))))

void *oe_malloc(size_t size) {
	if (size == 0)
		return NULL;
	
	for (size_t i = 0; i < HEAP_SIZE - size + 1; i++) {
		u8 is_here = 1;
		for (size_t k = 0; k < size; k++) {
			if (is_occupied_or_start(&heap[i + k])) {
				is_here = 0;
				break;
			}
		}
		if (is_here) {
			set_start(&heap[i]);
			for (size_t k = 1; k < size; k++) {
				set_start(&heap[i + k]);
			}
			return &heap[i];
		}
	}
	oe_errno = oe_err_no_mem;
	return NULL;
}

void oe_free(void *ptr) {
	u8 *ptr2 = (u8 *)ptr;
	if ((ptr2 < heap || ptr2 >= heap + HEAP_SIZE) || !is_start(ptr2)) {
		oe_errno = oe_err_free_invalid_pointer;
		return ;
	}
	int i = 0;
	while (i == 0 || is_occupied(ptr2 + i)) {
		clear_info(ptr2 + i);
		i++;
	}
}


