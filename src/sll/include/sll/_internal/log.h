#ifndef __SLL_INTERNAL_LOG_H__
#define __SLL_INTERNAL_LOG_H__ 1
#include <sll/types.h>



typedef struct __FUNCTION_LOG_DATA{
	const sll_string_t nm;
	sll_flags_t fl;
} function_log_data_t;



typedef struct __FILE_LOG_DATA{
	const sll_string_t nm;
	function_log_data_t** dt;
	sll_array_length_t dtl;
	sll_flags_t fl;
} file_log_data_t;



void _log_release_data(void);



#endif
