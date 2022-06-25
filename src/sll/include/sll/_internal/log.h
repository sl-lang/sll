#ifndef __SLL__INTERNAL_LOG_H__
#define __SLL__INTERNAL_LOG_H__ 1
#include <sll/container.h>
#include <sll/log.h>
#include <sll/string.h>
#include <sll/types.h>



typedef struct _FUNCTION_LOG_DATA{
	const sll_string_t name;
	sll_logger_flags_t flags;
} function_log_data_t;



typedef struct _FILE_LOG_DATA{
	const sll_string_t name;
	sll_container_t functions;
	sll_logger_flags_t flags;
} file_log_data_t;



void _log_release_data(void);



#endif
