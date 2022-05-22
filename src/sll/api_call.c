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
	const sll_internal_function_t* dt=sll_current_runtime_data->internal_function_table->data+fn;
	void* bf=sll_allocate_stack(dt->_arg_size);
	arg_output_t ao={
		ARG_OUTPUT_TYPE_ARRAY,
		{
			.array={
				bf,
				dt->_arg_size
			}
		}
	};
	sll_arg_state_t st=_parse_args_raw(dt->format,al,all,&ao);
	api_return_value_t ret;
	sll_float_t ret_f=_call_api_func_assembly(&ret,dt->_registers,bf,dt->_arg_count,dt->function);
	sll_object_t* o;
	if ((dt->_return_value&RETURN_VALUE_FLAG_ERROR)&&ret_f){
		o=sll_int_to_object(ret.error);
	}
	else{
		switch (RETURN_VALUE_GET_TYPE(dt->_return_value)){
			case 'b':
				o=SLL_ACQUIRE_STATIC_INT(ret.bool_);
				break;
			case 'B':
				o=sll_int_to_object(ret.int_&0xff);
				break;
			case 'W':
				o=sll_int_to_object(ret.int_&0xffff);
				break;
			case 'D':
				o=sll_int_to_object(ret.int_&0xffffffff);
				break;
			case 'Q':
			case 'i':
				o=sll_int_to_object(ret.int_);
				break;
			case 'f':
				if (dt->_return_value&RETURN_VALUE_FLAG_ERROR){
					SLL_UNIMPLEMENTED();
				}
				o=sll_float_to_object(ret_f);
				break;
			case 'c':
				o=SLL_FROM_CHAR(ret.char_);
				break;
			case 'd':
				o=sll_complex_to_object(&(ret.complex_));
				break;
			case 'x':
				if (ret.number.type==SLL_PARSE_ARGS_TYPE_INT){
					o=sll_int_to_object(ret.number.data.int_);
				}
				else if (ret.number.type==SLL_PARSE_ARGS_TYPE_FLOAT){
					o=sll_float_to_object(ret.number.data.float_);
				}
				else{
					o=sll_complex_to_object(&(ret.number.data.complex_));
				}
				break;
			case 's':
				o=STRING_TO_OBJECT_NOCOPY(&(ret.string));
				break;
			case 'a':
				o=sll_array_to_object_nocopy(&(ret.array));
				break;
			case 'm':
				o=sll_map_to_object_nocopy(&(ret.map));
				break;
			case 'o':
				o=ret.object;
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
	o->format=sll_allocate(off+1);
	sll_copy_data(fmt,off,o->format);
	o->format[off]=0;
	o->_return_value='v';
	__SLL_U16 flags=0;
	fmt+=off;
	while (*fmt){
		if (*fmt=='b'||*fmt=='B'||*fmt=='W'||*fmt=='D'||*fmt=='Q'||*fmt=='i'||*fmt=='f'||*fmt=='c'||*fmt=='d'||*fmt=='x'||*fmt=='s'||*fmt=='a'||*fmt=='m'||*fmt=='o'||*fmt=='v'){
			o->_return_value=*fmt;
		}
		else if (*fmt=='~'){
			flags|=RETURN_VALUE_FLAG_ERROR;
		}
		fmt++;
	}
	if (o->_return_value=='x'&&(flags&RETURN_VALUE_FLAG_ERROR)){
		SLL_UNIMPLEMENTED();
	}
	o->_return_value|=flags;
	o->_arg_count=_parse_arg_count(o->format,o->_return_value,&(o->_registers),&(o->_arg_size));
}
