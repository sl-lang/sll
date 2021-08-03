#ifndef __LLL_IO_H__
#define __LLL_IO_H__ 1
#include <lll/common.h>
#include <lll/types.h>



__LLL_FUNC __LLL_RETURN lll_load_assembly(lll_input_data_stream_t* is,lll_assembly_data_t* a_dt,lll_error_t* e);



__LLL_FUNC __LLL_RETURN lll_load_compiled_object(lll_input_data_stream_t* is,lll_compilation_data_t* c_dt,lll_error_t* e);



__LLL_FUNC __LLL_RETURN lll_load_object(lll_compilation_data_t* c_dt,lll_input_data_stream_t* is,lll_object_t** o,lll_error_t* e);



__LLL_FUNC void lll_write_assembly(lll_output_data_stream_t* os,const lll_assembly_data_t* o);



__LLL_FUNC void lll_write_compiled_object(lll_output_data_stream_t* os,const lll_compilation_data_t* c_dt);



__LLL_FUNC void lll_write_object(lll_output_data_stream_t* os,const lll_object_t* o);



#endif
