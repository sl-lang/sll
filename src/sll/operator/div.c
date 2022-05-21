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
	if (a->type==SLL_OBJECT_TYPE_ARRAY){
		return sll_int_to_object((b->type==SLL_OBJECT_TYPE_ARRAY?sll_array_count_multiple(&(a->data.array),b->data.array.data,b->data.array.length):sll_array_count(&(a->data.array),b)));
	}
	switch (COMBINED_ARGS){
		case COMBINED_TYPE_II:
			return sll_float_to_object(((sll_float_t)(a->data.int_))/b->data.int_);
		case COMBINED_TYPE_IF:
			return sll_float_to_object(a->data.int_/b->data.float_);
		case COMBINED_TYPE_IC:
			return sll_float_to_object(((sll_float_t)(a->data.int_))/b->data.char_);
		case COMBINED_TYPE_IS:
		case COMBINED_TYPE_FS:
			{
				sll_object_t* o=sll_array_length_to_object(b->data.array.length);
				for (sll_string_length_t i=0;i<b->data.string.l;i++){
					o->data.array.data[i]=sll_operator_div(a,sll_static_char[b->data.string.v[i]]);
				}
				return o;
			}
		case COMBINED_TYPE_IA:
		case COMBINED_TYPE_FA:
		case COMBINED_TYPE_CA:
			{
				sll_object_t* o=sll_array_length_to_object(b->data.array.length);
				for (sll_array_length_t i=0;i<b->data.array.length;i++){
					o->data.array.data[i]=sll_operator_div(a,b->data.array.data[i]);
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
					o->data.map.data[i+1]=sll_operator_div(a,b->data.map.data[i+1]);
				}
				return o;
			}
		case COMBINED_TYPE_FI:
			return sll_float_to_object(a->data.float_/b->data.int_);
		case COMBINED_TYPE_FF:
			return sll_float_to_object(a->data.float_/b->data.float_);
		case COMBINED_TYPE_FC:
			return sll_float_to_object(a->data.float_/b->data.char_);
		case COMBINED_TYPE_CI:
			return sll_float_to_object(((sll_float_t)(a->data.char_))/b->data.int_);
		case COMBINED_TYPE_CF:
			return sll_float_to_object(((sll_float_t)(a->data.char_))/b->data.float_);
		case COMBINED_TYPE_CC:
			return sll_float_to_object(((sll_float_t)(a->data.char_))/b->data.char_);
		case COMBINED_TYPE_CS:
			return sll_int_to_object(sll_string_count_char(&(b->data.string),a->data.char_));
		case COMBINED_TYPE_SI:
		case COMBINED_TYPE_SF:
			{
				sll_object_t* o=sll_array_length_to_object(a->data.string.l);
				for (sll_string_length_t i=0;i<a->data.string.l;i++){
					o->data.array.data[i]=sll_operator_div(sll_static_char[a->data.string.v[i]],b);
				}
				return o;
			}
		case COMBINED_TYPE_SC:
			return sll_int_to_object(sll_string_count_char(&(a->data.string),b->data.char_));
		case COMBINED_TYPE_SS:
			return sll_int_to_object(sll_string_count(&(a->data.string),&(b->data.string)));
		case COMBINED_TYPE_SA:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_ARRAY);
				sll_string_op_array(&(a->data.string),&(b->data.array),sll_operator_div,0,&(o->data.array));
				return o;
			}
		case COMBINED_TYPE_SM:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_MAP);
				sll_map_remove_string(&(b->data.map),&(a->data.string),&(o->data.map));
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
					o->data.map.data[i+1]=sll_operator_div(a->data.map.data[i+1],b);
				}
				return o;
			}
		case COMBINED_TYPE_MS:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_MAP);
				sll_map_remove_string(&(a->data.map),&(b->data.string),&(o->data.map));
				return o;
			}
		case COMBINED_TYPE_MA:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_MAP);
				sll_map_remove_array(&(a->data.map),&(b->data.array),&(o->data.map));
				return o;
			}
		case COMBINED_TYPE_MM:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_MAP);
				sll_map_remove_map(&(a->data.map),&(b->data.map),&(o->data.map));
				return o;
			}
		default:
			SLL_UNREACHABLE();
	}
	return SLL_ACQUIRE_STATIC_INT(0);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_operator_floor_div(sll_object_t* a,sll_object_t* b){
	if (a->type==SLL_OBJECT_TYPE_ARRAY){
		return sll_int_to_object((b->type==SLL_OBJECT_TYPE_ARRAY?sll_array_count_multiple(&(a->data.array),b->data.array.data,b->data.array.length):sll_array_count(&(a->data.array),b)));
	}
	switch (COMBINED_ARGS){
		case COMBINED_TYPE_II:
			return sll_int_to_object(a->data.int_/b->data.int_);
		case COMBINED_TYPE_IF:
			return sll_int_to_object((sll_integer_t)round(a->data.int_/b->data.float_));
		case COMBINED_TYPE_IC:
			return sll_int_to_object(a->data.int_/b->data.char_);
		case COMBINED_TYPE_IS:
		case COMBINED_TYPE_FS:
			{
				sll_object_t* o=sll_array_length_to_object(b->data.string.l);
				for (sll_string_length_t i=0;i<b->data.string.l;i++){
					o->data.array.data[i]=sll_operator_floor_div(a,sll_static_char[b->data.string.v[i]]);
				}
				return o;
			}
		case COMBINED_TYPE_IA:
		case COMBINED_TYPE_FA:
		case COMBINED_TYPE_CA:
			{
				sll_object_t* o=sll_array_length_to_object(b->data.array.length);
				for (sll_array_length_t i=0;i<b->data.array.length;i++){
					o->data.array.data[i]=sll_operator_floor_div(a,b->data.array.data[i]);
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
					o->data.map.data[i+1]=sll_operator_floor_div(a,b->data.map.data[i+1]);
				}
				return o;
			}
		case COMBINED_TYPE_FI:
			return sll_int_to_object((sll_integer_t)round(a->data.float_/b->data.int_));
		case COMBINED_TYPE_FF:
			return sll_int_to_object((sll_integer_t)round(a->data.float_/b->data.float_));
		case COMBINED_TYPE_FC:
			return sll_int_to_object((sll_integer_t)round(a->data.float_/b->data.char_));
		case COMBINED_TYPE_CI:
			return sll_int_to_object(a->data.char_/b->data.int_);
		case COMBINED_TYPE_CF:
			return sll_int_to_object((sll_integer_t)round(a->data.char_/b->data.float_));
		case COMBINED_TYPE_CC:
			return SLL_FROM_CHAR(a->data.char_/b->data.char_);
		case COMBINED_TYPE_CS:
			return sll_int_to_object(sll_string_count_char(&(b->data.string),a->data.char_));
		case COMBINED_TYPE_SI:
		case COMBINED_TYPE_SF:
			{
				sll_object_t* o=sll_array_length_to_object(a->data.string.l);
				for (sll_string_length_t i=0;i<a->data.string.l;i++){
					o->data.array.data[i]=sll_operator_floor_div(sll_static_char[a->data.string.v[i]],b);
				}
				return o;
			}
		case COMBINED_TYPE_SC:
			return sll_int_to_object(sll_string_count_char(&(a->data.string),b->data.char_));
		case COMBINED_TYPE_SS:
			return sll_int_to_object(sll_string_count(&(a->data.string),&(b->data.string)));
		case COMBINED_TYPE_SA:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_ARRAY);
				sll_string_op_array(&(a->data.string),&(b->data.array),sll_operator_floor_div,0,&(o->data.array));
				return o;
			}
		case COMBINED_TYPE_SM:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_MAP);
				sll_map_remove_string(&(b->data.map),&(a->data.string),&(o->data.map));
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
					o->data.map.data[i+1]=sll_operator_floor_div(a->data.map.data[i+1],b);
				}
				return o;
			}
		case COMBINED_TYPE_MS:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_MAP);
				sll_map_remove_string(&(a->data.map),&(b->data.string),&(o->data.map));
				return o;
			}
		case COMBINED_TYPE_MA:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_MAP);
				sll_map_remove_array(&(a->data.map),&(b->data.array),&(o->data.map));
				return o;
			}
		case COMBINED_TYPE_MM:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_MAP);
				sll_map_remove_map(&(a->data.map),&(b->data.map),&(o->data.map));
				return o;
			}
		default:
			SLL_UNREACHABLE();
	}
	return SLL_ACQUIRE_STATIC_INT(0);
}
