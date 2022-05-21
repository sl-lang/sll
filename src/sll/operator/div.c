#include <sll/_internal/common.h>
#include <sll/_internal/operator.h>
#include <sll/array.h>
#include <sll/common.h>
#include <sll/gc.h>
#include <sll/map.h>
#include <sll/object.h>
#include <sll/static_object.h>
#include <sll/string.h>
#include <sll/types.h>
#include <math.h>



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_operator_div(sll_object_t* a,sll_object_t* b){
	if (a->t==SLL_OBJECT_TYPE_ARRAY){
		return sll_int_to_object((b->t==SLL_OBJECT_TYPE_ARRAY?sll_array_count_multiple(&(a->dt.array),b->dt.array.data,b->dt.array.length):sll_array_count(&(a->dt.array),b)));
	}
	switch (COMBINED_ARGS){
		case COMBINED_TYPE_II:
			return sll_float_to_object(((sll_float_t)(a->dt.int_value))/b->dt.int_value);
		case COMBINED_TYPE_IF:
			return sll_float_to_object(a->dt.int_value/b->dt.float_value);
		case COMBINED_TYPE_IC:
			return sll_float_to_object(((sll_float_t)(a->dt.int_value))/b->dt.char_value);
		case COMBINED_TYPE_IS:
		case COMBINED_TYPE_FS:
			{
				sll_object_t* o=sll_array_length_to_object(b->dt.array.length);
				for (sll_string_length_t i=0;i<b->dt.string.l;i++){
					o->dt.array.data[i]=sll_operator_div(a,sll_static_char[b->dt.string.v[i]]);
				}
				return o;
			}
		case COMBINED_TYPE_IA:
		case COMBINED_TYPE_FA:
		case COMBINED_TYPE_CA:
			{
				sll_object_t* o=sll_array_length_to_object(b->dt.array.length);
				for (sll_array_length_t i=0;i<b->dt.array.length;i++){
					o->dt.array.data[i]=sll_operator_div(a,b->dt.array.data[i]);
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
					o->dt.map.data[i+1]=sll_operator_div(a,b->dt.map.data[i+1]);
				}
				return o;
			}
		case COMBINED_TYPE_FI:
			return sll_float_to_object(a->dt.float_value/b->dt.int_value);
		case COMBINED_TYPE_FF:
			return sll_float_to_object(a->dt.float_value/b->dt.float_value);
		case COMBINED_TYPE_FC:
			return sll_float_to_object(a->dt.float_value/b->dt.char_value);
		case COMBINED_TYPE_CI:
			return sll_float_to_object(((sll_float_t)(a->dt.char_value))/b->dt.int_value);
		case COMBINED_TYPE_CF:
			return sll_float_to_object(((sll_float_t)(a->dt.char_value))/b->dt.float_value);
		case COMBINED_TYPE_CC:
			return sll_float_to_object(((sll_float_t)(a->dt.char_value))/b->dt.char_value);
		case COMBINED_TYPE_CS:
			return sll_int_to_object(sll_string_count_char(&(b->dt.string),a->dt.char_value));
		case COMBINED_TYPE_SI:
		case COMBINED_TYPE_SF:
			{
				sll_object_t* o=sll_array_length_to_object(a->dt.string.l);
				for (sll_string_length_t i=0;i<a->dt.string.l;i++){
					o->dt.array.data[i]=sll_operator_div(sll_static_char[a->dt.string.v[i]],b);
				}
				return o;
			}
		case COMBINED_TYPE_SC:
			return sll_int_to_object(sll_string_count_char(&(a->dt.string),b->dt.char_value));
		case COMBINED_TYPE_SS:
			return sll_int_to_object(sll_string_count(&(a->dt.string),&(b->dt.string)));
		case COMBINED_TYPE_SA:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_ARRAY);
				sll_string_op_array(&(a->dt.string),&(b->dt.array),sll_operator_div,0,&(o->dt.array));
				return o;
			}
		case COMBINED_TYPE_SM:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_MAP);
				sll_map_remove_string(&(b->dt.map),&(a->dt.string),&(o->dt.map));
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
					o->dt.map.data[i+1]=sll_operator_div(a->dt.map.data[i+1],b);
				}
				return o;
			}
		case COMBINED_TYPE_MS:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_MAP);
				sll_map_remove_string(&(a->dt.map),&(b->dt.string),&(o->dt.map));
				return o;
			}
		case COMBINED_TYPE_MA:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_MAP);
				sll_map_remove_array(&(a->dt.map),&(b->dt.array),&(o->dt.map));
				return o;
			}
		case COMBINED_TYPE_MM:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_MAP);
				sll_map_remove_map(&(a->dt.map),&(b->dt.map),&(o->dt.map));
				return o;
			}
		default:
			SLL_UNREACHABLE();
	}
	return SLL_ACQUIRE_STATIC_INT(0);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_operator_floor_div(sll_object_t* a,sll_object_t* b){
	if (a->t==SLL_OBJECT_TYPE_ARRAY){
		return sll_int_to_object((b->t==SLL_OBJECT_TYPE_ARRAY?sll_array_count_multiple(&(a->dt.array),b->dt.array.data,b->dt.array.length):sll_array_count(&(a->dt.array),b)));
	}
	switch (COMBINED_ARGS){
		case COMBINED_TYPE_II:
			return sll_int_to_object(a->dt.int_value/b->dt.int_value);
		case COMBINED_TYPE_IF:
			return sll_int_to_object((sll_integer_t)round(a->dt.int_value/b->dt.float_value));
		case COMBINED_TYPE_IC:
			return sll_int_to_object(a->dt.int_value/b->dt.char_value);
		case COMBINED_TYPE_IS:
		case COMBINED_TYPE_FS:
			{
				sll_object_t* o=sll_array_length_to_object(b->dt.string.l);
				for (sll_string_length_t i=0;i<b->dt.string.l;i++){
					o->dt.array.data[i]=sll_operator_floor_div(a,sll_static_char[b->dt.string.v[i]]);
				}
				return o;
			}
		case COMBINED_TYPE_IA:
		case COMBINED_TYPE_FA:
		case COMBINED_TYPE_CA:
			{
				sll_object_t* o=sll_array_length_to_object(b->dt.array.length);
				for (sll_array_length_t i=0;i<b->dt.array.length;i++){
					o->dt.array.data[i]=sll_operator_floor_div(a,b->dt.array.data[i]);
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
					o->dt.map.data[i+1]=sll_operator_floor_div(a,b->dt.map.data[i+1]);
				}
				return o;
			}
		case COMBINED_TYPE_FI:
			return sll_int_to_object((sll_integer_t)round(a->dt.float_value/b->dt.int_value));
		case COMBINED_TYPE_FF:
			return sll_int_to_object((sll_integer_t)round(a->dt.float_value/b->dt.float_value));
		case COMBINED_TYPE_FC:
			return sll_int_to_object((sll_integer_t)round(a->dt.float_value/b->dt.char_value));
		case COMBINED_TYPE_CI:
			return sll_int_to_object(a->dt.char_value/b->dt.int_value);
		case COMBINED_TYPE_CF:
			return sll_int_to_object((sll_integer_t)round(a->dt.char_value/b->dt.float_value));
		case COMBINED_TYPE_CC:
			return SLL_FROM_CHAR(a->dt.char_value/b->dt.char_value);
		case COMBINED_TYPE_CS:
			return sll_int_to_object(sll_string_count_char(&(b->dt.string),a->dt.char_value));
		case COMBINED_TYPE_SI:
		case COMBINED_TYPE_SF:
			{
				sll_object_t* o=sll_array_length_to_object(a->dt.string.l);
				for (sll_string_length_t i=0;i<a->dt.string.l;i++){
					o->dt.array.data[i]=sll_operator_floor_div(sll_static_char[a->dt.string.v[i]],b);
				}
				return o;
			}
		case COMBINED_TYPE_SC:
			return sll_int_to_object(sll_string_count_char(&(a->dt.string),b->dt.char_value));
		case COMBINED_TYPE_SS:
			return sll_int_to_object(sll_string_count(&(a->dt.string),&(b->dt.string)));
		case COMBINED_TYPE_SA:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_ARRAY);
				sll_string_op_array(&(a->dt.string),&(b->dt.array),sll_operator_floor_div,0,&(o->dt.array));
				return o;
			}
		case COMBINED_TYPE_SM:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_MAP);
				sll_map_remove_string(&(b->dt.map),&(a->dt.string),&(o->dt.map));
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
					o->dt.map.data[i+1]=sll_operator_floor_div(a->dt.map.data[i+1],b);
				}
				return o;
			}
		case COMBINED_TYPE_MS:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_MAP);
				sll_map_remove_string(&(a->dt.map),&(b->dt.string),&(o->dt.map));
				return o;
			}
		case COMBINED_TYPE_MA:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_MAP);
				sll_map_remove_array(&(a->dt.map),&(b->dt.array),&(o->dt.map));
				return o;
			}
		case COMBINED_TYPE_MM:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_MAP);
				sll_map_remove_map(&(a->dt.map),&(b->dt.map),&(o->dt.map));
				return o;
			}
		default:
			SLL_UNREACHABLE();
	}
	return SLL_ACQUIRE_STATIC_INT(0);
}
