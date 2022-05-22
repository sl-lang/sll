#include <sll/_internal/common.h>
#include <sll/_internal/operator.h>
#include <sll/_internal/util.h>
#include <sll/array.h>
#include <sll/common.h>
#include <sll/gc.h>
#include <sll/map.h>
#include <sll/object.h>
#include <sll/static_object.h>
#include <sll/string.h>
#include <sll/types.h>
#include <math.h>



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_operator_and(sll_object_t* a,sll_object_t* b){
	COMMUTATIVE_OPERATOR;
	switch (COMBINED_ARGS){
		case COMBINED_TYPE_II:
			return sll_int_to_object(a->data.int_&b->data.int_);
		case COMBINED_TYPE_IF:
			{
				f64_data_t v={
					.data=a->data.int_&TO_FLOAT_BITS(b)
				};
				return sll_float_to_object(v.value);
			}
		case COMBINED_TYPE_IC:
			return sll_int_to_object(a->data.int_&b->data.char_);
		case COMBINED_TYPE_IS:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_STRING);
				sll_string_and_char(&(b->data.string),(sll_char_t)(a->data.int_),&(o->data.string));
				return o;
			}
		case COMBINED_TYPE_IA:
		case COMBINED_TYPE_FA:
		case COMBINED_TYPE_CA:
			{
				sll_object_t* o=sll_array_length_to_object(b->data.array.length);
				for (sll_array_length_t i=0;i<b->data.array.length;i++){
					o->data.array.data[i]=sll_operator_and(a,b->data.array.data[i]);
				}
				return o;
			}
		case COMBINED_TYPE_IM:
		case COMBINED_TYPE_FM:
		case COMBINED_TYPE_CM:
			{
				sll_object_t* o=sll_map_length_to_object(b->data.map.length);
				for (sll_map_length_t i=0;i<(b->data.array.length<<1);i+=2){
					o->data.map.data[i]=a->data.map.data[i];
					SLL_ACQUIRE(a->data.map.data[i]);
					o->data.map.data[i+1]=sll_operator_and(a,b->data.map.data[i+1]);
				}
				return o;
			}
		case COMBINED_TYPE_FF:
			{
				f64_data_t v={
					.data=TO_FLOAT_BITS(a)&TO_FLOAT_BITS(b)
				};
				return sll_float_to_object(v.value);
			}
		case COMBINED_TYPE_FC:
			{
				f64_data_t v={
					.data=TO_FLOAT_BITS(a)&b->data.char_
				};
				return sll_float_to_object(v.value);
			}
		case COMBINED_TYPE_FS:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_STRING);
				sll_string_and_char(&(b->data.string),(sll_char_t)round(a->data.float_),&(o->data.string));
				return o;
			}
		case COMBINED_TYPE_CC:
			return SLL_FROM_CHAR(a->data.char_&b->data.char_);
		case COMBINED_TYPE_CS:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_STRING);
				sll_string_and_char(&(b->data.string),a->data.char_,&(o->data.string));
				return o;
			}
		case COMBINED_TYPE_SS:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_STRING);
				sll_string_and(&(a->data.string),&(b->data.string),&(o->data.string));
				return o;
			}
		case COMBINED_TYPE_SA:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_ARRAY);
				sll_string_op_array(&(a->data.string),&(b->data.array),sll_operator_and,inv,&(o->data.array));
				return o;
			}
		case COMBINED_TYPE_SM:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_MAP);
				sll_map_and_string(&(b->data.map),&(a->data.string),&(o->data.map));
				return o;
			}
		case COMBINED_TYPE_AA:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_ARRAY);
				sll_array_and(&(a->data.array),&(b->data.array),&(o->data.array));
				return o;
			}
		case COMBINED_TYPE_AM:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_MAP);
				sll_map_and_array(&(b->data.map),&(a->data.array),&(o->data.map));
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



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_operator_inv(sll_object_t* a){
	switch (a->type){
		case SLL_OBJECT_TYPE_INT:
			return sll_int_to_object(~a->data.int_);
		case SLL_OBJECT_TYPE_FLOAT:
			return sll_float_to_object(-a->data.float_-1);
		case SLL_OBJECT_TYPE_CHAR:
			return SLL_FROM_CHAR(~a->data.char_);
		case SLL_OBJECT_TYPE_COMPLEX:
			{
				sll_complex_t n={
					-a->data.complex_.real-1,
					-a->data.complex_.imag-1
				};
				return sll_complex_to_object(&n);
			}
		case SLL_OBJECT_TYPE_STRING:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_STRING);
				sll_string_inv(&(a->data.string),&(o->data.string));
				return o;
			}
		case SLL_OBJECT_TYPE_ARRAY:
			{
				sll_object_t* o=sll_array_length_to_object(a->data.array.length);
				for (sll_array_length_t i=0;i<a->data.array.length;i++){
					o->data.array.data[i]=sll_operator_inv(a->data.array.data[i]);
				}
				return o;
			}
		case SLL_OBJECT_TYPE_MAP:
			{
				sll_object_t* o=sll_map_length_to_object(a->data.map.length);
				for (sll_array_length_t i=0;i<(a->data.map.length<<1);i+=2){
					o->data.map.data[i]=a->data.map.data[i];
					SLL_ACQUIRE(o->data.map.data[i]);
					o->data.map.data[i+1]=sll_operator_inv(a->data.map.data[i+1]);
				}
				return o;
			}
		default:
			SLL_UNREACHABLE();
	}
	return SLL_ACQUIRE_STATIC_INT(0);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_operator_or(sll_object_t* a,sll_object_t* b){
	COMMUTATIVE_OPERATOR;
	switch (COMBINED_ARGS){
		case COMBINED_TYPE_II:
			return sll_int_to_object(a->data.int_|b->data.int_);
		case COMBINED_TYPE_IF:
			{
				f64_data_t v={
					.data=a->data.int_|TO_FLOAT_BITS(b)
				};
				return sll_float_to_object(v.value);
			}
		case COMBINED_TYPE_IC:
			return sll_int_to_object(a->data.int_|b->data.char_);
		case COMBINED_TYPE_IS:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_STRING);
				sll_string_or_char(&(b->data.string),(sll_char_t)(a->data.int_),&(o->data.string));
				return o;
			}
		case COMBINED_TYPE_IA:
		case COMBINED_TYPE_FA:
		case COMBINED_TYPE_CA:
			{
				sll_object_t* o=sll_array_length_to_object(b->data.array.length);
				for (sll_array_length_t i=0;i<b->data.array.length;i++){
					o->data.array.data[i]=sll_operator_or(a,b->data.array.data[i]);
				}
				return o;
			}
		case COMBINED_TYPE_IM:
		case COMBINED_TYPE_FM:
		case COMBINED_TYPE_CM:
			{
				sll_object_t* o=sll_map_length_to_object(b->data.map.length);
				for (sll_map_length_t i=0;i<(b->data.array.length<<1);i+=2){
					o->data.map.data[i]=a->data.map.data[i];
					SLL_ACQUIRE(a->data.map.data[i]);
					o->data.map.data[i+1]=sll_operator_or(a,b->data.map.data[i+1]);
				}
				return o;
			}
		case COMBINED_TYPE_FF:
			{
				f64_data_t v={
					.data=TO_FLOAT_BITS(a)|TO_FLOAT_BITS(b)
				};
				return sll_float_to_object(v.value);
			}
		case COMBINED_TYPE_FC:
			{
				f64_data_t v={
					.data=TO_FLOAT_BITS(a)|b->data.char_
				};
				return sll_float_to_object(v.value);
			}
		case COMBINED_TYPE_FS:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_STRING);
				sll_string_or_char(&(b->data.string),(sll_char_t)round(a->data.float_),&(o->data.string));
				return o;
			}
		case COMBINED_TYPE_CC:
			return SLL_FROM_CHAR(a->data.char_|b->data.char_);
		case COMBINED_TYPE_CS:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_STRING);
				sll_string_or_char(&(b->data.string),a->data.char_,&(o->data.string));
				return o;
			}
		case COMBINED_TYPE_SS:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_STRING);
				sll_string_or(&(a->data.string),&(b->data.string),&(o->data.string));
				return o;
			}
		case COMBINED_TYPE_SA:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_ARRAY);
				sll_string_op_array(&(a->data.string),&(b->data.array),sll_operator_or,inv,&(o->data.array));
				return o;
			}
		case COMBINED_TYPE_SM:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_MAP);
				sll_map_add_string(&(b->data.map),&(a->data.string),&(o->data.map));
				return o;
			}
		case COMBINED_TYPE_AA:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_ARRAY);
				sll_array_or(&(a->data.array),&(b->data.array),&(o->data.array));
				return o;
			}
		case COMBINED_TYPE_AM:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_MAP);
				sll_map_add_array(&(b->data.map),&(a->data.array),&(o->data.map));
				return o;
			}
		case COMBINED_TYPE_MM:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_MAP);
				sll_map_join(&(a->data.map),&(b->data.map),&(o->data.map));
				return o;
			}
		default:
			SLL_UNREACHABLE();
	}
	return SLL_ACQUIRE_STATIC_INT(0);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_operator_xor(sll_object_t* a,sll_object_t* b){
	COMMUTATIVE_OPERATOR;
	switch (COMBINED_ARGS){
		case COMBINED_TYPE_II:
			return sll_int_to_object(a->data.int_^b->data.int_);
		case COMBINED_TYPE_IF:
			{
				f64_data_t v={
					.data=a->data.int_^TO_FLOAT_BITS(b)
				};
				return sll_float_to_object(v.value);
			}
		case COMBINED_TYPE_IC:
			return sll_int_to_object(a->data.int_^b->data.char_);
		case COMBINED_TYPE_IS:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_STRING);
				sll_string_xor_char(&(b->data.string),(sll_char_t)(a->data.int_),&(o->data.string));
				return o;
			}
		case COMBINED_TYPE_IA:
		case COMBINED_TYPE_FA:
		case COMBINED_TYPE_CA:
			{
				sll_object_t* o=sll_array_length_to_object(b->data.array.length);
				for (sll_array_length_t i=0;i<b->data.array.length;i++){
					o->data.array.data[i]=sll_operator_xor(a,b->data.array.data[i]);
				}
				return o;
			}
		case COMBINED_TYPE_IM:
		case COMBINED_TYPE_FM:
		case COMBINED_TYPE_CM:
			{
				sll_object_t* o=sll_map_length_to_object(b->data.map.length);
				for (sll_map_length_t i=0;i<(b->data.array.length<<1);i+=2){
					o->data.map.data[i]=a->data.map.data[i];
					SLL_ACQUIRE(a->data.map.data[i]);
					o->data.map.data[i+1]=sll_operator_xor(a,b->data.map.data[i+1]);
				}
				return o;
			}
		case COMBINED_TYPE_FF:
			{
				f64_data_t v={
					.data=TO_FLOAT_BITS(a)^TO_FLOAT_BITS(b)
				};
				return sll_float_to_object(v.value);
			}
		case COMBINED_TYPE_FC:
			{
				f64_data_t v={
					.data=TO_FLOAT_BITS(a)^b->data.char_
				};
				return sll_float_to_object(v.value);
			}
		case COMBINED_TYPE_FS:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_STRING);
				sll_string_xor_char(&(b->data.string),(sll_char_t)round(a->data.float_),&(o->data.string));
				return o;
			}
		case COMBINED_TYPE_CC:
			return SLL_FROM_CHAR(a->data.char_^b->data.char_);
		case COMBINED_TYPE_CS:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_STRING);
				sll_string_xor_char(&(b->data.string),a->data.char_,&(o->data.string));
				return o;
			}
		case COMBINED_TYPE_SS:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_STRING);
				sll_string_xor(&(a->data.string),&(b->data.string),&(o->data.string));
				return o;
			}
		case COMBINED_TYPE_SA:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_ARRAY);
				sll_string_op_array(&(a->data.string),&(b->data.array),sll_operator_xor,inv,&(o->data.array));
				return o;
			}
		case COMBINED_TYPE_SM:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_MAP);
				sll_map_add_string(&(b->data.map),&(a->data.string),&(o->data.map));
				return o;
			}
		case COMBINED_TYPE_AA:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_ARRAY);
				sll_array_xor(&(a->data.array),&(b->data.array),&(o->data.array));
				return o;
			}
		case COMBINED_TYPE_AM:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_MAP);
				sll_map_add_array(&(b->data.map),&(a->data.array),&(o->data.map));
				return o;
			}
		case COMBINED_TYPE_MM:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_MAP);
				sll_map_join(&(a->data.map),&(b->data.map),&(o->data.map));
				return o;
			}
		default:
			SLL_UNREACHABLE();
	}
	return SLL_ACQUIRE_STATIC_INT(0);
}
