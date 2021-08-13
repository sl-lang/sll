#include <sll/_sll_internal.h>
#include <sll/common.h>
#include <sll/platform.h>
#include <sll/string.h>
#include <sll/types.h>
#include <stdint.h>
#include <stdlib.h>



void* _allocator_page_ptr=NULL;
memory_node_t* _allocator_heap_node=NULL;
uint64_t _pg_sz=0;



void _free_allocator(void){
	void* c=_allocator_page_ptr;
	while (c){
		page_header_t* h=(page_header_t*)c;
		void* n=h->p;
		sll_platform_free_page(c,h->sz);
		c=n;
	}
}



__SLL_FUNC void sll_reset_allocator(void){
	_free_allocator();
	_allocator_page_ptr=NULL;
	_allocator_heap_node=NULL;
}



__SLL_FUNC sll_allocated_block_t* sll_allocate_raw(sll_allocation_size_t sz){
	sz=ALIGN(sz);
	if (sz<sizeof(memory_node_t)){
		sz=sizeof(memory_node_t);
	}
	CORRECT_ALIGNMENT(sz);
	memory_node_t* n=_allocator_heap_node;
	while (n){
		if (MEMORY_NODE_GET_SIZE(n)>sz){
			break;
		}
		n=n->n;
	}
	if (!n){
		if (!_pg_sz){
			atexit(_free_allocator);
			_pg_sz=sll_platform_get_page_size();
		}
		sll_page_size_t pg_sz=(sz+sizeof(page_header_t)+sizeof(uint64_t)+_pg_sz-1)/_pg_sz*_pg_sz;
		void* pg=sll_platform_allocate_page(pg_sz);
		page_header_t* pg_h=(page_header_t*)pg;
		pg_h->p=_allocator_page_ptr;
		pg_h->sz=pg_sz;
		n=(memory_node_t*)((uint64_t)pg+sizeof(page_header_t));
		CORRECT_ALIGNMENT(n);
		n->sz=(pg_sz-sizeof(page_header_t)-sizeof(uint64_t))|MEMORY_NODE_SIGNATURE;
		n->p=NULL;
		n->n=_allocator_heap_node;
		if (n->n){
			n->n->p=n;
		}
		((memory_node_t*)((uint64_t)pg+pg_sz-sizeof(uint64_t)))->sz=0;
		_allocator_page_ptr=pg;
		_allocator_heap_node=n;
	}
	uint8_t p=0;
	if (MEMORY_NODE_GET_SIZE(n)-sz>=sizeof(memory_node_t)){
		memory_node_t* nn=(memory_node_t*)((uint64_t)n+sz);
		CORRECT_ALIGNMENT(nn);
		nn->sz=(MEMORY_NODE_GET_SIZE(n)-sz)|MEMORY_NODE_SIGNATURE;
		nn->p=n->p;
		nn->n=n->n;
		if (!nn->p){
			_allocator_heap_node=nn;
		}
		else{
			nn->p->n=nn;
		}
		if (nn->n){
			nn->n->p=nn;
		}
	}
	else{
		p=(uint8_t)(MEMORY_NODE_GET_SIZE(n)-sz);
		SLL_ASSERT(p<sizeof(memory_node_t));
		sz=MEMORY_NODE_GET_SIZE(n);
		if (!n->p){
			_allocator_heap_node=n->n;
		}
		else{
			n->p->n=n->n;
		}
		if (n->n){
			n->n->p=n->p;
		}
	}
	sll_allocated_block_t* o=(sll_allocated_block_t*)n;
	o->rc=0;
	*((sll_heap_data_t*)(&(o->h)))=HEAP_SIGNATURE|(p<<8);
	return o;
}



__SLL_FUNC void sll_deallocate_raw(sll_allocated_block_t* b,sll_allocation_size_t sz){
	SLL_ASSERT(!b->rc);
	SLL_ASSERT(GET_HEAP_SIGNATURE(b)==HEAP_SIGNATURE);
	sz=ALIGN(sz);
	if (sz<sizeof(memory_node_t)){
		sz=sizeof(memory_node_t);
	}
	sz+=GET_PADDING(b);
	memory_node_t* n=(memory_node_t*)b;
	n->sz=sz;
	memory_node_t* nn=(memory_node_t*)((uint64_t)n+n->sz);
	if (!(nn->sz&MEMORY_NODE_SIGNATURE)){
		n->p=NULL;
		n->n=_allocator_heap_node;
		_allocator_heap_node=n;
	}
	else{
		n->sz+=MEMORY_NODE_GET_SIZE(nn);
		n->p=nn->p;
		n->n=nn->n;
		if (!n->p){
			SLL_ASSERT(_allocator_heap_node==nn);
			_allocator_heap_node=n;
		}
		else{
			n->p->n=n;
		}
	}
	if (n->n){
		n->n->p=n;
	}
	n->sz|=MEMORY_NODE_SIGNATURE;
}
