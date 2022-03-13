#ifndef __SLL_INTERNAL_LIBRARY_H__
#define __SLL_INTERNAL_LIBRARY_H__ 1
#include <sll/types.h>



#ifdef __SLL_BUILD_WINDOWS
#define LIBRARY_EXTENSION ".dll"
#else
#define LIBRARY_EXTENSION ".so"
#endif

#define LIBRARY_DIRECTORY "/sys_lib/"
#define LIBRARY_HASH_BUFFER_SIZE 4096



typedef struct __LIBRARY{
	const sll_string_t nm;
	sll_library_handle_t h;
} library_t;



#endif
