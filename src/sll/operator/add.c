#include <sll/_internal/common.h>
#include <sll/_internal/complex.h>
#include <sll/_internal/operator.h>
#include <sll/api/string.h>
#include <sll/array.h>
#include <sll/common.h>
#include <sll/map.h>
#include <sll/object.h>
#include <sll/static_object.h>
#include <sll/types.h>



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_operator_add(sll_object_t* a,sll_object_t* b){
	COMMUTATIVE_OPERATOR;
	switch (COMBINED_ARGS){
		case COMBINED_TYPE_II:
			return sll_int_to_object(a->dt.i+b->dt.i);
		case COMBINED_TYPE_IF:
			return sll_float_to_object(a->dt.i+b->dt.f);
		case COMBINED_TYPE_IC:
			return sll_int_to_object(a->dt.i+b->dt.c);
		case COMBINED_TYPE_ID:
			{
				sll_complex_t n={
					a->dt.i+b->dt.d.real,
					b->dt.d.imag
				};
				return sll_complex_to_object(n);
			}
		case COMBINED_TYPE_IS:
		case COMBINED_TYPE_FS:
		case COMBINED_TYPE_CS:
		case COMBINED_TYPE_DS:
			{
				sll_object_t* tmp=a;
				a=b;
				b=tmp;
_add_to_string:
				SLL_ASSERT(a->t==SLL_OBJECT_TYPE_STRING);
				sll_string_t sa=a->dt.s;
				sll_string_t sb;
				sll_api_string_convert(&b,1,&sb);
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_STRING);
				if (!sa.l){
					o->dt.s=sb;
					return o;
				}
				if (!inv){
					sll_string_t tmp_s=sa;
					sa=sb;
					sb=tmp_s;
				}
				sll_string_concat(&sa,&sb,&(o->dt.s));
				sll_free_string((!inv?&sa:&sb));
				return o;
			}
		case COMBINED_TYPE_IA:
		case COMBINED_TYPE_FA:
		case COMBINED_TYPE_CA:
		case COMBINED_TYPE_DA:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_ARRAY);
				if (inv){
					sll_array_push(&(b->dt.a),a,&(o->dt.a));
				}
				else{
					sll_array_unshift(&(b->dt.a),a,&(o->dt.a));
				}
				return o;
			}
		case COMBINED_TYPE_IM:
		case COMBINED_TYPE_FM:
		case COMBINED_TYPE_CM:
		case COMBINED_TYPE_DM:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_MAP);
				sll_map_add(&(b->dt.m),a,sll_static_int[0],&(o->dt.m));
				return o;
			}
		case COMBINED_TYPE_FF:
			return sll_float_to_object(a->dt.f+b->dt.f);
		case COMBINED_TYPE_FC:
			return sll_float_to_object(a->dt.f+b->dt.c);
		case COMBINED_TYPE_FD:
			return sll_float_to_object(a->dt.f+b->dt.c);
		case COMBINED_TYPE_CC:
			return SLL_FROM_CHAR(a->dt.c+b->dt.c);
		case COMBINED_TYPE_CD:
			{
				sll_complex_t n={
					a->dt.c+b->dt.d.real,
					b->dt.d.imag
				};
				return sll_complex_to_object(n);
			}
		case COMBINED_TYPE_DD:
			return sll_complex_to_object(COMPLEX_ADD(a->dt.d,b->dt.d));
		case COMBINED_TYPE_SS:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_STRING);
				sll_string_concat(&(a->dt.s),&(b->dt.s),&(o->dt.s));
				return o;
			}
		case COMBINED_TYPE_SA:
			if (inv){
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_ARRAY);
				sll_array_push(&(b->dt.a),a,&(o->dt.a));
				return o;
			}
			goto _add_to_string;
		case COMBINED_TYPE_SM:
			if (inv){
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_MAP);
				sll_map_add(&(b->dt.m),a,sll_static_int[0],&(o->dt.m));
				return o;
			}
			goto _add_to_string;
		case COMBINED_TYPE_AA:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_ARRAY);
				sll_array_join(&(a->dt.a),&(b->dt.a),&(o->dt.a));
				return o;
			}
		case COMBINED_TYPE_AM:
			{
				if (inv){
					sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_MAP);
					sll_map_add(&(b->dt.m),a,sll_static_int[0],&(o->dt.m));
					return o;
				}
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_ARRAY);
				sll_array_push(&(a->dt.a),b,&(o->dt.a));
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
