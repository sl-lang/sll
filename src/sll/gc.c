#include <sll/_internal/common.h>
#include <sll/_internal/gc.h>
#include <sll/_internal/scheduler.h>
#include <sll/_internal/weakref.h>
#include <sll/api/string.h>
#include <sll/array.h>
#include <sll/common.h>
#include <sll/data.h>
#include <sll/gc.h>
#include <sll/map.h>
#include <sll/memory.h>
#include <sll/object.h>
#include <sll/platform/memory.h>
#include <sll/string.h>
#include <sll/thread.h>
#include <sll/types.h>
#include <sll/vm.h>



static gc_page_header_t* _gc_page_ptr=NULL;
static sll_object_t* _gc_next_object=NULL;
static sll_size_t _gc_alloc=0;
static sll_size_t _gc_dealloc=0;
static void* _gc_page_pool[GC_PAGE_POOL_SIZE];
static sll_array_length_t _gc_page_pool_len=0;



void _gc_release_data(void){
	SLL_ASSERT(_gc_alloc==_gc_dealloc);
	while (_gc_page_ptr){
		gc_page_header_t* n=_gc_page_ptr->n;
		sll_platform_free_page(_gc_page_ptr,SLL_PAGE_SIZE);
		_gc_page_ptr=n;
	}
	while (_gc_page_pool_len){
		_gc_page_pool_len--;
		sll_platform_free_page(_gc_page_pool[_gc_page_pool_len],SLL_PAGE_SIZE);
	}
	_gc_next_object=NULL;
	_gc_alloc=0;
	_gc_dealloc=0;
}



__SLL_EXTERNAL void sll_acquire_object(sll_object_t* o){
	GC_LOCK(o);
	o->rc++;
	GC_UNLOCK(o);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_create_object(sll_object_type_t t){
	if (!_gc_next_object){
		gc_page_header_t* pg;
		if (!_gc_page_pool_len){
			sll_array_length_t i=(_gc_page_ptr?GC_PAGE_DYNAMIC_POOL_SIZE:GC_PAGE_INIT_POOL_SIZE);
			while (i){
				i--;
				_gc_page_pool[_gc_page_pool_len]=sll_platform_allocate_page(SLL_PAGE_SIZE,0);
				_gc_page_pool_len++;
			}
		}
		_gc_page_pool_len--;
		pg=_gc_page_pool[_gc_page_pool_len];
		if (_gc_page_ptr){
			_gc_page_ptr->p=pg;
		}
		pg->p=NULL;
		pg->n=_gc_page_ptr;
		pg->cnt=0;
		_gc_page_ptr=pg;
		sll_object_t* c=(sll_object_t*)(ADDR(pg)+sizeof(gc_page_header_t));
		c->dt._ptr.p=NULL;
		_gc_next_object=c;
		void* e=PTR(ADDR(pg)+sizeof(gc_page_header_t)+((SLL_PAGE_SIZE-sizeof(gc_page_header_t))/sizeof(sll_object_t)-1)*sizeof(sll_object_t));
		while (PTR(c)<e){
			c->dt._ptr.n=c+1;
			c->rc=0;
			c++;
			c->dt._ptr.p=c-1;
		}
		c->dt._ptr.n=NULL;
		c->rc=0;
	}
	sll_object_t* o=_gc_next_object;
	_gc_next_object=o->dt._ptr.n;
	if (_gc_next_object){
		_gc_next_object->dt._ptr.p=NULL;
	}
	((gc_page_header_t*)PTR(ADDR(o)&0xfffffffffffff000ull))->cnt++;
	o->rc=1;
	*((sll_object_type_t*)(&(o->t)))=t;
	_ATOMIC_STORE((sll_object_type_t*)(&(o->_f)),0);
	_gc_alloc++;
	return o;
}



__SLL_EXTERNAL void sll_lock_object(sll_object_t* o){
	GC_LOCK(o);
}



__SLL_EXTERNAL void sll_release_object(sll_object_t* o){
	GC_LOCK(o);
	SLL_ASSERT(o->rc);
	o->rc--;
	if (o->rc){
		GC_UNLOCK(o);
		return;
	}
	if (o->_f&GC_FLAG_HAS_WEAKREF){
		o->rc++;
		GC_UNLOCK(o);
		_weakref_delete(o);
		GC_LOCK(o);
		o->rc--;
		if (o->rc){
			GC_UNLOCK(o);
			return;
		}
	}
	if (o->t==SLL_OBJECT_TYPE_STRING){
		sll_free_string(&(o->dt.s));
	}
	else if (o->t==SLL_OBJECT_TYPE_ARRAY||o->t==SLL_OBJECT_TYPE_MAP_KEYS||o->t==SLL_OBJECT_TYPE_MAP_VALUES){
		sll_free_array(&(o->dt.a));
	}
	else if (o->t==SLL_OBJECT_TYPE_MAP){
		sll_free_map(&(o->dt.m));
	}
	else if (o->t>SLL_MAX_OBJECT_TYPE&&o->t<SLL_OBJECT_TYPE_OBJECT){
		if (sll_current_runtime_data&&o->t<=sll_current_runtime_data->tt->l+SLL_MAX_OBJECT_TYPE){
			const sll_object_type_data_t* dt=*(sll_current_runtime_data->tt->dt+o->t-SLL_MAX_OBJECT_TYPE-1);
			if (_scheduler_current_thread_index!=SLL_UNKNOWN_THREAD_INDEX&&dt->fn.del){
				o->rc++;
				GC_UNLOCK(o);
				sll_release_object(sll_execute_function(dt->fn.del,&o,1,0));
				GC_LOCK(o);
				o->rc--;
				if (o->rc){
					GC_UNLOCK(o);
					return;
				}
			}
			sll_object_field_t* p=o->dt.p;
			for (sll_arg_count_t i=0;i<dt->l;i++){
				if (dt->dt[i].t>SLL_OBJECT_TYPE_CHAR){
					sll_release_object(p->o);
				}
				p++;
			}
		}
		sll_deallocate(o->dt.p);
	}
	*((sll_object_type_t*)(&(o->t)))=SLL_OBJECT_TYPE_INT;
	o->dt.i=0;
	_ATOMIC_STORE((sll_object_type_t*)(&(o->_f)),0);
	o->dt._ptr.n=_gc_next_object;
	o->dt._ptr.p=NULL;
	if (_gc_next_object){
		_gc_next_object->dt._ptr.p=o;
	}
	_gc_next_object=o;
	_gc_dealloc++;
	gc_page_header_t* pg=PTR(ADDR(o)&0xfffffffffffff000ull);
	pg->cnt--;
	if (pg->cnt){
		return;
	}
	while ((ADDR(_gc_next_object)&0xfffffffffffff000ull)==ADDR(pg)){
		_gc_next_object=_gc_next_object->dt._ptr.n;
	}
	if (pg->p){
		pg->p->n=pg->n;
		if (_gc_page_ptr==pg){
			_gc_page_ptr=pg->p;
		}
	}
	if (pg->n){
		pg->n->p=pg->p;
		if (_gc_page_ptr==pg){
			_gc_page_ptr=pg->n;
		}
	}
	if (_gc_page_ptr==pg){
		_gc_page_ptr=NULL;
	}
	sll_object_t* c=(sll_object_t*)(ADDR(pg)+sizeof(gc_page_header_t));
	void* e=PTR(ADDR(pg)+sizeof(gc_page_header_t)+(SLL_PAGE_SIZE-sizeof(gc_page_header_t))/sizeof(sll_object_t)*sizeof(sll_object_t));
	while (PTR(c)<e){
		SLL_ASSERT(c!=_gc_next_object);
		if (c->dt._ptr.p){
			c->dt._ptr.p->dt._ptr.n=c->dt._ptr.n;
		}
		if (c->dt._ptr.n){
			c->dt._ptr.n->dt._ptr.p=c->dt._ptr.p;
		}
		c++;
	}
	if (_gc_page_pool_len<GC_PAGE_POOL_SIZE){
		_gc_page_pool[_gc_page_pool_len]=pg;
		_gc_page_pool_len++;
	}
	else{
		sll_platform_free_page(pg,SLL_PAGE_SIZE);
	}
}



__SLL_EXTERNAL void sll_unlock_object(sll_object_t* o){
	GC_UNLOCK(o);
}
