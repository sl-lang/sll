#include <sll/_internal/common.h>
#include <sll/_internal/operator.h>
#include <sll/api/math.h>
#include <sll/array.h>
#include <sll/common.h>
#include <sll/gc.h>
#include <sll/map.h>
#include <sll/object.h>
#include <sll/static_object.h>
#include <sll/types.h>



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_operator_mod(sll_object_t* a,sll_object_t* b){
	switch (COMBINED_ARGS){
		case COMBINED_TYPE_II:
			return sll_int_to_object(a->data.int_%b->data.int_);
		case COMBINED_TYPE_IF:
			return sll_float_to_object(sll_math_mod((sll_float_t)(a->data.int_),b->data.float_));
		case COMBINED_TYPE_IC:
			return sll_int_to_object(a->data.int_%b->data.char_);
		case COMBINED_TYPE_IS:
		case COMBINED_TYPE_FS:
			{
				sll_object_t* o=sll_array_length_to_object(b->data.string.l);
				for (sll_string_length_t i=0;i<b->data.string.l;i++){
					o->data.array.data[i]=sll_operator_mod(a,sll_static_char[b->data.string.v[i]]);
				}
				return o;
			}
		case COMBINED_TYPE_IA:
		case COMBINED_TYPE_FA:
		case COMBINED_TYPE_CA:
			{
				sll_object_t* o=sll_array_length_to_object(b->data.array.length);
				for (sll_array_length_t i=0;i<b->data.array.length;i++){
					o->data.array.data[i]=sll_operator_mod(a,b->data.array.data[i]);
				}
				return o;
			}
		case COMBINED_TYPE_IM:
		case COMBINED_TYPE_FM:
		case COMBINED_TYPE_CM:
			{
				sll_object_t* o=sll_map_length_to_object(b->data.map.length);
				for (sll_map_length_t i=0;i<(b->data.map.length<<1);i+=2){
					o->data.map.data[i]=b->data.map.data[i];
					SLL_ACQUIRE(o->data.map.data[i]);
					o->data.map.data[i+1]=sll_operator_mod(a,b->data.map.data[i+1]);
				}
				return o;
			}
		case COMBINED_TYPE_FI:
			return sll_float_to_object(sll_math_mod(a->data.float_,(sll_float_t)(b->data.int_)));
		case COMBINED_TYPE_FF:
			return sll_float_to_object(sll_math_mod(a->data.float_,b->data.float_));
		case COMBINED_TYPE_FC:
			return sll_float_to_object(sll_math_mod(a->data.float_,b->data.char_));
		case COMBINED_TYPE_CI:
			return SLL_FROM_CHAR(a->data.char_%b->data.int_);
		case COMBINED_TYPE_CF:
			return sll_float_to_object(sll_math_mod(a->data.char_,b->data.float_));
		case COMBINED_TYPE_CC:
			return SLL_FROM_CHAR(a->data.char_%b->data.char_);
		case COMBINED_TYPE_CS:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_ARRAY);
				sll_string_split_char(&(b->data.string),a->data.char_,&(o->data.array));
				return o;
			}
		case COMBINED_TYPE_SI:
		case COMBINED_TYPE_SF:
			{
				sll_object_t* o=sll_array_length_to_object(a->data.string.l);
				for (sll_string_length_t i=0;i<a->data.string.l;i++){
					o->data.array.data[i]=sll_operator_mod(sll_static_char[a->data.string.v[i]],b);
				}
				return o;
			}
		case COMBINED_TYPE_SC:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_ARRAY);
				sll_string_split_char(&(a->data.string),b->data.char_,&(o->data.array));
				return o;
			}
		case COMBINED_TYPE_SS:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_ARRAY);
				sll_string_split(&(a->data.string),&(b->data.string),&(o->data.array));
				return o;
			}
		case COMBINED_TYPE_SA:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_ARRAY);
				sll_string_op_array(&(a->data.string),&(b->data.array),sll_operator_mod,0,&(o->data.array));
				return o;
			}
		case COMBINED_TYPE_SM:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_MAP);
				sll_string_op_map(&(a->data.string),&(b->data.map),sll_operator_mod,0,&(o->data.map));
				return o;
			}
		case COMBINED_TYPE_AI:
		case COMBINED_TYPE_AF:
		case COMBINED_TYPE_AC:
			{
				sll_object_t* o=sll_array_length_to_object(a->data.array.length);
				for (sll_array_length_t i=0;i<a->data.array.length;i++){
					o->data.array.data[i]=sll_operator_mod(a->data.array.data[i],b);
				}
				return o;
			}
		case COMBINED_TYPE_AS:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_ARRAY);
				sll_string_op_array(&(b->data.string),&(a->data.array),sll_operator_mod,1,&(o->data.array));
				return o;
			}
		case COMBINED_TYPE_AA:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_ARRAY);
				sll_array_op(&(a->data.array),&(b->data.array),sll_operator_mod,&(o->data.array));
				return o;
			}
		case COMBINED_TYPE_AM:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_MAP);
				sll_array_op_map(&(a->data.array),&(b->data.map),sll_operator_mod,0,&(o->data.map));
				return o;
			}
		case COMBINED_TYPE_MI:
		case COMBINED_TYPE_MF:
		case COMBINED_TYPE_MC:
			{
				sll_object_t* o=sll_map_length_to_object(a->data.map.length);
				for (sll_map_length_t i=0;i<(a->data.map.length<<1);i+=2){
					o->data.map.data[i]=a->data.map.data[i];
					SLL_ACQUIRE(o->data.map.data[i]);
					o->data.map.data[i+1]=sll_operator_mod(a->data.map.data[i+1],b);
				}
				return o;
			}
		case COMBINED_TYPE_MS:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_MAP);
				sll_map_and_string(&(a->data.map),&(b->data.string),&(o->data.map));
				return o;
			}
		case COMBINED_TYPE_MA:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_MAP);
				sll_map_and_array(&(a->data.map),&(b->data.array),&(o->data.map));
				return o;
			}
		case COMBINED_TYPE_MM:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_MAP);
				sll_map_and(&(a->data.map),&(b->data.map),&(o->data.map));
				return o;
			}
		default:
			SLL_UNREACHABLE();
	}
	return SLL_ACQUIRE_STATIC_INT(0);
}
