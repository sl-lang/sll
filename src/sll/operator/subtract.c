#include <sll/_internal/gc.h>
#include <sll/_internal/operator.h>
#include <sll/array.h>
#include <sll/common.h>
#include <sll/gc.h>
#include <sll/map.h>
#include <sll/object.h>
#include <sll/operator.h>
#include <sll/static_object.h>
#include <sll/types.h>



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_operator_sub(sll_object_t* a,sll_object_t* b){
	if (a->t==SLL_OBJECT_TYPE_ARRAY){
		sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_ARRAY);
		if (b->t==SLL_OBJECT_TYPE_ARRAY){
			sll_array_remove_multiple(&(a->dt.a),b->dt.a.v,b->dt.a.l,&(o->dt.a));
		}
		else{
			sll_array_remove(&(a->dt.a),b,&(o->dt.a));
		}
		return o;
	}
	if (a->t==SLL_OBJECT_TYPE_MAP){
		sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_MAP);
		GC_RELEASE(sll_map_remove(&(a->dt.m),b,&(o->dt.m)));
		return o;
	}
	switch (COMBINED_ARGS){
		case COMBINED_TYPE_II:
			return sll_int_to_object(a->dt.i-b->dt.i);
		case COMBINED_TYPE_IF:
			return sll_float_to_object(a->dt.i-b->dt.f);
		case COMBINED_TYPE_IC:
			return sll_int_to_object(a->dt.i-b->dt.c);
		case COMBINED_TYPE_IS:
		case COMBINED_TYPE_FS:
		case COMBINED_TYPE_CS:
			{
				sll_object_t* o=sll_array_length_to_object(b->dt.s.l);
				sll_array_create(b->dt.s.l,&(o->dt.a));
				for (sll_array_length_t i=0;i<b->dt.s.l;i++){
					o->dt.a.v[i]=sll_operator_sub(a,sll_static_char[b->dt.s.v[i]]);
				}
				return o;
			}
		case COMBINED_TYPE_IA:
		case COMBINED_TYPE_FA:
		case COMBINED_TYPE_CA:
			{
				sll_object_t* o=sll_array_length_to_object(b->dt.a.l);
				for (sll_array_length_t i=0;i<b->dt.a.l;i++){
					o->dt.a.v[i]=sll_operator_sub(a,b->dt.a.v[i]);
				}
				return o;
			}
		case COMBINED_TYPE_IM:
		case COMBINED_TYPE_FM:
		case COMBINED_TYPE_CM:
			{
				sll_map_t m=b->dt.m;
				sll_object_t* o=sll_map_length_to_object(m.l);
				for (sll_array_length_t i=0;i<(m.l<<1);i+=2){
					SLL_ACQUIRE(m.v[i]);
					o->dt.m.v[i]=m.v[i];
					o->dt.m.v[i+1]=sll_operator_sub(a,m.v[i+1]);
				}
				return o;
			}
		case COMBINED_TYPE_FI:
			return sll_float_to_object(a->dt.f-b->dt.i);
		case COMBINED_TYPE_FF:
			return sll_float_to_object(a->dt.f-b->dt.f);
		case COMBINED_TYPE_FC:
			return sll_float_to_object(a->dt.f-b->dt.c);
		case COMBINED_TYPE_CI:
			return sll_int_to_object(a->dt.c-b->dt.i);
		case COMBINED_TYPE_CF:
			return sll_float_to_object(a->dt.c-b->dt.f);
		case COMBINED_TYPE_CC:
			return SLL_FROM_CHAR(a->dt.c-b->dt.c);
		case COMBINED_TYPE_SI:
		case COMBINED_TYPE_SF:
		case COMBINED_TYPE_SC:
			{
				sll_object_t* o=sll_array_length_to_object(a->dt.s.l);
				for (sll_string_length_t i=0;i<a->dt.s.l;i++){
					o->dt.a.v[i]=sll_operator_sub(sll_static_char[a->dt.s.v[i]],b);
				}
				return o;
			}
		case COMBINED_TYPE_SS:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_STRING);
				sll_string_remove(&(a->dt.s),&(b->dt.s),&(o->dt.s));
				return o;
			}
		case COMBINED_TYPE_SA:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_ARRAY);
				sll_string_op_array(&(a->dt.s),&(b->dt.a),sll_operator_sub,0,&(o->dt.a));
				return o;
			}
		case COMBINED_TYPE_SM:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_MAP);
				sll_string_op_map(&(a->dt.s),&(b->dt.m),sll_operator_sub,0,&(o->dt.m));
				return o;
			}
		default:
			SLL_UNREACHABLE();
	}
	return SLL_ACQUIRE_STATIC_INT(0);
}
