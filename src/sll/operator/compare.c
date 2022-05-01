#include <sll/_internal/common.h>
#include <sll/_internal/complex.h>
#include <sll/_internal/operator.h>
#include <sll/api/math.h>
#include <sll/array.h>
#include <sll/common.h>
#include <sll/data.h>
#include <sll/gc.h>
#include <sll/map.h>
#include <sll/object.h>
#include <sll/string.h>
#include <sll/types.h>



#define COMPARE_RESULT(a,b) ((a)<(b)?SLL_COMPARE_RESULT_BELOW:((a)>(b)?SLL_COMPARE_RESULT_ABOVE:SLL_COMPARE_RESULT_EQUAL))
#define COMPARE_RESULT_FLOAT(a,b) (EQUAL_FLOAT((a),(b))?SLL_COMPARE_RESULT_EQUAL:((a)>(b)?SLL_COMPARE_RESULT_ABOVE:SLL_COMPARE_RESULT_BELOW))



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_compare_result_t sll_operator_compare(sll_object_t* a,sll_object_t* b){
	switch (COMBINED_ARGS){
		case COMBINED_TYPE_II:
			return COMPARE_RESULT(a->dt.i,b->dt.i);
		case COMBINED_TYPE_IF:
			return COMPARE_RESULT_FLOAT(a->dt.i,b->dt.f);
		case COMBINED_TYPE_IC:
			return COMPARE_RESULT(a->dt.i,b->dt.c);
		case COMBINED_TYPE_ID:
			return COMPARE_RESULT_FLOAT(a->dt.i,COMPLEX_ABS(b->dt.d));
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
		case COMBINED_TYPE_FD:
			return COMPARE_RESULT_FLOAT(a->dt.f,COMPLEX_ABS(b->dt.d));
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
		case COMBINED_TYPE_CD:
			return COMPARE_RESULT_FLOAT(a->dt.c,COMPLEX_ABS(b->dt.d));
		case COMBINED_TYPE_CS:
			if (!b->dt.s.l){
				return SLL_COMPARE_RESULT_ABOVE;
			}
			if (b->dt.s.v[0]!=a->dt.c){
				return COMPARE_RESULT(a->dt.c,b->dt.s.v[0]);
			}
			return (b->dt.s.l==1?SLL_COMPARE_RESULT_EQUAL:SLL_COMPARE_RESULT_BELOW);
		case COMBINED_TYPE_DI:
			return COMPARE_RESULT_FLOAT(COMPLEX_ABS(a->dt.d),b->dt.i);
		case COMBINED_TYPE_DF:
			return COMPARE_RESULT_FLOAT(COMPLEX_ABS(a->dt.d),b->dt.f);
		case COMBINED_TYPE_DC:
			return COMPARE_RESULT_FLOAT(COMPLEX_ABS(a->dt.d),b->dt.c);
		case COMBINED_TYPE_DD:
			return COMPARE_RESULT_FLOAT(COMPLEX_ABS(a->dt.d),COMPLEX_ABS(b->dt.d));
		case COMBINED_TYPE_DS:
			return COMPARE_RESULT_FLOAT(COMPLEX_ABS(a->dt.d),b->dt.s.l);
		case COMBINED_TYPE_DA:
			return COMPARE_RESULT_FLOAT(COMPLEX_ABS(a->dt.d),b->dt.a.l);
		case COMBINED_TYPE_DM:
			return COMPARE_RESULT_FLOAT(COMPLEX_ABS(a->dt.d),b->dt.m.l);
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
		case COMBINED_TYPE_SD:
			return COMPARE_RESULT_FLOAT(a->dt.s.l,COMPLEX_ABS(b->dt.d));
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
		case COMBINED_TYPE_AD:
			return COMPARE_RESULT_FLOAT(a->dt.a.l,COMPLEX_ABS(b->dt.d));
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
		case COMBINED_TYPE_MD:
			return COMPARE_RESULT_FLOAT(a->dt.m.l,COMPLEX_ABS(b->dt.d));
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
