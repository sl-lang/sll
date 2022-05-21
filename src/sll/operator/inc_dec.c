#include <sll/_internal/common.h>
#include <sll/array.h>
#include <sll/common.h>
#include <sll/gc.h>
#include <sll/map.h>
#include <sll/object.h>
#include <sll/static_object.h>
#include <sll/string.h>
#include <sll/types.h>



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_operator_dec(sll_object_t* a){
	switch (a->t){
		case SLL_OBJECT_TYPE_INT:
			return sll_int_to_object(a->dt.int_value-1);
		case SLL_OBJECT_TYPE_FLOAT:
			return sll_float_to_object(a->dt.float_value-1);
		case SLL_OBJECT_TYPE_CHAR:
			return SLL_FROM_CHAR(a->dt.char_value-1);
		case SLL_OBJECT_TYPE_COMPLEX:
			{
				sll_complex_t n={
					a->dt.complex_value.real-1,
					a->dt.complex_value.imag
				};
				return sll_complex_to_object(&n);
			}
		case SLL_OBJECT_TYPE_STRING:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_STRING);
				sll_string_t str;
				sll_string_from_char('1',&str);
				sll_string_remove(&(a->dt.string),&str,&(o->dt.string));
				sll_free_string(&str);
				return o;
			}
		case SLL_OBJECT_TYPE_ARRAY:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_ARRAY);
				sll_array_remove(&(a->dt.array),sll_static_int[1],&(o->dt.array));
				return o;
			}
		case SLL_OBJECT_TYPE_MAP:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_MAP);
				SLL_RELEASE(sll_map_remove(&(a->dt.map),sll_static_int[1],&(o->dt.map)));
				return o;
			}
		default:
			SLL_UNREACHABLE();
	}
	return SLL_ACQUIRE_STATIC_INT(0);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_operator_inc(sll_object_t* a){
	switch (a->t){
		case SLL_OBJECT_TYPE_INT:
			return sll_int_to_object(a->dt.int_value+1);
		case SLL_OBJECT_TYPE_FLOAT:
			return sll_float_to_object(a->dt.float_value+1);
		case SLL_OBJECT_TYPE_CHAR:
			return SLL_FROM_CHAR(a->dt.char_value+1);
		case SLL_OBJECT_TYPE_COMPLEX:
			{
				sll_complex_t n={
					a->dt.complex_value.real+1,
					a->dt.complex_value.imag
				};
				return sll_complex_to_object(&n);
			}
		case SLL_OBJECT_TYPE_STRING:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_STRING);
				sll_string_concat_char(&(a->dt.string),'1',&(o->dt.string));
				return o;
			}
		case SLL_OBJECT_TYPE_ARRAY:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_ARRAY);
				sll_array_push(&(a->dt.array),sll_static_int[1],&(o->dt.array));
				return o;
			}
		case SLL_OBJECT_TYPE_MAP:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_MAP);
				sll_map_add(&(a->dt.map),sll_static_int[1],sll_static_int[0],&(o->dt.map));
				return o;
			}
		default:
			SLL_UNREACHABLE();
	}
	return SLL_ACQUIRE_STATIC_INT(0);
}
