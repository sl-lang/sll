#ifndef __SLL_STREAM_H__
#define __SLL_STREAM_H__ 1
#include <sll/common.h>
#include <sll/types.h>
#include <stdio.h>



__SLL_FUNC void sll_stream_create_input_from_file(FILE* f,sll_input_data_stream_t* o);



__SLL_FUNC void sll_stream_create_input_from_buffer(sll_input_buffer_t* bf,sll_input_data_stream_t* o);



__SLL_FUNC void sll_stream_create_output_from_file(FILE* f,sll_output_data_stream_t* o);



#endif
