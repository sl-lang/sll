#include <sll/_internal/common.h>
#include <sll/_internal/complex.h>
#include <sll/_internal/operator.h>
#include <sll/api/string.h>
#include <sll/array.h>
#include <sll/common.h>
#include <sll/gc.h>
#include <sll/map.h>
#include <sll/object.h>
#include <sll/static_object.h>
#include <sll/types.h>



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t sll_operator_add(sll_object_t a,sll_object_t b){
	COMMUTATIVE_OPERATOR;
	switch (COMBINED_ARGS){
		case COMBINED_TYPE_II:
			return sll_int_to_object(a->data.int_+b->data.int_);
		case COMBINED_TYPE_IF:
			return sll_float_to_object(a->data.int_+b->data.float_);
		case COMBINED_TYPE_IC:
			return sll_int_to_object(a->data.int_+b->data.char_);
		case COMBINED_TYPE_ID:
			{
				sll_complex_t n={
					a->data.int_+b->data.complex_.real,
					b->data.complex_.imag
				};
				return sll_complex_to_object(&n);
			}
		case COMBINED_TYPE_IS:
		case COMBINED_TYPE_FS:
		case COMBINED_TYPE_CS:
		case COMBINED_TYPE_DS:
			{
				sll_object_t tmp=a;
				a=b;
				b=tmp;
_add_to_string:
				SLL_ASSERT(a->type==SLL_OBJECT_TYPE_STRING);
				sll_string_t sa=a->data.string;
				sll_string_t sb;
				sll_api_string_convert(&b,1,&sb);
				sll_object_t o=sll_create_object(SLL_OBJECT_TYPE_STRING);
				if (!sa.length){
					o->data.string=sb;
					return o;
				}
				if (!inv){
					sll_string_t tmp_s=sa;
					sa=sb;
					sb=tmp_s;
				}
				sll_string_concat(&sa,&sb,&(o->data.string));
				sll_free_string((!inv?&sa:&sb));
				return o;
			}
		case COMBINED_TYPE_IA:
		case COMBINED_TYPE_FA:
		case COMBINED_TYPE_CA:
		case COMBINED_TYPE_DA:
			{
				sll_object_t o=sll_create_object(SLL_OBJECT_TYPE_ARRAY);
				if (inv){
					sll_array_push(&(b->data.array),a,&(o->data.array));
				}
				else{
					sll_array_unshift(&(b->data.array),a,&(o->data.array));
				}
				return o;
			}
		case COMBINED_TYPE_IM:
		case COMBINED_TYPE_FM:
		case COMBINED_TYPE_CM:
		case COMBINED_TYPE_DM:
			{
				sll_object_t o=sll_create_object(SLL_OBJECT_TYPE_MAP);
				sll_map_add(&(b->data.map),a,sll_static_int[0],&(o->data.map));
				return o;
			}
		case COMBINED_TYPE_FF:
			return sll_float_to_object(a->data.float_+b->data.float_);
		case COMBINED_TYPE_FC:
			return sll_float_to_object(a->data.float_+b->data.char_);
		case COMBINED_TYPE_FD:
			return sll_float_to_object(a->data.float_+b->data.char_);
		case COMBINED_TYPE_CC:
			return SLL_FROM_CHAR(a->data.char_+b->data.char_);
		case COMBINED_TYPE_CD:
			{
				sll_complex_t n={
					a->data.char_+b->data.complex_.real,
					b->data.complex_.imag
				};
				return sll_complex_to_object(&n);
			}
		case COMBINED_TYPE_DD:
			{
				sll_complex_t n=COMPLEX_ADD(a->data.complex_,b->data.complex_);
				return sll_complex_to_object(&n);
			}
		case COMBINED_TYPE_SS:
			{
				sll_object_t o=sll_create_object(SLL_OBJECT_TYPE_STRING);
				sll_string_concat(&(a->data.string),&(b->data.string),&(o->data.string));
				return o;
			}
		case COMBINED_TYPE_SA:
			if (inv){
				sll_object_t o=sll_create_object(SLL_OBJECT_TYPE_ARRAY);
				sll_array_push(&(b->data.array),a,&(o->data.array));
				return o;
			}
			goto _add_to_string;
		case COMBINED_TYPE_SM:
			if (inv){
				sll_object_t o=sll_create_object(SLL_OBJECT_TYPE_MAP);
				sll_map_add(&(b->data.map),a,sll_static_int[0],&(o->data.map));
				return o;
			}
			goto _add_to_string;
		case COMBINED_TYPE_AA:
			{
				sll_object_t o=sll_create_object(SLL_OBJECT_TYPE_ARRAY);
				sll_array_extend(&(a->data.array),&(b->data.array),&(o->data.array));
				return o;
			}
		case COMBINED_TYPE_AM:
			{
				if (inv){
					sll_object_t o=sll_create_object(SLL_OBJECT_TYPE_MAP);
					sll_map_add(&(b->data.map),a,sll_static_int[0],&(o->data.map));
					return o;
				}
				sll_object_t o=sll_create_object(SLL_OBJECT_TYPE_ARRAY);
				sll_array_push(&(a->data.array),b,&(o->data.array));
				return o;
			}
		case COMBINED_TYPE_MM:
			{
				sll_object_t o=sll_create_object(SLL_OBJECT_TYPE_MAP);
				sll_map_join(&(a->data.map),&(b->data.map),&(o->data.map));
				return o;
			}
		default:
			SLL_UNREACHABLE();
	}
	return SLL_ACQUIRE_STATIC_INT(0);
}
