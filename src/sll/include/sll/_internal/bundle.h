#ifndef __SLL__INTERNAL_BUNDLE_H__
#define __SLL__INTERNAL_BUNDLE_H__ 1
#include <sll/types.h>



#define BUNDLE_DO_NOT_REMAP_IMPORT 0x80000000
#define BUNDLE_GET_INDEX(x) ((*(x))&0x7fffffff)



typedef struct _SOURCE_FILE_WITH_INDEX{
	sll_source_file_t* dt;
	sll_source_file_index_t idx;
} source_file_with_index_t;



typedef struct _SOURCE_FILE_LIST{
	source_file_with_index_t* dt;
	sll_source_file_index_t l;
} source_file_list_t;



#endif
