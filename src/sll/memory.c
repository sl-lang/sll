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



#define USED_BLOCK_FLAG_USED 0x8000000000000000ull
#define USED_BLOCK_FLAG_STACK 0x4000000000000000ull

#define USED_BLOCK_GET_SIZE(b) ((b)->dt&0x3fffffffffffffffull)

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
		case 2:
			nm=SLL_CHAR("sll_reallocate");
			break;
		case 3:
			nm=SLL_CHAR("sll_zero_allocate");
			break;
		default:
			nm=SLL_CHAR("sll_zero_allocate_stack");
			break;
	}
	sll_char_t bf[64];
	sll_string_length_t i=sll_string_length_unaligned(nm);
	sll_copy_data(nm,i,bf);
	bf[i]='(';
	i++;
	if (nm[4]=='r'){
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



static void _fill_zero(void* o,sll_size_t sz){
	wide_data_t* p=o;
	ASSUME_ALIGNED(p,4,8);
	sz=(sz+7)>>3;
	do{
		*p=0;
		p++;
		sz--;
	} while (sz);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT void* sll_allocate(sll_size_t sz){
	if (!sz){
		return NULL;
	}
	void* o=sll_allocate_fail(sz);
	if (!o){
		_raise_error(0,NULL,sz);
	}
	return o;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT void* sll_allocate_fail(sll_size_t sz){
	if (!sz){
		return NULL;
	}
	user_mem_block_t* b=malloc((sz+sizeof(user_mem_block_t)+15)&0xfffffffffffffff0ull);
	if (!b){
		return NULL;
	}
	b->dt=USED_BLOCK_FLAG_USED|sz;
	return PTR(ADDR(b)+sizeof(user_mem_block_t));
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT void* sll_allocate_stack(sll_size_t sz){
	if (!sz){
		return NULL;
	}
	void* o=sll_allocate_stack_fail(sz);
	if (!o){
		_raise_error(1,NULL,sz);
	}
	return o;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT void* sll_allocate_stack_fail(sll_size_t sz){
	return sll_allocate(sz);
}



__SLL_EXTERNAL void sll_deallocate(void* p){
	if (!p){
		return;
	}
	user_mem_block_t* b=(user_mem_block_t*)(ADDR(p)-sizeof(user_mem_block_t));
	SLL_ASSERT(b->dt&USED_BLOCK_FLAG_USED);
	free(b);
}



__SLL_EXTERNAL void* sll_memory_move(void* p,sll_bool_t d){
	return p;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT void* sll_reallocate(void* p,sll_size_t sz){
	void* o=sll_reallocate_fail(p,sz);
	if (!o&&sz){
		_raise_error(2,p,sz);
	}
	return o;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT void* sll_reallocate_fail(void* p,sll_size_t sz){
	if (!p){
		return sll_allocate_fail(sz);
	}
	if (!sz){
		sll_deallocate(p);
		return NULL;
	}
	user_mem_block_t* b=(user_mem_block_t*)(ADDR(p)-sizeof(user_mem_block_t));
	SLL_ASSERT(b->dt&USED_BLOCK_FLAG_USED);
	sz=(sz+sizeof(user_mem_block_t)+15)&0xfffffffffffffff0ull;
	if (USED_BLOCK_GET_SIZE(b)==sz){
		return p;
	}
	b=realloc(b,sz);
	if (!b){
		return NULL;
	}
	b->dt=USED_BLOCK_FLAG_USED|sz;
	return PTR(ADDR(b)+sizeof(user_mem_block_t));
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT void* sll_zero_allocate(sll_size_t sz){
	if (!sz){
		return NULL;
	}
	void* o=sll_allocate_fail(sz);
	if (!o){
		_raise_error(3,NULL,sz);
	}
	_fill_zero(o,sz);
	return o;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT void* sll_zero_allocate_fail(sll_size_t sz){
	if (!sz){
		return NULL;
	}
	void* o=sll_allocate_fail(sz);
	if (!o){
		return NULL;
	}
	_fill_zero(o,sz);
	return o;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT void* sll_zero_allocate_stack(sll_size_t sz){
	if (!sz){
		return NULL;
	}
	void* o=sll_allocate_stack_fail(sz);
	if (!o){
		_raise_error(4,NULL,sz);
	}
	_fill_zero(o,sz);
	return o;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT void* sll_zero_allocate_stack_fail(sll_size_t sz){
	if (!sz){
		return NULL;
	}
	void* o=sll_allocate_stack_fail(sz);
	if (!o){
		return NULL;
	}
	_fill_zero(o,sz);
	return o;
}
