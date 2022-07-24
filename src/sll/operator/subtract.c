#include <sll/_internal/common.h>
#include <sll/_internal/complex.h>
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



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t sll_operator_sub(sll_object_t a,sll_object_t b){
	if (a->type==SLL_OBJECT_TYPE_ARRAY){
		sll_object_t o=sll_create_object(SLL_OBJECT_TYPE_ARRAY);
		if (b->type==SLL_OBJECT_TYPE_ARRAY){
			sll_array_remove_multiple(&(a->data.array),b->data.array.data,b->data.array.length,&(o->data.array));
		}
		else{
			sll_array_remove(&(a->data.array),b,&(o->data.array));
		}
		return o;
	}
	if (a->type==SLL_OBJECT_TYPE_MAP){
		sll_object_t o=sll_create_object(SLL_OBJECT_TYPE_MAP);
		SLL_RELEASE(sll_map_remove(&(a->data.map),b,&(o->data.map)));
		return o;
	}
	switch (COMBINED_ARGS){
		case COMBINED_TYPE_II:
			return sll_int_to_object(a->data.int_-b->data.int_);
		case COMBINED_TYPE_IF:
			return sll_float_to_object(a->data.int_-b->data.float_);
		case COMBINED_TYPE_IC:
			return sll_int_to_object(a->data.int_-b->data.char_);
		case COMBINED_TYPE_IS:
		case COMBINED_TYPE_FS:
		case COMBINED_TYPE_CS:
			{
				sll_object_t o=sll_array_length_to_object(b->data.string.length);
				sll_array_create(b->data.string.length,&(o->data.array));
				for (sll_array_length_t i=0;i<b->data.string.length;i++){
					o->data.array.data[i]=sll_operator_sub(a,sll_static_char[b->data.string.data[i]]);
				}
				return o;
			}
		case COMBINED_TYPE_IA:
		case COMBINED_TYPE_FA:
		case COMBINED_TYPE_CA:
			{
				sll_object_t o=sll_array_length_to_object(b->data.array.length);
				for (sll_array_length_t i=0;i<b->data.array.length;i++){
					o->data.array.data[i]=sll_operator_sub(a,b->data.array.data[i]);
				}
				return o;
			}
		case COMBINED_TYPE_IM:
		case COMBINED_TYPE_FM:
		case COMBINED_TYPE_CM:
			{
				sll_map_t m=b->data.map;
				sll_object_t o=sll_map_length_to_object(m.length);
				for (sll_array_length_t i=0;i<(m.length<<1);i+=2){
					SLL_ACQUIRE(m.data[i]);
					o->data.map.data[i]=m.data[i];
					o->data.map.data[i+1]=sll_operator_sub(a,m.data[i+1]);
				}
				return o;
			}
		case COMBINED_TYPE_FI:
			return sll_float_to_object(a->data.float_-b->data.int_);
		case COMBINED_TYPE_FF:
			return sll_float_to_object(a->data.float_-b->data.float_);
		case COMBINED_TYPE_FC:
			return sll_float_to_object(a->data.float_-b->data.char_);
		case COMBINED_TYPE_CI:
			return sll_int_to_object(a->data.char_-b->data.int_);
		case COMBINED_TYPE_CF:
			return sll_float_to_object(a->data.char_-b->data.float_);
		case COMBINED_TYPE_CC:
			return SLL_FROM_CHAR(a->data.char_-b->data.char_);
		case COMBINED_TYPE_DD:
			{
				sll_complex_t o=COMPLEX_SUB(a->data.complex_,b->data.complex_);
				return sll_complex_to_object(&o);
			}
		case COMBINED_TYPE_SI:
		case COMBINED_TYPE_SF:
		case COMBINED_TYPE_SC:
			{
				sll_object_t o=sll_array_length_to_object(a->data.string.length);
				for (sll_string_length_t i=0;i<a->data.string.length;i++){
					o->data.array.data[i]=sll_operator_sub(sll_static_char[a->data.string.data[i]],b);
				}
				return o;
			}
		case COMBINED_TYPE_SS:
			{
				sll_object_t o=sll_create_object(SLL_OBJECT_TYPE_STRING);
				sll_string_remove(&(a->data.string),&(b->data.string),&(o->data.string));
				return o;
			}
		case COMBINED_TYPE_SA:
			{
				sll_object_t o=sll_create_object(SLL_OBJECT_TYPE_ARRAY);
				sll_string_op_array(&(a->data.string),&(b->data.array),sll_operator_sub,0,&(o->data.array));
				return o;
			}
		case COMBINED_TYPE_SM:
			{
				sll_object_t o=sll_create_object(SLL_OBJECT_TYPE_MAP);
				sll_string_op_map(&(a->data.string),&(b->data.map),sll_operator_sub,0,&(o->data.map));
				return o;
			}
		default:
			SLL_UNREACHABLE();
	}
	return SLL_ACQUIRE_STATIC_INT(0);
}
