#include <sll/_internal/util.h>
#include <sll/array.h>
#include <sll/common.h>
#include <sll/gc.h>
#include <sll/map.h>
#include <sll/object.h>
#include <sll/static_object.h>
#include <sll/string.h>
#include <sll/types.h>
#include <sll/vm.h>



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_operator_access(sll_object_t* a,sll_object_t* b){
	if (a->type==SLL_OBJECT_TYPE_STRING){
		if (b->type==SLL_OBJECT_TYPE_INT){
			if (!a->data.string.length){
				return SLL_ACQUIRE_STATIC_INT(0);
			}
			WRAP_ARRAY_INDEX(b->data.int_,a->data.array.length,idx);
			if (idx<0||idx>=a->data.array.length){
				return SLL_ACQUIRE_STATIC_INT(0);
			}
			return SLL_FROM_CHAR(a->data.string.data[idx]);
		}
	}
	else if (a->type==SLL_OBJECT_TYPE_ARRAY){
		if (b->type==SLL_OBJECT_TYPE_INT){
			if (!a->data.array.length){
				return SLL_ACQUIRE_STATIC_INT(0);
			}
			WRAP_ARRAY_INDEX(b->data.int_,a->data.array.length,idx);
			if (idx<0||idx>=a->data.array.length){
				return SLL_ACQUIRE_STATIC_INT(0);
			}
			sll_object_t* o=a->data.array.data[idx];
			SLL_ACQUIRE(o);
			return o;
		}
	}
	else if (a->type==SLL_OBJECT_TYPE_MAP){
		return sll_map_get(&(a->data.map),b);
	}
	else if (sll_current_runtime_data&&b->type==SLL_OBJECT_TYPE_STRING&&a->type>SLL_MAX_OBJECT_TYPE&&a->type<=sll_current_runtime_data->tt->length+SLL_MAX_OBJECT_TYPE){
		return sll_object_get_field(sll_current_runtime_data->tt,a,&(b->data.string));
	}
	SLL_ACQUIRE(a);
	return a;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_operator_access_range(sll_object_t* a,sll_object_t* b,sll_object_t* c){
	if (a->type==SLL_OBJECT_TYPE_STRING){
		if (b->type==SLL_OBJECT_TYPE_INT&&c->type==SLL_OBJECT_TYPE_INT){
			sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_STRING);
			sll_string_select(&(a->data.string),b->data.int_,c->data.int_,1,&(o->data.string));
			return o;
		}
	}
	else if (a->type==SLL_OBJECT_TYPE_ARRAY){
		if (b->type==SLL_OBJECT_TYPE_INT&&c->type==SLL_OBJECT_TYPE_INT){
			sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_ARRAY);
			sll_array_select(&(a->data.array),b->data.int_,c->data.int_,1,&(o->data.array));
			return o;
		}
	}
	else if (a->type==SLL_OBJECT_TYPE_MAP){
		sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_MAP);
		sll_object_t* l[2]={b,c};
		sll_array_t arr={
			2,
			l
		};
		sll_map_and_array(&(a->data.map),&arr,&(o->data.map));
		return o;
	}
	SLL_ACQUIRE(a);
	return a;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_operator_access_range_step(sll_object_t* a,sll_object_t* b,sll_object_t* c,sll_object_t* d){
	if (a->type==SLL_OBJECT_TYPE_STRING){
		if (b->type==SLL_OBJECT_TYPE_INT&&c->type==SLL_OBJECT_TYPE_INT&&d->type==SLL_OBJECT_TYPE_INT){
			sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_STRING);
			sll_string_select(&(a->data.string),b->data.int_,c->data.int_,d->data.int_,&(o->data.string));
			return o;
		}
	}
	else if (a->type==SLL_OBJECT_TYPE_ARRAY){
		if (b->type==SLL_OBJECT_TYPE_INT&&c->type==SLL_OBJECT_TYPE_INT&&d->type==SLL_OBJECT_TYPE_INT){
			sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_ARRAY);
			sll_array_select(&(a->data.array),b->data.int_,c->data.int_,d->data.int_,&(o->data.array));
			return o;
		}
	}
	else if (a->type==SLL_OBJECT_TYPE_MAP){
		sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_MAP);
		sll_object_t* l[3]={b,c,d};
		sll_array_t arr={
			3,
			l
		};
		sll_map_and_array(&(a->data.map),&arr,&(o->data.map));
		return o;
	}
	SLL_ACQUIRE(a);
	return a;
}
