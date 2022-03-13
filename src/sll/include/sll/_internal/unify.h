#ifndef __SLL_INTERNAL_UNIFY_H__
#define __SLL_INTERNAL_UNIFY_H__ 1
#include <sll/types.h>



typedef struct __SOURCE_FILE_MAPPING_DATA{
	sll_string_index_t* sm;
	sll_function_index_t f_id_off;
	sll_scope_t sc_off;
	sll_identifier_index_t* idx_off[SLL_MAX_SHORT_IDENTIFIER_LENGTH+1];
} source_file_mapping_data_t;



#endif
