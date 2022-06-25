#ifndef __SLL__INTERNAL_API_FILE_H__
#define __SLL__INTERNAL_API_FILE_H__ 1
#include <sll/file.h>
#include <sll/gc.h>
#include <sll/types.h>



typedef union _EXTENDED_FILE_DATA{
	sll_file_t struct_;
	sll_file_t* pointer;
} extended_file_data_t;



typedef struct _EXTENDED_FILE{
	extended_file_data_t data;
	sll_reference_count_t rc;
	sll_bool_t is_pointer;
	void* data_pointer;
} extended_file_t;


#endif
