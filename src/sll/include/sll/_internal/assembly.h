#ifndef __SLL__INTERNAL_ASSEMBLY_H__
#define __SLL__INTERNAL_ASSEMBLY_H__ 1
#include <sll/_internal/common.h>
#include <sll/api/serial.h>
#include <sll/assembly.h>
#include <sll/compression.h>
#include <sll/node.h>
#include <sll/string_table.h>
#include <sll/types.h>



#define VARIABLE_OFFSET_NEVER_DELETE ((void*)0xffffffffffffffffull)

#define ASSEMBLY_INSTRUCTION_TYPE_DBG SLL_ASSEMBLY_INSTRUCTION_TYPE_RESERVED0
#define ASSEMBLY_INSTRUCTION_TYPE_DBG_FUNC SLL_ASSEMBLY_INSTRUCTION_TYPE_RESERVED1
#define ASSEMBLY_INSTRUCTION_TYPE_FUNC_START SLL_ASSEMBLY_INSTRUCTION_TYPE_RESERVED2
#define ASSEMBLY_INSTRUCTION_TYPE_LABEL_TARGET SLL_ASSEMBLY_INSTRUCTION_TYPE_RESERVED3
#define ASSEMBLY_INSTRUCTION_TYPE_NOP SLL_ASSEMBLY_INSTRUCTION_TYPE_RESERVED4
#define ASSEMBLY_INSTRUCTION_FLAG_LABEL 128
#define ASSEMBLY_INSTRUCTION_MISC_FIELD_GET(ai) ADDR((ai)->data._next_instruction)
#define ASSEMBLY_INSTRUCTION_MISC_FIELD_SET(ai,v) ((ai)->data._next_instruction=PTR(v))
#define ASSEMBLY_INSTRUCTION_GET_LABEL_DATA(lbl) ((lbl)&0x7fffffff)
#define ASSEMBLY_INSTRUCTION_FLAG_LABEL_REF 0x80000000

#define MAX_ASSEMBLY_INSTRUCTION_LABEL SLL_MAX_INSTRUCTION_INDEX

#define NOT_FIELD(o) SLL_ASSERT((o)->type!=SLL_NODE_TYPE_FIELD)



typedef sll_instruction_index_t assembly_instruction_label_t;



typedef struct _IDENTIFIER_DATA{
	sll_variable_index_t variable;
	sll_bool_t remove;
} identifier_data_t;



typedef struct _IDENTIFIER_MAP_DATA{
	identifier_data_t* data;
	sll_variable_index_t variable_count;
	sll_variable_index_t tls_variable_count;
} identifier_map_data_t;



typedef struct _LOOP{
	assembly_instruction_label_t continue_;
	assembly_instruction_label_t break_;
} loop_t;



typedef struct _ASSEMBLY_GENERATOR_DATA{
	sll_assembly_data_t* assembly_data;
	const sll_source_file_t* source_file;
	identifier_map_data_t identifier_map;
	assembly_instruction_label_t next_label;
	void** identifier_remove_data;
	loop_t loop_data;
	assembly_instruction_label_t return_label;
	sll_stack_offset_t stack_offset;
	sll_variable_index_t loop_variable_count;
} assembly_generator_data_t;



typedef struct _ASSEMBLY_LOOP_GENERATOR_DATA{
	loop_t last_loop_data;
	bitmap_t* variable_data;
} assembly_loop_generator_data_t;



typedef struct _STRING_MAP_DATA{
	sll_string_index_t map_length;
	bitmap_t* map_data;
	sll_string_index_t* new_indicies;
} strint_map_data_t;



typedef struct _NODE_LABEL{
	const sll_node_t* node;
	assembly_instruction_label_t label;
} node_label_t;



typedef struct _FILE_LINE{
	sll_file_offset_t line;
	sll_string_index_t file_path_string_index;
} file_line_t;



static __SLL_FORCE_INLINE void _assembly_optimize_int(sll_assembly_instruction_t* ai){
	SLL_ASSERT(SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(ai)==SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_INT);
	sll_size_t enc=SLL_ENCODE_SIGNED_INTEGER(ai->data.int_);
	sll_size_t ci=sll_compress_integer(enc);
	if (ci!=enc){
		ai->type=SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_INT_COMPRESSED;
		ai->data.compressed_int=(sll_compressed_integer_t)ci;
	}
}



#endif
