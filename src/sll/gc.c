#include <sll/api/string.h>
#include <sll/array.h>
#include <sll/common.h>
#include <sll/data.h>
#include <sll/file.h>
#include <sll/gc.h>
#include <sll/internal/common.h>
#include <sll/internal/gc.h>
#include <sll/map.h>
#include <sll/memory.h>
#include <sll/object.h>
#include <sll/platform/memory.h>
#include <sll/string.h>
#include <sll/types.h>
#include <sll/vm.h>



static void* _gc_page_ptr=NULL;
static sll_object_t* _gc_next_object=NULL;
static sll_ref_count_t _gc_alloc=0;
static sll_ref_count_t _gc_dealloc=0;



void _gc_release_data(void){
	if (!_gc_page_ptr){
		return;
	}
	SLL_ASSERT(_gc_alloc==_gc_dealloc);
	void* c=_gc_page_ptr;
	while (c){
		void* n=*((void**)c);
		sll_platform_free_page(c,SLL_ROUND_PAGE(GC_OBJECT_POOL_ALLOC_SIZE));
		c=n;
	}
	_gc_page_ptr=NULL;
}



__SLL_EXTERNAL void sll_acquire_object(sll_object_t* o){
	o->rc++;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_create_object(void){
	if (!_gc_next_object){
		void* pg=sll_platform_allocate_page(SLL_ROUND_PAGE(GC_OBJECT_POOL_ALLOC_SIZE),0);
		*((void**)pg)=_gc_page_ptr;
		_gc_page_ptr=pg;
		sll_object_t* c=(sll_object_t*)(ADDR(pg)+sizeof(void*));
		_gc_next_object=c;
		void* e=PTR(ADDR(pg)+sizeof(void*)+((SLL_ROUND_PAGE(GC_OBJECT_POOL_ALLOC_SIZE)-sizeof(void*))/sizeof(sll_object_t)-1)*sizeof(sll_object_t));
		while (PTR(c)<e){
			GC_SET_NEXT_OBJECT(c,c+1);
			c->rc=0;
			c++;
		}
		GC_SET_NEXT_OBJECT(c,NULL);
		c->rc=0;
	}
	sll_object_t* o=_gc_next_object;
	_gc_next_object=GC_GET_NEXT_OBJECT(o);
	o->rc=1;
	_gc_alloc++;
	return o;
}



__SLL_EXTERNAL void sll_release_object(sll_object_t* o){
	SLL_ASSERT(o->rc);
	o->rc--;
	if (!o->rc){
		if (SLL_OBJECT_GET_TYPE(o)==SLL_OBJECT_TYPE_STRING){
			sll_free_string(&(o->dt.s));
		}
		else if (SLL_OBJECT_GET_TYPE(o)==SLL_OBJECT_TYPE_ARRAY||SLL_OBJECT_GET_TYPE(o)==SLL_OBJECT_TYPE_MAP_KEYS||SLL_OBJECT_GET_TYPE(o)==SLL_OBJECT_TYPE_MAP_VALUES){
			sll_free_array(&(o->dt.a));
		}
		else if (SLL_OBJECT_GET_TYPE(o)==SLL_OBJECT_TYPE_MAP){
			sll_free_map(&(o->dt.m));
		}
		else if (SLL_OBJECT_GET_TYPE(o)>SLL_MAX_OBJECT_TYPE&&SLL_OBJECT_GET_TYPE(o)<SLL_OBJECT_TYPE_OBJECT){
			if (sll_current_runtime_data&&SLL_OBJECT_GET_TYPE(o)<=sll_current_runtime_data->tt->l+SLL_MAX_OBJECT_TYPE){
				const sll_object_type_data_t* dt=*(sll_current_runtime_data->tt->dt+SLL_OBJECT_GET_TYPE(o)-SLL_MAX_OBJECT_TYPE-1);
				if (dt->fn.del){
					o->rc++;
					sll_release_object(sll_execute_function(dt->fn.del,&o,1));
					o->rc--;
					if (o->rc){
						return;
					}
				}
				sll_object_field_t* p=o->dt.p;
				for (sll_arg_count_t i=0;i<dt->l;i++){
					if (SLL_OBJECT_GET_TYPE_MASK(dt->dt[i].t)>SLL_OBJECT_TYPE_CHAR){
						sll_release_object(p->o);
					}
					p++;
				}
			}
			sll_deallocate(o->dt.p);
		}
		o->t=SLL_OBJECT_TYPE_INT;
		o->dt.i=0;
		GC_SET_NEXT_OBJECT(o,_gc_next_object);
		_gc_next_object=o;
		_gc_dealloc++;
	}
}
