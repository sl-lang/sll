#include <sll/_internal/common.h>
#include <sll/_internal/operator.h>
#include <sll/array.h>
#include <sll/common.h>
#include <sll/gc.h>
#include <sll/map.h>
#include <sll/object.h>
#include <sll/operator.h>
#include <sll/static_object.h>
#include <sll/string.h>
#include <sll/types.h>



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_operator_sub(sll_object_t* a,sll_object_t* b){
	if (a->t==SLL_OBJECT_TYPE_ARRAY){
		sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_ARRAY);
		if (b->t==SLL_OBJECT_TYPE_ARRAY){
			sll_array_remove_multiple(&(a->dt.array),b->dt.array.data,b->dt.array.length,&(o->dt.array));
		}
		else{
			sll_array_remove(&(a->dt.array),b,&(o->dt.array));
		}
		return o;
	}
	if (a->t==SLL_OBJECT_TYPE_MAP){
		sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_MAP);
		SLL_RELEASE(sll_map_remove(&(a->dt.map),b,&(o->dt.map)));
		return o;
	}
	switch (COMBINED_ARGS){
		case COMBINED_TYPE_II:
			return sll_int_to_object(a->dt.int_value-b->dt.int_value);
		case COMBINED_TYPE_IF:
			return sll_float_to_object(a->dt.int_value-b->dt.float_value);
		case COMBINED_TYPE_IC:
			return sll_int_to_object(a->dt.int_value-b->dt.char_value);
		case COMBINED_TYPE_IS:
		case COMBINED_TYPE_FS:
		case COMBINED_TYPE_CS:
			{
				sll_object_t* o=sll_array_length_to_object(b->dt.string.l);
				sll_array_create(b->dt.string.l,&(o->dt.array));
				for (sll_array_length_t i=0;i<b->dt.string.l;i++){
					o->dt.array.data[i]=sll_operator_sub(a,sll_static_char[b->dt.string.v[i]]);
				}
				return o;
			}
		case COMBINED_TYPE_IA:
		case COMBINED_TYPE_FA:
		case COMBINED_TYPE_CA:
			{
				sll_object_t* o=sll_array_length_to_object(b->dt.array.length);
				for (sll_array_length_t i=0;i<b->dt.array.length;i++){
					o->dt.array.data[i]=sll_operator_sub(a,b->dt.array.data[i]);
				}
				return o;
			}
		case COMBINED_TYPE_IM:
		case COMBINED_TYPE_FM:
		case COMBINED_TYPE_CM:
			{
				sll_map_t m=b->dt.map;
				sll_object_t* o=sll_map_length_to_object(m.length);
				for (sll_array_length_t i=0;i<(m.length<<1);i+=2){
					SLL_ACQUIRE(m.data[i]);
					o->dt.map.data[i]=m.data[i];
					o->dt.map.data[i+1]=sll_operator_sub(a,m.data[i+1]);
				}
				return o;
			}
		case COMBINED_TYPE_FI:
			return sll_float_to_object(a->dt.float_value-b->dt.int_value);
		case COMBINED_TYPE_FF:
			return sll_float_to_object(a->dt.float_value-b->dt.float_value);
		case COMBINED_TYPE_FC:
			return sll_float_to_object(a->dt.float_value-b->dt.char_value);
		case COMBINED_TYPE_CI:
			return sll_int_to_object(a->dt.char_value-b->dt.int_value);
		case COMBINED_TYPE_CF:
			return sll_float_to_object(a->dt.char_value-b->dt.float_value);
		case COMBINED_TYPE_CC:
			return SLL_FROM_CHAR(a->dt.char_value-b->dt.char_value);
		case COMBINED_TYPE_SI:
		case COMBINED_TYPE_SF:
		case COMBINED_TYPE_SC:
			{
				sll_object_t* o=sll_array_length_to_object(a->dt.string.l);
				for (sll_string_length_t i=0;i<a->dt.string.l;i++){
					o->dt.array.data[i]=sll_operator_sub(sll_static_char[a->dt.string.v[i]],b);
				}
				return o;
			}
		case COMBINED_TYPE_SS:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_STRING);
				sll_string_remove(&(a->dt.string),&(b->dt.string),&(o->dt.string));
				return o;
			}
		case COMBINED_TYPE_SA:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_ARRAY);
				sll_string_op_array(&(a->dt.string),&(b->dt.array),sll_operator_sub,0,&(o->dt.array));
				return o;
			}
		case COMBINED_TYPE_SM:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_MAP);
				sll_string_op_map(&(a->dt.string),&(b->dt.map),sll_operator_sub,0,&(o->dt.map));
				return o;
			}
		default:
			SLL_UNREACHABLE();
	}
	return SLL_ACQUIRE_STATIC_INT(0);
}
