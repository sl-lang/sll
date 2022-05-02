#include <sll/_internal/api_call.h>
#include <sll/_internal/common.h>
#include <sll/_internal/parse_args.h>
#include <sll/_internal/static_object.h>
#include <sll/_internal/string.h>
#include <sll/common.h>
#include <sll/data.h>
#include <sll/gc.h>
#include <sll/ift.h>
#include <sll/memory.h>
#include <sll/parse_args.h>
#include <sll/static_object.h>
#include <sll/string.h>
#include <sll/vm.h>
#include <sll/types.h>



sll_object_t* _call_api_func(sll_function_index_t fn,sll_object_t*const* al,sll_arg_count_t all){
	const sll_internal_function_t* dt=sll_current_runtime_data->ift->dt+fn;
	void* bf=sll_allocate_stack(dt->_arg_sz);
	arg_output_t ao={
		ARG_OUTPUT_TYPE_ARRAY,
		{
			.arr={
				bf,
				dt->_arg_sz
			}
		}
	};
	sll_arg_state_t st=_parse_args_raw(dt->fmt,al,all,&ao);
	api_return_value_t ret;
	sll_float_t ret_f=_call_api_func_assembly(&ret,dt->_regs,bf,dt->_arg_cnt,dt->p);
	sll_object_t* o;
	if ((dt->_ret&RETURN_VALUE_FLAG_ERROR)&&ret_f){
		o=sll_int_to_object(ret.err);
	}
	else{
		switch (RETURN_VALUE_GET_TYPE(dt->_ret)){
			case 'b':
				o=SLL_ACQUIRE_STATIC_INT(ret.b);
				break;
			case 'B':
				o=sll_int_to_object(ret.i&0xff);
				break;
			case 'W':
				o=sll_int_to_object(ret.i&0xffff);
				break;
			case 'D':
				o=sll_int_to_object(ret.i&0xffffffff);
				break;
			case 'Q':
			case 'i':
				o=sll_int_to_object(ret.i);
				break;
			case 'f':
				o=sll_float_to_object(ret_f);
				break;
			case 'c':
				o=SLL_FROM_CHAR(ret.c);
				break;
			case 'd':
				o=sll_complex_to_object(&(ret.d));
				break;
			case 'x':
				if (ret.x.t==SLL_PARSE_ARGS_TYPE_FLOAT){
					o=sll_float_to_object(ret.x.dt.f);
					break;
				}
				o=sll_complex_to_object(&(ret.x.dt.d));
				break;
			case 's':
				o=STRING_TO_OBJECT_NOCOPY(&(ret.s));
				break;
			case 'a':
				o=sll_array_to_object_nocopy(&(ret.a));
				break;
			case 'm':
				o=sll_map_to_object_nocopy(&(ret.m));
				break;
			case 'o':
				o=ret.o;
				break;
			case 'v':
				o=SLL_ACQUIRE_STATIC_INT(0);
				break;
			default:
				SLL_UNREACHABLE();
		}
	}
	sll_free_args(st);
	sll_deallocate(bf);
	return o;
}



void _parse_api_call_format(const sll_char_t* fmt,sll_internal_function_t* o){
	sll_string_length_t off=0;
	while (*(fmt+off)&&*(fmt+off)!='|'){
		off++;
	}
	o->fmt=sll_allocate(off+1);
	sll_copy_data(fmt,off,o->fmt);
	o->fmt[off]=0;
	o->_ret='v';
	__SLL_U16 flags=0;
	fmt+=off;
	while (*fmt){
		if (*fmt=='b'||*fmt=='B'||*fmt=='W'||*fmt=='D'||*fmt=='Q'||*fmt=='i'||*fmt=='f'||*fmt=='c'||*fmt=='d'||*fmt=='x'||*fmt=='s'||*fmt=='a'||*fmt=='m'||*fmt=='o'||*fmt=='v'){
			o->_ret=*fmt;
		}
		else if (*fmt=='~'){
			flags|=RETURN_VALUE_FLAG_ERROR;
		}
		fmt++;
	}
	if (o->_ret=='x'&&(flags&RETURN_VALUE_FLAG_ERROR)){
		SLL_UNIMPLEMENTED();
	}
	o->_ret|=flags;
	o->_arg_cnt=_parse_arg_count(o->fmt,o->_ret,&(o->_regs),&(o->_arg_sz));
}
