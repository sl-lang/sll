#ifndef __SLL_FILE_H__
#define __SLL_FILE_H__ 1
#include <sll/common.h>
#include <sll/types.h>



#define SLL_FILE_FLAG_READ 1
#define SLL_FILE_FLAG_WRITE 2
#define SLL_FILE_FLAG_APPEND 4
#define SLL_FILE_FLAG_NO_BUFFER 8

#define SLL_FILE_GET_LINE_NUMBER(f) ((f)->_l_num)
#define SLL_FILE_GET_LINE_OFFSET(f) ((f)->_l_off)
#define SLL_FILE_GET_OFFSET(f) ((f)->_off)

#define SLL_UNKNOWN_FILE_DESCRIPTOR ((void*)0xffffffffffffffffull)

#define SLL_END_OF_DATA UINT16_MAX



__SLL_EXTERNAL extern sll_file_t* sll_stdin;
__SLL_EXTERNAL extern sll_file_t* sll_stdout;
__SLL_EXTERNAL extern sll_file_t* sll_stderr;



__SLL_EXTERNAL void sll_file_close(sll_file_t* f);



__SLL_EXTERNAL void sll_file_flush(sll_file_t* f);



__SLL_EXTERNAL void sll_file_from_data(void* p,sll_size_t sz,sll_file_flags_t f,sll_file_t* o);



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_file_open(const sll_char_t* nm,sll_file_flags_t f,sll_file_t* o);



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_size_t sll_file_read(sll_file_t* f,void* p,sll_size_t sz);



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_read_char_t sll_file_read_char(sll_file_t* f);



__SLL_EXTERNAL void sll_file_reset(sll_file_t* f);



__SLL_EXTERNAL void sll_file_reset_line(sll_file_t* f,sll_file_offset_t off);



__SLL_EXTERNAL sll_size_t sll_file_write(sll_file_t* f,const void* p,sll_size_t sz);



__SLL_EXTERNAL void sll_file_write_char(sll_file_t* f,sll_char_t c);



#endif
