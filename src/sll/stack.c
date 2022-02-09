#include <sll/_sll_internal.h>
#include <sll/assembly.h>
#include <sll/common.h>
#include <sll/node.h>
#include <sll/platform.h>
#include <sll/types.h>



sll_assembly_instruction_t* _acquire_next_instruction(sll_assembly_data_t* a_dt){
	sll_assembly_instruction_t* o=a_dt->_s.p;
	a_dt->ic++;
	a_dt->_s.c--;
	a_dt->_s.p++;
	if (!a_dt->_s.c){
		void* n=sll_platform_allocate_page(SLL_ROUND_PAGE(ASSEMBLY_INSTRUCTION_STACK_ALLOC_SIZE),0);
		*((void**)(a_dt->_s.e))=n;
		*((void**)n)=NULL;
		sll_assembly_instruction_t* s=(sll_assembly_instruction_t*)((char*)n+sizeof(void*));
		s->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_CHANGE_STACK;
		s->dt._p=a_dt->_s.p-1;
		SLL_ASSERT(a_dt->_s.p->t==SLL_ASSEMBLY_INSTRUCTION_TYPE_CHANGE_STACK);
		a_dt->_s.p->dt._p=s+1;
		a_dt->_s.c=((SLL_ROUND_PAGE(ASSEMBLY_INSTRUCTION_STACK_ALLOC_SIZE)-sizeof(void*)-sizeof(sll_assembly_instruction_t)*2)/sizeof(sll_assembly_instruction_t));
		a_dt->_s.p=s+1;
		s+=a_dt->_s.c+1;
		s->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_CHANGE_STACK;
		s->dt._p=NULL;
		a_dt->_s.e=n;
	}
	return o;
}



sll_node_t* _acquire_next_node(sll_source_file_t* c_dt){
	sll_node_t* o=c_dt->_s.p;
	c_dt->_s.off++;
	c_dt->_s.c--;
	c_dt->_s.p++;
	if (!c_dt->_s.c){
		void* n=sll_platform_allocate_page(SLL_ROUND_PAGE(NODE_STACK_ALLOC_SIZE),0);
		*((void**)(c_dt->_s.e))=n;
		*((void**)n)=NULL;
		sll_node_t* s=(sll_node_t*)((char*)n+sizeof(void*));
		s->t=SLL_NODE_TYPE_CHANGE_STACK;
		s->dt._p=c_dt->_s.p-1;
		SLL_ASSERT(c_dt->_s.p->t==SLL_NODE_TYPE_CHANGE_STACK);
		c_dt->_s.p->dt._p=s+1;
		c_dt->_s.c=((SLL_ROUND_PAGE(NODE_STACK_ALLOC_SIZE)-sizeof(void*)-sizeof(sll_node_t)*2)/sizeof(sll_node_t));
		c_dt->_s.p=s+1;
		s+=c_dt->_s.c+1;
		s->t=SLL_NODE_TYPE_CHANGE_STACK;
		s->dt._p=NULL;
		c_dt->_s.e=n;
	}
	return o;
}



sll_assembly_instruction_t* _get_instruction_at_offset(const sll_assembly_data_t* a_dt,sll_instruction_index_t off){
	sll_instruction_index_t cnt=(sll_instruction_index_t)(((SLL_ROUND_PAGE(ASSEMBLY_INSTRUCTION_STACK_ALLOC_SIZE)-sizeof(void*)-sizeof(sll_assembly_instruction_t)*2)/sizeof(sll_assembly_instruction_t)));
	void* pg=a_dt->_s.s;
	while (off>=cnt){
		pg=*((void**)pg);
		off-=cnt;
	}
	return (sll_assembly_instruction_t*)((char*)pg+sizeof(void*)+sizeof(sll_assembly_instruction_t)*(off+1));
}



sll_node_t* _get_node_at_offset(const sll_source_file_t* c_dt,sll_node_offset_t off){
	sll_node_offset_t cnt=(sll_node_offset_t)(((SLL_ROUND_PAGE(NODE_STACK_ALLOC_SIZE)-sizeof(void*)-sizeof(sll_node_t)*2)/sizeof(sll_node_t)));
	void* pg=c_dt->_s.s;
	while (off>=cnt){
		pg=*((void**)pg);
		off-=cnt;
	}
	return (sll_node_t*)((char*)pg+sizeof(void*)+sizeof(sll_node_t)*(off+1));
}



void _init_assembly_stack(sll_assembly_data_t* a_dt){
	a_dt->_s.s=sll_platform_allocate_page(SLL_ROUND_PAGE(ASSEMBLY_INSTRUCTION_STACK_ALLOC_SIZE),0);
	a_dt->_s.e=a_dt->_s.s;
	*((void**)(a_dt->_s.s))=NULL;
	sll_assembly_instruction_t* s=(sll_assembly_instruction_t*)((char*)(a_dt->_s.s)+sizeof(void*));
	s->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_CHANGE_STACK;
	s->dt._p=NULL;
	a_dt->_s.c=((SLL_ROUND_PAGE(ASSEMBLY_INSTRUCTION_STACK_ALLOC_SIZE)-sizeof(void*)-sizeof(sll_assembly_instruction_t)*2)/sizeof(sll_assembly_instruction_t));
	a_dt->_s.p=s+1;
	s+=a_dt->_s.c+1;
	s->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_CHANGE_STACK;
	s->dt._p=NULL;
}



void _init_node_stack(sll_source_file_t* c_dt){
	c_dt->_s.s=sll_platform_allocate_page(SLL_ROUND_PAGE(NODE_STACK_ALLOC_SIZE),0);
	c_dt->_s.e=c_dt->_s.s;
	*((void**)(c_dt->_s.s))=NULL;
	sll_node_t* s=(sll_node_t*)((char*)(c_dt->_s.s)+sizeof(void*));
	s->t=SLL_NODE_TYPE_CHANGE_STACK;
	s->dt._p=NULL;
	c_dt->_s.c=((SLL_ROUND_PAGE(NODE_STACK_ALLOC_SIZE)-sizeof(void*)-sizeof(sll_node_t)*2)/sizeof(sll_node_t));
	c_dt->_s.p=s+1;
	s+=c_dt->_s.c+1;
	s->t=SLL_NODE_TYPE_CHANGE_STACK;
	s->dt._p=NULL;
}



void _shift_nodes(sll_node_t* o,sll_source_file_t* c_dt,sll_node_offset_t off){
	if (!off){
		return;
	}
	sll_node_t* s=c_dt->_s.p-1;
	if (o->t==SLL_NODE_TYPE_CHANGE_STACK){
		o=o->dt._p;
	}
	if (s->t==SLL_NODE_TYPE_CHANGE_STACK){
		s=s->dt._p;
	}
	for (sll_node_offset_t i=1;i<off;i++){
		_acquire_next_node(c_dt);
	}
	sll_node_t* d=_acquire_next_node(c_dt);
	do{
		*d=*s;
		if (d->t==SLL_NODE_TYPE_FUNC){
			(*(c_dt->ft.dt+d->dt.fn.id))->off+=off;
		}
		s--;
		if (s->t==SLL_NODE_TYPE_CHANGE_STACK){
			s=s->dt._p;
		}
		d--;
		if (d->t==SLL_NODE_TYPE_CHANGE_STACK){
			d=d->dt._p;
		}
	} while (s!=o);
	*d=*s;
}
