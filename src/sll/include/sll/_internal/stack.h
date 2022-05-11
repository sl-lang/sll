#ifndef __SLL__INTERNAL_STACK_H__
#define __SLL__INTERNAL_STACK_H__ 1
#include <sll/assembly.h>
#include <sll/node.h>
#include <sll/types.h>



#define ASSEMBLY_INSTRUCTION_STACK_ALLOC_SIZE 65536
#define NODE_STACK_ALLOC_SIZE 65536



typedef struct _ASSEMBLY_STACK_PAGE{
	struct _ASSEMBLY_STACK_PAGE* nxt;
} assembly_stack_page_t;



typedef struct _NODE_STACK_PAGE{
	struct _NODE_STACK_PAGE* nxt;
} node_stack_page_t;



sll_assembly_instruction_t* _acquire_next_instruction(sll_assembly_data_t* a_dt);



sll_node_t* _acquire_next_node(sll_source_file_t* sf);



void _clone_node_stack(const sll_source_file_t* src_sf,sll_source_file_t* dst_sf);



sll_assembly_instruction_t* _get_instruction_at_offset(const sll_assembly_data_t* a_dt,sll_instruction_index_t off);



sll_node_t* _get_node_at_offset(const sll_source_file_t* sf,sll_node_offset_t off);



void _init_assembly_stack(sll_assembly_data_t* a_dt);



void _init_node_stack(sll_source_file_t* sf);



#endif
