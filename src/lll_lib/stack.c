#include <lll_lib.h>
#include <_lll_internal.h>
#include <stdint.h>



uint8_t* _bf=NULL;
uint32_t _bf_off;
uint32_t _bf_sz;



__LLL_IMPORT_EXPORT void lll_set_internal_stack(uint8_t* bf,uint32_t sz){
	_bf=bf;
	_bf_off=0;
	_bf_sz=sz;
}



__LLL_IMPORT_EXPORT void lll_save_stack_context(lll_stack_context_t* ctx){
	ctx->ptr=_bf;
	ctx->off=_bf_off;
	ctx->sz=_bf_sz;
}



__LLL_IMPORT_EXPORT void lll_load_stack_context(const lll_stack_context_t* ctx){
	_bf=ctx->ptr;
	_bf_off=ctx->off;
	_bf_sz=ctx->sz;
}
