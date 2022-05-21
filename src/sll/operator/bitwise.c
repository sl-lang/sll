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
			return sll_int_to_object(a->dt.int_&b->dt.int_);
		case COMBINED_TYPE_IF:
			{
				f64_data_t v={
					.dt=a->dt.int_&TO_FLOAT_BITS(b)
				};
				return sll_float_to_object(v.v);
			}
		case COMBINED_TYPE_IC:
			return sll_int_to_object(a->dt.int_&b->dt.char_);
		case COMBINED_TYPE_IS:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_STRING);
				sll_string_and_char(&(b->dt.string),(sll_char_t)(a->dt.int_),&(o->dt.string));
				return o;
			}
		case COMBINED_TYPE_IA:
		case COMBINED_TYPE_FA:
		case COMBINED_TYPE_CA:
			{
				sll_object_t* o=sll_array_length_to_object(b->dt.array.length);
				for (sll_array_length_t i=0;i<b->dt.array.length;i++){
					o->dt.array.data[i]=sll_operator_and(a,b->dt.array.data[i]);
				}
				return o;
			}
		case COMBINED_TYPE_IM:
		case COMBINED_TYPE_FM:
		case COMBINED_TYPE_CM:
			{
				sll_object_t* o=sll_map_length_to_object(b->dt.map.length);
				for (sll_map_length_t i=0;i<(b->dt.array.length<<1);i+=2){
					o->dt.map.data[i]=a->dt.map.data[i];
					SLL_ACQUIRE(a->dt.map.data[i]);
					o->dt.map.data[i+1]=sll_operator_and(a,b->dt.map.data[i+1]);
				}
				return o;
			}
		case COMBINED_TYPE_FF:
			{
				f64_data_t v={
					.dt=TO_FLOAT_BITS(a)&TO_FLOAT_BITS(b)
				};
				return sll_float_to_object(v.v);
			}
		case COMBINED_TYPE_FC:
			{
				f64_data_t v={
					.dt=TO_FLOAT_BITS(a)&b->dt.char_
				};
				return sll_float_to_object(v.v);
			}
		case COMBINED_TYPE_FS:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_STRING);
				sll_string_and_char(&(b->dt.string),(sll_char_t)round(a->dt.float_),&(o->dt.string));
				return o;
			}
		case COMBINED_TYPE_CC:
			return SLL_FROM_CHAR(a->dt.char_&b->dt.char_);
		case COMBINED_TYPE_CS:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_STRING);
				sll_string_and_char(&(b->dt.string),a->dt.char_,&(o->dt.string));
				return o;
			}
		case COMBINED_TYPE_SS:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_STRING);
				sll_string_and(&(a->dt.string),&(b->dt.string),&(o->dt.string));
				return o;
			}
		case COMBINED_TYPE_SA:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_ARRAY);
				sll_string_op_array(&(a->dt.string),&(b->dt.array),sll_operator_and,inv,&(o->dt.array));
				return o;
			}
		case COMBINED_TYPE_SM:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_MAP);
				sll_map_and_string(&(b->dt.map),&(a->dt.string),&(o->dt.map));
				return o;
			}
		case COMBINED_TYPE_AA:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_ARRAY);
				sll_array_and(&(a->dt.array),&(b->dt.array),&(o->dt.array));
				return o;
			}
		case COMBINED_TYPE_AM:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_MAP);
				sll_map_and_array(&(b->dt.map),&(a->dt.array),&(o->dt.map));
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



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_operator_inv(sll_object_t* a){
	switch (a->t){
		case SLL_OBJECT_TYPE_INT:
			return sll_int_to_object(~a->dt.int_);
		case SLL_OBJECT_TYPE_FLOAT:
			return sll_float_to_object(-a->dt.float_-1);
		case SLL_OBJECT_TYPE_CHAR:
			return SLL_FROM_CHAR(~a->dt.char_);
		case SLL_OBJECT_TYPE_COMPLEX:
			{
				sll_complex_t n={
					-a->dt.complex_.real-1,
					-a->dt.complex_.imag-1
				};
				return sll_complex_to_object(&n);
			}
		case SLL_OBJECT_TYPE_STRING:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_STRING);
				sll_string_inv(&(a->dt.string),&(o->dt.string));
				return o;
			}
		case SLL_OBJECT_TYPE_ARRAY:
			{
				sll_object_t* o=sll_array_length_to_object(a->dt.array.length);
				for (sll_array_length_t i=0;i<a->dt.array.length;i++){
					o->dt.array.data[i]=sll_operator_inv(a->dt.array.data[i]);
				}
				return o;
			}
		case SLL_OBJECT_TYPE_MAP:
			{
				sll_object_t* o=sll_map_length_to_object(a->dt.map.length);
				for (sll_array_length_t i=0;i<(a->dt.map.length<<1);i+=2){
					o->dt.map.data[i]=a->dt.map.data[i];
					SLL_ACQUIRE(o->dt.map.data[i]);
					o->dt.map.data[i+1]=sll_operator_inv(a->dt.map.data[i+1]);
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
			return sll_int_to_object(a->dt.int_|b->dt.int_);
		case COMBINED_TYPE_IF:
			{
				f64_data_t v={
					.dt=a->dt.int_|TO_FLOAT_BITS(b)
				};
				return sll_float_to_object(v.v);
			}
		case COMBINED_TYPE_IC:
			return sll_int_to_object(a->dt.int_|b->dt.char_);
		case COMBINED_TYPE_IS:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_STRING);
				sll_string_or_char(&(b->dt.string),(sll_char_t)(a->dt.int_),&(o->dt.string));
				return o;
			}
		case COMBINED_TYPE_IA:
		case COMBINED_TYPE_FA:
		case COMBINED_TYPE_CA:
			{
				sll_object_t* o=sll_array_length_to_object(b->dt.array.length);
				for (sll_array_length_t i=0;i<b->dt.array.length;i++){
					o->dt.array.data[i]=sll_operator_or(a,b->dt.array.data[i]);
				}
				return o;
			}
		case COMBINED_TYPE_IM:
		case COMBINED_TYPE_FM:
		case COMBINED_TYPE_CM:
			{
				sll_object_t* o=sll_map_length_to_object(b->dt.map.length);
				for (sll_map_length_t i=0;i<(b->dt.array.length<<1);i+=2){
					o->dt.map.data[i]=a->dt.map.data[i];
					SLL_ACQUIRE(a->dt.map.data[i]);
					o->dt.map.data[i+1]=sll_operator_or(a,b->dt.map.data[i+1]);
				}
				return o;
			}
		case COMBINED_TYPE_FF:
			{
				f64_data_t v={
					.dt=TO_FLOAT_BITS(a)|TO_FLOAT_BITS(b)
				};
				return sll_float_to_object(v.v);
			}
		case COMBINED_TYPE_FC:
			{
				f64_data_t v={
					.dt=TO_FLOAT_BITS(a)|b->dt.char_
				};
				return sll_float_to_object(v.v);
			}
		case COMBINED_TYPE_FS:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_STRING);
				sll_string_or_char(&(b->dt.string),(sll_char_t)round(a->dt.float_),&(o->dt.string));
				return o;
			}
		case COMBINED_TYPE_CC:
			return SLL_FROM_CHAR(a->dt.char_|b->dt.char_);
		case COMBINED_TYPE_CS:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_STRING);
				sll_string_or_char(&(b->dt.string),a->dt.char_,&(o->dt.string));
				return o;
			}
		case COMBINED_TYPE_SS:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_STRING);
				sll_string_or(&(a->dt.string),&(b->dt.string),&(o->dt.string));
				return o;
			}
		case COMBINED_TYPE_SA:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_ARRAY);
				sll_string_op_array(&(a->dt.string),&(b->dt.array),sll_operator_or,inv,&(o->dt.array));
				return o;
			}
		case COMBINED_TYPE_SM:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_MAP);
				sll_map_add_string(&(b->dt.map),&(a->dt.string),&(o->dt.map));
				return o;
			}
		case COMBINED_TYPE_AA:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_ARRAY);
				sll_array_or(&(a->dt.array),&(b->dt.array),&(o->dt.array));
				return o;
			}
		case COMBINED_TYPE_AM:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_MAP);
				sll_map_add_array(&(b->dt.map),&(a->dt.array),&(o->dt.map));
				return o;
			}
		case COMBINED_TYPE_MM:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_MAP);
				sll_map_join(&(a->dt.map),&(b->dt.map),&(o->dt.map));
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
			return sll_int_to_object(a->dt.int_^b->dt.int_);
		case COMBINED_TYPE_IF:
			{
				f64_data_t v={
					.dt=a->dt.int_^TO_FLOAT_BITS(b)
				};
				return sll_float_to_object(v.v);
			}
		case COMBINED_TYPE_IC:
			return sll_int_to_object(a->dt.int_^b->dt.char_);
		case COMBINED_TYPE_IS:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_STRING);
				sll_string_xor_char(&(b->dt.string),(sll_char_t)(a->dt.int_),&(o->dt.string));
				return o;
			}
		case COMBINED_TYPE_IA:
		case COMBINED_TYPE_FA:
		case COMBINED_TYPE_CA:
			{
				sll_object_t* o=sll_array_length_to_object(b->dt.array.length);
				for (sll_array_length_t i=0;i<b->dt.array.length;i++){
					o->dt.array.data[i]=sll_operator_xor(a,b->dt.array.data[i]);
				}
				return o;
			}
		case COMBINED_TYPE_IM:
		case COMBINED_TYPE_FM:
		case COMBINED_TYPE_CM:
			{
				sll_object_t* o=sll_map_length_to_object(b->dt.map.length);
				for (sll_map_length_t i=0;i<(b->dt.array.length<<1);i+=2){
					o->dt.map.data[i]=a->dt.map.data[i];
					SLL_ACQUIRE(a->dt.map.data[i]);
					o->dt.map.data[i+1]=sll_operator_xor(a,b->dt.map.data[i+1]);
				}
				return o;
			}
		case COMBINED_TYPE_FF:
			{
				f64_data_t v={
					.dt=TO_FLOAT_BITS(a)^TO_FLOAT_BITS(b)
				};
				return sll_float_to_object(v.v);
			}
		case COMBINED_TYPE_FC:
			{
				f64_data_t v={
					.dt=TO_FLOAT_BITS(a)^b->dt.char_
				};
				return sll_float_to_object(v.v);
			}
		case COMBINED_TYPE_FS:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_STRING);
				sll_string_xor_char(&(b->dt.string),(sll_char_t)round(a->dt.float_),&(o->dt.string));
				return o;
			}
		case COMBINED_TYPE_CC:
			return SLL_FROM_CHAR(a->dt.char_^b->dt.char_);
		case COMBINED_TYPE_CS:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_STRING);
				sll_string_xor_char(&(b->dt.string),a->dt.char_,&(o->dt.string));
				return o;
			}
		case COMBINED_TYPE_SS:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_STRING);
				sll_string_xor(&(a->dt.string),&(b->dt.string),&(o->dt.string));
				return o;
			}
		case COMBINED_TYPE_SA:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_ARRAY);
				sll_string_op_array(&(a->dt.string),&(b->dt.array),sll_operator_xor,inv,&(o->dt.array));
				return o;
			}
		case COMBINED_TYPE_SM:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_MAP);
				sll_map_add_string(&(b->dt.map),&(a->dt.string),&(o->dt.map));
				return o;
			}
		case COMBINED_TYPE_AA:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_ARRAY);
				sll_array_xor(&(a->dt.array),&(b->dt.array),&(o->dt.array));
				return o;
			}
		case COMBINED_TYPE_AM:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_MAP);
				sll_map_add_array(&(b->dt.map),&(a->dt.array),&(o->dt.map));
				return o;
			}
		case COMBINED_TYPE_MM:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_MAP);
				sll_map_join(&(a->dt.map),&(b->dt.map),&(o->dt.map));
				return o;
			}
		default:
			SLL_UNREACHABLE();
	}
	return SLL_ACQUIRE_STATIC_INT(0);
}
