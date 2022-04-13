#ifndef __SLL__INTERNAL_MEMORY_H__
#define __SLL__INTERNAL_MEMORY_H__ 1
#include <sll/_size_types.h>
#include <sll/types.h>



#define MEMORY_POOL_SIZE 256



typedef struct __USER_MEM_BLOCK{
	__SLL_U64 dt;
} user_mem_block_t;



typedef struct __EMPTY_POOL_POINTER{
	struct __EMPTY_POOL_POINTER* next;
} empty_pool_pointer_t;



typedef struct __POOL_DATA{
	sll_size_t cnt;
	empty_pool_pointer_t* ptr;
} pool_data_t;



void _memory_deinit(void);



void _memory_init(void);



#endif
