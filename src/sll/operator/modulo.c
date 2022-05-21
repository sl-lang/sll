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
			return sll_int_to_object(a->dt.int_value%b->dt.int_value);
		case COMBINED_TYPE_IF:
			return sll_float_to_object(sll_math_mod((sll_float_t)(a->dt.int_value),b->dt.float_value));
		case COMBINED_TYPE_IC:
			return sll_int_to_object(a->dt.int_value%b->dt.char_value);
		case COMBINED_TYPE_IS:
		case COMBINED_TYPE_FS:
			{
				sll_object_t* o=sll_array_length_to_object(b->dt.string.l);
				for (sll_string_length_t i=0;i<b->dt.string.l;i++){
					o->dt.array.data[i]=sll_operator_mod(a,sll_static_char[b->dt.string.v[i]]);
				}
				return o;
			}
		case COMBINED_TYPE_IA:
		case COMBINED_TYPE_FA:
		case COMBINED_TYPE_CA:
			{
				sll_object_t* o=sll_array_length_to_object(b->dt.array.length);
				for (sll_array_length_t i=0;i<b->dt.array.length;i++){
					o->dt.array.data[i]=sll_operator_mod(a,b->dt.array.data[i]);
				}
				return o;
			}
		case COMBINED_TYPE_IM:
		case COMBINED_TYPE_FM:
		case COMBINED_TYPE_CM:
			{
				sll_object_t* o=sll_map_length_to_object(b->dt.map.length);
				for (sll_map_length_t i=0;i<(b->dt.map.length<<1);i+=2){
					o->dt.map.data[i]=b->dt.map.data[i];
					SLL_ACQUIRE(o->dt.map.data[i]);
					o->dt.map.data[i+1]=sll_operator_mod(a,b->dt.map.data[i+1]);
				}
				return o;
			}
		case COMBINED_TYPE_FI:
			return sll_float_to_object(sll_math_mod(a->dt.float_value,(sll_float_t)(b->dt.int_value)));
		case COMBINED_TYPE_FF:
			return sll_float_to_object(sll_math_mod(a->dt.float_value,b->dt.float_value));
		case COMBINED_TYPE_FC:
			return sll_float_to_object(sll_math_mod(a->dt.float_value,b->dt.char_value));
		case COMBINED_TYPE_CI:
			return SLL_FROM_CHAR(a->dt.char_value%b->dt.int_value);
		case COMBINED_TYPE_CF:
			return sll_float_to_object(sll_math_mod(a->dt.char_value,b->dt.float_value));
		case COMBINED_TYPE_CC:
			return SLL_FROM_CHAR(a->dt.char_value%b->dt.char_value);
		case COMBINED_TYPE_CS:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_ARRAY);
				sll_string_split_char(&(b->dt.string),a->dt.char_value,&(o->dt.array));
				return o;
			}
		case COMBINED_TYPE_SI:
		case COMBINED_TYPE_SF:
			{
				sll_object_t* o=sll_array_length_to_object(a->dt.string.l);
				for (sll_string_length_t i=0;i<a->dt.string.l;i++){
					o->dt.array.data[i]=sll_operator_mod(sll_static_char[a->dt.string.v[i]],b);
				}
				return o;
			}
		case COMBINED_TYPE_SC:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_ARRAY);
				sll_string_split_char(&(a->dt.string),b->dt.char_value,&(o->dt.array));
				return o;
			}
		case COMBINED_TYPE_SS:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_ARRAY);
				sll_string_split(&(a->dt.string),&(b->dt.string),&(o->dt.array));
				return o;
			}
		case COMBINED_TYPE_SA:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_ARRAY);
				sll_string_op_array(&(a->dt.string),&(b->dt.array),sll_operator_mod,0,&(o->dt.array));
				return o;
			}
		case COMBINED_TYPE_SM:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_MAP);
				sll_string_op_map(&(a->dt.string),&(b->dt.map),sll_operator_mod,0,&(o->dt.map));
				return o;
			}
		case COMBINED_TYPE_AI:
		case COMBINED_TYPE_AF:
		case COMBINED_TYPE_AC:
			{
				sll_object_t* o=sll_array_length_to_object(a->dt.array.length);
				for (sll_array_length_t i=0;i<a->dt.array.length;i++){
					o->dt.array.data[i]=sll_operator_mod(a->dt.array.data[i],b);
				}
				return o;
			}
		case COMBINED_TYPE_AS:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_ARRAY);
				sll_string_op_array(&(b->dt.string),&(a->dt.array),sll_operator_mod,1,&(o->dt.array));
				return o;
			}
		case COMBINED_TYPE_AA:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_ARRAY);
				sll_array_op(&(a->dt.array),&(b->dt.array),sll_operator_mod,&(o->dt.array));
				return o;
			}
		case COMBINED_TYPE_AM:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_MAP);
				sll_array_op_map(&(a->dt.array),&(b->dt.map),sll_operator_mod,0,&(o->dt.map));
				return o;
			}
		case COMBINED_TYPE_MI:
		case COMBINED_TYPE_MF:
		case COMBINED_TYPE_MC:
			{
				sll_object_t* o=sll_map_length_to_object(a->dt.map.length);
				for (sll_map_length_t i=0;i<(a->dt.map.length<<1);i+=2){
					o->dt.map.data[i]=a->dt.map.data[i];
					SLL_ACQUIRE(o->dt.map.data[i]);
					o->dt.map.data[i+1]=sll_operator_mod(a->dt.map.data[i+1],b);
				}
				return o;
			}
		case COMBINED_TYPE_MS:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_MAP);
				sll_map_and_string(&(a->dt.map),&(b->dt.string),&(o->dt.map));
				return o;
			}
		case COMBINED_TYPE_MA:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_MAP);
				sll_map_and_array(&(a->dt.map),&(b->dt.array),&(o->dt.map));
				return o;
			}
		case COMBINED_TYPE_MM:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_MAP);
				sll_map_and(&(a->dt.map),&(b->dt.map),&(o->dt.map));
				return o;
			}
		default:
			SLL_UNREACHABLE();
	}
	return SLL_ACQUIRE_STATIC_INT(0);
}
