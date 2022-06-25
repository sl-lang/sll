#ifndef __SLL__INTERNAL_LOG_H__
#define __SLL__INTERNAL_LOG_H__ 1
#include <sll/container.h>
#include <sll/log.h>
#include <sll/string.h>
#include <sll/types.h>



typedef struct _LOG_FUNCTION_DATA{
	const sll_string_t name;
	sll_logger_flags_t flags;
} log_function_data_t;



typedef struct _LOG_FILE_DATA{
	const sll_string_t name;
	sll_container_t functions;
	sll_logger_flags_t flags;
} log_file_data_t;



void _log_release_data(void);



#endif
