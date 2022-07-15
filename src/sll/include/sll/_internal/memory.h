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

#define MEMORY_STACK_SIZE 2097152

#define USER_MEM_BLOCK_FLAG_USED 1
#define USER_MEM_BLOCK_FLAG_STACK 2

#define USER_MEM_BLOCK_GET_SIZE(b) ((b)->data&0xfffffffffffffff8ull)
#define USER_MEM_BLOCK_INIT(sz) (USER_MEM_BLOCK_FLAG_USED|(sz))

#define STACK_BLOCK_SET_END(b) \
	do{ \
		((stack_block_end_t*)PTR(ADDR(b)+STACK_BLOCK_GET_SIZE(b)))->head=(b); \
	} while (0)
#define STACK_BLOCK_GET_NEXT(b) ((stack_block_header_t*)PTR(ADDR(b)+STACK_BLOCK_GET_SIZE(b)+sizeof(stack_block_end_t)))
#define STACK_BLOCK_GET_PREV(b) ((stack_block_header_t*)(((stack_block_end_t*)PTR(ADDR(b)-sizeof(stack_block_end_t)))->head))
#define STACK_BLOCK_GET_SIZE(b) ((b)->data&0xfffffffffffffff8ull)
#define STACK_BLOCK_INIT(sz) (USER_MEM_BLOCK_FLAG_STACK|(sz))
#define STACK_BLOCK_END_OF_STACK(b) (b->data==STACK_BLOCK_END_OF_STACK_MARKER)
#define STACK_BLOCK_END_OF_STACK_MARKER 0xffffffffffffffff
#define STACK_BLOCK_NODE_FROM_USER_BLOCK(b,node) \
	do{ \
		*(node)=(b); \
		(b)->data&=~USER_MEM_BLOCK_FLAG_USED; \
	} while (0)



typedef __SLL_U32 pool_data_miss_bitmap_t;



typedef __SLL_U32 pool_data_counter_t;



typedef __SLL_U32 memory_update_timer_t;



typedef struct _USER_MEM_BLOCK{
	__SLL_U64 data;
} user_mem_block_t;



typedef struct _POOL_DATA{
	pool_data_counter_t alloc;
	pool_data_counter_t miss;
	pool_data_miss_bitmap_t last_miss;
	pool_data_counter_t cnt;
	void* ptr[MEMORY_POOL_MAX_BLOCKS];
} pool_data_t;



typedef user_mem_block_t stack_block_header_t;



typedef struct _STACK_BLOCK_END{
	stack_block_header_t* head;
} stack_block_end_t;



void _memory_deinit(void);



void _memory_init(void);



#endif
