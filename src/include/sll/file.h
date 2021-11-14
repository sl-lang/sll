#ifndef __SLL_FILE_H__
#define __SLL_FILE_H__ 1
#include <sll/common.h>
#include <sll/types.h>



#define SLL_FILE_FLAG_READ 1
#define SLL_FILE_FLAG_WRITE 2
#define SLL_FILE_FLAG_APPEND 4

#define SLL_UNKNOWN_FILE_DESCRIPTOR ((void*)0xffffffffffffffffull)



__SLL_FUNC void sll_file_close(sll_file_t* f);



__SLL_FUNC void sll_file_flush(sll_file_t* f);



__SLL_FUNC __SLL_CHECK_OUTPUT sll_bool_t sll_file_open(const sll_char_t* nm,sll_file_flags_t f,sll_file_t* o);



__SLL_FUNC __SLL_CHECK_OUTPUT sll_size_t sll_file_read(sll_file_t* f,void* p,sll_size_t sz);



__SLL_FUNC __SLL_CHECK_OUTPUT sll_size_t sll_file_write(sll_file_t* f,const void* p,sll_size_t sz);



#endif
