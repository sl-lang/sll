#include <sll/_sll_internal.h>
#include <sll/api/string.h>
#include <sll/array.h>
#include <sll/assembly.h>
#include <sll/common.h>
#include <sll/file.h>
#include <sll/gc.h>
#include <sll/handle.h>
#include <sll/map.h>
#include <sll/memory.h>
#include <sll/object.h>
#include <sll/platform.h>
#include <sll/string.h>
#include <sll/types.h>
#include <sll/util.h>
#include <stdint.h>



STATIC_OBJECT_SETUP;



static void* _gc_page_ptr=NULL;
static sll_object_t* _gc_next_object=NULL;
static uint32_t _gc_dbg_dtl=0;
static object_debug_data_t** _gc_dbg_dt=NULL;
static uint32_t _gc_alloc=0;
static uint32_t _gc_dealloc=0;
static sll_bool_t _gc_verify=1;



static const sll_char_t* _get_type_string(sll_object_t* o){
	switch (SLL_OBJECT_GET_TYPE(o)){
		case SLL_OBJECT_TYPE_INT:
			return SLL_CHAR("int");
		case SLL_OBJECT_TYPE_FLOAT:
			return SLL_CHAR("float");
		case SLL_OBJECT_TYPE_CHAR:
			return SLL_CHAR("char");
		case SLL_OBJECT_TYPE_STRING:
			return SLL_CHAR("string");
		case SLL_OBJECT_TYPE_ARRAY:
			return SLL_CHAR("array");
		case SLL_OBJECT_TYPE_HANDLE:
			return SLL_CHAR("handle");
		case SLL_OBJECT_TYPE_MAP:
			return SLL_CHAR("map");
		case OBJECT_TYPE_FUNCTION_ID:
			return SLL_CHAR("<function id>");
		case OBJECT_TYPE_UNKNOWN:
			return SLL_CHAR("<unknown>");
	}
	return SLL_CHAR("<unknown type>");
}



static void _print_gc_data(sll_object_t* o,object_debug_data_t* dt){
	sll_string_t str;
	sll_api_string_convert((const sll_object_t*const*)(&o),1,&str);
	sll_file_write_format(sll_stderr,SLL_CHAR("{type: %s, ref: %u, data: %s}\n  Acquire (%u):\n"),_get_type_string(o),o->rc,str.v,dt->all);
	sll_free_string(&str);
	for (uint32_t m=0;m<dt->all;m++){
		object_debug_data_trace_data_t* t_dt=*(dt->al+m);
		sll_file_write_format(sll_stderr,SLL_CHAR("    %s:%u (%s)\n"),t_dt->fp,t_dt->ln,t_dt->fn);
	}
	sll_file_write_format(sll_stderr,SLL_CHAR("  Release (%u):\n"),dt->rll);
	for (uint32_t m=0;m<dt->rll;m++){
		object_debug_data_trace_data_t* t_dt=*(dt->rl+m);
		sll_file_write_format(sll_stderr,SLL_CHAR("    %s:%u (%s)\n"),t_dt->fp,t_dt->ln,t_dt->fn);
	}
}



static void* _release_custom_type(void* p,sll_object_type_t t){
	const sll_object_type_data_t* dt=*(sll_current_runtime_data->tt->dt+t-SLL_MAX_OBJECT_TYPE-1);
	for (sll_arg_count_t i=0;i<dt->l;i++){
		switch (SLL_OBJECT_GET_TYPE_MASK(dt->dt[i].t)){
			case SLL_OBJECT_TYPE_INT:
			case SLL_OBJECT_TYPE_CHAR:
				p=(void*)(((uint64_t)p)+sizeof(sll_integer_t));
				break;
			case SLL_OBJECT_TYPE_FLOAT:
				p=(void*)(((uint64_t)p)+sizeof(sll_float_t));
				break;
			case SLL_OBJECT_TYPE_STRING:
				sll_free_string((sll_string_t*)p);
				p=(void*)(((uint64_t)p)+sizeof(sll_string_t));
				break;
			case SLL_OBJECT_TYPE_ARRAY:
			case SLL_OBJECT_TYPE_MAP_KEYS:
			case SLL_OBJECT_TYPE_MAP_VALUES:
				sll_free_array((sll_array_t*)p);
				p=(void*)(((uint64_t)p)+sizeof(sll_array_t));
				break;
			case SLL_OBJECT_TYPE_HANDLE:
				{
					sll_handle_descriptor_t* hd=sll_handle_lookup_descriptor(sll_current_runtime_data->hl,((sll_handle_data_t*)p)->t);
					if (hd&&hd->df){
						hd->df(((sll_handle_data_t*)p)->h);
					}
					p=(void*)(((uint64_t)p)+sizeof(sll_handle_data_t));
					break;
				}
			case SLL_OBJECT_TYPE_MAP:
				sll_free_map((sll_map_t*)p);
				p=(void*)(((uint64_t)p)+sizeof(sll_map_t));
				break;
			default:
				p=_release_custom_type(p,SLL_OBJECT_GET_TYPE_MASK(dt->dt[i].t));
				break;
		}
	}
	return p;
}



void _gc_release_data(void){
	if (!_gc_page_ptr){
		return;
	}
	if (_gc_verify){
		if (!sll_verify_object_stack_cleanup()){
			SLL_UNIMPLEMENTED();
		}
		_gc_verify=0;
	}
	SLL_ASSERT(_gc_alloc<=_gc_dealloc);
	for (uint32_t i=0;i<_gc_dbg_dtl;i++){
		if (*(_gc_dbg_dt+i)){
			object_debug_data_t* dt=*(_gc_dbg_dt+i);
			*(_gc_dbg_dt+i)=NULL;
			for (uint32_t j=0;j<dt->all;j++){
				sll_deallocate(*(dt->al+j));
			}
			sll_deallocate(dt->al);
			for (uint32_t j=0;j<dt->rll;j++){
				sll_deallocate(*(dt->rl+j));
			}
			sll_deallocate(dt->rl);
			sll_deallocate(dt);
		}
	}
	sll_deallocate(_gc_dbg_dt);
	_gc_dbg_dtl=0;
	_gc_dbg_dt=NULL;
	void* c=_gc_page_ptr;
	while (c){
		void* n=*((void**)c);
		sll_platform_free_page(c,SLL_ROUND_PAGE(GC_OBJECT_POOL_ALLOC_SIZE));
		c=n;
	}
	_gc_page_ptr=NULL;
}



__SLL_EXTERNAL sll_object_t* sll_add_debug_data(sll_object_t* o,const char* fp,unsigned int ln,const char* fn,unsigned int t){
	if (!(o->_dbg)){
		object_debug_data_t* dt=sll_allocate(sizeof(object_debug_data_t));
		dt->c.fp=NULL;
		dt->al=NULL;
		dt->all=0;
		dt->rl=NULL;
		dt->rll=0;
		o->_dbg=dt;
	}
	object_debug_data_trace_data_t* n=sll_allocate(sizeof(object_debug_data_trace_data_t));
	n->fp=fp;
	n->ln=ln;
	uint8_t j=0;
	while (*(fn+j)){
		n->fn[j]=*(fn+j);
		j++;
		SLL_ASSERT(j);
	}
	n->fn[j]=0;
	object_debug_data_t* dt=o->_dbg;
	if (t==__SLL_DEBUG_TYPE_CREATE){
		if (!dt->c.fp){
			dt->c=*n;
		}
	}
	if (t==__SLL_DEBUG_TYPE_RELEASE){
		dt->all++;
		dt->al=sll_reallocate(dt->al,dt->all*sizeof(object_debug_data_trace_data_t*));
		*(dt->al+dt->all-1)=n;
	}
	else{
		dt->rll++;
		dt->rl=sll_reallocate(dt->rl,dt->rll*sizeof(object_debug_data_trace_data_t*));
		*(dt->rl+dt->rll-1)=n;
	}
	return o;
}



__SLL_EXTERNAL void sll_acquire_object(sll_object_t* o){
	o->rc++;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_create_object(void){
	if (!_gc_next_object){
		void* pg=sll_platform_allocate_page(SLL_ROUND_PAGE(GC_OBJECT_POOL_ALLOC_SIZE),0);
		*((void**)pg)=_gc_page_ptr;
		_gc_page_ptr=pg;
		sll_object_t* c=(sll_object_t*)((uint64_t)pg+sizeof(void*));
		_gc_next_object=c;
		void* e=(void*)((uint64_t)pg+sizeof(void*)+((SLL_ROUND_PAGE(GC_OBJECT_POOL_ALLOC_SIZE)-sizeof(void*))/sizeof(sll_object_t)-1)*sizeof(sll_object_t));
		while ((void*)c<e){
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
	o->_dbg=NULL;
	_gc_alloc++;
	_gc_verify=1;
	return o;
}



__SLL_EXTERNAL void sll_release_object(sll_object_t* o){
	SLL_ASSERT(o->rc);
	o->rc--;
	if (!o->rc){
		sll_remove_debug_data(o);
		if (SLL_OBJECT_GET_TYPE(o)==SLL_OBJECT_TYPE_STRING){
			if (!(o->t&OBJECT_EXTERNAL_STRING)){
				sll_free_string(&(o->dt.s));
			}
		}
		else if (SLL_OBJECT_GET_TYPE(o)==SLL_OBJECT_TYPE_ARRAY||SLL_OBJECT_GET_TYPE(o)==SLL_OBJECT_TYPE_MAP_KEYS||SLL_OBJECT_GET_TYPE(o)==SLL_OBJECT_TYPE_MAP_VALUES){
			sll_free_array(&(o->dt.a));
		}
		else if (SLL_OBJECT_GET_TYPE(o)==SLL_OBJECT_TYPE_HANDLE){
			if (sll_current_runtime_data){
				sll_handle_descriptor_t* hd=sll_handle_lookup_descriptor(sll_current_runtime_data->hl,o->dt.h.t);
				if (hd&&hd->df){
					hd->df(o->dt.h.h);
				}
			}
		}
		else if (SLL_OBJECT_GET_TYPE(o)==SLL_OBJECT_TYPE_MAP){
			sll_free_map(&(o->dt.m));
		}
		else if (SLL_OBJECT_GET_TYPE(o)>SLL_MAX_OBJECT_TYPE&&SLL_OBJECT_GET_TYPE(o)<SLL_OBJECT_TYPE_RESERVED0){
			if (sll_current_runtime_data&&SLL_OBJECT_GET_TYPE(o)<=sll_current_runtime_data->tt->l+SLL_MAX_OBJECT_TYPE){
				_release_custom_type(o->dt.p,SLL_OBJECT_GET_TYPE(o));
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



__SLL_EXTERNAL void sll_remove_debug_data(sll_object_t* o){
	if (o->_dbg){
		object_debug_data_t* dt=o->_dbg;
		o->_dbg=NULL;
		for (uint32_t j=0;j<dt->all;j++){
			sll_deallocate(*(dt->al+j));
		}
		sll_deallocate(dt->al);
		for (uint32_t j=0;j<dt->rll;j++){
			sll_deallocate(*(dt->rl+j));
		}
		sll_deallocate(dt->rl);
		sll_deallocate(dt);
	}
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_verify_object_stack_cleanup(void){
	sll_file_flush(sll_stdout);
	uint8_t err=0;
	_gc_verify=0;
	void* pg=_gc_page_ptr;
	uint32_t i=0;
	while (pg){
		sll_object_t* c=(sll_object_t*)((uint64_t)pg+sizeof(void*));
		void* e=(void*)((uint64_t)pg+sizeof(void*)+(SLL_ROUND_PAGE(GC_OBJECT_POOL_ALLOC_SIZE)-sizeof(void*))/sizeof(sll_object_t)*sizeof(sll_object_t));
		while ((void*)c<e){
			if (c->rc){
				if (!err){
					err=1;
					sll_file_write_string(sll_stderr,SLL_CHAR("\nUnreleased Objects:\n"));
				}
				if (c->_dbg){
					object_debug_data_t* dt=c->_dbg;
					if (dt->c.fp){
						sll_file_write_format(sll_stderr,SLL_CHAR("%s:%u (%s): "),dt->c.fp,dt->c.ln,dt->c.fn);
					}
					else{
						sll_file_write_string(sll_stderr,SLL_CHAR("<unknown>: "));
					}
					_print_gc_data(c,dt);
				}
				else{
					sll_string_t str;
					sll_api_string_convert((const sll_object_t*const*)&c,1,&str);
					sll_file_write_format(sll_stderr,SLL_CHAR("<unknown>: {type: %s, ref: %u, data: %s}\n  Acquire (0):\n  Release (0):\n"),_get_type_string(c),c->rc,str.v);
					sll_free_string(&str);
				}
			}
			i++;
			c++;
		}
		pg=*((void**)pg);
	}
	i=0;
	const static_object_t*const* l=&__strto_start;
	while (l<&__strto_end){
		const static_object_t* k=*l;
		if (k){
			SLL_ASSERT(k->dt->rc);
			if (k->dt->rc>1){
				if (!err){
					err=1;
					sll_file_write_string(sll_stderr,SLL_CHAR("\nUnreleased Objects:\n"));
				}
				SLL_ASSERT(k->dt->_dbg);
				sll_file_write_format(sll_stderr,SLL_CHAR("%s: %u (<static>): "),k->fp,k->ln);
				_print_gc_data(k->dt,k->dt->_dbg);
			}
			else{
				sll_remove_debug_data(k->dt);
			}
			i++;
		}
		l++;
	}
	sll_file_flush(sll_stderr);
	return (err?0:1);
}
