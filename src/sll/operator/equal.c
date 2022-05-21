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
			return a->data.int_==b->data.int_;
		case COMBINED_TYPE_IF:
			return a->data.int_==b->data.float_;
		case COMBINED_TYPE_IC:
			return a->data.int_==b->data.char_;
		case COMBINED_TYPE_ID:
			return (!b->data.complex_.imag&&a->data.int_==b->data.complex_.real);
		case COMBINED_TYPE_IS:
			return a->data.int_==b->data.string.l;
		case COMBINED_TYPE_IA:
			return a->data.int_==b->data.array.length;
		case COMBINED_TYPE_IM:
			return a->data.int_==b->data.map.length;
		case COMBINED_TYPE_FI:
			return a->data.float_==b->data.int_;
		case COMBINED_TYPE_FF:
			return a->data.float_==b->data.float_;
		case COMBINED_TYPE_FC:
			return a->data.float_==b->data.char_;
		case COMBINED_TYPE_FD:
			return (!b->data.complex_.imag&&a->data.float_==b->data.complex_.real);
		case COMBINED_TYPE_FS:
			return a->data.float_==b->data.string.l;
		case COMBINED_TYPE_FA:
			return a->data.float_==b->data.array.length;
		case COMBINED_TYPE_FM:
			return a->data.float_==b->data.map.length;
		case COMBINED_TYPE_CI:
			return a->data.char_==b->data.int_;
		case COMBINED_TYPE_CF:
			return a->data.char_==b->data.float_;
		case COMBINED_TYPE_CC:
			return a->data.char_==b->data.char_;
		case COMBINED_TYPE_CD:
			return (!b->data.complex_.imag&&a->data.char_==b->data.complex_.real);
		case COMBINED_TYPE_CS:
			return (b->data.string.l==1&&b->data.string.v[0]==a->data.char_);
		case COMBINED_TYPE_CA:
			return a->data.char_==b->data.array.length;
		case COMBINED_TYPE_CM:
			return a->data.char_==b->data.map.length;
		case COMBINED_TYPE_DI:
			return (!a->data.complex_.imag&&a->data.complex_.real==b->data.int_);
		case COMBINED_TYPE_DF:
			return (!a->data.complex_.imag&&a->data.complex_.real==b->data.float_);
		case COMBINED_TYPE_DC:
			return (!a->data.complex_.imag&&a->data.complex_.real==b->data.char_);
		case COMBINED_TYPE_DD:
			return (a->data.complex_.real==b->data.complex_.real&&a->data.complex_.imag==b->data.complex_.imag);
		case COMBINED_TYPE_DS:
			return (!a->data.complex_.imag&&a->data.complex_.real==b->data.string.l);
		case COMBINED_TYPE_DA:
			return (!a->data.complex_.imag&&a->data.complex_.real==b->data.array.length);
		case COMBINED_TYPE_DM:
			return (!a->data.complex_.imag&&a->data.complex_.real==b->data.map.length);
		case COMBINED_TYPE_SI:
			return a->data.string.l==b->data.int_;
		case COMBINED_TYPE_SF:
			return a->data.string.l==b->data.float_;
		case COMBINED_TYPE_SC:
			return (a->data.string.l==1&&a->data.string.v[0]==b->data.char_);
		case COMBINED_TYPE_SD:
			return (!b->data.complex_.imag&&a->data.string.l==b->data.complex_.real);
		case COMBINED_TYPE_SS:
			return STRING_EQUAL(&(a->data.string),&(b->data.string));
		case COMBINED_TYPE_SA:
			return sll_string_equal_array(&(a->data.string),&(b->data.array));
		case COMBINED_TYPE_SM:
			return sll_string_equal_map(&(a->data.string),&(b->data.map));
		case COMBINED_TYPE_AI:
			return a->data.array.length==b->data.int_;
		case COMBINED_TYPE_AF:
			return a->data.array.length==b->data.float_;
		case COMBINED_TYPE_AC:
			return a->data.array.length==b->data.char_;
		case COMBINED_TYPE_AD:
			return (!b->data.complex_.imag&&a->data.array.length==b->data.complex_.real);
		case COMBINED_TYPE_AS:
			return sll_string_equal_array(&(b->data.string),&(a->data.array));
		case COMBINED_TYPE_AA:
			return sll_array_equal(&(a->data.array),&(b->data.array));
		case COMBINED_TYPE_AM:
			return sll_array_equal_map(&(a->data.array),&(b->data.map));
		case COMBINED_TYPE_MI:
			return a->data.map.length==b->data.int_;
		case COMBINED_TYPE_MF:
			return a->data.map.length==b->data.float_;
		case COMBINED_TYPE_MC:
			return a->data.map.length==b->data.char_;
		case COMBINED_TYPE_MD:
			return (!b->data.complex_.imag&&a->data.map.length==b->data.complex_.real);
		case COMBINED_TYPE_MS:
			return sll_string_equal_map(&(b->data.string),&(a->data.map));
		case COMBINED_TYPE_MA:
			return sll_array_equal_map(&(b->data.array),&(a->data.map));
		case COMBINED_TYPE_MM:
			return sll_map_equal(&(a->data.map),&(b->data.map));
		default:
			SLL_UNREACHABLE();
	}
	return 0;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_operator_strict_equal(sll_object_t* a,sll_object_t* b){
	if (a->type!=b->type){
		return 0;
	}
	switch (a->type){
		case SLL_OBJECT_TYPE_INT:
			return a->data.int_==b->data.int_;
		case SLL_OBJECT_TYPE_FLOAT:
			return a->data.float_==b->data.float_;
		case SLL_OBJECT_TYPE_CHAR:
			return a->data.char_==b->data.char_;
		case SLL_OBJECT_TYPE_STRING:
			return STRING_EQUAL(&(a->data.string),&(b->data.string));
		case SLL_OBJECT_TYPE_ARRAY:
			return sll_array_equal(&(a->data.array),&(b->data.array));
		case SLL_OBJECT_TYPE_MAP:
			return sll_map_equal(&(a->data.map),&(b->data.map));
	}
	if (sll_current_runtime_data&&a->type>SLL_MAX_OBJECT_TYPE&&a->type<=sll_current_runtime_data->tt->l+SLL_MAX_OBJECT_TYPE){
		const sll_object_type_data_t* dt=*(sll_current_runtime_data->tt->dt+a->type-SLL_MAX_OBJECT_TYPE-1);
		sll_object_field_t* pa=a->data.fields;
		sll_object_field_t* pb=b->data.fields;
		for (sll_arg_count_t i=0;i<dt->l;i++){
			switch (dt->dt[i].t){
				case SLL_OBJECT_TYPE_INT:
					if (pa->int_!=pb->int_){
						return 0;
					}
					break;
				case SLL_OBJECT_TYPE_FLOAT:
					if (pa->float_!=pb->float_){
						return 0;
					}
					break;
				case SLL_OBJECT_TYPE_CHAR:
					if (pa->char_!=pb->char_){
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
