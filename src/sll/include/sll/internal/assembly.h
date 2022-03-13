#ifndef __SLL_INTERNAL_
#define __SLL_INTERNAL_ 1
#include <sll/internal/common.h>
#include <sll/assembly.h>
#include <sll/types.h>



#define VARIABLE_OFFSET_NEVER_DELETE ((void*)0xffffffffffffffffull)

#define ASSEMBLY_INSTRUCTION_TYPE_DBG SLL_ASSEMBLY_INSTRUCTION_TYPE_RESERVED0
#define ASSEMBLY_INSTRUCTION_TYPE_DBG_FUNC SLL_ASSEMBLY_INSTRUCTION_TYPE_RESERVED1
#define ASSEMBLY_INSTRUCTION_TYPE_FUNC_START SLL_ASSEMBLY_INSTRUCTION_TYPE_RESERVED2
#define ASSEMBLY_INSTRUCTION_TYPE_LABEL_TARGET SLL_ASSEMBLY_INSTRUCTION_TYPE_RESERVED3
#define ASSEMBLY_INSTRUCTION_TYPE_NOP SLL_ASSEMBLY_INSTRUCTION_TYPE_RESERVED4
#define ASSEMBLY_INSTRUCTION_FLAG_LABEL 128
#define ASSEMBLY_INSTRUCTION_MISC_FIELD(ai) ((ai)->dt.j)
#define ASSEMBLY_INSTRUCTION_GET_LABEL_DATA(lbl) ((lbl)&0x7fffffff)
#define ASSEMBLY_INSTRUCTION_FLAG_LABEL_REF 0x80000000

#define MAX_ASSEMBLY_INSTRUCTION_LABEL SLL_MAX_INSTRUCTION_INDEX

#define NOT_FIELD(o) SLL_ASSERT((o)->t!=SLL_NODE_TYPE_FIELD)



typedef sll_instruction_index_t assembly_instruction_label_t;



typedef struct __IDENTIFIER_DATA{
	sll_variable_index_t v;
	sll_bool_t rm;
} identifier_data_t;



typedef struct __IDENTIFIER_MAP_DATA{
	identifier_data_t* s_im[SLL_MAX_SHORT_IDENTIFIER_LENGTH];
	identifier_data_t* l_im;
	sll_scope_t l_sc;
	sll_variable_index_t* sc_vi;
	sll_variable_index_t vc;
} identifier_map_data_t;



typedef struct __IDENTIFIER_REMOVE_DATA{
	void** s[SLL_MAX_SHORT_IDENTIFIER_LENGTH];
	void** l;
} identifier_remove_data_t;



typedef struct __LOOP{
	assembly_instruction_label_t cnt;
	assembly_instruction_label_t brk;
} loop_t;



typedef struct __ASSEMBLY_GENERATOR_DATA{
	sll_assembly_data_t* a_dt;
	const sll_source_file_t* sf;
	identifier_map_data_t it;
	assembly_instruction_label_t n_lbl;
	identifier_remove_data_t rm;
	loop_t l_dt;
	assembly_instruction_label_t rt;
	sll_stack_offset_t s_off;
} assembly_generator_data_t;



typedef struct __ASSEMBLY_LOOP_GENERATOR_DATA{
	loop_t p_l_dt;
	bitmap_t* v_st;
} assembly_loop_generator_data_t;



typedef struct __STRING_MAP_DATA{
	sll_string_index_t ml;
	bitmap_t* m;
	sll_string_index_t* im;
} strint_map_data_t;



typedef struct __NODE_LABEL{
	const sll_node_t* o;
	assembly_instruction_label_t l;
} node_label_t;



typedef struct __FILE_LINE{
	sll_file_offset_t ln;
	sll_string_index_t fp;
} file_line_t;



#endif
