#include <sll/_internal/operator.h>
#include <sll/_internal/string.h>
#include <sll/api/math.h>
#include <sll/array.h>
#include <sll/common.h>
#include <sll/gc.h>
#include <sll/map.h>
#include <sll/object.h>
#include <sll/string.h>
#include <sll/types.h>
#include <sll/vm.h>



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_operator_equal(sll_object_t* a,sll_object_t* b){
	switch (COMBINED_ARGS){
		case COMBINED_TYPE_II:
			return a->dt.i==b->dt.i;
		case COMBINED_TYPE_IF:
			return EQUAL_FLOAT(a->dt.i,b->dt.f);
		case COMBINED_TYPE_IC:
			return a->dt.i==b->dt.c;
		case COMBINED_TYPE_ID:
			return (!b->dt.d.imag&&EQUAL_FLOAT(a->dt.i,b->dt.d.real));
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
		case COMBINED_TYPE_FD:
			return (!b->dt.d.imag&&EQUAL_FLOAT(a->dt.f,b->dt.d.real));
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
		case COMBINED_TYPE_CD:
			return (!b->dt.d.imag&&EQUAL_FLOAT(a->dt.c,b->dt.d.real));
		case COMBINED_TYPE_CS:
			return (b->dt.s.l==1&&b->dt.s.v[0]==a->dt.c);
		case COMBINED_TYPE_CA:
			return a->dt.c==b->dt.a.l;
		case COMBINED_TYPE_CM:
			return a->dt.c==b->dt.m.l;
		case COMBINED_TYPE_DI:
			return (!a->dt.d.imag&&EQUAL_FLOAT(a->dt.d.real,b->dt.i));
		case COMBINED_TYPE_DF:
			return (!a->dt.d.imag&&EQUAL_FLOAT(a->dt.d.real,b->dt.f));
		case COMBINED_TYPE_DC:
			return (!a->dt.d.imag&&EQUAL_FLOAT(a->dt.d.real,b->dt.c));
		case COMBINED_TYPE_DD:
			return (a->dt.d.real==b->dt.d.real&&a->dt.d.imag==b->dt.d.imag);
		case COMBINED_TYPE_DS:
			return (!a->dt.d.imag&&EQUAL_FLOAT(a->dt.d.real,b->dt.s.l));
		case COMBINED_TYPE_DA:
			return (!a->dt.d.imag&&EQUAL_FLOAT(a->dt.d.real,b->dt.a.l));
		case COMBINED_TYPE_DM:
			return (!a->dt.d.imag&&EQUAL_FLOAT(a->dt.d.real,b->dt.m.l));
		case COMBINED_TYPE_SI:
			return a->dt.s.l==b->dt.i;
		case COMBINED_TYPE_SF:
			return EQUAL_FLOAT(a->dt.s.l,b->dt.f);
		case COMBINED_TYPE_SC:
			return (a->dt.s.l==1&&a->dt.s.v[0]==b->dt.c);
		case COMBINED_TYPE_SD:
			return (!b->dt.d.imag&&EQUAL_FLOAT(a->dt.s.l,b->dt.d.real));
		case COMBINED_TYPE_SS:
			return STRING_EQUAL(&(a->dt.s),&(b->dt.s));
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
		case COMBINED_TYPE_AD:
			return (!b->dt.d.imag&&EQUAL_FLOAT(a->dt.a.l,b->dt.d.real));
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
		case COMBINED_TYPE_MD:
			return (!b->dt.d.imag&&EQUAL_FLOAT(a->dt.m.l,b->dt.d.real));
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



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_operator_strict_equal(sll_object_t* a,sll_object_t* b){
	if (a->t!=b->t){
		return 0;
	}
	switch (a->t){
		case SLL_OBJECT_TYPE_INT:
			return a->dt.i==b->dt.i;
		case SLL_OBJECT_TYPE_FLOAT:
			return a->dt.f==b->dt.f;
		case SLL_OBJECT_TYPE_CHAR:
			return a->dt.c==b->dt.c;
		case SLL_OBJECT_TYPE_STRING:
			return STRING_EQUAL(&(a->dt.s),&(b->dt.s));
		case SLL_OBJECT_TYPE_ARRAY:
			return sll_array_equal(&(a->dt.a),&(b->dt.a));
		case SLL_OBJECT_TYPE_MAP:
			return sll_map_equal(&(a->dt.m),&(b->dt.m));
	}
	if (sll_current_runtime_data&&a->t>SLL_MAX_OBJECT_TYPE&&a->t<=sll_current_runtime_data->tt->l+SLL_MAX_OBJECT_TYPE){
		const sll_object_type_data_t* dt=*(sll_current_runtime_data->tt->dt+a->t-SLL_MAX_OBJECT_TYPE-1);
		sll_object_field_t* pa=a->dt.p;
		sll_object_field_t* pb=b->dt.p;
		for (sll_arg_count_t i=0;i<dt->l;i++){
			switch (dt->dt[i].t){
				case SLL_OBJECT_TYPE_INT:
					if (pa->i!=pb->i){
						return 0;
					}
					break;
				case SLL_OBJECT_TYPE_FLOAT:
					if (pa->f!=pb->f){
						return 0;
					}
					break;
				case SLL_OBJECT_TYPE_CHAR:
					if (pa->c!=pb->c){
						return 0;
					}
					break;
				default:
					if (!sll_operator_strict_equal(pa->o,pb->o)){
						return 0;
					}
					break;
			}
			pa++;
			pb++;
		}
		return 1;
	}
	return 0;
}
