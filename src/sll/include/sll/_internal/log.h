#ifndef __SLL__INTERNAL_LOG_H__
#define __SLL__INTERNAL_LOG_H__ 1
#include <sll/log.h>
#include <sll/string.h>
#include <sll/types.h>



typedef struct _FUNCTION_LOG_DATA{
	const sll_string_t nm;
	sll_logger_flags_t fl;
} function_log_data_t;



typedef struct _FILE_LOG_DATA{
	const sll_string_t nm;
	function_log_data_t** dt;
	sll_array_length_t dtl;
	sll_logger_flags_t fl;
} file_log_data_t;



void _log_release_data(void);



#endif
