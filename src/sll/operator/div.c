#include <sll/_internal/common.h>
#include <sll/_internal/operator.h>
#include <sll/array.h>
#include <sll/common.h>
#include <sll/gc.h>
#include <sll/map.h>
#include <sll/object.h>
#include <sll/static_object.h>
#include <sll/string.h>
#include <sll/types.h>
#include <math.h>



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_operator_div(sll_object_t* a,sll_object_t* b){
	if (a->t==SLL_OBJECT_TYPE_ARRAY){
		return sll_int_to_object((b->t==SLL_OBJECT_TYPE_ARRAY?sll_array_count_multiple(&(a->dt.a),b->dt.a.data,b->dt.a.length):sll_array_count(&(a->dt.a),b)));
	}
	switch (COMBINED_ARGS){
		case COMBINED_TYPE_II:
			return sll_float_to_object(((sll_float_t)(a->dt.i))/b->dt.i);
		case COMBINED_TYPE_IF:
			return sll_float_to_object(a->dt.i/b->dt.f);
		case COMBINED_TYPE_IC:
			return sll_float_to_object(((sll_float_t)(a->dt.i))/b->dt.c);
		case COMBINED_TYPE_IS:
		case COMBINED_TYPE_FS:
			{
				sll_object_t* o=sll_array_length_to_object(b->dt.a.length);
				for (sll_string_length_t i=0;i<b->dt.s.l;i++){
					o->dt.a.data[i]=sll_operator_div(a,sll_static_char[b->dt.s.v[i]]);
				}
				return o;
			}
		case COMBINED_TYPE_IA:
		case COMBINED_TYPE_FA:
		case COMBINED_TYPE_CA:
			{
				sll_object_t* o=sll_array_length_to_object(b->dt.a.length);
				for (sll_array_length_t i=0;i<b->dt.a.length;i++){
					o->dt.a.data[i]=sll_operator_div(a,b->dt.a.data[i]);
				}
				return o;
			}
		case COMBINED_TYPE_IM:
		case COMBINED_TYPE_FM:
		case COMBINED_TYPE_CM:
			{
				sll_object_t* o=sll_map_length_to_object(b->dt.m.length);
				for (sll_map_length_t i=0;i<(b->dt.m.length<<1);i+=2){
					o->dt.m.data[i]=b->dt.m.data[i];
					SLL_ACQUIRE(o->dt.m.data[i]);
					o->dt.m.data[i+1]=sll_operator_div(a,b->dt.m.data[i+1]);
				}
				return o;
			}
		case COMBINED_TYPE_FI:
			return sll_float_to_object(a->dt.f/b->dt.i);
		case COMBINED_TYPE_FF:
			return sll_float_to_object(a->dt.f/b->dt.f);
		case COMBINED_TYPE_FC:
			return sll_float_to_object(a->dt.f/b->dt.c);
		case COMBINED_TYPE_CI:
			return sll_float_to_object(((sll_float_t)(a->dt.c))/b->dt.i);
		case COMBINED_TYPE_CF:
			return sll_float_to_object(((sll_float_t)(a->dt.c))/b->dt.f);
		case COMBINED_TYPE_CC:
			return sll_float_to_object(((sll_float_t)(a->dt.c))/b->dt.c);
		case COMBINED_TYPE_CS:
			return sll_int_to_object(sll_string_count_char(&(b->dt.s),a->dt.c));
		case COMBINED_TYPE_SI:
		case COMBINED_TYPE_SF:
			{
				sll_object_t* o=sll_array_length_to_object(a->dt.s.l);
				for (sll_string_length_t i=0;i<a->dt.s.l;i++){
					o->dt.a.data[i]=sll_operator_div(sll_static_char[a->dt.s.v[i]],b);
				}
				return o;
			}
		case COMBINED_TYPE_SC:
			return sll_int_to_object(sll_string_count_char(&(a->dt.s),b->dt.c));
		case COMBINED_TYPE_SS:
			return sll_int_to_object(sll_string_count(&(a->dt.s),&(b->dt.s)));
		case COMBINED_TYPE_SA:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_ARRAY);
				sll_string_op_array(&(a->dt.s),&(b->dt.a),sll_operator_div,0,&(o->dt.a));
				return o;
			}
		case COMBINED_TYPE_SM:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_MAP);
				sll_map_remove_string(&(b->dt.m),&(a->dt.s),&(o->dt.m));
				return o;
			}
		case COMBINED_TYPE_MI:
		case COMBINED_TYPE_MF:
		case COMBINED_TYPE_MC:
			{
				sll_object_t* o=sll_map_length_to_object(a->dt.m.length);
				for (sll_map_length_t i=0;i<(a->dt.m.length<<1);i+=2){
					o->dt.m.data[i]=a->dt.m.data[i];
					SLL_ACQUIRE(o->dt.m.data[i]);
					o->dt.m.data[i+1]=sll_operator_div(a->dt.m.data[i+1],b);
				}
				return o;
			}
		case COMBINED_TYPE_MS:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_MAP);
				sll_map_remove_string(&(a->dt.m),&(b->dt.s),&(o->dt.m));
				return o;
			}
		case COMBINED_TYPE_MA:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_MAP);
				sll_map_remove_array(&(a->dt.m),&(b->dt.a),&(o->dt.m));
				return o;
			}
		case COMBINED_TYPE_MM:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_MAP);
				sll_map_remove_map(&(a->dt.m),&(b->dt.m),&(o->dt.m));
				return o;
			}
		default:
			SLL_UNREACHABLE();
	}
	return SLL_ACQUIRE_STATIC_INT(0);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_operator_floor_div(sll_object_t* a,sll_object_t* b){
	if (a->t==SLL_OBJECT_TYPE_ARRAY){
		return sll_int_to_object((b->t==SLL_OBJECT_TYPE_ARRAY?sll_array_count_multiple(&(a->dt.a),b->dt.a.data,b->dt.a.length):sll_array_count(&(a->dt.a),b)));
	}
	switch (COMBINED_ARGS){
		case COMBINED_TYPE_II:
			return sll_int_to_object(a->dt.i/b->dt.i);
		case COMBINED_TYPE_IF:
			return sll_int_to_object((sll_integer_t)round(a->dt.i/b->dt.f));
		case COMBINED_TYPE_IC:
			return sll_int_to_object(a->dt.i/b->dt.c);
		case COMBINED_TYPE_IS:
		case COMBINED_TYPE_FS:
			{
				sll_object_t* o=sll_array_length_to_object(b->dt.s.l);
				for (sll_string_length_t i=0;i<b->dt.s.l;i++){
					o->dt.a.data[i]=sll_operator_floor_div(a,sll_static_char[b->dt.s.v[i]]);
				}
				return o;
			}
		case COMBINED_TYPE_IA:
		case COMBINED_TYPE_FA:
		case COMBINED_TYPE_CA:
			{
				sll_object_t* o=sll_array_length_to_object(b->dt.a.length);
				for (sll_array_length_t i=0;i<b->dt.a.length;i++){
					o->dt.a.data[i]=sll_operator_floor_div(a,b->dt.a.data[i]);
				}
				return o;
			}
		case COMBINED_TYPE_IM:
		case COMBINED_TYPE_FM:
		case COMBINED_TYPE_CM:
			{
				sll_object_t* o=sll_map_length_to_object(b->dt.m.length);
				for (sll_map_length_t i=0;i<(b->dt.m.length<<1);i+=2){
					o->dt.m.data[i]=b->dt.m.data[i];
					SLL_ACQUIRE(o->dt.m.data[i]);
					o->dt.m.data[i+1]=sll_operator_floor_div(a,b->dt.m.data[i+1]);
				}
				return o;
			}
		case COMBINED_TYPE_FI:
			return sll_int_to_object((sll_integer_t)round(a->dt.f/b->dt.i));
		case COMBINED_TYPE_FF:
			return sll_int_to_object((sll_integer_t)round(a->dt.f/b->dt.f));
		case COMBINED_TYPE_FC:
			return sll_int_to_object((sll_integer_t)round(a->dt.f/b->dt.c));
		case COMBINED_TYPE_CI:
			return sll_int_to_object(a->dt.c/b->dt.i);
		case COMBINED_TYPE_CF:
			return sll_int_to_object((sll_integer_t)round(a->dt.c/b->dt.f));
		case COMBINED_TYPE_CC:
			return SLL_FROM_CHAR(a->dt.c/b->dt.c);
		case COMBINED_TYPE_CS:
			return sll_int_to_object(sll_string_count_char(&(b->dt.s),a->dt.c));
		case COMBINED_TYPE_SI:
		case COMBINED_TYPE_SF:
			{
				sll_object_t* o=sll_array_length_to_object(a->dt.s.l);
				for (sll_string_length_t i=0;i<a->dt.s.l;i++){
					o->dt.a.data[i]=sll_operator_floor_div(sll_static_char[a->dt.s.v[i]],b);
				}
				return o;
			}
		case COMBINED_TYPE_SC:
			return sll_int_to_object(sll_string_count_char(&(a->dt.s),b->dt.c));
		case COMBINED_TYPE_SS:
			return sll_int_to_object(sll_string_count(&(a->dt.s),&(b->dt.s)));
		case COMBINED_TYPE_SA:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_ARRAY);
				sll_string_op_array(&(a->dt.s),&(b->dt.a),sll_operator_floor_div,0,&(o->dt.a));
				return o;
			}
		case COMBINED_TYPE_SM:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_MAP);
				sll_map_remove_string(&(b->dt.m),&(a->dt.s),&(o->dt.m));
				return o;
			}
		case COMBINED_TYPE_MI:
		case COMBINED_TYPE_MF:
		case COMBINED_TYPE_MC:
			{
				sll_object_t* o=sll_map_length_to_object(a->dt.m.length);
				for (sll_map_length_t i=0;i<(a->dt.m.length<<1);i+=2){
					o->dt.m.data[i]=a->dt.m.data[i];
					SLL_ACQUIRE(o->dt.m.data[i]);
					o->dt.m.data[i+1]=sll_operator_floor_div(a->dt.m.data[i+1],b);
				}
				return o;
			}
		case COMBINED_TYPE_MS:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_MAP);
				sll_map_remove_string(&(a->dt.m),&(b->dt.s),&(o->dt.m));
				return o;
			}
		case COMBINED_TYPE_MA:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_MAP);
				sll_map_remove_array(&(a->dt.m),&(b->dt.a),&(o->dt.m));
				return o;
			}
		case COMBINED_TYPE_MM:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_MAP);
				sll_map_remove_map(&(a->dt.m),&(b->dt.m),&(o->dt.m));
				return o;
			}
		default:
			SLL_UNREACHABLE();
	}
	return SLL_ACQUIRE_STATIC_INT(0);
}
