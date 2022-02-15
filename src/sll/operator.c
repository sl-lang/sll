#include <sll/_size_types.h>
#include <sll/_sll_internal.h>
#include <sll/api/math.h>
#include <sll/api/string.h>
#include <sll/array.h>
#include <sll/common.h>
#include <sll/data.h>
#include <sll/gc.h>
#include <sll/map.h>
#include <sll/memory.h>
#include <sll/object.h>
#include <sll/operator.h>
#include <sll/static_object.h>
#include <sll/string.h>
#include <sll/types.h>
#include <sll/vm.h>



#define COMBINE_TYPES(a,b) (((unsigned int)(a)<<8)|(b))
#define COMBINED_ARGS COMBINE_TYPES(SLL_OBJECT_GET_TYPE(a),SLL_OBJECT_GET_TYPE(b))
#define COMBINED_TYPE_II COMBINE_TYPES(SLL_OBJECT_TYPE_INT,SLL_OBJECT_TYPE_INT)
#define COMBINED_TYPE_IF COMBINE_TYPES(SLL_OBJECT_TYPE_INT,SLL_OBJECT_TYPE_FLOAT)
#define COMBINED_TYPE_IC COMBINE_TYPES(SLL_OBJECT_TYPE_INT,SLL_OBJECT_TYPE_CHAR)
#define COMBINED_TYPE_IS COMBINE_TYPES(SLL_OBJECT_TYPE_INT,SLL_OBJECT_TYPE_STRING)
#define COMBINED_TYPE_IA COMBINE_TYPES(SLL_OBJECT_TYPE_INT,SLL_OBJECT_TYPE_ARRAY)
#define COMBINED_TYPE_IM COMBINE_TYPES(SLL_OBJECT_TYPE_INT,SLL_OBJECT_TYPE_MAP)
#define COMBINED_TYPE_FI COMBINE_TYPES(SLL_OBJECT_TYPE_FLOAT,SLL_OBJECT_TYPE_INT)
#define COMBINED_TYPE_FF COMBINE_TYPES(SLL_OBJECT_TYPE_FLOAT,SLL_OBJECT_TYPE_FLOAT)
#define COMBINED_TYPE_FC COMBINE_TYPES(SLL_OBJECT_TYPE_FLOAT,SLL_OBJECT_TYPE_CHAR)
#define COMBINED_TYPE_FS COMBINE_TYPES(SLL_OBJECT_TYPE_FLOAT,SLL_OBJECT_TYPE_STRING)
#define COMBINED_TYPE_FA COMBINE_TYPES(SLL_OBJECT_TYPE_FLOAT,SLL_OBJECT_TYPE_ARRAY)
#define COMBINED_TYPE_FM COMBINE_TYPES(SLL_OBJECT_TYPE_FLOAT,SLL_OBJECT_TYPE_MAP)
#define COMBINED_TYPE_CI COMBINE_TYPES(SLL_OBJECT_TYPE_CHAR,SLL_OBJECT_TYPE_INT)
#define COMBINED_TYPE_CF COMBINE_TYPES(SLL_OBJECT_TYPE_CHAR,SLL_OBJECT_TYPE_FLOAT)
#define COMBINED_TYPE_CC COMBINE_TYPES(SLL_OBJECT_TYPE_CHAR,SLL_OBJECT_TYPE_CHAR)
#define COMBINED_TYPE_CS COMBINE_TYPES(SLL_OBJECT_TYPE_CHAR,SLL_OBJECT_TYPE_STRING)
#define COMBINED_TYPE_CA COMBINE_TYPES(SLL_OBJECT_TYPE_CHAR,SLL_OBJECT_TYPE_ARRAY)
#define COMBINED_TYPE_CM COMBINE_TYPES(SLL_OBJECT_TYPE_CHAR,SLL_OBJECT_TYPE_MAP)
#define COMBINED_TYPE_SI COMBINE_TYPES(SLL_OBJECT_TYPE_STRING,SLL_OBJECT_TYPE_INT)
#define COMBINED_TYPE_SF COMBINE_TYPES(SLL_OBJECT_TYPE_STRING,SLL_OBJECT_TYPE_FLOAT)
#define COMBINED_TYPE_SC COMBINE_TYPES(SLL_OBJECT_TYPE_STRING,SLL_OBJECT_TYPE_CHAR)
#define COMBINED_TYPE_SS COMBINE_TYPES(SLL_OBJECT_TYPE_STRING,SLL_OBJECT_TYPE_STRING)
#define COMBINED_TYPE_SA COMBINE_TYPES(SLL_OBJECT_TYPE_STRING,SLL_OBJECT_TYPE_ARRAY)
#define COMBINED_TYPE_SM COMBINE_TYPES(SLL_OBJECT_TYPE_STRING,SLL_OBJECT_TYPE_MAP)
#define COMBINED_TYPE_AI COMBINE_TYPES(SLL_OBJECT_TYPE_ARRAY,SLL_OBJECT_TYPE_INT)
#define COMBINED_TYPE_AF COMBINE_TYPES(SLL_OBJECT_TYPE_ARRAY,SLL_OBJECT_TYPE_FLOAT)
#define COMBINED_TYPE_AC COMBINE_TYPES(SLL_OBJECT_TYPE_ARRAY,SLL_OBJECT_TYPE_CHAR)
#define COMBINED_TYPE_AS COMBINE_TYPES(SLL_OBJECT_TYPE_ARRAY,SLL_OBJECT_TYPE_STRING)
#define COMBINED_TYPE_AA COMBINE_TYPES(SLL_OBJECT_TYPE_ARRAY,SLL_OBJECT_TYPE_ARRAY)
#define COMBINED_TYPE_AM COMBINE_TYPES(SLL_OBJECT_TYPE_ARRAY,SLL_OBJECT_TYPE_MAP)
#define COMBINED_TYPE_MI COMBINE_TYPES(SLL_OBJECT_TYPE_MAP,SLL_OBJECT_TYPE_INT)
#define COMBINED_TYPE_MF COMBINE_TYPES(SLL_OBJECT_TYPE_MAP,SLL_OBJECT_TYPE_FLOAT)
#define COMBINED_TYPE_MC COMBINE_TYPES(SLL_OBJECT_TYPE_MAP,SLL_OBJECT_TYPE_CHAR)
#define COMBINED_TYPE_MS COMBINE_TYPES(SLL_OBJECT_TYPE_MAP,SLL_OBJECT_TYPE_STRING)
#define COMBINED_TYPE_MA COMBINE_TYPES(SLL_OBJECT_TYPE_MAP,SLL_OBJECT_TYPE_ARRAY)
#define COMBINED_TYPE_MM COMBINE_TYPES(SLL_OBJECT_TYPE_MAP,SLL_OBJECT_TYPE_MAP)

#define COMPARE_RESULT(a,b) ((a)<(b)?SLL_COMPARE_RESULT_BELOW:((a)>(b)?SLL_COMPARE_RESULT_ABOVE:SLL_COMPARE_RESULT_EQUAL))
#define COMPARE_RESULT_FLOAT(a,b) (EQUAL_FLOAT((a),(b))?SLL_COMPARE_RESULT_EQUAL:((a)>(b)?SLL_COMPARE_RESULT_ABOVE:SLL_COMPARE_RESULT_BELOW))
#define EQUAL_FLOAT(a,b) (sll_api_math_abs((a)-(b))<sll_float_compare_error)

#define COMMUTATIVE_OPERATOR \
	sll_bool_t inv=0; \
	if (SLL_OBJECT_GET_TYPE(a)>SLL_OBJECT_GET_TYPE(b)){ \
		sll_object_t* __tmp=a; \
		a=b; \
		b=__tmp; \
		inv=1; \
	} \

#define TO_FLOAT_BITS(n) ((n)->dt.i)



__SLL_EXTERNAL sll_float_t sll_float_compare_error=1e-6;



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_operator_access(sll_object_t* a,sll_object_t* b){
	if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_STRING){
		if (SLL_OBJECT_GET_TYPE(b)==SLL_OBJECT_TYPE_INT){
			sll_integer_t idx=b->dt.i-b->dt.i/a->dt.s.l*a->dt.s.l;
			if (idx<0){
				idx+=a->dt.s.l;
			}
			return SLL_FROM_CHAR(a->dt.s.v[idx]);
		}
	}
	else if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_ARRAY){
		if (SLL_OBJECT_GET_TYPE(b)==SLL_OBJECT_TYPE_INT){
			sll_integer_t idx=b->dt.i-b->dt.i/a->dt.a.l*a->dt.a.l;
			if (idx<0){
				idx+=a->dt.a.l;
			}
			sll_object_t* o=a->dt.a.v[idx];
			SLL_ACQUIRE(o);
			return o;
		}
	}
	else if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_MAP){
		return sll_map_get(&(a->dt.m),b);
	}
	else if (sll_current_runtime_data&&SLL_OBJECT_GET_TYPE(b)==SLL_OBJECT_TYPE_STRING&&SLL_OBJECT_GET_TYPE(a)>SLL_MAX_OBJECT_TYPE&&SLL_OBJECT_GET_TYPE(a)<=sll_current_runtime_data->tt->l+SLL_MAX_OBJECT_TYPE){
		return sll_object_get_field(sll_current_runtime_data->tt,a,&(b->dt.s));
	}
	SLL_ACQUIRE(a);
	return a;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_operator_access_range(sll_object_t* a,sll_object_t* b,sll_object_t* c){
	if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_STRING){
		if (SLL_OBJECT_GET_TYPE(b)==SLL_OBJECT_TYPE_INT&&SLL_OBJECT_GET_TYPE(c)==SLL_OBJECT_TYPE_INT){
			sll_object_t* o=SLL_CREATE();
			o->t=SLL_OBJECT_TYPE_STRING;
			sll_string_select(&(a->dt.s),b->dt.i,c->dt.i,1,&(o->dt.s));
			return o;
		}
	}
	else if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_ARRAY){
		if (SLL_OBJECT_GET_TYPE(b)==SLL_OBJECT_TYPE_INT&&SLL_OBJECT_GET_TYPE(c)==SLL_OBJECT_TYPE_INT){
			sll_object_t* o=SLL_CREATE();
			o->t=SLL_OBJECT_TYPE_ARRAY;
			sll_array_select(&(a->dt.a),b->dt.i,c->dt.i,1,&(o->dt.a));
			return o;
		}
	}
	else if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_MAP){
		sll_object_t* o=SLL_CREATE();
		o->t=SLL_OBJECT_TYPE_MAP;
		sll_object_t* l[2]={b,c};
		sll_array_t arr={
			2,
			l
		};
		sll_map_and_array(&(a->dt.m),&arr,&(o->dt.m));
		return o;
	}
	SLL_ACQUIRE(a);
	return a;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_operator_access_range_step(sll_object_t* a,sll_object_t* b,sll_object_t* c,sll_object_t* d){
	if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_STRING){
		if (SLL_OBJECT_GET_TYPE(b)==SLL_OBJECT_TYPE_INT&&SLL_OBJECT_GET_TYPE(c)==SLL_OBJECT_TYPE_INT&&SLL_OBJECT_GET_TYPE(d)==SLL_OBJECT_TYPE_INT){
			sll_object_t* o=SLL_CREATE();
			o->t=SLL_OBJECT_TYPE_STRING;
			sll_string_select(&(a->dt.s),b->dt.i,c->dt.i,d->dt.i,&(o->dt.s));
			return o;
		}
	}
	else if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_ARRAY){
		if (SLL_OBJECT_GET_TYPE(b)==SLL_OBJECT_TYPE_INT&&SLL_OBJECT_GET_TYPE(c)==SLL_OBJECT_TYPE_INT&&SLL_OBJECT_GET_TYPE(d)==SLL_OBJECT_TYPE_INT){
			sll_object_t* o=SLL_CREATE();
			o->t=SLL_OBJECT_TYPE_ARRAY;
			sll_array_select(&(a->dt.a),b->dt.i,c->dt.i,d->dt.i,&(o->dt.a));
			return o;
		}
	}
	else if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_MAP){
		sll_object_t* o=SLL_CREATE();
		o->t=SLL_OBJECT_TYPE_MAP;
		sll_object_t* l[3]={b,c,d};
		sll_array_t arr={
			3,
			l
		};
		sll_map_and_array(&(a->dt.m),&arr,&(o->dt.m));
		return o;
	}
	SLL_ACQUIRE(a);
	return a;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_operator_add(sll_object_t* a,sll_object_t* b){
	COMMUTATIVE_OPERATOR;
	switch (COMBINED_ARGS){
		case COMBINED_TYPE_II:
			return SLL_FROM_INT(a->dt.i+b->dt.i);
		case COMBINED_TYPE_IF:
			return SLL_FROM_FLOAT(a->dt.i+b->dt.f);
		case COMBINED_TYPE_IC:
			return SLL_FROM_INT(a->dt.i+b->dt.c);
		case COMBINED_TYPE_IS:
		case COMBINED_TYPE_FS:
		case COMBINED_TYPE_CS:
			{
				sll_object_t* tmp=a;
				a=b;
				b=tmp;
_add_to_string:
				SLL_ASSERT(SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_STRING);
				sll_string_t sa=a->dt.s;
				sll_string_t sb;
				sll_api_string_convert(&b,1,&sb);
				sll_object_t* o=SLL_CREATE();
				o->t=SLL_OBJECT_TYPE_STRING;
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
			{
				sll_object_t* o=SLL_CREATE();
				o->t=SLL_OBJECT_TYPE_ARRAY;
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
			{
				sll_object_t* o=SLL_CREATE();
				o->t=SLL_OBJECT_TYPE_MAP;
				sll_map_add(&(b->dt.m),a,sll_static_int[0],&(o->dt.m));
				return o;
			}
		case COMBINED_TYPE_FF:
			return SLL_FROM_FLOAT(a->dt.f+b->dt.f);
		case COMBINED_TYPE_FC:
			return SLL_FROM_FLOAT(a->dt.f+b->dt.c);
		case COMBINED_TYPE_CC:
			return SLL_FROM_CHAR(a->dt.c+b->dt.c);
		case COMBINED_TYPE_SS:
			{
				sll_string_t sa=a->dt.s;
				sll_string_t sb=b->dt.s;
				sll_object_t* o=SLL_CREATE();
				o->t=SLL_OBJECT_TYPE_STRING;
				if (!sa.l&&!sb.l){
					sll_string_create(0,&(o->dt.s));
				}
				else{
					sll_string_concat(&sa,&sb,&(o->dt.s));
				}
				return o;
			}
		case COMBINED_TYPE_SA:
			if (inv){
				sll_object_t* o=SLL_CREATE();
				o->t=SLL_OBJECT_TYPE_ARRAY;
				sll_array_push(&(b->dt.a),a,&(o->dt.a));
				return o;
			}
			goto _add_to_string;
		case COMBINED_TYPE_SM:
			if (inv){
				sll_object_t* o=SLL_CREATE();
				o->t=SLL_OBJECT_TYPE_MAP;
				sll_map_add(&(b->dt.m),a,sll_static_int[0],&(o->dt.m));
				return o;
			}
			goto _add_to_string;
		case COMBINED_TYPE_AA:
			{
				sll_object_t* o=SLL_CREATE();
				o->t=SLL_OBJECT_TYPE_ARRAY;
				sll_array_join(&(a->dt.a),&(b->dt.a),&(o->dt.a));
				return o;
			}
		case COMBINED_TYPE_AM:
			{
				if (inv){
					sll_object_t* o=SLL_CREATE();
					o->t=SLL_OBJECT_TYPE_MAP;
					sll_map_add(&(b->dt.m),a,sll_static_int[0],&(o->dt.m));
					return o;
				}
				sll_object_t* o=SLL_CREATE();
				o->t=SLL_OBJECT_TYPE_ARRAY;
				sll_array_push(&(a->dt.a),b,&(o->dt.a));
				return o;
			}
		case COMBINED_TYPE_MM:
			{
				sll_object_t* o=SLL_CREATE();
				o->t=SLL_OBJECT_TYPE_MAP;
				sll_map_join(&(a->dt.m),&(b->dt.m),&(o->dt.m));
				return o;
			}
		default:
			SLL_UNREACHABLE();
	}
	return SLL_ACQUIRE_STATIC_INT(0);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_operator_and(sll_object_t* a,sll_object_t* b){
	COMMUTATIVE_OPERATOR;
	switch (COMBINED_ARGS){
		case COMBINED_TYPE_II:
			return SLL_FROM_INT(a->dt.i&b->dt.i);
		case COMBINED_TYPE_IF:
			{
				float_data_t v={
					.dt=a->dt.i&TO_FLOAT_BITS(b)
				};
				return SLL_FROM_FLOAT(v.v);
			}
		case COMBINED_TYPE_IC:
			return SLL_FROM_INT(a->dt.i&b->dt.c);
		case COMBINED_TYPE_IS:
			{
				sll_object_t* o=SLL_CREATE();
				o->t=SLL_OBJECT_TYPE_STRING;
				sll_string_and_char(&(b->dt.s),(sll_char_t)(a->dt.i),&(o->dt.s));
				return o;
			}
		case COMBINED_TYPE_IA:
		case COMBINED_TYPE_FA:
		case COMBINED_TYPE_CA:
			{
				sll_object_t* o=SLL_CREATE();
				o->t=SLL_OBJECT_TYPE_ARRAY;
				sll_array_create(b->dt.a.l,&(o->dt.a));
				for (sll_array_length_t i=0;i<b->dt.a.l;i++){
					o->dt.a.v[i]=sll_operator_and(a,b->dt.a.v[i]);
				}
				return o;
			}
		case COMBINED_TYPE_IM:
		case COMBINED_TYPE_FM:
		case COMBINED_TYPE_CM:
			{
				sll_object_t* o=SLL_CREATE();
				o->t=SLL_OBJECT_TYPE_MAP;
				sll_map_create(b->dt.m.l,&(o->dt.m));
				for (sll_map_length_t i=0;i<(b->dt.a.l<<1);i+=2){
					o->dt.m.v[i]=a->dt.m.v[i];
					SLL_ACQUIRE(a->dt.m.v[i]);
					o->dt.m.v[i+1]=sll_operator_and(a,b->dt.m.v[i+1]);
				}
				return o;
			}
		case COMBINED_TYPE_FF:
			{
				float_data_t v={
					.dt=TO_FLOAT_BITS(a)&TO_FLOAT_BITS(b)
				};
				return SLL_FROM_FLOAT(v.v);
			}
		case COMBINED_TYPE_FC:
			{
				float_data_t v={
					.dt=TO_FLOAT_BITS(a)&b->dt.c
				};
				return SLL_FROM_FLOAT(v.v);
			}
		case COMBINED_TYPE_FS:
			{
				sll_object_t* o=SLL_CREATE();
				o->t=SLL_OBJECT_TYPE_STRING;
				sll_string_and_char(&(b->dt.s),(sll_char_t)sll_api_math_round(a->dt.f),&(o->dt.s));
				return o;
			}
		case COMBINED_TYPE_CC:
			return SLL_FROM_CHAR(a->dt.c&b->dt.c);
		case COMBINED_TYPE_CS:
			{
				sll_object_t* o=SLL_CREATE();
				o->t=SLL_OBJECT_TYPE_STRING;
				sll_string_and_char(&(b->dt.s),a->dt.c,&(o->dt.s));
				return o;
			}
		case COMBINED_TYPE_SS:
			{
				sll_object_t* o=SLL_CREATE();
				o->t=SLL_OBJECT_TYPE_STRING;
				sll_string_and(&(a->dt.s),&(b->dt.s),&(o->dt.s));
				return o;
			}
		case COMBINED_TYPE_SA:
			{
				sll_object_t* o=SLL_CREATE();
				o->t=SLL_OBJECT_TYPE_ARRAY;
				sll_string_op_array(&(a->dt.s),&(b->dt.a),sll_operator_and,inv,&(o->dt.a));
				return o;
			}
		case COMBINED_TYPE_SM:
			{
				sll_object_t* o=SLL_CREATE();
				o->t=SLL_OBJECT_TYPE_MAP;
				sll_map_and_string(&(b->dt.m),&(a->dt.s),&(o->dt.m));
				return o;
			}
		case COMBINED_TYPE_AA:
			{
				sll_object_t* o=SLL_CREATE();
				o->t=SLL_OBJECT_TYPE_ARRAY;
				sll_array_and(&(a->dt.a),&(b->dt.a),&(o->dt.a));
				return o;
			}
		case COMBINED_TYPE_AM:
			{
				sll_object_t* o=SLL_CREATE();
				o->t=SLL_OBJECT_TYPE_MAP;
				sll_map_and_array(&(b->dt.m),&(a->dt.a),&(o->dt.m));
				return o;
			}
		case COMBINED_TYPE_MM:
			{
				sll_object_t* o=SLL_CREATE();
				o->t=SLL_OBJECT_TYPE_MAP;
				sll_map_and(&(a->dt.m),&(b->dt.m),&(o->dt.m));
				return o;
			}
		default:
			SLL_UNREACHABLE();
	}
	return SLL_ACQUIRE_STATIC_INT(0);
}



__SLL_EXTERNAL void sll_operator_assign(sll_object_t* a,sll_object_t* b,sll_object_t* v){
	if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_STRING){
		if (SLL_OBJECT_GET_TYPE(b)==SLL_OBJECT_TYPE_INT){
			sll_object_t* tmp=sll_operator_cast(v,sll_static_int[SLL_OBJECT_TYPE_CHAR]);
			SLL_ASSERT(SLL_OBJECT_GET_TYPE(tmp)==SLL_OBJECT_TYPE_CHAR);
			sll_integer_t idx=b->dt.i-b->dt.i/a->dt.s.l*a->dt.s.l;
			if (idx<0){
				idx+=a->dt.s.l;
			}
			sll_string_set_char(tmp->dt.c,(sll_string_length_t)idx,&(a->dt.s));
			SLL_RELEASE(tmp);
		}
	}
	else if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_ARRAY){
		if (SLL_OBJECT_GET_TYPE(b)==SLL_OBJECT_TYPE_INT){
			sll_integer_t idx=b->dt.i-b->dt.i/a->dt.a.l*a->dt.a.l;
			if (idx<0){
				idx+=a->dt.a.l;
			}
			sll_array_set(&(a->dt.a),(sll_array_length_t)idx,v);
		}
	}
	else if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_MAP){
		sll_map_set(&(a->dt.m),b,v);
	}
	else if (sll_current_runtime_data&&SLL_OBJECT_GET_TYPE(b)==SLL_OBJECT_TYPE_STRING&&SLL_OBJECT_GET_TYPE(a)>SLL_MAX_OBJECT_TYPE&&SLL_OBJECT_GET_TYPE(a)<=sll_current_runtime_data->tt->l+SLL_MAX_OBJECT_TYPE){
		sll_object_set_field(sll_current_runtime_data->tt,a,&(b->dt.s),v);
	}
}



__SLL_EXTERNAL void sll_operator_assign_range(sll_object_t* a,sll_object_t* b,sll_object_t* c,sll_object_t* v){
	SLL_UNIMPLEMENTED();
}



__SLL_EXTERNAL void sll_operator_assign_range_step(sll_object_t* a,sll_object_t* b,sll_object_t* c,sll_object_t* d,sll_object_t* v){
	SLL_UNIMPLEMENTED();
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_operator_bool(const sll_object_t* a){
	switch (SLL_OBJECT_GET_TYPE(a)){
		case SLL_OBJECT_TYPE_INT:
			return !!a->dt.i;
		case SLL_OBJECT_TYPE_FLOAT:
			return !!a->dt.f;
		case SLL_OBJECT_TYPE_CHAR:
			return !!a->dt.c;
		case SLL_OBJECT_TYPE_STRING:
			return !!a->dt.s.l;
		case SLL_OBJECT_TYPE_ARRAY:
			return !!a->dt.a.l;
		case SLL_OBJECT_TYPE_MAP:
			return !!a->dt.m.l;
	}
	return 1;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_compare_result_t sll_operator_compare(const sll_object_t* a,const sll_object_t* b){
	switch (COMBINED_ARGS){
		case COMBINED_TYPE_II:
			return COMPARE_RESULT(a->dt.i,b->dt.i);
		case COMBINED_TYPE_IF:
			return COMPARE_RESULT_FLOAT(a->dt.i,b->dt.f);
		case COMBINED_TYPE_IC:
			return COMPARE_RESULT(a->dt.i,b->dt.c);
		case COMBINED_TYPE_IS:
			return COMPARE_RESULT(a->dt.i,b->dt.s.l);
		case COMBINED_TYPE_IA:
			return COMPARE_RESULT(a->dt.i,b->dt.a.l);
		case COMBINED_TYPE_IM:
			return COMPARE_RESULT(a->dt.i,b->dt.m.l);
		case COMBINED_TYPE_FI:
			return COMPARE_RESULT_FLOAT(a->dt.f,b->dt.i);
		case COMBINED_TYPE_FF:
			return COMPARE_RESULT_FLOAT(a->dt.f,b->dt.f);
		case COMBINED_TYPE_FC:
			return COMPARE_RESULT_FLOAT(a->dt.f,b->dt.c);
		case COMBINED_TYPE_FS:
			return COMPARE_RESULT_FLOAT(a->dt.f,b->dt.s.l);
		case COMBINED_TYPE_FA:
			return COMPARE_RESULT_FLOAT(a->dt.f,b->dt.a.l);
		case COMBINED_TYPE_FM:
			return COMPARE_RESULT_FLOAT(a->dt.f,b->dt.m.l);
		case COMBINED_TYPE_CI:
			return COMPARE_RESULT(a->dt.c,b->dt.i);
		case COMBINED_TYPE_CF:
			return COMPARE_RESULT(a->dt.c,b->dt.f);
		case COMBINED_TYPE_CC:
			return COMPARE_RESULT(a->dt.c,b->dt.c);
		case COMBINED_TYPE_CS:
			if (!b->dt.s.l){
				return SLL_COMPARE_RESULT_ABOVE;
			}
			if (b->dt.s.v[0]!=a->dt.c){
				return COMPARE_RESULT(a->dt.c,b->dt.s.v[0]);
			}
			return (b->dt.s.l==1?SLL_COMPARE_RESULT_EQUAL:SLL_COMPARE_RESULT_BELOW);
		case COMBINED_TYPE_CA:
			return COMPARE_RESULT(a->dt.c,b->dt.a.l);
		case COMBINED_TYPE_CM:
			return COMPARE_RESULT(a->dt.c,b->dt.m.l);
		case COMBINED_TYPE_SI:
			return COMPARE_RESULT(a->dt.s.l,b->dt.i);
		case COMBINED_TYPE_SF:
			return COMPARE_RESULT_FLOAT(a->dt.s.l,b->dt.f);
		case COMBINED_TYPE_SC:
			if (!a->dt.s.l){
				return SLL_COMPARE_RESULT_BELOW;
			}
			if (a->dt.s.v[0]!=b->dt.c){
				return COMPARE_RESULT(b->dt.c,a->dt.s.v[0]);
			}
			return (a->dt.s.l==1?SLL_COMPARE_RESULT_EQUAL:SLL_COMPARE_RESULT_ABOVE);
		case COMBINED_TYPE_SS:
			return sll_string_compare(&(a->dt.s),&(b->dt.s));
		case COMBINED_TYPE_SA:
			return sll_string_compare_array(&(a->dt.s),&(b->dt.a),0);
		case COMBINED_TYPE_SM:
			return sll_string_compare_map(&(a->dt.s),&(b->dt.m),0);
		case COMBINED_TYPE_AI:
			return COMPARE_RESULT(a->dt.a.l,b->dt.i);
		case COMBINED_TYPE_AF:
			return COMPARE_RESULT_FLOAT(a->dt.a.l,b->dt.f);
		case COMBINED_TYPE_AC:
			return COMPARE_RESULT(a->dt.a.l,b->dt.c);
		case COMBINED_TYPE_AS:
			return sll_string_compare_array(&(b->dt.s),&(a->dt.a),1);
		case COMBINED_TYPE_AA:
			return sll_array_compare(&(a->dt.a),&(b->dt.a));
		case COMBINED_TYPE_AM:
			return sll_array_compare_map(&(a->dt.a),&(b->dt.m),0);
		case COMBINED_TYPE_MI:
			return COMPARE_RESULT(a->dt.m.l,b->dt.i);
		case COMBINED_TYPE_MF:
			return COMPARE_RESULT_FLOAT(a->dt.m.l,b->dt.f);
		case COMBINED_TYPE_MC:
			return COMPARE_RESULT(a->dt.m.l,b->dt.c);
		case COMBINED_TYPE_MS:
			return sll_string_compare_map(&(b->dt.s),&(a->dt.m),1);
		case COMBINED_TYPE_MA:
			return sll_array_compare_map(&(b->dt.a),&(a->dt.m),1);
		case COMBINED_TYPE_MM:
			return sll_map_compare(&(a->dt.m),&(b->dt.m));
		default:
			SLL_UNREACHABLE();
	}
	return 0;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_operator_dec(sll_object_t* a){
	switch (SLL_OBJECT_GET_TYPE(a)){
		case SLL_OBJECT_TYPE_INT:
			return SLL_FROM_INT(a->dt.i-1);
		case SLL_OBJECT_TYPE_FLOAT:
			return SLL_FROM_FLOAT(a->dt.f-1);
		case SLL_OBJECT_TYPE_CHAR:
			return SLL_FROM_CHAR(a->dt.c-1);
		case SLL_OBJECT_TYPE_STRING:
			{
				sll_object_t* o=SLL_CREATE();
				o->t=SLL_OBJECT_TYPE_STRING;
				sll_string_t str;
				sll_string_from_char('1',&str);
				sll_string_remove(&(a->dt.s),&str,&(o->dt.s));
				sll_free_string(&str);
				return o;
			}
		case SLL_OBJECT_TYPE_ARRAY:
			{
				sll_object_t* o=SLL_CREATE();
				o->t=SLL_OBJECT_TYPE_ARRAY;
				sll_array_remove(&(a->dt.a),sll_static_int[1],&(o->dt.a));
				return o;
			}
		case SLL_OBJECT_TYPE_MAP:
			{
				sll_object_t* o=SLL_CREATE();
				o->t=SLL_OBJECT_TYPE_MAP;
				SLL_RELEASE(sll_map_remove(&(a->dt.m),sll_static_int[1],&(o->dt.m)));
				return o;
			}
		default:
			SLL_UNREACHABLE();
	}
	return SLL_ACQUIRE_STATIC_INT(0);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_operator_div(sll_object_t* a,sll_object_t* b){
	if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_ARRAY){
		return SLL_FROM_INT((SLL_OBJECT_GET_TYPE(b)==SLL_OBJECT_TYPE_ARRAY?sll_array_count_multiple(&(a->dt.a),b->dt.a.v,b->dt.a.l):sll_array_count(&(a->dt.a),b)));
	}
	switch (COMBINED_ARGS){
		case COMBINED_TYPE_II:
			return SLL_FROM_FLOAT(((sll_float_t)(a->dt.i))/b->dt.i);
		case COMBINED_TYPE_IF:
			return SLL_FROM_FLOAT(a->dt.i/b->dt.f);
		case COMBINED_TYPE_IC:
			return SLL_FROM_FLOAT(((sll_float_t)(a->dt.i))/b->dt.c);
		case COMBINED_TYPE_IS:
		case COMBINED_TYPE_FS:
			{
				sll_object_t* o=SLL_CREATE();
				o->t=SLL_OBJECT_TYPE_ARRAY;
				sll_array_create(b->dt.s.l,&(o->dt.a));
				for (sll_string_length_t i=0;i<b->dt.s.l;i++){
					o->dt.a.v[i]=sll_operator_div(a,sll_static_char[b->dt.s.v[i]]);
				}
				return o;
			}
		case COMBINED_TYPE_IA:
		case COMBINED_TYPE_FA:
		case COMBINED_TYPE_CA:
			{
				sll_object_t* o=SLL_CREATE();
				o->t=SLL_OBJECT_TYPE_ARRAY;
				sll_array_create(b->dt.a.l,&(o->dt.a));
				for (sll_array_length_t i=0;i<b->dt.a.l;i++){
					o->dt.a.v[i]=sll_operator_div(a,b->dt.a.v[i]);
				}
				return o;
			}
		case COMBINED_TYPE_IM:
		case COMBINED_TYPE_FM:
		case COMBINED_TYPE_CM:
			{
				sll_object_t* o=SLL_CREATE();
				o->t=SLL_OBJECT_TYPE_MAP;
				sll_map_create(b->dt.m.l,&(o->dt.m));
				for (sll_map_length_t i=0;i<(b->dt.m.l<<1);i+=2){
					o->dt.m.v[i]=b->dt.m.v[i];
					SLL_ACQUIRE(o->dt.m.v[i]);
					o->dt.m.v[i+1]=sll_operator_div(a,b->dt.m.v[i+1]);
				}
				return o;
			}
		case COMBINED_TYPE_FI:
			return SLL_FROM_FLOAT(a->dt.f/b->dt.i);
		case COMBINED_TYPE_FF:
			return SLL_FROM_FLOAT(a->dt.f/b->dt.f);
		case COMBINED_TYPE_FC:
			return SLL_FROM_FLOAT(a->dt.f/b->dt.c);
		case COMBINED_TYPE_CI:
			return SLL_FROM_FLOAT(((sll_float_t)(a->dt.c))/b->dt.i);
		case COMBINED_TYPE_CF:
			return SLL_FROM_FLOAT(((sll_float_t)(a->dt.c))/b->dt.f);
		case COMBINED_TYPE_CC:
			return SLL_FROM_FLOAT(((sll_float_t)(a->dt.c))/b->dt.c);
		case COMBINED_TYPE_CS:
			return SLL_FROM_INT(sll_string_count_char(&(b->dt.s),a->dt.c));
		case COMBINED_TYPE_SI:
		case COMBINED_TYPE_SF:
			{
				sll_object_t* o=SLL_CREATE();
				o->t=SLL_OBJECT_TYPE_ARRAY;
				sll_array_create(a->dt.s.l,&(o->dt.a));
				for (sll_string_length_t i=0;i<a->dt.s.l;i++){
					o->dt.a.v[i]=sll_operator_div(sll_static_char[a->dt.s.v[i]],b);
				}
				return o;
			}
		case COMBINED_TYPE_SC:
			return SLL_FROM_INT(sll_string_count_char(&(a->dt.s),b->dt.c));
		case COMBINED_TYPE_SS:
			return SLL_FROM_INT(sll_string_count(&(a->dt.s),&(b->dt.s)));
		case COMBINED_TYPE_SA:
			{
				sll_object_t* o=SLL_CREATE();
				o->t=SLL_OBJECT_TYPE_ARRAY;
				sll_string_op_array(&(a->dt.s),&(b->dt.a),sll_operator_div,0,&(o->dt.a));
				return o;
			}
		case COMBINED_TYPE_SM:
			{
				sll_object_t* o=SLL_CREATE();
				o->t=SLL_OBJECT_TYPE_MAP;
				sll_map_remove_string(&(b->dt.m),&(a->dt.s),&(o->dt.m));
				return o;
			}
		case COMBINED_TYPE_MI:
		case COMBINED_TYPE_MF:
		case COMBINED_TYPE_MC:
			{
				sll_object_t* o=SLL_CREATE();
				o->t=SLL_OBJECT_TYPE_MAP;
				sll_map_create(a->dt.m.l,&(o->dt.m));
				for (sll_map_length_t i=0;i<(a->dt.m.l<<1);i+=2){
					o->dt.m.v[i]=a->dt.m.v[i];
					SLL_ACQUIRE(o->dt.m.v[i]);
					o->dt.m.v[i+1]=sll_operator_div(a->dt.m.v[i+1],b);
				}
				return o;
			}
		case COMBINED_TYPE_MS:
			{
				sll_object_t* o=SLL_CREATE();
				o->t=SLL_OBJECT_TYPE_MAP;
				sll_map_remove_string(&(a->dt.m),&(b->dt.s),&(o->dt.m));
				return o;
			}
		case COMBINED_TYPE_MA:
			{
				sll_object_t* o=SLL_CREATE();
				o->t=SLL_OBJECT_TYPE_MAP;
				sll_map_remove_array(&(a->dt.m),&(b->dt.a),&(o->dt.m));
				return o;
			}
		case COMBINED_TYPE_MM:
			{
				sll_object_t* o=SLL_CREATE();
				o->t=SLL_OBJECT_TYPE_MAP;
				sll_map_remove_map(&(a->dt.m),&(b->dt.m),&(o->dt.m));
				return o;
			}
		default:
			SLL_UNREACHABLE();
	}
	return SLL_ACQUIRE_STATIC_INT(0);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_operator_dup(sll_object_t* a){
	switch (SLL_OBJECT_GET_TYPE(a)){
		case SLL_OBJECT_TYPE_INT:
		case SLL_OBJECT_TYPE_FLOAT:
		case SLL_OBJECT_TYPE_CHAR:
			SLL_ACQUIRE(a);
			return a;
		case SLL_OBJECT_TYPE_STRING:
			{
				sll_object_t* o=SLL_CREATE();
				o->t=SLL_OBJECT_TYPE_STRING;
				sll_string_clone(&(a->dt.s),&(o->dt.s));
				return o;
			}
		case SLL_OBJECT_TYPE_ARRAY:
			{
				sll_object_t* o=SLL_CREATE();
				o->t=SLL_OBJECT_TYPE_ARRAY;
				sll_array_clone(&(a->dt.a),&(o->dt.a));
				return o;
			}
		case SLL_OBJECT_TYPE_MAP:
			{
				sll_object_t* o=SLL_CREATE();
				o->t=SLL_OBJECT_TYPE_MAP;
				sll_map_clone(&(a->dt.m),&(o->dt.m));
				return o;
			}
	}
	SLL_ASSERT(SLL_OBJECT_GET_TYPE(a)>SLL_MAX_OBJECT_TYPE);
	if (sll_current_runtime_data&&SLL_OBJECT_GET_TYPE(a)<=sll_current_runtime_data->tt->l+SLL_MAX_OBJECT_TYPE){
		return sll_object_clone(sll_current_runtime_data->tt,a);
	}
	SLL_ACQUIRE(a);
	return a;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_operator_equal(const sll_object_t* a,const sll_object_t* b){
	switch (COMBINED_ARGS){
		case COMBINED_TYPE_II:
			return a->dt.i==b->dt.i;
		case COMBINED_TYPE_IF:
			return EQUAL_FLOAT(a->dt.i,b->dt.f);
		case COMBINED_TYPE_IC:
			return a->dt.i==b->dt.c;
		case COMBINED_TYPE_IS:
			return a->dt.i==b->dt.s.l;
		case COMBINED_TYPE_IA:
			return a->dt.i==b->dt.a.l;
		case COMBINED_TYPE_IM:
			return a->dt.i==b->dt.m.l;
		case COMBINED_TYPE_FI:
			return EQUAL_FLOAT(a->dt.f,b->dt.i);
		case COMBINED_TYPE_FF:
			return EQUAL_FLOAT(a->dt.f,b->dt.f);
		case COMBINED_TYPE_FC:
			return EQUAL_FLOAT(a->dt.f,b->dt.c);
		case COMBINED_TYPE_FS:
			return EQUAL_FLOAT(a->dt.f,b->dt.s.l);
		case COMBINED_TYPE_FA:
			return EQUAL_FLOAT(a->dt.f,b->dt.a.l);
		case COMBINED_TYPE_FM:
			return EQUAL_FLOAT(a->dt.f,b->dt.m.l);
		case COMBINED_TYPE_CI:
			return a->dt.c==b->dt.i;
		case COMBINED_TYPE_CF:
			return EQUAL_FLOAT(a->dt.c,b->dt.f);
		case COMBINED_TYPE_CC:
			return a->dt.c==b->dt.c;
		case COMBINED_TYPE_CS:
			return (b->dt.s.l==1&&b->dt.s.v[0]==a->dt.c);
		case COMBINED_TYPE_CA:
			return a->dt.c==b->dt.a.l;
		case COMBINED_TYPE_CM:
			return a->dt.c==b->dt.m.l;
		case COMBINED_TYPE_SI:
			return a->dt.s.l==b->dt.i;
		case COMBINED_TYPE_SF:
			return EQUAL_FLOAT(a->dt.s.l,b->dt.f);
		case COMBINED_TYPE_SC:
			return (a->dt.s.l==1&&a->dt.s.v[0]==b->dt.c);
		case COMBINED_TYPE_SS:
			return sll_string_equal(&(a->dt.s),&(b->dt.s));
		case COMBINED_TYPE_SA:
			return sll_string_equal_array(&(a->dt.s),&(b->dt.a));
		case COMBINED_TYPE_SM:
			return sll_string_equal_map(&(a->dt.s),&(b->dt.m));
		case COMBINED_TYPE_AI:
			return a->dt.a.l==b->dt.i;
		case COMBINED_TYPE_AF:
			return EQUAL_FLOAT(a->dt.a.l,b->dt.f);
		case COMBINED_TYPE_AC:
			return a->dt.a.l==b->dt.c;
		case COMBINED_TYPE_AS:
			return sll_string_equal_array(&(b->dt.s),&(a->dt.a));
		case COMBINED_TYPE_AA:
			return sll_array_equal(&(a->dt.a),&(b->dt.a));
		case COMBINED_TYPE_AM:
			return sll_array_equal_map(&(a->dt.a),&(b->dt.m));
		case COMBINED_TYPE_MI:
			return a->dt.m.l==b->dt.i;
		case COMBINED_TYPE_MF:
			return EQUAL_FLOAT(a->dt.m.l,b->dt.f);
		case COMBINED_TYPE_MC:
			return a->dt.m.l==b->dt.c;
		case COMBINED_TYPE_MS:
			return sll_string_equal_map(&(b->dt.s),&(a->dt.m));
		case COMBINED_TYPE_MA:
			return sll_array_equal_map(&(b->dt.a),&(a->dt.m));
		case COMBINED_TYPE_MM:
			return sll_map_equal(&(a->dt.m),&(b->dt.m));
		default:
			SLL_UNREACHABLE();
	}
	return 0;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_operator_floor_div(sll_object_t* a,sll_object_t* b){
	if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_ARRAY){
		return SLL_FROM_INT((SLL_OBJECT_GET_TYPE(b)==SLL_OBJECT_TYPE_ARRAY?sll_array_count_multiple(&(a->dt.a),b->dt.a.v,b->dt.a.l):sll_array_count(&(a->dt.a),b)));
	}
	switch (COMBINED_ARGS){
		case COMBINED_TYPE_II:
			return SLL_FROM_INT(a->dt.i/b->dt.i);
		case COMBINED_TYPE_IF:
			return SLL_FROM_INT(sll_api_math_round(a->dt.i/b->dt.f));
		case COMBINED_TYPE_IC:
			return SLL_FROM_INT(a->dt.i/b->dt.c);
		case COMBINED_TYPE_IS:
		case COMBINED_TYPE_FS:
			{
				sll_object_t* o=SLL_CREATE();
				o->t=SLL_OBJECT_TYPE_ARRAY;
				sll_array_create(b->dt.s.l,&(o->dt.a));
				for (sll_string_length_t i=0;i<b->dt.s.l;i++){
					o->dt.a.v[i]=sll_operator_floor_div(a,sll_static_char[b->dt.s.v[i]]);
				}
				return o;
			}
		case COMBINED_TYPE_IA:
		case COMBINED_TYPE_FA:
		case COMBINED_TYPE_CA:
			{
				sll_object_t* o=SLL_CREATE();
				o->t=SLL_OBJECT_TYPE_ARRAY;
				sll_array_create(b->dt.a.l,&(o->dt.a));
				for (sll_array_length_t i=0;i<b->dt.a.l;i++){
					o->dt.a.v[i]=sll_operator_floor_div(a,b->dt.a.v[i]);
				}
				return o;
			}
		case COMBINED_TYPE_IM:
		case COMBINED_TYPE_FM:
		case COMBINED_TYPE_CM:
			{
				sll_object_t* o=SLL_CREATE();
				o->t=SLL_OBJECT_TYPE_MAP;
				sll_map_create(b->dt.m.l,&(o->dt.m));
				for (sll_map_length_t i=0;i<(b->dt.m.l<<1);i+=2){
					o->dt.m.v[i]=b->dt.m.v[i];
					SLL_ACQUIRE(o->dt.m.v[i]);
					o->dt.m.v[i+1]=sll_operator_floor_div(a,b->dt.m.v[i+1]);
				}
				return o;
			}
		case COMBINED_TYPE_FI:
			return SLL_FROM_INT(sll_api_math_round(a->dt.f/b->dt.i));
		case COMBINED_TYPE_FF:
			return SLL_FROM_INT(sll_api_math_round(a->dt.f/b->dt.f));
		case COMBINED_TYPE_FC:
			return SLL_FROM_INT(sll_api_math_round(a->dt.f/b->dt.c));
		case COMBINED_TYPE_CI:
			return SLL_FROM_INT(a->dt.c/b->dt.i);
		case COMBINED_TYPE_CF:
			return SLL_FROM_INT(sll_api_math_round(a->dt.c/b->dt.f));
		case COMBINED_TYPE_CC:
			return SLL_FROM_CHAR(a->dt.c/b->dt.c);
		case COMBINED_TYPE_CS:
			return SLL_FROM_INT(sll_string_count_char(&(b->dt.s),a->dt.c));
		case COMBINED_TYPE_SI:
		case COMBINED_TYPE_SF:
			{
				sll_object_t* o=SLL_CREATE();
				o->t=SLL_OBJECT_TYPE_ARRAY;
				sll_array_create(a->dt.s.l,&(o->dt.a));
				for (sll_string_length_t i=0;i<a->dt.s.l;i++){
					o->dt.a.v[i]=sll_operator_floor_div(sll_static_char[a->dt.s.v[i]],b);
				}
				return o;
			}
		case COMBINED_TYPE_SC:
			return SLL_FROM_INT(sll_string_count_char(&(a->dt.s),b->dt.c));
		case COMBINED_TYPE_SS:
			return SLL_FROM_INT(sll_string_count(&(a->dt.s),&(b->dt.s)));
		case COMBINED_TYPE_SA:
			{
				sll_object_t* o=SLL_CREATE();
				o->t=SLL_OBJECT_TYPE_ARRAY;
				sll_string_op_array(&(a->dt.s),&(b->dt.a),sll_operator_floor_div,0,&(o->dt.a));
				return o;
			}
		case COMBINED_TYPE_SM:
			{
				sll_object_t* o=SLL_CREATE();
				o->t=SLL_OBJECT_TYPE_MAP;
				sll_map_remove_string(&(b->dt.m),&(a->dt.s),&(o->dt.m));
				return o;
			}
		case COMBINED_TYPE_MI:
		case COMBINED_TYPE_MF:
		case COMBINED_TYPE_MC:
			{
				sll_object_t* o=SLL_CREATE();
				o->t=SLL_OBJECT_TYPE_MAP;
				sll_map_create(a->dt.m.l,&(o->dt.m));
				for (sll_map_length_t i=0;i<(a->dt.m.l<<1);i+=2){
					o->dt.m.v[i]=a->dt.m.v[i];
					SLL_ACQUIRE(o->dt.m.v[i]);
					o->dt.m.v[i+1]=sll_operator_floor_div(a->dt.m.v[i+1],b);
				}
				return o;
			}
		case COMBINED_TYPE_MS:
			{
				sll_object_t* o=SLL_CREATE();
				o->t=SLL_OBJECT_TYPE_MAP;
				sll_map_remove_string(&(a->dt.m),&(b->dt.s),&(o->dt.m));
				return o;
			}
		case COMBINED_TYPE_MA:
			{
				sll_object_t* o=SLL_CREATE();
				o->t=SLL_OBJECT_TYPE_MAP;
				sll_map_remove_array(&(a->dt.m),&(b->dt.a),&(o->dt.m));
				return o;
			}
		case COMBINED_TYPE_MM:
			{
				sll_object_t* o=SLL_CREATE();
				o->t=SLL_OBJECT_TYPE_MAP;
				sll_map_remove_map(&(a->dt.m),&(b->dt.m),&(o->dt.m));
				return o;
			}
		default:
			SLL_UNREACHABLE();
	}
	return SLL_ACQUIRE_STATIC_INT(0);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_operator_inc(sll_object_t* a){
	switch (SLL_OBJECT_GET_TYPE(a)){
		case SLL_OBJECT_TYPE_INT:
			return SLL_FROM_INT(a->dt.i+1);
		case SLL_OBJECT_TYPE_FLOAT:
			return SLL_FROM_FLOAT(a->dt.f+1);
		case SLL_OBJECT_TYPE_CHAR:
			return SLL_FROM_CHAR(a->dt.c+1);
		case SLL_OBJECT_TYPE_STRING:
			{
				sll_object_t* o=SLL_CREATE();
				o->t=SLL_OBJECT_TYPE_STRING;
				sll_string_concat_char(&(a->dt.s),'1',&(o->dt.s));
				return o;
			}
		case SLL_OBJECT_TYPE_ARRAY:
			{
				sll_object_t* o=SLL_CREATE();
				o->t=SLL_OBJECT_TYPE_ARRAY;
				sll_array_push(&(a->dt.a),sll_static_int[1],&(o->dt.a));
				return o;
			}
		case SLL_OBJECT_TYPE_MAP:
			{
				sll_object_t* o=SLL_CREATE();
				o->t=SLL_OBJECT_TYPE_MAP;
				sll_map_add(&(a->dt.m),sll_static_int[1],sll_static_int[0],&(o->dt.m));
				return o;
			}
		default:
			SLL_UNREACHABLE();
	}
	return SLL_ACQUIRE_STATIC_INT(0);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_operator_includes(sll_object_t* a,sll_object_t* b){
	if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_STRING){
		if (SLL_OBJECT_GET_TYPE(b)==SLL_OBJECT_TYPE_CHAR){
			return sll_string_includes_char(&(a->dt.s),b->dt.c);
		}
		if (SLL_OBJECT_GET_TYPE(b)==SLL_OBJECT_TYPE_STRING){
			return sll_string_includes(&(a->dt.s),&(b->dt.s));
		}
	}
	else if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_ARRAY){
		return sll_array_includes(&(a->dt.a),b);
	}
	else if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_MAP){
		return sll_map_includes(&(a->dt.m),b);
	}
	return 0;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_operator_inv(sll_object_t* a){
	switch (SLL_OBJECT_GET_TYPE(a)){
		case SLL_OBJECT_TYPE_INT:
			return SLL_FROM_INT(~a->dt.i);
		case SLL_OBJECT_TYPE_FLOAT:
			return SLL_FROM_FLOAT(-a->dt.f-1);
		case SLL_OBJECT_TYPE_CHAR:
			return SLL_FROM_CHAR(~a->dt.c);
		case SLL_OBJECT_TYPE_STRING:
			{
				sll_object_t* o=SLL_CREATE();
				o->t=SLL_OBJECT_TYPE_STRING;
				sll_string_inv(&(a->dt.s),&(o->dt.s));
				return o;
			}
		case SLL_OBJECT_TYPE_ARRAY:
			{
				sll_object_t* o=SLL_CREATE();
				o->t=SLL_OBJECT_TYPE_ARRAY;
				sll_array_create(a->dt.a.l,&(o->dt.a));
				for (sll_array_length_t i=0;i<a->dt.a.l;i++){
					o->dt.a.v[i]=sll_operator_inv(a->dt.a.v[i]);
				}
				return o;
			}
		case SLL_OBJECT_TYPE_MAP:
			{
				sll_object_t* o=SLL_CREATE();
				o->t=SLL_OBJECT_TYPE_MAP;
				sll_map_create(a->dt.m.l,&(o->dt.m));
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



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_operator_len(sll_object_t* a){
	switch (SLL_OBJECT_GET_TYPE(a)){
		case SLL_OBJECT_TYPE_INT:
		case SLL_OBJECT_TYPE_FLOAT:
		case SLL_OBJECT_TYPE_CHAR:
		case SLL_OBJECT_TYPE_STRING:
			return SLL_FROM_INT(a->dt.s.l);
		case SLL_OBJECT_TYPE_ARRAY:
			return SLL_FROM_INT(a->dt.a.l);
		case SLL_OBJECT_TYPE_MAP:
			return SLL_FROM_INT(a->dt.m.l);
		default:
			SLL_UNREACHABLE();
	}
	return SLL_ACQUIRE_STATIC_INT(0);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_operator_mod(sll_object_t* a,sll_object_t* b){
	switch (COMBINED_ARGS){
		case COMBINED_TYPE_II:
			return SLL_FROM_INT(a->dt.i%b->dt.i);
		case COMBINED_TYPE_IF:
			return SLL_FROM_FLOAT(sll_math_mod((sll_float_t)(a->dt.i),b->dt.f));
		case COMBINED_TYPE_IC:
			return SLL_FROM_INT(a->dt.i%b->dt.c);
		case COMBINED_TYPE_IS:
		case COMBINED_TYPE_FS:
			{
				sll_object_t* o=SLL_CREATE();
				o->t=SLL_OBJECT_TYPE_ARRAY;
				sll_array_create(b->dt.s.l,&(o->dt.a));
				for (sll_string_length_t i=0;i<b->dt.s.l;i++){
					o->dt.a.v[i]=sll_operator_mod(a,sll_static_char[b->dt.s.v[i]]);
				}
				return o;
			}
		case COMBINED_TYPE_IA:
		case COMBINED_TYPE_FA:
		case COMBINED_TYPE_CA:
			{
				sll_object_t* o=SLL_CREATE();
				o->t=SLL_OBJECT_TYPE_ARRAY;
				sll_array_create(b->dt.a.l,&(o->dt.a));
				for (sll_array_length_t i=0;i<b->dt.a.l;i++){
					o->dt.a.v[i]=sll_operator_mod(a,b->dt.a.v[i]);
				}
				return o;
			}
		case COMBINED_TYPE_IM:
		case COMBINED_TYPE_FM:
		case COMBINED_TYPE_CM:
			{
				sll_object_t* o=SLL_CREATE();
				o->t=SLL_OBJECT_TYPE_MAP;
				sll_map_create(b->dt.m.l,&(o->dt.m));
				for (sll_map_length_t i=0;i<(b->dt.m.l<<1);i+=2){
					o->dt.m.v[i]=b->dt.m.v[i];
					SLL_ACQUIRE(o->dt.m.v[i]);
					o->dt.m.v[i+1]=sll_operator_mod(a,b->dt.m.v[i+1]);
				}
				return o;
			}
		case COMBINED_TYPE_FI:
			return SLL_FROM_FLOAT(sll_math_mod(a->dt.f,(sll_float_t)(b->dt.i)));
		case COMBINED_TYPE_FF:
			return SLL_FROM_FLOAT(sll_math_mod(a->dt.f,b->dt.f));
		case COMBINED_TYPE_FC:
			return SLL_FROM_FLOAT(sll_math_mod(a->dt.f,b->dt.c));
		case COMBINED_TYPE_CI:
			return SLL_FROM_CHAR(a->dt.c%b->dt.i);
		case COMBINED_TYPE_CF:
			return SLL_FROM_FLOAT(sll_math_mod(a->dt.c,b->dt.f));
		case COMBINED_TYPE_CC:
			return SLL_FROM_CHAR(a->dt.c%b->dt.c);
		case COMBINED_TYPE_CS:
			{
				sll_object_t* o=SLL_CREATE();
				o->t=SLL_OBJECT_TYPE_ARRAY;
				sll_string_split_char(&(b->dt.s),a->dt.c,&(o->dt.a));
				return o;
			}
		case COMBINED_TYPE_SI:
		case COMBINED_TYPE_SF:
			{
				sll_object_t* o=SLL_CREATE();
				o->t=SLL_OBJECT_TYPE_ARRAY;
				sll_array_create(a->dt.s.l,&(o->dt.a));
				for (sll_string_length_t i=0;i<a->dt.s.l;i++){
					o->dt.a.v[i]=sll_operator_mod(sll_static_char[a->dt.s.v[i]],b);
				}
				return o;
			}
		case COMBINED_TYPE_SC:
			{
				sll_object_t* o=SLL_CREATE();
				o->t=SLL_OBJECT_TYPE_ARRAY;
				sll_string_split_char(&(a->dt.s),b->dt.c,&(o->dt.a));
				return o;
			}
		case COMBINED_TYPE_SS:
			{
				sll_object_t* o=SLL_CREATE();
				o->t=SLL_OBJECT_TYPE_ARRAY;
				sll_string_split(&(a->dt.s),&(b->dt.s),&(o->dt.a));
				return o;
			}
		case COMBINED_TYPE_SA:
			{
				sll_object_t* o=SLL_CREATE();
				o->t=SLL_OBJECT_TYPE_ARRAY;
				sll_string_op_array(&(a->dt.s),&(b->dt.a),sll_operator_mod,0,&(o->dt.a));
				return o;
			}
		case COMBINED_TYPE_SM:
			{
				sll_object_t* o=SLL_CREATE();
				o->t=SLL_OBJECT_TYPE_MAP;
				sll_string_op_map(&(a->dt.s),&(b->dt.m),sll_operator_mod,0,&(o->dt.m));
				return o;
			}
		case COMBINED_TYPE_AI:
		case COMBINED_TYPE_AF:
		case COMBINED_TYPE_AC:
			{
				sll_object_t* o=SLL_CREATE();
				o->t=SLL_OBJECT_TYPE_ARRAY;
				sll_array_create(a->dt.a.l,&(o->dt.a));
				for (sll_array_length_t i=0;i<a->dt.a.l;i++){
					o->dt.a.v[i]=sll_operator_mod(a->dt.a.v[i],b);
				}
				return o;
			}
		case COMBINED_TYPE_AS:
			{
				sll_object_t* o=SLL_CREATE();
				o->t=SLL_OBJECT_TYPE_ARRAY;
				sll_string_op_array(&(b->dt.s),&(a->dt.a),sll_operator_mod,1,&(o->dt.a));
				return o;
			}
		case COMBINED_TYPE_AA:
			{
				sll_object_t* o=SLL_CREATE();
				o->t=SLL_OBJECT_TYPE_ARRAY;
				sll_array_op(&(a->dt.a),&(b->dt.a),sll_operator_mod,&(o->dt.a));
				return o;
			}
		case COMBINED_TYPE_AM:
			{
				sll_object_t* o=SLL_CREATE();
				o->t=SLL_OBJECT_TYPE_MAP;
				sll_array_op_map(&(a->dt.a),&(b->dt.m),sll_operator_mod,0,&(o->dt.m));
				return o;
			}
		case COMBINED_TYPE_MI:
		case COMBINED_TYPE_MF:
		case COMBINED_TYPE_MC:
			{
				sll_object_t* o=SLL_CREATE();
				o->t=SLL_OBJECT_TYPE_MAP;
				sll_map_create(a->dt.m.l,&(o->dt.m));
				for (sll_map_length_t i=0;i<(a->dt.m.l<<1);i+=2){
					o->dt.m.v[i]=a->dt.m.v[i];
					SLL_ACQUIRE(o->dt.m.v[i]);
					o->dt.m.v[i+1]=sll_operator_mod(a->dt.m.v[i+1],b);
				}
				return o;
			}
		case COMBINED_TYPE_MS:
			{
				sll_object_t* o=SLL_CREATE();
				o->t=SLL_OBJECT_TYPE_MAP;
				sll_map_and_string(&(a->dt.m),&(b->dt.s),&(o->dt.m));
				return o;
			}
		case COMBINED_TYPE_MA:
			{
				sll_object_t* o=SLL_CREATE();
				o->t=SLL_OBJECT_TYPE_MAP;
				sll_map_and_array(&(a->dt.m),&(b->dt.a),&(o->dt.m));
				return o;
			}
		case COMBINED_TYPE_MM:
			{
				sll_object_t* o=SLL_CREATE();
				o->t=SLL_OBJECT_TYPE_MAP;
				sll_map_and(&(a->dt.m),&(b->dt.m),&(o->dt.m));
				return o;
			}
		default:
			SLL_UNREACHABLE();
	}
	return SLL_ACQUIRE_STATIC_INT(0);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_operator_mult(sll_object_t* a,sll_object_t* b){
	COMMUTATIVE_OPERATOR;
	switch (COMBINED_ARGS){
		case COMBINED_TYPE_II:
			return SLL_FROM_INT(a->dt.i*b->dt.i);
		case COMBINED_TYPE_IF:
			return SLL_FROM_FLOAT(a->dt.i*b->dt.f);
		case COMBINED_TYPE_IC:
			return SLL_FROM_INT(a->dt.i*b->dt.c);
		case COMBINED_TYPE_IS:
			{
				sll_object_t* o=SLL_CREATE();
				o->t=SLL_OBJECT_TYPE_STRING;
				sll_string_duplicate(&(b->dt.s),a->dt.i*(inv?1:-1),0,&(o->dt.s));
				return o;
			}
		case COMBINED_TYPE_IA:
			{
				sll_object_t* o=SLL_CREATE();
				o->t=SLL_OBJECT_TYPE_ARRAY;
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
				sll_object_t* o=SLL_CREATE();
				o->t=SLL_OBJECT_TYPE_MAP;
				sll_map_create(m.l,&(o->dt.m));
				for (sll_map_length_t i=0;i<(m.l<<1);i+=2){
					SLL_ACQUIRE(m.v[i]);
					o->dt.m.v[i]=m.v[i];
					o->dt.m.v[i+1]=sll_operator_mult(a,m.v[i+1]);
				}
				return o;
			}
		case COMBINED_TYPE_FF:
			return SLL_FROM_FLOAT(a->dt.f*b->dt.f);
		case COMBINED_TYPE_FC:
			return SLL_FROM_FLOAT(a->dt.f*b->dt.c);
		case COMBINED_TYPE_FS:
			{
				sll_object_t* o=SLL_CREATE();
				o->t=SLL_OBJECT_TYPE_STRING;
				sll_integer_t n=(sll_integer_t)a->dt.f;
				sll_string_duplicate(&(b->dt.s),n,(sll_string_length_t)sll_api_math_round(sll_api_math_abs(a->dt.f-n)*b->dt.s.l),&(o->dt.s));
				return o;
			}
		case COMBINED_TYPE_FA:
			{
				sll_object_t* o=SLL_CREATE();
				o->t=SLL_OBJECT_TYPE_ARRAY;
				sll_integer_t n=(sll_integer_t)a->dt.f;
				sll_array_duplicate(&(b->dt.a),n,(sll_array_length_t)sll_api_math_round(sll_api_math_abs(a->dt.f-n)*b->dt.a.l),&(o->dt.a));
				return o;
			}
		case COMBINED_TYPE_CC:
			return SLL_FROM_CHAR(a->dt.c*b->dt.c);
		case COMBINED_TYPE_CS:
			{
				sll_object_t* o=SLL_CREATE();
				o->t=SLL_OBJECT_TYPE_STRING;
				sll_string_duplicate(&(b->dt.s),a->dt.c,0,&(o->dt.s));
				return o;
			}
		case COMBINED_TYPE_CA:
			{
				sll_object_t* o=SLL_CREATE();
				o->t=SLL_OBJECT_TYPE_ARRAY;
				sll_array_duplicate(&(b->dt.a),a->dt.c,0,&(o->dt.a));
				return o;
			}
		case COMBINED_TYPE_SS:
			{
				sll_string_t sa=a->dt.s;
				sll_string_t sb=b->dt.s;
				sll_object_t* o=SLL_CREATE();
				o->t=SLL_OBJECT_TYPE_ARRAY;
				if (!sa.l||!sb.l){
					sll_array_create(0,&(o->dt.a));
				}
				else{
					sll_string_combinations(&sa,&sb,&(o->dt.a));
				}
				return o;
			}
		case COMBINED_TYPE_SA:
			{
				sll_string_t sa=a->dt.s;
				sll_array_t ab=b->dt.a;
				sll_object_t* o=SLL_CREATE();
				o->t=SLL_OBJECT_TYPE_ARRAY;
				if (!sa.l||!ab.l){
					sll_array_create(0,&(o->dt.a));
				}
				else{
					sll_array_combinations_string(&ab,&sa,&(o->dt.a));
				}
				return o;
			}
		case COMBINED_TYPE_AA:
			{
				sll_array_t aa=a->dt.a;
				sll_array_t ab=b->dt.a;
				sll_object_t* o=SLL_CREATE();
				o->t=SLL_OBJECT_TYPE_ARRAY;
				if (!aa.l||!ab.l){
					sll_array_create(0,&(o->dt.a));
				}
				else{
					sll_array_combinations(&aa,&ab,&(o->dt.a));
				}
				return o;
			}
		case COMBINED_TYPE_MM:
			{
				sll_object_t* o=SLL_CREATE();
				o->t=SLL_OBJECT_TYPE_MAP;
				sll_map_and(&(a->dt.m),&(b->dt.m),&(o->dt.m));
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
			return SLL_FROM_INT(a->dt.i|b->dt.i);
		case COMBINED_TYPE_IF:
			{
				float_data_t v={
					.dt=a->dt.i|TO_FLOAT_BITS(b)
				};
				return SLL_FROM_FLOAT(v.v);
			}
		case COMBINED_TYPE_IC:
			return SLL_FROM_INT(a->dt.i|b->dt.c);
		case COMBINED_TYPE_IS:
			{
				sll_object_t* o=SLL_CREATE();
				o->t=SLL_OBJECT_TYPE_STRING;
				sll_string_or_char(&(b->dt.s),(sll_char_t)(a->dt.i),&(o->dt.s));
				return o;
			}
		case COMBINED_TYPE_IA:
		case COMBINED_TYPE_FA:
		case COMBINED_TYPE_CA:
			{
				sll_object_t* o=SLL_CREATE();
				o->t=SLL_OBJECT_TYPE_ARRAY;
				sll_array_create(b->dt.a.l,&(o->dt.a));
				for (sll_array_length_t i=0;i<b->dt.a.l;i++){
					o->dt.a.v[i]=sll_operator_or(a,b->dt.a.v[i]);
				}
				return o;
			}
		case COMBINED_TYPE_IM:
		case COMBINED_TYPE_FM:
		case COMBINED_TYPE_CM:
			{
				sll_object_t* o=SLL_CREATE();
				o->t=SLL_OBJECT_TYPE_MAP;
				sll_map_create(b->dt.m.l,&(o->dt.m));
				for (sll_map_length_t i=0;i<(b->dt.a.l<<1);i+=2){
					o->dt.m.v[i]=a->dt.m.v[i];
					SLL_ACQUIRE(a->dt.m.v[i]);
					o->dt.m.v[i+1]=sll_operator_or(a,b->dt.m.v[i+1]);
				}
				return o;
			}
		case COMBINED_TYPE_FF:
			{
				float_data_t v={
					.dt=TO_FLOAT_BITS(a)|TO_FLOAT_BITS(b)
				};
				return SLL_FROM_FLOAT(v.v);
			}
		case COMBINED_TYPE_FC:
			{
				float_data_t v={
					.dt=TO_FLOAT_BITS(a)|b->dt.c
				};
				return SLL_FROM_FLOAT(v.v);
			}
		case COMBINED_TYPE_FS:
			{
				sll_object_t* o=SLL_CREATE();
				o->t=SLL_OBJECT_TYPE_STRING;
				sll_string_or_char(&(b->dt.s),(sll_char_t)sll_api_math_round(a->dt.f),&(o->dt.s));
				return o;
			}
		case COMBINED_TYPE_CC:
			return SLL_FROM_CHAR(a->dt.c|b->dt.c);
		case COMBINED_TYPE_CS:
			{
				sll_object_t* o=SLL_CREATE();
				o->t=SLL_OBJECT_TYPE_STRING;
				sll_string_or_char(&(b->dt.s),a->dt.c,&(o->dt.s));
				return o;
			}
		case COMBINED_TYPE_SS:
			{
				sll_object_t* o=SLL_CREATE();
				o->t=SLL_OBJECT_TYPE_STRING;
				sll_string_or(&(a->dt.s),&(b->dt.s),&(o->dt.s));
				return o;
			}
		case COMBINED_TYPE_SA:
			{
				sll_object_t* o=SLL_CREATE();
				o->t=SLL_OBJECT_TYPE_ARRAY;
				sll_string_op_array(&(a->dt.s),&(b->dt.a),sll_operator_or,inv,&(o->dt.a));
				return o;
			}
		case COMBINED_TYPE_SM:
			{
				sll_object_t* o=SLL_CREATE();
				o->t=SLL_OBJECT_TYPE_MAP;
				sll_map_add_string(&(b->dt.m),&(a->dt.s),&(o->dt.m));
				return o;
			}
		case COMBINED_TYPE_AA:
			{
				sll_object_t* o=SLL_CREATE();
				o->t=SLL_OBJECT_TYPE_ARRAY;
				sll_array_or(&(a->dt.a),&(b->dt.a),&(o->dt.a));
				return o;
			}
		case COMBINED_TYPE_AM:
			{
				sll_object_t* o=SLL_CREATE();
				o->t=SLL_OBJECT_TYPE_MAP;
				sll_map_add_array(&(b->dt.m),&(a->dt.a),&(o->dt.m));
				return o;
			}
		case COMBINED_TYPE_MM:
			{
				sll_object_t* o=SLL_CREATE();
				o->t=SLL_OBJECT_TYPE_MAP;
				sll_map_join(&(a->dt.m),&(b->dt.m),&(o->dt.m));
				return o;
			}
		default:
			SLL_UNREACHABLE();
	}
	return SLL_ACQUIRE_STATIC_INT(0);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_operator_shl(sll_object_t* a,sll_object_t* b){
	switch (COMBINED_ARGS){
		case COMBINED_TYPE_II:
			return SLL_FROM_INT(a->dt.i<<b->dt.i);
		case COMBINED_TYPE_IF:
			return SLL_FROM_FLOAT(a->dt.i*sll_api_math_pow(2,b->dt.f));
		case COMBINED_TYPE_IC:
			return SLL_FROM_INT(a->dt.i<<b->dt.c);
		case COMBINED_TYPE_IS:
		case COMBINED_TYPE_FS:
		case COMBINED_TYPE_CS:
			{
				sll_object_t* o=SLL_CREATE();
				o->t=SLL_OBJECT_TYPE_ARRAY;
				sll_array_create(b->dt.s.l,&(o->dt.a));
				for (sll_string_length_t i=0;i<b->dt.s.l;i++){
					o->dt.a.v[i]=sll_operator_shl(a,sll_static_char[b->dt.s.v[i]]);
				}
				return o;
			}
		case COMBINED_TYPE_IA:
		case COMBINED_TYPE_FA:
		case COMBINED_TYPE_CA:
			{
				sll_object_t* o=SLL_CREATE();
				o->t=SLL_OBJECT_TYPE_ARRAY;
				sll_array_create(b->dt.a.l,&(o->dt.a));
				for (sll_array_length_t i=0;i<b->dt.a.l;i++){
					o->dt.a.v[i]=sll_operator_shl(a,b->dt.a.v[i]);
				}
				return o;
			}
		case COMBINED_TYPE_IM:
		case COMBINED_TYPE_FM:
		case COMBINED_TYPE_CM:
			{
				sll_object_t* o=SLL_CREATE();
				o->t=SLL_OBJECT_TYPE_MAP;
				sll_map_create(b->dt.m.l,&(o->dt.m));
				for (sll_map_length_t i=0;i<(b->dt.m.l<<1);i+=2){
					o->dt.m.v[i]=b->dt.m.v[i];
					SLL_ACQUIRE(o->dt.m.v[i]);
					o->dt.m.v[i+1]=sll_operator_shl(a,b->dt.m.v[i+1]);
				}
				return o;
			}
		case COMBINED_TYPE_FI:
			return SLL_FROM_FLOAT(a->dt.f*sll_api_math_pow(2,(sll_float_t)(b->dt.i)));
		case COMBINED_TYPE_FF:
			return SLL_FROM_FLOAT(a->dt.f*sll_api_math_pow(2,b->dt.f));
		case COMBINED_TYPE_FC:
			return SLL_FROM_FLOAT(a->dt.f*sll_api_math_pow(2,b->dt.c));
		case COMBINED_TYPE_CI:
			return SLL_FROM_INT(((sll_integer_t)a->dt.c)<<b->dt.i);
		case COMBINED_TYPE_CF:
			return SLL_FROM_FLOAT(a->dt.c*sll_api_math_pow(2,b->dt.f));
		case COMBINED_TYPE_CC:
			return SLL_FROM_CHAR((((sll_integer_t)a->dt.c))<<b->dt.c);
		case COMBINED_TYPE_SI:
			{
				sll_object_t* o=SLL_CREATE();
				o->t=SLL_OBJECT_TYPE_STRING;
				sll_string_shift(&(a->dt.s),b->dt.i,&(o->dt.s));
				return o;
			}
		case COMBINED_TYPE_SF:
			{
				sll_object_t* o=SLL_CREATE();
				o->t=SLL_OBJECT_TYPE_STRING;
				sll_string_shift(&(a->dt.s),sll_api_math_round(a->dt.f),&(o->dt.s));
				return o;
			}
		case COMBINED_TYPE_SC:
			{
				sll_object_t* o=SLL_CREATE();
				o->t=SLL_OBJECT_TYPE_STRING;
				sll_string_shift(&(a->dt.s),b->dt.c,&(o->dt.s));
				return o;
			}
		case COMBINED_TYPE_SS:
			{
				sll_object_t* o=SLL_CREATE();
				o->t=SLL_OBJECT_TYPE_STRING;
				sll_string_op(&(a->dt.s),&(b->dt.s),sll_operator_shl,&(o->dt.s));
				return o;
			}
		case COMBINED_TYPE_SA:
			{
				sll_object_t* o=SLL_CREATE();
				o->t=SLL_OBJECT_TYPE_ARRAY;
				sll_string_op_array(&(a->dt.s),&(b->dt.a),sll_operator_shl,0,&(o->dt.a));
				return o;
			}
		case COMBINED_TYPE_SM:
			{
				sll_object_t* o=SLL_CREATE();
				o->t=SLL_OBJECT_TYPE_MAP;
				sll_string_op_map(&(a->dt.s),&(b->dt.m),sll_operator_shl,0,&(o->dt.m));
				return o;
			}
		case COMBINED_TYPE_AI:
			{
				sll_object_t* o=SLL_CREATE();
				o->t=SLL_OBJECT_TYPE_ARRAY;
				sll_array_resize(&(a->dt.a),b->dt.i,&(o->dt.a));
				return o;
			}
		case COMBINED_TYPE_AF:
			{
				sll_object_t* o=SLL_CREATE();
				o->t=SLL_OBJECT_TYPE_ARRAY;
				sll_array_resize(&(a->dt.a),sll_api_math_round(a->dt.f),&(o->dt.a));
				return o;
			}
		case COMBINED_TYPE_AC:
			{
				sll_object_t* o=SLL_CREATE();
				o->t=SLL_OBJECT_TYPE_ARRAY;
				sll_array_resize(&(a->dt.a),b->dt.c,&(o->dt.a));
				return o;
			}
		case COMBINED_TYPE_AS:
			{
				sll_object_t* o=SLL_CREATE();
				o->t=SLL_OBJECT_TYPE_ARRAY;
				sll_string_op_array(&(b->dt.s),&(a->dt.a),sll_operator_shl,1,&(o->dt.a));
				return o;
			}
		case COMBINED_TYPE_AA:
			{
				sll_object_t* o=SLL_CREATE();
				o->t=SLL_OBJECT_TYPE_ARRAY;
				sll_array_op(&(a->dt.a),&(b->dt.a),sll_operator_shl,&(o->dt.a));
				return o;
			}
		case COMBINED_TYPE_AM:
			{
				sll_object_t* o=SLL_CREATE();
				o->t=SLL_OBJECT_TYPE_MAP;
				sll_array_op_map(&(a->dt.a),&(b->dt.m),sll_operator_shl,0,&(o->dt.m));
				return o;
			}
		case COMBINED_TYPE_MI:
		case COMBINED_TYPE_MF:
		case COMBINED_TYPE_MC:
			{
				sll_object_t* o=SLL_CREATE();
				o->t=SLL_OBJECT_TYPE_MAP;
				sll_map_create(a->dt.m.l,&(o->dt.m));
				for (sll_map_length_t i=0;i<(a->dt.m.l<<1);i+=2){
					o->dt.m.v[i]=a->dt.m.v[i];
					SLL_ACQUIRE(o->dt.m.v[i]);
					o->dt.m.v[i+1]=sll_operator_shl(a->dt.m.v[i+1],b);
				}
				return o;
			}
		case COMBINED_TYPE_MS:
			{
				sll_object_t* o=SLL_CREATE();
				o->t=SLL_OBJECT_TYPE_MAP;
				sll_string_op_map(&(b->dt.s),&(a->dt.m),sll_operator_shl,1,&(o->dt.m));
				return o;
			}
		case COMBINED_TYPE_MA:
			{
				sll_object_t* o=SLL_CREATE();
				o->t=SLL_OBJECT_TYPE_MAP;
				sll_array_op_map(&(b->dt.a),&(a->dt.m),sll_operator_shl,1,&(o->dt.m));
				return o;
			}
		case COMBINED_TYPE_MM:
			{
				sll_object_t* o=SLL_CREATE();
				o->t=SLL_OBJECT_TYPE_MAP;
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
			return SLL_FROM_INT(a->dt.i>>b->dt.i);
		case COMBINED_TYPE_IF:
			return SLL_FROM_FLOAT(a->dt.i/sll_api_math_pow(2,b->dt.f));
		case COMBINED_TYPE_IC:
			return SLL_FROM_INT(a->dt.i>>b->dt.c);
		case COMBINED_TYPE_IS:
		case COMBINED_TYPE_FS:
		case COMBINED_TYPE_CS:
			{
				sll_object_t* o=SLL_CREATE();
				o->t=SLL_OBJECT_TYPE_ARRAY;
				sll_array_create(b->dt.s.l,&(o->dt.a));
				for (sll_string_length_t i=0;i<b->dt.s.l;i++){
					o->dt.a.v[i]=sll_operator_shr(a,sll_static_char[b->dt.s.v[i]]);
				}
				return o;
			}
		case COMBINED_TYPE_IA:
		case COMBINED_TYPE_FA:
		case COMBINED_TYPE_CA:
			{
				sll_object_t* o=SLL_CREATE();
				o->t=SLL_OBJECT_TYPE_ARRAY;
				sll_array_create(b->dt.a.l,&(o->dt.a));
				for (sll_array_length_t i=0;i<b->dt.a.l;i++){
					o->dt.a.v[i]=sll_operator_shr(a,b->dt.a.v[i]);
				}
				return o;
			}
		case COMBINED_TYPE_IM:
		case COMBINED_TYPE_FM:
		case COMBINED_TYPE_CM:
			{
				sll_object_t* o=SLL_CREATE();
				o->t=SLL_OBJECT_TYPE_MAP;
				sll_map_create(b->dt.m.l,&(o->dt.m));
				for (sll_map_length_t i=0;i<(b->dt.m.l<<1);i+=2){
					o->dt.m.v[i]=b->dt.m.v[i];
					SLL_ACQUIRE(o->dt.m.v[i]);
					o->dt.m.v[i+1]=sll_operator_shr(a,b->dt.m.v[i+1]);
				}
				return o;
			}
		case COMBINED_TYPE_FI:
			return SLL_FROM_FLOAT(a->dt.f/sll_api_math_pow(2,(sll_float_t)(b->dt.i)));
		case COMBINED_TYPE_FF:
			return SLL_FROM_FLOAT(a->dt.f/sll_api_math_pow(2,b->dt.f));
		case COMBINED_TYPE_FC:
			return SLL_FROM_FLOAT(a->dt.f/sll_api_math_pow(2,b->dt.c));
		case COMBINED_TYPE_CI:
			return SLL_FROM_INT(a->dt.c>>b->dt.i);
		case COMBINED_TYPE_CF:
			return SLL_FROM_FLOAT(a->dt.c/sll_api_math_pow(2,b->dt.f));
		case COMBINED_TYPE_CC:
			return SLL_FROM_CHAR(a->dt.c>>b->dt.c);
		case COMBINED_TYPE_SI:
			{
				sll_object_t* o=SLL_CREATE();
				o->t=SLL_OBJECT_TYPE_STRING;
				sll_string_shift(&(a->dt.s),-b->dt.i,&(o->dt.s));
				return o;
			}
		case COMBINED_TYPE_SF:
			{
				sll_object_t* o=SLL_CREATE();
				o->t=SLL_OBJECT_TYPE_STRING;
				sll_string_shift(&(a->dt.s),-sll_api_math_round(a->dt.f),&(o->dt.s));
				return o;
			}
		case COMBINED_TYPE_SC:
			{
				sll_object_t* o=SLL_CREATE();
				o->t=SLL_OBJECT_TYPE_STRING;
				sll_string_shift(&(a->dt.s),-b->dt.c,&(o->dt.s));
				return o;
			}
		case COMBINED_TYPE_SS:
			{
				sll_object_t* o=SLL_CREATE();
				o->t=SLL_OBJECT_TYPE_STRING;
				sll_string_op(&(a->dt.s),&(b->dt.s),sll_operator_shr,&(o->dt.s));
				return o;
			}
		case COMBINED_TYPE_SA:
			{
				sll_object_t* o=SLL_CREATE();
				o->t=SLL_OBJECT_TYPE_ARRAY;
				sll_string_op_array(&(a->dt.s),&(b->dt.a),sll_operator_shr,0,&(o->dt.a));
				return o;
			}
		case COMBINED_TYPE_SM:
			{
				sll_object_t* o=SLL_CREATE();
				o->t=SLL_OBJECT_TYPE_MAP;
				sll_string_op_map(&(a->dt.s),&(b->dt.m),sll_operator_shr,0,&(o->dt.m));
				return o;
			}
		case COMBINED_TYPE_AI:
			{
				sll_object_t* o=SLL_CREATE();
				o->t=SLL_OBJECT_TYPE_ARRAY;
				sll_array_resize(&(a->dt.a),-b->dt.i,&(o->dt.a));
				return o;
			}
		case COMBINED_TYPE_AF:
			{
				sll_object_t* o=SLL_CREATE();
				o->t=SLL_OBJECT_TYPE_ARRAY;
				sll_array_resize(&(a->dt.a),-sll_api_math_round(a->dt.f),&(o->dt.a));
				return o;
			}
		case COMBINED_TYPE_AC:
			{
				sll_object_t* o=SLL_CREATE();
				o->t=SLL_OBJECT_TYPE_ARRAY;
				sll_array_resize(&(a->dt.a),-b->dt.c,&(o->dt.a));
				return o;
			}
		case COMBINED_TYPE_AS:
			{
				sll_object_t* o=SLL_CREATE();
				o->t=SLL_OBJECT_TYPE_ARRAY;
				sll_string_op_array(&(b->dt.s),&(a->dt.a),sll_operator_shr,1,&(o->dt.a));
				return o;
			}
		case COMBINED_TYPE_AA:
			{
				sll_object_t* o=SLL_CREATE();
				o->t=SLL_OBJECT_TYPE_ARRAY;
				sll_array_op(&(a->dt.a),&(b->dt.a),sll_operator_shr,&(o->dt.a));
				return o;
			}
		case COMBINED_TYPE_AM:
			{
				sll_object_t* o=SLL_CREATE();
				o->t=SLL_OBJECT_TYPE_MAP;
				sll_array_op_map(&(a->dt.a),&(b->dt.m),sll_operator_shr,0,&(o->dt.m));
				return o;
			}
		case COMBINED_TYPE_MI:
		case COMBINED_TYPE_MF:
		case COMBINED_TYPE_MC:
			{
				sll_object_t* o=SLL_CREATE();
				o->t=SLL_OBJECT_TYPE_MAP;
				sll_map_create(a->dt.m.l,&(o->dt.m));
				for (sll_map_length_t i=0;i<(a->dt.m.l<<1);i+=2){
					o->dt.m.v[i]=a->dt.m.v[i];
					SLL_ACQUIRE(o->dt.m.v[i]);
					o->dt.m.v[i+1]=sll_operator_shr(a->dt.m.v[i+1],b);
				}
				return o;
			}
		case COMBINED_TYPE_MS:
			{
				sll_object_t* o=SLL_CREATE();
				o->t=SLL_OBJECT_TYPE_MAP;
				sll_string_op_map(&(b->dt.s),&(a->dt.m),sll_operator_shr,1,&(o->dt.m));
				return o;
			}
		case COMBINED_TYPE_MA:
			{
				sll_object_t* o=SLL_CREATE();
				o->t=SLL_OBJECT_TYPE_MAP;
				sll_array_op_map(&(b->dt.a),&(a->dt.m),sll_operator_shr,1,&(o->dt.m));
				return o;
			}
		case COMBINED_TYPE_MM:
			{
				sll_object_t* o=SLL_CREATE();
				o->t=SLL_OBJECT_TYPE_MAP;
				sll_map_op(&(a->dt.m),&(b->dt.m),sll_operator_shr,&(o->dt.m));
				return o;
			}
		default:
			SLL_UNREACHABLE();
	}
	return SLL_ACQUIRE_STATIC_INT(0);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_operator_strict_equal(const sll_object_t* a,const sll_object_t* b){
	if (SLL_OBJECT_GET_TYPE(a)!=SLL_OBJECT_GET_TYPE(b)){
		return 0;
	}
	switch (SLL_OBJECT_GET_TYPE(a)){
		case SLL_OBJECT_TYPE_INT:
			return a->dt.i==b->dt.i;
		case SLL_OBJECT_TYPE_FLOAT:
			return a->dt.f==b->dt.f;
		case SLL_OBJECT_TYPE_CHAR:
			return a->dt.c==b->dt.c;
		case SLL_OBJECT_TYPE_STRING:
			return sll_string_equal(&(a->dt.s),&(b->dt.s));
		case SLL_OBJECT_TYPE_ARRAY:
			return sll_array_equal(&(a->dt.a),&(b->dt.a));
		case SLL_OBJECT_TYPE_MAP:
			return sll_map_equal(&(a->dt.m),&(b->dt.m));
		default:
			SLL_UNREACHABLE();
	}
	return 0;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_operator_sub(sll_object_t* a,sll_object_t* b){
	if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_ARRAY){
		sll_object_t* o=SLL_CREATE();
		o->t=SLL_OBJECT_TYPE_ARRAY;
		if (SLL_OBJECT_GET_TYPE(b)==SLL_OBJECT_TYPE_ARRAY){
			sll_array_remove_multiple(&(a->dt.a),b->dt.a.v,b->dt.a.l,&(o->dt.a));
		}
		else{
			sll_array_remove(&(a->dt.a),b,&(o->dt.a));
		}
		return o;
	}
	if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_MAP){
		sll_object_t* o=SLL_CREATE();
		o->t=SLL_OBJECT_TYPE_MAP;
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
		case COMBINED_TYPE_IS:
		case COMBINED_TYPE_FS:
		case COMBINED_TYPE_CS:
			{
				sll_string_t as=b->dt.s;
				sll_object_t* o=SLL_CREATE();
				o->t=SLL_OBJECT_TYPE_ARRAY;
				sll_array_create(as.l,&(o->dt.a));
				for (sll_array_length_t i=0;i<as.l;i++){
					o->dt.a.v[i]=sll_operator_sub(a,sll_static_char[as.v[i]]);
				}
				return o;
			}
		case COMBINED_TYPE_IA:
		case COMBINED_TYPE_FA:
		case COMBINED_TYPE_CA:
			{
				sll_object_t* o=SLL_CREATE();
				o->t=SLL_OBJECT_TYPE_ARRAY;
				sll_array_create(b->dt.a.l,&(o->dt.a));
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
				sll_object_t* o=SLL_CREATE();
				o->t=SLL_OBJECT_TYPE_MAP;
				sll_map_create(m.l,&(o->dt.m));
				for (sll_array_length_t i=0;i<(m.l<<1);i+=2){
					SLL_ACQUIRE(m.v[i]);
					o->dt.m.v[i]=m.v[i];
					o->dt.m.v[i+1]=sll_operator_sub(a,m.v[i+1]);
				}
				return o;
			}
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
		case COMBINED_TYPE_SI:
		case COMBINED_TYPE_SF:
		case COMBINED_TYPE_SC:
			{
				sll_object_t* o=SLL_CREATE();
				o->t=SLL_OBJECT_TYPE_ARRAY;
				sll_array_create(a->dt.s.l,&(o->dt.a));
				for (sll_string_length_t i=0;i<a->dt.s.l;i++){
					o->dt.a.v[i]=sll_operator_sub(sll_static_char[a->dt.s.v[i]],b);
				}
				return o;
			}
		case COMBINED_TYPE_SS:
			{
				sll_object_t* o=SLL_CREATE();
				o->t=SLL_OBJECT_TYPE_STRING;
				sll_string_remove(&(a->dt.s),&(b->dt.s),&(o->dt.s));
				return o;
			}
		case COMBINED_TYPE_SA:
			{
				sll_object_t* o=SLL_CREATE();
				o->t=SLL_OBJECT_TYPE_ARRAY;
				sll_string_op_array(&(a->dt.s),&(b->dt.a),sll_operator_sub,0,&(o->dt.a));
				return o;
			}
		case COMBINED_TYPE_SM:
			{
				sll_object_t* o=SLL_CREATE();
				o->t=SLL_OBJECT_TYPE_MAP;
				sll_string_op_map(&(a->dt.s),&(b->dt.m),sll_operator_sub,0,&(o->dt.m));
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
			return SLL_FROM_INT(a->dt.i^b->dt.i);
		case COMBINED_TYPE_IF:
			{
				float_data_t v={
					.dt=a->dt.i^TO_FLOAT_BITS(b)
				};
				return SLL_FROM_FLOAT(v.v);
			}
		case COMBINED_TYPE_IC:
			return SLL_FROM_INT(a->dt.i^b->dt.c);
		case COMBINED_TYPE_IS:
			{
				sll_object_t* o=SLL_CREATE();
				o->t=SLL_OBJECT_TYPE_STRING;
				sll_string_xor_char(&(b->dt.s),(sll_char_t)(a->dt.i),&(o->dt.s));
				return o;
			}
		case COMBINED_TYPE_IA:
		case COMBINED_TYPE_FA:
		case COMBINED_TYPE_CA:
			{
				sll_object_t* o=SLL_CREATE();
				o->t=SLL_OBJECT_TYPE_ARRAY;
				sll_array_create(b->dt.a.l,&(o->dt.a));
				for (sll_array_length_t i=0;i<b->dt.a.l;i++){
					o->dt.a.v[i]=sll_operator_xor(a,b->dt.a.v[i]);
				}
				return o;
			}
		case COMBINED_TYPE_IM:
		case COMBINED_TYPE_FM:
		case COMBINED_TYPE_CM:
			{
				sll_object_t* o=SLL_CREATE();
				o->t=SLL_OBJECT_TYPE_MAP;
				sll_map_create(b->dt.m.l,&(o->dt.m));
				for (sll_map_length_t i=0;i<(b->dt.a.l<<1);i+=2){
					o->dt.m.v[i]=a->dt.m.v[i];
					SLL_ACQUIRE(a->dt.m.v[i]);
					o->dt.m.v[i+1]=sll_operator_xor(a,b->dt.m.v[i+1]);
				}
				return o;
			}
		case COMBINED_TYPE_FF:
			{
				float_data_t v={
					.dt=TO_FLOAT_BITS(a)^TO_FLOAT_BITS(b)
				};
				return SLL_FROM_FLOAT(v.v);
			}
		case COMBINED_TYPE_FC:
			{
				float_data_t v={
					.dt=TO_FLOAT_BITS(a)^b->dt.c
				};
				return SLL_FROM_FLOAT(v.v);
			}
		case COMBINED_TYPE_FS:
			{
				sll_object_t* o=SLL_CREATE();
				o->t=SLL_OBJECT_TYPE_STRING;
				sll_string_xor_char(&(b->dt.s),(sll_char_t)sll_api_math_round(a->dt.f),&(o->dt.s));
				return o;
			}
		case COMBINED_TYPE_CC:
			return SLL_FROM_CHAR(a->dt.c^b->dt.c);
		case COMBINED_TYPE_CS:
			{
				sll_object_t* o=SLL_CREATE();
				o->t=SLL_OBJECT_TYPE_STRING;
				sll_string_xor_char(&(b->dt.s),a->dt.c,&(o->dt.s));
				return o;
			}
		case COMBINED_TYPE_SS:
			{
				sll_object_t* o=SLL_CREATE();
				o->t=SLL_OBJECT_TYPE_STRING;
				sll_string_xor(&(a->dt.s),&(b->dt.s),&(o->dt.s));
				return o;
			}
		case COMBINED_TYPE_SA:
			{
				sll_object_t* o=SLL_CREATE();
				o->t=SLL_OBJECT_TYPE_ARRAY;
				sll_string_op_array(&(a->dt.s),&(b->dt.a),sll_operator_xor,inv,&(o->dt.a));
				return o;
			}
		case COMBINED_TYPE_SM:
			{
				sll_object_t* o=SLL_CREATE();
				o->t=SLL_OBJECT_TYPE_MAP;
				sll_map_add_string(&(b->dt.m),&(a->dt.s),&(o->dt.m));
				return o;
			}
		case COMBINED_TYPE_AA:
			{
				sll_object_t* o=SLL_CREATE();
				o->t=SLL_OBJECT_TYPE_ARRAY;
				sll_array_xor(&(a->dt.a),&(b->dt.a),&(o->dt.a));
				return o;
			}
		case COMBINED_TYPE_AM:
			{
				sll_object_t* o=SLL_CREATE();
				o->t=SLL_OBJECT_TYPE_MAP;
				sll_map_add_array(&(b->dt.m),&(a->dt.a),&(o->dt.m));
				return o;
			}
		case COMBINED_TYPE_MM:
			{
				sll_object_t* o=SLL_CREATE();
				o->t=SLL_OBJECT_TYPE_MAP;
				sll_map_join(&(a->dt.m),&(b->dt.m),&(o->dt.m));
				return o;
			}
		default:
			SLL_UNREACHABLE();
	}
	return SLL_ACQUIRE_STATIC_INT(0);
}
