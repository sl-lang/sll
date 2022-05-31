#include <sll/_internal/common.h>
#include <sll/_internal/gc.h>
#include <sll/_internal/platform.h>
#include <sll/_internal/scheduler.h>
#include <sll/_internal/weakref.h>
#include <sll/api/string.h>
#include <sll/array.h>
#include <sll/common.h>
#include <sll/data.h>
#include <sll/file.h>
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
static void* _gc_page_pool[GC_PAGE_POOL_SIZE];
static sll_array_length_t _gc_page_pool_len=0;
static sll_object_t* _gc_object_pool[GC_OBJECT_POOL_SIZE];
static sll_array_length_t _gc_object_pool_len=0;
static gc_fast_object_pool_t _gc_fast_object_pool={
	.read=0,
	.write=0,
	.space=GC_FAST_OBJECT_POOL_SIZE
};
static gc_root_data_t _gc_root_data={
	NULL
};



void _gc_release_data(void){
	SLL_ASSERT(!_gc_root_data.single_root);
	while (_gc_fast_object_pool.space!=GC_FAST_OBJECT_POOL_SIZE){
		GC_PAGE_HEADER_DECREASE(GC_MEMORY_PAGE_HEADER(_gc_fast_object_pool.data[_gc_fast_object_pool.read]));
		_gc_fast_object_pool.read=(_gc_fast_object_pool.read+1)&(GC_FAST_OBJECT_POOL_SIZE-1);
		_gc_fast_object_pool.space++;
	}
	SLL_ASSERT(_gc_fast_object_pool.read==_gc_fast_object_pool.write);
	sll_bool_t err=0;
	while (_gc_page_ptr){
		sll_object_t* c=(sll_object_t*)(ADDR(_gc_page_ptr)+sizeof(gc_page_header_t));
		if (_gc_page_ptr->cnt){
			void* e=PTR(ADDR(_gc_page_ptr)+sizeof(gc_page_header_t)+(GC_MEMORY_PAGE_SIZE-sizeof(gc_page_header_t))/sizeof(sll_object_t)*sizeof(sll_object_t));
			while (PTR(c)<e){
				if (c->rc){
					err=1;
					sll_file_write_format(sll_stderr,SLL_CHAR("[%p]: "),NULL,c);
					sll_string_t str;
					sll_api_string_convert(&c,1,&str);
					sll_file_write(sll_stderr,str.data,str.length*sizeof(sll_char_t),NULL);
					sll_free_string(&str);
					sll_file_write_char(sll_stderr,'\n',NULL);
				}
				c++;
			}
		}
		gc_page_header_t* nxt=_gc_page_ptr->next;
		SLL_CRITICAL_ERROR(sll_platform_free_page(_gc_page_ptr,GC_MEMORY_PAGE_SIZE));
		_gc_page_ptr=nxt;
	}
	if (err){
		_force_exit_platform();
	}
	_gc_next_object=NULL;
	_gc_page_pool_len=0;
	_gc_object_pool_len=0;
}



__SLL_EXTERNAL void sll__gc_error(sll_object_t* object){
	SLL_CRITICAL_ERROR(object->rc);
}



__SLL_EXTERNAL void sll__release_object_internal(sll_object_t* object){
	SLL_ASSERT(!object->rc);
	if (object->_flags&GC_FLAG_HAS_WEAKREF){
		object->rc++;
		_weakref_delete(object);
		object->rc--;
		if (object->rc){
			return;
		}
	}
	if (object->type==SLL_OBJECT_TYPE_STRING){
		sll_free_string(&(object->data.string));
	}
	else if (object->type==SLL_OBJECT_TYPE_ARRAY||object->type==SLL_OBJECT_TYPE_MAP_KEYS||object->type==SLL_OBJECT_TYPE_MAP_VALUES){
		sll_free_array(&(object->data.array));
	}
	else if (object->type==SLL_OBJECT_TYPE_MAP){
		sll_free_map(&(object->data.map));
	}
	else if (object->type>SLL_MAX_OBJECT_TYPE&&object->type<SLL_OBJECT_TYPE_OBJECT){
		if (sll_current_runtime_data&&object->type<=sll_current_runtime_data->type_table->length+SLL_MAX_OBJECT_TYPE){
			const sll_object_type_data_t* dt=*(sll_current_runtime_data->type_table->data+object->type-SLL_MAX_OBJECT_TYPE-1);
			if (_scheduler_current_thread_index!=SLL_UNKNOWN_THREAD_INDEX&&dt->functions[SLL_OBJECT_FUNC_DELETE]){
				object->rc++;
				SLL_RELEASE(sll_execute_function(dt->functions[SLL_OBJECT_FUNC_DELETE],&object,1,0));
				object->rc--;
				if (object->rc){
					return;
				}
			}
			sll_object_field_t* p=object->data.fields;
			for (sll_arg_count_t i=0;i<dt->field_count;i++){
				if (dt->fields[i].type>SLL_OBJECT_TYPE_CHAR){
					SLL_RELEASE(p->any);
				}
				p++;
			}
		}
		sll_deallocate(object->data.fields);
	}
	if (object->_data){
		SLL_UNIMPLEMENTED();
	}
	object->_flags=0;
	if (_gc_fast_object_pool.space){
		_gc_fast_object_pool.data[_gc_fast_object_pool.write]=object;
		_gc_fast_object_pool.write=(_gc_fast_object_pool.write+1)&(GC_FAST_OBJECT_POOL_SIZE-1);
		_gc_fast_object_pool.space--;
		return;
	}
	gc_page_header_t* pg=GC_MEMORY_PAGE_HEADER(object);
	GC_PAGE_HEADER_DECREASE(pg);
	if (!GC_PAGE_HEADER_CAN_DELETE(pg)){
		if (_gc_object_pool_len<GC_OBJECT_POOL_SIZE){
			object->_flags=GC_FLAG_IN_FAST_POOL;
			object->data._pool_index=_gc_object_pool_len;
			_gc_object_pool[_gc_object_pool_len]=object;
			_gc_object_pool_len++;
			return;
		}
		object->data._next_object.next=_gc_next_object;
		object->data._next_object.prev=NULL;
		if (_gc_next_object){
			_gc_next_object->data._next_object.prev=object;
		}
		_gc_next_object=object;
		return;
	}
	object->data._next_object.next=NULL;
	object->data._next_object.prev=NULL;
	while (GC_MEMORY_PAGE_HEADER(_gc_next_object)==pg){
		_gc_next_object=_gc_next_object->data._next_object.next;
	}
	sll_object_t* c=(sll_object_t*)(ADDR(pg)+sizeof(gc_page_header_t));
	addr_t e=ADDR(pg)+sizeof(gc_page_header_t)+(GC_MEMORY_PAGE_SIZE-sizeof(gc_page_header_t))/sizeof(sll_object_t)*sizeof(sll_object_t);
	sll_bool_t pool_shift=0;
	do{
		SLL_ASSERT(c!=_gc_next_object);
		if (c->_flags&GC_FLAG_IN_FAST_POOL){
			_gc_object_pool[c->data._pool_index]=NULL;
			pool_shift=1;
		}
		else{
			if (c->data._next_object.prev){
				c->data._next_object.prev->data._next_object.next=c->data._next_object.next;
			}
			if (c->data._next_object.next){
				c->data._next_object.next->data._next_object.prev=c->data._next_object.prev;
			}
		}
		c++;
	} while (ADDR(c)<e);
	if (_gc_page_pool_len<GC_PAGE_POOL_SIZE){
		_gc_page_pool[_gc_page_pool_len]=pg;
		_gc_page_pool_len++;
	}
	else{
		if (pg->prev){
			pg->prev->next=pg->next;
			if (_gc_page_ptr==pg){
				_gc_page_ptr=pg->prev;
			}
		}
		if (pg->next){
			pg->next->prev=pg->prev;
			if (_gc_page_ptr==pg){
				_gc_page_ptr=pg->next;
			}
		}
		if (_gc_page_ptr==pg){
			_gc_page_ptr=pg->next;
		}
		SLL_CRITICAL_ERROR(sll_platform_free_page(pg,GC_MEMORY_PAGE_SIZE));
	}
	if (!pool_shift){
		return;
	}
	sll_array_length_t i=0;
	for (sll_array_length_t j=0;j<_gc_object_pool_len;j++){
		if (_gc_object_pool[j]){
			_gc_object_pool[i]=_gc_object_pool[j];
			_gc_object_pool[i]->data._pool_index=i;
			i++;
		}
	}
	_gc_object_pool_len=i;
}



__SLL_EXTERNAL void sll_acquire_object(sll_object_t* object){
	object->rc++;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_create_object(sll_object_type_t type){
	sll_object_t* o;
	if (_gc_fast_object_pool.space!=GC_FAST_OBJECT_POOL_SIZE){
		o=_gc_fast_object_pool.data[_gc_fast_object_pool.read];
		_gc_fast_object_pool.read=(_gc_fast_object_pool.read+1)&(GC_FAST_OBJECT_POOL_SIZE-1);
		_gc_fast_object_pool.space++;
	}
	else{
		if (_gc_object_pool_len){
			_gc_object_pool_len--;
			o=_gc_object_pool[_gc_object_pool_len];
		}
		else if (_gc_next_object){
			o=_gc_next_object;
			_gc_next_object=o->data._next_object.next;
			if (_gc_next_object){
				_gc_next_object->data._next_object.prev=NULL;
			}
		}
		else{
			if (!_gc_page_pool_len){
				sll_array_length_t i=(_gc_page_ptr?GC_PAGE_DYNAMIC_POOL_SIZE:GC_PAGE_INIT_POOL_SIZE);
				while (i){
					i--;
					gc_page_header_t* pg=sll_platform_allocate_page_aligned(GC_MEMORY_PAGE_SIZE,GC_MEMORY_PAGE_SIZE,NULL);
					if (_gc_page_ptr){
						_gc_page_ptr->prev=pg;
					}
					pg->prev=NULL;
					pg->next=_gc_page_ptr;
					pg->cnt=0;
					_gc_page_ptr=pg;
					_gc_page_pool[_gc_page_pool_len]=pg;
					_gc_page_pool_len++;
				}
			}
			_gc_page_pool_len--;
			gc_page_header_t* pg=_gc_page_pool[_gc_page_pool_len];
			o=PTR(ADDR(pg)+sizeof(gc_page_header_t));
			SLL_ASSERT((GC_MEMORY_PAGE_SIZE-sizeof(gc_page_header_t)-sizeof(sll_object_t))/sizeof(sll_object_t)<=GC_OBJECT_POOL_SIZE);
			sll_object_t* c=o+1;
			while (_gc_object_pool_len<(GC_MEMORY_PAGE_SIZE-sizeof(gc_page_header_t)-sizeof(sll_object_t))/sizeof(sll_object_t)){
				c->rc=0;
				c->_flags=GC_FLAG_IN_FAST_POOL;
				c->data._pool_index=_gc_object_pool_len;
				_gc_object_pool[_gc_object_pool_len]=c;
				_gc_object_pool_len++;
				c++;
			}
		}
		GC_PAGE_HEADER_INCREASE(GC_MEMORY_PAGE_HEADER(o));
		o->_flags=0;
	}
	o->rc=1;
	o->_data=0;
	*((sll_object_type_t*)(&(o->type)))=type;
	return o;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_destroy_object(sll_object_t* object){
	SLL_ASSERT(object->rc);
	object->rc--;
	if (object->rc){
		return 0;
	}
	*((sll_object_type_t*)(&(object->type)))=SLL_OBJECT_TYPE_INT;
	sll__release_object_internal(object);
	return 1;
}



__SLL_EXTERNAL void sll_gc_add_root(sll_object_t* object){
	GC_SET_PREV_OBJECT(object,NULL);
	GC_SET_NEXT_OBJECT(object,_gc_root_data.single_root);
	if (_gc_root_data.single_root){
		GC_SET_PREV_OBJECT(_gc_root_data.single_root,object);
	}
	_gc_root_data.single_root=object;
}



__SLL_EXTERNAL void sll_gc_add_roots(sll_object_t*const* pointer,sll_size_t length){
}



__SLL_EXTERNAL void sll_gc_remove_root(sll_object_t* object){
	sll_object_t* prev=GC_GET_PREV_OBJECT(object);
	sll_object_t* next=GC_GET_NEXT_OBJECT(object);
	if (_gc_root_data.single_root==object){
		_gc_root_data.single_root=(prev?prev:next);
	}
	if (prev){
		GC_SET_NEXT_OBJECT(prev,next);
	}
	if (next){
		GC_SET_PREV_OBJECT(next,prev);
	}
	GC_CLEAR_OBJECTS(object);
}



__SLL_EXTERNAL void sll_gc_remove_roots(sll_object_t*const* pointer){
}



__SLL_EXTERNAL void sll_release_object(sll_object_t* object){
	SLL_RELEASE(object);
}
