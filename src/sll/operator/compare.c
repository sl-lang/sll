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
			return COMPARE_RESULT(a->data.int_,b->data.int_);
		case COMBINED_TYPE_IF:
			return COMPARE_RESULT(a->data.int_,b->data.float_);
		case COMBINED_TYPE_IC:
			return COMPARE_RESULT(a->data.int_,b->data.char_);
		case COMBINED_TYPE_ID:
			return COMPARE_RESULT(a->data.int_,COMPLEX_ABS(b->data.complex_));
		case COMBINED_TYPE_IS:
			return COMPARE_RESULT(a->data.int_,b->data.string.l);
		case COMBINED_TYPE_IA:
			return COMPARE_RESULT(a->data.int_,b->data.array.length);
		case COMBINED_TYPE_IM:
			return COMPARE_RESULT(a->data.int_,b->data.map.length);
		case COMBINED_TYPE_FI:
			return COMPARE_RESULT(a->data.float_,b->data.int_);
		case COMBINED_TYPE_FF:
			return COMPARE_RESULT(a->data.float_,b->data.float_);
		case COMBINED_TYPE_FC:
			return COMPARE_RESULT(a->data.float_,b->data.char_);
		case COMBINED_TYPE_FD:
			return COMPARE_RESULT(a->data.float_,COMPLEX_ABS(b->data.complex_));
		case COMBINED_TYPE_FS:
			return COMPARE_RESULT(a->data.float_,b->data.string.l);
		case COMBINED_TYPE_FA:
			return COMPARE_RESULT(a->data.float_,b->data.array.length);
		case COMBINED_TYPE_FM:
			return COMPARE_RESULT(a->data.float_,b->data.map.length);
		case COMBINED_TYPE_CI:
			return COMPARE_RESULT(a->data.char_,b->data.int_);
		case COMBINED_TYPE_CF:
			return COMPARE_RESULT(a->data.char_,b->data.float_);
		case COMBINED_TYPE_CC:
			return COMPARE_RESULT(a->data.char_,b->data.char_);
		case COMBINED_TYPE_CD:
			return COMPARE_RESULT(a->data.char_,COMPLEX_ABS(b->data.complex_));
		case COMBINED_TYPE_CS:
			if (!b->data.string.l){
				return SLL_COMPARE_RESULT_ABOVE;
			}
			if (b->data.string.v[0]!=a->data.char_){
				return COMPARE_RESULT(a->data.char_,b->data.string.v[0]);
			}
			return (b->data.string.l==1?SLL_COMPARE_RESULT_EQUAL:SLL_COMPARE_RESULT_BELOW);
		case COMBINED_TYPE_DI:
			return COMPARE_RESULT(COMPLEX_ABS(a->data.complex_),b->data.int_);
		case COMBINED_TYPE_DF:
			return COMPARE_RESULT(COMPLEX_ABS(a->data.complex_),b->data.float_);
		case COMBINED_TYPE_DC:
			return COMPARE_RESULT(COMPLEX_ABS(a->data.complex_),b->data.char_);
		case COMBINED_TYPE_DD:
			return COMPARE_RESULT(COMPLEX_ABS(a->data.complex_),COMPLEX_ABS(b->data.complex_));
		case COMBINED_TYPE_DS:
			return COMPARE_RESULT(COMPLEX_ABS(a->data.complex_),b->data.string.l);
		case COMBINED_TYPE_DA:
			return COMPARE_RESULT(COMPLEX_ABS(a->data.complex_),b->data.array.length);
		case COMBINED_TYPE_DM:
			return COMPARE_RESULT(COMPLEX_ABS(a->data.complex_),b->data.map.length);
		case COMBINED_TYPE_CA:
			return COMPARE_RESULT(a->data.char_,b->data.array.length);
		case COMBINED_TYPE_CM:
			return COMPARE_RESULT(a->data.char_,b->data.map.length);
		case COMBINED_TYPE_SI:
			return COMPARE_RESULT(a->data.string.l,b->data.int_);
		case COMBINED_TYPE_SF:
			return COMPARE_RESULT(a->data.string.l,b->data.float_);
		case COMBINED_TYPE_SC:
			if (!a->data.string.l){
				return SLL_COMPARE_RESULT_BELOW;
			}
			if (a->data.string.v[0]!=b->data.char_){
				return COMPARE_RESULT(b->data.char_,a->data.string.v[0]);
			}
			return (a->data.string.l==1?SLL_COMPARE_RESULT_EQUAL:SLL_COMPARE_RESULT_ABOVE);
		case COMBINED_TYPE_SD:
			return COMPARE_RESULT(a->data.string.l,COMPLEX_ABS(b->data.complex_));
		case COMBINED_TYPE_SS:
			return sll_string_compare(&(a->data.string),&(b->data.string));
		case COMBINED_TYPE_SA:
			return sll_string_compare_array(&(a->data.string),&(b->data.array),0);
		case COMBINED_TYPE_SM:
			return sll_string_compare_map(&(a->data.string),&(b->data.map),0);
		case COMBINED_TYPE_AI:
			return COMPARE_RESULT(a->data.array.length,b->data.int_);
		case COMBINED_TYPE_AF:
			return COMPARE_RESULT(a->data.array.length,b->data.float_);
		case COMBINED_TYPE_AC:
			return COMPARE_RESULT(a->data.array.length,b->data.char_);
		case COMBINED_TYPE_AD:
			return COMPARE_RESULT(a->data.array.length,COMPLEX_ABS(b->data.complex_));
		case COMBINED_TYPE_AS:
			return sll_string_compare_array(&(b->data.string),&(a->data.array),1);
		case COMBINED_TYPE_AA:
			return sll_array_compare(&(a->data.array),&(b->data.array));
		case COMBINED_TYPE_AM:
			return sll_array_compare_map(&(a->data.array),&(b->data.map),0);
		case COMBINED_TYPE_MI:
			return COMPARE_RESULT(a->data.map.length,b->data.int_);
		case COMBINED_TYPE_MF:
			return COMPARE_RESULT(a->data.map.length,b->data.float_);
		case COMBINED_TYPE_MC:
			return COMPARE_RESULT(a->data.map.length,b->data.char_);
		case COMBINED_TYPE_MD:
			return COMPARE_RESULT(a->data.map.length,COMPLEX_ABS(b->data.complex_));
		case COMBINED_TYPE_MS:
			return sll_string_compare_map(&(b->data.string),&(a->data.map),1);
		case COMBINED_TYPE_MA:
			return sll_array_compare_map(&(b->data.array),&(a->data.map),1);
		case COMBINED_TYPE_MM:
			return sll_map_compare(&(a->data.map),&(b->data.map));
		default:
			SLL_UNREACHABLE();
	}
	return 0;
}
