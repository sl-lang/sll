#ifndef __SLL__INTERNAL_MEMORY_H__
#define __SLL__INTERNAL_MEMORY_H__ 1
#include <sll/_size_types.h>
#include <sll/types.h>



#define MEMORY_POOL_SIZE 256
#define MEMORY_POOL_MAX_BLOCKS 4096
#define MEMORY_POOL_UPDATE_TIMER 4096
#define MEMORY_POOL_MIN_EXTEND_COUNT 3
#define MEMORY_POOL_EXTEND_THRESHOLD 10
#define MEMORY_POOL_EXTEND_FACTOR_COUNT 15
#define MEMORY_POOL_EXTEND_FACTOR_MISS 3
#define MEMORY_POOL_MAX_NEW_SIZE 32768



typedef struct _USER_MEM_BLOCK{
	__SLL_U64 dt;
} user_mem_block_t;



typedef struct _EMPTY_POOL_POINTER{
	struct _EMPTY_POOL_POINTER* next;
} empty_pool_pointer_t;



typedef struct _POOL_DATA{
	__SLL_U32 alloc;
	__SLL_U32 miss;
	__SLL_U32 last_miss;
	__SLL_U32 sz;
	empty_pool_pointer_t* ptr;
} pool_data_t;



void _memory_deinit(void);



void _memory_init(void);



#endif
