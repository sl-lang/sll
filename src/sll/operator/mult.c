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



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_operator_mult(sll_object_t* a,sll_object_t* b){
	COMMUTATIVE_OPERATOR;
	switch (COMBINED_ARGS){
		case COMBINED_TYPE_II:
			return sll_int_to_object(a->dt.i*b->dt.i);
		case COMBINED_TYPE_IF:
			return sll_float_to_object(a->dt.i*b->dt.f);
		case COMBINED_TYPE_IC:
			return sll_int_to_object(a->dt.i*b->dt.c);
		case COMBINED_TYPE_IS:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_STRING);
				sll_string_duplicate(&(b->dt.s),a->dt.i*(inv?1:-1),0,&(o->dt.s));
				return o;
			}
		case COMBINED_TYPE_IA:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_ARRAY);
				sll_array_duplicate(&(b->dt.a),a->dt.i*(inv?1:-1),0,&(o->dt.a));
				return o;
			}
		case COMBINED_TYPE_IM:
		case COMBINED_TYPE_FM:
		case COMBINED_TYPE_CM:
		case COMBINED_TYPE_SM:
		case COMBINED_TYPE_AM:
			{
				sll_map_t m=b->dt.m;
				sll_object_t* o=sll_map_length_to_object(m.l);
				for (sll_map_length_t i=0;i<(m.l<<1);i+=2){
					SLL_ACQUIRE(m.v[i]);
					o->dt.m.v[i]=m.v[i];
					o->dt.m.v[i+1]=sll_operator_mult(a,m.v[i+1]);
				}
				return o;
			}
		case COMBINED_TYPE_FF:
			return sll_float_to_object(a->dt.f*b->dt.f);
		case COMBINED_TYPE_FC:
			return sll_float_to_object(a->dt.f*b->dt.c);
		case COMBINED_TYPE_FS:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_STRING);
				sll_integer_t n=(sll_integer_t)a->dt.f;
				sll_string_duplicate(&(b->dt.s),n,(sll_string_length_t)sll_api_math_round(sll_math_abs(a->dt.f-n)*b->dt.s.l),&(o->dt.s));
				return o;
			}
		case COMBINED_TYPE_FA:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_ARRAY);
				sll_integer_t n=(sll_integer_t)a->dt.f;
				sll_array_duplicate(&(b->dt.a),n,(sll_array_length_t)sll_api_math_round(sll_math_abs(a->dt.f-n)*b->dt.a.l),&(o->dt.a));
				return o;
			}
		case COMBINED_TYPE_CC:
			return SLL_FROM_CHAR(a->dt.c*b->dt.c);
		case COMBINED_TYPE_CS:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_STRING);
				sll_string_duplicate(&(b->dt.s),a->dt.c,0,&(o->dt.s));
				return o;
			}
		case COMBINED_TYPE_CA:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_ARRAY);
				sll_array_duplicate(&(b->dt.a),a->dt.c,0,&(o->dt.a));
				return o;
			}
		case COMBINED_TYPE_SS:
			{
				sll_string_t sa=a->dt.s;
				sll_string_t sb=b->dt.s;
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_ARRAY);
				sll_string_combinations(&sa,&sb,&(o->dt.a));
				return o;
			}
		case COMBINED_TYPE_SA:
			{
				sll_string_t sa=a->dt.s;
				sll_array_t ab=b->dt.a;
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_ARRAY);
				sll_array_combinations_string(&ab,&sa,&(o->dt.a));
				return o;
			}
		case COMBINED_TYPE_AA:
			{
				sll_array_t aa=a->dt.a;
				sll_array_t ab=b->dt.a;
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_ARRAY);
				sll_array_combinations(&aa,&ab,&(o->dt.a));
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
