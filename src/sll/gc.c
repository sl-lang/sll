#include <sll/_sll_internal.h>
#include <sll/assembly.h>
#include <sll/common.h>
#include <sll/constants.h>
#include <sll/core.h>
#include <sll/gc.h>
#include <sll/handle.h>
#include <sll/platform.h>
#include <sll/string.h>
#include <sll/types.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>



static void* _gc_page_ptr=NULL;
static sll_runtime_object_t* _gc_next_object=NULL;
static uint32_t _gc_dbg_dtl=0;
static runtime_object_debug_data_t** _gc_dbg_dt=NULL;
static uint32_t _gc_alloc=0;
static uint32_t _gc_dealloc=0;
static uint8_t _gc_verify=1;



static void _gc_free_pages(void){
	if (_gc_verify){
		sll_runtime_object_stack_data_t rst={
			0,
			NULL,
			0
		};
		if (!sll_verify_runtime_object_stack_cleanup(&rst)){
			SLL_UNIMPLEMENTED();
		}
		_gc_verify=0;
	}
	SLL_ASSERT(_gc_alloc==_gc_dealloc);
	sll_page_size_t sz=sll_platform_get_page_size();
	void* c=_gc_page_ptr;
	while (c){
		void* n=*((void**)c);
		sll_platform_free_page(c,sz*(n?1:GC_INIT_PAGE_COUNT));
		c=n;
	}
}



__SLL_FUNC sll_runtime_object_t* sll__add_debug_data(sll_runtime_object_t* o,const char* fp,unsigned int ln,const char* fn){
	uint32_t i=0;
	while (i<_gc_dbg_dtl){
		if (!(*(_gc_dbg_dt+i))){
			goto _found_index;
		}
		i++;
	}
	_gc_dbg_dtl++;
	SLL_ASSERT(_gc_dbg_dtl<GC_MAX_DBG_ID);
	_gc_dbg_dt=realloc(_gc_dbg_dt,_gc_dbg_dtl*sizeof(runtime_object_debug_data_t*));
_found_index:
	o->_dbg0=i&0xffff;
	o->_dbg1=i>>16;
	runtime_object_debug_data_t* dt=malloc(sizeof(runtime_object_debug_data_t));
	dt->fp=fp;
	dt->ln=ln;
	uint8_t j=0;
	while (*(fn+j)){
		dt->fn[j]=*(fn+j);
		j++;
		SLL_ASSERT(j);
	}
	dt->fn[j]=0;
	*(_gc_dbg_dt+i)=dt;
	return o;
}



__SLL_FUNC sll_runtime_object_t* sll_create_object(void){
	if (!_gc_next_object){
		sll_page_size_t sz=sll_platform_get_page_size();
		if (!_gc_page_ptr){
			sz*=GC_INIT_PAGE_COUNT;
			sll_register_cleanup(_gc_free_pages,CLEANUP_ORDER_LAST);
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
	o->_dbg0=0xffff;
	o->_dbg1=0xff;
	_gc_alloc++;
	_gc_verify=1;
	return o;
}



__SLL_FUNC void sll_get_runtime_object_stack_data(sll_runtime_object_stack_data_t* o){
	o->off=_gc_alloc-_gc_dealloc;
	o->m=NULL;
	o->ml=0;
	sll_page_size_t sz=sll_platform_get_page_size();
	void* pg=_gc_page_ptr;
	uint32_t i=0;
	while (pg){
		sll_runtime_object_t* c=(sll_runtime_object_t*)((uint64_t)pg+sizeof(void*));
		void* e=(void*)((uint64_t)pg+sizeof(void*)+(sz*(*((void**)pg)?1:GC_INIT_PAGE_COUNT)-sizeof(void*))/sizeof(sll_runtime_object_t)*sizeof(sll_runtime_object_t));
		while ((void*)c<e){
			if (c->rc){
				if ((i>>6)+1>o->ml){
					uint32_t j=o->ml;
					o->ml=(i>>6)+1;
					o->m=realloc(o->m,o->ml*sizeof(uint64_t));
					while (j<o->ml){
						*(o->m+j)=0;
						j++;
					}
				}
				*(o->m+(i>>6))|=1ull<<(i&63);
			}
			i++;
			c++;
		}
		pg=*((void**)pg);
	}
}



__SLL_FUNC void sll_release_object(sll_runtime_object_t* o){
	SLL_ASSERT(o->rc);
	o->rc--;
	if (!o->rc){
		uint32_t i=o->_dbg0|(o->_dbg1<<16);
		if (i!=GC_MAX_DBG_ID){
			SLL_ASSERT(i<_gc_dbg_dtl);
			free(*(_gc_dbg_dt+i));
			*(_gc_dbg_dt+i)=NULL;
		}
		if (o->t==SLL_RUNTIME_OBJECT_TYPE_STRING){
			free(o->dt.s.v);
		}
		else if (o->t==SLL_RUNTIME_OBJECT_TYPE_ARRAY){
			for (sll_array_length_t j=0;j<o->dt.a.l;j++){
				sll_release_object(*(o->dt.a.v+j));
			}
			free(o->dt.a.v);
		}
		else if (o->t==SLL_RUNTIME_OBJECT_TYPE_HANDLE){
			if (sll_current_runtime_data){
				sll_handle_descriptor_t* hd=SLL_LOOKUP_HANDLE_DESCRIPTOR(sll_current_runtime_data->hl,o->dt.h.t);
				hd->df(o->dt.h.h);
			}
		}
		else if (o->t==SLL_RUNTIME_OBJECT_TYPE_MAP){
			for (sll_map_length_t j=0;j<o->dt.m.l;j++){
				sll_release_object(*(o->dt.m.v+j));
			}
			free(o->dt.m.v);
		}
		o->t=SLL_OBJECT_TYPE_INT;
		o->dt.i=0;
		GC_SET_NEXT_OBJECT(o,_gc_next_object);
		_gc_next_object=o;
		_gc_dealloc++;
	}
}



__SLL_FUNC __SLL_RETURN sll_verify_runtime_object_stack_cleanup(const sll_runtime_object_stack_data_t* rst){
	if (_gc_alloc-_gc_dealloc<=rst->off){
		return SLL_RETURN_NO_ERROR;
	}
	_gc_verify=0;
	fflush(stdout);
	sll_page_size_t sz=sll_platform_get_page_size();
	fprintf(stderr,"\n%u Unreleased Object%s:\n",_gc_alloc-_gc_dealloc-rst->off,(_gc_alloc-_gc_dealloc-rst->off==1?"":"s"));
	void* pg=_gc_page_ptr;
	uint32_t i=0;
	while (pg){
		sll_runtime_object_t* c=(sll_runtime_object_t*)((uint64_t)pg+sizeof(void*));
		void* e=(void*)((uint64_t)pg+sizeof(void*)+(sz*(*((void**)pg)?1:GC_INIT_PAGE_COUNT)-sizeof(void*))/sizeof(sll_runtime_object_t)*sizeof(sll_runtime_object_t));
		while ((void*)c<e){
			if (c->rc&&((i>>6)>=rst->ml||!(*(rst->m+(i>>6))&(1ull<<(i&63))))){
				const char* t="<unknown>";
				switch (c->t){
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
				}
				sll_string_t str;
				sll_object_to_string((const sll_runtime_object_t*const*)&c,1,&str);
				uint32_t j=c->_dbg0|(c->_dbg1<<16);
				if (j!=GC_MAX_DBG_ID){
					runtime_object_debug_data_t* dt=*(_gc_dbg_dt+j);
					fprintf(stderr,"%s:%u (%s): {type: %s, ref: %u, data: %s}\n",dt->fp,dt->ln,dt->fn,t,c->rc,str.v);
				}
				else{
					fprintf(stderr,"<unknown>: {type: %s, ref: %u, data: %s}\n",t,c->rc,str.v);
				}
				free(str.v);
			}
			i++;
			c++;
		}
		pg=*((void**)pg);
	}
	fflush(stderr);
	return SLL_RETURN_ERROR;
}
