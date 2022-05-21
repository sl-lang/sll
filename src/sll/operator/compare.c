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



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_compare_result_t sll_operator_compare(sll_object_t* a,sll_object_t* b){
	switch (COMBINED_ARGS){
		case COMBINED_TYPE_II:
			return COMPARE_RESULT(a->dt.int_value,b->dt.int_value);
		case COMBINED_TYPE_IF:
			return COMPARE_RESULT(a->dt.int_value,b->dt.float_value);
		case COMBINED_TYPE_IC:
			return COMPARE_RESULT(a->dt.int_value,b->dt.char_value);
		case COMBINED_TYPE_ID:
			return COMPARE_RESULT(a->dt.int_value,COMPLEX_ABS(b->dt.complex_value));
		case COMBINED_TYPE_IS:
			return COMPARE_RESULT(a->dt.int_value,b->dt.s.l);
		case COMBINED_TYPE_IA:
			return COMPARE_RESULT(a->dt.int_value,b->dt.a.length);
		case COMBINED_TYPE_IM:
			return COMPARE_RESULT(a->dt.int_value,b->dt.m.length);
		case COMBINED_TYPE_FI:
			return COMPARE_RESULT(a->dt.float_value,b->dt.int_value);
		case COMBINED_TYPE_FF:
			return COMPARE_RESULT(a->dt.float_value,b->dt.float_value);
		case COMBINED_TYPE_FC:
			return COMPARE_RESULT(a->dt.float_value,b->dt.char_value);
		case COMBINED_TYPE_FD:
			return COMPARE_RESULT(a->dt.float_value,COMPLEX_ABS(b->dt.complex_value));
		case COMBINED_TYPE_FS:
			return COMPARE_RESULT(a->dt.float_value,b->dt.s.l);
		case COMBINED_TYPE_FA:
			return COMPARE_RESULT(a->dt.float_value,b->dt.a.length);
		case COMBINED_TYPE_FM:
			return COMPARE_RESULT(a->dt.float_value,b->dt.m.length);
		case COMBINED_TYPE_CI:
			return COMPARE_RESULT(a->dt.char_value,b->dt.int_value);
		case COMBINED_TYPE_CF:
			return COMPARE_RESULT(a->dt.char_value,b->dt.float_value);
		case COMBINED_TYPE_CC:
			return COMPARE_RESULT(a->dt.char_value,b->dt.char_value);
		case COMBINED_TYPE_CD:
			return COMPARE_RESULT(a->dt.char_value,COMPLEX_ABS(b->dt.complex_value));
		case COMBINED_TYPE_CS:
			if (!b->dt.s.l){
				return SLL_COMPARE_RESULT_ABOVE;
			}
			if (b->dt.s.v[0]!=a->dt.char_value){
				return COMPARE_RESULT(a->dt.char_value,b->dt.s.v[0]);
			}
			return (b->dt.s.l==1?SLL_COMPARE_RESULT_EQUAL:SLL_COMPARE_RESULT_BELOW);
		case COMBINED_TYPE_DI:
			return COMPARE_RESULT(COMPLEX_ABS(a->dt.complex_value),b->dt.int_value);
		case COMBINED_TYPE_DF:
			return COMPARE_RESULT(COMPLEX_ABS(a->dt.complex_value),b->dt.float_value);
		case COMBINED_TYPE_DC:
			return COMPARE_RESULT(COMPLEX_ABS(a->dt.complex_value),b->dt.char_value);
		case COMBINED_TYPE_DD:
			return COMPARE_RESULT(COMPLEX_ABS(a->dt.complex_value),COMPLEX_ABS(b->dt.complex_value));
		case COMBINED_TYPE_DS:
			return COMPARE_RESULT(COMPLEX_ABS(a->dt.complex_value),b->dt.s.l);
		case COMBINED_TYPE_DA:
			return COMPARE_RESULT(COMPLEX_ABS(a->dt.complex_value),b->dt.a.length);
		case COMBINED_TYPE_DM:
			return COMPARE_RESULT(COMPLEX_ABS(a->dt.complex_value),b->dt.m.length);
		case COMBINED_TYPE_CA:
			return COMPARE_RESULT(a->dt.char_value,b->dt.a.length);
		case COMBINED_TYPE_CM:
			return COMPARE_RESULT(a->dt.char_value,b->dt.m.length);
		case COMBINED_TYPE_SI:
			return COMPARE_RESULT(a->dt.s.l,b->dt.int_value);
		case COMBINED_TYPE_SF:
			return COMPARE_RESULT(a->dt.s.l,b->dt.float_value);
		case COMBINED_TYPE_SC:
			if (!a->dt.s.l){
				return SLL_COMPARE_RESULT_BELOW;
			}
			if (a->dt.s.v[0]!=b->dt.char_value){
				return COMPARE_RESULT(b->dt.char_value,a->dt.s.v[0]);
			}
			return (a->dt.s.l==1?SLL_COMPARE_RESULT_EQUAL:SLL_COMPARE_RESULT_ABOVE);
		case COMBINED_TYPE_SD:
			return COMPARE_RESULT(a->dt.s.l,COMPLEX_ABS(b->dt.complex_value));
		case COMBINED_TYPE_SS:
			return sll_string_compare(&(a->dt.s),&(b->dt.s));
		case COMBINED_TYPE_SA:
			return sll_string_compare_array(&(a->dt.s),&(b->dt.a),0);
		case COMBINED_TYPE_SM:
			return sll_string_compare_map(&(a->dt.s),&(b->dt.m),0);
		case COMBINED_TYPE_AI:
			return COMPARE_RESULT(a->dt.a.length,b->dt.int_value);
		case COMBINED_TYPE_AF:
			return COMPARE_RESULT(a->dt.a.length,b->dt.float_value);
		case COMBINED_TYPE_AC:
			return COMPARE_RESULT(a->dt.a.length,b->dt.char_value);
		case COMBINED_TYPE_AD:
			return COMPARE_RESULT(a->dt.a.length,COMPLEX_ABS(b->dt.complex_value));
		case COMBINED_TYPE_AS:
			return sll_string_compare_array(&(b->dt.s),&(a->dt.a),1);
		case COMBINED_TYPE_AA:
			return sll_array_compare(&(a->dt.a),&(b->dt.a));
		case COMBINED_TYPE_AM:
			return sll_array_compare_map(&(a->dt.a),&(b->dt.m),0);
		case COMBINED_TYPE_MI:
			return COMPARE_RESULT(a->dt.m.length,b->dt.int_value);
		case COMBINED_TYPE_MF:
			return COMPARE_RESULT(a->dt.m.length,b->dt.float_value);
		case COMBINED_TYPE_MC:
			return COMPARE_RESULT(a->dt.m.length,b->dt.char_value);
		case COMBINED_TYPE_MD:
			return COMPARE_RESULT(a->dt.m.length,COMPLEX_ABS(b->dt.complex_value));
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
