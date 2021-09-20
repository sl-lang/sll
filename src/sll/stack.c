#include <sll/_sll_internal.h>
#include <sll/common.h>
#include <sll/platform.h>
#include <sll/types.h>



sll_object_t* _acquire_next_object(sll_compilation_data_t* c_dt){
	sll_object_t* o=c_dt->_s.p;
	c_dt->_s.off++;
	c_dt->_s.c--;
	c_dt->_s.p++;
	if (!c_dt->_s.c){
		sll_page_size_t sz=sll_platform_get_page_size()*OBJECT_STACK_PAGE_ALLOC_COUNT;
		void* n=sll_platform_allocate_page(sz);
		c_dt->_s.sz++;
		void** h=(void**)(c_dt->_s.e);
		*h=(void*)(((uint64_t)(*h))^((uint64_t)n));
		*((void**)n)=c_dt->_s.e;
		sll_object_t* s=(sll_object_t*)((char*)n+sizeof(void*));
		s->t=OBJECT_TYPE_NEXT_STACK;
		s->dt._p=c_dt->_s.p-1;
		SLL_ASSERT(c_dt->_s.p->t==OBJECT_TYPE_NEXT_STACK);
		c_dt->_s.p->dt._p=s+1;
		c_dt->_s.c=(uint32_t)(((sz-sizeof(void*)-sizeof(sll_object_t)*2)/sizeof(sll_object_t)));
		c_dt->_s.p=s+1;
		s+=c_dt->_s.c+1;
		s->t=OBJECT_TYPE_NEXT_STACK;
		s->dt._p=NULL;
		c_dt->_s.e=n;
	}
	return o;
}



sll_object_t* _get_object_at_offset(const sll_compilation_data_t* c_dt,sll_object_offset_t off){
	sll_page_size_t sz=sll_platform_get_page_size()*OBJECT_STACK_PAGE_ALLOC_COUNT;
	sll_object_offset_t cnt=(sll_object_offset_t)(((sz-sizeof(void*)-sizeof(sll_object_t)*2)/sizeof(sll_object_t)));
	void* pg=c_dt->_s.s;
	void* p_pg=NULL;
	while (off>=cnt){
		void* n=(void*)(((uint64_t)(*((void**)pg)))^((uint64_t)p_pg));
		p_pg=pg;
		pg=n;
		off-=cnt;
	}
	return (sll_object_t*)((char*)pg+sizeof(void*)+sizeof(sll_object_t)*(off+1));
}



void _init_object_stack(sll_compilation_data_t* c_dt){
	sll_page_size_t sz=sll_platform_get_page_size()*OBJECT_STACK_PAGE_ALLOC_COUNT;
	c_dt->_s.s=sll_platform_allocate_page(sz);
	c_dt->_s.sz=1;
	c_dt->_s.e=c_dt->_s.s;
	void** h=(void**)(c_dt->_s.s);
	*h=NULL;
	sll_object_t* s=(sll_object_t*)((char*)(c_dt->_s.s)+sizeof(void*));
	s->t=OBJECT_TYPE_NEXT_STACK;
	s->dt._p=NULL;
	c_dt->_s.c=(uint32_t)(((sz-sizeof(void*)-sizeof(sll_object_t)*2)/sizeof(sll_object_t)));
	c_dt->_s.p=s+1;
	s+=c_dt->_s.c+1;
	s->t=OBJECT_TYPE_NEXT_STACK;
	s->dt._p=NULL;
}



void _shift_objects(sll_object_t* o,sll_compilation_data_t* c_dt,sll_object_offset_t off){
	if (!off){
		return;
	}
	sll_object_t* s=c_dt->_s.p-1;
	if (s->t==OBJECT_TYPE_NEXT_STACK){
		s=s->dt._p;
	}
	for (sll_object_offset_t i=1;i<off;i++){
		_acquire_next_object(c_dt);
	}
	sll_object_t* d=_acquire_next_object(c_dt);
	do{
		*d=*s;
		if (d->t==SLL_OBJECT_TYPE_FUNC){
			(*(c_dt->ft.dt+d->dt.fn.id))->off+=off;
		}
		s--;
		if (s->t==OBJECT_TYPE_NEXT_STACK){
			s=s->dt._p;
		}
		d--;
		if (d->t==OBJECT_TYPE_NEXT_STACK){
			d=d->dt._p;
		}
	} while (s!=o);
	*d=*s;
}



__SLL_FUNC void sll_set_assembly_data_stack(sll_assembly_data_t* a_dt,sll_buffer_t bf,sll_stack_offset_t sz){
	a_dt->_s.ptr=bf;
	a_dt->_s.off=0;
	a_dt->_s.sz=sz;
}



__SLL_FUNC void sll_setup_stack(sll_stack_data_t* o,sll_buffer_t bf,sll_stack_offset_t sz){
	o->ptr=bf;
	o->off=0;
	o->sz=sz;
}
