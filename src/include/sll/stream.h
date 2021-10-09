#ifndef __SLL_STREAM_H__
#define __SLL_STREAM_H__ 1
#include <sll/common.h>
#include <sll/types.h>
#include <stdio.h>



#define SLL_END_OF_DATA UINT16_MAX

#define SLL_READ_FROM_INPUT_DATA_STREAM(is) ((is)->rf((is)))
#define SLL_READ_BUFFER_FROM_INPUT_DATA_STREAM(is,bf,sz) ((is)->rbf((is),(bf),(sz)))
#define SLL_INPUT_DATA_STREAM_RESTART_LINE(is,lp) ((is)->rlf((is),(lp)))
#define SLL_GET_INPUT_DATA_STREAM_OFFSET(is) ((is)->_off)
#define SLL_GET_INPUT_DATA_STREAM_LINE_NUMBER(is) ((is)->_lc)
#define SLL_GET_INPUT_DATA_STREAM_LINE_OFFSET(is) ((is)->_loff)

#define SLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,c) ((os)->wcf((os),(c)))
#define SLL_WRITE_STRING_TO_OUTPUT_DATA_STREAM(os,s) ((os)->wsf((os),(s)))
#define SLL_WRITE_TO_OUTPUT_DATA_STREAM(os,bf,sz) ((os)->wf((os),(bf),(sz)))



__SLL_FUNC void sll_stream_create_input_from_file(FILE* f,sll_input_data_stream_t* o);



__SLL_FUNC void sll_stream_create_input_from_buffer(sll_input_buffer_t* bf,sll_input_data_stream_t* o);



__SLL_FUNC void sll_stream_create_output_from_file(FILE* f,sll_output_data_stream_t* o);



#endif
