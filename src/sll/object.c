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



#define SET_DATA(p,t,v) \
	do{ \
		*((t*)(p))=(v); \
		p=(void*)(((uint64_t)(p))+sizeof(t)); \
	} while (0)



static void* _zero_struct(const sll_object_type_table_t* tt,void* op,sll_object_type_t t,sll_arg_count_t i){
	const sll_object_type_data_t* dt=*(tt->dt+t-SLL_MAX_OBJECT_TYPE-1);
	while (i<dt->l){
		switch (dt->dt[i].t){
			case SLL_OBJECT_TYPE_INT:
				SET_DATA(op,sll_integer_t,0);
				break;
			case SLL_OBJECT_TYPE_FLOAT:
				SET_DATA(op,sll_float_t,0);
				break;
			case SLL_OBJECT_TYPE_CHAR:
				SET_DATA(op,sll_integer_t,0);
				break;
			case SLL_OBJECT_TYPE_STRING:
				{
					sll_string_t s=SLL_INIT_STRING_STRUCT;
					SET_DATA(op,sll_string_t,s);
					break;
				}
			case SLL_OBJECT_TYPE_ARRAY:
			case SLL_OBJECT_TYPE_MAP_KEYS:
			case SLL_OBJECT_TYPE_MAP_VALUES:
				{
					sll_array_t a=SLL_INIT_ARRAY_STRUCT;
					SET_DATA(op,sll_array_t,a);
					break;
				}
			case SLL_OBJECT_TYPE_HANDLE:
				{
					sll_handle_data_t h_dt=SLL_INIT_HANDLE_DATA_STRUCT;
					SET_DATA(op,sll_handle_data_t,h_dt);
					break;
				}
			case SLL_OBJECT_TYPE_MAP:
				{
					sll_map_t m=SLL_INIT_MAP_STRUCT;
					SET_DATA(op,sll_map_t,m);
					break;
				}
			default:
				op=_zero_struct(tt,op,dt->dt[i].t,0);
				break;
		}
		i++;
	}
	return op;
}



static void* _init_struct(const sll_object_type_table_t* tt,sll_object_type_t t,void* op,sll_object_t*const* p,sll_arg_count_t l){
	const sll_object_type_data_t* dt=*(tt->dt+t-SLL_MAX_OBJECT_TYPE-1);
	for (sll_arg_count_t i=0;i<dt->l;i++){
		if (!l){
			op=_zero_struct(tt,op,t,i);
			break;
		}
		sll_object_t* n=sll_operator_cast(*p,sll_static_int[(dt->dt[i].t<=SLL_MAX_OBJECT_TYPE?dt->dt[i].t:SLL_OBJECT_TYPE_ARRAY)]);
		switch (dt->dt[i].t){
			case SLL_OBJECT_TYPE_INT:
				SET_DATA(op,sll_integer_t,n->dt.i);
				break;
			case SLL_OBJECT_TYPE_FLOAT:
				SET_DATA(op,sll_float_t,n->dt.f);
				break;
			case SLL_OBJECT_TYPE_CHAR:
				SET_DATA(op,sll_integer_t,n->dt.c);
				break;
			case SLL_OBJECT_TYPE_STRING:
				SET_DATA(op,sll_string_t,n->dt.s);
				break;
			case SLL_OBJECT_TYPE_ARRAY:
			case SLL_OBJECT_TYPE_MAP_KEYS:
			case SLL_OBJECT_TYPE_MAP_VALUES:
				SET_DATA(op,sll_array_t,n->dt.a);
				break;
			case SLL_OBJECT_TYPE_HANDLE:
				SET_DATA(op,sll_handle_data_t,n->dt.h);
				break;
			case SLL_OBJECT_TYPE_MAP:
				SET_DATA(op,sll_map_t,n->dt.m);
				break;
			default:
				op=_init_struct(tt,dt->dt[i].t,op,n->dt.a.v,n->dt.a.l);
				break;
		}
		SLL_RELEASE(n);
		l--;
		p++;
	}
	return op;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_type_t sll_add_type(sll_object_type_table_t* tt,const sll_object_t*const* p,sll_arg_count_t l){
	tt->l++;
	SLL_ASSERT(tt->l+SLL_MAX_OBJECT_TYPE<SLL_OBJECT_TYPE_RESERVED0);
	tt->dt=sll_reallocate((void*)(tt->dt),tt->l*sizeof(const sll_object_type_data_t*));
	sll_object_type_data_t* n=sll_allocate(sizeof(sll_object_type_data_t)+l*sizeof(sll_object_type_data_entry_t));
	n->sz=0;
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
		if (n->dt[i].t<=SLL_MAX_OBJECT_TYPE){
			switch (n->dt[i].t){
				case SLL_OBJECT_TYPE_INT:
				case SLL_OBJECT_TYPE_CHAR:
					n->sz+=sizeof(sll_integer_t);
					break;
				case SLL_OBJECT_TYPE_FLOAT:
					n->sz+=sizeof(sll_float_t);
					break;
				case SLL_OBJECT_TYPE_STRING:
					n->sz+=sizeof(sll_string_t);
					break;
				case SLL_OBJECT_TYPE_ARRAY:
				case SLL_OBJECT_TYPE_MAP_KEYS:
				case SLL_OBJECT_TYPE_MAP_VALUES:
					n->dt[i].t=SLL_OBJECT_TYPE_ARRAY;
					n->sz+=sizeof(sll_array_t);
					break;
				case SLL_OBJECT_TYPE_HANDLE:
					n->sz+=sizeof(sll_handle_data_t);
					break;
				case SLL_OBJECT_TYPE_MAP:
					n->sz+=sizeof(sll_map_t);
					break;
			}
		}
		else if (n->dt[i].t<=tt->l+SLL_MAX_OBJECT_TYPE-1){
			n->sz+=(*(tt->dt+n->dt[i].t-SLL_MAX_OBJECT_TYPE-1))->sz;
		}
		else{
			n->dt[i].t=SLL_OBJECT_TYPE_INT;
			n->sz+=sizeof(sll_integer_t);
		}
		p++;
		v=sll_operator_cast((sll_object_t*)(*p),sll_static_int[SLL_OBJECT_TYPE_STRING]);
		sll_string_clone(&(v->dt.s),&(n->dt[i].nm));
		SLL_RELEASE(v);
		p++;
	}
	*(tt->dt+tt->l-1)=n;
	return tt->l+SLL_MAX_OBJECT_TYPE;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_create_new_object_type(sll_object_type_table_t* tt){
	tt->l++;
	SLL_ASSERT(tt->l+SLL_MAX_OBJECT_TYPE<SLL_OBJECT_TYPE_RESERVED0);
	tt->dt=sll_reallocate((void*)(tt->dt),tt->l*sizeof(const sll_object_type_data_t*));
	sll_object_type_data_t* n=sll_allocate(sizeof(sll_object_type_data_t));
	n->sz=0;
	n->l=0;
	*(tt->dt+tt->l-1)=n;
	sll_object_t* o=SLL_CREATE();
	o->t=tt->l+SLL_MAX_OBJECT_TYPE;
	o->dt.p=NULL;
	return o;
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
	const sll_object_type_data_t* dt=*(tt->dt+t-SLL_MAX_OBJECT_TYPE-1);
	sll_object_t* o=SLL_CREATE();
	o->t=t;
	o->dt.p=sll_allocate(dt->sz);
	_init_struct(tt,t,o->dt.p,p,l);
	return o;
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
