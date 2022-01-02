#ifndef __SLL_API_FILE_H__
#define __SLL_API_FILE_H__ 1
#include <sll/api.h>
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
 * \name sll_file_to_handle
 * \group file-api
 * \desc Desc!
 * \arg sll_file_t* f
 * \ret sll_integer_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_integer_t sll_file_to_handle(sll_file_t* f);



/**
 * \flags api func
 * \name sll_api_file_close
 * \group file-api
 * \desc Docs!
 * \arg I
 * \ret B
 */
__API_FUNC_DECL(file_close);



/**
 * \flags api func
 * \name sll_api_file_flush
 * \group file-api
 * \desc Docs!
 * \arg I
 * \ret B
 */
__API_FUNC_DECL(file_flush);



/**
 * \flags api func optimizable
 * \name sll_api_file_open
 * \group file-api
 * \desc Docs!
 * \arg S
 * \arg I
 * \ret I
 */
__API_FUNC_DECL(file_open);



/**
 * \flags api func
 * \name sll_api_file_read
 * \group file-api
 * \desc Docs!
 * \arg I
 * \arg I
 * \ret S
 */
__API_FUNC_DECL(file_read);



/**
 * \flags api func optimizable
 * \name sll_api_file_std_handle
 * \group file-api
 * \desc Docs!
 * \arg I
 * \ret I
 */
__API_FUNC_DECL(file_std_handle);



/**
 * \flags api func var_arg
 * \name sll_api_file_write
 * \group file-api
 * \desc Docs!
 * \arg I
 * \arg O
 * \ret I
 */
__API_FUNC_DECL(file_write);



#endif
