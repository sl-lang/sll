#include <sll/_internal/common.h>
#include <sll/_internal/gc.h>
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
#include <math.h>



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



static sll_object_t* _array_from_length(sll_integer_t len){
	if (len<=0){
		return sll_array_to_object(NULL);
	}
	sll_object_t* o=sll_array_length_to_object((sll_array_length_t)len);
	sll_static_int[0]->rc+=o->dt.a.l;
	for (sll_array_length_t i=0;i<o->dt.a.l;i++){
		o->dt.a.v[i]=sll_static_int[0];
	}
	return o;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_operator_cast(sll_object_t* a,sll_object_t* b){
	if (b->t!=SLL_OBJECT_TYPE_INT||b->dt.i<0||b->dt.i==a->t||b->dt.i==SLL_OBJECT_TYPE_OBJECT){
		SLL_ACQUIRE(a);
		return a;
	}
	if (a->t>SLL_MAX_OBJECT_TYPE){
		if (sll_current_runtime_data&&a->t<=sll_current_runtime_data->tt->l+SLL_MAX_OBJECT_TYPE){
			const sll_object_type_data_t* dt=*(sll_current_runtime_data->tt->dt+a->t-SLL_MAX_OBJECT_TYPE-1);
			switch (b->dt.i){
				case SLL_OBJECT_TYPE_INT:
					return sll_int_to_object(dt->l);
				case SLL_OBJECT_TYPE_FLOAT:
					SLL_UNIMPLEMENTED();
				case SLL_OBJECT_TYPE_CHAR:
					SLL_UNIMPLEMENTED();
				case SLL_OBJECT_TYPE_STRING:
					if (dt->fn[SLL_OBJECT_FUNC_STRING]){
						sll_object_t* v=sll_execute_function(dt->fn[SLL_OBJECT_FUNC_STRING],&a,1,0);
						sll_object_t* str=sll_operator_cast(v,sll_static_int[SLL_OBJECT_TYPE_STRING]);
						SLL_RELEASE(v);
						return str;
					}
					break;
				case SLL_OBJECT_TYPE_ARRAY:
					{
						sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_ARRAY);
						sll_object_to_array(sll_current_runtime_data->tt,a,&(o->dt.a));
						return o;
					}
				case SLL_OBJECT_TYPE_MAP:
					SLL_UNIMPLEMENTED();
				case SLL_OBJECT_TYPE_MAP_KEYS:
					SLL_UNIMPLEMENTED();
				case SLL_OBJECT_TYPE_MAP_VALUES:
					SLL_UNIMPLEMENTED();
			}
		}
		else{
			SLL_ACQUIRE(a);
			return a;
		}
	}
	if (b->dt.i>SLL_MAX_OBJECT_TYPE){
		if (sll_current_runtime_data&&b->dt.i<=sll_current_runtime_data->tt->l+SLL_MAX_OBJECT_TYPE){
			sll_object_t* src=sll_operator_cast(a,sll_static_int[SLL_OBJECT_TYPE_ARRAY]);
			sll_object_t* o=sll_create_object_type(sll_current_runtime_data->tt,(sll_object_type_t)(b->dt.i),src->dt.a.v,src->dt.a.l);
			SLL_RELEASE(src);
			return o;
		}
		SLL_ACQUIRE(a);
		return a;
	}
	switch (COMBINE_TYPES(a->t,b->dt.i)){
		case COMBINED_TYPE_IF:
			return sll_float_to_object((sll_float_t)(a->dt.i));
		case COMBINED_TYPE_IC:
			return SLL_FROM_CHAR((a->dt.i)&0xff);
		case COMBINED_TYPE_IS:
		case COMBINED_TYPE_FS:
		case COMBINED_TYPE_MS:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_STRING);
				sll_api_string_convert(&a,1,&(o->dt.s));
				return o;
			}
		case COMBINED_TYPE_IA:
			return _array_from_length(a->dt.i);
		case COMBINED_TYPE_IM:
		case COMBINED_TYPE_FM:
		case COMBINED_TYPE_CM:
			{
				sll_object_t* o=sll_map_length_to_object(1);
				o->dt.m.v[0]=SLL_ACQUIRE_STATIC_INT(0);
				o->dt.m.v[1]=a;
				SLL_ACQUIRE(a);
				return o;
			}
		case COMBINED_TYPE_IK:
		case COMBINED_TYPE_FK:
		case COMBINED_TYPE_CK:
			{
				sll_object_t* o=sll_array_length_to_object(1);
				o->dt.a.v[0]=SLL_ACQUIRE_STATIC_INT(0);
				return o;
			}
		case COMBINED_TYPE_IV:
		case COMBINED_TYPE_FV:
		case COMBINED_TYPE_CA:
		case COMBINED_TYPE_CV:
			{
				sll_object_t* o=sll_array_length_to_object(1);
				o->dt.a.v[0]=a;
				SLL_ACQUIRE(a);
				return o;
			}
		case COMBINED_TYPE_FI:
			return sll_int_to_object((sll_integer_t)(a->dt.f));
		case COMBINED_TYPE_FC:
			return SLL_FROM_CHAR((sll_char_t)(a->dt.f));
		case COMBINED_TYPE_FA:
			return _array_from_length((sll_integer_t)round(a->dt.f));
		case COMBINED_TYPE_CI:
			return sll_int_to_object(a->dt.c);
		case COMBINED_TYPE_CF:
			return sll_float_to_object(a->dt.c);
		case COMBINED_TYPE_CS:
			return sll_char_to_string_object(a->dt.c);
		case COMBINED_TYPE_SI:
			return sll_int_to_object(sll_string_parse_int(&(a->dt.s)));
		case COMBINED_TYPE_SF:
			return sll_float_to_object(sll_string_parse_float(&(a->dt.s)));
		case COMBINED_TYPE_SC:
			{
				sll_char_t c=sll_string_parse_char(&(a->dt.s));
				return SLL_FROM_CHAR(c);
			}
		case COMBINED_TYPE_SA:
		case COMBINED_TYPE_SV:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_ARRAY);
				sll_string_to_array(&(a->dt.s),&(o->dt.a));
				return o;
			}
		case COMBINED_TYPE_SM:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_MAP);
				sll_string_to_map(&(a->dt.s),&(o->dt.m));
				return o;
			}
		case COMBINED_TYPE_SK:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_ARRAY);
				sll_array_range(0,a->dt.s.l,1,&(o->dt.a));
				return o;
			}
		case COMBINED_TYPE_AI:
			return sll_int_to_object(sll_array_parse_int(&(a->dt.a)));
		case COMBINED_TYPE_AF:
			return sll_float_to_object(sll_array_parse_float(&(a->dt.a)));
		case COMBINED_TYPE_AC:
			{
				sll_char_t c=sll_array_parse_char(&(a->dt.a));
				return SLL_FROM_CHAR(c);
			}
		case COMBINED_TYPE_AS:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_STRING);
				sll_string_from_data(a->dt.a.v,a->dt.a.l,&(o->dt.s));
				return o;
			}
		case COMBINED_TYPE_AM:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_MAP);
				sll_array_to_map(&(a->dt.a),&(o->dt.m));
				return o;
			}
		case COMBINED_TYPE_AK:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_ARRAY);
				sll_array_range(0,a->dt.a.l,1,&(o->dt.a));
				return o;
			}
		case COMBINED_TYPE_MI:
			return sll_int_to_object(a->dt.m.l);
		case COMBINED_TYPE_MF:
			return sll_float_to_object(a->dt.m.l);
		case COMBINED_TYPE_MC:
			return SLL_FROM_CHAR(a->dt.m.l);
		case COMBINED_TYPE_MA:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_ARRAY);
				sll_map_to_array(&(a->dt.m),&(o->dt.a));
				return o;
			}
		case COMBINED_TYPE_MK:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_ARRAY);
				sll_map_keys(&(a->dt.m),&(o->dt.a));
				return o;
			}
		case COMBINED_TYPE_MV:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_ARRAY);
				sll_map_values(&(a->dt.m),&(o->dt.a));
				return o;
			}
		default:
			SLL_UNREACHABLE();
	}
	SLL_ACQUIRE(a);
	return a;
}
