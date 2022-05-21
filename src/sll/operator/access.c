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
	if (a->t==SLL_OBJECT_TYPE_STRING){
		if (b->t==SLL_OBJECT_TYPE_INT){
			if (!a->dt.s.l){
				return SLL_ACQUIRE_STATIC_INT(0);
			}
			WRAP_ARRAY_INDEX(b->dt.int_value,a->dt.a.length,idx);
			if (idx<0||idx>=a->dt.a.length){
				return SLL_ACQUIRE_STATIC_INT(0);
			}
			return SLL_FROM_CHAR(a->dt.s.v[idx]);
		}
	}
	else if (a->t==SLL_OBJECT_TYPE_ARRAY){
		if (b->t==SLL_OBJECT_TYPE_INT){
			if (!a->dt.a.length){
				return SLL_ACQUIRE_STATIC_INT(0);
			}
			WRAP_ARRAY_INDEX(b->dt.int_value,a->dt.a.length,idx);
			if (idx<0||idx>=a->dt.a.length){
				return SLL_ACQUIRE_STATIC_INT(0);
			}
			sll_object_t* o=a->dt.a.data[idx];
			SLL_ACQUIRE(o);
			return o;
		}
	}
	else if (a->t==SLL_OBJECT_TYPE_MAP){
		return sll_map_get(&(a->dt.m),b);
	}
	else if (sll_current_runtime_data&&b->t==SLL_OBJECT_TYPE_STRING&&a->t>SLL_MAX_OBJECT_TYPE&&a->t<=sll_current_runtime_data->tt->l+SLL_MAX_OBJECT_TYPE){
		return sll_object_get_field(sll_current_runtime_data->tt,a,&(b->dt.s));
	}
	SLL_ACQUIRE(a);
	return a;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_operator_access_range(sll_object_t* a,sll_object_t* b,sll_object_t* c){
	if (a->t==SLL_OBJECT_TYPE_STRING){
		if (b->t==SLL_OBJECT_TYPE_INT&&c->t==SLL_OBJECT_TYPE_INT){
			sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_STRING);
			sll_string_select(&(a->dt.s),b->dt.int_value,c->dt.int_value,1,&(o->dt.s));
			return o;
		}
	}
	else if (a->t==SLL_OBJECT_TYPE_ARRAY){
		if (b->t==SLL_OBJECT_TYPE_INT&&c->t==SLL_OBJECT_TYPE_INT){
			sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_ARRAY);
			sll_array_select(&(a->dt.a),b->dt.int_value,c->dt.int_value,1,&(o->dt.a));
			return o;
		}
	}
	else if (a->t==SLL_OBJECT_TYPE_MAP){
		sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_MAP);
		sll_object_t* l[2]={b,c};
		sll_array_t arr={
			2,
			l
		};
		sll_map_and_array(&(a->dt.m),&arr,&(o->dt.m));
		return o;
	}
	SLL_ACQUIRE(a);
	return a;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_operator_access_range_step(sll_object_t* a,sll_object_t* b,sll_object_t* c,sll_object_t* d){
	if (a->t==SLL_OBJECT_TYPE_STRING){
		if (b->t==SLL_OBJECT_TYPE_INT&&c->t==SLL_OBJECT_TYPE_INT&&d->t==SLL_OBJECT_TYPE_INT){
			sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_STRING);
			sll_string_select(&(a->dt.s),b->dt.int_value,c->dt.int_value,d->dt.int_value,&(o->dt.s));
			return o;
		}
	}
	else if (a->t==SLL_OBJECT_TYPE_ARRAY){
		if (b->t==SLL_OBJECT_TYPE_INT&&c->t==SLL_OBJECT_TYPE_INT&&d->t==SLL_OBJECT_TYPE_INT){
			sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_ARRAY);
			sll_array_select(&(a->dt.a),b->dt.int_value,c->dt.int_value,d->dt.int_value,&(o->dt.a));
			return o;
		}
	}
	else if (a->t==SLL_OBJECT_TYPE_MAP){
		sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_MAP);
		sll_object_t* l[3]={b,c,d};
		sll_array_t arr={
			3,
			l
		};
		sll_map_and_array(&(a->dt.m),&arr,&(o->dt.m));
		return o;
	}
	SLL_ACQUIRE(a);
	return a;
}
