#ifndef __SLL_API_FILE_H__
#define __SLL_API_FILE_H__ 1
#include <sll/common.h>
#include <sll/types.h>
/**
 * \flags group
 * \name File API
 * \group file-api
 * \desc Docs!
 */



/**
 * \flags check_output func
 * \name sll_file_from_handle
 * \group file-api
 * \desc Desc!
 * \arg sll_integer_t h
 * \ret sll_file_t*
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_file_t* sll_file_from_handle(sll_integer_t h);



/**
 * \flags check_output func
 * \name sll_file_to_handle
 * \group file-api
 * \desc Desc!
 * \arg sll_file_t* f
 * \ret sll_integer_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_integer_t sll_file_to_handle(sll_file_t* f);



/**
 * \flags func
 * \name sll_api_file_close
 * \group file-api
 * \desc Docs!
 * \api i|b
 * \arg sll_integer_t fh
 * \ret sll_bool_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_bool_t sll_api_file_close(sll_integer_t fh);



/**
 * \flags func
 * \name sll_api_file_copy
 * \group file-api
 * \desc Docs!
 * \api ss|i
 * \arg sll_string_t* src
 * \arg sll_string_t* dst
 * \ret sll_integer_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_integer_t sll_api_file_copy(sll_string_t* src,sll_string_t* dst);



/**
 * \flags func
 * \name sll_api_file_delete
 * \group file-api
 * \desc Docs!
 * \api s|i
 * \arg sll_string_t* path
 * \ret sll_integer_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_integer_t sll_api_file_delete(sll_string_t* path);



/**
 * \flags func
 * \name sll_api_file_flush
 * \group file-api
 * \desc Docs!
 * \api i|b
 * \arg sll_integer_t fh
 * \ret sll_bool_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_bool_t sll_api_file_flush(sll_integer_t fh);



/**
 * \flags func
 * \name sll_api_file_from_data
 * \group file-api
 * \desc Docs!
 * \api si|i
 * \arg sll_string_t* data
 * \arg sll_file_flags_t flags
 * \ret sll_integer_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_integer_t sll_api_file_from_data(sll_string_t* data,sll_file_flags_t flags);



/**
 * \flags func
 * \name sll_api_file_get_buffer
 * \group file-api
 * \desc Docs!
 * \api i|s
 * \arg sll_integer_t fh
 * \arg sll_string_t* out
 */
__SLL_EXTERNAL __SLL_API_CALL void sll_api_file_get_buffer(sll_integer_t fh,sll_string_t* out);



/**
 * \flags func
 * \name sll_api_file_get_temp_path
 * \group file-api
 * \desc Docs!
 * \api |s
 * \arg sll_string_t* out
 */
__SLL_EXTERNAL __SLL_API_CALL void sll_api_file_get_temp_path(sll_string_t* out);



/**
 * \flags func
 * \name sll_api_file_inc_handle
 * \group file-api
 * \desc Docs!
 * \api i|v
 * \arg sll_integer_t a
 */
__SLL_EXTERNAL __SLL_API_CALL void sll_api_file_inc_handle(sll_integer_t a);



/**
 * \flags func
 * \name sll_api_file_open
 * \group file-api
 * \desc Docs!
 * \api si|i
 * \arg sll_string_t* path
 * \arg sll_file_flags_t flags
 * \ret sll_integer_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_integer_t sll_api_file_open(sll_string_t* path,sll_file_flags_t flags);



/**
 * \flags func
 * \name sll_api_file_peek
 * \group file-api
 * \desc Docs!
 * \api i|o
 * \arg sll_integer_t fh
 * \ret sll_object_t*
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_object_t* sll_api_file_peek(sll_integer_t fh);



/**
 * \flags func
 * \name sll_api_file_read
 * \group file-api
 * \desc Docs!
 * \api ii|o
 * \arg sll_integer_t fh
 * \arg sll_string_length_t size
 * \ret sll_object_t*
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_object_t* sll_api_file_read(sll_integer_t fh,sll_string_length_t size);



/**
 * \flags func
 * \name sll_api_file_read_char
 * \group file-api
 * \desc Docs!
 * \api i|o
 * \arg sll_integer_t fh
 * \ret sll_object_t*
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_object_t* sll_api_file_read_char(sll_integer_t fh);



/**
 * \flags func
 * \name sll_api_file_rename
 * \group file-api
 * \desc Docs!
 * \api ss|i
 * \arg sll_string_t* src
 * \arg sll_string_t* dst
 * \ret sll_integer_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_integer_t sll_api_file_rename(sll_string_t* src,sll_string_t* dst);



/**
 * \flags func
 * \name sll_api_file_std_handle
 * \group file-api
 * \desc Docs!
 * \api i|i
 * \arg sll_char_t id
 * \ret sll_integer_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_integer_t sll_api_file_std_handle(sll_char_t id);



/**
 * \flags func
 * \name sll_api_file_write
 * \group file-api
 * \desc Docs!
 * \api is|i
 * \arg sll_integer_t fh
 * \arg sll_string_t* data
 * \ret sll_integer_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_integer_t sll_api_file_write(sll_integer_t fh,sll_string_t* data);



#endif
