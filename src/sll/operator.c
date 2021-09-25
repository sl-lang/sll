#include <sll/_sll_internal.h>
#include <sll/api/string.h>
#include <sll/array.h>
#include <sll/common.h>
#include <sll/constants.h>
#include <sll/gc.h>
#include <sll/handle.h>
#include <sll/map.h>
#include <sll/operator.h>
#include <sll/static_object.h>
#include <sll/string.h>
#include <sll/types.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>



#define COMBINE_TYPES(a,b) (((unsigned int)(a)<<8)|(b))
#define COMBINED_ARGS COMBINE_TYPES(SLL_RUNTIME_OBJECT_GET_TYPE(a),SLL_RUNTIME_OBJECT_GET_TYPE(b))
#define COMBINED_TYPE_II COMBINE_TYPES(SLL_RUNTIME_OBJECT_TYPE_INT,SLL_RUNTIME_OBJECT_TYPE_INT)
#define COMBINED_TYPE_IF COMBINE_TYPES(SLL_RUNTIME_OBJECT_TYPE_INT,SLL_RUNTIME_OBJECT_TYPE_FLOAT)
#define COMBINED_TYPE_IC COMBINE_TYPES(SLL_RUNTIME_OBJECT_TYPE_INT,SLL_RUNTIME_OBJECT_TYPE_CHAR)
#define COMBINED_TYPE_IS COMBINE_TYPES(SLL_RUNTIME_OBJECT_TYPE_INT,SLL_RUNTIME_OBJECT_TYPE_STRING)
#define COMBINED_TYPE_IA COMBINE_TYPES(SLL_RUNTIME_OBJECT_TYPE_INT,SLL_RUNTIME_OBJECT_TYPE_ARRAY)
#define COMBINED_TYPE_IH COMBINE_TYPES(SLL_RUNTIME_OBJECT_TYPE_INT,SLL_RUNTIME_OBJECT_TYPE_HANDLE)
#define COMBINED_TYPE_IM COMBINE_TYPES(SLL_RUNTIME_OBJECT_TYPE_INT,SLL_RUNTIME_OBJECT_TYPE_MAP)
#define COMBINED_TYPE_FI COMBINE_TYPES(SLL_RUNTIME_OBJECT_TYPE_FLOAT,SLL_RUNTIME_OBJECT_TYPE_INT)
#define COMBINED_TYPE_FF COMBINE_TYPES(SLL_RUNTIME_OBJECT_TYPE_FLOAT,SLL_RUNTIME_OBJECT_TYPE_FLOAT)
#define COMBINED_TYPE_FC COMBINE_TYPES(SLL_RUNTIME_OBJECT_TYPE_FLOAT,SLL_RUNTIME_OBJECT_TYPE_CHAR)
#define COMBINED_TYPE_FS COMBINE_TYPES(SLL_RUNTIME_OBJECT_TYPE_FLOAT,SLL_RUNTIME_OBJECT_TYPE_STRING)
#define COMBINED_TYPE_FA COMBINE_TYPES(SLL_RUNTIME_OBJECT_TYPE_FLOAT,SLL_RUNTIME_OBJECT_TYPE_ARRAY)
#define COMBINED_TYPE_FH COMBINE_TYPES(SLL_RUNTIME_OBJECT_TYPE_FLOAT,SLL_RUNTIME_OBJECT_TYPE_HANDLE)
#define COMBINED_TYPE_FM COMBINE_TYPES(SLL_RUNTIME_OBJECT_TYPE_FLOAT,SLL_RUNTIME_OBJECT_TYPE_MAP)
#define COMBINED_TYPE_CI COMBINE_TYPES(SLL_RUNTIME_OBJECT_TYPE_CHAR,SLL_RUNTIME_OBJECT_TYPE_INT)
#define COMBINED_TYPE_CF COMBINE_TYPES(SLL_RUNTIME_OBJECT_TYPE_CHAR,SLL_RUNTIME_OBJECT_TYPE_FLOAT)
#define COMBINED_TYPE_CC COMBINE_TYPES(SLL_RUNTIME_OBJECT_TYPE_CHAR,SLL_RUNTIME_OBJECT_TYPE_CHAR)
#define COMBINED_TYPE_CS COMBINE_TYPES(SLL_RUNTIME_OBJECT_TYPE_CHAR,SLL_RUNTIME_OBJECT_TYPE_STRING)
#define COMBINED_TYPE_CA COMBINE_TYPES(SLL_RUNTIME_OBJECT_TYPE_CHAR,SLL_RUNTIME_OBJECT_TYPE_ARRAY)
#define COMBINED_TYPE_CH COMBINE_TYPES(SLL_RUNTIME_OBJECT_TYPE_CHAR,SLL_RUNTIME_OBJECT_TYPE_HANDLE)
#define COMBINED_TYPE_CM COMBINE_TYPES(SLL_RUNTIME_OBJECT_TYPE_CHAR,SLL_RUNTIME_OBJECT_TYPE_MAP)
#define COMBINED_TYPE_SI COMBINE_TYPES(SLL_RUNTIME_OBJECT_TYPE_STRING,SLL_RUNTIME_OBJECT_TYPE_INT)
#define COMBINED_TYPE_SF COMBINE_TYPES(SLL_RUNTIME_OBJECT_TYPE_STRING,SLL_RUNTIME_OBJECT_TYPE_FLOAT)
#define COMBINED_TYPE_SC COMBINE_TYPES(SLL_RUNTIME_OBJECT_TYPE_STRING,SLL_RUNTIME_OBJECT_TYPE_CHAR)
#define COMBINED_TYPE_SS COMBINE_TYPES(SLL_RUNTIME_OBJECT_TYPE_STRING,SLL_RUNTIME_OBJECT_TYPE_STRING)
#define COMBINED_TYPE_SA COMBINE_TYPES(SLL_RUNTIME_OBJECT_TYPE_STRING,SLL_RUNTIME_OBJECT_TYPE_ARRAY)
#define COMBINED_TYPE_SH COMBINE_TYPES(SLL_RUNTIME_OBJECT_TYPE_STRING,SLL_RUNTIME_OBJECT_TYPE_HANDLE)
#define COMBINED_TYPE_SM COMBINE_TYPES(SLL_RUNTIME_OBJECT_TYPE_STRING,SLL_RUNTIME_OBJECT_TYPE_MAP)
#define COMBINED_TYPE_AI COMBINE_TYPES(SLL_RUNTIME_OBJECT_TYPE_ARRAY,SLL_RUNTIME_OBJECT_TYPE_INT)
#define COMBINED_TYPE_AF COMBINE_TYPES(SLL_RUNTIME_OBJECT_TYPE_ARRAY,SLL_RUNTIME_OBJECT_TYPE_FLOAT)
#define COMBINED_TYPE_AC COMBINE_TYPES(SLL_RUNTIME_OBJECT_TYPE_ARRAY,SLL_RUNTIME_OBJECT_TYPE_CHAR)
#define COMBINED_TYPE_AS COMBINE_TYPES(SLL_RUNTIME_OBJECT_TYPE_ARRAY,SLL_RUNTIME_OBJECT_TYPE_STRING)
#define COMBINED_TYPE_AA COMBINE_TYPES(SLL_RUNTIME_OBJECT_TYPE_ARRAY,SLL_RUNTIME_OBJECT_TYPE_ARRAY)
#define COMBINED_TYPE_AH COMBINE_TYPES(SLL_RUNTIME_OBJECT_TYPE_ARRAY,SLL_RUNTIME_OBJECT_TYPE_HANDLE)
#define COMBINED_TYPE_AM COMBINE_TYPES(SLL_RUNTIME_OBJECT_TYPE_ARRAY,SLL_RUNTIME_OBJECT_TYPE_MAP)
#define COMBINED_TYPE_HI COMBINE_TYPES(SLL_RUNTIME_OBJECT_TYPE_HANDLE,SLL_RUNTIME_OBJECT_TYPE_INT)
#define COMBINED_TYPE_HF COMBINE_TYPES(SLL_RUNTIME_OBJECT_TYPE_HANDLE,SLL_RUNTIME_OBJECT_TYPE_FLOAT)
#define COMBINED_TYPE_HC COMBINE_TYPES(SLL_RUNTIME_OBJECT_TYPE_HANDLE,SLL_RUNTIME_OBJECT_TYPE_CHAR)
#define COMBINED_TYPE_HS COMBINE_TYPES(SLL_RUNTIME_OBJECT_TYPE_HANDLE,SLL_RUNTIME_OBJECT_TYPE_STRING)
#define COMBINED_TYPE_HA COMBINE_TYPES(SLL_RUNTIME_OBJECT_TYPE_HANDLE,SLL_RUNTIME_OBJECT_TYPE_ARRAY)
#define COMBINED_TYPE_HH COMBINE_TYPES(SLL_RUNTIME_OBJECT_TYPE_HANDLE,SLL_RUNTIME_OBJECT_TYPE_HANDLE)
#define COMBINED_TYPE_HM COMBINE_TYPES(SLL_RUNTIME_OBJECT_TYPE_HANDLE,SLL_RUNTIME_OBJECT_TYPE_MAP)
#define COMBINED_TYPE_MI COMBINE_TYPES(SLL_RUNTIME_OBJECT_TYPE_MAP,SLL_RUNTIME_OBJECT_TYPE_INT)
#define COMBINED_TYPE_MF COMBINE_TYPES(SLL_RUNTIME_OBJECT_TYPE_MAP,SLL_RUNTIME_OBJECT_TYPE_FLOAT)
#define COMBINED_TYPE_MC COMBINE_TYPES(SLL_RUNTIME_OBJECT_TYPE_MAP,SLL_RUNTIME_OBJECT_TYPE_CHAR)
#define COMBINED_TYPE_MS COMBINE_TYPES(SLL_RUNTIME_OBJECT_TYPE_MAP,SLL_RUNTIME_OBJECT_TYPE_STRING)
#define COMBINED_TYPE_MA COMBINE_TYPES(SLL_RUNTIME_OBJECT_TYPE_MAP,SLL_RUNTIME_OBJECT_TYPE_ARRAY)
#define COMBINED_TYPE_MH COMBINE_TYPES(SLL_RUNTIME_OBJECT_TYPE_MAP,SLL_RUNTIME_OBJECT_TYPE_HANDLE)
#define COMBINED_TYPE_MM COMBINE_TYPES(SLL_RUNTIME_OBJECT_TYPE_MAP,SLL_RUNTIME_OBJECT_TYPE_MAP)
#define COMBINE_CAST_TYPES(a,b) (((unsigned int)(a)<<8)|(b))
#define COMBINED_CAST_ARGS COMBINE_TYPES(SLL_RUNTIME_OBJECT_GET_TYPE(a),b->dt.i)
#define COMBINED_CAST_TYPE_II COMBINE_CAST_TYPES(SLL_RUNTIME_OBJECT_TYPE_INT,SLL_CONSTANT_TYPE_INT)
#define COMBINED_CAST_TYPE_IF COMBINE_CAST_TYPES(SLL_RUNTIME_OBJECT_TYPE_INT,SLL_CONSTANT_TYPE_FLOAT)
#define COMBINED_CAST_TYPE_IC COMBINE_CAST_TYPES(SLL_RUNTIME_OBJECT_TYPE_INT,SLL_CONSTANT_TYPE_CHAR)
#define COMBINED_CAST_TYPE_IS COMBINE_CAST_TYPES(SLL_RUNTIME_OBJECT_TYPE_INT,SLL_CONSTANT_TYPE_STRING)
#define COMBINED_CAST_TYPE_IA COMBINE_CAST_TYPES(SLL_RUNTIME_OBJECT_TYPE_INT,SLL_CONSTANT_TYPE_ARRAY)
#define COMBINED_CAST_TYPE_IH COMBINE_CAST_TYPES(SLL_RUNTIME_OBJECT_TYPE_INT,SLL_CONSTANT_TYPE_HANDLE)
#define COMBINED_CAST_TYPE_IM COMBINE_CAST_TYPES(SLL_RUNTIME_OBJECT_TYPE_INT,SLL_CONSTANT_TYPE_MAP)
#define COMBINED_CAST_TYPE_IK COMBINE_CAST_TYPES(SLL_RUNTIME_OBJECT_TYPE_INT,SLL_CONSTANT_TYPE_MAP_KEY)
#define COMBINED_CAST_TYPE_IV COMBINE_CAST_TYPES(SLL_RUNTIME_OBJECT_TYPE_INT,SLL_CONSTANT_TYPE_MAP_VALUE)
#define COMBINED_CAST_TYPE_FI COMBINE_CAST_TYPES(SLL_RUNTIME_OBJECT_TYPE_FLOAT,SLL_CONSTANT_TYPE_INT)
#define COMBINED_CAST_TYPE_FF COMBINE_CAST_TYPES(SLL_RUNTIME_OBJECT_TYPE_FLOAT,SLL_CONSTANT_TYPE_FLOAT)
#define COMBINED_CAST_TYPE_FC COMBINE_CAST_TYPES(SLL_RUNTIME_OBJECT_TYPE_FLOAT,SLL_CONSTANT_TYPE_CHAR)
#define COMBINED_CAST_TYPE_FS COMBINE_CAST_TYPES(SLL_RUNTIME_OBJECT_TYPE_FLOAT,SLL_CONSTANT_TYPE_STRING)
#define COMBINED_CAST_TYPE_FA COMBINE_CAST_TYPES(SLL_RUNTIME_OBJECT_TYPE_FLOAT,SLL_CONSTANT_TYPE_ARRAY)
#define COMBINED_CAST_TYPE_FH COMBINE_CAST_TYPES(SLL_RUNTIME_OBJECT_TYPE_FLOAT,SLL_CONSTANT_TYPE_HANDLE)
#define COMBINED_CAST_TYPE_FM COMBINE_CAST_TYPES(SLL_RUNTIME_OBJECT_TYPE_FLOAT,SLL_CONSTANT_TYPE_MAP)
#define COMBINED_CAST_TYPE_FK COMBINE_CAST_TYPES(SLL_RUNTIME_OBJECT_TYPE_FLOAT,SLL_CONSTANT_TYPE_MAP_KEY)
#define COMBINED_CAST_TYPE_FV COMBINE_CAST_TYPES(SLL_RUNTIME_OBJECT_TYPE_FLOAT,SLL_CONSTANT_TYPE_MAP_VALUE)
#define COMBINED_CAST_TYPE_CI COMBINE_CAST_TYPES(SLL_RUNTIME_OBJECT_TYPE_CHAR,SLL_CONSTANT_TYPE_INT)
#define COMBINED_CAST_TYPE_CF COMBINE_CAST_TYPES(SLL_RUNTIME_OBJECT_TYPE_CHAR,SLL_CONSTANT_TYPE_FLOAT)
#define COMBINED_CAST_TYPE_CC COMBINE_CAST_TYPES(SLL_RUNTIME_OBJECT_TYPE_CHAR,SLL_CONSTANT_TYPE_CHAR)
#define COMBINED_CAST_TYPE_CS COMBINE_CAST_TYPES(SLL_RUNTIME_OBJECT_TYPE_CHAR,SLL_CONSTANT_TYPE_STRING)
#define COMBINED_CAST_TYPE_CA COMBINE_CAST_TYPES(SLL_RUNTIME_OBJECT_TYPE_CHAR,SLL_CONSTANT_TYPE_ARRAY)
#define COMBINED_CAST_TYPE_CH COMBINE_CAST_TYPES(SLL_RUNTIME_OBJECT_TYPE_CHAR,SLL_CONSTANT_TYPE_HANDLE)
#define COMBINED_CAST_TYPE_CM COMBINE_CAST_TYPES(SLL_RUNTIME_OBJECT_TYPE_CHAR,SLL_CONSTANT_TYPE_MAP)
#define COMBINED_CAST_TYPE_CK COMBINE_CAST_TYPES(SLL_RUNTIME_OBJECT_TYPE_CHAR,SLL_CONSTANT_TYPE_MAP_KEY)
#define COMBINED_CAST_TYPE_CV COMBINE_CAST_TYPES(SLL_RUNTIME_OBJECT_TYPE_CHAR,SLL_CONSTANT_TYPE_MAP_VALUE)
#define COMBINED_CAST_TYPE_SI COMBINE_CAST_TYPES(SLL_RUNTIME_OBJECT_TYPE_STRING,SLL_CONSTANT_TYPE_INT)
#define COMBINED_CAST_TYPE_SF COMBINE_CAST_TYPES(SLL_RUNTIME_OBJECT_TYPE_STRING,SLL_CONSTANT_TYPE_FLOAT)
#define COMBINED_CAST_TYPE_SC COMBINE_CAST_TYPES(SLL_RUNTIME_OBJECT_TYPE_STRING,SLL_CONSTANT_TYPE_CHAR)
#define COMBINED_CAST_TYPE_SS COMBINE_CAST_TYPES(SLL_RUNTIME_OBJECT_TYPE_STRING,SLL_CONSTANT_TYPE_STRING)
#define COMBINED_CAST_TYPE_SA COMBINE_CAST_TYPES(SLL_RUNTIME_OBJECT_TYPE_STRING,SLL_CONSTANT_TYPE_ARRAY)
#define COMBINED_CAST_TYPE_SH COMBINE_CAST_TYPES(SLL_RUNTIME_OBJECT_TYPE_STRING,SLL_CONSTANT_TYPE_HANDLE)
#define COMBINED_CAST_TYPE_SM COMBINE_CAST_TYPES(SLL_RUNTIME_OBJECT_TYPE_STRING,SLL_CONSTANT_TYPE_MAP)
#define COMBINED_CAST_TYPE_SK COMBINE_CAST_TYPES(SLL_RUNTIME_OBJECT_TYPE_STRING,SLL_CONSTANT_TYPE_MAP_KEY)
#define COMBINED_CAST_TYPE_SV COMBINE_CAST_TYPES(SLL_RUNTIME_OBJECT_TYPE_STRING,SLL_CONSTANT_TYPE_MAP_VALUE)
#define COMBINED_CAST_TYPE_AI COMBINE_CAST_TYPES(SLL_RUNTIME_OBJECT_TYPE_ARRAY,SLL_CONSTANT_TYPE_INT)
#define COMBINED_CAST_TYPE_AF COMBINE_CAST_TYPES(SLL_RUNTIME_OBJECT_TYPE_ARRAY,SLL_CONSTANT_TYPE_FLOAT)
#define COMBINED_CAST_TYPE_AC COMBINE_CAST_TYPES(SLL_RUNTIME_OBJECT_TYPE_ARRAY,SLL_CONSTANT_TYPE_CHAR)
#define COMBINED_CAST_TYPE_AS COMBINE_CAST_TYPES(SLL_RUNTIME_OBJECT_TYPE_ARRAY,SLL_CONSTANT_TYPE_STRING)
#define COMBINED_CAST_TYPE_AA COMBINE_CAST_TYPES(SLL_RUNTIME_OBJECT_TYPE_ARRAY,SLL_CONSTANT_TYPE_ARRAY)
#define COMBINED_CAST_TYPE_AH COMBINE_CAST_TYPES(SLL_RUNTIME_OBJECT_TYPE_ARRAY,SLL_CONSTANT_TYPE_HANDLE)
#define COMBINED_CAST_TYPE_AM COMBINE_CAST_TYPES(SLL_RUNTIME_OBJECT_TYPE_ARRAY,SLL_CONSTANT_TYPE_MAP)
#define COMBINED_CAST_TYPE_AK COMBINE_CAST_TYPES(SLL_RUNTIME_OBJECT_TYPE_ARRAY,SLL_CONSTANT_TYPE_MAP_KEY)
#define COMBINED_CAST_TYPE_AV COMBINE_CAST_TYPES(SLL_RUNTIME_OBJECT_TYPE_ARRAY,SLL_CONSTANT_TYPE_MAP_VALUE)
#define COMBINED_CAST_TYPE_HI COMBINE_CAST_TYPES(SLL_RUNTIME_OBJECT_TYPE_HANDLE,SLL_CONSTANT_TYPE_INT)
#define COMBINED_CAST_TYPE_HF COMBINE_CAST_TYPES(SLL_RUNTIME_OBJECT_TYPE_HANDLE,SLL_CONSTANT_TYPE_FLOAT)
#define COMBINED_CAST_TYPE_HC COMBINE_CAST_TYPES(SLL_RUNTIME_OBJECT_TYPE_HANDLE,SLL_CONSTANT_TYPE_CHAR)
#define COMBINED_CAST_TYPE_HS COMBINE_CAST_TYPES(SLL_RUNTIME_OBJECT_TYPE_HANDLE,SLL_CONSTANT_TYPE_STRING)
#define COMBINED_CAST_TYPE_HA COMBINE_CAST_TYPES(SLL_RUNTIME_OBJECT_TYPE_HANDLE,SLL_CONSTANT_TYPE_ARRAY)
#define COMBINED_CAST_TYPE_HH COMBINE_CAST_TYPES(SLL_RUNTIME_OBJECT_TYPE_HANDLE,SLL_CONSTANT_TYPE_HANDLE)
#define COMBINED_CAST_TYPE_HM COMBINE_CAST_TYPES(SLL_RUNTIME_OBJECT_TYPE_HANDLE,SLL_CONSTANT_TYPE_MAP)
#define COMBINED_CAST_TYPE_HK COMBINE_CAST_TYPES(SLL_RUNTIME_OBJECT_TYPE_HANDLE,SLL_CONSTANT_TYPE_MAP_KEY)
#define COMBINED_CAST_TYPE_HV COMBINE_CAST_TYPES(SLL_RUNTIME_OBJECT_TYPE_HANDLE,SLL_CONSTANT_TYPE_MAP_VALUE)
#define COMBINED_CAST_TYPE_MI COMBINE_CAST_TYPES(SLL_RUNTIME_OBJECT_TYPE_MAP,SLL_CONSTANT_TYPE_INT)
#define COMBINED_CAST_TYPE_MF COMBINE_CAST_TYPES(SLL_RUNTIME_OBJECT_TYPE_MAP,SLL_CONSTANT_TYPE_FLOAT)
#define COMBINED_CAST_TYPE_MC COMBINE_CAST_TYPES(SLL_RUNTIME_OBJECT_TYPE_MAP,SLL_CONSTANT_TYPE_CHAR)
#define COMBINED_CAST_TYPE_MS COMBINE_CAST_TYPES(SLL_RUNTIME_OBJECT_TYPE_MAP,SLL_CONSTANT_TYPE_STRING)
#define COMBINED_CAST_TYPE_MA COMBINE_CAST_TYPES(SLL_RUNTIME_OBJECT_TYPE_MAP,SLL_CONSTANT_TYPE_ARRAY)
#define COMBINED_CAST_TYPE_MH COMBINE_CAST_TYPES(SLL_RUNTIME_OBJECT_TYPE_MAP,SLL_CONSTANT_TYPE_HANDLE)
#define COMBINED_CAST_TYPE_MM COMBINE_CAST_TYPES(SLL_RUNTIME_OBJECT_TYPE_MAP,SLL_CONSTANT_TYPE_MAP)
#define COMBINED_CAST_TYPE_MK COMBINE_CAST_TYPES(SLL_RUNTIME_OBJECT_TYPE_MAP,SLL_CONSTANT_TYPE_MAP_KEY)
#define COMBINED_CAST_TYPE_MV COMBINE_CAST_TYPES(SLL_RUNTIME_OBJECT_TYPE_MAP,SLL_CONSTANT_TYPE_MAP_VALUE)
#define COMPARE_RESULT(a,b) (a<b?SLL_COMPARE_RESULT_BELOW:(a>b?SLL_COMPARE_RESULT_ABOVE:SLL_COMPARE_RESULT_EQUAL))
#define COMMUTATIVE_OPERATOR \
	sll_bool_t inv=0; \
	do{ \
		if (SLL_RUNTIME_OBJECT_GET_TYPE(a)>SLL_RUNTIME_OBJECT_GET_TYPE(b)){ \
			sll_runtime_object_t* c=a; \
			a=b; \
			b=c; \
			inv=1; \
		} \
	} while (0)



const sll_float_t sll_float_compare_error=1e-6;



__SLL_OPERATOR_UNARY(inc){
	switch (SLL_RUNTIME_OBJECT_GET_TYPE(a)){
		case SLL_RUNTIME_OBJECT_TYPE_INT:
			return SLL_FROM_INT(a->dt.i+1);
		case SLL_RUNTIME_OBJECT_TYPE_FLOAT:
			return SLL_FROM_FLOAT(a->dt.f+1);
		case SLL_RUNTIME_OBJECT_TYPE_CHAR:
			return SLL_FROM_CHAR(a->dt.c+1);
		case SLL_RUNTIME_OBJECT_TYPE_STRING:
			{
				sll_runtime_object_t* o=SLL_CREATE();
				o->t=SLL_RUNTIME_OBJECT_TYPE_STRING;
				sll_string_create(a->dt.s.l+1,&(o->dt.s));
				o->dt.s.c=SLL_STRING_COMBINE_CHECKSUMS_FAST(a->dt.s.c,a->dt.s.l,'1');
				memcpy(o->dt.s.v,a->dt.s.v,a->dt.s.l);
				o->dt.s.v[a->dt.s.l]='1';
				return o;
			}
		case SLL_RUNTIME_OBJECT_TYPE_ARRAY:
			{
				sll_runtime_object_t* o=SLL_CREATE();
				o->t=SLL_RUNTIME_OBJECT_TYPE_ARRAY;
				sll_array_push(&(a->dt.a),sll_static_int[1],&(o->dt.a));
				return o;
			}
		case SLL_RUNTIME_OBJECT_TYPE_HANDLE:
			return SLL_FROM_HANDLE(a->dt.h.t,a->dt.h.h+1);
		case SLL_RUNTIME_OBJECT_TYPE_MAP:
			{
				sll_runtime_object_t* o=SLL_CREATE();
				o->t=SLL_RUNTIME_OBJECT_TYPE_MAP;
				sll_map_add(&(a->dt.m),sll_static_int[1],sll_static_int[0],&(o->dt.m));
				return o;
			}
		default:
			SLL_UNREACHABLE();
	}
	return SLL_ACQUIRE_STATIC_INT(0);
}



__SLL_OPERATOR_UNARY(dec){
	switch (SLL_RUNTIME_OBJECT_GET_TYPE(a)){
		case SLL_RUNTIME_OBJECT_TYPE_INT:
			return SLL_FROM_INT(a->dt.i-1);
		case SLL_RUNTIME_OBJECT_TYPE_FLOAT:
			return SLL_FROM_FLOAT(a->dt.f-1);
		case SLL_RUNTIME_OBJECT_TYPE_CHAR:
			return SLL_FROM_CHAR(a->dt.c-1);
		case SLL_RUNTIME_OBJECT_TYPE_STRING:
			SLL_UNIMPLEMENTED();
		case SLL_RUNTIME_OBJECT_TYPE_ARRAY:
			{
				sll_runtime_object_t* o=SLL_CREATE();
				o->t=SLL_RUNTIME_OBJECT_TYPE_ARRAY;
				sll_array_remove(&(a->dt.a),sll_static_int[1],&(o->dt.a));
				return o;
			}
		case SLL_RUNTIME_OBJECT_TYPE_HANDLE:
			return SLL_FROM_HANDLE(a->dt.h.t,a->dt.h.h-1);
		case SLL_RUNTIME_OBJECT_TYPE_MAP:
			{
				sll_runtime_object_t* o=SLL_CREATE();
				o->t=SLL_RUNTIME_OBJECT_TYPE_MAP;
				SLL_RELEASE(sll_map_remove(&(a->dt.m),sll_static_int[1],&(o->dt.m)));
				return o;
			}
		default:
			SLL_UNREACHABLE();
	}
	return SLL_ACQUIRE_STATIC_INT(0);
}



__SLL_OPERATOR_BINARY(add){
	COMMUTATIVE_OPERATOR;
	IGNORE_RESULT(inv);
	switch (COMBINED_ARGS){
		case COMBINED_TYPE_II:
			return SLL_FROM_INT(a->dt.i+b->dt.i);
		case COMBINED_TYPE_IF:
			return SLL_FROM_FLOAT(a->dt.i+b->dt.f);
		case COMBINED_TYPE_IC:
			return SLL_FROM_INT(a->dt.i+b->dt.c);
		case COMBINED_TYPE_IH:
			return SLL_FROM_HANDLE(b->dt.h.t,a->dt.i+b->dt.h.h);
		case COMBINED_TYPE_FF:
			return SLL_FROM_FLOAT(a->dt.f+b->dt.f);
		case COMBINED_TYPE_FC:
			return SLL_FROM_FLOAT(a->dt.f+b->dt.c);
		case COMBINED_TYPE_FH:
			return SLL_FROM_FLOAT(a->dt.f+b->dt.h.h);
		case COMBINED_TYPE_CC:
			return SLL_FROM_CHAR(a->dt.c+b->dt.c);
		case COMBINED_TYPE_CH:
			return SLL_FROM_HANDLE(b->dt.h.t,a->dt.c+b->dt.h.h);
		case COMBINED_TYPE_IS:
		case COMBINED_TYPE_FS:
		case COMBINED_TYPE_CS:
			{
				sll_runtime_object_t* tmp=a;
				a=b;
				b=tmp;
			}
		case COMBINED_TYPE_SH:
_add_to_string:
			{
				SLL_ASSERT(SLL_RUNTIME_OBJECT_GET_TYPE(a)==SLL_RUNTIME_OBJECT_TYPE_STRING);
				sll_string_t sa=a->dt.s;
				sll_string_t sb;
				sll_object_to_string((const sll_runtime_object_t*const*)&b,1,&sb);
				sll_runtime_object_t* o=SLL_CREATE();
				o->t=SLL_RUNTIME_OBJECT_TYPE_STRING;
				if (!sa.l){
					o->dt.s=sb;
					return o;
				}
				sll_string_join(&sa,&sb,&(o->dt.s));
				free(sb.v);
				return o;
			}
		case COMBINED_TYPE_SS:
			{
				sll_string_t sa=a->dt.s;
				sll_string_t sb=b->dt.s;
				if (!sa.l&&!sb.l){
					return SLL_ACQUIRE_STATIC(str_zero);
				}
				sll_runtime_object_t* o=SLL_CREATE();
				o->t=SLL_RUNTIME_OBJECT_TYPE_STRING;
				sll_string_join(&sa,&sb,&(o->dt.s));
				return o;
			}
		case COMBINED_TYPE_HH:
			if (a->dt.h.t==b->dt.h.t){
				return SLL_FROM_HANDLE(a->dt.h.t,a->dt.h.h+b->dt.h.h);
			}
			return SLL_FROM_INT(a->dt.h.h+b->dt.h.h);
		case COMBINED_TYPE_AH:
			{
				sll_runtime_object_t* tmp=a;
				a=b;
				b=tmp;
			}
		case COMBINED_TYPE_IA:
		case COMBINED_TYPE_FA:
		case COMBINED_TYPE_CA:
_add_to_array:
			{
				sll_runtime_object_t* o=SLL_CREATE();
				o->t=SLL_RUNTIME_OBJECT_TYPE_ARRAY;
				sll_array_push(&(b->dt.a),a,&(o->dt.a));
				return o;
			}
		case COMBINED_TYPE_AA:
			{
				sll_runtime_object_t* o=SLL_CREATE();
				o->t=SLL_RUNTIME_OBJECT_TYPE_ARRAY;
				sll_array_join(&(a->dt.a),&(b->dt.a),&(o->dt.a));
				return o;
			}
		case COMBINED_TYPE_IM:
		case COMBINED_TYPE_FM:
		case COMBINED_TYPE_CM:
		case COMBINED_TYPE_HM:
_add_to_map:
			{
				sll_runtime_object_t* o=SLL_CREATE();
				o->t=SLL_RUNTIME_OBJECT_TYPE_MAP;
				sll_map_add(&(b->dt.m),a,sll_static_int[0],&(o->dt.m));
				return o;
			}
		case COMBINED_TYPE_MM:
			{
				sll_runtime_object_t* o=SLL_CREATE();
				o->t=SLL_RUNTIME_OBJECT_TYPE_MAP;
				sll_map_join(&(a->dt.m),&(b->dt.m),&(o->dt.m));
				return o;
			}
		case COMBINED_TYPE_SA:
			if (inv){
				goto _add_to_array;
			}
			goto _add_to_string;
		case COMBINED_TYPE_SM:
			if (inv){
				goto _add_to_map;
			}
			goto _add_to_string;
		case COMBINED_TYPE_AM:
			{
				if (inv){
					goto _add_to_map;
				}
				sll_runtime_object_t* tmp=a;
				a=b;
				b=tmp;
				goto _add_to_array;
			}
		default:
			SLL_UNREACHABLE();
	}
	return SLL_ACQUIRE_STATIC_INT(0);
}



__SLL_OPERATOR_BINARY(sub){
	if (SLL_RUNTIME_OBJECT_GET_TYPE(a)==SLL_RUNTIME_OBJECT_TYPE_ARRAY){
		sll_runtime_object_t* o=SLL_CREATE();
		o->t=SLL_RUNTIME_OBJECT_TYPE_ARRAY;
		if (SLL_RUNTIME_OBJECT_GET_TYPE(b)==SLL_RUNTIME_OBJECT_TYPE_ARRAY){
			sll_array_remove_multiple(&(a->dt.a),b->dt.a.v,b->dt.a.l,&(o->dt.a));
		}
		else{
			sll_array_remove(&(a->dt.a),b,&(o->dt.a));
		}
		return o;
	}
	if (SLL_RUNTIME_OBJECT_GET_TYPE(a)==SLL_RUNTIME_OBJECT_TYPE_MAP){
		sll_runtime_object_t* o=SLL_CREATE();
		o->t=SLL_RUNTIME_OBJECT_TYPE_MAP;
		SLL_RELEASE(sll_map_remove(&(a->dt.m),b,&(o->dt.m)));
		return o;
	}
	switch (COMBINED_ARGS){
		case COMBINED_TYPE_II:
			return SLL_FROM_INT(a->dt.i-b->dt.i);
		case COMBINED_TYPE_IF:
			return SLL_FROM_FLOAT(a->dt.i-b->dt.f);
		case COMBINED_TYPE_IC:
			return SLL_FROM_INT(a->dt.i-b->dt.c);
		case COMBINED_TYPE_FI:
			return SLL_FROM_FLOAT(a->dt.f-b->dt.i);
		case COMBINED_TYPE_FF:
			return SLL_FROM_FLOAT(a->dt.f-b->dt.f);
		case COMBINED_TYPE_FC:
			return SLL_FROM_FLOAT(a->dt.f-b->dt.c);
		case COMBINED_TYPE_CI:
			return SLL_FROM_INT(a->dt.c-b->dt.i);
		case COMBINED_TYPE_CF:
			return SLL_FROM_FLOAT(a->dt.c-b->dt.f);
		case COMBINED_TYPE_CC:
			return SLL_FROM_CHAR(a->dt.c-b->dt.c);
		case COMBINED_TYPE_SS:
			{
				sll_runtime_object_t* o=SLL_CREATE();
				o->t=SLL_RUNTIME_OBJECT_TYPE_STRING;
				sll_string_remove(&(a->dt.s),&(b->dt.s),&(o->dt.s));
				return o;
			}
		case COMBINED_TYPE_HI:
			return SLL_FROM_HANDLE(a->dt.h.t,a->dt.h.h-b->dt.i);
		case COMBINED_TYPE_HF:
			return SLL_FROM_HANDLE(a->dt.h.t,a->dt.h.h-SLL_ROUND_FLOAT(b->dt.f));
		case COMBINED_TYPE_HC:
			return SLL_FROM_HANDLE(a->dt.h.t,a->dt.h.h-b->dt.c);
		case COMBINED_TYPE_HH:
			return (a->dt.h.t==b->dt.h.t?SLL_FROM_HANDLE(a->dt.h.t,a->dt.h.h-b->dt.h.h):SLL_FROM_INT(a->dt.h.h-b->dt.h.h));
		default:
			SLL_UNIMPLEMENTED();
	}
	return SLL_ACQUIRE_STATIC_INT(0);
}



__SLL_OPERATOR_BINARY(mult){
	COMMUTATIVE_OPERATOR;
	IGNORE_RESULT(inv);
	switch (COMBINED_ARGS){
		case COMBINED_TYPE_II:
			return SLL_FROM_INT(a->dt.i*b->dt.i);
		case COMBINED_TYPE_IF:
			return SLL_FROM_FLOAT(a->dt.i*b->dt.f);
		case COMBINED_TYPE_FF:
			return SLL_FROM_FLOAT(a->dt.f*b->dt.f);
		default:
			SLL_UNIMPLEMENTED();
	}
	return SLL_ACQUIRE_STATIC_INT(0);
}



__SLL_OPERATOR_BINARY(div){
	if (SLL_RUNTIME_OBJECT_GET_TYPE(a)==SLL_RUNTIME_OBJECT_TYPE_ARRAY){
		return SLL_FROM_INT((SLL_RUNTIME_OBJECT_GET_TYPE(b)==SLL_RUNTIME_OBJECT_TYPE_ARRAY?sll_array_count_multiple(&(a->dt.a),b->dt.a.v,b->dt.a.l):sll_array_count(&(a->dt.a),b)));
	}
	switch (COMBINED_ARGS){
		case COMBINED_TYPE_II:
			return SLL_FROM_FLOAT(((sll_float_t)a->dt.i)/b->dt.i);
		case COMBINED_TYPE_IF:
			return SLL_FROM_FLOAT(a->dt.i/b->dt.f);
		case COMBINED_TYPE_FI:
			return SLL_FROM_FLOAT(a->dt.f/b->dt.i);
		case COMBINED_TYPE_FF:
			return SLL_FROM_FLOAT(a->dt.f/b->dt.f);
		case COMBINED_TYPE_SS:
			return SLL_FROM_INT(sll_string_count(&(a->dt.s),&(b->dt.s)));
		default:
			SLL_UNIMPLEMENTED();
	}
	return SLL_ACQUIRE_STATIC_INT(0);
}



__SLL_OPERATOR_BINARY(floor_div){
	if (SLL_RUNTIME_OBJECT_GET_TYPE(a)==SLL_RUNTIME_OBJECT_TYPE_ARRAY){
		return SLL_FROM_INT((SLL_RUNTIME_OBJECT_GET_TYPE(b)==SLL_RUNTIME_OBJECT_TYPE_ARRAY?sll_array_count_multiple(&(a->dt.a),b->dt.a.v,b->dt.a.l):sll_array_count(&(a->dt.a),b)));
	}
	switch (COMBINED_ARGS){
		case COMBINED_TYPE_II:
			return SLL_FROM_INT(a->dt.i/b->dt.i);
		case COMBINED_TYPE_IF:
			return SLL_FROM_INT(SLL_ROUND_FLOAT(a->dt.i/b->dt.f));
		case COMBINED_TYPE_FI:
			return SLL_FROM_INT(SLL_ROUND_FLOAT(a->dt.f/b->dt.i));
		case COMBINED_TYPE_FF:
			return SLL_FROM_INT(SLL_ROUND_FLOAT(a->dt.f/b->dt.f));
		case COMBINED_TYPE_SS:
			return SLL_FROM_INT(sll_string_count(&(a->dt.s),&(b->dt.s)));
		default:
			SLL_UNIMPLEMENTED();
	}
	return SLL_ACQUIRE_STATIC_INT(0);
}



__SLL_OPERATOR_BINARY(mod){
	switch (COMBINED_ARGS){
		case COMBINED_TYPE_II:
			return SLL_FROM_INT(a->dt.i%b->dt.i);
		case COMBINED_TYPE_IF:
			return SLL_FROM_INT(a->dt.i%SLL_ROUND_FLOAT(b->dt.f));
		case COMBINED_TYPE_FI:
			return SLL_FROM_INT(SLL_ROUND_FLOAT(a->dt.f)%b->dt.i);
		case COMBINED_TYPE_FF:
			return SLL_FROM_INT(SLL_ROUND_FLOAT(a->dt.f)%SLL_ROUND_FLOAT(b->dt.f));
		default:
			SLL_UNIMPLEMENTED();
	}
	return SLL_ACQUIRE_STATIC_INT(0);
}



__SLL_OPERATOR_BINARY(and){
	COMMUTATIVE_OPERATOR;
	IGNORE_RESULT(inv);
	switch (COMBINED_ARGS){
		case COMBINED_TYPE_II:
			return SLL_FROM_INT(a->dt.i&b->dt.i);
		case COMBINED_TYPE_IF:
			return SLL_FROM_INT(a->dt.i&SLL_ROUND_FLOAT(b->dt.f));
		case COMBINED_TYPE_FF:
			return SLL_FROM_INT(SLL_ROUND_FLOAT(a->dt.f)&SLL_ROUND_FLOAT(b->dt.f));
		default:
			SLL_UNIMPLEMENTED();
	}
	return SLL_ACQUIRE_STATIC_INT(0);
}



__SLL_OPERATOR_BINARY(or){
	COMMUTATIVE_OPERATOR;
	IGNORE_RESULT(inv);
	switch (COMBINED_ARGS){
		case COMBINED_TYPE_II:
			return SLL_FROM_INT(a->dt.i|b->dt.i);
		case COMBINED_TYPE_IF:
			return SLL_FROM_INT(a->dt.i|SLL_ROUND_FLOAT(b->dt.f));
		case COMBINED_TYPE_FF:
			return SLL_FROM_INT(SLL_ROUND_FLOAT(a->dt.f)|SLL_ROUND_FLOAT(b->dt.f));
		default:
			SLL_UNIMPLEMENTED();
	}
	return SLL_ACQUIRE_STATIC_INT(0);
}



__SLL_OPERATOR_BINARY(xor){
	COMMUTATIVE_OPERATOR;
	IGNORE_RESULT(inv);
	switch (COMBINED_ARGS){
		case COMBINED_TYPE_II:
			return SLL_FROM_INT(a->dt.i^b->dt.i);
		case COMBINED_TYPE_IF:
			return SLL_FROM_INT(a->dt.i^SLL_ROUND_FLOAT(b->dt.f));
		case COMBINED_TYPE_FF:
			return SLL_FROM_INT(SLL_ROUND_FLOAT(a->dt.f)^SLL_ROUND_FLOAT(b->dt.f));
		default:
			SLL_UNIMPLEMENTED();
	}
	return SLL_ACQUIRE_STATIC_INT(0);
}



__SLL_OPERATOR_UNARY(inv){
	switch (SLL_RUNTIME_OBJECT_GET_TYPE(a)){
		case SLL_RUNTIME_OBJECT_TYPE_INT:
			return SLL_FROM_INT(~a->dt.i);
		case SLL_RUNTIME_OBJECT_TYPE_FLOAT:
			return SLL_FROM_INT(~SLL_ROUND_FLOAT(a->dt.f));
		case SLL_RUNTIME_OBJECT_TYPE_CHAR:
			return SLL_FROM_CHAR(~a->dt.c);
		case SLL_RUNTIME_OBJECT_TYPE_STRING:
			SLL_UNIMPLEMENTED();
		case SLL_RUNTIME_OBJECT_TYPE_ARRAY:
			SLL_UNIMPLEMENTED();
		case SLL_RUNTIME_OBJECT_TYPE_HANDLE:
			SLL_UNIMPLEMENTED();
		case SLL_RUNTIME_OBJECT_TYPE_MAP:
			SLL_UNIMPLEMENTED();
		default:
			SLL_UNREACHABLE();
	}
	return SLL_ACQUIRE_STATIC_INT(0);
}



__SLL_OPERATOR_BINARY(shr){
	switch (COMBINED_ARGS){
		case COMBINED_TYPE_II:
			return SLL_FROM_INT(a->dt.i>>b->dt.i);
		case COMBINED_TYPE_IH:
			return SLL_FROM_HANDLE(b->dt.h.t,a->dt.i>>b->dt.h.h);
		case COMBINED_TYPE_AI:
			{
				sll_runtime_object_t* o=SLL_CREATE();
				o->t=SLL_RUNTIME_OBJECT_TYPE_ARRAY;
				sll_array_resize(&(a->dt.a),-b->dt.i,&(o->dt.a));
				return o;
			}
		case COMBINED_TYPE_HI:
			return SLL_FROM_HANDLE(a->dt.h.t,a->dt.h.h>>a->dt.i);
		default:
			SLL_UNIMPLEMENTED();
	}
	return SLL_ACQUIRE_STATIC_INT(0);
}



__SLL_OPERATOR_BINARY(shl){
	switch (COMBINED_ARGS){
		case COMBINED_TYPE_II:
			return SLL_FROM_INT(a->dt.i<<b->dt.i);
		case COMBINED_TYPE_IH:
			return SLL_FROM_HANDLE(b->dt.h.t,a->dt.i<<b->dt.h.h);
		case COMBINED_TYPE_AI:
			{
				sll_runtime_object_t* o=SLL_CREATE();
				o->t=SLL_RUNTIME_OBJECT_TYPE_ARRAY;
				sll_array_resize(&(a->dt.a),b->dt.i,&(o->dt.a));
				return o;
			}
		case COMBINED_TYPE_HI:
			return SLL_FROM_HANDLE(a->dt.h.t,a->dt.h.h<<a->dt.i);
		default:
			SLL_UNIMPLEMENTED();
	}
	return SLL_ACQUIRE_STATIC_INT(0);
}



__SLL_OPERATOR_UNARY(len){
	switch (SLL_RUNTIME_OBJECT_GET_TYPE(a)){
		case SLL_RUNTIME_OBJECT_TYPE_INT:
		case SLL_RUNTIME_OBJECT_TYPE_FLOAT:
		case SLL_RUNTIME_OBJECT_TYPE_CHAR:
		case SLL_RUNTIME_OBJECT_TYPE_HANDLE:
			return SLL_ACQUIRE_STATIC_INT(0);
		case SLL_RUNTIME_OBJECT_TYPE_STRING:
			return SLL_FROM_INT(a->dt.s.l);
		case SLL_RUNTIME_OBJECT_TYPE_ARRAY:
			return SLL_FROM_INT(a->dt.a.l);
		case SLL_RUNTIME_OBJECT_TYPE_MAP:
			return SLL_FROM_INT(a->dt.m.l);
		default:
			SLL_UNREACHABLE();
	}
	return SLL_ACQUIRE_STATIC_INT(0);
}



__SLL_OPERATOR_UNARY(access_zero){
	SLL_UNIMPLEMENTED();
	return SLL_ACQUIRE_STATIC_INT(0);
}



__SLL_OPERATOR_BINARY(access){
	if (SLL_RUNTIME_OBJECT_GET_TYPE(a)==SLL_RUNTIME_OBJECT_TYPE_STRING){
		if (SLL_RUNTIME_OBJECT_GET_TYPE(b)==SLL_RUNTIME_OBJECT_TYPE_INT){
			if (b->dt.i>=0&&b->dt.i<a->dt.s.l){
				sll_runtime_object_t* o=SLL_CREATE();
				o->t=SLL_RUNTIME_OBJECT_TYPE_CHAR;
				o->dt.c=a->dt.s.v[b->dt.i];
				return o;
			}
		}
	}
	else if (SLL_RUNTIME_OBJECT_GET_TYPE(a)==SLL_RUNTIME_OBJECT_TYPE_ARRAY){
		if (SLL_RUNTIME_OBJECT_GET_TYPE(b)==SLL_RUNTIME_OBJECT_TYPE_INT){
			if (b->dt.i>=0&&b->dt.i<a->dt.a.l){
				sll_runtime_object_t* o=a->dt.a.v[b->dt.i];
				SLL_ACQUIRE(o);
				return o;
			}
		}
	}
	else if (SLL_RUNTIME_OBJECT_GET_TYPE(a)==SLL_RUNTIME_OBJECT_TYPE_MAP){
		sll_map_t m=a->dt.m;
		for (sll_map_length_t i=0;i<m.l;i++){
			if (sll_operator_equal(m.v[i<<1],b)){
				SLL_ACQUIRE(m.v[(i<<1)+1]);
				return m.v[(i<<1)+1];
			}
		}
		return SLL_ACQUIRE_STATIC_INT(0);
	}
	SLL_ACQUIRE(a);
	return a;
}



__SLL_OPERATOR_TERNARY(access_range){
	SLL_UNIMPLEMENTED();
	return SLL_ACQUIRE_STATIC_INT(0);
}



__SLL_OPERATOR_QUATERNARY(access_range_step){
	SLL_UNIMPLEMENTED();
	return SLL_ACQUIRE_STATIC_INT(0);
}



__SLL_OPERATOR_BINARY(cast){
	if (SLL_RUNTIME_OBJECT_GET_TYPE(b)==SLL_RUNTIME_OBJECT_TYPE_INT&&b->dt.i>0&&b->dt.i<=SLL_MAX_CONSTANT_TYPE){
		switch (COMBINED_CAST_ARGS){
			case COMBINED_CAST_TYPE_FI:
				return SLL_FROM_INT((sll_integer_t)(a->dt.f));
			case COMBINED_CAST_TYPE_MK:
				{
					sll_runtime_object_t* o=SLL_CREATE();
					o->t=SLL_RUNTIME_OBJECT_TYPE_ARRAY;
					sll_map_keys(&(a->dt.m),&(o->dt.a));
					return o;
				}
			case COMBINED_CAST_TYPE_MV:
				{
					sll_runtime_object_t* o=SLL_CREATE();
					o->t=SLL_RUNTIME_OBJECT_TYPE_ARRAY;
					sll_map_values(&(a->dt.m),&(o->dt.a));
					return o;
				}
		}
	}
	SLL_ACQUIRE(a);
	return a;
}



__SLL_FUNC __SLL_CHECK_OUTPUT sll_bool_t sll_operator_bool(const sll_runtime_object_t* a){
	switch (SLL_RUNTIME_OBJECT_GET_TYPE(a)){
		case SLL_RUNTIME_OBJECT_TYPE_INT:
			return !!a->dt.i;
		case SLL_RUNTIME_OBJECT_TYPE_FLOAT:
			return !!a->dt.f;
		case SLL_RUNTIME_OBJECT_TYPE_CHAR:
			return !!a->dt.c;
		case SLL_RUNTIME_OBJECT_TYPE_STRING:
			return !!a->dt.s.l;
		case SLL_RUNTIME_OBJECT_TYPE_ARRAY:
			return !!a->dt.a.l;
		case SLL_RUNTIME_OBJECT_TYPE_HANDLE:
			return !!a->dt.h.t;
		case SLL_RUNTIME_OBJECT_TYPE_MAP:
			return !!a->dt.m.l;
		default:
			SLL_UNREACHABLE();
	}
	return 0;
}



__SLL_FUNC __SLL_CHECK_OUTPUT sll_compare_result_t sll_operator_compare(const sll_runtime_object_t* a,const sll_runtime_object_t* b){
	switch (COMBINED_ARGS){
		case COMBINED_TYPE_II:
			return COMPARE_RESULT(a->dt.i,b->dt.i);
		case COMBINED_TYPE_IF:
			return COMPARE_RESULT(a->dt.i,b->dt.f);
		case COMBINED_TYPE_FI:
			return COMPARE_RESULT(a->dt.f,b->dt.i);
		case COMBINED_TYPE_FF:
			return COMPARE_RESULT(a->dt.f,b->dt.f);
		default:
			SLL_UNIMPLEMENTED();
	}
	return 0;
}



__SLL_FUNC __SLL_CHECK_OUTPUT sll_bool_t sll_operator_equal(const sll_runtime_object_t* a,const sll_runtime_object_t* b){
	switch (COMBINED_ARGS){
		case COMBINED_TYPE_II:
			return a->dt.i==b->dt.i;
		case COMBINED_TYPE_IF:
			return fabs(a->dt.i-b->dt.f)<sll_float_compare_error;
		case COMBINED_TYPE_FI:
			return fabs(a->dt.f-b->dt.i)<sll_float_compare_error;
		case COMBINED_TYPE_FF:
			return fabs(a->dt.f-b->dt.f)<sll_float_compare_error;
		case COMBINED_TYPE_SS:
			{
				sll_string_t as=a->dt.s;
				sll_string_t bs=b->dt.s;
				if (as.l!=bs.l||as.c!=bs.c){
					return 0;
				}
				return !memcmp(as.v,bs.v,as.l*sizeof(sll_char_t));
			}
		default:
			SLL_UNIMPLEMENTED();
	}
	return 0;
}
