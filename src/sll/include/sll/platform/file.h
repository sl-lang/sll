#ifndef __SLL_PLATFORM_FILE_H__
#define __SLL_PLATFORM_FILE_H__ 1
#include <sll/common.h>
#include <sll/types.h>
/**
 * \flags subgroup
 * \name Files
 * \group platform
 * \subgroup platform-file
 * \desc Docs!
 */



/**
 * \flags macro var
 * \name SLL_PLATFORM_STREAM_INPUT
 * \group platform
 * \subgroup platform-file
 * \desc Docs!
 * \type sll_char_t
 */
#define SLL_PLATFORM_STREAM_INPUT 0



/**
 * \flags macro var
 * \name SLL_PLATFORM_STREAM_OUTPUT
 * \group platform
 * \subgroup platform-file
 * \desc Docs!
 * \type sll_char_t
 */
#define SLL_PLATFORM_STREAM_OUTPUT 1



/**
 * \flags macro var
 * \name SLL_PLATFORM_STREAM_ERROR
 * \group platform
 * \subgroup platform-file
 * \desc Docs!
 * \type sll_char_t
 */
#define SLL_PLATFORM_STREAM_ERROR 2



/**
 * \flags macro var
 * \name SLL_NO_FILE_SIZE
 * \group platform
 * \subgroup platform-file
 * \desc Docs!
 * \type sll_size_t
 */
#define SLL_NO_FILE_SIZE 0xffffffffffffffffull



/**
 * \flags check_output func
 * \name sll_platform_file_async_read
 * \group platform
 * \subgroup platform-file
 * \desc Docs!
 * \arg sll_file_descriptor_t fd
 * \ret sll_bool_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_platform_file_async_read(sll_file_descriptor_t fd);



/**
 * \flags func
 * \name sll_platform_file_close
 * \group platform
 * \subgroup platform-file
 * \desc Docs!
 * \arg sll_file_descriptor_t fd
 */
__SLL_EXTERNAL void sll_platform_file_close(sll_file_descriptor_t fd);



/**
 * \flags check_output func
 * \name sll_platform_file_data_available
 * \group platform
 * \subgroup platform-file
 * \desc Docs!
 * \arg sll_file_descriptor_t fd
 * \ret sll_bool_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_platform_file_data_available(sll_file_descriptor_t fd);



/**
 * \flags check_output func
 * \name sll_platform_file_open
 * \group platform
 * \subgroup platform-file
 * \desc Docs!
 * \arg const sll_char_t* fp
 * \arg sll_file_flags_t ff
 * \arg sll_error_t* err
 * \ret sll_file_descriptor_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_file_descriptor_t sll_platform_file_open(const sll_char_t* fp,sll_file_flags_t ff,sll_error_t* err);



/**
 * \flags check_output func
 * \name sll_platform_file_read
 * \group platform
 * \subgroup platform-file
 * \desc Docs!
 * \arg sll_file_descriptor_t fd
 * \arg void* p
 * \arg sll_size_t sz
 * \arg sll_error_t* err
 * \ret sll_size_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_size_t sll_platform_file_read(sll_file_descriptor_t fd,void* p,sll_size_t sz,sll_error_t* err);



/**
 * \flags check_output func
 * \name sll_platform_file_seek
 * \group platform
 * \subgroup platform-file
 * \desc Docs!
 * \arg sll_file_descriptor_t fd
 * \arg sll_file_offset_t off
 * \ret sll_error_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_error_t sll_platform_file_seek(sll_file_descriptor_t fd,sll_file_offset_t off);



/**
 * \flags check_output func
 * \name sll_platform_file_size
 * \group platform
 * \subgroup platform-file
 * \desc Docs!
 * \arg sll_file_descriptor_t fd
 * \arg sll_error_t* err
 * \ret sll_size_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_size_t sll_platform_file_size(sll_file_descriptor_t fd,sll_error_t* err);



/**
 * \flags func
 * \name sll_platform_file_write
 * \group platform
 * \subgroup platform-file
 * \desc Docs!
 * \arg sll_file_descriptor_t fd
 * \arg const void* p
 * \arg sll_size_t sz
 * \arg sll_error_t* err
 * \ret sll_size_t
 */
__SLL_EXTERNAL sll_size_t sll_platform_file_write(sll_file_descriptor_t fd,const void* p,sll_size_t sz,sll_error_t* err);



/**
 * \flags check_output func
 * \name sll_platform_get_default_stream_descriptor
 * \group platform
 * \subgroup platform-file
 * \desc Docs!
 * \arg sll_char_t t
 * \ret sll_file_descriptor_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_file_descriptor_t sll_platform_get_default_stream_descriptor(sll_char_t t);



#endif
