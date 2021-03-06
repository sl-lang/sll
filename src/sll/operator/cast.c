#include <sll/_internal/complex.h>
#include <sll/_internal/common.h>
#include <sll/_internal/operator.h>
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



#define COMBINED_TYPE_IK COMBINE_TYPES(SLL_OBJECT_TYPE_INT,SLL_OBJECT_TYPE_MAP_KEYS)
#define COMBINED_TYPE_IV COMBINE_TYPES(SLL_OBJECT_TYPE_INT,SLL_OBJECT_TYPE_MAP_VALUES)
#define COMBINED_TYPE_FK COMBINE_TYPES(SLL_OBJECT_TYPE_FLOAT,SLL_OBJECT_TYPE_MAP_KEYS)
#define COMBINED_TYPE_FV COMBINE_TYPES(SLL_OBJECT_TYPE_FLOAT,SLL_OBJECT_TYPE_MAP_VALUES)
#define COMBINED_TYPE_CK COMBINE_TYPES(SLL_OBJECT_TYPE_CHAR,SLL_OBJECT_TYPE_MAP_KEYS)
#define COMBINED_TYPE_CV COMBINE_TYPES(SLL_OBJECT_TYPE_CHAR,SLL_OBJECT_TYPE_MAP_VALUES)
#define COMBINED_TYPE_DK COMBINE_TYPES(SLL_OBJECT_TYPE_COMPLEX,SLL_OBJECT_TYPE_MAP_KEYS)
#define COMBINED_TYPE_DV COMBINE_TYPES(SLL_OBJECT_TYPE_COMPLEX,SLL_OBJECT_TYPE_MAP_VALUES)
#define COMBINED_TYPE_SK COMBINE_TYPES(SLL_OBJECT_TYPE_STRING,SLL_OBJECT_TYPE_MAP_KEYS)
#define COMBINED_TYPE_SV COMBINE_TYPES(SLL_OBJECT_TYPE_STRING,SLL_OBJECT_TYPE_MAP_VALUES)
#define COMBINED_TYPE_AK COMBINE_TYPES(SLL_OBJECT_TYPE_ARRAY,SLL_OBJECT_TYPE_MAP_KEYS)
#define COMBINED_TYPE_AV COMBINE_TYPES(SLL_OBJECT_TYPE_ARRAY,SLL_OBJECT_TYPE_MAP_VALUES)
#define COMBINED_TYPE_MK COMBINE_TYPES(SLL_OBJECT_TYPE_MAP,SLL_OBJECT_TYPE_MAP_KEYS)
#define COMBINED_TYPE_MV COMBINE_TYPES(SLL_OBJECT_TYPE_MAP,SLL_OBJECT_TYPE_MAP_VALUES)



static sll_object_t _array_from_length(sll_integer_t len){
	if (len<=0){
		return sll_array_to_object(NULL);
	}
	sll_object_t o=sll_array_length_to_object((sll_array_length_t)len);
	sll_static_int[0]->reference_count+=o->data.array.length;
	for (sll_array_length_t i=0;i<o->data.array.length;i++){
		o->data.array.data[i]=sll_static_int[0];
	}
	return o;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t sll_operator_cast(sll_object_t a,sll_object_t b){
	if (b->type!=SLL_OBJECT_TYPE_INT||b->data.int_<0||b->data.int_==a->type||b->data.int_==SLL_OBJECT_TYPE_SELF||b->data.int_==SLL_OBJECT_TYPE_OBJECT){
		SLL_ACQUIRE(a);
		return a;
	}
	if (a->type>SLL_MAX_OBJECT_TYPE){
		if (sll_current_runtime_data&&a->type<=sll_current_runtime_data->type_table->length+SLL_MAX_OBJECT_TYPE){
			const sll_object_type_data_t* dt=*(sll_current_runtime_data->type_table->data+a->type-SLL_MAX_OBJECT_TYPE-1);
			switch (b->data.int_){
				case SLL_OBJECT_TYPE_INT:
					return sll_int_to_object(dt->field_count);
				case SLL_OBJECT_TYPE_FLOAT:
					SLL_UNIMPLEMENTED();
				case SLL_OBJECT_TYPE_CHAR:
					SLL_UNIMPLEMENTED();
				case SLL_OBJECT_TYPE_STRING:
					if (dt->functions[SLL_OBJECT_FUNC_STRING]){
						sll_object_t v=sll_execute_function(dt->functions[SLL_OBJECT_FUNC_STRING],&a,1,0);
						sll_object_t str=sll_operator_cast(v,sll_static_int[SLL_OBJECT_TYPE_STRING]);
						SLL_RELEASE(v);
						return str;
					}
					break;
				case SLL_OBJECT_TYPE_ARRAY:
					{
						sll_object_t o=sll_create_object(SLL_OBJECT_TYPE_ARRAY);
						sll_object_to_array(sll_current_runtime_data->type_table,a,&(o->data.array));
						return o;
					}
				case SLL_OBJECT_TYPE_MAP:
					{
						sll_object_t o=sll_create_object(SLL_OBJECT_TYPE_MAP);
						sll_object_to_map(sll_current_runtime_data->type_table,a,&(o->data.map));
						return o;
					}
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
	if (b->data.int_>SLL_MAX_OBJECT_TYPE){
		if (sll_current_runtime_data&&b->data.int_<=sll_current_runtime_data->type_table->length+SLL_MAX_OBJECT_TYPE){
			sll_object_t src=sll_operator_cast(a,sll_static_int[SLL_OBJECT_TYPE_ARRAY]);
			sll_object_t o=sll_create_object_type(sll_current_runtime_data->type_table,(sll_object_type_t)(b->data.int_),src->data.array.data,src->data.array.length);
			SLL_RELEASE(src);
			return o;
		}
		SLL_ACQUIRE(a);
		return a;
	}
	switch (COMBINE_TYPES(a->type,b->data.int_)){
		case COMBINED_TYPE_IF:
			return sll_float_to_object((sll_float_t)(a->data.int_));
		case COMBINED_TYPE_IC:
			return SLL_FROM_CHAR((a->data.int_)&0xff);
		case COMBINED_TYPE_ID:
			{
				sll_complex_t v={
					(sll_float_t)a->data.int_,
				};
				return sll_complex_to_object(&v);
			}
		case COMBINED_TYPE_IS:
		case COMBINED_TYPE_FS:
		case COMBINED_TYPE_DS:
		case COMBINED_TYPE_MS:
			{
				sll_object_t o=sll_create_object(SLL_OBJECT_TYPE_STRING);
				sll_api_string_convert(&a,1,&(o->data.string));
				return o;
			}
		case COMBINED_TYPE_IA:
			return _array_from_length(a->data.int_);
		case COMBINED_TYPE_IM:
		case COMBINED_TYPE_FM:
		case COMBINED_TYPE_CM:
		case COMBINED_TYPE_DM:
			{
				sll_object_t o=sll_map_length_to_object(1);
				o->data.map.data[0]=SLL_ACQUIRE_STATIC_INT(0);
				o->data.map.data[1]=a;
				SLL_ACQUIRE(a);
				return o;
			}
		case COMBINED_TYPE_IK:
		case COMBINED_TYPE_FK:
		case COMBINED_TYPE_CK:
		case COMBINED_TYPE_DK:
			{
				sll_object_t o=sll_array_length_to_object(1);
				o->data.array.data[0]=SLL_ACQUIRE_STATIC_INT(0);
				return o;
			}
		case COMBINED_TYPE_IV:
		case COMBINED_TYPE_FV:
		case COMBINED_TYPE_CA:
		case COMBINED_TYPE_CV:
		case COMBINED_TYPE_DV:
			{
				sll_object_t o=sll_array_length_to_object(1);
				o->data.array.data[0]=a;
				SLL_ACQUIRE(a);
				return o;
			}
		case COMBINED_TYPE_FI:
			return sll_int_to_object((sll_integer_t)(a->data.float_));
		case COMBINED_TYPE_FC:
			return SLL_FROM_CHAR((sll_char_t)(a->data.float_));
		case COMBINED_TYPE_FD:
			SLL_UNIMPLEMENTED();
		case COMBINED_TYPE_FA:
			return _array_from_length((sll_integer_t)round(a->data.float_));
		case COMBINED_TYPE_CI:
			return sll_int_to_object(a->data.char_);
		case COMBINED_TYPE_CF:
			return sll_float_to_object(a->data.char_);
		case COMBINED_TYPE_CD:
			SLL_UNIMPLEMENTED();
		case COMBINED_TYPE_CS:
			return sll_char_to_string_object(a->data.char_);
		case COMBINED_TYPE_DI:
			return sll_int_to_object((sll_integer_t)round(COMPLEX_ABS(a->data.complex_)));
		case COMBINED_TYPE_DF:
			return sll_float_to_object(COMPLEX_ABS(a->data.complex_));
		case COMBINED_TYPE_DC:
			{
				sll_char_t v=(sll_char_t)round(COMPLEX_ABS(a->data.complex_));
				return SLL_FROM_CHAR(v);
			}
		case COMBINED_TYPE_DA:
			return _array_from_length((sll_integer_t)round(COMPLEX_ABS(a->data.complex_)));
		case COMBINED_TYPE_SI:
			return sll_int_to_object(sll_string_parse_int(&(a->data.string)));
		case COMBINED_TYPE_SF:
			return sll_float_to_object(sll_string_parse_float(&(a->data.string)));
		case COMBINED_TYPE_SC:
			{
				sll_char_t c=sll_string_parse_char(&(a->data.string));
				return SLL_FROM_CHAR(c);
			}
		case COMBINED_TYPE_SD:
			SLL_UNIMPLEMENTED();
		case COMBINED_TYPE_SA:
		case COMBINED_TYPE_SV:
			{
				sll_object_t o=sll_create_object(SLL_OBJECT_TYPE_ARRAY);
				sll_string_to_array(&(a->data.string),&(o->data.array));
				return o;
			}
		case COMBINED_TYPE_SM:
			{
				sll_object_t o=sll_create_object(SLL_OBJECT_TYPE_MAP);
				sll_string_to_map(&(a->data.string),&(o->data.map));
				return o;
			}
		case COMBINED_TYPE_SK:
			{
				sll_object_t o=sll_create_object(SLL_OBJECT_TYPE_ARRAY);
				sll_array_range(0,a->data.string.length,1,&(o->data.array));
				return o;
			}
		case COMBINED_TYPE_AI:
			return sll_int_to_object(a->data.array.length);
		case COMBINED_TYPE_AF:
			return sll_float_to_object(a->data.array.length);
		case COMBINED_TYPE_AC:
			return SLL_FROM_CHAR(a->data.array.length&0xff);
		case COMBINED_TYPE_AD:
			SLL_UNIMPLEMENTED();
		case COMBINED_TYPE_AS:
			{
				sll_object_t o=sll_create_object(SLL_OBJECT_TYPE_STRING);
				sll_string_from_data(a->data.array.data,a->data.array.length,&(o->data.string));
				return o;
			}
		case COMBINED_TYPE_AM:
			{
				sll_object_t o=sll_create_object(SLL_OBJECT_TYPE_MAP);
				sll_array_to_map(&(a->data.array),&(o->data.map));
				return o;
			}
		case COMBINED_TYPE_AK:
			{
				sll_object_t o=sll_create_object(SLL_OBJECT_TYPE_ARRAY);
				sll_array_range(0,a->data.array.length,1,&(o->data.array));
				return o;
			}
		case COMBINED_TYPE_MI:
			return sll_int_to_object(a->data.map.length);
		case COMBINED_TYPE_MF:
			return sll_float_to_object(a->data.map.length);
		case COMBINED_TYPE_MC:
			return SLL_FROM_CHAR(a->data.map.length);
		case COMBINED_TYPE_MD:
			{
				sll_complex_t v={
					a->data.map.length,
					0
				};
				return sll_complex_to_object(&v);
			}
		case COMBINED_TYPE_MA:
			{
				sll_object_t o=sll_create_object(SLL_OBJECT_TYPE_ARRAY);
				sll_map_to_array(&(a->data.map),&(o->data.array));
				return o;
			}
		case COMBINED_TYPE_MK:
			{
				sll_object_t o=sll_create_object(SLL_OBJECT_TYPE_ARRAY);
				sll_map_keys(&(a->data.map),&(o->data.array));
				return o;
			}
		case COMBINED_TYPE_MV:
			{
				sll_object_t o=sll_create_object(SLL_OBJECT_TYPE_ARRAY);
				sll_map_values(&(a->data.map),&(o->data.array));
				return o;
			}
		default:
			SLL_UNREACHABLE();
	}
	SLL_ACQUIRE(a);
	return a;
}
