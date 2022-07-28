#include <sll/_internal/intrinsics.h>
#include <sll/_internal/memory.h>
#include <sll/_internal/platform.h>
#include <sll/common.h>
#include <sll/data.h>
#include <sll/file.h>
#include <sll/generated/memory_fail.h>
#include <sll/memory.h>
#include <sll/platform/file.h>
#include <sll/platform/memory.h>
#include <sll/string.h>
#include <sll/types.h>
#include <stdlib.h>



#define POINTER_TO_STRING(p) \
	do{ \
		addr_t ptr=(addr_t)(p); \
		sll_string_length_t j=64; \
		do{ \
			j-=4; \
			sll_char_t c=(ptr>>j)&15; \
			buffer[i]=c+(c>9?87:48); \
			i++; \
		} while (j); \
	} while (0)



static pool_data_t _memory_pool[MEMORY_POOL_SIZE];
static memory_update_timer_t _memory_update;
static stack_block_header_t* _memory_stack;
static stack_block_header_t* _memory_stack_top;



static __SLL_NO_RETURN void _raise_error(sll_char_t type,void* pointer,sll_size_t size){
	sll_file_flush(sll_stdout);
	sll_file_flush(sll_stderr);
	const sll_char_t* nm;
	switch (type){
		case 0:
			nm=SLL_CHAR("sll_allocate");
			break;
		case 1:
			nm=SLL_CHAR("sll_allocate_stack");
			break;
		default:
			nm=SLL_CHAR("sll_reallocate");
			break;
	}
	sll_char_t buffer[64];
	sll_string_length_t i=sll_string_length(nm);
	sll_copy_data(nm,i,buffer);
	buffer[i]='(';
	i++;
	if (type==2){
		POINTER_TO_STRING(pointer);
		buffer[i]=',';
		i++;
	}
	POINTER_TO_STRING(size);
	buffer[i]=')';
	buffer[i+1]=0;
	sll_file_descriptor_t fd=sll_platform_get_default_stream_descriptor(SLL_PLATFORM_STREAM_ERROR);
	sll_platform_file_write(fd,MEMORY_FAIL_START,MEMORY_FAIL_START_SIZE,NULL);
	sll_platform_file_write(fd,buffer,i+2,NULL);
	sll_platform_file_write(fd,MEMORY_FAIL_END,MEMORY_FAIL_END_SIZE,NULL);
	_force_exit_platform();
}



static void _update_pool_extra(void){
	_memory_update=MEMORY_POOL_UPDATE_TIMER;
	for (sll_arg_count_t i=0;i<MEMORY_POOL_SIZE;i++){
		sll_bool_t expand=(_memory_pool[i].miss>=MEMORY_POOL_EXTEND_THRESHOLD);
		_memory_pool[i].last_miss=(_memory_pool[i].last_miss<<1)|expand;
		if (expand&&POPULATION_COUNT(_memory_pool[i].last_miss)>=MEMORY_POOL_MIN_EXTEND_COUNT){
			sll_size_t new=(POPULATION_COUNT(_memory_pool[i].last_miss)-1)*MEMORY_POOL_EXTEND_FACTOR_COUNT+_memory_pool[i].miss*MEMORY_POOL_EXTEND_FACTOR_MISS;
			sll_size_t b_sz=(i+1)<<4;
			if (new*b_sz>=MEMORY_POOL_MAX_NEW_SIZE){
				new=MEMORY_POOL_MAX_NEW_SIZE/b_sz;
			}
			if (new>MEMORY_POOL_MAX_BLOCKS-_memory_pool[i].cnt){
				new=MEMORY_POOL_MAX_BLOCKS-_memory_pool[i].cnt;
			}
			while (new){
				new--;
				_memory_pool[i].ptr[_memory_pool[i].cnt]=malloc(b_sz);
				_memory_pool[i].cnt++;
			}
		}
		_memory_pool[i].alloc=0;
		_memory_pool[i].miss=0;
	}
}



static __SLL_FORCE_INLINE void _fill_zero(void* pointer,sll_size_t size){
	wide_data_t* p=pointer;
	size=(size+7)>>3;
#ifndef __SLL_BUILD_DARWIN
	__m256i zero=_mm256_setzero_si256();
	while (size>3){
		_mm256_storeu_si256((__m256i*)p,zero);
		size-=4;
		p+=4;
	}
#endif
	while (size){
		*p=0;
		size--;
		p++;
	}
}



static __SLL_FORCE_INLINE void _pool_add(user_mem_block_t* block){
	SLL_ASSERT(block->data&USER_MEM_BLOCK_FLAG_USED);
	SLL_ASSERT(!(block->data&USER_MEM_BLOCK_FLAG_STACK));
	SLL_ASSERT(!(USER_MEM_BLOCK_GET_SIZE(block)&15));
	sll_size_t sz=(USER_MEM_BLOCK_GET_SIZE(block)>>4)-1;
	if (sz>=MEMORY_POOL_SIZE||_memory_pool[sz].cnt==MEMORY_POOL_MAX_BLOCKS){
		free(block);
		return;
	}
	_memory_pool[sz].ptr[_memory_pool[sz].cnt]=block;
	_memory_pool[sz].cnt++;
}



static __SLL_FORCE_INLINE void* _pool_get(sll_size_t size){
	if (size>=MEMORY_POOL_SIZE){
		return NULL;
	}
	_memory_update--;
	if (!_memory_update){
		_update_pool_extra();
	}
	_memory_pool[size].alloc++;
	if (!_memory_pool[size].cnt){
		_memory_pool[size].miss++;
		return NULL;
	}
	_memory_pool[size].cnt--;
	return _memory_pool[size].ptr[_memory_pool[size].cnt];
}



static __SLL_FORCE_INLINE void _init_stack_node(stack_block_header_t* node,sll_size_t size){
	SLL_ASSERT(size&&!(size&7));
	node->data=STACK_BLOCK_INIT(size);
	STACK_BLOCK_SET_END(node);
}



static __SLL_FORCE_INLINE void _stack_add(user_mem_block_t* block){
	SLL_ASSERT(block->data&USER_MEM_BLOCK_FLAG_USED);
	SLL_ASSERT(block->data&USER_MEM_BLOCK_FLAG_STACK);
	stack_block_header_t* node;
	STACK_BLOCK_NODE_FROM_USER_BLOCK(block,&node);
	stack_block_header_t* next=STACK_BLOCK_GET_NEXT(node);
	sll_size_t sz=STACK_BLOCK_GET_SIZE(node);
	if (ADDR(block)!=ADDR(_memory_stack)){
		stack_block_header_t* prev=STACK_BLOCK_GET_PREV(node);
		if (!(prev->data&USER_MEM_BLOCK_FLAG_USED)){
			sz+=STACK_BLOCK_GET_SIZE(prev)+sizeof(stack_block_end_t);
			node=prev;
		}
	}
	if (STACK_BLOCK_END_OF_STACK(next)){
		_memory_stack_top=node;
	}
	else if (!(next->data&USER_MEM_BLOCK_FLAG_USED)){
		sz+=STACK_BLOCK_GET_SIZE(next)+sizeof(stack_block_end_t);
	}
	node->data=STACK_BLOCK_INIT(sz);
	STACK_BLOCK_SET_END(node);
	if (STACK_BLOCK_END_OF_STACK(STACK_BLOCK_GET_NEXT(next))){
		_memory_stack_top=node;
	}
}



static __SLL_FORCE_INLINE void* _stack_get(sll_size_t size){
	size=(size+7)&0xfffffffffffffff8ull;
	if (!_memory_stack_top||STACK_BLOCK_GET_SIZE(_memory_stack_top)<size){
		return NULL;
	}
	stack_block_header_t* out=_memory_stack_top;
	sll_size_t blk_sz=STACK_BLOCK_GET_SIZE(out);
	if (blk_sz==size){
		_memory_stack_top=NULL;
	}
	else{
		out->data=STACK_BLOCK_INIT(size);
		STACK_BLOCK_SET_END(out);
		_memory_stack_top=STACK_BLOCK_GET_NEXT(out);
		_init_stack_node(_memory_stack_top,blk_sz-size-sizeof(stack_block_end_t));
	}
	out->data=USER_MEM_BLOCK_INIT(size)|USER_MEM_BLOCK_FLAG_STACK;
	return out;
}



static __SLL_FORCE_INLINE void* _allocate_chunk(sll_size_t size,sll_bool_t fail_on_error){
	size=(size+15)>>4;
	user_mem_block_t* b=_pool_get(size-1);
	size<<=4;
	if (!b){
		b=malloc(size);
		if (!b){
			if (fail_on_error){
				_raise_error(0,NULL,size);
			}
			return NULL;
		}
	}
	b->data=USER_MEM_BLOCK_INIT(size);
	return b;
}



void _memory_deinit(void){
	for (sll_size_t i=0;i<MEMORY_POOL_SIZE;i++){
		for (pool_data_counter_t j=0;j<_memory_pool[i].cnt;j++){
			free(_memory_pool[i].ptr[j]);
		}
	}
	SLL_CRITICAL_ERROR(sll_platform_free_page(_memory_stack,SLL_ROUND_LARGE_PAGE(MEMORY_STACK_SIZE)));
}



void _memory_init(void){
	_memory_update=MEMORY_POOL_UPDATE_TIMER;
	for (sll_size_t i=0;i<MEMORY_POOL_SIZE;i++){
		_memory_pool[i].alloc=0;
		_memory_pool[i].miss=0;
		_memory_pool[i].last_miss=0;
		_memory_pool[i].cnt=0;
	}
	_memory_stack=sll_platform_allocate_page(SLL_ROUND_LARGE_PAGE(MEMORY_STACK_SIZE),1,NULL);
	_memory_stack_top=_memory_stack;
	_init_stack_node(_memory_stack_top,SLL_ROUND_LARGE_PAGE(MEMORY_STACK_SIZE)-sizeof(stack_block_end_t)-sizeof(user_mem_block_t));
	STACK_BLOCK_GET_NEXT(_memory_stack_top)->data=STACK_BLOCK_END_OF_STACK_MARKER;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT void* sll_allocate(sll_size_t size){
	return sll_allocate_raw(size,1);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT void* sll_allocate_raw(sll_size_t size,sll_bool_t fail_on_error){
	if (!size){
		return NULL;
	}
	return PTR(ADDR(_allocate_chunk(size+sizeof(user_mem_block_t),fail_on_error))+sizeof(user_mem_block_t));
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT void* sll_allocate_stack(sll_size_t size){
	return sll_allocate_stack_raw(size,1);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT void* sll_allocate_stack_raw(sll_size_t size,sll_bool_t fail_on_error){
	if (!size){
		return NULL;
	}
	size+=sizeof(user_mem_block_t);
	user_mem_block_t* b=_stack_get(size);
	if (!b){
		b=_allocate_chunk(size,fail_on_error);
	}
	return PTR(ADDR(b)+sizeof(user_mem_block_t));
}



__SLL_EXTERNAL void sll_deallocate(void* pointer){
	if (!pointer){
		return;
	}
	user_mem_block_t* b=(user_mem_block_t*)(ADDR(pointer)-sizeof(user_mem_block_t));
	SLL_ASSERT(b->data&USER_MEM_BLOCK_FLAG_USED);
	if (b->data&USER_MEM_BLOCK_FLAG_STACK){
		_stack_add(b);
	}
	else{
		_pool_add(b);
	}
}



__SLL_EXTERNAL void* sll_memory_move(void* pointer,sll_bool_t direction){
	user_mem_block_t* b=(user_mem_block_t*)(ADDR(pointer)-sizeof(user_mem_block_t));
	SLL_ASSERT(b->data&USER_MEM_BLOCK_FLAG_USED);
	sll_size_t sz=USER_MEM_BLOCK_GET_SIZE(b);
	if (b->data&USER_MEM_BLOCK_FLAG_STACK){
		if (direction==SLL_MEMORY_MOVE_DIRECTION_FROM_STACK){
			user_mem_block_t* n=_allocate_chunk(sz,1);
			sll_copy_data(PTR(ADDR(b)+sizeof(user_mem_block_t)),sz-sizeof(user_mem_block_t),PTR(ADDR(n)+sizeof(user_mem_block_t)));
			_stack_add(b);
			pointer=PTR(ADDR(n)+sizeof(user_mem_block_t));
		}
	}
	else if (direction==SLL_MEMORY_MOVE_DIRECTION_TO_STACK){
		user_mem_block_t* n=_stack_get(sz);
		if (n){
			sll_copy_data(PTR(ADDR(b)+sizeof(user_mem_block_t)),sz-sizeof(user_mem_block_t),PTR(ADDR(n)+sizeof(user_mem_block_t)));
			_pool_add(b);
			pointer=PTR(ADDR(n)+sizeof(user_mem_block_t));
		}
	}
	return pointer;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT void* sll_reallocate(void* pointer,sll_size_t size){
	return sll_reallocate_raw(pointer,size,1);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT void* sll_reallocate_raw(void* pointer,sll_size_t size,sll_bool_t fail_on_error){
	if (!pointer){
		return sll_allocate_raw(size,fail_on_error);
	}
	if (!size){
		sll_deallocate(pointer);
		return NULL;
	}
	size+=sizeof(user_mem_block_t);
	sll_size_t raw_sz=size;
	user_mem_block_t* b=(user_mem_block_t*)(ADDR(pointer)-sizeof(user_mem_block_t));
	SLL_ASSERT(b->data&USER_MEM_BLOCK_FLAG_USED);
	sll_size_t prev_sz=USER_MEM_BLOCK_GET_SIZE(b);
	if (b->data&USER_MEM_BLOCK_FLAG_STACK){
		size=(size+7)&0xfffffffffffffff8ull;
		if (prev_sz==size){
			return pointer;
		}
		if (size>prev_sz){
			if (STACK_BLOCK_END_OF_STACK(STACK_BLOCK_GET_NEXT(b))){
				SLL_UNIMPLEMENTED();
			}
			else{
				user_mem_block_t* n_blk=_stack_get(size);
				if (!n_blk){
					n_blk=_allocate_chunk(raw_sz,fail_on_error);
				}
				sll_copy_data(PTR(ADDR(b)+sizeof(user_mem_block_t)),prev_sz-sizeof(user_mem_block_t),PTR(ADDR(n_blk)+sizeof(user_mem_block_t)));
				_stack_add(b);
				b=n_blk;
			}
		}
		else{
			stack_block_header_t* next=STACK_BLOCK_GET_NEXT(b);
			sll_size_t next_sz=STACK_BLOCK_GET_SIZE(next)+prev_sz-size;
			b->data=USER_MEM_BLOCK_INIT(size)|USER_MEM_BLOCK_FLAG_STACK;
			STACK_BLOCK_SET_END(b);
			if (STACK_BLOCK_END_OF_STACK(next)){
				SLL_UNIMPLEMENTED();
			}
			else if (next->data&USER_MEM_BLOCK_FLAG_USED){
				SLL_UNIMPLEMENTED();
			}
			else{
				next=STACK_BLOCK_GET_NEXT(b);
				next->data=STACK_BLOCK_INIT(next_sz);
				STACK_BLOCK_SET_END(next);
			}
		}
	}
	else{
		size=(size+15)&0xfffffffffffffff0ull;
		if (prev_sz==size){
			return pointer;
		}
		void* n_ptr=_pool_get((size>>4)-1);
		if (n_ptr){
			sll_copy_data(pointer,(prev_sz<size?prev_sz:size)-sizeof(user_mem_block_t),PTR(ADDR(n_ptr)+sizeof(user_mem_block_t)));
			_pool_add(b);
			b=n_ptr;
		}
		else{
			b=realloc(b,size);
			if (!b){
				if (fail_on_error){
					_raise_error(2,pointer,size);
				}
				return NULL;
			}
		}
		b->data=USER_MEM_BLOCK_INIT(size);
	}
	return PTR(ADDR(b)+sizeof(user_mem_block_t));
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT void* sll_zero_allocate(sll_size_t size){
	return sll_zero_allocate_raw(size,0);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT void* sll_zero_allocate_raw(sll_size_t size,sll_bool_t fail_on_error){
	if (!size){
		return NULL;
	}
	void* out=sll_allocate_raw(size,0);
	if (!out){
		return NULL;
	}
	_fill_zero(out,size);
	return out;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT void* sll_zero_allocate_stack(sll_size_t size){
	return sll_zero_allocate_stack_raw(size,1);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT void* sll_zero_allocate_stack_raw(sll_size_t size,sll_bool_t fail_on_error){
	if (!size){
		return NULL;
	}
	void* out=sll_allocate_stack_raw(size,fail_on_error);
	if (!out){
		return NULL;
	}
	_fill_zero(out,size);
	return out;
}
