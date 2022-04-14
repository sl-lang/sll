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
#include <sll/types.h>
#include <stdlib.h>



#define USER_MEM_BLOCK_FLAG_USED 1

#define USER_MEM_BLOCK_GET_SIZE(b) ((b)->dt>>1)
#define USER_MEM_BLOCK_INIT(sz) (USER_MEM_BLOCK_FLAG_USED|((sz)<<1))

#define POINTER_TO_STRING(p) \
	do{ \
		addr_t ptr=(addr_t)(p); \
		sll_string_length_t j=64; \
		do{ \
			j-=4; \
			sll_char_t c=(ptr>>j)&15; \
			bf[i]=c+(c>9?87:48); \
			i++; \
		} while (j); \
	} while (0)



static pool_data_t _memory_pool[MEMORY_POOL_SIZE];
static __SLL_U32 _memory_update;



static __SLL_NO_RETURN void _raise_error(sll_char_t t,void* p,sll_size_t sz){
	sll_file_flush(sll_stdout);
	sll_file_flush(sll_stderr);
	const sll_char_t* nm;
	switch (t){
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
	sll_char_t bf[64];
	sll_string_length_t i=sll_string_length(nm);
	sll_copy_data(nm,i,bf);
	bf[i]='(';
	i++;
	if (t==2){
		POINTER_TO_STRING(p);
		bf[i]=',';
		i++;
	}
	POINTER_TO_STRING(sz);
	bf[i]=')';
	bf[i+1]=0;
	sll_file_descriptor_t fd=sll_platform_get_default_stream_descriptor(SLL_PLATFORM_STREAM_ERROR);
	sll_platform_file_write(fd,MEMORY_FAIL_START,MEMORY_FAIL_START_SIZE,NULL);
	sll_platform_file_write(fd,bf,i+2,NULL);
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
			while (new){
				new--;
				empty_pool_pointer_t* ptr=malloc(b_sz);
				ptr->next=_memory_pool[i].ptr;
				_memory_pool[i].ptr=ptr;
			}
		}
		_memory_pool[i].alloc=0;
		_memory_pool[i].miss=0;
	}
}



static __SLL_FORCE_INLINE void _fill_zero(void* o,sll_size_t sz){
	wide_data_t* p=o;
	ASSUME_ALIGNED(p,4,8);
	sz=(sz+7)>>3;
#ifndef __SLL_BUILD_DARWIN
	__m256i zero=_mm256_setzero_si256();
	while (sz>3){
		_mm256_storeu_si256((__m256i*)p,zero);
		sz-=4;
		p+=4;
	}
#endif
	while (sz){
		*p=0;
		sz--;
		p++;
	}
}



static __SLL_FORCE_INLINE void _pool_add(user_mem_block_t* b){
	SLL_ASSERT(b->dt&USER_MEM_BLOCK_FLAG_USED);
	sll_size_t sz=USER_MEM_BLOCK_GET_SIZE(b)-1;
	if (sz>=MEMORY_POOL_SIZE||!_memory_pool[sz].sz){
		free(b);
		return;
	}
	empty_pool_pointer_t* ptr=(empty_pool_pointer_t*)b;
	ptr->next=_memory_pool[sz].ptr;
	_memory_pool[sz].ptr=ptr;
	_memory_pool[sz].sz--;
}



static __SLL_FORCE_INLINE void* _pool_get(sll_size_t sz){
	if (sz>=MEMORY_POOL_SIZE){
		return NULL;
	}
	_memory_update--;
	if (!_memory_update){
		_update_pool_extra();
	}
	_memory_pool[sz].alloc++;
	empty_pool_pointer_t* ptr=_memory_pool[sz].ptr;
	if (!ptr){
		_memory_pool[sz].miss++;
		return NULL;
	}
	_memory_pool[sz].sz++;
	_memory_pool[sz].ptr=ptr->next;
	return ptr;
}



void _memory_deinit(void){
	for (sll_size_t i=0;i<MEMORY_POOL_SIZE;i++){
		empty_pool_pointer_t* ptr=_memory_pool[i].ptr;
		while (ptr){
			empty_pool_pointer_t* nxt=ptr->next;
			free(ptr);
			ptr=nxt;
		}
	}
}



void _memory_init(void){
	_memory_update=MEMORY_POOL_UPDATE_TIMER;
	for (sll_size_t i=0;i<MEMORY_POOL_SIZE;i++){
		_memory_pool[i].alloc=0;
		_memory_pool[i].miss=0;
		_memory_pool[i].last_miss=0;
		_memory_pool[i].sz=MEMORY_POOL_MAX_BLOCKS;
		_memory_pool[i].ptr=NULL;
	}
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT void* sll_allocate(sll_size_t sz){
	return sll_allocate_raw(sz,1);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT void* sll_allocate_raw(sll_size_t sz,sll_bool_t err){
	if (!sz){
		return NULL;
	}
	sz=(sz+sizeof(user_mem_block_t)+15)>>4;
	user_mem_block_t* b=_pool_get(sz-1);
	if (!b){
		b=malloc(sz<<4);
		if (!b){
			if (err){
				_raise_error(0,NULL,sz);
			}
			return NULL;
		}
	}
	b->dt=USER_MEM_BLOCK_INIT(sz);
	return PTR(ADDR(b)+sizeof(user_mem_block_t));
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT void* sll_allocate_stack(sll_size_t sz){
	return sll_allocate_stack_raw(sz,1);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT void* sll_allocate_stack_raw(sll_size_t sz,sll_bool_t err){
	return sll_allocate_raw(sz,err);
}



__SLL_EXTERNAL void sll_deallocate(void* p){
	if (!p){
		return;
	}
	_pool_add(PTR(ADDR(p)-sizeof(user_mem_block_t)));
}



__SLL_EXTERNAL void* sll_memory_move(void* p,sll_bool_t d){
	return p;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT void* sll_reallocate(void* p,sll_size_t sz){
	return sll_reallocate_raw(p,sz,1);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT void* sll_reallocate_raw(void* p,sll_size_t sz,sll_bool_t err){
	if (!p){
		return sll_allocate_raw(sz,err);
	}
	if (!sz){
		sll_deallocate(p);
		return NULL;
	}
	user_mem_block_t* b=(user_mem_block_t*)(ADDR(p)-sizeof(user_mem_block_t));
	SLL_ASSERT(b->dt&USER_MEM_BLOCK_FLAG_USED);
	sz=(sz+sizeof(user_mem_block_t)+15)>>4;
	sll_size_t prev_sz=USER_MEM_BLOCK_GET_SIZE(b);
	if (prev_sz==sz){
		return p;
	}
	void* n_ptr=_pool_get(sz-1);
	if (n_ptr){
		sll_copy_data(p,((prev_sz<sz?prev_sz:sz)<<4)-sizeof(user_mem_block_t),PTR(ADDR(n_ptr)+sizeof(user_mem_block_t)));
		_pool_add(b);
		b=n_ptr;
	}
	else{
		b=realloc(b,sz<<4);
		if (!b){
			if (err){
				_raise_error(2,p,sz<<4);
			}
			return NULL;
		}
	}
	b->dt=USER_MEM_BLOCK_INIT(sz);
	return PTR(ADDR(b)+sizeof(user_mem_block_t));
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT void* sll_zero_allocate(sll_size_t sz){
	return sll_zero_allocate_raw(sz,0);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT void* sll_zero_allocate_raw(sll_size_t sz,sll_bool_t err){
	if (!sz){
		return NULL;
	}
	void* o=sll_allocate_raw(sz,0);
	if (!o){
		return NULL;
	}
	_fill_zero(o,sz);
	return o;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT void* sll_zero_allocate_stack(sll_size_t sz){
	return sll_zero_allocate_stack_raw(sz,1);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT void* sll_zero_allocate_stack_raw(sll_size_t sz,sll_bool_t err){
	if (!sz){
		return NULL;
	}
	void* o=sll_allocate_stack_raw(sz,err);
	if (!o){
		return NULL;
	}
	_fill_zero(o,sz);
	return o;
}
