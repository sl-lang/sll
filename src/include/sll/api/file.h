#ifndef __SLL_API_FILE_H__
#define __SLL_API_FILE_H__ 1
#include <sll/api.h>
#include <sll/common.h>
#include <sll/gc.h>
#include <sll/types.h>
/**
 * \flags group
 * \name File API
 * \group file-api
 * \desc Docs!
 */



#define SLL_FROM_FILE(f) __SLL_ADD_DEBUG_DATA(sll_file_to_object((f)),__SLL_DEBUG_TYPE_CREATE)


/**
 * \flags check_output func
 * \name sll_file_to_object
 * \group file-api
 * \desc Desc!
 * \arg sll_file_t* f
 * \ret sll_object_t*
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_file_to_object(sll_file_t* f);



/**
 * \flags api func
 * \name sll_api_file_close
 * \group file-api
 * \desc Closes a file previously opened by :sll_api_file_open: and invalidates its handle
 * \arg H -> Previously allocated file handle
 * \ret B
 */
__API_FUNC_DECL(file_close);



/**
 * \flags api func optimizable
 * \name sll_api_file_open
 * \group file-api
 * \desc Opens a file with the specified mode
 * \arg S -> The path to the file to open
 * \arg S? -> The optional file mode
 * \ret H
 */
__API_FUNC_DECL(file_open);



/**
 * \flags api func
 * \name sll_api_file_read
 * \group file-api
 * \desc Reads from a file prevoiusly opened by :sll_api_file_open:
 * \arg H -> Previously opened file handle
 * \arg I? -> Maximum number of bytes to read
 * \ret S -> The content of the file
 */
__API_FUNC_DECL(file_read);



/**
 * \flags api func optimizable
 * \name sll_api_file_std_handle
 * \group file-api
 * \desc Docs!
 * \arg I
 * \ret H
 */
__API_FUNC_DECL(file_std_handle);



/**
 * \flags api func var_arg
 * \name sll_api_file_write
 * \group file-api
 * \desc Writes to a file prevoiusly opened by :sll_api_file_open:
 * \arg H -> Previously opened file handle
 * \arg O -> The object to write to file
 * \ret I -> Number of bytes written
 */
__API_FUNC_DECL(file_write);



#endif
