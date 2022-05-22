#ifndef __SLL_FILE_H__
#define __SLL_FILE_H__ 1
#include <sll/_file_descriptor.h>
#include <sll/_size_types.h>
#include <sll/api/hash.h>
#include <sll/common.h>
#include <sll/error.h>
#include <sll/platform/lock.h>
#include <sll/string.h>
#include <sll/types.h>
/**
 * \flags group
 * \name File
 * \group file
 * \desc Docs!
 */
/**
 * \flags subgroup
 * \name Access Flags
 * \group file
 * \subgroup file-flag
 * \desc Docs!
 */
/**
 * \flags subgroup
 * \name I/O
 * \group file
 * \subgroup file-io
 * \desc Docs!
 */



/**
 * \flags macro var
 * \name SLL_MAX_FILE_OFFSET
 * \group file
 * \desc Docs!
 * \type sll_file_offset_t
 */
#define SLL_MAX_FILE_OFFSET __SLL_U64_MAX



/**
 * \flags macro var
 * \name SLL_FILE_FLAG_READ
 * \group file
 * \subgroup file-flag
 * \desc Docs!
 * \type sll_file_flags_t
 */
#define SLL_FILE_FLAG_READ 1



/**
 * \flags macro var
 * \name SLL_FILE_FLAG_WRITE
 * \group file
 * \subgroup file-flag
 * \desc Docs!
 * \type sll_file_flags_t
 */
#define SLL_FILE_FLAG_WRITE 2



/**
 * \flags macro var
 * \name SLL_FILE_FLAG_APPEND
 * \group file
 * \subgroup file-flag
 * \desc Docs!
 * \type sll_file_flags_t
 */
#define SLL_FILE_FLAG_APPEND 4



/**
 * \flags macro var
 * \name SLL_FILE_FLAG_NO_BUFFER
 * \group file
 * \subgroup file-flag
 * \desc Docs!
 * \type sll_file_flags_t
 */
#define SLL_FILE_FLAG_NO_BUFFER 8



/**
 * \flags macro var
 * \name SLL_FILE_FLUSH_ON_NEWLINE
 * \group file
 * \subgroup file-flag
 * \desc Docs!
 * \type sll_file_flags_t
 */
#define SLL_FILE_FLUSH_ON_NEWLINE 16



/**
 * \flags macro var
 * \name SLL_FILE_FLAG_ASYNC
 * \group file
 * \subgroup file-flag
 * \desc Docs!
 * \type sll_file_flags_t
 */
#define SLL_FILE_FLAG_ASYNC 32



/**
 * \flags macro var
 * \name SLL_FILE_FLAG_RESERVED0
 * \group file
 * \subgroup file-flag
 * \desc Internally used as `FILE_FLAG_MEMORY`
 * \type sll_file_flags_t
 */
#define SLL_FILE_FLAG_RESERVED0 64



/**
 * \flags macro var
 * \name SLL_FILE_FLAG_RESERVED1
 * \group file
 * \subgroup file-flag
 * \desc Internally used as `FILE_FLAG_NO_RELEASE`
 * \type sll_file_flags_t
 */
#define SLL_FILE_FLAG_RESERVED1 128



/**
 * \flags macro var
 * \name SLL_FILE_FLAG_RESERVED2
 * \group file
 * \subgroup file-flag
 * \desc Internally used as `FILE_FLAG_DYNAMIC_BUFFERS`
 * \type sll_file_flags_t
 */
#define SLL_FILE_FLAG_RESERVED2 256



/**
 * \flags func macro
 * \name SLL_FILE_GET_LINE_NUMBER
 * \group file
 * \desc Docs!
 * \arg sll_file_t* file
 * \ret sll_file_offset_t
 */
#define SLL_FILE_GET_LINE_NUMBER(file) ((file)->_line_number)



/**
 * \flags func macro
 * \name SLL_FILE_GET_OFFSET
 * \group file
 * \desc Docs!
 * \arg sll_file_t* file
 * \ret sll_file_offset_t
 */
#define SLL_FILE_GET_OFFSET(file) ((file)->_offset)



/**
 * \flags macro var
 * \name SLL_END_OF_DATA
 * \group file
 * \desc Docs!
 * \type sll_read_char_t
 */
#define SLL_END_OF_DATA __SLL_U16_MAX



/**
 * \flags type var
 * \name sll_file_flags_t
 * \group file
 * \desc Docs!
 * \type __SLL_U16
 */
typedef __SLL_U16 sll_file_flags_t;



/**
 * \flags type var
 * \name sll_read_char_t
 * \group file
 * \desc Docs!
 * \type __SLL_U16
 */
typedef __SLL_U16 sll_read_char_t;



/**
 * \flags type var
 * \name sll_file_offset_t
 * \group file
 * \desc Docs!
 * \type __SLL_U64
 */
typedef __SLL_U64 sll_file_offset_t;



/**
 * \flags type
 * \name sll_file_data_file_t
 * \group file
 * \desc Docs!
 * \arg const sll_file_descriptor_t fd
 * \arg const sll_string_t path
 */
typedef struct _SLL_FILE_DATA_FILE{
	const sll_file_descriptor_t fd;
	const sll_string_t path;
} sll_file_data_file_t;



/**
 * \flags type
 * \name sll_file_data_memory_t
 * \group file
 * \desc Docs!
 * \arg const void* pointer
 * \arg const sll_size_t size
 */
typedef struct _SLL_FILE_DATA_MEMORY{
	const void* pointer;
	const sll_size_t size;
} sll_file_data_memory_t;



/**
 * \flags type union
 * \name sll_file_data_t
 * \group file
 * \desc Docs!
 * \arg const sll_file_data_file_t file
 * \arg const sll_file_data_memory_t memory
 */
typedef union _SLL_FILE_DATA{
	const sll_file_data_file_t file;
	const sll_file_data_memory_t memory;
} sll_file_data_t;



/**
 * \flags type
 * \name sll_file_write_buffer_static_t
 * \group file
 * \desc Docs!
 * \arg sll_char_t* pointer
 * \arg sll_file_offset_t offset
 */
typedef struct _SLL_FILE_WRITE_BUFFER_STATIC{
	sll_char_t* pointer;
	sll_file_offset_t offset;
} sll_file_write_buffer_static_t;



/**
 * \flags type
 * \name sll_file_write_buffer_dynamic_t
 * \group file
 * \desc Docs!
 * \arg void* b
 * \arg void* t
 * \arg sll_file_offset_t sz
 * \arg sll_file_offset_t off
 */
typedef struct _SLL_FILE_WRITE_BUFFER_DYNAMIC{
	void* start;
	void* end;
	sll_file_offset_t size;
	sll_file_offset_t offset;
} sll_file_write_buffer_dynamic_t;



/**
 * \flags type union
 * \name sll_file_write_buffer_t
 * \group file
 * \desc Docs!
 * \arg sll_file_write_buffer_static_t static_
 * \arg sll_file_write_buffer_dynamic_t dynamic
 */
typedef union _SLL_FILE_WRITE_BUFFER{
	sll_file_write_buffer_static_t static_;
	sll_file_write_buffer_dynamic_t dynamic;
} sll_file_write_buffer_t;



/**
 * \flags type
 * \name sll_file_hash_t
 * \group file
 * \desc Docs!
 * \arg sll_sha256_data_t hash
 * \arg sll_char_t[64] buffer
 * \arg __SLL_U8 buffer_offset
 */
typedef struct _SLL_FILE_HASH{
	sll_sha256_data_t hash;
	sll_char_t buffer[64];
	__SLL_U8 buffer_offset;
} sll_file_hash_t;



/**
 * \flags type
 * \name sll_file_t
 * \group file
 * \desc Docs!
 * \arg const sll_file_data_t source
 * \arg const sll_file_flags_t flags
 * \arg sll_file_offset_t _line_number
 * \arg sll_file_offset_t _offset
 * \arg sll_char_t* _read_buffer
 * \arg sll_file_offset_t _read_buffer_offset
 * \arg sll_file_offset_t _read_buffer_size
 * \arg sll_file_write_buffer_t _write_buffer
 * \arg sll_file_hash_t _hash
 * \arg sll_lock_handle_t _lock
 */
typedef struct _SLL_FILE{
	const sll_file_data_t source;
	const sll_file_flags_t flags;
	sll_file_offset_t _line_number;
	sll_file_offset_t _offset;
	sll_char_t* _read_buffer;
	sll_file_offset_t _read_buffer_offset;
	sll_file_offset_t _read_buffer_size;
	sll_file_write_buffer_t _write_buffer;
	sll_file_hash_t _hash;
	sll_lock_handle_t _lock;
} sll_file_t;



/**
 * \flags var
 * \name sll_stdin
 * \group file
 * \desc Docs!
 * \type sll_file_t*
 */
__SLL_EXTERNAL extern sll_file_t* sll_stdin;



/**
 * \flags var
 * \name sll_stdout
 * \group file
 * \desc Docs!
 * \type sll_file_t*
 */
__SLL_EXTERNAL extern sll_file_t* sll_stdout;



/**
 * \flags var
 * \name sll_stderr
 * \group file
 * \desc Docs!
 * \type sll_file_t*
 */
__SLL_EXTERNAL extern sll_file_t* sll_stderr;



/**
 * \flags func
 * \name sll_file_close
 * \group file
 * \desc Docs!
 * \arg sll_file_t* file
 */
__SLL_EXTERNAL void sll_file_close(sll_file_t* file);



/**
 * \flags check_output func
 * \name sll_file_data_available
 * \group file
 * \desc Docs!
 * \arg sll_file_t* file
 * \ret sll_bool_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_file_data_available(sll_file_t* file);



/**
 * \flags check_output func
 * \name sll_file_flush
 * \group file
 * \subgroup file-io
 * \desc Docs!
 * \arg sll_file_t* file
 * \ret sll_error_t
 */
__SLL_EXTERNAL sll_error_t sll_file_flush(sll_file_t* file);



/**
 * \flags func
 * \name sll_file_from_data
 * \group file
 * \desc Docs!
 * \arg const void* pointer
 * \arg sll_size_t size
 * \arg sll_file_flags_t flags
 * \arg sll_file_t* out
 */
__SLL_EXTERNAL void sll_file_from_data(const void* pointer,sll_size_t size,sll_file_flags_t flags,sll_file_t* out);



/**
 * \flags func
 * \name sll_file_get_buffer
 * \group file
 * \desc Docs!
 * \arg void* p
 * \arg sll_file_t* file
 * \arg sll_string_t* out
 */
__SLL_EXTERNAL void sll_file_get_buffer(sll_file_t* file,sll_string_t* out);



/**
 * \flags func
 * \name sll_file_open
 * \group file
 * \desc Docs!
 * \arg const sll_char_t* name
 * \arg sll_file_flags_t flags
 * \arg sll_file_t* out
 * \ret sll_error_t
 */
__SLL_EXTERNAL sll_error_t sll_file_open(const sll_char_t* name,sll_file_flags_t flags,sll_file_t* out);



/**
 * \flags func
 * \name sll_file_open_descriptor
 * \group file
 * \desc Docs!
 * \arg const sll_char_t* name
 * \arg sll_file_descriptor_t fd
 * \arg sll_file_flags_t flags
 * \arg sll_file_t* out
 */
__SLL_EXTERNAL void sll_file_open_descriptor(const sll_char_t* name,sll_file_descriptor_t fd,sll_file_flags_t flags,sll_file_t* out);



/**
 * \flags check_output func
 * \name sll_file_peek_char
 * \group file
 * \subgroup file-io
 * \desc Docs!
 * \arg sll_file_t* f
 * \arg sll_error_t* err
 * \ret sll_read_char_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_read_char_t sll_file_peek_char(sll_file_t* f,sll_error_t* err);



/**
 * \flags check_output func
 * \name sll_file_read
 * \group file
 * \subgroup file-io
 * \desc Docs!
 * \arg sll_file_t* f
 * \arg void* ptr
 * \arg sll_size_t sz
 * \arg sll_error_t* err
 * \ret sll_size_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_size_t sll_file_read(sll_file_t* f,void* ptr,sll_size_t sz,sll_error_t* err);



/**
 * \flags check_output func
 * \name sll_file_read_all
 * \group file
 * \subgroup file-io
 * \desc Docs!
 * \arg sll_file_t* f
 * \arg sll_string_t* out
 * \ret sll_error_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_error_t sll_file_read_all(sll_file_t* f,sll_string_t* out);



/**
 * \flags func
 * \name sll_file_read_char
 * \group file
 * \subgroup file-io
 * \desc Docs!
 * \arg sll_file_t* f
 * \arg sll_error_t* err
 * \ret sll_read_char_t
 */
__SLL_EXTERNAL sll_read_char_t sll_file_read_char(sll_file_t* f,sll_error_t* err);



/**
 * \flags check_output func
 * \name sll_file_reset
 * \group file
 * \desc Docs!
 * \arg sll_file_t* f
 * \ret sll_error_t
 */
__SLL_EXTERNAL sll_error_t sll_file_reset(sll_file_t* f);



/**
 * \flags check_output func
 * \name sll_file_write
 * \group file
 * \subgroup file-io
 * \desc Docs!
 * \arg sll_file_t* f
 * \arg const void* ptr
 * \arg sll_size_t sz
 * \arg sll_error_t* err
 * \ret sll_size_t
 */
__SLL_EXTERNAL sll_size_t sll_file_write(sll_file_t* f,const void* ptr,sll_size_t sz,sll_error_t* err);



/**
 * \flags check_output func
 * \name sll_file_write_char
 * \group file
 * \subgroup file-io
 * \desc Docs!
 * \arg sll_file_t* f
 * \arg sll_char_t chr
 * \arg sll_error_t* err
 * \ret sll_bool_t
 */
__SLL_EXTERNAL sll_bool_t sll_file_write_char(sll_file_t* f,sll_char_t chr,sll_error_t* err);



/**
 * \flags check_output func
 * \name sll_file_write_char_count
 * \group file
 * \subgroup file-io
 * \desc Docs!
 * \arg sll_file_t* f
 * \arg sll_char_t chr
 * \arg sll_size_t cnt
 * \arg sll_error_t* err
 * \ret sll_size_t
 */
__SLL_EXTERNAL sll_size_t sll_file_write_char_count(sll_file_t* f,sll_char_t chr,sll_size_t cnt,sll_error_t* err);



/**
 * \flags check_output func var_arg
 * \name sll_file_write_format
 * \group file
 * \subgroup file-io
 * \desc Docs!
 * \arg sll_file_t* f
 * \arg const sll_char_t* fmt
 * \arg sll_error_t* err
 * \ret sll_size_t
 */
__SLL_EXTERNAL sll_size_t sll_file_write_format(sll_file_t* f,const sll_char_t* fmt,sll_error_t* err,...);



/**
 * \flags check_output func
 * \name sll_file_write_string
 * \group file
 * \subgroup file-io
 * \desc Docs!
 * \arg sll_file_t* f
 * \arg const sll_char_t* str
 * \arg sll_error_t* err
 * \ret sll_size_t
 */
__SLL_EXTERNAL sll_size_t sll_file_write_string(sll_file_t* f,const sll_char_t* str,sll_error_t* err);



#endif
