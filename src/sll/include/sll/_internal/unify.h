#ifndef __SLL__INTERNAL_UNIFY_H__
#define __SLL__INTERNAL_UNIFY_H__ 1
#include <sll/string_table.h>
#include <sll/types.h>



typedef struct _SOURCE_FILE_MAPPING_DATA{
	sll_string_index_t* string_map;
	sll_function_index_t function_index_offset;
	sll_scope_t scope_offset;
	sll_identifier_index_t* identifier_index_offset[SLL_MAX_SHORT_IDENTIFIER_LENGTH+1];
} source_file_mapping_data_t;



#endif
