#ifndef __SLL_STREAM_H__
#define __SLL_STREAM_H__ 1
#include <sll/common.h>
#include <sll/types.h>
#include <stdio.h>



__SLL_FUNC void sll_stream_create_input_from_file(FILE* restrict f,sll_input_data_stream_t* restrict o);



__SLL_FUNC void sll_stream_create_input_from_buffer(sll_input_buffer_t* restrict bf,sll_input_data_stream_t* restrict o);



__SLL_FUNC void sll_stream_create_output_from_file(FILE* restrict f,sll_output_data_stream_t* restrict o);



#endif
