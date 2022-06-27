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



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_operator_mult(sll_object_t* a,sll_object_t* b){
	COMMUTATIVE_OPERATOR;
	switch (COMBINED_ARGS){
		case COMBINED_TYPE_II:
			return sll_int_to_object(a->data.int_*b->data.int_);
		case COMBINED_TYPE_IF:
			return sll_float_to_object(a->data.int_*b->data.float_);
		case COMBINED_TYPE_IC:
			return sll_int_to_object(a->data.int_*b->data.char_);
		case COMBINED_TYPE_IS:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_STRING);
				sll_string_duplicate(&(b->data.string),a->data.int_*(inv?1:-1),0,&(o->data.string));
				return o;
			}
		case COMBINED_TYPE_IA:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_ARRAY);
				sll_array_duplicate(&(b->data.array),a->data.int_*(inv?1:-1),0,&(o->data.array));
				return o;
			}
		case COMBINED_TYPE_IM:
		case COMBINED_TYPE_FM:
		case COMBINED_TYPE_CM:
		case COMBINED_TYPE_SM:
		case COMBINED_TYPE_AM:
			{
				sll_map_t m=b->data.map;
				sll_object_t* o=sll_map_length_to_object(m.length);
				for (sll_map_length_t i=0;i<(m.length<<1);i+=2){
					SLL_ACQUIRE(m.data[i]);
					o->data.map.data[i]=m.data[i];
					o->data.map.data[i+1]=sll_operator_mult(a,m.data[i+1]);
				}
				return o;
			}
		case COMBINED_TYPE_FF:
			return sll_float_to_object(a->data.float_*b->data.float_);
		case COMBINED_TYPE_FC:
			return sll_float_to_object(a->data.float_*b->data.char_);
		case COMBINED_TYPE_FS:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_STRING);
				sll_integer_t n=(sll_integer_t)a->data.float_;
				sll_string_duplicate(&(b->data.string),n,(sll_string_length_t)round(sll_math_abs(a->data.float_-n)*b->data.string.length),&(o->data.string));
				return o;
			}
		case COMBINED_TYPE_FA:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_ARRAY);
				sll_integer_t n=(sll_integer_t)a->data.float_;
				sll_array_duplicate(&(b->data.array),n,(sll_array_length_t)round(sll_math_abs(a->data.float_-n)*b->data.array.length),&(o->data.array));
				return o;
			}
		case COMBINED_TYPE_CC:
			return SLL_FROM_CHAR(a->data.char_*b->data.char_);
		case COMBINED_TYPE_CS:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_STRING);
				sll_string_duplicate(&(b->data.string),a->data.char_,0,&(o->data.string));
				return o;
			}
		case COMBINED_TYPE_CA:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_ARRAY);
				sll_array_duplicate(&(b->data.array),a->data.char_,0,&(o->data.array));
				return o;
			}
		case COMBINED_TYPE_SS:
			{
				sll_string_t sa=a->data.string;
				sll_string_t sb=b->data.string;
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_ARRAY);
				sll_string_combinations(&sa,&sb,&(o->data.array));
				return o;
			}
		case COMBINED_TYPE_SA:
			{
				sll_string_t sa=a->data.string;
				sll_array_t ab=b->data.array;
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_ARRAY);
				sll_array_combinations_string(&ab,&sa,&(o->data.array));
				return o;
			}
		case COMBINED_TYPE_AA:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_ARRAY);
				sll_array_combinations(&(a->data.array),&(b->data.array),&(o->data.array));
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
