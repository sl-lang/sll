#ifndef __SLL__INTERNAL_LIBRARY_H__
#define __SLL__INTERNAL_LIBRARY_H__ 1
#include <sll/platform/library.h>
#include <sll/string.h>
#include <sll/types.h>



#define LIBRARY_HASH_BUFFER_SIZE 65536



typedef struct _LIBRARY{
	const sll_string_t name;
	sll_library_handle_t handle;
} library_t;



#endif
