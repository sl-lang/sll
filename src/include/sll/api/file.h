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
 * \flags api func
 * \name sll_api_file_close
 * \group file-api
 * \desc Closes a file previously opened by :sll_api_file_open: and invalidates its handle
 * \arg H -> Previously allocated file handle
 * \ret 0 -> Failure
 * \ret 1 -> Success
 */
__API_FUNC_DECL(file_close);



/**
 * \flags api func
 * \name sll_api_file_open
 * \group file-api
 * \desc Opens a file with the specified mode
 * \arg S -> The path to the file to open
 * \arg S? -> The optional file mode
 * \ret h -> Failure
 * \ret H -> Success
 */
__API_FUNC_DECL(file_open);



/**
 * \flags api func
 * \name sll_api_file_read
 * \group file-api
 * \desc Reads from a file prevoiusly opened by :sll_api_file_open:
 * \arg HI -> Previously opened file handle
 * \arg I? -> Maximum number of bytes to read
 * \ret Z -> Failure
 * \ret S -> The content of the file
 */
__API_FUNC_DECL(file_read);



/**
 * \flags api func var_arg
 * \name sll_api_file_write
 * \group file-api
 * \desc Writes to a file prevoiusly opened by :sll_api_file_open:
 * \arg HI -> Previously opened file handle
 * \arg O -> The object to write to file
 * \ret 0 -> Failure
 * \ret I -> Number of bytes written
 */
__API_FUNC_DECL(file_write);



#endif
