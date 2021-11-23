#include <sll/_sll_internal.h>
#include <sll/api/string.h>
#include <sll/assembly.h>
#include <sll/common.h>
#include <sll/gc.h>
#include <sll/handle.h>
#include <sll/init.h>
#include <sll/memory.h>
#include <sll/platform.h>
#include <sll/runtime_object.h>
#include <sll/string.h>
#include <sll/types.h>
#include <sll/util.h>
#include <stdint.h>



STATIC_RUNTIME_OBJECT_SETUP;



static void* _gc_page_ptr=NULL;
static sll_runtime_object_t* _gc_next_object=NULL;
static uint32_t _gc_dbg_dtl=0;
static runtime_object_debug_data_t** _gc_dbg_dt=NULL;
static uint32_t _gc_alloc=0;
static uint32_t _gc_dealloc=0;
static uint8_t _gc_verify=1;



static const sll_char_t* _get_type_string(sll_runtime_object_t* o){
	switch (SLL_RUNTIME_OBJECT_GET_TYPE(o)){
		case SLL_RUNTIME_OBJECT_TYPE_INT:
			return SLL_CHAR("int");
		case SLL_RUNTIME_OBJECT_TYPE_FLOAT:
			return SLL_CHAR("float");
		case SLL_RUNTIME_OBJECT_TYPE_CHAR:
			return SLL_CHAR("char");
		case SLL_RUNTIME_OBJECT_TYPE_STRING:
			return SLL_CHAR("string");
		case SLL_RUNTIME_OBJECT_TYPE_ARRAY:
			return SLL_CHAR("array");
		case SLL_RUNTIME_OBJECT_TYPE_HANDLE:
			return SLL_CHAR("handle");
		case SLL_RUNTIME_OBJECT_TYPE_MAP:
			return SLL_CHAR("map");
		case RUNTIME_OBJECT_TYPE_FUNCTION_ID:
			return SLL_CHAR("<function id>");
		case RUNTIME_OBJECT_TYPE_UNKNOWN:
			return SLL_CHAR("<unknown>");
	}
	return SLL_CHAR("<unknown type>");
}



static void _print_gc_data(sll_runtime_object_t* o,runtime_object_debug_data_t* dt){
	sll_string_t str;
	sll_object_to_string((const sll_runtime_object_t*const*)(&o),1,&str);
	sll_file_write_format(sll_stderr,SLL_CHAR("{type: %s, ref: %u, data: %s}\n  Acquire (%u):\n"),_get_type_string(o),o->rc,str.v,dt->all);
	sll_deinit_string(&str);
	for (uint32_t m=0;m<dt->all;m++){
		runtime_object_debug_data_trace_data_t* t_dt=*(dt->al+m);
		sll_file_write_format(sll_stderr,SLL_CHAR("    %s:%u (%s)\n"),t_dt->fp,t_dt->ln,t_dt->fn);
	}
	sll_file_write_format(sll_stderr,SLL_CHAR("  Release (%u):\n"),dt->rll);
	for (uint32_t m=0;m<dt->rll;m++){
		runtime_object_debug_data_trace_data_t* t_dt=*(dt->rl+m);
		sll_file_write_format(sll_stderr,SLL_CHAR("    %s:%u (%s)\n"),t_dt->fp,t_dt->ln,t_dt->fn);
	}
}



void _gc_release_data(void){
	if (!_gc_page_ptr){
		return;
	}
	if (_gc_verify){
		if (!sll_verify_runtime_object_stack_cleanup()){
			SLL_UNIMPLEMENTED();
		}
		_gc_verify=0;
	}
	SLL_ASSERT(_gc_alloc<=_gc_dealloc);
	for (uint32_t i=0;i<_gc_dbg_dtl;i++){
		if (*(_gc_dbg_dt+i)){
			runtime_object_debug_data_t* dt=*(_gc_dbg_dt+i);
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
	sll_page_size_t sz=sll_platform_get_page_size()*GC_RUNTIME_OBJECT_POOL_PAGE_ALLOC_COUNT;
	void* c=_gc_page_ptr;
	while (c){
		void* n=*((void**)c);
		sll_platform_free_page(c,sz);
		c=n;
	}
	_gc_page_ptr=NULL;
}



__SLL_EXTERNAL sll_runtime_object_t* sll_add_debug_data(sll_runtime_object_t* o,const char* fp,unsigned int ln,const char* fn,unsigned int t){
	uint32_t i=o->_dbg0|(o->_dbg1<<8);
	if (i==GC_MAX_DEBUG_ID){
		i=0;
		if (_gc_dbg_dt){
			while (i<_gc_dbg_dtl){
				if (!(*(_gc_dbg_dt+i))){
					goto _found_index;
				}
				i++;
			}
		}
		_gc_dbg_dtl++;
		SLL_ASSERT(_gc_dbg_dtl<GC_MAX_DEBUG_ID);
		void* tmp=sll_reallocate(_gc_dbg_dt,_gc_dbg_dtl*sizeof(runtime_object_debug_data_t*));
		SLL_ASSERT(tmp||!"Unable to sll_reallocateate Debug Data Array");
		_gc_dbg_dt=tmp;
_found_index:
		o->_dbg0=i&0xff;
		o->_dbg1=i>>8;
		runtime_object_debug_data_t* dt=sll_allocate(sizeof(runtime_object_debug_data_t));
		dt->c.fp=NULL;
		dt->al=NULL;
		dt->all=0;
		dt->rl=NULL;
		dt->rll=0;
		*(_gc_dbg_dt+i)=dt;
	}
	else{
		SLL_ASSERT(i<_gc_dbg_dtl);
	}
	runtime_object_debug_data_trace_data_t* n=sll_allocate(sizeof(runtime_object_debug_data_trace_data_t));
	n->fp=fp;
	n->ln=ln;
	uint8_t j=0;
	while (*(fn+j)){
		n->fn[j]=*(fn+j);
		j++;
		SLL_ASSERT(j);
	}
	n->fn[j]=0;
	runtime_object_debug_data_t* dt=*(_gc_dbg_dt+i);
	if (t==__SLL_DEBUG_TYPE_CREATE){
		if (!dt->c.fp){
			dt->c=*n;
		}
	}
	if (t==__SLL_DEBUG_TYPE_RELEASE){
		dt->all++;
		dt->al=sll_reallocate(dt->al,dt->all*sizeof(runtime_object_debug_data_trace_data_t*));
		*(dt->al+dt->all-1)=n;
	}
	else{
		dt->rll++;
		dt->rl=sll_reallocate(dt->rl,dt->rll*sizeof(runtime_object_debug_data_trace_data_t*));
		*(dt->rl+dt->rll-1)=n;
	}
	return o;
}



__SLL_EXTERNAL void sll_acquire_object(sll_runtime_object_t* o){
	o->rc++;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_runtime_object_t* sll_create_object(void){
	if (!_gc_next_object){
		sll_page_size_t sz=sll_platform_get_page_size()*GC_RUNTIME_OBJECT_POOL_PAGE_ALLOC_COUNT;
		void* pg=sll_platform_allocate_page(sz);
		*((void**)pg)=_gc_page_ptr;
		_gc_page_ptr=pg;
		sll_runtime_object_t* c=(sll_runtime_object_t*)((uint64_t)pg+sizeof(void*));
		_gc_next_object=c;
		void* e=(void*)((uint64_t)pg+sizeof(void*)+((sz-sizeof(void*))/sizeof(sll_runtime_object_t)-1)*sizeof(sll_runtime_object_t));
		while ((void*)c<e){
			GC_SET_NEXT_OBJECT(c,c+1);
			c->rc=0;
			c++;
		}
		GC_SET_NEXT_OBJECT(c,NULL);
		c->rc=0;
	}
	sll_runtime_object_t* o=_gc_next_object;
	_gc_next_object=GC_GET_NEXT_OBJECT(o);
	o->rc=1;
	o->_dbg0=0xff;
	o->_dbg1=0xffff;
	_gc_alloc++;
	_gc_verify=1;
	return o;
}



__SLL_EXTERNAL void sll_release_object(sll_runtime_object_t* o){
	SLL_ASSERT(o->rc);
	o->rc--;
	if (!o->rc){
		sll_deinit_runtime_object(o);
		GC_SET_NEXT_OBJECT(o,_gc_next_object);
		_gc_next_object=o;
		_gc_dealloc++;
	}
}



__SLL_EXTERNAL void sll_remove_debug_data(sll_runtime_object_t* o){
	uint32_t i=o->_dbg0|(o->_dbg1<<8);
	if (i!=GC_MAX_DEBUG_ID){
		SLL_ASSERT(i<_gc_dbg_dtl);
		runtime_object_debug_data_t* dt=*(_gc_dbg_dt+i);
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
	o->_dbg0=0xff;
	o->_dbg1=0xffff;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_verify_runtime_object_stack_cleanup(void){
	sll_file_flush(sll_stdout);
	uint8_t err=0;
	_gc_verify=0;
	sll_page_size_t sz=sll_platform_get_page_size();
	void* pg=_gc_page_ptr;
	uint32_t i=0;
	while (pg){
		sll_runtime_object_t* c=(sll_runtime_object_t*)((uint64_t)pg+sizeof(void*));
		void* e=(void*)((uint64_t)pg+sizeof(void*)+(sz*(*((void**)pg)?1:GC_INIT_PAGE_COUNT)-sizeof(void*))/sizeof(sll_runtime_object_t)*sizeof(sll_runtime_object_t));
		while ((void*)c<e){
			if (c->rc){
				if (!err){
					err=1;
					sll_file_write_string(sll_stderr,SLL_CHAR("\nUnreleased Objects:\n"));
				}
				uint32_t j=c->_dbg0|(c->_dbg1<<8);
				if (j!=GC_MAX_DEBUG_ID){
					runtime_object_debug_data_t* dt=*(_gc_dbg_dt+j);
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
					sll_object_to_string((const sll_runtime_object_t*const*)&c,1,&str);
					sll_file_write_format(sll_stderr,SLL_CHAR("<unknown>: {type: %s, ref: %u, data: %s}\n  Acquire (0):\n  Release (0):\n"),_get_type_string(c),c->rc,str.v);
					sll_deinit_string(&str);
				}
			}
			i++;
			c++;
		}
		pg=*((void**)pg);
	}
	i=0;
	const static_runtime_object_t*const* l=&__strto_start;
	while (l<&__strto_end){
		const static_runtime_object_t* k=*l;
		if (k){
			SLL_ASSERT(k->dt->rc);
			if (k->dt->rc>1){
				if (!err){
					err=1;
					sll_file_write_string(sll_stderr,SLL_CHAR("\nUnreleased Objects:\n"));
				}
				SLL_ASSERT((k->dt->_dbg0|(k->dt->_dbg1<<8))!=GC_MAX_DEBUG_ID);
				sll_file_write_format(sll_stderr,SLL_CHAR("%s: %u (<static>): "),k->fp,k->ln);
				_print_gc_data(k->dt,*(_gc_dbg_dt+(k->dt->_dbg0|(k->dt->_dbg1<<8))));
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
