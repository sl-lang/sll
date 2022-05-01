#include <sll/array.h>
#include <sll/common.h>
#include <sll/gc.h>
#include <sll/map.h>
#include <sll/object.h>
#include <sll/string.h>
#include <sll/types.h>



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_operator_includes(sll_object_t* a,sll_object_t* b){
	if (a->t==SLL_OBJECT_TYPE_STRING){
		if (b->t==SLL_OBJECT_TYPE_CHAR){
			return sll_string_includes_char(&(a->dt.s),b->dt.c);
		}
		if (b->t==SLL_OBJECT_TYPE_STRING){
			return sll_string_includes(&(a->dt.s),&(b->dt.s));
		}
	}
	else if (a->t==SLL_OBJECT_TYPE_ARRAY){
		return sll_array_includes(&(a->dt.a),b);
	}
	else if (a->t==SLL_OBJECT_TYPE_MAP){
		return sll_map_includes(&(a->dt.m),b);
	}
	return 0;
}
