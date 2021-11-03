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



void _gc_release_data(void){
	if (_gc_verify){
		if (!sll_verify_runtime_object_stack_cleanup()){
			SLL_UNIMPLEMENTED();
		}
		_gc_verify=0;
	}
	SLL_ASSERT(_gc_alloc==_gc_dealloc);
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
	sll_page_size_t sz=sll_platform_get_page_size();
	void* c=_gc_page_ptr;
	while (c){
		void* n=*((void**)c);
		sll_platform_free_page(c,sz*(n?1:GC_INIT_PAGE_COUNT));
		c=n;
	}
}



__SLL_FUNC sll_runtime_object_t* sll_add_debug_data(sll_runtime_object_t* o,const char* fp,unsigned int ln,const char* fn,unsigned int t){
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
		void* tmp=sll_rellocate(_gc_dbg_dt,_gc_dbg_dtl*sizeof(runtime_object_debug_data_t*));
		SLL_ASSERT(tmp||!"Unable to sll_rellocateate Debug Data Array");
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
		dt->al=sll_rellocate(dt->al,dt->all*sizeof(runtime_object_debug_data_trace_data_t*));
		*(dt->al+dt->all-1)=n;
	}
	else{
		dt->rll++;
		dt->rl=sll_rellocate(dt->rl,dt->rll*sizeof(runtime_object_debug_data_trace_data_t*));
		*(dt->rl+dt->rll-1)=n;
	}
	return o;
}



__SLL_FUNC void sll_acquire_object(sll_runtime_object_t* o){
	o->rc++;
}



__SLL_FUNC __SLL_CHECK_OUTPUT sll_runtime_object_t* sll_create_object(void){
	if (!_gc_next_object){
		sll_page_size_t sz=sll_platform_get_page_size();
		if (!_gc_page_ptr){
			sz*=GC_INIT_PAGE_COUNT;
		}
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



__SLL_FUNC void sll_release_object(sll_runtime_object_t* o){
	SLL_ASSERT(o->rc);
	o->rc--;
	if (!o->rc){
		sll_deinit_runtime_object(o);
		GC_SET_NEXT_OBJECT(o,_gc_next_object);
		_gc_next_object=o;
		_gc_dealloc++;
	}
}



__SLL_FUNC void sll_remove_debug_data(sll_runtime_object_t* o){
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



__SLL_FUNC __SLL_CHECK_OUTPUT sll_return_t sll_verify_runtime_object_stack_cleanup(void){
	uint8_t err=0;
	_gc_verify=0;
	fflush(stdout);
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
					fputs("\nUnreleased Objects:\n",stderr);
				}
				const char* t="<wrong type>";
				switch (SLL_RUNTIME_OBJECT_GET_TYPE(c)){
					case SLL_RUNTIME_OBJECT_TYPE_INT:
						t="int";
						break;
					case SLL_RUNTIME_OBJECT_TYPE_FLOAT:
						t="float";
						break;
					case SLL_RUNTIME_OBJECT_TYPE_CHAR:
						t="char";
						break;
					case SLL_RUNTIME_OBJECT_TYPE_STRING:
						t="string";
						break;
					case SLL_RUNTIME_OBJECT_TYPE_ARRAY:
						t="array";
						break;
					case SLL_RUNTIME_OBJECT_TYPE_HANDLE:
						t="handle";
						break;
					case SLL_RUNTIME_OBJECT_TYPE_MAP:
						t="map";
						break;
					case RUNTIME_OBJECT_TYPE_FUNCTION_ID:
						t="<function id>";
						break;
					case RUNTIME_OBJECT_TYPE_UNKNOWN:
						t="<unknown>";
						break;
				}
				sll_string_t str;
				sll_object_to_string((const sll_runtime_object_t*const*)&c,1,&str);
				uint32_t j=c->_dbg0|(c->_dbg1<<8);
				if (j!=GC_MAX_DEBUG_ID){
					runtime_object_debug_data_t* dt=*(_gc_dbg_dt+j);
					if (dt->c.fp){
						fprintf(stderr,"%s:%u (%s)",dt->c.fp,dt->c.ln,dt->c.fn);
					}
					else{
						fprintf(stderr,"<unknown>");
					}
					fprintf(stderr,": {type: %s, ref: %u, data: %s}\n  Acquire (%u):\n",t,c->rc,str.v,dt->all);
					for (uint32_t k=0;k<dt->all;k++){
						runtime_object_debug_data_trace_data_t* t_dt=*(dt->al+k);
						fprintf(stderr,"    %s:%u (%s)\n",t_dt->fp,t_dt->ln,t_dt->fn);
					}
					fprintf(stderr,"  Release (%u):\n",dt->rll);
					for (uint32_t k=0;k<dt->rll;k++){
						runtime_object_debug_data_trace_data_t* t_dt=*(dt->rl+k);
						fprintf(stderr,"    %s:%u (%s)\n",t_dt->fp,t_dt->ln,t_dt->fn);
					}
				}
				else{
					fprintf(stderr,"<unknown>: {type: %s, ref: %u, data: %s}\n  Acquire (0):\n  Release (0):\n",t,c->rc,str.v);
				}
				sll_deallocate(str.v);
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
					fputs("\nUnreleased Objects:\n",stderr);
				}
				sll_runtime_object_t* c=k->dt;
				const char* t="<wrong type>";
				switch (SLL_RUNTIME_OBJECT_GET_TYPE(c)){
					case SLL_RUNTIME_OBJECT_TYPE_INT:
						t="int";
						break;
					case SLL_RUNTIME_OBJECT_TYPE_FLOAT:
						t="float";
						break;
					case SLL_RUNTIME_OBJECT_TYPE_CHAR:
						t="char";
						break;
					case SLL_RUNTIME_OBJECT_TYPE_STRING:
						t="string";
						break;
					case SLL_RUNTIME_OBJECT_TYPE_ARRAY:
						t="array";
						break;
					case SLL_RUNTIME_OBJECT_TYPE_HANDLE:
						t="handle";
						break;
					case SLL_RUNTIME_OBJECT_TYPE_MAP:
						t="map";
						break;
					case RUNTIME_OBJECT_TYPE_FUNCTION_ID:
						t="<function id>";
						break;
					case RUNTIME_OBJECT_TYPE_UNKNOWN:
						t="<unknown>";
						break;
				}
				sll_string_t str;
				sll_object_to_string((const sll_runtime_object_t*const*)&c,1,&str);
				uint32_t j=c->_dbg0|(c->_dbg1<<8);
				SLL_ASSERT(j!=GC_MAX_DEBUG_ID);
				runtime_object_debug_data_t* dt=*(_gc_dbg_dt+j);
				fprintf(stderr,"%s: %u (<static>): {type: %s, ref: %u, data: %s}\n  Acquire (%u):\n",k->fp,k->ln,t,c->rc,str.v,dt->all);
				for (uint32_t m=0;m<dt->all;m++){
					runtime_object_debug_data_trace_data_t* t_dt=*(dt->al+m);
					fprintf(stderr,"    %s:%u (%s)\n",t_dt->fp,t_dt->ln,t_dt->fn);
				}
				fprintf(stderr,"  Release (%u):\n",dt->rll);
				for (uint32_t m=0;m<dt->rll;m++){
					runtime_object_debug_data_trace_data_t* t_dt=*(dt->rl+m);
					fprintf(stderr,"    %s:%u (%s)\n",t_dt->fp,t_dt->ln,t_dt->fn);
				}
				sll_deallocate(str.v);
			}
			else{
				sll_remove_debug_data(k->dt);
			}
			i++;
		}
		l++;
	}
	fflush(stderr);
	return (err?SLL_RETURN_ERROR:SLL_RETURN_NO_ERROR);
}
