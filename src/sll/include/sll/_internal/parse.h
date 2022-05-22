#ifndef __SLL__INTERNAL_PARSE_H__
#define __SLL__INTERNAL_PARSE_H__ 1
#include <sll/ift.h>
#include <sll/node.h>
#include <sll/string_table.h>
#include <sll/types.h>



#define EXTRA_COMPILATION_DATA_IMPORT 1
#define EXTRA_COMPILATION_DATA_EXPORT 2
#define EXTRA_COMPILATION_DATA_VARIABLE_DEFINITION 4

#define NODE_TYPE_UNKNOWN SLL_NODE_TYPE_RESERVED0

#define GET_VAR_INDEX_FLAG_UNKNOWN 1
#define GET_VAR_INDEX_FLAG_ASSIGN 2
#define GET_VAR_INDEX_FLAG_FUNC 4
#define GET_VAR_INDEX_FLAG_TLS 8



typedef struct _SCOPE_DATA{
	bitmap_t* data;
	sll_scope_t length;
	sll_scope_t scope;
} scope_data_t;



typedef struct _NEW_VARIABLE_DATA{
	sll_node_t** data;
	sll_arg_count_t length;
} new_variable_data_t;



typedef struct _EXTRA_COMPILATION_DATA{
	scope_data_t scope;
	sll_compilation_data_t* compilation_data;
	sll_internal_function_table_t* internal_function_table;
	sll_import_resolver_t import_resolver;
	new_variable_data_t* new_variable_data;
	const scope_data_t* non_function_scope;
	sll_string_index_t variable_assignment_name_string_index;
	sll_bool_t is_function;
	sll_file_offset_t* file_line;
} extra_compilation_data_t;



typedef struct _NUMBER_PARSER_STATE{
	sll_integer_t hi;
	sll_integer_t lo;
	sll_integer_t exp;
	__SLL_U32 digit_count;
} number_parser_state_t;



#endif
