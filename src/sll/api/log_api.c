#include <sll/_sll_internal.h>
#include <sll/api.h>
#include <sll/api/string.h>
#include <sll/api/sys.h>
#include <sll/common.h>
#include <sll/log.h>
#include <sll/types.h>
#include <sll/vm.h>



__API_FUNC(log_log){
	sll_string_t fp;
	sll_api_sys_arg_get(0,&fp);
	sll_string_t s;
	sll_api_string_convert(a,ac,&s);
	sll_log_raw(fp.v,(!sll_current_runtime_data->c_st->l?SLL_CHAR("@code"):(sll_current_runtime_data->a_dt->st.dt+(sll_current_runtime_data->c_st->dt+sll_current_runtime_data->c_st->l-1)->nm)->v),&s);
	sll_free_string(&s);
	sll_free_string(&fp);
}



__API_FUNC(log_set_default){
	sll_set_log_default(a);
}



__API_FUNC(log_set_file){
	sll_set_log_file(a->v,b);
}



__API_FUNC(log_set_function){
	sll_set_log_function(a->v,b->v,c);
}
