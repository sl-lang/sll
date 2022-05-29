#include <clib/common.h>
#include <clib/function_table.h>
#include <sll.h>



__CLIB_EXTERNAL sll_bool_t SLL_ABI_INIT(sll_version_t version){
	if (version!=SLL_VERSION){
		return 0;
	}
	sll_register_internal_functions(sll_current_runtime_data->internal_function_table,clib_internal_function_data,clib_internal_function_count);
	return 1;
}
