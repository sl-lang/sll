#ifndef __SLL__INTERNAL_FILE_H__
#define __SLL__INTERNAL_FILE_H__ 1
#include <sll/_internal/common.h>
#include <sll/file.h>
#include <sll/gc.h>
#include <sll/types.h>



#define FILE_DYNAMIC_BUFFER_ALLOC_SIZE 65536

#define FILE_FLAG_MEMORY SLL_FILE_FLAG_RESERVED0
#define FILE_FLAG_NO_RELEASE SLL_FILE_FLAG_RESERVED1
#define FILE_FLAG_DYNAMIC_BUFFERS SLL_FILE_FLAG_RESERVED2
#define FILE_BUFFER_SIZE 2097152

#define FILE_BUFFER_POOL_SIZE 32

#define DISABLE_FILE_HASH 255



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



typedef struct _DYNAMIC_BUFFER_CHUNK{
	sll_size_t size;
	struct _DYNAMIC_BUFFER_CHUNK* next;
	sll_char_t data[];
} dynamic_buffer_chunk_t;



void _file_end_hash(sll_file_t* f);



void _file_init_std_streams(void);



void _file_release_std_streams(void);



void _file_start_hash(sll_file_t* f);



#endif
