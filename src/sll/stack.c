#include <sll/_internal/common.h>
#include <sll/_internal/stack.h>
#include <sll/assembly.h>
#include <sll/common.h>
#include <sll/data.h>
#include <sll/node.h>
#include <sll/platform/memory.h>
#include <sll/types.h>



static void _request_new_node_page(sll_source_file_t* sf){
	node_stack_page_t* n=sll_platform_allocate_page(SLL_ROUND_PAGE(NODE_STACK_ALLOC_SIZE),0,NULL);
	((node_stack_page_t*)(sf->_s.e))->nxt=n;
	n->nxt=NULL;
	sll_node_t* s=PTR(ADDR(n)+sizeof(node_stack_page_t));
	s->t=SLL_NODE_TYPE_CHANGE_STACK;
	s->dt._p=sf->_s.p-1;
	SLL_ASSERT(sf->_s.p->t==SLL_NODE_TYPE_CHANGE_STACK);
	sf->_s.p->dt._p=s+1;
	sf->_s.c=((SLL_ROUND_PAGE(NODE_STACK_ALLOC_SIZE)-sizeof(node_stack_page_t)-sizeof(sll_node_t)*2)/sizeof(sll_node_t));
	sf->_s.p=s+1;
	s+=sf->_s.c+1;
	s->t=SLL_NODE_TYPE_CHANGE_STACK;
	s->dt._p=NULL;
	sf->_s.e=n;
}



sll_assembly_instruction_t* _acquire_next_instruction(sll_assembly_data_t* a_dt){
	sll_assembly_instruction_t* o=a_dt->_instruction_stack.next_instruction;
	a_dt->instruction_count++;
	a_dt->_instruction_stack.count--;
	a_dt->_instruction_stack.next_instruction++;
	if (!a_dt->_instruction_stack.count){
		assembly_stack_page_t* n=sll_platform_allocate_page(SLL_ROUND_PAGE(ASSEMBLY_INSTRUCTION_STACK_ALLOC_SIZE),0,NULL);
		((assembly_stack_page_t*)(a_dt->_instruction_stack.end))->nxt=n;
		n->nxt=NULL;
		sll_assembly_instruction_t* s=PTR(ADDR(n)+sizeof(assembly_stack_page_t));
		s->type=SLL_ASSEMBLY_INSTRUCTION_TYPE_CHANGE_STACK;
		s->data._next_instruction=a_dt->_instruction_stack.next_instruction-1;
		SLL_ASSERT(a_dt->_instruction_stack.next_instruction->type==SLL_ASSEMBLY_INSTRUCTION_TYPE_CHANGE_STACK);
		a_dt->_instruction_stack.next_instruction->data._next_instruction=s+1;
		a_dt->_instruction_stack.count=((SLL_ROUND_PAGE(ASSEMBLY_INSTRUCTION_STACK_ALLOC_SIZE)-sizeof(assembly_stack_page_t)-sizeof(sll_assembly_instruction_t)*2)/sizeof(sll_assembly_instruction_t));
		a_dt->_instruction_stack.next_instruction=s+1;
		s+=a_dt->_instruction_stack.count+1;
		s->type=SLL_ASSEMBLY_INSTRUCTION_TYPE_CHANGE_STACK;
		s->data._next_instruction=NULL;
		a_dt->_instruction_stack.end=n;
	}
	return o;
}



sll_node_t* _acquire_next_node(sll_source_file_t* sf){
	sll_node_t* o=sf->_s.p;
	sf->_s.off++;
	sf->_s.c--;
	sf->_s.p++;
	if (!sf->_s.c){
		_request_new_node_page(sf);
	}
	return o;
}



void _clone_node_stack(const sll_source_file_t* src_sf,sll_source_file_t* dst_sf){
	_init_node_stack(dst_sf);
	dst_sf->dt=dst_sf->_s.p;
	sll_node_offset_t cnt=((SLL_ROUND_PAGE(NODE_STACK_ALLOC_SIZE)-sizeof(node_stack_page_t)-sizeof(sll_node_t)*2)/sizeof(sll_node_t));
	node_stack_page_t* src=src_sf->_s.s;
	node_stack_page_t* dst=dst_sf->_s.s;
	sll_node_offset_t src_cnt=src_sf->_s.off;
	while (1){
		if (!src->nxt){
			cnt=src_cnt;
		}
		sll_copy_data(PTR(ADDR(src)+sizeof(node_stack_page_t)+sizeof(sll_node_t)),cnt*sizeof(sll_node_t),PTR(ADDR(dst)+sizeof(node_stack_page_t)+sizeof(sll_node_t)));
		dst_sf->_s.off+=cnt;
		dst_sf->_s.p+=cnt;
		src_cnt-=cnt;
		if (!src->nxt){
			return;
		}
		_request_new_node_page(dst_sf);
		src=src->nxt;
		dst=dst->nxt;
	}
}



sll_assembly_instruction_t* _get_instruction_at_offset(const sll_assembly_data_t* a_dt,sll_instruction_index_t off){
	sll_instruction_index_t cnt=(sll_instruction_index_t)(((SLL_ROUND_PAGE(ASSEMBLY_INSTRUCTION_STACK_ALLOC_SIZE)-sizeof(assembly_stack_page_t)-sizeof(sll_assembly_instruction_t)*2)/sizeof(sll_assembly_instruction_t)));
	assembly_stack_page_t* pg=a_dt->_instruction_stack.start;
	while (off>=cnt){
		pg=pg->nxt;
		off-=cnt;
	}
	return PTR(ADDR(pg)+sizeof(assembly_stack_page_t)+sizeof(sll_assembly_instruction_t)*(off+1));
}



sll_node_t* _get_node_at_offset(const sll_source_file_t* sf,sll_node_offset_t off){
	sll_node_offset_t cnt=(sll_node_offset_t)(((SLL_ROUND_PAGE(NODE_STACK_ALLOC_SIZE)-sizeof(node_stack_page_t)-sizeof(sll_node_t)*2)/sizeof(sll_node_t)));
	node_stack_page_t* pg=sf->_s.s;
	while (off>=cnt){
		pg=pg->nxt;
		off-=cnt;
	}
	return PTR(ADDR(pg)+sizeof(node_stack_page_t)+sizeof(sll_node_t)*(off+1));
}



void _init_assembly_stack(sll_assembly_data_t* a_dt){
	a_dt->_instruction_stack.start=sll_platform_allocate_page(SLL_ROUND_PAGE(ASSEMBLY_INSTRUCTION_STACK_ALLOC_SIZE),0,NULL);
	a_dt->_instruction_stack.end=a_dt->_instruction_stack.start;
	((assembly_stack_page_t*)(a_dt->_instruction_stack.start))->nxt=NULL;
	sll_assembly_instruction_t* s=PTR(ADDR(a_dt->_instruction_stack.start)+sizeof(assembly_stack_page_t));
	s->type=SLL_ASSEMBLY_INSTRUCTION_TYPE_CHANGE_STACK;
	s->data._next_instruction=NULL;
	a_dt->_instruction_stack.count=((SLL_ROUND_PAGE(ASSEMBLY_INSTRUCTION_STACK_ALLOC_SIZE)-sizeof(assembly_stack_page_t)-sizeof(sll_assembly_instruction_t)*2)/sizeof(sll_assembly_instruction_t));
	a_dt->_instruction_stack.next_instruction=s+1;
	s+=a_dt->_instruction_stack.count+1;
	s->type=SLL_ASSEMBLY_INSTRUCTION_TYPE_CHANGE_STACK;
	s->data._next_instruction=NULL;
}



void _init_node_stack(sll_source_file_t* sf){
	sf->_s.s=sll_platform_allocate_page(SLL_ROUND_PAGE(NODE_STACK_ALLOC_SIZE),0,NULL);
	sf->_s.e=sf->_s.s;
	((node_stack_page_t*)(sf->_s.s))->nxt=NULL;
	sll_node_t* s=PTR(ADDR(sf->_s.s)+sizeof(node_stack_page_t));
	s->t=SLL_NODE_TYPE_CHANGE_STACK;
	s->dt._p=NULL;
	sf->_s.c=((SLL_ROUND_PAGE(NODE_STACK_ALLOC_SIZE)-sizeof(node_stack_page_t)-sizeof(sll_node_t)*2)/sizeof(sll_node_t));
	sf->_s.p=s+1;
	sf->_s.off=0;
	s+=sf->_s.c+1;
	s->t=SLL_NODE_TYPE_CHANGE_STACK;
	s->dt._p=NULL;
}
