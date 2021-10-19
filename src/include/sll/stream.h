#ifndef __SLL_STREAM_H__
#define __SLL_STREAM_H__ 1
#include <sll/common.h>
#include <sll/types.h>
#include <stdio.h>



#define SLL_END_OF_DATA UINT16_MAX

#define SLL_GET_INPUT_DATA_STREAM_LINE_NUMBER(is) ((is)->_lc)
#define SLL_GET_INPUT_DATA_STREAM_LINE_OFFSET(is) ((is)->_loff)
#define SLL_GET_INPUT_DATA_STREAM_OFFSET(is) ((is)->_off)
#define SLL_INPUT_DATA_STREAM_RESTART_LINE(is,lp) ((is)->rlf((is),(lp)))
#define SLL_READ_BUFFER_FROM_INPUT_DATA_STREAM(is,bf,l) ((is)->rbf((is),(bf),(l)))
#define SLL_READ_FROM_INPUT_DATA_STREAM(is) ((is)->rf((is)))

#define SLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,c) ((os)->wcf((os),(c)))
#define SLL_WRITE_TO_OUTPUT_DATA_STREAM(os,bf,l) ((os)->wf((os),(bf),(l)))



__SLL_FUNC void sll_stream_create_input_from_file(FILE* f,sll_input_data_stream_t* o);



__SLL_FUNC void sll_stream_create_input_from_buffer(sll_input_buffer_t* bf,sll_input_data_stream_t* o);



__SLL_FUNC void sll_stream_create_output_from_file(FILE* f,sll_output_data_stream_t* o);



__SLL_FUNC sll_read_char_t sll_stream_read(sll_input_data_stream_t* is);



__SLL_FUNC sll_read_char_t sll_stream_read_buffer(sll_input_data_stream_t* is,sll_char_t* bf,sll_string_length_t l);



__SLL_FUNC void sll_stream_restart_line(sll_input_data_stream_t* is,sll_file_offset_t l);



__SLL_FUNC void sll_stream_write(sll_output_data_stream_t* os,sll_char_t c);



__SLL_FUNC void sll_stream_write_buffer(sll_output_data_stream_t* os,const sll_char_t* bf,sll_string_length_t l);



#endif
