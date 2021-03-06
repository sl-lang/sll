#ifndef __SLL_PLATFORM_FILE_H__
#define __SLL_PLATFORM_FILE_H__ 1
#include <sll/_file_descriptor.h>
#include <sll/_size_types.h>
#include <sll/common.h>
#include <sll/error.h>
#include <sll/file.h>
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
 * \type sll_std_file_type_t
 */
#define SLL_PLATFORM_STREAM_INPUT 0



/**
 * \flags macro var
 * \name SLL_PLATFORM_STREAM_OUTPUT
 * \group platform
 * \subgroup platform-file
 * \desc Docs!
 * \type sll_std_file_type_t
 */
#define SLL_PLATFORM_STREAM_OUTPUT 1



/**
 * \flags macro var
 * \name SLL_PLATFORM_STREAM_ERROR
 * \group platform
 * \subgroup platform-file
 * \desc Docs!
 * \type sll_std_file_type_t
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
 * \flags macro var
 * \name SLL_UNKNOWN_FILE_DESCRIPTOR
 * \group platform
 * \subgroup platform-file
 * \desc Docs!
 * \type sll_file_descriptor_t
 */
#define SLL_UNKNOWN_FILE_DESCRIPTOR ((sll_file_descriptor_t)0xffffffffffffffffull)



/**
 * \flags type var
 * \name sll_std_file_type_t
 * \group platform
 * \subgroup platform-file
 * \desc Docs!
 * \type __SLL_U8
 */
typedef __SLL_U8 sll_std_file_type_t;



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
 * \flags check_output func
 * \name sll_platform_file_close
 * \group platform
 * \subgroup platform-file
 * \desc Docs!
 * \arg sll_file_descriptor_t fd
 * \ret sll_error_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_error_t sll_platform_file_close(sll_file_descriptor_t fd);



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
 * \arg const sll_char_t* file_path
 * \arg sll_file_flags_t flags
 * \arg sll_error_t* err
 * \ret sll_file_descriptor_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_file_descriptor_t sll_platform_file_open(const sll_char_t* file_path,sll_file_flags_t flags,sll_error_t* err);



/**
 * \flags check_output func
 * \name sll_platform_file_read
 * \group platform
 * \subgroup platform-file
 * \desc Docs!
 * \arg sll_file_descriptor_t fd
 * \arg void* pointer
 * \arg sll_size_t size
 * \arg sll_error_t* err
 * \ret sll_size_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_size_t sll_platform_file_read(sll_file_descriptor_t fd,void* pointer,sll_size_t size,sll_error_t* err);



/**
 * \flags check_output func
 * \name sll_platform_file_seek
 * \group platform
 * \subgroup platform-file
 * \desc Docs!
 * \arg sll_file_descriptor_t fd
 * \arg sll_file_offset_t offset
 * \ret sll_error_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_error_t sll_platform_file_seek(sll_file_descriptor_t fd,sll_file_offset_t offset);



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
 * \arg const void* pointer
 * \arg sll_size_t size
 * \arg sll_error_t* err
 * \ret sll_size_t
 */
__SLL_EXTERNAL sll_size_t sll_platform_file_write(sll_file_descriptor_t fd,const void* pointer,sll_size_t size,sll_error_t* err);



/**
 * \flags check_output func
 * \name sll_platform_get_default_stream_descriptor
 * \group platform
 * \subgroup platform-file
 * \desc Docs!
 * \arg sll_std_file_type_t type
 * \ret sll_file_descriptor_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_file_descriptor_t sll_platform_get_default_stream_descriptor(sll_std_file_type_t type);



#endif
