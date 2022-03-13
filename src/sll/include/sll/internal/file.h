#ifndef __SLL_INTERNAL_FILE_H__
#define __SLL_INTERNAL_FILE_H__ 1
#include <sll/file.h>
#include <sll/internal/common.h>
#include <sll/types.h>



#define FILE_DYNAMIC_BUFFER_ALLOC_SIZE 65536

#define FILE_FLAG_MEMORY SLL_FILE_FLAG_RESERVED0
#define FILE_FLAG_NO_RELEASE SLL_FILE_FLAG_RESERVED1
#define FILE_FLAG_DYNAMIC_BUFFERS SLL_FILE_FLAG_RESERVED2
#define FILE_BUFFER_SIZE 2097152

#define DISABLE_FILE_HASH 255



typedef union __EXTENDED_FILE_DATA{
	sll_file_t f;
	sll_file_t* p;
} extended_file_data_t;



typedef struct __EXTENDED_FILE{
	extended_file_data_t dt;
	sll_ref_count_t rc;
	sll_bool_t p;
} extended_file_t;



typedef struct __DYNAMIC_BUFFER_CHUNK{
	sll_size_t sz;
	struct __DYNAMIC_BUFFER_CHUNK* n;
	sll_char_t dt[];
} dynamic_buffer_chunk_t;



void _file_end_hash(sll_file_t* f);



void _file_init_std_streams(void);



void _file_release_std_streams(void);



void _file_start_hash(sll_file_t* f);



#endif
