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
	((node_stack_page_t*)(sf->_stack.end))->next=n;
	n->next=NULL;
	sll_node_t s=PTR(ADDR(n)+sizeof(node_stack_page_t));
	s->type=SLL_NODE_TYPE_CHANGE_STACK;
	s->data._next_node=sf->_stack.next_node-1;
	SLL_ASSERT(sf->_stack.next_node->type==SLL_NODE_TYPE_CHANGE_STACK);
	sf->_stack.next_node->data._next_node=s+1;
	sf->_stack.count=((SLL_ROUND_PAGE(NODE_STACK_ALLOC_SIZE)-sizeof(node_stack_page_t)-sizeof(struct _SLL_NODE)*2)/sizeof(struct _SLL_NODE));
	sf->_stack.next_node=s+1;
	s+=sf->_stack.count+1;
	s->type=SLL_NODE_TYPE_CHANGE_STACK;
	s->data._next_node=NULL;
	sf->_stack.end=n;
}



sll_assembly_instruction_t _acquire_next_instruction(sll_assembly_data_t* assembly_data){
	sll_assembly_instruction_t out=assembly_data->_instruction_stack.next_instruction;
	assembly_data->instruction_count++;
	assembly_data->_instruction_stack.count--;
	assembly_data->_instruction_stack.next_instruction++;
	if (!assembly_data->_instruction_stack.count){
		assembly_stack_page_t* n=sll_platform_allocate_page(SLL_ROUND_PAGE(ASSEMBLY_INSTRUCTION_STACK_ALLOC_SIZE),0,NULL);
		((assembly_stack_page_t*)(assembly_data->_instruction_stack.end))->next=n;
		n->next=NULL;
		sll_assembly_instruction_t s=PTR(ADDR(n)+sizeof(assembly_stack_page_t));
		s->type=SLL_ASSEMBLY_INSTRUCTION_TYPE_CHANGE_STACK;
		s->data._next_instruction=assembly_data->_instruction_stack.next_instruction-1;
		SLL_ASSERT(assembly_data->_instruction_stack.next_instruction->type==SLL_ASSEMBLY_INSTRUCTION_TYPE_CHANGE_STACK);
		assembly_data->_instruction_stack.next_instruction->data._next_instruction=s+1;
		assembly_data->_instruction_stack.count=((SLL_ROUND_PAGE(ASSEMBLY_INSTRUCTION_STACK_ALLOC_SIZE)-sizeof(assembly_stack_page_t)-sizeof(struct _SLL_ASSEMBLY_INSTRUCTION)*2)/sizeof(struct _SLL_ASSEMBLY_INSTRUCTION));
		assembly_data->_instruction_stack.next_instruction=s+1;
		s+=assembly_data->_instruction_stack.count+1;
		s->type=SLL_ASSEMBLY_INSTRUCTION_TYPE_CHANGE_STACK;
		s->data._next_instruction=NULL;
		assembly_data->_instruction_stack.end=n;
	}
	return out;
}



sll_node_t _acquire_next_node(sll_source_file_t* sf){
	sll_node_t out=sf->_stack.next_node;
	sf->_stack.offset++;
	sf->_stack.count--;
	sf->_stack.next_node++;
	if (!sf->_stack.count){
		_request_new_node_page(sf);
	}
	return out;
}



void _clone_node_stack(const sll_source_file_t* src_sf,sll_source_file_t* dst_sf){
	_init_node_stack(dst_sf);
	dst_sf->first_node=dst_sf->_stack.next_node;
	sll_node_offset_t count=((SLL_ROUND_PAGE(NODE_STACK_ALLOC_SIZE)-sizeof(node_stack_page_t)-sizeof(struct _SLL_NODE)*2)/sizeof(struct _SLL_NODE));
	node_stack_page_t* src=src_sf->_stack.start;
	node_stack_page_t* dst=dst_sf->_stack.start;
	sll_node_offset_t src_cnt=src_sf->_stack.offset;
	while (1){
		if (!src->next){
			count=src_cnt;
		}
		sll_copy_data(PTR(ADDR(src)+sizeof(node_stack_page_t)+sizeof(struct _SLL_NODE)),count*sizeof(struct _SLL_NODE),PTR(ADDR(dst)+sizeof(node_stack_page_t)+sizeof(struct _SLL_NODE)));
		dst_sf->_stack.offset+=count;
		dst_sf->_stack.next_node+=count;
		src_cnt-=count;
		if (!src->next){
			return;
		}
		_request_new_node_page(dst_sf);
		src=src->next;
		dst=dst->next;
	}
}



sll_assembly_instruction_t _get_instruction_at_offset(const sll_assembly_data_t* assembly_data,sll_instruction_index_t off){
	sll_instruction_index_t count=(sll_instruction_index_t)(((SLL_ROUND_PAGE(ASSEMBLY_INSTRUCTION_STACK_ALLOC_SIZE)-sizeof(assembly_stack_page_t)-sizeof(struct _SLL_ASSEMBLY_INSTRUCTION)*2)/sizeof(struct _SLL_ASSEMBLY_INSTRUCTION)));
	assembly_stack_page_t* pg=assembly_data->_instruction_stack.start;
	while (off>=count){
		pg=pg->next;
		off-=count;
	}
	return PTR(ADDR(pg)+sizeof(assembly_stack_page_t)+sizeof(struct _SLL_ASSEMBLY_INSTRUCTION)*(off+1));
}



sll_node_t _get_node_at_offset(const sll_source_file_t* sf,sll_node_offset_t off){
	sll_node_offset_t count=(sll_node_offset_t)(((SLL_ROUND_PAGE(NODE_STACK_ALLOC_SIZE)-sizeof(node_stack_page_t)-sizeof(struct _SLL_NODE)*2)/sizeof(struct _SLL_NODE)));
	node_stack_page_t* pg=sf->_stack.start;
	while (off>=count){
		pg=pg->next;
		off-=count;
	}
	return PTR(ADDR(pg)+sizeof(node_stack_page_t)+sizeof(struct _SLL_NODE)*(off+1));
}



void _init_assembly_stack(sll_assembly_data_t* assembly_data){
	assembly_data->_instruction_stack.start=sll_platform_allocate_page(SLL_ROUND_PAGE(ASSEMBLY_INSTRUCTION_STACK_ALLOC_SIZE),0,NULL);
	assembly_data->_instruction_stack.end=assembly_data->_instruction_stack.start;
	((assembly_stack_page_t*)(assembly_data->_instruction_stack.start))->next=NULL;
	sll_assembly_instruction_t s=PTR(ADDR(assembly_data->_instruction_stack.start)+sizeof(assembly_stack_page_t));
	s->type=SLL_ASSEMBLY_INSTRUCTION_TYPE_CHANGE_STACK;
	s->data._next_instruction=NULL;
	assembly_data->_instruction_stack.count=((SLL_ROUND_PAGE(ASSEMBLY_INSTRUCTION_STACK_ALLOC_SIZE)-sizeof(assembly_stack_page_t)-sizeof(struct _SLL_ASSEMBLY_INSTRUCTION)*2)/sizeof(struct _SLL_ASSEMBLY_INSTRUCTION));
	assembly_data->_instruction_stack.next_instruction=s+1;
	s+=assembly_data->_instruction_stack.count+1;
	s->type=SLL_ASSEMBLY_INSTRUCTION_TYPE_CHANGE_STACK;
	s->data._next_instruction=NULL;
}



void _init_node_stack(sll_source_file_t* sf){
	sf->_stack.start=sll_platform_allocate_page(SLL_ROUND_PAGE(NODE_STACK_ALLOC_SIZE),0,NULL);
	sf->_stack.end=sf->_stack.start;
	((node_stack_page_t*)(sf->_stack.start))->next=NULL;
	sll_node_t s=PTR(ADDR(sf->_stack.start)+sizeof(node_stack_page_t));
	s->type=SLL_NODE_TYPE_CHANGE_STACK;
	s->data._next_node=NULL;
	sf->_stack.count=((SLL_ROUND_PAGE(NODE_STACK_ALLOC_SIZE)-sizeof(node_stack_page_t)-sizeof(struct _SLL_NODE)*2)/sizeof(struct _SLL_NODE));
	sf->_stack.next_node=s+1;
	sf->_stack.offset=0;
	s+=sf->_stack.count+1;
	s->type=SLL_NODE_TYPE_CHANGE_STACK;
	s->data._next_node=NULL;
}



void _require_node_stack_space(sll_source_file_t* sf,sll_node_t node,sll_node_offset_t size){
	if (!size){
		return;
	}
	sll_node_t source=sf->_stack.next_node-1;
	if (node->type==SLL_NODE_TYPE_CHANGE_STACK){
		node=node->data._next_node;
	}
	if (source->type==SLL_NODE_TYPE_CHANGE_STACK){
		source=source->data._next_node;
	}
	for (sll_node_offset_t i=1;i<size;i++){
		_acquire_next_node(sf);
	}
	sll_node_t dest=_acquire_next_node(sf);
	do{
		*dest=*source;
		if (dest->type==SLL_NODE_TYPE_FUNC){
			(*(sf->function_table.data+dest->data.function.function_index))->offset+=size;
		}
		source--;
		if (source->type==SLL_NODE_TYPE_CHANGE_STACK){
			source=source->data._next_node;
		}
		dest--;
		if (dest->type==SLL_NODE_TYPE_CHANGE_STACK){
			dest=dest->data._next_node;
		}
	} while (source!=node);
	*dest=*source;
}
