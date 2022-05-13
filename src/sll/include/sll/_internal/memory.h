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

#define USER_MEM_BLOCK_GET_SIZE(b) ((b)->dt&0xfffffffffffffff8ull)
#define USER_MEM_BLOCK_INIT(sz) (USER_MEM_BLOCK_FLAG_USED|(sz))

#define STACK_BLOCK_GET_END(b) ((stack_block_end_t*)PTR(ADDR(b)+STACK_BLOCK_GET_SIZE(b)))
#define STACK_BLOCK_GET_NEXT(b) ((stack_block_header_t*)PTR(ADDR(b)+STACK_BLOCK_GET_SIZE(b)+sizeof(stack_block_end_t)))
#define STACK_BLOCK_GET_SIZE(b) ((b)->dt&0xfffffffffffffff8ull)
#define STACK_BLOCK_INIT(sz) (USER_MEM_BLOCK_FLAG_STACK|(sz))
#define STACK_BLOCK_END_OF_STACK(b) (b->dt==STACK_BLOCK_END_OF_STACK_MARKER)
#define STACK_BLOCK_END_OF_STACK_MARKER 0xffffffffffffffff
#define STACK_BLOCK_NODE_FROM_USER_BLOCK(b,node) \
	do{ \
		*(node)=(b); \
		(b)->dt&=~USER_MEM_BLOCK_FLAG_USED; \
	} while (0)



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



typedef user_mem_block_t stack_block_header_t;



typedef struct _STACK_BLOCK_END{
	stack_block_header_t* head;
} stack_block_end_t;



void _memory_deinit(void);



void _memory_init(void);



#endif
