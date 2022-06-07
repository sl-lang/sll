#ifndef __SLL_API_FILE_H__
#define __SLL_API_FILE_H__ 1
#include <sll/_size_types.h>
#include <sll/common.h>
#include <sll/error.h>
#include <sll/file.h>
#include <sll/string.h>
#include <sll/types.h>
/**
 * \flags group
 * \name File API
 * \group file-api
 * \desc Docs!
 */



/**
 * \flags type var
 * \name sll_file_handle_t
 * \group file-api
 * \desc Docs!
 * \type __SLL_U32
 */
typedef __SLL_U32 sll_file_handle_t;



/**
 * \flags check_output func
 * \name sll_api_file_close
 * \group file-api
 * \desc Docs!
 * \api D|b
 * \arg sll_file_handle_t handle
 * \ret sll_bool_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_bool_t sll_api_file_close(sll_file_handle_t handle);



/**
 * \flags check_output func
 * \name sll_api_file_copy
 * \group file-api
 * \desc Docs!
 * \api #s#s|Q
 * \arg const sll_string_t* src
 * \arg const sll_string_t* dst
 * \ret sll_error_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_error_t sll_api_file_copy(const sll_string_t* src,const sll_string_t* dst);



/**
 * \flags check_output func
 * \name sll_api_file_delete
 * \group file-api
 * \desc Docs!
 * \api #s|Q
 * \arg const sll_string_t* path
 * \ret sll_error_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_error_t sll_api_file_delete(const sll_string_t* path);



/**
 * \flags check_output func
 * \name sll_api_file_flush
 * \group file-api
 * \desc Docs!
 * \api D|b
 * \arg sll_file_handle_t handle
 * \ret sll_bool_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_bool_t sll_api_file_flush(sll_file_handle_t handle);



/**
 * \flags check_output func
 * \name sll_api_file_from_data
 * \group file-api
 * \desc Docs!
 * \api #sB|~D
 * \arg const sll_string_t* data
 * \arg sll_file_flags_t flags
 * \arg sll_file_handle_t* out
 * \ret sll_error_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_error_t sll_api_file_from_data(const sll_string_t* data,sll_file_flags_t flags,sll_file_handle_t* out);



/**
 * \flags func
 * \name sll_api_file_get_buffer
 * \group file-api
 * \desc Docs!
 * \api D|s
 * \arg sll_file_handle_t handle
 * \arg sll_string_t* out
 */
__SLL_EXTERNAL __SLL_API_CALL void sll_api_file_get_buffer(sll_file_handle_t handle,sll_string_t* out);



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
 * \api D|
 * \arg sll_file_handle_t handle
 */
__SLL_EXTERNAL __SLL_API_CALL void sll_api_file_inc_handle(sll_file_handle_t handle);



/**
 * \flags check_output func
 * \name sll_api_file_open
 * \group file-api
 * \desc Docs!
 * \api #sB|~D
 * \arg const sll_string_t* path
 * \arg sll_file_flags_t flags
 * \arg sll_file_handle_t* out
 * \ret sll_error_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_error_t sll_api_file_open(const sll_string_t* path,sll_file_flags_t flags,sll_file_handle_t* out);



/**
 * \flags check_output func
 * \name sll_api_file_peek
 * \group file-api
 * \desc Docs!
 * \api D|~c
 * \arg sll_file_handle_t handle
 * \arg sll_char_t* out
 * \ret sll_error_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_error_t sll_api_file_peek(sll_file_handle_t handle,sll_char_t* out);



/**
 * \flags check_output func
 * \name sll_api_file_read
 * \group file-api
 * \desc Docs!
 * \api DD|~s
 * \arg sll_file_handle_t handle
 * \arg sll_string_length_t size
 * \arg sll_string_t* out
 * \ret sll_error_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_error_t sll_api_file_read(sll_file_handle_t handle,sll_string_length_t size,sll_string_t* out);



/**
 * \flags check_output func
 * \name sll_api_file_read_char
 * \group file-api
 * \desc Docs!
 * \api D|~c
 * \arg sll_file_handle_t handle
 * \arg sll_char_t* out
 * \ret sll_error_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_error_t sll_api_file_read_char(sll_file_handle_t handle,sll_char_t* out);



/**
 * \flags check_output func
 * \name sll_api_file_rename
 * \group file-api
 * \desc Docs!
 * \api #s#s|Q
 * \arg const sll_string_t* src
 * \arg const sll_string_t* dst
 * \ret sll_error_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_error_t sll_api_file_rename(const sll_string_t* src,const sll_string_t* dst);



/**
 * \flags check_output func
 * \name sll_api_file_std_handle
 * \group file-api
 * \desc Docs!
 * \api c|~D
 * \arg sll_char_t id
 * \arg sll_file_handle_t* out
 * \ret sll_error_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_error_t sll_api_file_std_handle(sll_char_t id,sll_file_handle_t* out);



/**
 * \flags check_output func
 * \name sll_api_file_write
 * \group file-api
 * \desc Docs!
 * \api D#s|~Q
 * \arg sll_file_handle_t handle
 * \arg const sll_string_t* data
 * \arg sll_size_t* out
 * \ret sll_error_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_error_t sll_api_file_write(sll_file_handle_t handle,const sll_string_t* data,sll_size_t* out);



/**
 * \flags check_output func
 * \name sll_file_from_handle
 * \group file-api
 * \desc Desc!
 * \arg sll_file_handle_t handle
 * \ret sll_file_t*
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_file_t* sll_file_from_handle(sll_file_handle_t handle);



/**
 * \flags check_output func
 * \name sll_file_to_handle
 * \group file-api
 * \desc Desc!
 * \arg sll_file_t* f
 * \ret sll_file_handle_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_file_handle_t sll_file_to_handle(sll_file_t* f);



#endif
