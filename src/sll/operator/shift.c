#include <sll/_internal/common.h>
#include <sll/_internal/operator.h>
#include <sll/api/math.h>
#include <sll/array.h>
#include <sll/common.h>
#include <sll/gc.h>
#include <sll/map.h>
#include <sll/object.h>
#include <sll/static_object.h>
#include <sll/string.h>
#include <sll/types.h>
#include <math.h>



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t sll_operator_shl(sll_object_t a,sll_object_t b){
	switch (COMBINED_ARGS){
		case COMBINED_TYPE_II:
			return sll_int_to_object(a->data.int_<<b->data.int_);
		case COMBINED_TYPE_IF:
			return sll_float_to_object(a->data.int_*pow(2,b->data.float_));
		case COMBINED_TYPE_IC:
			return sll_int_to_object(a->data.int_<<b->data.char_);
		case COMBINED_TYPE_IS:
		case COMBINED_TYPE_FS:
		case COMBINED_TYPE_CS:
			{
				sll_object_t o=sll_array_length_to_object(b->data.string.length);
				for (sll_string_length_t i=0;i<b->data.string.length;i++){
					o->data.array.data[i]=sll_operator_shl(a,sll_static_char[b->data.string.data[i]]);
				}
				return o;
			}
		case COMBINED_TYPE_IA:
		case COMBINED_TYPE_FA:
		case COMBINED_TYPE_CA:
			{
				sll_object_t o=sll_array_length_to_object(b->data.array.length);
				for (sll_array_length_t i=0;i<b->data.array.length;i++){
					o->data.array.data[i]=sll_operator_shl(a,b->data.array.data[i]);
				}
				return o;
			}
		case COMBINED_TYPE_IM:
		case COMBINED_TYPE_FM:
		case COMBINED_TYPE_CM:
			{
				sll_object_t o=sll_map_length_to_object(b->data.map.length);
				for (sll_map_length_t i=0;i<(b->data.map.length<<1);i+=2){
					o->data.map.data[i]=b->data.map.data[i];
					SLL_ACQUIRE(o->data.map.data[i]);
					o->data.map.data[i+1]=sll_operator_shl(a,b->data.map.data[i+1]);
				}
				return o;
			}
		case COMBINED_TYPE_FI:
			return sll_float_to_object(a->data.float_*pow(2,(sll_float_t)(b->data.int_)));
		case COMBINED_TYPE_FF:
			return sll_float_to_object(a->data.float_*pow(2,b->data.float_));
		case COMBINED_TYPE_FC:
			return sll_float_to_object(a->data.float_*pow(2,b->data.char_));
		case COMBINED_TYPE_CI:
			return sll_int_to_object(((sll_integer_t)a->data.char_)<<b->data.int_);
		case COMBINED_TYPE_CF:
			return sll_float_to_object(a->data.char_*pow(2,b->data.float_));
		case COMBINED_TYPE_CC:
			return SLL_FROM_CHAR((((sll_integer_t)a->data.char_))<<b->data.char_);
		case COMBINED_TYPE_SI:
			{
				sll_object_t o=sll_create_object(SLL_OBJECT_TYPE_STRING);
				sll_string_resize(&(a->data.string),b->data.int_,&(o->data.string));
				return o;
			}
		case COMBINED_TYPE_SF:
			{
				sll_object_t o=sll_create_object(SLL_OBJECT_TYPE_STRING);
				sll_string_resize(&(a->data.string),(sll_integer_t)round(b->data.float_),&(o->data.string));
				return o;
			}
		case COMBINED_TYPE_SC:
			{
				sll_object_t o=sll_create_object(SLL_OBJECT_TYPE_STRING);
				sll_string_resize(&(a->data.string),b->data.char_,&(o->data.string));
				return o;
			}
		case COMBINED_TYPE_SS:
			{
				sll_object_t o=sll_create_object(SLL_OBJECT_TYPE_STRING);
				sll_string_op(&(a->data.string),&(b->data.string),sll_operator_shl,&(o->data.string));
				return o;
			}
		case COMBINED_TYPE_SA:
			{
				sll_object_t o=sll_create_object(SLL_OBJECT_TYPE_ARRAY);
				sll_string_op_array(&(a->data.string),&(b->data.array),sll_operator_shl,0,&(o->data.array));
				return o;
			}
		case COMBINED_TYPE_SM:
			{
				sll_object_t o=sll_create_object(SLL_OBJECT_TYPE_MAP);
				sll_string_op_map(&(a->data.string),&(b->data.map),sll_operator_shl,0,&(o->data.map));
				return o;
			}
		case COMBINED_TYPE_AI:
			{
				sll_object_t o=sll_create_object(SLL_OBJECT_TYPE_ARRAY);
				sll_array_resize(&(a->data.array),b->data.int_,&(o->data.array));
				return o;
			}
		case COMBINED_TYPE_AF:
			{
				sll_object_t o=sll_create_object(SLL_OBJECT_TYPE_ARRAY);
				sll_array_resize(&(a->data.array),(sll_integer_t)round(b->data.float_),&(o->data.array));
				return o;
			}
		case COMBINED_TYPE_AC:
			{
				sll_object_t o=sll_create_object(SLL_OBJECT_TYPE_ARRAY);
				sll_array_resize(&(a->data.array),b->data.char_,&(o->data.array));
				return o;
			}
		case COMBINED_TYPE_AS:
			{
				sll_object_t o=sll_create_object(SLL_OBJECT_TYPE_ARRAY);
				sll_string_op_array(&(b->data.string),&(a->data.array),sll_operator_shl,1,&(o->data.array));
				return o;
			}
		case COMBINED_TYPE_AA:
			{
				sll_object_t o=sll_create_object(SLL_OBJECT_TYPE_ARRAY);
				sll_array_op(&(a->data.array),&(b->data.array),sll_operator_shl,&(o->data.array));
				return o;
			}
		case COMBINED_TYPE_AM:
			{
				sll_object_t o=sll_create_object(SLL_OBJECT_TYPE_MAP);
				sll_array_op_map(&(a->data.array),&(b->data.map),sll_operator_shl,0,&(o->data.map));
				return o;
			}
		case COMBINED_TYPE_MI:
		case COMBINED_TYPE_MF:
		case COMBINED_TYPE_MC:
			{
				sll_object_t o=sll_map_length_to_object(a->data.map.length);
				for (sll_map_length_t i=0;i<(a->data.map.length<<1);i+=2){
					o->data.map.data[i]=a->data.map.data[i];
					SLL_ACQUIRE(o->data.map.data[i]);
					o->data.map.data[i+1]=sll_operator_shl(a->data.map.data[i+1],b);
				}
				return o;
			}
		case COMBINED_TYPE_MS:
			{
				sll_object_t o=sll_create_object(SLL_OBJECT_TYPE_MAP);
				sll_string_op_map(&(b->data.string),&(a->data.map),sll_operator_shl,1,&(o->data.map));
				return o;
			}
		case COMBINED_TYPE_MA:
			{
				sll_object_t o=sll_create_object(SLL_OBJECT_TYPE_MAP);
				sll_array_op_map(&(b->data.array),&(a->data.map),sll_operator_shl,1,&(o->data.map));
				return o;
			}
		case COMBINED_TYPE_MM:
			{
				sll_object_t o=sll_create_object(SLL_OBJECT_TYPE_MAP);
				sll_map_op(&(a->data.map),&(b->data.map),sll_operator_shl,&(o->data.map));
				return o;
			}
		default:
			SLL_UNREACHABLE();
	}
	return SLL_ACQUIRE_STATIC_INT(0);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t sll_operator_shr(sll_object_t a,sll_object_t b){
	switch (COMBINED_ARGS){
		case COMBINED_TYPE_II:
			return sll_int_to_object(a->data.int_>>b->data.int_);
		case COMBINED_TYPE_IF:
			return sll_float_to_object(a->data.int_/pow(2,b->data.float_));
		case COMBINED_TYPE_IC:
			return sll_int_to_object(a->data.int_>>b->data.char_);
		case COMBINED_TYPE_IS:
		case COMBINED_TYPE_FS:
		case COMBINED_TYPE_CS:
			{
				sll_object_t o=sll_array_length_to_object(b->data.string.length);
				for (sll_string_length_t i=0;i<b->data.string.length;i++){
					o->data.array.data[i]=sll_operator_shr(a,sll_static_char[b->data.string.data[i]]);
				}
				return o;
			}
		case COMBINED_TYPE_IA:
		case COMBINED_TYPE_FA:
		case COMBINED_TYPE_CA:
			{
				sll_object_t o=sll_array_length_to_object(b->data.array.length);
				for (sll_array_length_t i=0;i<b->data.array.length;i++){
					o->data.array.data[i]=sll_operator_shr(a,b->data.array.data[i]);
				}
				return o;
			}
		case COMBINED_TYPE_IM:
		case COMBINED_TYPE_FM:
		case COMBINED_TYPE_CM:
			{
				sll_object_t o=sll_map_length_to_object(b->data.map.length);
				for (sll_map_length_t i=0;i<(b->data.map.length<<1);i+=2){
					o->data.map.data[i]=b->data.map.data[i];
					SLL_ACQUIRE(o->data.map.data[i]);
					o->data.map.data[i+1]=sll_operator_shr(a,b->data.map.data[i+1]);
				}
				return o;
			}
		case COMBINED_TYPE_FI:
			return sll_float_to_object(a->data.float_/pow(2,(sll_float_t)(b->data.int_)));
		case COMBINED_TYPE_FF:
			return sll_float_to_object(a->data.float_/pow(2,b->data.float_));
		case COMBINED_TYPE_FC:
			return sll_float_to_object(a->data.float_/pow(2,b->data.char_));
		case COMBINED_TYPE_CI:
			return sll_int_to_object(a->data.char_>>b->data.int_);
		case COMBINED_TYPE_CF:
			return sll_float_to_object(a->data.char_/pow(2,b->data.float_));
		case COMBINED_TYPE_CC:
			return SLL_FROM_CHAR(a->data.char_>>b->data.char_);
		case COMBINED_TYPE_SI:
			{
				sll_object_t o=sll_create_object(SLL_OBJECT_TYPE_STRING);
				sll_string_resize(&(a->data.string),-b->data.int_,&(o->data.string));
				return o;
			}
		case COMBINED_TYPE_SF:
			{
				sll_object_t o=sll_create_object(SLL_OBJECT_TYPE_STRING);
				sll_string_resize(&(a->data.string),-(sll_integer_t)round(b->data.float_),&(o->data.string));
				return o;
			}
		case COMBINED_TYPE_SC:
			{
				sll_object_t o=sll_create_object(SLL_OBJECT_TYPE_STRING);
				sll_string_resize(&(a->data.string),-b->data.char_,&(o->data.string));
				return o;
			}
		case COMBINED_TYPE_SS:
			{
				sll_object_t o=sll_create_object(SLL_OBJECT_TYPE_STRING);
				sll_string_op(&(a->data.string),&(b->data.string),sll_operator_shr,&(o->data.string));
				return o;
			}
		case COMBINED_TYPE_SA:
			{
				sll_object_t o=sll_create_object(SLL_OBJECT_TYPE_ARRAY);
				sll_string_op_array(&(a->data.string),&(b->data.array),sll_operator_shr,0,&(o->data.array));
				return o;
			}
		case COMBINED_TYPE_SM:
			{
				sll_object_t o=sll_create_object(SLL_OBJECT_TYPE_MAP);
				sll_string_op_map(&(a->data.string),&(b->data.map),sll_operator_shr,0,&(o->data.map));
				return o;
			}
		case COMBINED_TYPE_AI:
			{
				sll_object_t o=sll_create_object(SLL_OBJECT_TYPE_ARRAY);
				sll_array_resize(&(a->data.array),-b->data.int_,&(o->data.array));
				return o;
			}
		case COMBINED_TYPE_AF:
			{
				sll_object_t o=sll_create_object(SLL_OBJECT_TYPE_ARRAY);
				sll_array_resize(&(a->data.array),-(sll_integer_t)round(b->data.float_),&(o->data.array));
				return o;
			}
		case COMBINED_TYPE_AC:
			{
				sll_object_t o=sll_create_object(SLL_OBJECT_TYPE_ARRAY);
				sll_array_resize(&(a->data.array),-b->data.char_,&(o->data.array));
				return o;
			}
		case COMBINED_TYPE_AS:
			{
				sll_object_t o=sll_create_object(SLL_OBJECT_TYPE_ARRAY);
				sll_string_op_array(&(b->data.string),&(a->data.array),sll_operator_shr,1,&(o->data.array));
				return o;
			}
		case COMBINED_TYPE_AA:
			{
				sll_object_t o=sll_create_object(SLL_OBJECT_TYPE_ARRAY);
				sll_array_op(&(a->data.array),&(b->data.array),sll_operator_shr,&(o->data.array));
				return o;
			}
		case COMBINED_TYPE_AM:
			{
				sll_object_t o=sll_create_object(SLL_OBJECT_TYPE_MAP);
				sll_array_op_map(&(a->data.array),&(b->data.map),sll_operator_shr,0,&(o->data.map));
				return o;
			}
		case COMBINED_TYPE_MI:
		case COMBINED_TYPE_MF:
		case COMBINED_TYPE_MC:
			{
				sll_object_t o=sll_map_length_to_object(a->data.map.length);
				for (sll_map_length_t i=0;i<(a->data.map.length<<1);i+=2){
					o->data.map.data[i]=a->data.map.data[i];
					SLL_ACQUIRE(o->data.map.data[i]);
					o->data.map.data[i+1]=sll_operator_shr(a->data.map.data[i+1],b);
				}
				return o;
			}
		case COMBINED_TYPE_MS:
			{
				sll_object_t o=sll_create_object(SLL_OBJECT_TYPE_MAP);
				sll_string_op_map(&(b->data.string),&(a->data.map),sll_operator_shr,1,&(o->data.map));
				return o;
			}
		case COMBINED_TYPE_MA:
			{
				sll_object_t o=sll_create_object(SLL_OBJECT_TYPE_MAP);
				sll_array_op_map(&(b->data.array),&(a->data.map),sll_operator_shr,1,&(o->data.map));
				return o;
			}
		case COMBINED_TYPE_MM:
			{
				sll_object_t o=sll_create_object(SLL_OBJECT_TYPE_MAP);
				sll_map_op(&(a->data.map),&(b->data.map),sll_operator_shr,&(o->data.map));
				return o;
			}
		default:
			SLL_UNREACHABLE();
	}
	return SLL_ACQUIRE_STATIC_INT(0);
}
