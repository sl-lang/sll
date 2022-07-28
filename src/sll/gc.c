#include <sll/_internal/common.h>
#include <sll/_internal/gc.h>
#include <sll/_internal/platform.h>
#include <sll/_internal/scheduler.h>
#include <sll/_internal/weakref.h>
#include <sll/_size_types.h>
#include <sll/api/string.h>
#include <sll/api/time.h>
#include <sll/allocator.h>
#include <sll/common.h>
#include <sll/data.h>
#include <sll/file.h>
#include <sll/gc.h>
#include <sll/memory.h>
#include <sll/object.h>
#include <sll/platform/memory.h>
#include <sll/string.h>
#include <sll/thread.h>
#include <sll/types.h>
#include <sll/vm.h>



static sll_object_t _gc_next_object=NULL;
static gc_memory_page_data_t _gc_memory_page_data={
	NULL,
	.length=0
};
static gc_object_pool_t _gc_object_pool={
	.length=0
};
static gc_fast_object_pool_t _gc_fast_object_pool={
	.read=0,
	.write=0,
	.space=GC_FAST_OBJECT_POOL_SIZE
};
static gc_root_data_t _gc_root_data={
	NULL,
	NULL,
	0,
	.fast_count=0,
	.fast_empty_index=__SLL_U16_MAX
};
static gc_data_t _gc_data={
	GC_GARBAGE_COLLECTION_INTERVAL,
	0,
	1,
	0
};



static void _mark_objects(sll_object_t object){
	SLL_ASSERT(SLL_GET_OBJECT_REFERENCE_COUNTER(object));
	if ((object->_flags&GC_FLAG_STATIC)||GC_IS_MARKED(object)){
		return;
	}
	GC_SET_MARKED(object);
	GC_MEMORY_PAGE_HEADER(object)->garbage_cnt--;
	if (object->type==SLL_OBJECT_TYPE_ARRAY){
		for (sll_array_length_t i=0;i<object->data.array.length;i++){
			_mark_objects(object->data.array.data[i]);
		}
	}
	else if (object->type==SLL_OBJECT_TYPE_MAP){
		for (sll_map_length_t i=0;i<(object->data.map.length<<1);i++){
			_mark_objects(object->data.map.data[i]);
		}
	}
	else if (object->type>SLL_MAX_OBJECT_TYPE&&object->type<SLL_OBJECT_TYPE_SELF&&sll_current_runtime_data&&object->type<=sll_current_runtime_data->type_table->length+SLL_MAX_OBJECT_TYPE){
		const sll_object_type_data_t* dt=*(sll_current_runtime_data->type_table->data+object->type-SLL_MAX_OBJECT_TYPE-1);
		sll_object_field_t* pointer=object->data.fields;
		for (sll_arg_count_t i=0;i<dt->field_count;i++){
			if (dt->fields[i].type>SLL_OBJECT_TYPE_CHAR){
				_mark_objects(pointer->any);
			}
			pointer++;
		}
	}
}



void _gc_release_data(void){
	SLL_ASSERT(_gc_data.enabled);
	sll_gc_collect();
	SLL_ASSERT(!_gc_root_data.single);
	SLL_ASSERT(!_gc_root_data.multiple_length);
	SLL_ASSERT(!_gc_root_data.fast_count);
	while (_gc_fast_object_pool.space!=GC_FAST_OBJECT_POOL_SIZE){
		GC_PAGE_HEADER_DECREASE(GC_MEMORY_PAGE_HEADER(_gc_fast_object_pool.data[_gc_fast_object_pool.read]));
		_gc_fast_object_pool.read=(_gc_fast_object_pool.read+1)&(GC_FAST_OBJECT_POOL_SIZE-1);
		_gc_fast_object_pool.space++;
	}
	SLL_ASSERT(_gc_fast_object_pool.read==_gc_fast_object_pool.write);
	sll_bool_t err=0;
	while (_gc_memory_page_data.root){
		if (_gc_memory_page_data.root->cnt){
			GC_ITER_PAGE_OBJECTS(_gc_memory_page_data.root){
				if (SLL_GET_OBJECT_REFERENCE_COUNTER(current)){
					err=1;
					sll_file_write_format(sll_stderr,SLL_CHAR("[%p]: "),NULL,current);
					sll_string_t str;
					sll_api_string_convert(&current,1,&str);
					sll_file_write(sll_stderr,str.data,str.length*sizeof(sll_char_t),NULL);
					sll_free_string(&str);
					sll_file_write_char(sll_stderr,'\n',NULL);
				}
			}
		}
		gc_page_header_t* nxt=_gc_memory_page_data.root->next;
		SLL_CRITICAL_ERROR(sll_platform_free_page(_gc_memory_page_data.root,GC_MEMORY_PAGE_SIZE));
		_gc_memory_page_data.root=nxt;
	}
	if (err){
		_force_exit_platform();
	}
	_gc_next_object=NULL;
	_gc_memory_page_data.length=0;
	_gc_object_pool.length=0;
}



__SLL_EXTERNAL void sll__gc_error(sll_object_t object){
	SLL_CRITICAL(SLL_GET_OBJECT_REFERENCE_COUNTER(object));
}



__SLL_EXTERNAL void sll__release_object_internal(sll_object_t object){
	SLL_ASSERT(!SLL_GET_OBJECT_REFERENCE_COUNTER(object));
	if (object->_flags&GC_FLAG_HAS_WEAKREF){
		object->reference_count++;
		_weakref_delete(object);
		object->reference_count--;
		if (SLL_GET_OBJECT_REFERENCE_COUNTER(object)){
			return;
		}
	}
	sll_bool_t gc_state=_gc_data.enabled;
	_gc_data.enabled=0;
	if (object->type==SLL_OBJECT_TYPE_STRING){
		sll_free_string(&(object->data.string));
	}
	else if (object->type==SLL_OBJECT_TYPE_ARRAY||object->type==SLL_OBJECT_TYPE_MAP_KEYS||object->type==SLL_OBJECT_TYPE_MAP_VALUES){
		for (sll_array_length_t i=0;i<object->data.array.length;i++){
			GC_RELEASE_CHECK_ZERO_REF(object->data.array.data[i]);
		}
		sll_allocator_release(object->data.array.data);
	}
	else if (object->type==SLL_OBJECT_TYPE_MAP){
		for (sll_map_length_t j=0;j<(object->data.map.length<<1);j++){
			GC_RELEASE_CHECK_ZERO_REF(*(object->data.map.data+j));
		}
		sll_deallocate(object->data.map.data);
	}
	else if (object->type>SLL_MAX_OBJECT_TYPE&&object->type<SLL_OBJECT_TYPE_SELF){
		if (sll_current_runtime_data&&object->type<=sll_current_runtime_data->type_table->length+SLL_MAX_OBJECT_TYPE){
			const sll_object_type_data_t* dt=*(sll_current_runtime_data->type_table->data+object->type-SLL_MAX_OBJECT_TYPE-1);
			if (_scheduler_current_thread_index!=SLL_UNKNOWN_THREAD_INDEX&&dt->functions[SLL_OBJECT_FUNC_DELETE]){
				_gc_data.enabled=1;
				object->reference_count++;
				SLL_RELEASE(sll_execute_function(dt->functions[SLL_OBJECT_FUNC_DELETE],&object,1,0));
				object->reference_count--;
				if (SLL_GET_OBJECT_REFERENCE_COUNTER(object)){
					_gc_data.enabled=gc_state;
					return;
				}
				_gc_data.enabled=0;
			}
			sll_object_field_t* p=object->data.fields;
			for (sll_arg_count_t i=0;i<dt->field_count;i++){
				if (dt->fields[i].type>SLL_OBJECT_TYPE_CHAR){
					GC_RELEASE_CHECK_ZERO_REF(p->any);
				}
				p++;
			}
		}
		sll_deallocate(object->data.fields);
	}
	if ((object->_flags&GC_FLAG_IN_FAST_ROOT_POOL)||GC_GET_PREV_OBJECT(object)||GC_GET_NEXT_OBJECT(object)){
		SLL_UNIMPLEMENTED();
	}
	object->_flags=0;
	if (_gc_fast_object_pool.space){
		_gc_fast_object_pool.data[_gc_fast_object_pool.write]=object;
		_gc_fast_object_pool.write=(_gc_fast_object_pool.write+1)&(GC_FAST_OBJECT_POOL_SIZE-1);
		_gc_fast_object_pool.space--;
		_gc_data.enabled=gc_state;
		return;
	}
	gc_page_header_t* pg=GC_MEMORY_PAGE_HEADER(object);
	GC_PAGE_HEADER_DECREASE(pg);
	if (!GC_PAGE_HEADER_CAN_DELETE(pg)){
		if (_gc_object_pool.length<GC_OBJECT_POOL_SIZE){
			object->_flags=GC_FLAG_IN_FAST_POOL;
			object->data._pool_index=_gc_object_pool.length;
			_gc_object_pool.data[_gc_object_pool.length]=object;
			_gc_object_pool.length++;
			goto _check_garbage_collect;
		}
		object->data._next_object.next=_gc_next_object;
		object->data._next_object.prev=NULL;
		if (_gc_next_object){
			_gc_next_object->data._next_object.prev=object;
		}
		_gc_next_object=object;
		goto _check_garbage_collect;
	}
	object->data._next_object.next=NULL;
	object->data._next_object.prev=NULL;
	while (GC_MEMORY_PAGE_HEADER(_gc_next_object)==pg){
		_gc_next_object=_gc_next_object->data._next_object.next;
	}
	sll_bool_t pool_shift=0;
	GC_ITER_PAGE_OBJECTS(pg){
		SLL_ASSERT(current!=_gc_next_object);
		if (current->_flags&GC_FLAG_IN_FAST_POOL){
			_gc_object_pool.data[current->data._pool_index]=NULL;
			pool_shift=1;
		}
		else{
			if (current->data._next_object.prev){
				current->data._next_object.prev->data._next_object.next=current->data._next_object.next;
			}
			if (current->data._next_object.next){
				current->data._next_object.next->data._next_object.prev=current->data._next_object.prev;
			}
		}
	}
	if (_gc_memory_page_data.length<GC_PAGE_POOL_SIZE){
		_gc_memory_page_data.data[_gc_memory_page_data.length]=pg;
		_gc_memory_page_data.length++;
	}
	else{
		if (pg->prev){
			pg->prev->next=pg->next;
			if (_gc_memory_page_data.root==pg){
				_gc_memory_page_data.root=pg->prev;
			}
		}
		if (pg->next){
			pg->next->prev=pg->prev;
			if (_gc_memory_page_data.root==pg){
				_gc_memory_page_data.root=pg->next;
			}
		}
		if (_gc_memory_page_data.root==pg){
			_gc_memory_page_data.root=pg->next;
		}
		SLL_CRITICAL_ERROR(sll_platform_free_page(pg,GC_MEMORY_PAGE_SIZE));
	}
	if (pool_shift){
		sll_object_pool_index_t i=0;
		for (sll_object_pool_index_t j=0;j<_gc_object_pool.length;j++){
			if (_gc_object_pool.data[j]){
				_gc_object_pool.data[i]=_gc_object_pool.data[j];
				_gc_object_pool.data[i]->data._pool_index=i;
				i++;
			}
		}
		_gc_object_pool.length=i;
	}
_check_garbage_collect:
	if (gc_state&&!_gc_data.cleanup_in_progress){
		if (!_gc_data.time){
			sll_gc_collect();
		}
		_gc_data.time--;
	}
	_gc_data.enabled=gc_state;
}



__SLL_EXTERNAL void sll_acquire_object(sll_object_t object){
	object->reference_count++;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t sll_create_object(sll_object_type_t type){
	sll_object_t out;
	if (_gc_fast_object_pool.space!=GC_FAST_OBJECT_POOL_SIZE){
		out=_gc_fast_object_pool.data[_gc_fast_object_pool.read];
		_gc_fast_object_pool.read=(_gc_fast_object_pool.read+1)&(GC_FAST_OBJECT_POOL_SIZE-1);
		_gc_fast_object_pool.space++;
	}
	else{
		if (_gc_object_pool.length){
			_gc_object_pool.length--;
			out=_gc_object_pool.data[_gc_object_pool.length];
		}
		else if (_gc_next_object){
			out=_gc_next_object;
			_gc_next_object=out->data._next_object.next;
			if (_gc_next_object){
				_gc_next_object->data._next_object.prev=NULL;
			}
		}
		else{
			if (!_gc_memory_page_data.length){
				gc_memory_page_data_length_t i=(_gc_memory_page_data.root?GC_PAGE_DYNAMIC_POOL_SIZE:GC_PAGE_INIT_POOL_SIZE);
				while (i){
					i--;
					gc_page_header_t* pg=sll_platform_allocate_page_aligned(GC_MEMORY_PAGE_SIZE,GC_MEMORY_PAGE_SIZE,NULL);
					if (_gc_memory_page_data.root){
						_gc_memory_page_data.root->prev=pg;
					}
					pg->prev=NULL;
					pg->next=_gc_memory_page_data.root;
					pg->cnt=0;
					_gc_memory_page_data.root=pg;
					_gc_memory_page_data.data[_gc_memory_page_data.length]=pg;
					_gc_memory_page_data.length++;
				}
			}
			_gc_memory_page_data.length--;
			gc_page_header_t* pg=_gc_memory_page_data.data[_gc_memory_page_data.length];
			out=PTR(ADDR(pg)+sizeof(gc_page_header_t));
			SLL_ASSERT(GC_PAGE_OBJECT_COUNT<GC_OBJECT_POOL_SIZE);
			sll_object_t c=out+1;
			while (_gc_object_pool.length<GC_PAGE_OBJECT_COUNT-1){
				c->reference_count=0;
				c->_flags=GC_FLAG_IN_FAST_POOL;
				c->data._pool_index=_gc_object_pool.length;
				_gc_object_pool.data[_gc_object_pool.length]=c;
				_gc_object_pool.length++;
				c++;
			}
		}
		GC_PAGE_HEADER_INCREASE(GC_MEMORY_PAGE_HEADER(out));
		out->_flags=0;
	}
	GC_SET_MARKED(out);
	out->reference_count=1;
	*((sll_object_type_t*)(&(out->type)))=type;
	return out;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_destroy_object(sll_object_t object){
	SLL_ASSERT(SLL_GET_OBJECT_REFERENCE_COUNTER(object));
	object->reference_count--;
	if (SLL_GET_OBJECT_REFERENCE_COUNTER(object)){
		return 0;
	}
	*((sll_object_type_t*)(&(object->type)))=SLL_OBJECT_TYPE_INT;
	sll__release_object_internal(object);
	return 1;
}



__SLL_EXTERNAL void sll_gc_add_root(sll_object_t object,sll_bool_t fast){
	if (object->_flags&GC_FLAG_STATIC){
		return;
	}
	if (GC_IS_ANY_ROOT(object)){
		GC_INCREASE_ROOT(object);
		if (!fast&&(object->_flags&GC_FLAG_IN_FAST_ROOT_POOL)){
			SLL_UNIMPLEMENTED();
		}
		return;
	}
	GC_INCREASE_ROOT(object);
	if (fast&&_gc_root_data.fast_count<GC_FAST_ROOT_DATA_COUNT){
		_gc_root_data.fast_count++;
		if (_gc_root_data.fast_empty_index==__SLL_U16_MAX){
			_gc_root_data.fast_empty_index=0;
			for (fast_root_index_t i=0;i<GC_FAST_ROOT_DATA_COUNT-1;i++){
				_gc_root_data.fast[i]=GC_FAST_ROOT_SET_NEXT_INDEX(i+1);
			}
			_gc_root_data.fast[GC_FAST_ROOT_DATA_COUNT-1]=NULL;
		}
		object->_data=_gc_root_data.fast_empty_index;
		void* nxt=_gc_root_data.fast[object->_data];
		if (!nxt){
			_gc_root_data.fast_empty_index=GC_FAST_ROOT_DATA_COUNT;
		}
		else{
			_gc_root_data.fast_empty_index=GC_FAST_ROOT_GET_NEXT_INDEX(nxt);
		}
		_gc_root_data.fast[object->_data]=object;
		object->_flags|=GC_FLAG_IN_FAST_ROOT_POOL;
		return;
	}
	GC_SET_PREV_OBJECT(object,NULL);
	GC_SET_NEXT_OBJECT(object,_gc_root_data.single);
	if (_gc_root_data.single){
		GC_SET_PREV_OBJECT(_gc_root_data.single,object);
	}
	_gc_root_data.single=object;
}



__SLL_EXTERNAL void sll_gc_add_roots(const sll_object_t* pointer,sll_size_t length){
	if (!length){
		return;
	}
	SLL_CRITICAL(!(length>>GC_ROOTS_LENGTH_SHIFT));
	_gc_root_data.multiple=sll_reallocate(_gc_root_data.multiple,(_gc_root_data.multiple_length+1)*sizeof(gc_multiple_root_t));
	*(_gc_root_data.multiple+_gc_root_data.multiple_length)=GC_ENCODE_ROOT(pointer,length);
	_gc_root_data.multiple_length++;
}



__SLL_EXTERNAL void sll_gc_collect(void){
	_gc_data.time=GC_GARBAGE_COLLECTION_INTERVAL;
	gc_page_header_t* page=_gc_memory_page_data.root;
	if (!page){
		return;
	}
	_gc_data.cleanup_in_progress=1;
	do{
		page->garbage_cnt=page->cnt>>1;
		page=page->next;
	} while (page);
	_gc_data.object_marker_signature=!_gc_data.object_marker_signature;
	if (_gc_root_data.fast_count){
		fast_root_index_t i=_gc_root_data.fast_count;
		for (fast_root_index_t j=0;i&&j<GC_FAST_ROOT_DATA_COUNT;j++){
			if (GC_FAST_ROOT_IS_OBJECT(_gc_root_data.fast[j])){
				_mark_objects(_gc_root_data.fast[j]);
				i--;
			}
		}
	}
	sll_object_t object=_gc_root_data.single;
	while (object){
		_mark_objects(object);
		object=GC_GET_NEXT_OBJECT(object);
	}
	for (sll_size_t i=0;i<_gc_root_data.multiple_length;i++){
		const sll_object_t* pointer=GC_GET_ROOT(*(_gc_root_data.multiple+i));
		sll_size_t length=GC_GET_LENGTH(*(_gc_root_data.multiple+i));
		while (length&&*pointer&&SLL_GET_OBJECT_REFERENCE_COUNTER(*pointer)){
			_mark_objects(*pointer);
			length--;
			pointer++;
		}
	}
	page=_gc_memory_page_data.root;
	do{
		sll_size_t cnt=page->garbage_cnt;
		if (cnt){
			GC_ITER_PAGE_OBJECTS(page){
				if (SLL_GET_OBJECT_REFERENCE_COUNTER(current)&&!GC_IS_MARKED(current)){
					cnt--;
					SLL_RELEASE(current);
					if (!cnt){
						break;
					}
				}
			}
		}
		page=page->next;
	} while (page);
	_gc_data.cleanup_in_progress=0;
}



__SLL_EXTERNAL void sll_gc_remove_root(sll_object_t object){
	if (object->_flags&GC_FLAG_STATIC){
		return;
	}
	SLL_ASSERT(GC_IS_ANY_ROOT(object));
	GC_DECREASE_ROOT(object);
	if (GC_IS_ANY_ROOT(object)){
		return;
	}
	if (object->_flags&GC_FLAG_IN_FAST_ROOT_POOL){
		object->_flags&=~GC_FLAG_IN_FAST_ROOT_POOL;
		SLL_ASSERT(_gc_root_data.fast[object->_data]==object);
		_gc_root_data.fast[object->_data]=GC_FAST_ROOT_SET_NEXT_INDEX(_gc_root_data.fast_empty_index);
		_gc_root_data.fast_empty_index=(fast_root_index_t)(object->_data);
		_gc_root_data.fast_count--;
		object->_data=0;
		return;
	}
	sll_object_t prev=GC_GET_PREV_OBJECT(object);
	sll_object_t next=GC_GET_NEXT_OBJECT(object);
	if (_gc_root_data.single==object){
		_gc_root_data.single=(prev?prev:next);
	}
	if (prev){
		GC_SET_NEXT_OBJECT(prev,next);
	}
	if (next){
		GC_SET_PREV_OBJECT(next,prev);
	}
	GC_CLEAR_OBJECTS(object);
}



__SLL_EXTERNAL void sll_gc_remove_roots(const sll_object_t* pointer){
	sll_size_t i=_gc_root_data.multiple_length;
	while (i){
		i--;
		if (GC_GET_ROOT(*(_gc_root_data.multiple+i))!=pointer){
			continue;
		}
		i++;
		while (i<_gc_root_data.multiple_length){
			*(_gc_root_data.multiple+i-1)=*(_gc_root_data.multiple+i);
			i++;
		}
		_gc_root_data.multiple_length=i-1;
		_gc_root_data.multiple=sll_reallocate(_gc_root_data.multiple,_gc_root_data.multiple_length*sizeof(gc_multiple_root_t));
		return;
	}
}



__SLL_EXTERNAL void sll_release_object(sll_object_t object){
	SLL_RELEASE(object);
}
