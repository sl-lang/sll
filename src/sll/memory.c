#include <sll/_sll_internal.h>
#include <sll/common.h>
#include <sll/data.h>
#include <sll/file.h>
#include <sll/generated/memory_fail.h>
#include <sll/memory.h>
#include <sll/platform.h>
#include <sll/types.h>
#include <stdlib.h>



#define USED_BLOCK_FLAG_USED 0x8000000000000000ull
#define USED_BLOCK_FLAG_STACK 0x4000000000000000ull

#define USED_BLOCK_GET_SIZE(b) ((b)->dt&0x3fffffffffffffffull)

#ifdef USE_STACK_ALLOCATOR
#define ASSERT_CORRECT_TOP() SLL_ASSERT(!_memory_stack_top||(ADDR(_memory_stack_top))+GET_STACK_BLOCK_SIZE(_memory_stack_top)==(ADDR(_memory_stack_page))+SLL_ROUND_LARGE_PAGE(ALLOCATOR_STACK_ALLOC_SIZE))
#define GET_STACK_BLOCK_SIZE(b) (((b)->dt&0x7ffffff)<<4)
#define GET_STACK_BLOCK_PREV_SIZE(b) (((b)->dt>>27)&0x7ffffff0)
#define SET_STACK_BLOCK_SIZE(p_sz,sz) (((p_sz)<<27)|((sz)>>4))
#define UPDATE_STACK_BLOCK_PREV_SIZE(b,p_sz) ((b)->dt=((b)->dt&0xffc0000007ffffffull)|((p_sz)<<27))
#define UPDATE_STACK_BLOCK_SIZE(b,sz) ((b)->dt=((b)->dt&0xfffffffff8000000ull)|((sz)>>4))
#endif

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



static small_bitmap_t _memory_data_mask=0;
static mem_block_t* _memory_head_blocks[ALLOCATOR_MAX_SMALL_SIZE>>4];
static page_header_t* _memory_page_head=NULL;
#ifdef USE_STACK_ALLOCATOR
static void* _memory_stack_page=NULL;
static mem_stack_block_t* _memory_stack_top=0;
#endif



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
	sll_platform_file_write(fd,MEMORY_FAIL_START,MEMORY_FAIL_START_SIZE);
	sll_platform_file_write(fd,bf,i+2);
	sll_platform_file_write(fd,MEMORY_FAIL_END,MEMORY_FAIL_END_SIZE);
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



static void* _allocate_chunk(sll_size_t sz){
	SLL_ASSERT(sz>=16&&sz<=ALLOCATOR_MAX_SMALL_SIZE&&!(sz&15));
	bucket_index_t i=(bucket_index_t)((sz>>4)-1);
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
		return PTR(ADDR(o)+sizeof(user_mem_block_t));
	}
	if (i<(ALLOCATOR_MAX_SMALL_SIZE>>5)){
		bucket_index_t j=(i<<1)+1;
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
			user_mem_block_t* o=(user_mem_block_t*)(ADDR(a)+sz);
			o->dt=USED_BLOCK_FLAG_USED|sz;
			return PTR(ADDR(o)+sizeof(user_mem_block_t));
		}
	}
	if (i+1<(ALLOCATOR_MAX_SMALL_SIZE>>4)){
		small_bitmap_t v=_memory_data_mask>>(i+1);
		if (v){
			bucket_index_t j=FIND_FIRST_SET_BIT(v)+i+1;
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
			b=(mem_block_t*)(ADDR(b)+sz);
			b->n=((_memory_data_mask&(1<<i))?_memory_head_blocks[i]:NULL);
			_memory_data_mask|=1<<i;
			_memory_head_blocks[i]=b;
			return PTR(ADDR(o)+sizeof(user_mem_block_t));
		}
	}
	void* pg=sll_platform_allocate_page(SLL_ROUND_PAGE(ALLOCATOR_ALLOC_SIZE),0);
	page_header_t* h=pg;
	h->n=_memory_page_head;
	_memory_page_head=h;
	sll_size_t pg_sz=(SLL_ROUND_PAGE(ALLOCATOR_ALLOC_SIZE)-sizeof(page_header_t))&0xfffffffffffffff0ull;
	pg=(void*)((ADDR(pg)+sizeof(page_header_t)+15)&0xfffffffffffffff0ull);
	while (pg_sz>=ALLOCATOR_MAX_SMALL_SIZE){
		mem_block_t* b=pg;
		b->n=((_memory_data_mask&(1<<((ALLOCATOR_MAX_SMALL_SIZE>>4)-1)))?_memory_head_blocks[(ALLOCATOR_MAX_SMALL_SIZE>>4)-1]:NULL);
		_memory_data_mask|=1<<((ALLOCATOR_MAX_SMALL_SIZE>>4)-1);
		_memory_head_blocks[(ALLOCATOR_MAX_SMALL_SIZE>>4)-1]=b;
		pg_sz-=ALLOCATOR_MAX_SMALL_SIZE;
		pg=PTR(ADDR(pg)+ALLOCATOR_MAX_SMALL_SIZE);
	}
	if (pg_sz>=16){
		bucket_index_t j=(bucket_index_t)((pg_sz>>4)-1);
		mem_block_t* b=pg;
		b->n=((_memory_data_mask&(1<<j))?_memory_head_blocks[j]:NULL);
		_memory_data_mask|=1<<j;
		_memory_head_blocks[j]=b;
	}
	goto _find_block;
}



#ifdef USE_STACK_ALLOCATOR
static void* _allocate_chunk_stack(sll_size_t sz){
	if (sz<=SLL_ROUND_LARGE_PAGE(ALLOCATOR_STACK_ALLOC_SIZE)){
		if (!_memory_stack_page){
			_memory_stack_page=sll_platform_allocate_page(SLL_ROUND_LARGE_PAGE(ALLOCATOR_STACK_ALLOC_SIZE),0);
			_memory_stack_top=_memory_stack_page;
			SLL_ASSERT(!(SLL_ROUND_LARGE_PAGE(ALLOCATOR_STACK_ALLOC_SIZE)&15));
			_memory_stack_top->dt=SET_STACK_BLOCK_SIZE(0,SLL_ROUND_LARGE_PAGE(ALLOCATOR_STACK_ALLOC_SIZE));
		}
		if (_memory_stack_top&&GET_STACK_BLOCK_SIZE(_memory_stack_top)>=sz){
			sll_size_t p_sz=GET_STACK_BLOCK_PREV_SIZE(_memory_stack_top);
			sll_size_t n_sz=GET_STACK_BLOCK_SIZE(_memory_stack_top)-sz;
			SLL_ASSERT(!(p_sz&15)&&!(sz&15)&&!(n_sz&15));
			user_mem_block_t* b=(user_mem_block_t*)_memory_stack_top;
			b->dt=USED_BLOCK_FLAG_USED|USED_BLOCK_FLAG_STACK|SET_STACK_BLOCK_SIZE(p_sz,sz);
			if (n_sz){
				_memory_stack_top=(mem_stack_block_t*)(ADDR(_memory_stack_top)+sz);
				_memory_stack_top->dt=SET_STACK_BLOCK_SIZE(sz,n_sz);
			}
			else{
				_memory_stack_top=NULL;
			}
			ASSERT_CORRECT_TOP();
			return PTR(ADDR(b)+sizeof(user_mem_block_t));
		}
	}
	return NULL;
}
#endif



static void _release_chunk(user_mem_block_t* b){
	SLL_ASSERT(USED_BLOCK_GET_SIZE(b)<=ALLOCATOR_MAX_SMALL_SIZE);
	bucket_index_t i=(bucket_index_t)((USED_BLOCK_GET_SIZE(b)>>4)-1);
	mem_block_t* n=(mem_block_t*)b;
	n->n=((_memory_data_mask&(1<<i))?_memory_head_blocks[i]:NULL);
	_memory_data_mask|=1<<i;
	_memory_head_blocks[i]=n;
}



void _memory_release_data(void){
	if (_memory_page_head){
		page_header_t* p=_memory_page_head;
		while (p){
			page_header_t* n=p->n;
			sll_platform_free_page(p,SLL_ROUND_PAGE(ALLOCATOR_ALLOC_SIZE));
			p=n;
		}
		_memory_page_head=NULL;
	}
#ifdef USE_STACK_ALLOCATOR
	if (_memory_stack_page){
		sll_platform_free_page(_memory_stack_page,SLL_ROUND_LARGE_PAGE(ALLOCATOR_STACK_ALLOC_SIZE));
		_memory_stack_page=NULL;
	}
#endif
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
	sz=(sz+sizeof(user_mem_block_t)+15)&0xfffffffffffffff0ull;
	if (sz<=ALLOCATOR_MAX_SMALL_SIZE){
		return _allocate_chunk(sz);
	}
	user_mem_block_t* b=malloc(sz);
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
#ifdef USE_STACK_ALLOCATOR
	if (!sz){
		return NULL;
	}
	sz=(sz+sizeof(user_mem_block_t)+15)&0xfffffffffffffff0ull;
	void* o=_allocate_chunk_stack(sz);
	if (o){
		return o;
	}
#endif
	return sll_allocate(sz);
}

;

__SLL_EXTERNAL void sll_deallocate(void* p){
	if (!p){
		return;
	}
	user_mem_block_t* b=(user_mem_block_t*)(ADDR(p)-sizeof(user_mem_block_t));
	SLL_ASSERT(b->dt&USED_BLOCK_FLAG_USED);
#ifdef USE_STACK_ALLOCATOR
	if (b->dt&USED_BLOCK_FLAG_STACK){
		sll_size_t p_sz=GET_STACK_BLOCK_PREV_SIZE(b);
		sll_size_t sz=GET_STACK_BLOCK_SIZE(b);
		b->dt&=0x3fffffffffffffffull;
		mem_stack_block_t* n=(mem_stack_block_t*)(ADDR(b)+sz);
		if (ADDR(n)<ADDR(_memory_stack_page)+SLL_ROUND_LARGE_PAGE(ALLOCATOR_STACK_ALLOC_SIZE)&&!(n->dt&USED_BLOCK_FLAG_USED)){
			mem_stack_block_t* nn=(mem_stack_block_t*)(ADDR(n)+GET_STACK_BLOCK_SIZE(n));
			if (ADDR(nn)<ADDR(_memory_stack_page)+SLL_ROUND_LARGE_PAGE(ALLOCATOR_STACK_ALLOC_SIZE)){
				UPDATE_STACK_BLOCK_PREV_SIZE(nn,GET_STACK_BLOCK_SIZE(n)+sz);
			}
			if (n==_memory_stack_top){
				_memory_stack_top=(mem_stack_block_t*)b;
			}
			sz+=GET_STACK_BLOCK_SIZE(n);
			b->dt=SET_STACK_BLOCK_SIZE(p_sz,sz);
			ASSERT_CORRECT_TOP();
		}
		if (p_sz){
			mem_stack_block_t* pn=(mem_stack_block_t*)(ADDR(b)-p_sz);
			if (!(pn->dt&USED_BLOCK_FLAG_USED)){
				p_sz+=sz;
				SLL_ASSERT(!(p_sz&15));
				UPDATE_STACK_BLOCK_SIZE(pn,p_sz);
				n=(mem_stack_block_t*)(ADDR(b)+sz);
				if (ADDR(n)<ADDR(_memory_stack_page)+SLL_ROUND_LARGE_PAGE(ALLOCATOR_STACK_ALLOC_SIZE)){
					SLL_ASSERT(n->dt&USED_BLOCK_FLAG_USED);
					UPDATE_STACK_BLOCK_PREV_SIZE(n,p_sz);
				}
				else if (((mem_stack_block_t*)b)==_memory_stack_top){
					_memory_stack_top=pn;
					ASSERT_CORRECT_TOP();
				}
			}
		}
	}
	else
#endif
	if (USED_BLOCK_GET_SIZE(b)>ALLOCATOR_MAX_SMALL_SIZE){
		free(b);
	}
	else{
		_release_chunk(b);
	}
}



__SLL_EXTERNAL void* sll_memory_move(void* p,sll_bool_t d){
#ifdef USE_STACK_ALLOCATOR
	if (!p){
		return NULL;
	}
	user_mem_block_t* b=(user_mem_block_t*)(ADDR(p)-sizeof(user_mem_block_t));
	SLL_ASSERT(b->dt&USED_BLOCK_FLAG_USED);
	void* n;
	sll_size_t sz;
	if (d==SLL_MEMORY_MOVE_DIRECTION_FROM_STACK){
		sz=GET_STACK_BLOCK_SIZE(b);
		if (!(b->dt&USED_BLOCK_FLAG_STACK)||sz>ALLOCATOR_MAX_SMALL_SIZE){
			return p;
		}
		n=_allocate_chunk(sz);
	}
	else{
		if (b->dt&USED_BLOCK_FLAG_STACK){
			return p;
		}
		sz=USED_BLOCK_GET_SIZE(b);
		n=_allocate_chunk_stack(sz);
		if (!n){
			return p;
		}
	}
	const wide_data_t* p64=(const wide_data_t*)p;
	wide_data_t* n64=(wide_data_t*)n;
	ASSUME_ALIGNED(p64,4,8);
	ASSUME_ALIGNED(n64,4,8);
	for (sll_size_t i=0;i<((sz-sizeof(user_mem_block_t))>>3);i++){
		*(n64+i)=*(p64+i);
	}
	sll_deallocate(p);
	return n;
#else
	return p;
#endif
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
#ifdef USE_STACK_ALLOCATOR
	if (b->dt&USED_BLOCK_FLAG_STACK){
		if (GET_STACK_BLOCK_SIZE(b)==sz){
			return p;
		}
		mem_stack_block_t* n=(mem_stack_block_t*)(ADDR(b)+GET_STACK_BLOCK_SIZE(b));
		if (GET_STACK_BLOCK_SIZE(b)>sz){
			sll_size_t off=GET_STACK_BLOCK_SIZE(b)-sz;
			SLL_ASSERT(!(off&15));
			if (ADDR(n)<ADDR(_memory_stack_page)+SLL_ROUND_LARGE_PAGE(ALLOCATOR_STACK_ALLOC_SIZE)){
				if (n->dt&USED_BLOCK_FLAG_USED){
					SLL_UNIMPLEMENTED();
				}
				else{
					mem_stack_block_t* nn=(mem_stack_block_t*)(ADDR(n)+GET_STACK_BLOCK_SIZE(n));
					if (ADDR(nn)<ADDR(_memory_stack_page)+SLL_ROUND_LARGE_PAGE(ALLOCATOR_STACK_ALLOC_SIZE)){
						UPDATE_STACK_BLOCK_PREV_SIZE(nn,GET_STACK_BLOCK_SIZE(n)+off);
					}
					nn=(mem_stack_block_t*)(ADDR(n)-off);
					nn->dt=SET_STACK_BLOCK_SIZE(sz,GET_STACK_BLOCK_SIZE(n)+off);
				}
			}
			else{
				SLL_UNIMPLEMENTED();
			}
			UPDATE_STACK_BLOCK_SIZE(b,sz);
			return p;
		}
		if (ADDR(n)<ADDR(_memory_stack_page)+SLL_ROUND_LARGE_PAGE(ALLOCATOR_STACK_ALLOC_SIZE)&&!(n->dt&USED_BLOCK_FLAG_USED)&&sz-GET_STACK_BLOCK_SIZE(b)<=GET_STACK_BLOCK_SIZE(n)){
			sll_size_t n_sz=GET_STACK_BLOCK_SIZE(n)-sz+GET_STACK_BLOCK_SIZE(b);
			UPDATE_STACK_BLOCK_SIZE(b,sz);
			mem_stack_block_t* nn=(mem_stack_block_t*)(ADDR(b)+sz);
			if (ADDR(nn)>=ADDR(_memory_stack_page)+SLL_ROUND_LARGE_PAGE(ALLOCATOR_STACK_ALLOC_SIZE)){
				SLL_ASSERT(n==_memory_stack_top);
				_memory_stack_top=NULL;
			}
			else{
				SLL_ASSERT(!(n_sz&15));
				nn->dt=SET_STACK_BLOCK_SIZE(sz,n_sz);
				if (n==_memory_stack_top){
					_memory_stack_top=nn;
				}
				mem_stack_block_t* nnn=(mem_stack_block_t*)(ADDR(nn)+n_sz);
				if (ADDR(nnn)<ADDR(_memory_stack_page)+SLL_ROUND_LARGE_PAGE(ALLOCATOR_STACK_ALLOC_SIZE)){
					UPDATE_STACK_BLOCK_PREV_SIZE(nnn,n_sz);
				}
			}
			ASSERT_CORRECT_TOP();
			return p;
		}
		void* o=sll_allocate_stack(sz);
		const wide_data_t* s=(const wide_data_t*)p;
		wide_data_t* d=(wide_data_t*)n;
		sll_size_t i=((USED_BLOCK_GET_SIZE(b)>sz?sz:USED_BLOCK_GET_SIZE(b))-sizeof(user_mem_block_t))>>3;
		do{
			*d=*s;
			s++;
			d++;
			i--;
		} while (i);
		sll_deallocate(p);
		return o;
	}
#endif
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
			n=PTR(ADDR(blk)+sizeof(user_mem_block_t));
		}
		const wide_data_t* s=(const wide_data_t*)p;
		wide_data_t* d=(wide_data_t*)n;
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
		const wide_data_t* s=(const wide_data_t*)p;
		wide_data_t* d=(wide_data_t*)n;
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
