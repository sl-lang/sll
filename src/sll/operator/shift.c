#include <sll/_internal/common.h>
#include <sll/_internal/operator.h>
#include <sll/api/math.h>
#include <sll/array.h>
#include <sll/common.h>
#include <sll/gc.h>
#include <sll/map.h>
#include <sll/object.h>
#include <sll/static_object.h>
#include <sll/string.h>
#include <sll/types.h>



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_operator_shl(sll_object_t* a,sll_object_t* b){
	switch (COMBINED_ARGS){
		case COMBINED_TYPE_II:
			return sll_int_to_object(a->dt.i<<b->dt.i);
		case COMBINED_TYPE_IF:
			return sll_float_to_object(a->dt.i*sll_api_math_pow(2,b->dt.f));
		case COMBINED_TYPE_IC:
			return sll_int_to_object(a->dt.i<<b->dt.c);
		case COMBINED_TYPE_IS:
		case COMBINED_TYPE_FS:
		case COMBINED_TYPE_CS:
			{
				sll_object_t* o=sll_array_length_to_object(b->dt.s.l);
				for (sll_string_length_t i=0;i<b->dt.s.l;i++){
					o->dt.a.v[i]=sll_operator_shl(a,sll_static_char[b->dt.s.v[i]]);
				}
				return o;
			}
		case COMBINED_TYPE_IA:
		case COMBINED_TYPE_FA:
		case COMBINED_TYPE_CA:
			{
				sll_object_t* o=sll_array_length_to_object(b->dt.a.l);
				for (sll_array_length_t i=0;i<b->dt.a.l;i++){
					o->dt.a.v[i]=sll_operator_shl(a,b->dt.a.v[i]);
				}
				return o;
			}
		case COMBINED_TYPE_IM:
		case COMBINED_TYPE_FM:
		case COMBINED_TYPE_CM:
			{
				sll_object_t* o=sll_map_length_to_object(b->dt.m.l);
				for (sll_map_length_t i=0;i<(b->dt.m.l<<1);i+=2){
					o->dt.m.v[i]=b->dt.m.v[i];
					SLL_ACQUIRE(o->dt.m.v[i]);
					o->dt.m.v[i+1]=sll_operator_shl(a,b->dt.m.v[i+1]);
				}
				return o;
			}
		case COMBINED_TYPE_FI:
			return sll_float_to_object(a->dt.f*sll_api_math_pow(2,(sll_float_t)(b->dt.i)));
		case COMBINED_TYPE_FF:
			return sll_float_to_object(a->dt.f*sll_api_math_pow(2,b->dt.f));
		case COMBINED_TYPE_FC:
			return sll_float_to_object(a->dt.f*sll_api_math_pow(2,b->dt.c));
		case COMBINED_TYPE_CI:
			return sll_int_to_object(((sll_integer_t)a->dt.c)<<b->dt.i);
		case COMBINED_TYPE_CF:
			return sll_float_to_object(a->dt.c*sll_api_math_pow(2,b->dt.f));
		case COMBINED_TYPE_CC:
			return SLL_FROM_CHAR((((sll_integer_t)a->dt.c))<<b->dt.c);
		case COMBINED_TYPE_SI:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_STRING);
				sll_string_resize(&(a->dt.s),b->dt.i,&(o->dt.s));
				return o;
			}
		case COMBINED_TYPE_SF:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_STRING);
				sll_string_resize(&(a->dt.s),sll_api_math_round(b->dt.f),&(o->dt.s));
				return o;
			}
		case COMBINED_TYPE_SC:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_STRING);
				sll_string_resize(&(a->dt.s),b->dt.c,&(o->dt.s));
				return o;
			}
		case COMBINED_TYPE_SS:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_STRING);
				sll_string_op(&(a->dt.s),&(b->dt.s),sll_operator_shl,&(o->dt.s));
				return o;
			}
		case COMBINED_TYPE_SA:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_ARRAY);
				sll_string_op_array(&(a->dt.s),&(b->dt.a),sll_operator_shl,0,&(o->dt.a));
				return o;
			}
		case COMBINED_TYPE_SM:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_MAP);
				sll_string_op_map(&(a->dt.s),&(b->dt.m),sll_operator_shl,0,&(o->dt.m));
				return o;
			}
		case COMBINED_TYPE_AI:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_ARRAY);
				sll_array_resize(&(a->dt.a),b->dt.i,&(o->dt.a));
				return o;
			}
		case COMBINED_TYPE_AF:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_ARRAY);
				sll_array_resize(&(a->dt.a),sll_api_math_round(b->dt.f),&(o->dt.a));
				return o;
			}
		case COMBINED_TYPE_AC:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_ARRAY);
				sll_array_resize(&(a->dt.a),b->dt.c,&(o->dt.a));
				return o;
			}
		case COMBINED_TYPE_AS:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_ARRAY);
				sll_string_op_array(&(b->dt.s),&(a->dt.a),sll_operator_shl,1,&(o->dt.a));
				return o;
			}
		case COMBINED_TYPE_AA:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_ARRAY);
				sll_array_op(&(a->dt.a),&(b->dt.a),sll_operator_shl,&(o->dt.a));
				return o;
			}
		case COMBINED_TYPE_AM:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_MAP);
				sll_array_op_map(&(a->dt.a),&(b->dt.m),sll_operator_shl,0,&(o->dt.m));
				return o;
			}
		case COMBINED_TYPE_MI:
		case COMBINED_TYPE_MF:
		case COMBINED_TYPE_MC:
			{
				sll_object_t* o=sll_map_length_to_object(a->dt.m.l);
				for (sll_map_length_t i=0;i<(a->dt.m.l<<1);i+=2){
					o->dt.m.v[i]=a->dt.m.v[i];
					SLL_ACQUIRE(o->dt.m.v[i]);
					o->dt.m.v[i+1]=sll_operator_shl(a->dt.m.v[i+1],b);
				}
				return o;
			}
		case COMBINED_TYPE_MS:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_MAP);
				sll_string_op_map(&(b->dt.s),&(a->dt.m),sll_operator_shl,1,&(o->dt.m));
				return o;
			}
		case COMBINED_TYPE_MA:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_MAP);
				sll_array_op_map(&(b->dt.a),&(a->dt.m),sll_operator_shl,1,&(o->dt.m));
				return o;
			}
		case COMBINED_TYPE_MM:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_MAP);
				sll_map_op(&(a->dt.m),&(b->dt.m),sll_operator_shl,&(o->dt.m));
				return o;
			}
		default:
			SLL_UNREACHABLE();
	}
	return SLL_ACQUIRE_STATIC_INT(0);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_operator_shr(sll_object_t* a,sll_object_t* b){
	switch (COMBINED_ARGS){
		case COMBINED_TYPE_II:
			return sll_int_to_object(a->dt.i>>b->dt.i);
		case COMBINED_TYPE_IF:
			return sll_float_to_object(a->dt.i/sll_api_math_pow(2,b->dt.f));
		case COMBINED_TYPE_IC:
			return sll_int_to_object(a->dt.i>>b->dt.c);
		case COMBINED_TYPE_IS:
		case COMBINED_TYPE_FS:
		case COMBINED_TYPE_CS:
			{
				sll_object_t* o=sll_array_length_to_object(b->dt.s.l);
				for (sll_string_length_t i=0;i<b->dt.s.l;i++){
					o->dt.a.v[i]=sll_operator_shr(a,sll_static_char[b->dt.s.v[i]]);
				}
				return o;
			}
		case COMBINED_TYPE_IA:
		case COMBINED_TYPE_FA:
		case COMBINED_TYPE_CA:
			{
				sll_object_t* o=sll_array_length_to_object(b->dt.a.l);
				for (sll_array_length_t i=0;i<b->dt.a.l;i++){
					o->dt.a.v[i]=sll_operator_shr(a,b->dt.a.v[i]);
				}
				return o;
			}
		case COMBINED_TYPE_IM:
		case COMBINED_TYPE_FM:
		case COMBINED_TYPE_CM:
			{
				sll_object_t* o=sll_map_length_to_object(b->dt.m.l);
				for (sll_map_length_t i=0;i<(b->dt.m.l<<1);i+=2){
					o->dt.m.v[i]=b->dt.m.v[i];
					SLL_ACQUIRE(o->dt.m.v[i]);
					o->dt.m.v[i+1]=sll_operator_shr(a,b->dt.m.v[i+1]);
				}
				return o;
			}
		case COMBINED_TYPE_FI:
			return sll_float_to_object(a->dt.f/sll_api_math_pow(2,(sll_float_t)(b->dt.i)));
		case COMBINED_TYPE_FF:
			return sll_float_to_object(a->dt.f/sll_api_math_pow(2,b->dt.f));
		case COMBINED_TYPE_FC:
			return sll_float_to_object(a->dt.f/sll_api_math_pow(2,b->dt.c));
		case COMBINED_TYPE_CI:
			return sll_int_to_object(a->dt.c>>b->dt.i);
		case COMBINED_TYPE_CF:
			return sll_float_to_object(a->dt.c/sll_api_math_pow(2,b->dt.f));
		case COMBINED_TYPE_CC:
			return SLL_FROM_CHAR(a->dt.c>>b->dt.c);
		case COMBINED_TYPE_SI:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_STRING);
				sll_string_resize(&(a->dt.s),-b->dt.i,&(o->dt.s));
				return o;
			}
		case COMBINED_TYPE_SF:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_STRING);
				sll_string_resize(&(a->dt.s),-sll_api_math_round(b->dt.f),&(o->dt.s));
				return o;
			}
		case COMBINED_TYPE_SC:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_STRING);
				sll_string_resize(&(a->dt.s),-b->dt.c,&(o->dt.s));
				return o;
			}
		case COMBINED_TYPE_SS:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_STRING);
				sll_string_op(&(a->dt.s),&(b->dt.s),sll_operator_shr,&(o->dt.s));
				return o;
			}
		case COMBINED_TYPE_SA:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_ARRAY);
				sll_string_op_array(&(a->dt.s),&(b->dt.a),sll_operator_shr,0,&(o->dt.a));
				return o;
			}
		case COMBINED_TYPE_SM:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_MAP);
				sll_string_op_map(&(a->dt.s),&(b->dt.m),sll_operator_shr,0,&(o->dt.m));
				return o;
			}
		case COMBINED_TYPE_AI:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_ARRAY);
				sll_array_resize(&(a->dt.a),-b->dt.i,&(o->dt.a));
				return o;
			}
		case COMBINED_TYPE_AF:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_ARRAY);
				sll_array_resize(&(a->dt.a),-sll_api_math_round(b->dt.f),&(o->dt.a));
				return o;
			}
		case COMBINED_TYPE_AC:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_ARRAY);
				sll_array_resize(&(a->dt.a),-b->dt.c,&(o->dt.a));
				return o;
			}
		case COMBINED_TYPE_AS:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_ARRAY);
				sll_string_op_array(&(b->dt.s),&(a->dt.a),sll_operator_shr,1,&(o->dt.a));
				return o;
			}
		case COMBINED_TYPE_AA:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_ARRAY);
				sll_array_op(&(a->dt.a),&(b->dt.a),sll_operator_shr,&(o->dt.a));
				return o;
			}
		case COMBINED_TYPE_AM:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_MAP);
				sll_array_op_map(&(a->dt.a),&(b->dt.m),sll_operator_shr,0,&(o->dt.m));
				return o;
			}
		case COMBINED_TYPE_MI:
		case COMBINED_TYPE_MF:
		case COMBINED_TYPE_MC:
			{
				sll_object_t* o=sll_map_length_to_object(a->dt.m.l);
				for (sll_map_length_t i=0;i<(a->dt.m.l<<1);i+=2){
					o->dt.m.v[i]=a->dt.m.v[i];
					SLL_ACQUIRE(o->dt.m.v[i]);
					o->dt.m.v[i+1]=sll_operator_shr(a->dt.m.v[i+1],b);
				}
				return o;
			}
		case COMBINED_TYPE_MS:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_MAP);
				sll_string_op_map(&(b->dt.s),&(a->dt.m),sll_operator_shr,1,&(o->dt.m));
				return o;
			}
		case COMBINED_TYPE_MA:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_MAP);
				sll_array_op_map(&(b->dt.a),&(a->dt.m),sll_operator_shr,1,&(o->dt.m));
				return o;
			}
		case COMBINED_TYPE_MM:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_MAP);
				sll_map_op(&(a->dt.m),&(b->dt.m),sll_operator_shr,&(o->dt.m));
				return o;
			}
		default:
			SLL_UNREACHABLE();
	}
	return SLL_ACQUIRE_STATIC_INT(0);
}
