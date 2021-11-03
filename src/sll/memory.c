#include <sll/_sll_internal.h>
#include <sll/common.h>
#include <sll/memory.h>
#include <sll/platform.h>
#include <sll/types.h>
#include <sll/util.h>
#include <stdint.h>
#include <stdlib.h>



static char _memory_zero_ptr[1];
static uint32_t _memory_data_mask=0;
static mem_block_t* _memory_head_blocks[(ALLOCATOR_MAX_SMALL_SIZE>>4)];
static page_header_t* _memory_page_head=NULL;



static void* _memory_allocate_chunk(sll_size_t sz){
	SLL_ASSERT(sz&&sz<=ALLOCATOR_MAX_SMALL_SIZE&&!(sz&15));
	uint8_t i=(uint8_t)((sz>>4)-1);
_find_block:
	if (_memory_data_mask&(1<<i)){
		mem_block_t* b=_memory_head_blocks[i];
		if (b->n){
			_memory_head_blocks[i]=b->n;
		}
		else{
			_memory_data_mask&=~(1<<i);
		}
		user_mem_block_t* o=(user_mem_block_t*)b;
		o->sz=sz;
		return (void*)(((uint64_t)o)+sizeof(user_mem_block_t));
	}
	if (i<(ALLOCATOR_MAX_SMALL_SIZE>>5)){
		uint8_t j=((i+1)<<1)-1;
		if (_memory_data_mask&(1<<j)){
			mem_block_t* a=_memory_head_blocks[j];
			if (a->n){
				_memory_head_blocks[j]=a->n;
			}
			else{
				_memory_data_mask&=~(1<<j);
			}
			a->n=NULL;
			_memory_data_mask|=1<<i;
			_memory_head_blocks[i]=a;
			user_mem_block_t* o=(user_mem_block_t*)(((uint64_t)a)+((i+1)<<4));
			o->sz=sz;
			return (void*)(((uint64_t)o)+sizeof(user_mem_block_t));
		}
	}
	uint32_t v=_memory_data_mask>>(i+1);
	if (v){
		uint8_t j=FIND_FIRST_SET_BIT(v)+i+1;
		SLL_ASSERT(j<(ALLOCATOR_MAX_SMALL_SIZE>>4));
		mem_block_t* b=_memory_head_blocks[j];
		if (b->n){
			_memory_head_blocks[j]=b->n;
		}
		else{
			_memory_data_mask&=~(1<<j);
		}
		user_mem_block_t* o=(user_mem_block_t*)b;
		o->sz=sz;
		b=(mem_block_t*)(((uint64_t)b)+sz);
		i=j-i-1;
		SLL_ASSERT(i<(ALLOCATOR_MAX_SMALL_SIZE>>4));
		b->n=(_memory_data_mask&(1<<i)?_memory_head_blocks[i]:NULL);
		_memory_head_blocks[i]=b;
		_memory_data_mask|=1<<i;
		return (void*)(((uint64_t)o)+sizeof(user_mem_block_t));
	}
	sll_page_size_t pg_sz=sll_platform_get_page_size()*ALLOCATOR_PAGE_ALLOC_COUNT;
	void* pg=sll_platform_allocate_page(pg_sz);
	page_header_t* h=pg;
	h->n=_memory_page_head;
	_memory_page_head=h;
	pg_sz=(pg_sz-sizeof(page_header_t))&0xfffffffffffffff0ull;
	while (pg_sz>=ALLOCATOR_MAX_SMALL_SIZE){
		mem_block_t* b=pg;
		b->n=(_memory_data_mask&(1<<((ALLOCATOR_MAX_SMALL_SIZE>>4)-1))?_memory_head_blocks[(ALLOCATOR_MAX_SMALL_SIZE>>4)-1]:NULL);
		_memory_data_mask|=1<<((ALLOCATOR_MAX_SMALL_SIZE>>4)-1);
		_memory_head_blocks[(ALLOCATOR_MAX_SMALL_SIZE>>4)-1]=b;
		pg_sz-=ALLOCATOR_MAX_SMALL_SIZE;
		pg=(void*)(((uint64_t)pg)+ALLOCATOR_MAX_SMALL_SIZE);
	}
	if (pg_sz){
		uint8_t j=(uint8_t)((pg_sz>>4)-1);
		mem_block_t* b=pg;
		b->n=(_memory_data_mask&(1<<j)?_memory_head_blocks[j]:NULL);
		_memory_data_mask|=1<<j;
		_memory_head_blocks[j]=b;
	}
	goto _find_block;
}



static void _memory_release_chunk(user_mem_block_t* b){
	SLL_ASSERT(b->sz<=ALLOCATOR_MAX_SMALL_SIZE);
	uint8_t i=(uint8_t)((b->sz>>4)-1);
	mem_block_t* n=(mem_block_t*)b;
	n->n=(_memory_data_mask&(1<<i)?_memory_head_blocks[i]:NULL);
	_memory_data_mask|=1<<i;
	_memory_head_blocks[i]=n;
}



void _memory_release_data(void){
	page_header_t* p=_memory_page_head;
	sll_page_size_t sz=sll_platform_get_page_size()*ALLOCATOR_PAGE_ALLOC_COUNT;
	while (p){
		page_header_t* n=p->n;
		sll_platform_free_page(p,sz);
		p=n;
	}
	_memory_page_head=NULL;
}



__SLL_FUNC __SLL_CHECK_OUTPUT void* sll_allocate(sll_size_t sz){
	if (!sz){
		return _memory_zero_ptr;
	}
	sz=(sz+sizeof(user_mem_block_t)+15)&0xfffffffffffffff0ull;
	if (sz<=ALLOCATOR_MAX_SMALL_SIZE){
		return _memory_allocate_chunk(sz);
	}
	user_mem_block_t* o=malloc(sz);
	if (!o){
		return NULL;
	}
	o->sz=sz;
	return (void*)(((uint64_t)o)+sizeof(user_mem_block_t));
}



__SLL_FUNC __SLL_CHECK_OUTPUT void* sll_zero_allocate(sll_size_t sz,sll_size_t c){
	sz*=c;
	if (!sz){
		return _memory_zero_ptr;
	}
	void* o=sll_allocate(sz);
	uint64_t* p=o;
	sz=(sz+7)>>3;
	do{
		*p=0;
		p++;
		sz--;
	} while (sz);
	return o;
}



__SLL_FUNC __SLL_CHECK_OUTPUT void* sll_rellocate(void* p,sll_size_t sz){
	if (!p||p==_memory_zero_ptr){
		return sll_allocate(sz);
	}
	if (!sz){
		sll_deallocate(p);
		return _memory_zero_ptr;
	}
	user_mem_block_t* b=(user_mem_block_t*)(((uint64_t)p)-sizeof(user_mem_block_t));
	sz=(sz+sizeof(user_mem_block_t)+15)&0xfffffffffffffff0ull;
	if (b->sz==sz){
		return p;
	}
	if (b->sz<=ALLOCATOR_MAX_SMALL_SIZE){
		void* n=sll_allocate(sz);
		const uint64_t* s=(const uint64_t*)p;
		uint64_t* d=(uint64_t*)n;
		sz=((b->sz>sz?sz:b->sz)-sizeof(user_mem_block_t))>>3;
		do{
			*d=*s;
			s++;
			d++;
			sz--;
		} while (sz);
		_memory_release_chunk(b);
		return n;
	}
	if (sz<=ALLOCATOR_MAX_SMALL_SIZE){
		void* n=_memory_allocate_chunk(sz);
		const uint64_t* s=(const uint64_t*)p;
		uint64_t* d=(uint64_t*)n;
		sz=(sz-sizeof(user_mem_block_t))>>3;
		do{
			*d=*s;
			s++;
			d++;
			sz--;
		} while (sz);
		free(b);
		return n;
	}
	b=realloc(b,sz);
	if (!b){
		return NULL;
	}
	b->sz=sz;
	return (void*)(((uint64_t)b)+sizeof(user_mem_block_t));

}



__SLL_FUNC void sll_deallocate(void* p){
	if (!p||p==_memory_zero_ptr){
		return;
	}
	user_mem_block_t* b=(user_mem_block_t*)(((uint64_t)p)-sizeof(user_mem_block_t));
	if (b->sz>ALLOCATOR_MAX_SMALL_SIZE){
		free(b);
	}
	else{
		_memory_release_chunk(b);
	}
}
