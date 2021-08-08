#include <sll/_sll_internal.h>
#include <sll/common.h>
#include <sll/platform.h>
#include <sll/string.h>
#include <sll/types.h>
#include <stdint.h>
#include <stdlib.h>



void* _string_heap_page_ptr=NULL;
memory_node_t* _string_heap_node=NULL;
uint64_t _pg_sz=0;



void _free_string_heap(void){
	void* c=_string_heap_page_ptr;
	while (c){
		page_header_t* h=(page_header_t*)c;
		void* n=h->p;
		sll_platform_free_page(c,h->sz);
		c=n;
	}
}



__SLL_FUNC void sll_reset_heap(void){
	_free_string_heap();
	_string_heap_page_ptr=NULL;
	_string_heap_node=NULL;
}



__SLL_FUNC sll_string_t* sll_string_create(sll_string_length_t l){
	uint64_t sz=ALIGN(sizeof(sll_string_t)+(l+1)*sizeof(sll_char_t));
	if (sz<sizeof(memory_node_t)){
		sz=sizeof(memory_node_t);
	}
	CORRECT_ALIGNMENT(sz);
	memory_node_t* n=_string_heap_node;
	while (n){
		if (MEMORY_NODE_GET_SIZE(n)>sz){
			break;
		}
		n=n->n;
	}
	if (!n){
		if (!_pg_sz){
			atexit(_free_string_heap);
			_pg_sz=sll_platform_get_page_size();
		}
		sll_page_size_t pg_sz=(sz+sizeof(page_header_t)+sizeof(uint64_t)+_pg_sz-1)/_pg_sz*_pg_sz;
		void* pg=sll_platform_allocate_page(pg_sz);
		page_header_t* pg_h=(page_header_t*)pg;
		pg_h->p=_string_heap_page_ptr;
		pg_h->sz=pg_sz;
		n=(memory_node_t*)((uint64_t)pg+sizeof(page_header_t));
		CORRECT_ALIGNMENT(n);
		n->sz=(pg_sz-sizeof(page_header_t)-sizeof(uint64_t))|MEMORY_NODE_SIGNATURE;
		n->p=NULL;
		n->n=_string_heap_node;
		if (n->n){
			n->n->p=n;
		}
		((memory_node_t*)((uint64_t)pg+pg_sz-sizeof(uint64_t)))->sz=0;
		_string_heap_page_ptr=pg;
		_string_heap_node=n;
	}
	uint8_t p=0;
	if (MEMORY_NODE_GET_SIZE(n)-sz>=sizeof(memory_node_t)){
		memory_node_t* nn=(memory_node_t*)((uint64_t)n+sz);
		CORRECT_ALIGNMENT(nn);
		nn->sz=(MEMORY_NODE_GET_SIZE(n)-sz)|MEMORY_NODE_SIGNATURE;
		nn->p=n->p;
		nn->n=n->n;
		if (!nn->p){
			_string_heap_node=nn;
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
		ASSERT(p<sizeof(memory_node_t));
		sz=MEMORY_NODE_GET_SIZE(n);
		if (!n->p){
			_string_heap_node=n->n;
		}
		else{
			n->p->n=n->n;
		}
		if (n->n){
			n->n->p=n->p;
		}
	}
	sll_string_t* o=(sll_string_t*)n;
	*((sll_string_length_t*)(&(o->l)))=l;
	o->rc=0;
	*((sll_heap_data_t*)(&(o->h)))=HEAP_SIGNATURE|(p<<8);
	o->c=0;
	return o;
}



__SLL_FUNC void sll_string_release(sll_string_t* s){
	s->rc--;
	if (s->rc){
		return;
	}
	ASSERT(GET_HEAP_SIGNATURE(s)==HEAP_SIGNATURE);
	uint64_t sz=ALIGN(sizeof(sll_string_t)+(s->l+1)*sizeof(sll_char_t));
	if (sz<sizeof(memory_node_t)){
		sz=sizeof(memory_node_t);
	}
	sz+=GET_PADDING(s);
	memory_node_t* n=(memory_node_t*)s;
	n->sz=sz;
	memory_node_t* nn=(memory_node_t*)((uint64_t)n+n->sz);
	if (!(nn->sz&MEMORY_NODE_SIGNATURE)){
		n->p=NULL;
		n->n=_string_heap_node;
		_string_heap_node=n;
	}
	else{
		n->sz+=MEMORY_NODE_GET_SIZE(nn);
		n->p=nn->p;
		n->n=nn->n;
		if (!n->p){
			ASSERT(_string_heap_node==nn);
			_string_heap_node=n;
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
