#include <sll/_sll_internal.h>
#include <sll/array.h>
#include <sll/common.h>
#include <sll/handle.h>
#include <sll/map.h>
#include <sll/memory.h>
#include <sll/object.h>
#include <sll/operator.h>
#include <sll/static_object.h>
#include <sll/string.h>
#include <sll/types.h>



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_type_t sll_add_type(sll_object_type_table_t* tt,const sll_object_t*const* p,sll_arg_count_t l){
	tt->l++;
	SLL_ASSERT(tt->l+SLL_MAX_OBJECT_TYPE<SLL_OBJECT_TYPE_RESERVED2);
	tt->dt=sll_reallocate((void*)(tt->dt),tt->l*sizeof(const sll_object_type_data_t*));
	sll_object_type_data_t* n=sll_allocate(sizeof(sll_object_type_data_t)+l*sizeof(sll_object_type_data_entry_t));
	n->l=l;
	for (sll_arg_count_t i=0;i<l;i++){
		sll_object_t* v=sll_operator_cast((sll_object_t*)(*p),sll_static_int[SLL_OBJECT_TYPE_INT]);
		if (v->dt.i<0||v->dt.i>tt->l+SLL_MAX_OBJECT_TYPE-1){
			n->dt[i].t=SLL_OBJECT_TYPE_INT;
		}
		else{
			n->dt[i].t=(sll_object_type_t)(v->dt.i);
		}
		SLL_RELEASE(v);
		p++;
		v=sll_operator_cast((sll_object_t*)(*p),sll_static_int[SLL_OBJECT_TYPE_STRING]);
		sll_string_clone(&(v->dt.s),&(n->dt[i].nm));
		SLL_RELEASE(v);
		p++;
	}
	*(tt->dt+tt->l-1)=n;
	return tt->l+SLL_MAX_OBJECT_TYPE;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_create_object_type(const sll_object_type_table_t* tt,sll_object_type_t t,sll_object_t*const* p,sll_arg_count_t l){
	if (t<=SLL_MAX_OBJECT_TYPE){
		switch (t){
			case SLL_OBJECT_TYPE_INT:
				if (!l){
					return SLL_ACQUIRE_STATIC_INT(0);
				}
				return sll_operator_cast(*p,sll_static_int[SLL_OBJECT_TYPE_INT]);
			case SLL_OBJECT_TYPE_FLOAT:
				if (!l){
					return SLL_ACQUIRE_STATIC(float_zero);
				}
				return sll_operator_cast(*p,sll_static_int[SLL_OBJECT_TYPE_FLOAT]);
			case SLL_OBJECT_TYPE_CHAR:
				if (!l){
					return SLL_ACQUIRE_STATIC_CHAR(0);
				}
				return sll_operator_cast(*p,sll_static_int[SLL_OBJECT_TYPE_CHAR]);
			case SLL_OBJECT_TYPE_STRING:
				SLL_UNIMPLEMENTED();
			case SLL_OBJECT_TYPE_ARRAY:
			case SLL_OBJECT_TYPE_MAP_KEYS:
			case SLL_OBJECT_TYPE_MAP_VALUES:
				{
					if (!l){
						return SLL_ACQUIRE_STATIC(array_zero);
					}
					sll_object_t* o=SLL_CREATE();
					o->t=SLL_OBJECT_TYPE_ARRAY;
					sll_array_create(l,&(o->dt.a));
					for (sll_arg_count_t i=0;i<l;i++){
						o->dt.a.v[i]=*(p+i);
						SLL_ACQUIRE(*(p+i));
					}
					return o;
				}
			case SLL_OBJECT_TYPE_HANDLE:
				if (!l){
					return SLL_ACQUIRE_STATIC(handle_zero);
				}
				sll_handle_type_t ot=((*p)->dt.i<0||(*p)->dt.i>SLL_MAX_HANDLE_TYPE?0:(sll_handle_type_t)((*p)->dt.i));
				if (l==1){
					return SLL_FROM_HANDLE(ot,0);
				}
				sll_object_t* v=sll_operator_cast(*(p+1),sll_static_int[SLL_OBJECT_TYPE_INT]);
				sll_handle_t h=v->dt.i;
				SLL_RELEASE(v);
				return SLL_FROM_HANDLE(ot,h);
			case SLL_OBJECT_TYPE_MAP:
				{
					if (!l){
						return SLL_ACQUIRE_STATIC(map_zero);
					}
					sll_object_t* o=SLL_CREATE();
					o->t=SLL_OBJECT_TYPE_MAP;
					sll_map_create((l+1)>>1,&(o->dt.m));
					for (sll_arg_count_t i=0;i<l;i++){
						o->dt.m.v[i]=*(p+i);
						SLL_ACQUIRE(*(p+i));
					}
					if (l&1){
						o->dt.m.v[l]=SLL_ACQUIRE_STATIC_INT(0);
					}
					return o;
				}
			default:
				SLL_UNREACHABLE();
		}
	}
	SLL_UNIMPLEMENTED();
}



__SLL_EXTERNAL void sll_free_object_type_list(sll_object_type_table_t* tt){
	if (tt->l){
		for (sll_object_type_t i=0;i<tt->l;i++){
			const sll_object_type_data_t* k=*(tt->dt+i);
			for (sll_arg_count_t j=0;j<k->l;j++){
				sll_free_string((sll_string_t*)(&(k->dt[j].nm)));
			}
			sll_deallocate((void*)k);
		}
		sll_deallocate((void*)(tt->dt));
		tt->dt=NULL;
		tt->l=0;
	}
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_type_t sll_object_get_type(const sll_object_t* o){
	return SLL_OBJECT_GET_TYPE(o);
}
