#include <sll/array.h>
#include <sll/common.h>
#include <sll/gc.h>
#include <sll/map.h>
#include <sll/object.h>
#include <sll/string.h>
#include <sll/types.h>



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_operator_includes(sll_object_t* a,sll_object_t* b){
	if (a->type==SLL_OBJECT_TYPE_STRING){
		if (b->type==SLL_OBJECT_TYPE_CHAR){
			return sll_string_includes_char(&(a->data.string),b->data.char_);
		}
		if (b->type==SLL_OBJECT_TYPE_STRING){
			return sll_string_includes(&(a->data.string),&(b->data.string));
		}
	}
	else if (a->type==SLL_OBJECT_TYPE_ARRAY){
		return sll_array_includes(&(a->data.array),b);
	}
	else if (a->type==SLL_OBJECT_TYPE_MAP){
		return sll_map_includes(&(a->data.map),b);
	}
	return 0;
}
