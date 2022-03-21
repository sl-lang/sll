#ifndef __SLL__INTERNAL_MEMORY_H__
#define __SLL__INTERNAL_MEMORY_H__ 1
#include <sll/_size_types.h>
#include <sll/types.h>



#define ALLOCATOR_ALLOC_SIZE 65536
#define ALLOCATOR_STACK_ALLOC_SIZE 262144

#define ALLOCATOR_MAX_SMALL_SIZE 512



typedef __SLL_U8 bucket_index_t;



typedef struct __PAGE_HEADER{
	struct __PAGE_HEADER* n;
} page_header_t;



typedef struct __USER_MEM_BLOCK{
	__SLL_U64 dt;
} user_mem_block_t;



typedef struct __MEM_BLOCK{
	struct __MEM_BLOCK* n;
} mem_block_t;



typedef struct __MEM_STACK_BLOCK{
	__SLL_U64 dt;
} mem_stack_block_t;



void _memory_release_data(void);




#endif
