#include <sll/_sll_internal.h>
#include <sll/common.h>
#include <sll/constants.h>
#include <sll/platform.h>
#include <sll/string.h>
#include <sll/types.h>
#include <stdint.h>
#include <stdlib.h>



static void* _gc_page_ptr=NULL;
static sll_runtime_object_t* _gc_next_object=NULL;
#ifdef DEBUG_BUILD
static uint64_t _gc_alloc=0;
static uint64_t _gc_dealloc=0;
#endif



void _gc_free_pages(void){
#ifdef DEBUG_BUILD
	SLL_ASSERT(_gc_alloc==_gc_dealloc);
#endif
	sll_page_size_t sz=sll_platform_get_page_size();
	void* c=_gc_page_ptr;
	while (c){
		void* n=*((void**)c);
		sll_platform_free_page(c,sz*(n?1:GC_INIT_PAGE_COUNT));
		c=n;
	}
}



void _gc_init(void){
	SLL_ASSERT(!_gc_page_ptr);
	SLL_ASSERT(!_gc_next_object);
	sll_page_size_t sz=GC_INIT_PAGE_COUNT*sll_platform_get_page_size();
	_gc_page_ptr=sll_platform_allocate_page(sz);
	*((void**)_gc_page_ptr)=NULL;
	sll_runtime_object_t* c=(sll_runtime_object_t*)((uint64_t)_gc_page_ptr+sizeof(void*));
	_gc_next_object=c;
	void* e=(void*)((uint64_t)_gc_page_ptr+sizeof(void*)+(sz/sizeof(sll_runtime_object_t)-1)*sizeof(sll_runtime_object_t));
	while ((void*)c<e){
		GC_SET_NEXT_OBJECT(c,c+1);
		c->rc=1;
		c++;
	}
	GC_SET_NEXT_OBJECT(c,NULL);
	c->rc=1;
}



__SLL_FUNC sll_runtime_object_t* sll_create_object(void){
	if (!_gc_next_object){
		sll_page_size_t sz=sll_platform_get_page_size();
		void* pg=sll_platform_allocate_page(sz);
		*((void**)pg)=_gc_page_ptr;
		_gc_page_ptr=pg;
		sll_runtime_object_t* c=(sll_runtime_object_t*)((uint64_t)pg+sizeof(void*));
		_gc_next_object=c;
		void* e=(void*)((uint64_t)pg+sizeof(void*)+(sz/sizeof(sll_runtime_object_t)-1)*sizeof(sll_runtime_object_t));
		while ((void*)c<e){
			GC_SET_NEXT_OBJECT(c,c+1);
			c->rc=1;
			c++;
		}
		GC_SET_NEXT_OBJECT(c,NULL);
		c->rc=1;
	}
	sll_runtime_object_t* o=_gc_next_object;
	_gc_next_object=GC_GET_NEXT_OBJECT(o);
#ifdef DEBUG_BUILD
	_gc_alloc++;
#endif
	return o;
}



__SLL_FUNC void sll_release_object(sll_runtime_object_t* o){
	SLL_ASSERT(o->rc);
	if (o->rc==1){
		if (o->t==SLL_RUNTIME_OBJECT_TYPE_STRING){
			free(o->dt.s.v);
		}
		else if (o->t==SLL_RUNTIME_OBJECT_TYPE_ARRAY){
			for (sll_array_length_t i=0;i<o->dt.a.l;i++){
				sll_release_object(*(o->dt.a.v+i));
			}
			free(o->dt.a.v);
		}
		GC_SET_NEXT_OBJECT(o,_gc_next_object);
		_gc_next_object=o;
#ifdef DEBUG_BUILD
		_gc_dealloc++;
#endif
	}
	else{
		o->rc--;
	}
}
