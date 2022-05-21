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
	switch (a->type){
		case SLL_OBJECT_TYPE_INT:
			return sll_int_to_object(a->data.int_-1);
		case SLL_OBJECT_TYPE_FLOAT:
			return sll_float_to_object(a->data.float_-1);
		case SLL_OBJECT_TYPE_CHAR:
			return SLL_FROM_CHAR(a->data.char_-1);
		case SLL_OBJECT_TYPE_COMPLEX:
			{
				sll_complex_t n={
					a->data.complex_.real-1,
					a->data.complex_.imag
				};
				return sll_complex_to_object(&n);
			}
		case SLL_OBJECT_TYPE_STRING:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_STRING);
				sll_string_t str;
				sll_string_from_char('1',&str);
				sll_string_remove(&(a->data.string),&str,&(o->data.string));
				sll_free_string(&str);
				return o;
			}
		case SLL_OBJECT_TYPE_ARRAY:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_ARRAY);
				sll_array_remove(&(a->data.array),sll_static_int[1],&(o->data.array));
				return o;
			}
		case SLL_OBJECT_TYPE_MAP:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_MAP);
				SLL_RELEASE(sll_map_remove(&(a->data.map),sll_static_int[1],&(o->data.map)));
				return o;
			}
		default:
			SLL_UNREACHABLE();
	}
	return SLL_ACQUIRE_STATIC_INT(0);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_operator_inc(sll_object_t* a){
	switch (a->type){
		case SLL_OBJECT_TYPE_INT:
			return sll_int_to_object(a->data.int_+1);
		case SLL_OBJECT_TYPE_FLOAT:
			return sll_float_to_object(a->data.float_+1);
		case SLL_OBJECT_TYPE_CHAR:
			return SLL_FROM_CHAR(a->data.char_+1);
		case SLL_OBJECT_TYPE_COMPLEX:
			{
				sll_complex_t n={
					a->data.complex_.real+1,
					a->data.complex_.imag
				};
				return sll_complex_to_object(&n);
			}
		case SLL_OBJECT_TYPE_STRING:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_STRING);
				sll_string_concat_char(&(a->data.string),'1',&(o->data.string));
				return o;
			}
		case SLL_OBJECT_TYPE_ARRAY:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_ARRAY);
				sll_array_push(&(a->data.array),sll_static_int[1],&(o->data.array));
				return o;
			}
		case SLL_OBJECT_TYPE_MAP:
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_MAP);
				sll_map_add(&(a->data.map),sll_static_int[1],sll_static_int[0],&(o->data.map));
				return o;
			}
		default:
			SLL_UNREACHABLE();
	}
	return SLL_ACQUIRE_STATIC_INT(0);
}
