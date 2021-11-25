#ifndef __SLL_API_FILE_H__
#define __SLL_API_FILE_H__ 1
#include <sll/api.h>
#include <sll/common.h>
#include <sll/types.h>



/*~~DOCS~~
TYPE api
FUNC sll_api_file_close
DESC Closes a file previously opened by :sll_api_file_open: and invalidates its handle
ARG H Previously allocated file handle
RET 0 Failure
RET 1 Success*/
__API_FUNC_DECL(file_close);



/*~~DOCS~~
TYPE api
FUNC sll_api_file_open
DESC Opens a file with the specified mode
ARG S The path to the file to open
ARG S? The optional file mode
RET h Failure
RET H Success*/
__API_FUNC_DECL(file_open);



/*~~DOCS~~
TYPE api
FUNC sll_api_file_read
DESC Reads from a file prevoiusly opened by :sll_api_file_open:
ARG HI Previously opened file handle
ARG I? Maximum number of bytes to read
RET Z Failure
RET S The content of the file*/
__API_FUNC_DECL(file_read);



/*~~DOCS~~
TYPE api var_arg
FUNC sll_api_file_write
DESC Writes to a file prevoiusly opened by :sll_api_file_open:
ARG HI Previously opened file handle
ARG O The object to write to file
RET 0 Failure
RET I Number of bytes written*/
__API_FUNC_DECL(file_write);



#endif
