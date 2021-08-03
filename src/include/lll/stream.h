#ifndef __LLL_STREAM_H__
#define __LLL_STREAM_H__ 1
#include <lll/common.h>
#include <lll/types.h>
#include <stdio.h>



__LLL_FUNC void lll_stream_create_input_from_file(FILE* f,lll_input_data_stream_t* o);



__LLL_FUNC void lll_stream_create_input_from_buffer(lll_input_buffer_t* bf,lll_input_data_stream_t* o);



__LLL_FUNC void lll_stream_create_output_from_file(FILE* f,lll_output_data_stream_t* o);



#endif
