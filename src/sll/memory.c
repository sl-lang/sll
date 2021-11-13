#include <sll/_sll_internal.h>
#include <sll/common.h>
#include <sll/memory.h>
#include <sll/platform.h>
#include <sll/types.h>
#include <sll/util.h>
#include <stdint.h>
#include <stdlib.h>



#define USED_BLOCK_FLAG_USED 0x8000000000000000ull
#define USED_BLOCK_FLAG_STACK 0x4000000000000000ull

#define USED_BLOCK_GET_SIZE(b) ((b)->dt&0x3fffffffffffffffull)



#ifdef USE_BUILTIN_MALLOC



void _memory_release_data(void){
}



__SLL_FUNC __SLL_CHECK_OUTPUT void* sll_allocate(sll_size_t sz){
	if (!sz){
		return NULL;
	}
	sz=(sz+sizeof(user_mem_block_t)+15)&0xfffffffffffffff0ull;
	user_mem_block_t* b=malloc(sz);
	if (!b){
		return NULL;
	}
	b->dt=USED_BLOCK_FLAG_USED|sz;
	return (void*)(((uint64_t)b)+sizeof(user_mem_block_t));
}



__SLL_FUNC __SLL_CHECK_OUTPUT void* sll_allocate_stack(sll_size_t sz){
	return sll_allocate(sz);
}



__SLL_FUNC void sll_deallocate(void* p){
	if (!p){
		return;
	}
	user_mem_block_t* b=(user_mem_block_t*)(((uint64_t)p)-sizeof(user_mem_block_t));
	SLL_ASSERT(b->dt&USED_BLOCK_FLAG_USED);
	SLL_ASSERT(!(b->dt&USED_BLOCK_FLAG_STACK));
	free(b);
}



__SLL_FUNC __SLL_CHECK_OUTPUT void* sll_reallocate(void* p,sll_size_t sz){
	if (!p){
		return sll_allocate(sz);
	}
	if (!sz){
		sll_deallocate(p);
		return NULL;
	}
	user_mem_block_t* b=(user_mem_block_t*)(((uint64_t)p)-sizeof(user_mem_block_t));
	SLL_ASSERT(b->dt&USED_BLOCK_FLAG_USED);
	SLL_ASSERT(!(b->dt&USED_BLOCK_FLAG_STACK));
	sz=(sz+sizeof(user_mem_block_t)+15)&0xfffffffffffffff0ull;
	if (USED_BLOCK_GET_SIZE(b)==sz){
		return p;
	}
	b=realloc(b,sz);
	if (!b){
		return NULL;
	}
	b->dt=USED_BLOCK_FLAG_USED|sz;
	return (void*)(((uint64_t)b)+sizeof(user_mem_block_t));
}



#else



#define GET_STACK_BLOCK_SIZE(b) (((b)->dt&0x7ffffff)<<4)
#define GET_STACK_BLOCK_PREV_SIZE(b) (((b)->dt>>27)&0x7ffffff0)
#define SET_STACK_BLOCK_SIZE(p_sz,sz) (((p_sz)<<27)|((sz)>>4))
#define UPDATE_STACK_BLOCK_PREV_SIZE(b,p_sz) ((b)->dt=((b)->dt&0xffc0000007ffffffull)|((p_sz)<<27))
#define UPDATE_STACK_BLOCK_SIZE(b,p_sz) ((b)->dt=((b)->dt&0xfffffffff8000000ull)|((sz)>>4))



static uint32_t _memory_data_mask=0;
static mem_block_t* _memory_head_blocks[ALLOCATOR_MAX_SMALL_SIZE>>4];
static page_header_t* _memory_page_head=NULL;
static uint64_t _memory_stack_size=0;
static void* _memory_stack_page=NULL;
static mem_stack_block_t* _memory_stack_top=0;



void _memory_release_data(void){
	if (_memory_page_head){
		page_header_t* p=_memory_page_head;
		sll_page_size_t sz=sll_platform_get_page_size()*ALLOCATOR_PAGE_ALLOC_COUNT;
		while (p){
			page_header_t* n=p->n;
			sll_platform_free_page(p,sz);
			p=n;
		}
		_memory_page_head=NULL;
	}
	if (_memory_stack_page){
		sll_platform_free_page(_memory_stack_page,_memory_stack_size);
		_memory_stack_page=NULL;
	}
}



static void* _allocate_chunk(sll_size_t sz){
	SLL_ASSERT(sz>=16&&sz<=ALLOCATOR_MAX_SMALL_SIZE&&!(sz&15));
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
		o->dt=USED_BLOCK_FLAG_USED|sz;
		return (void*)(((uint64_t)o)+sizeof(user_mem_block_t));
	}
	if (i<(ALLOCATOR_MAX_SMALL_SIZE>>5)){
		uint8_t j=(i<<1)+1;
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
			user_mem_block_t* o=(user_mem_block_t*)(((uint64_t)a)+sz);
			o->dt=USED_BLOCK_FLAG_USED|sz;
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
		o->dt=USED_BLOCK_FLAG_USED|sz;
		i=j-i-1;
		SLL_ASSERT(i<(ALLOCATOR_MAX_SMALL_SIZE>>4));
		b=(mem_block_t*)(((uint64_t)b)+sz);
		b->n=((_memory_data_mask&(1<<i))?_memory_head_blocks[i]:NULL);
		_memory_data_mask|=1<<i;
		_memory_head_blocks[i]=b;
		return (void*)(((uint64_t)o)+sizeof(user_mem_block_t));
	}
	sll_page_size_t pg_sz=sll_platform_get_page_size()*ALLOCATOR_PAGE_ALLOC_COUNT;
	void* pg=sll_platform_allocate_page(pg_sz);
	page_header_t* h=pg;
	h->n=_memory_page_head;
	_memory_page_head=h;
	pg_sz=(pg_sz-sizeof(page_header_t))&0xfffffffffffffff0ull;
	pg=(void*)((((uint64_t)pg)+sizeof(page_header_t)+15)&0xfffffffffffffff0ull);
	while (pg_sz>=ALLOCATOR_MAX_SMALL_SIZE){
		mem_block_t* b=pg;
		b->n=((_memory_data_mask&(1<<((ALLOCATOR_MAX_SMALL_SIZE>>4)-1)))?_memory_head_blocks[(ALLOCATOR_MAX_SMALL_SIZE>>4)-1]:NULL);
		_memory_data_mask|=1<<((ALLOCATOR_MAX_SMALL_SIZE>>4)-1);
		_memory_head_blocks[(ALLOCATOR_MAX_SMALL_SIZE>>4)-1]=b;
		pg_sz-=ALLOCATOR_MAX_SMALL_SIZE;
		pg=(void*)(((uint64_t)pg)+ALLOCATOR_MAX_SMALL_SIZE);
	}
	if (pg_sz>=16){
		uint8_t j=(uint8_t)((pg_sz>>4)-1);
		mem_block_t* b=pg;
		b->n=((_memory_data_mask&(1<<j))?_memory_head_blocks[j]:NULL);
		_memory_data_mask|=1<<j;
		_memory_head_blocks[j]=b;
	}
	goto _find_block;
}



static void _release_chunk(user_mem_block_t* b){
	SLL_ASSERT(USED_BLOCK_GET_SIZE(b)<=ALLOCATOR_MAX_SMALL_SIZE);
	uint8_t i=(uint8_t)((USED_BLOCK_GET_SIZE(b)>>4)-1);
	mem_block_t* n=(mem_block_t*)b;
	n->n=((_memory_data_mask&(1<<i))?_memory_head_blocks[i]:NULL);
	_memory_data_mask|=1<<i;
	_memory_head_blocks[i]=n;
}



static void _fill_zero(void* o,sll_size_t sz){
	uint64_t* p=o;
	ASSUME_ALIGNED(p,4,8);
	sz=(sz+7)>>3;
	do{
		*p=0;
		p++;
		sz--;
	} while (sz);
}



__SLL_FUNC __SLL_CHECK_OUTPUT void* sll_allocate(sll_size_t sz){
	if (!sz){
		return NULL;
	}
	sz=(sz+sizeof(user_mem_block_t)+15)&0xfffffffffffffff0ull;
	if (sz<=ALLOCATOR_MAX_SMALL_SIZE){
		return _allocate_chunk(sz);
	}
	user_mem_block_t* b=malloc(sz);
	if (!b){
		return NULL;
	}
	b->dt=USED_BLOCK_FLAG_USED|sz;
	return (void*)(((uint64_t)b)+sizeof(user_mem_block_t));
}



__SLL_FUNC __SLL_CHECK_OUTPUT void* sll_allocate_stack(sll_size_t sz){
	if (!sz){
		return NULL;
	}
	sz=(sz+sizeof(user_mem_block_t)+15)&0xfffffffffffffff0ull;
	if (!_memory_stack_size){
		_memory_stack_size=sll_platform_get_page_size()*ALLOCATOR_STACK_PAGE_ALLOC_COUNT;
	}
	if (sz<=_memory_stack_size){
		if (!_memory_stack_page){
			_memory_stack_page=sll_platform_allocate_page(_memory_stack_size);
			_memory_stack_top=_memory_stack_page;
			SLL_ASSERT(!(_memory_stack_size&15));
			_memory_stack_top->dt=SET_STACK_BLOCK_SIZE(0,_memory_stack_size);
		}
		if (_memory_stack_top&&GET_STACK_BLOCK_SIZE(_memory_stack_top)>=sz){
			sll_size_t p_sz=GET_STACK_BLOCK_PREV_SIZE(_memory_stack_top);
			sll_size_t n_sz=GET_STACK_BLOCK_SIZE(_memory_stack_top)-sz;
			SLL_ASSERT(!(p_sz&15)&&!(sz&15)&&!(n_sz&15));
			user_mem_block_t* b=(user_mem_block_t*)_memory_stack_top;
			b->dt=USED_BLOCK_FLAG_USED|USED_BLOCK_FLAG_STACK|SET_STACK_BLOCK_SIZE(p_sz,sz);
			if (n_sz){
				_memory_stack_top=(mem_stack_block_t*)(((uint64_t)_memory_stack_top)+sz);
				_memory_stack_top->dt=SET_STACK_BLOCK_SIZE(sz,n_sz);
			}
			else{
				_memory_stack_top=NULL;
			}
			return (void*)(((uint64_t)b)+sizeof(user_mem_block_t));
		}
	}
	user_mem_block_t* b=malloc(sz);
	if (!b){
		return NULL;
	}
	b->dt=USED_BLOCK_FLAG_USED|sz;
	return (void*)(((uint64_t)b)+sizeof(user_mem_block_t));
}



__SLL_FUNC void sll_deallocate(void* p){
	if (!p){
		return;
	}
	user_mem_block_t* b=(user_mem_block_t*)(((uint64_t)p)-sizeof(user_mem_block_t));
	SLL_ASSERT(b->dt&USED_BLOCK_FLAG_USED);
	if (b->dt&USED_BLOCK_FLAG_STACK){
		sll_size_t p_sz=GET_STACK_BLOCK_PREV_SIZE(b);
		sll_size_t sz=GET_STACK_BLOCK_SIZE(b);
		b->dt&=0x3fffffffffffffffull;
		mem_stack_block_t* n=(mem_stack_block_t*)(((uint64_t)b)+sz);
		if (((uint64_t)n)<((uint64_t)_memory_stack_page)+_memory_stack_size&&!(n->dt&USED_BLOCK_FLAG_USED)){
			mem_stack_block_t* nn=(mem_stack_block_t*)(((uint64_t)n)+GET_STACK_BLOCK_SIZE(n));
			if (((uint64_t)nn)<((uint64_t)_memory_stack_page)+_memory_stack_size){
				UPDATE_STACK_BLOCK_PREV_SIZE(nn,GET_STACK_BLOCK_SIZE(n)+sz);
			}
			if (n==_memory_stack_top){
				_memory_stack_top=(mem_stack_block_t*)b;
			}
			sz+=GET_STACK_BLOCK_SIZE(n);
			b->dt=SET_STACK_BLOCK_SIZE(p_sz,sz);
		}
		if (p_sz){
			mem_stack_block_t* pn=(mem_stack_block_t*)(((uint64_t)b)-p_sz);
			if (!(pn->dt&USED_BLOCK_FLAG_USED)){
				p_sz+=sz;
				UPDATE_STACK_BLOCK_SIZE(pn,p_sz);
				n=(mem_stack_block_t*)(((uint64_t)b)+sz);
				if (((uint64_t)n)<((uint64_t)_memory_stack_page)+_memory_stack_size){
					SLL_ASSERT(n->dt&USED_BLOCK_FLAG_USED);
					UPDATE_STACK_BLOCK_PREV_SIZE(n,p_sz);
				}
				else if (((mem_stack_block_t*)b)==_memory_stack_top){
					_memory_stack_top=pn;
				}
			}
		}
	}
	else if (USED_BLOCK_GET_SIZE(b)>ALLOCATOR_MAX_SMALL_SIZE){
		free(b);
	}
	else{
		_release_chunk(b);
	}
}



__SLL_FUNC __SLL_CHECK_OUTPUT void* sll_reallocate(void* p,sll_size_t sz){
	if (!p){
		return sll_allocate(sz);
	}
	if (!sz){
		sll_deallocate(p);
		return NULL;
	}
	user_mem_block_t* b=(user_mem_block_t*)(((uint64_t)p)-sizeof(user_mem_block_t));
	SLL_ASSERT(b->dt&USED_BLOCK_FLAG_USED);
	sz=(sz+sizeof(user_mem_block_t)+15)&0xfffffffffffffff0ull;
	if (b->dt&USED_BLOCK_FLAG_STACK){
		if (GET_STACK_BLOCK_SIZE(b)==sz){
			return p;
		}
		mem_stack_block_t* n=(mem_stack_block_t*)(((uint64_t)b)+GET_STACK_BLOCK_SIZE(b));
		if (GET_STACK_BLOCK_SIZE(b)>sz){
			sll_size_t off=GET_STACK_BLOCK_SIZE(b)-sz;
			SLL_ASSERT(!(off&15));
			if (((uint64_t)n)<((uint64_t)_memory_stack_page)+_memory_stack_size){
				if (n->dt&USED_BLOCK_FLAG_USED){
					SLL_UNIMPLEMENTED();
				}
				else{
					mem_stack_block_t* nn=(mem_stack_block_t*)(((uint64_t)n)+GET_STACK_BLOCK_SIZE(n));
					if (((uint64_t)nn)<((uint64_t)_memory_stack_page)+_memory_stack_size){
						UPDATE_STACK_BLOCK_PREV_SIZE(nn,GET_STACK_BLOCK_SIZE(n)+off);
					}
					nn=(mem_stack_block_t*)(((uint64_t)n)-off);
					nn->dt=SET_STACK_BLOCK_SIZE(sz,GET_STACK_BLOCK_SIZE(n)+off);
				}
			}
			else{
				SLL_UNIMPLEMENTED();
			}
			UPDATE_STACK_BLOCK_SIZE(b,sz);
			return p;
		}
		if (((uint64_t)n)<((uint64_t)_memory_stack_page)+_memory_stack_size&&!(n->dt&USED_BLOCK_FLAG_USED)&&sz-GET_STACK_BLOCK_SIZE(b)<=GET_STACK_BLOCK_SIZE(n)){
			sll_size_t n_sz=GET_STACK_BLOCK_SIZE(n)-sz+GET_STACK_BLOCK_SIZE(b);
			UPDATE_STACK_BLOCK_SIZE(b,sz);
			mem_stack_block_t* nn=(mem_stack_block_t*)(((uint64_t)b)+sz);
			if (((uint64_t)nn)>=((uint64_t)_memory_stack_page)+_memory_stack_size){
				SLL_ASSERT(n==_memory_stack_top);
				_memory_stack_top=NULL;
			}
			else{
				SLL_ASSERT(!(n_sz&15));
				nn->dt=SET_STACK_BLOCK_SIZE(sz,n_sz);
				if (n==_memory_stack_top){
					_memory_stack_top=nn;
				}
				mem_stack_block_t* nnn=(mem_stack_block_t*)(((uint64_t)nn)+n_sz);
				if (((uint64_t)nnn)<((uint64_t)_memory_stack_page)+_memory_stack_size){
					UPDATE_STACK_BLOCK_PREV_SIZE(nnn,n_sz);
				}
			}
			return p;
		}
		SLL_UNIMPLEMENTED();
	}
	if (USED_BLOCK_GET_SIZE(b)==sz){
		return p;
	}
	if (USED_BLOCK_GET_SIZE(b)<=ALLOCATOR_MAX_SMALL_SIZE){
		void* n=NULL;
		if (sz<=ALLOCATOR_MAX_SMALL_SIZE){
			n=_allocate_chunk(sz);
		}
		else{
			user_mem_block_t* blk=malloc(sz);
			if (!blk){
				return NULL;
			}
			blk->dt=USED_BLOCK_FLAG_USED|sz;
			n=(void*)(((uint64_t)blk)+sizeof(user_mem_block_t));
		}
		const uint64_t* s=(const uint64_t*)p;
		uint64_t* d=(uint64_t*)n;
		sll_size_t i=((USED_BLOCK_GET_SIZE(b)>sz?sz:USED_BLOCK_GET_SIZE(b))-sizeof(user_mem_block_t))>>3;
		do{
			*d=*s;
			s++;
			d++;
			i--;
		} while (i);
		_release_chunk(b);
		return n;
	}
	if (sz<=ALLOCATOR_MAX_SMALL_SIZE){
		void* n=_allocate_chunk(sz);
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
	b->dt=USED_BLOCK_FLAG_USED|sz;
	return (void*)(((uint64_t)b)+sizeof(user_mem_block_t));
}



#endif



__SLL_FUNC __SLL_CHECK_OUTPUT void* sll_zero_allocate(sll_size_t sz){
	if (!sz){
		return NULL;
	}
	void* o=sll_allocate(sz);
	_fill_zero(o,sz);
	return o;
}



__SLL_FUNC __SLL_CHECK_OUTPUT void* sll_zero_allocate_stack(sll_size_t sz){
	if (!sz){
		return NULL;
	}
	void* o=sll_allocate_stack(sz);
	_fill_zero(o,sz);
	return o;
}
