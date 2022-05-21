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
			return a->dt.int_value==b->dt.int_value;
		case COMBINED_TYPE_IF:
			return a->dt.int_value==b->dt.float_value;
		case COMBINED_TYPE_IC:
			return a->dt.int_value==b->dt.char_value;
		case COMBINED_TYPE_ID:
			return (!b->dt.complex_value.imag&&a->dt.int_value==b->dt.complex_value.real);
		case COMBINED_TYPE_IS:
			return a->dt.int_value==b->dt.s.l;
		case COMBINED_TYPE_IA:
			return a->dt.int_value==b->dt.a.length;
		case COMBINED_TYPE_IM:
			return a->dt.int_value==b->dt.m.length;
		case COMBINED_TYPE_FI:
			return a->dt.float_value==b->dt.int_value;
		case COMBINED_TYPE_FF:
			return a->dt.float_value==b->dt.float_value;
		case COMBINED_TYPE_FC:
			return a->dt.float_value==b->dt.char_value;
		case COMBINED_TYPE_FD:
			return (!b->dt.complex_value.imag&&a->dt.float_value==b->dt.complex_value.real);
		case COMBINED_TYPE_FS:
			return a->dt.float_value==b->dt.s.l;
		case COMBINED_TYPE_FA:
			return a->dt.float_value==b->dt.a.length;
		case COMBINED_TYPE_FM:
			return a->dt.float_value==b->dt.m.length;
		case COMBINED_TYPE_CI:
			return a->dt.char_value==b->dt.int_value;
		case COMBINED_TYPE_CF:
			return a->dt.char_value==b->dt.float_value;
		case COMBINED_TYPE_CC:
			return a->dt.char_value==b->dt.char_value;
		case COMBINED_TYPE_CD:
			return (!b->dt.complex_value.imag&&a->dt.char_value==b->dt.complex_value.real);
		case COMBINED_TYPE_CS:
			return (b->dt.s.l==1&&b->dt.s.v[0]==a->dt.char_value);
		case COMBINED_TYPE_CA:
			return a->dt.char_value==b->dt.a.length;
		case COMBINED_TYPE_CM:
			return a->dt.char_value==b->dt.m.length;
		case COMBINED_TYPE_DI:
			return (!a->dt.complex_value.imag&&a->dt.complex_value.real==b->dt.int_value);
		case COMBINED_TYPE_DF:
			return (!a->dt.complex_value.imag&&a->dt.complex_value.real==b->dt.float_value);
		case COMBINED_TYPE_DC:
			return (!a->dt.complex_value.imag&&a->dt.complex_value.real==b->dt.char_value);
		case COMBINED_TYPE_DD:
			return (a->dt.complex_value.real==b->dt.complex_value.real&&a->dt.complex_value.imag==b->dt.complex_value.imag);
		case COMBINED_TYPE_DS:
			return (!a->dt.complex_value.imag&&a->dt.complex_value.real==b->dt.s.l);
		case COMBINED_TYPE_DA:
			return (!a->dt.complex_value.imag&&a->dt.complex_value.real==b->dt.a.length);
		case COMBINED_TYPE_DM:
			return (!a->dt.complex_value.imag&&a->dt.complex_value.real==b->dt.m.length);
		case COMBINED_TYPE_SI:
			return a->dt.s.l==b->dt.int_value;
		case COMBINED_TYPE_SF:
			return a->dt.s.l==b->dt.float_value;
		case COMBINED_TYPE_SC:
			return (a->dt.s.l==1&&a->dt.s.v[0]==b->dt.char_value);
		case COMBINED_TYPE_SD:
			return (!b->dt.complex_value.imag&&a->dt.s.l==b->dt.complex_value.real);
		case COMBINED_TYPE_SS:
			return STRING_EQUAL(&(a->dt.s),&(b->dt.s));
		case COMBINED_TYPE_SA:
			return sll_string_equal_array(&(a->dt.s),&(b->dt.a));
		case COMBINED_TYPE_SM:
			return sll_string_equal_map(&(a->dt.s),&(b->dt.m));
		case COMBINED_TYPE_AI:
			return a->dt.a.length==b->dt.int_value;
		case COMBINED_TYPE_AF:
			return a->dt.a.length==b->dt.float_value;
		case COMBINED_TYPE_AC:
			return a->dt.a.length==b->dt.char_value;
		case COMBINED_TYPE_AD:
			return (!b->dt.complex_value.imag&&a->dt.a.length==b->dt.complex_value.real);
		case COMBINED_TYPE_AS:
			return sll_string_equal_array(&(b->dt.s),&(a->dt.a));
		case COMBINED_TYPE_AA:
			return sll_array_equal(&(a->dt.a),&(b->dt.a));
		case COMBINED_TYPE_AM:
			return sll_array_equal_map(&(a->dt.a),&(b->dt.m));
		case COMBINED_TYPE_MI:
			return a->dt.m.length==b->dt.int_value;
		case COMBINED_TYPE_MF:
			return a->dt.m.length==b->dt.float_value;
		case COMBINED_TYPE_MC:
			return a->dt.m.length==b->dt.char_value;
		case COMBINED_TYPE_MD:
			return (!b->dt.complex_value.imag&&a->dt.m.length==b->dt.complex_value.real);
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
			return a->dt.int_value==b->dt.int_value;
		case SLL_OBJECT_TYPE_FLOAT:
			return a->dt.float_value==b->dt.float_value;
		case SLL_OBJECT_TYPE_CHAR:
			return a->dt.char_value==b->dt.char_value;
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
					if (pa->int_value!=pb->int_value){
						return 0;
					}
					break;
				case SLL_OBJECT_TYPE_FLOAT:
					if (pa->float_value!=pb->float_value){
						return 0;
					}
					break;
				case SLL_OBJECT_TYPE_CHAR:
					if (pa->char_value!=pb->char_value){
						return 0;
					}
					break;
				default:
					if (!sll_operator_strict_equal(pa->any,pb->any)){
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
