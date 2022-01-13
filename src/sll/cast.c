#include <sll/_sll_internal.h>
#include <sll/api/math.h>
#include <sll/api/string.h>
#include <sll/array.h>
#include <sll/common.h>
#include <sll/gc.h>
#include <sll/map.h>
#include <sll/object.h>
#include <sll/static_object.h>
#include <sll/string.h>
#include <sll/types.h>
#include <sll/vm.h>



#define COMBINE_TYPES(a,b) (((unsigned int)(a)<<8)|(b))
#define COMBINED_TYPE_II COMBINE_TYPES(SLL_OBJECT_TYPE_INT,SLL_OBJECT_TYPE_INT)
#define COMBINED_TYPE_IF COMBINE_TYPES(SLL_OBJECT_TYPE_INT,SLL_OBJECT_TYPE_FLOAT)
#define COMBINED_TYPE_IC COMBINE_TYPES(SLL_OBJECT_TYPE_INT,SLL_OBJECT_TYPE_CHAR)
#define COMBINED_TYPE_IS COMBINE_TYPES(SLL_OBJECT_TYPE_INT,SLL_OBJECT_TYPE_STRING)
#define COMBINED_TYPE_IA COMBINE_TYPES(SLL_OBJECT_TYPE_INT,SLL_OBJECT_TYPE_ARRAY)
#define COMBINED_TYPE_IM COMBINE_TYPES(SLL_OBJECT_TYPE_INT,SLL_OBJECT_TYPE_MAP)
#define COMBINED_TYPE_IK COMBINE_TYPES(SLL_OBJECT_TYPE_INT,SLL_OBJECT_TYPE_MAP_KEYS)
#define COMBINED_TYPE_IV COMBINE_TYPES(SLL_OBJECT_TYPE_INT,SLL_OBJECT_TYPE_MAP_VALUES)
#define COMBINED_TYPE_FI COMBINE_TYPES(SLL_OBJECT_TYPE_FLOAT,SLL_OBJECT_TYPE_INT)
#define COMBINED_TYPE_FF COMBINE_TYPES(SLL_OBJECT_TYPE_FLOAT,SLL_OBJECT_TYPE_FLOAT)
#define COMBINED_TYPE_FC COMBINE_TYPES(SLL_OBJECT_TYPE_FLOAT,SLL_OBJECT_TYPE_CHAR)
#define COMBINED_TYPE_FS COMBINE_TYPES(SLL_OBJECT_TYPE_FLOAT,SLL_OBJECT_TYPE_STRING)
#define COMBINED_TYPE_FA COMBINE_TYPES(SLL_OBJECT_TYPE_FLOAT,SLL_OBJECT_TYPE_ARRAY)
#define COMBINED_TYPE_FM COMBINE_TYPES(SLL_OBJECT_TYPE_FLOAT,SLL_OBJECT_TYPE_MAP)
#define COMBINED_TYPE_FK COMBINE_TYPES(SLL_OBJECT_TYPE_FLOAT,SLL_OBJECT_TYPE_MAP_KEYS)
#define COMBINED_TYPE_FV COMBINE_TYPES(SLL_OBJECT_TYPE_FLOAT,SLL_OBJECT_TYPE_MAP_VALUES)
#define COMBINED_TYPE_CI COMBINE_TYPES(SLL_OBJECT_TYPE_CHAR,SLL_OBJECT_TYPE_INT)
#define COMBINED_TYPE_CF COMBINE_TYPES(SLL_OBJECT_TYPE_CHAR,SLL_OBJECT_TYPE_FLOAT)
#define COMBINED_TYPE_CC COMBINE_TYPES(SLL_OBJECT_TYPE_CHAR,SLL_OBJECT_TYPE_CHAR)
#define COMBINED_TYPE_CS COMBINE_TYPES(SLL_OBJECT_TYPE_CHAR,SLL_OBJECT_TYPE_STRING)
#define COMBINED_TYPE_CA COMBINE_TYPES(SLL_OBJECT_TYPE_CHAR,SLL_OBJECT_TYPE_ARRAY)
#define COMBINED_TYPE_CM COMBINE_TYPES(SLL_OBJECT_TYPE_CHAR,SLL_OBJECT_TYPE_MAP)
#define COMBINED_TYPE_CK COMBINE_TYPES(SLL_OBJECT_TYPE_CHAR,SLL_OBJECT_TYPE_MAP_KEYS)
#define COMBINED_TYPE_CV COMBINE_TYPES(SLL_OBJECT_TYPE_CHAR,SLL_OBJECT_TYPE_MAP_VALUES)
#define COMBINED_TYPE_SI COMBINE_TYPES(SLL_OBJECT_TYPE_STRING,SLL_OBJECT_TYPE_INT)
#define COMBINED_TYPE_SF COMBINE_TYPES(SLL_OBJECT_TYPE_STRING,SLL_OBJECT_TYPE_FLOAT)
#define COMBINED_TYPE_SC COMBINE_TYPES(SLL_OBJECT_TYPE_STRING,SLL_OBJECT_TYPE_CHAR)
#define COMBINED_TYPE_SS COMBINE_TYPES(SLL_OBJECT_TYPE_STRING,SLL_OBJECT_TYPE_STRING)
#define COMBINED_TYPE_SA COMBINE_TYPES(SLL_OBJECT_TYPE_STRING,SLL_OBJECT_TYPE_ARRAY)
#define COMBINED_TYPE_SM COMBINE_TYPES(SLL_OBJECT_TYPE_STRING,SLL_OBJECT_TYPE_MAP)
#define COMBINED_TYPE_SK COMBINE_TYPES(SLL_OBJECT_TYPE_STRING,SLL_OBJECT_TYPE_MAP_KEYS)
#define COMBINED_TYPE_SV COMBINE_TYPES(SLL_OBJECT_TYPE_STRING,SLL_OBJECT_TYPE_MAP_VALUES)
#define COMBINED_TYPE_AI COMBINE_TYPES(SLL_OBJECT_TYPE_ARRAY,SLL_OBJECT_TYPE_INT)
#define COMBINED_TYPE_AF COMBINE_TYPES(SLL_OBJECT_TYPE_ARRAY,SLL_OBJECT_TYPE_FLOAT)
#define COMBINED_TYPE_AC COMBINE_TYPES(SLL_OBJECT_TYPE_ARRAY,SLL_OBJECT_TYPE_CHAR)
#define COMBINED_TYPE_AS COMBINE_TYPES(SLL_OBJECT_TYPE_ARRAY,SLL_OBJECT_TYPE_STRING)
#define COMBINED_TYPE_AA COMBINE_TYPES(SLL_OBJECT_TYPE_ARRAY,SLL_OBJECT_TYPE_ARRAY)
#define COMBINED_TYPE_AM COMBINE_TYPES(SLL_OBJECT_TYPE_ARRAY,SLL_OBJECT_TYPE_MAP)
#define COMBINED_TYPE_AK COMBINE_TYPES(SLL_OBJECT_TYPE_ARRAY,SLL_OBJECT_TYPE_MAP_KEYS)
#define COMBINED_TYPE_AV COMBINE_TYPES(SLL_OBJECT_TYPE_ARRAY,SLL_OBJECT_TYPE_MAP_VALUES)
#define COMBINED_TYPE_MI COMBINE_TYPES(SLL_OBJECT_TYPE_MAP,SLL_OBJECT_TYPE_INT)
#define COMBINED_TYPE_MF COMBINE_TYPES(SLL_OBJECT_TYPE_MAP,SLL_OBJECT_TYPE_FLOAT)
#define COMBINED_TYPE_MC COMBINE_TYPES(SLL_OBJECT_TYPE_MAP,SLL_OBJECT_TYPE_CHAR)
#define COMBINED_TYPE_MS COMBINE_TYPES(SLL_OBJECT_TYPE_MAP,SLL_OBJECT_TYPE_STRING)
#define COMBINED_TYPE_MA COMBINE_TYPES(SLL_OBJECT_TYPE_MAP,SLL_OBJECT_TYPE_ARRAY)
#define COMBINED_TYPE_MM COMBINE_TYPES(SLL_OBJECT_TYPE_MAP,SLL_OBJECT_TYPE_MAP)
#define COMBINED_TYPE_MK COMBINE_TYPES(SLL_OBJECT_TYPE_MAP,SLL_OBJECT_TYPE_MAP_KEYS)
#define COMBINED_TYPE_MV COMBINE_TYPES(SLL_OBJECT_TYPE_MAP,SLL_OBJECT_TYPE_MAP_VALUES)




__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_operator_cast(sll_object_t* a,sll_object_t* b){
	if (SLL_OBJECT_GET_TYPE(b)!=SLL_OBJECT_TYPE_INT||b->dt.i<0||b->dt.i>SLL_MAX_OBJECT_TYPE||b->dt.i==SLL_OBJECT_GET_TYPE(a)){
		SLL_ACQUIRE(a);
		return a;
	}
	if (SLL_OBJECT_GET_TYPE(a)>SLL_MAX_OBJECT_TYPE){
		if (sll_current_runtime_data&&SLL_OBJECT_GET_TYPE(a)<=sll_current_runtime_data->tt->l+SLL_MAX_OBJECT_TYPE){
			const sll_object_type_data_t* dt=*(sll_current_runtime_data->tt->dt+SLL_OBJECT_GET_TYPE(a)-SLL_MAX_OBJECT_TYPE-1);
			if (dt->fn.str){
				_push_call_stack(SLL_CHAR("@sll_api_string_convert"),SLL_MAX_STACK_OFFSET);
				sll_object_t* v=sll_execute_function(dt->fn.str,&a,1);
				_pop_call_stack();
				sll_object_t* str=sll_operator_cast(v,sll_static_int[SLL_OBJECT_TYPE_STRING]);
				SLL_RELEASE(v);
				return str;
			}
		}
		SLL_ACQUIRE(a);
		return a;
	}
	if (SLL_OBJECT_GET_TYPE(b)==SLL_OBJECT_TYPE_INT){
		switch (COMBINE_TYPES(SLL_OBJECT_GET_TYPE(a),b->dt.i)){
			case COMBINED_TYPE_IF:
				return SLL_FROM_FLOAT((sll_float_t)(a->dt.i));
			case COMBINED_TYPE_IC:
				return SLL_FROM_CHAR((a->dt.i)&0xff);
			case COMBINED_TYPE_IS:
			case COMBINED_TYPE_FS:
			case COMBINED_TYPE_MS:
				{
					sll_object_t* o=SLL_CREATE();
					o->t=SLL_OBJECT_TYPE_STRING;
					sll_api_string_convert(&a,1,&(o->dt.s));
					return o;
				}
			case COMBINED_TYPE_IA:
				{
					sll_object_t* o=SLL_CREATE();
					o->t=SLL_OBJECT_TYPE_ARRAY;
					if (!(a->dt.i<1?sll_array_create(0,&(o->dt.a)):sll_array_from_length((sll_array_length_t)(a->dt.i),&(o->dt.a)))){
						SLL_UNIMPLEMENTED();
					}
					return o;
				}
			case COMBINED_TYPE_IM:
			case COMBINED_TYPE_FM:
			case COMBINED_TYPE_CM:
				{
					sll_object_t* o=SLL_CREATE();
					o->t=SLL_OBJECT_TYPE_MAP;
					sll_map_create(1,&(o->dt.m));
					o->dt.m.v[0]=SLL_ACQUIRE_STATIC_INT(0);
					o->dt.m.v[1]=a;
					SLL_ACQUIRE(a);
					return o;
				}
			case COMBINED_TYPE_IK:
			case COMBINED_TYPE_FK:
			case COMBINED_TYPE_CK:
				{
					sll_object_t* o=SLL_CREATE();
					o->t=SLL_OBJECT_TYPE_ARRAY;
					if (!sll_array_create(1,&(o->dt.a))){
						SLL_UNIMPLEMENTED();
					}
					o->dt.a.v[0]=SLL_ACQUIRE_STATIC_INT(0);
					return o;
				}
			case COMBINED_TYPE_IV:
			case COMBINED_TYPE_FV:
			case COMBINED_TYPE_CA:
			case COMBINED_TYPE_CV:
				{
					sll_object_t* o=SLL_CREATE();
					o->t=SLL_OBJECT_TYPE_ARRAY;
					if (!sll_array_create(1,&(o->dt.a))){
						SLL_UNIMPLEMENTED();
					}
					o->dt.a.v[0]=a;
					SLL_ACQUIRE(a);
					return o;
				}
			case COMBINED_TYPE_FI:
				return SLL_FROM_INT((sll_integer_t)(a->dt.f));
			case COMBINED_TYPE_FC:
				return SLL_FROM_CHAR((sll_char_t)(a->dt.f));
			case COMBINED_TYPE_FA:
				{
					sll_integer_t n=sll_api_math_round(a->dt.f);
					sll_object_t* o=SLL_CREATE();
					o->t=SLL_OBJECT_TYPE_ARRAY;
					if (!(n<1?sll_array_create(0,&(o->dt.a)):sll_array_from_length((sll_array_length_t)(n),&(o->dt.a)))){
						SLL_UNIMPLEMENTED();
					}
					return o;
				}
			case COMBINED_TYPE_CI:
				return SLL_FROM_INT(a->dt.c);
			case COMBINED_TYPE_CF:
				return SLL_FROM_FLOAT(a->dt.c);
			case COMBINED_TYPE_CS:
				{
					sll_object_t* o=SLL_CREATE();
					o->t=SLL_OBJECT_TYPE_STRING;
					sll_string_from_char(a->dt.c,&(o->dt.s));
					return o;
				}
			case COMBINED_TYPE_SI:
				return SLL_FROM_INT(sll_string_parse_int(&(a->dt.s)));
			case COMBINED_TYPE_SF:
				return SLL_FROM_FLOAT(sll_string_parse_float(&(a->dt.s)));
			case COMBINED_TYPE_SC:
				{
					sll_char_t c=sll_string_parse_char(&(a->dt.s));
					return SLL_FROM_CHAR(c);
				}
			case COMBINED_TYPE_SA:
			case COMBINED_TYPE_SV:
				{
					sll_object_t* o=SLL_CREATE();
					o->t=SLL_OBJECT_TYPE_ARRAY;
					sll_string_to_array(&(a->dt.s),&(o->dt.a));
					return o;
				}
			case COMBINED_TYPE_SM:
				{
					sll_object_t* o=SLL_CREATE();
					o->t=SLL_OBJECT_TYPE_MAP;
					sll_string_to_map(&(a->dt.s),&(o->dt.m));
					return o;
				}
			case COMBINED_TYPE_SK:
				{
					sll_object_t* o=SLL_CREATE();
					o->t=SLL_OBJECT_TYPE_ARRAY;
					if (!sll_array_range(0,a->dt.s.l,1,&(o->dt.a))){
						SLL_UNIMPLEMENTED();
					}
					return o;
				}
			case COMBINED_TYPE_AI:
				return SLL_FROM_INT(sll_array_parse_int(&(a->dt.a)));
			case COMBINED_TYPE_AF:
				return SLL_FROM_FLOAT(sll_array_parse_float(&(a->dt.a)));
			case COMBINED_TYPE_AC:
				{
					sll_char_t c=sll_array_parse_char(&(a->dt.a));
					return SLL_FROM_CHAR(c);
				}
			case COMBINED_TYPE_AS:
				{
					sll_object_t* o=SLL_CREATE();
					o->t=SLL_OBJECT_TYPE_STRING;
					sll_string_from_data(a->dt.a.v,a->dt.a.l,&(o->dt.s));
					return o;
				}
			case COMBINED_TYPE_AM:
				{
					sll_object_t* o=SLL_CREATE();
					o->t=SLL_OBJECT_TYPE_MAP;
					if (!sll_array_to_map(&(a->dt.a),&(o->dt.m))){
						SLL_UNIMPLEMENTED();
					}
					return o;
				}
			case COMBINED_TYPE_AK:
				{
					sll_object_t* o=SLL_CREATE();
					o->t=SLL_OBJECT_TYPE_ARRAY;
					if (!sll_array_range(0,a->dt.a.l,1,&(o->dt.a))){
						SLL_UNIMPLEMENTED();
					}
					return o;
				}
			case COMBINED_TYPE_MI:
				return SLL_FROM_INT(a->dt.m.l);
			case COMBINED_TYPE_MF:
				return SLL_FROM_FLOAT(a->dt.m.l);
			case COMBINED_TYPE_MC:
				return SLL_FROM_CHAR(a->dt.m.l);
			case COMBINED_TYPE_MA:
				{
					sll_object_t* o=SLL_CREATE();
					o->t=SLL_OBJECT_TYPE_ARRAY;
					sll_map_to_array(&(a->dt.m),&(o->dt.a));
					return o;
				}
			case COMBINED_TYPE_MK:
				{
					sll_object_t* o=SLL_CREATE();
					o->t=SLL_OBJECT_TYPE_ARRAY;
					sll_map_keys(&(a->dt.m),&(o->dt.a));
					return o;
				}
			case COMBINED_TYPE_MV:
				{
					sll_object_t* o=SLL_CREATE();
					o->t=SLL_OBJECT_TYPE_ARRAY;
					sll_map_values(&(a->dt.m),&(o->dt.a));
					return o;
				}
			default:
				SLL_UNREACHABLE();
		}
	}
	SLL_ACQUIRE(a);
	return a;
}
