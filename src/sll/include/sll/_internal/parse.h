#ifndef __SLL__INTERNAL_PARSE_H__
#define __SLL__INTERNAL_PARSE_H__ 1
#include <sll/node.h>
#include <sll/types.h>



#define EXTRA_COMPILATION_DATA_IMPORT 1
#define EXTRA_COMPILATION_DATA_EXPORT 2
#define EXTRA_COMPILATION_DATA_VARIABLE_DEFINITION 4

#define NODE_TYPE_UNKNOWN SLL_NODE_TYPE_RESERVED0

#define GET_VAR_INDEX_FLAG_UNKNOWN 1
#define GET_VAR_INDEX_FLAG_ASSIGN 2
#define GET_VAR_INDEX_FLAG_FUNC 4
#define GET_VAR_INDEX_FLAG_TLS 8



typedef struct __SCOPE_DATA{
	bitmap_t* m;
	sll_scope_t l_sc;
	sll_scope_t ml;
} scope_data_t;



typedef struct __NEW_VARIABLE_DATA{
	sll_node_t** dt;
	sll_arg_count_t sz;
} new_variable_data_t;



typedef struct __EXTRA_COMPILATION_DATA{
	scope_data_t sc;
	sll_compilation_data_t* c_dt;
	sll_internal_function_table_t* i_ft;
	sll_import_resolver_t ir;
	new_variable_data_t* nv_dt;
	const scope_data_t* not_fn_sc;
	sll_string_index_t a_nm;
	sll_bool_t fn;
	sll_file_offset_t* ln;
} extra_compilation_data_t;



typedef struct __NUMBER_PARSER_STATE{
	sll_integer_t hi;
	sll_integer_t lo;
	sll_integer_t exp;
	__SLL_U32 cnt;
} number_parser_state_t;



#endif
