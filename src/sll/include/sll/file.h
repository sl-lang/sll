#ifndef __SLL_FILE_H__
#define __SLL_FILE_H__ 1
#include <sll/_size_types.h>
#include <sll/common.h>
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
 * \arg sll_file_t* f
 * \ret sll_file_offset_t
 */
#define SLL_FILE_GET_LINE_NUMBER(f) ((f)->_l_num)



/**
 * \flags func macro
 * \name SLL_FILE_GET_OFFSET
 * \group file
 * \desc Docs!
 * \arg sll_file_t* f
 * \ret sll_file_offset_t
 */
#define SLL_FILE_GET_OFFSET(f) ((f)->_off)



/**
 * \flags macro var
 * \name SLL_UNKNOWN_FILE_DESCRIPTOR
 * \group file
 * \desc Docs!
 * \type sll_file_descriptor_t
 */
#define SLL_UNKNOWN_FILE_DESCRIPTOR ((sll_file_descriptor_t)0xffffffffffffffffull)



/**
 * \flags macro var
 * \name SLL_END_OF_DATA
 * \group file
 * \desc Docs!
 * \type sll_read_char_t
 */
#define SLL_END_OF_DATA __SLL_U16_MAX



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
 * \arg sll_file_t* f
 */
__SLL_EXTERNAL void sll_file_close(sll_file_t* f);



/**
 * \flags check_output func
 * \name sll_file_data_available
 * \group file
 * \desc Docs!
 * \arg sll_file_t* f
 * \ret sll_bool_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_file_data_available(sll_file_t* f);



/**
 * \flags check_output func
 * \name sll_file_flush
 * \group file
 * \subgroup file-io
 * \desc Docs!
 * \arg sll_file_t* f
 * \ret sll_error_t
 */
__SLL_EXTERNAL sll_error_t sll_file_flush(sll_file_t* f);



/**
 * \flags func
 * \name sll_file_from_data
 * \group file
 * \desc Docs!
 * \arg void* p
 * \arg sll_size_t sz
 * \arg sll_file_flags_t f
 * \arg sll_file_t* o
 */
__SLL_EXTERNAL void sll_file_from_data(void* p,sll_size_t sz,sll_file_flags_t f,sll_file_t* o);



/**
 * \flags func
 * \name sll_file_get_buffer
 * \group file
 * \desc Docs!
 * \arg void* p
 * \arg sll_file_t* f
 * \arg sll_string_t* o
 */
__SLL_EXTERNAL void sll_file_get_buffer(sll_file_t* f,sll_string_t* o);



/**
 * \flags func
 * \name sll_file_open
 * \group file
 * \desc Docs!
 * \arg const sll_char_t* nm
 * \arg sll_file_flags_t f
 * \arg sll_file_t* o
 * \ret sll_error_t
 */
__SLL_EXTERNAL sll_error_t sll_file_open(const sll_char_t* nm,sll_file_flags_t f,sll_file_t* o);



/**
 * \flags func
 * \name sll_file_open_descriptor
 * \group file
 * \desc Docs!
 * \arg const sll_char_t* nm
 * \arg sll_file_descriptor_t fd
 * \arg sll_file_flags_t f
 * \arg sll_file_t* o
 */
__SLL_EXTERNAL void sll_file_open_descriptor(const sll_char_t* nm,sll_file_descriptor_t fd,sll_file_flags_t f,sll_file_t* o);



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
 * \arg void* p
 * \arg sll_size_t sz
 * \arg sll_error_t* err
 * \ret sll_size_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_size_t sll_file_read(sll_file_t* f,void* p,sll_size_t sz,sll_error_t* err);



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
 * \arg const void* p
 * \arg sll_size_t sz
 * \arg sll_error_t* err
 * \ret sll_size_t
 */
__SLL_EXTERNAL sll_size_t sll_file_write(sll_file_t* f,const void* p,sll_size_t sz,sll_error_t* err);



/**
 * \flags check_output func
 * \name sll_file_write_char
 * \group file
 * \subgroup file-io
 * \desc Docs!
 * \arg sll_file_t* f
 * \arg sll_char_t c
 * \arg sll_error_t* err
 * \ret sll_bool_t
 */
__SLL_EXTERNAL sll_bool_t sll_file_write_char(sll_file_t* f,sll_char_t c,sll_error_t* err);



/**
 * \flags check_output func
 * \name sll_file_write_char_count
 * \group file
 * \subgroup file-io
 * \desc Docs!
 * \arg sll_file_t* f
 * \arg sll_char_t c
 * \arg sll_size_t n
 * \arg sll_error_t* err
 * \ret sll_size_t
 */
__SLL_EXTERNAL sll_size_t sll_file_write_char_count(sll_file_t* f,sll_char_t c,sll_size_t n,sll_error_t* err);



/**
 * \flags check_output func var_arg
 * \name sll_file_write_format
 * \group file
 * \subgroup file-io
 * \desc Docs!
 * \arg sll_file_t* f
 * \arg const sll_char_t* t
 * \arg sll_error_t* err
 * \ret sll_size_t
 */
__SLL_EXTERNAL sll_size_t sll_file_write_format(sll_file_t* f,const sll_char_t* t,sll_error_t* err,...);



/**
 * \flags check_output func
 * \name sll_file_write_string
 * \group file
 * \subgroup file-io
 * \desc Docs!
 * \arg sll_file_t* f
 * \arg const sll_char_t* p
 * \arg sll_error_t* err
 * \ret sll_size_t
 */
__SLL_EXTERNAL sll_size_t sll_file_write_string(sll_file_t* f,const sll_char_t* p,sll_error_t* err);



#endif
