#include <sll/_internal/common.h>
#include <sll/_internal/operator.h>
#include <sll/_internal/util.h>
#include <sll/api/math.h>
#include <sll/array.h>
#include <sll/common.h>
#include <sll/gc.h>
#include <sll/map.h>
#include <sll/object.h>
#include <sll/static_object.h>
#include <sll/string.h>
#include <sll/types.h>



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_operator_and(sll_object_t* a,sll_object_t* b){
	COMMUTATIVE_OPERATOR;
	switch (COMBINED_ARGS){
		case COMBINED_TYPE_II:
			return sll_int_to_object(a->dt.i&b->dt.i);
		case COMBINED_TYPE_IF:
			{
				double_data_t v={
					.dt=a->dt.i&TO_FLOAT_BITS(b)
				};
				return sll_float_to_object(v.v);
			}
		case COMBINED_TYPE_IC:
			return sll_int_to_object(a->dt.i&b->dt.c);
		case COMBINED_TYPE_IS:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_STRING);
				sll_string_and_char(&(b->dt.s),(sll_char_t)(a->dt.i),&(o->dt.s));
				return o;
			}
		case COMBINED_TYPE_IA:
		case COMBINED_TYPE_FA:
		case COMBINED_TYPE_CA:
			{
				sll_object_t* o=sll_array_length_to_object(b->dt.a.l);
				for (sll_array_length_t i=0;i<b->dt.a.l;i++){
					o->dt.a.v[i]=sll_operator_and(a,b->dt.a.v[i]);
				}
				return o;
			}
		case COMBINED_TYPE_IM:
		case COMBINED_TYPE_FM:
		case COMBINED_TYPE_CM:
			{
				sll_object_t* o=sll_map_length_to_object(b->dt.m.l);
				for (sll_map_length_t i=0;i<(b->dt.a.l<<1);i+=2){
					o->dt.m.v[i]=a->dt.m.v[i];
					SLL_ACQUIRE(a->dt.m.v[i]);
					o->dt.m.v[i+1]=sll_operator_and(a,b->dt.m.v[i+1]);
				}
				return o;
			}
		case COMBINED_TYPE_FF:
			{
				double_data_t v={
					.dt=TO_FLOAT_BITS(a)&TO_FLOAT_BITS(b)
				};
				return sll_float_to_object(v.v);
			}
		case COMBINED_TYPE_FC:
			{
				double_data_t v={
					.dt=TO_FLOAT_BITS(a)&b->dt.c
				};
				return sll_float_to_object(v.v);
			}
		case COMBINED_TYPE_FS:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_STRING);
				sll_string_and_char(&(b->dt.s),(sll_char_t)sll_api_math_round(a->dt.f),&(o->dt.s));
				return o;
			}
		case COMBINED_TYPE_CC:
			return SLL_FROM_CHAR(a->dt.c&b->dt.c);
		case COMBINED_TYPE_CS:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_STRING);
				sll_string_and_char(&(b->dt.s),a->dt.c,&(o->dt.s));
				return o;
			}
		case COMBINED_TYPE_SS:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_STRING);
				sll_string_and(&(a->dt.s),&(b->dt.s),&(o->dt.s));
				return o;
			}
		case COMBINED_TYPE_SA:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_ARRAY);
				sll_string_op_array(&(a->dt.s),&(b->dt.a),sll_operator_and,inv,&(o->dt.a));
				return o;
			}
		case COMBINED_TYPE_SM:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_MAP);
				sll_map_and_string(&(b->dt.m),&(a->dt.s),&(o->dt.m));
				return o;
			}
		case COMBINED_TYPE_AA:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_ARRAY);
				sll_array_and(&(a->dt.a),&(b->dt.a),&(o->dt.a));
				return o;
			}
		case COMBINED_TYPE_AM:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_MAP);
				sll_map_and_array(&(b->dt.m),&(a->dt.a),&(o->dt.m));
				return o;
			}
		case COMBINED_TYPE_MM:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_MAP);
				sll_map_and(&(a->dt.m),&(b->dt.m),&(o->dt.m));
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
			return sll_int_to_object(~a->dt.i);
		case SLL_OBJECT_TYPE_FLOAT:
			return sll_float_to_object(-a->dt.f-1);
		case SLL_OBJECT_TYPE_CHAR:
			return SLL_FROM_CHAR(~a->dt.c);
		case SLL_OBJECT_TYPE_COMPLEX:
			{
				sll_complex_t n={
					-a->dt.d.real-1,
					-a->dt.d.imag-1
				};
				return sll_complex_to_object(&n);
			}
		case SLL_OBJECT_TYPE_STRING:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_STRING);
				sll_string_inv(&(a->dt.s),&(o->dt.s));
				return o;
			}
		case SLL_OBJECT_TYPE_ARRAY:
			{
				sll_object_t* o=sll_array_length_to_object(a->dt.a.l);
				for (sll_array_length_t i=0;i<a->dt.a.l;i++){
					o->dt.a.v[i]=sll_operator_inv(a->dt.a.v[i]);
				}
				return o;
			}
		case SLL_OBJECT_TYPE_MAP:
			{
				sll_object_t* o=sll_map_length_to_object(a->dt.m.l);
				for (sll_array_length_t i=0;i<(a->dt.m.l<<1);i+=2){
					o->dt.m.v[i]=a->dt.m.v[i];
					SLL_ACQUIRE(o->dt.m.v[i]);
					o->dt.m.v[i+1]=sll_operator_inv(a->dt.m.v[i+1]);
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
			return sll_int_to_object(a->dt.i|b->dt.i);
		case COMBINED_TYPE_IF:
			{
				double_data_t v={
					.dt=a->dt.i|TO_FLOAT_BITS(b)
				};
				return sll_float_to_object(v.v);
			}
		case COMBINED_TYPE_IC:
			return sll_int_to_object(a->dt.i|b->dt.c);
		case COMBINED_TYPE_IS:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_STRING);
				sll_string_or_char(&(b->dt.s),(sll_char_t)(a->dt.i),&(o->dt.s));
				return o;
			}
		case COMBINED_TYPE_IA:
		case COMBINED_TYPE_FA:
		case COMBINED_TYPE_CA:
			{
				sll_object_t* o=sll_array_length_to_object(b->dt.a.l);
				for (sll_array_length_t i=0;i<b->dt.a.l;i++){
					o->dt.a.v[i]=sll_operator_or(a,b->dt.a.v[i]);
				}
				return o;
			}
		case COMBINED_TYPE_IM:
		case COMBINED_TYPE_FM:
		case COMBINED_TYPE_CM:
			{
				sll_object_t* o=sll_map_length_to_object(b->dt.m.l);
				for (sll_map_length_t i=0;i<(b->dt.a.l<<1);i+=2){
					o->dt.m.v[i]=a->dt.m.v[i];
					SLL_ACQUIRE(a->dt.m.v[i]);
					o->dt.m.v[i+1]=sll_operator_or(a,b->dt.m.v[i+1]);
				}
				return o;
			}
		case COMBINED_TYPE_FF:
			{
				double_data_t v={
					.dt=TO_FLOAT_BITS(a)|TO_FLOAT_BITS(b)
				};
				return sll_float_to_object(v.v);
			}
		case COMBINED_TYPE_FC:
			{
				double_data_t v={
					.dt=TO_FLOAT_BITS(a)|b->dt.c
				};
				return sll_float_to_object(v.v);
			}
		case COMBINED_TYPE_FS:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_STRING);
				sll_string_or_char(&(b->dt.s),(sll_char_t)sll_api_math_round(a->dt.f),&(o->dt.s));
				return o;
			}
		case COMBINED_TYPE_CC:
			return SLL_FROM_CHAR(a->dt.c|b->dt.c);
		case COMBINED_TYPE_CS:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_STRING);
				sll_string_or_char(&(b->dt.s),a->dt.c,&(o->dt.s));
				return o;
			}
		case COMBINED_TYPE_SS:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_STRING);
				sll_string_or(&(a->dt.s),&(b->dt.s),&(o->dt.s));
				return o;
			}
		case COMBINED_TYPE_SA:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_ARRAY);
				sll_string_op_array(&(a->dt.s),&(b->dt.a),sll_operator_or,inv,&(o->dt.a));
				return o;
			}
		case COMBINED_TYPE_SM:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_MAP);
				sll_map_add_string(&(b->dt.m),&(a->dt.s),&(o->dt.m));
				return o;
			}
		case COMBINED_TYPE_AA:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_ARRAY);
				sll_array_or(&(a->dt.a),&(b->dt.a),&(o->dt.a));
				return o;
			}
		case COMBINED_TYPE_AM:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_MAP);
				sll_map_add_array(&(b->dt.m),&(a->dt.a),&(o->dt.m));
				return o;
			}
		case COMBINED_TYPE_MM:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_MAP);
				sll_map_join(&(a->dt.m),&(b->dt.m),&(o->dt.m));
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
			return sll_int_to_object(a->dt.i^b->dt.i);
		case COMBINED_TYPE_IF:
			{
				double_data_t v={
					.dt=a->dt.i^TO_FLOAT_BITS(b)
				};
				return sll_float_to_object(v.v);
			}
		case COMBINED_TYPE_IC:
			return sll_int_to_object(a->dt.i^b->dt.c);
		case COMBINED_TYPE_IS:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_STRING);
				sll_string_xor_char(&(b->dt.s),(sll_char_t)(a->dt.i),&(o->dt.s));
				return o;
			}
		case COMBINED_TYPE_IA:
		case COMBINED_TYPE_FA:
		case COMBINED_TYPE_CA:
			{
				sll_object_t* o=sll_array_length_to_object(b->dt.a.l);
				for (sll_array_length_t i=0;i<b->dt.a.l;i++){
					o->dt.a.v[i]=sll_operator_xor(a,b->dt.a.v[i]);
				}
				return o;
			}
		case COMBINED_TYPE_IM:
		case COMBINED_TYPE_FM:
		case COMBINED_TYPE_CM:
			{
				sll_object_t* o=sll_map_length_to_object(b->dt.m.l);
				for (sll_map_length_t i=0;i<(b->dt.a.l<<1);i+=2){
					o->dt.m.v[i]=a->dt.m.v[i];
					SLL_ACQUIRE(a->dt.m.v[i]);
					o->dt.m.v[i+1]=sll_operator_xor(a,b->dt.m.v[i+1]);
				}
				return o;
			}
		case COMBINED_TYPE_FF:
			{
				double_data_t v={
					.dt=TO_FLOAT_BITS(a)^TO_FLOAT_BITS(b)
				};
				return sll_float_to_object(v.v);
			}
		case COMBINED_TYPE_FC:
			{
				double_data_t v={
					.dt=TO_FLOAT_BITS(a)^b->dt.c
				};
				return sll_float_to_object(v.v);
			}
		case COMBINED_TYPE_FS:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_STRING);
				sll_string_xor_char(&(b->dt.s),(sll_char_t)sll_api_math_round(a->dt.f),&(o->dt.s));
				return o;
			}
		case COMBINED_TYPE_CC:
			return SLL_FROM_CHAR(a->dt.c^b->dt.c);
		case COMBINED_TYPE_CS:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_STRING);
				sll_string_xor_char(&(b->dt.s),a->dt.c,&(o->dt.s));
				return o;
			}
		case COMBINED_TYPE_SS:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_STRING);
				sll_string_xor(&(a->dt.s),&(b->dt.s),&(o->dt.s));
				return o;
			}
		case COMBINED_TYPE_SA:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_ARRAY);
				sll_string_op_array(&(a->dt.s),&(b->dt.a),sll_operator_xor,inv,&(o->dt.a));
				return o;
			}
		case COMBINED_TYPE_SM:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_MAP);
				sll_map_add_string(&(b->dt.m),&(a->dt.s),&(o->dt.m));
				return o;
			}
		case COMBINED_TYPE_AA:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_ARRAY);
				sll_array_xor(&(a->dt.a),&(b->dt.a),&(o->dt.a));
				return o;
			}
		case COMBINED_TYPE_AM:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_MAP);
				sll_map_add_array(&(b->dt.m),&(a->dt.a),&(o->dt.m));
				return o;
			}
		case COMBINED_TYPE_MM:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_MAP);
				sll_map_join(&(a->dt.m),&(b->dt.m),&(o->dt.m));
				return o;
			}
		default:
			SLL_UNREACHABLE();
	}
	return SLL_ACQUIRE_STATIC_INT(0);
}
