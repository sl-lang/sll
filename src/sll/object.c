#include <sll/_sll_internal.h>
#include <sll/array.h>
#include <sll/assembly.h>
#include <sll/common.h>
#include <sll/handle.h>
#include <sll/map.h>
#include <sll/memory.h>
#include <sll/object.h>
#include <sll/operator.h>
#include <sll/static_object.h>
#include <sll/string.h>
#include <sll/types.h>
#include <sll/util.h>



static void* _init_struct(const sll_object_type_table_t* tt,sll_object_type_t t,void* op,sll_object_t*const* p,sll_arg_count_t l);



static void* _zero_struct(const sll_object_type_table_t* tt,void* op,sll_object_type_t t,sll_arg_count_t i){
	const sll_object_type_data_t* dt=*(tt->dt+t-SLL_MAX_OBJECT_TYPE-1);
	while (i<dt->l){
		switch (SLL_OBJECT_GET_TYPE_MASK(dt->dt[i].t)){
			case SLL_OBJECT_TYPE_INT:
				*((sll_integer_t*)op)=0;
				op=(void*)(((uint64_t)op)+sizeof(sll_integer_t));
				break;
			case SLL_OBJECT_TYPE_FLOAT:
				*((sll_float_t*)op)=0;
				op=(void*)(((uint64_t)op)+sizeof(sll_float_t));
				break;
			case SLL_OBJECT_TYPE_CHAR:
				*((sll_integer_t*)op)=0;
				op=(void*)(((uint64_t)op)+sizeof(sll_integer_t));
				break;
			case SLL_OBJECT_TYPE_STRING:
				SLL_INIT_STRING((sll_string_t*)op);
				op=(void*)(((uint64_t)op)+sizeof(sll_string_t));
				break;
			case SLL_OBJECT_TYPE_ARRAY:
			case SLL_OBJECT_TYPE_MAP_KEYS:
			case SLL_OBJECT_TYPE_MAP_VALUES:
				SLL_INIT_ARRAY((sll_array_t*)op);
				op=(void*)(((uint64_t)op)+sizeof(sll_array_t));
				break;
			case SLL_OBJECT_TYPE_HANDLE:
				SLL_INIT_HANDLE_DATA((sll_handle_data_t*)op);
				op=(void*)(((uint64_t)op)+sizeof(sll_handle_data_t));
				break;
			case SLL_OBJECT_TYPE_MAP:
				SLL_INIT_MAP((sll_map_t*)op);
				op=(void*)(((uint64_t)op)+sizeof(sll_map_t));
				break;
			default:
				op=_zero_struct(tt,op,dt->dt[i].t,0);
				break;
		}
		i++;
	}
	return op;
}



static void* _set_field(const sll_object_type_table_t* tt,void* p,sll_object_type_t t,sll_object_t* v){
	t=SLL_OBJECT_GET_TYPE_MASK(t);
	v=sll_operator_cast(v,sll_static_int[(t<=SLL_MAX_OBJECT_TYPE?t:SLL_OBJECT_TYPE_ARRAY)]);
	switch (t){
		case SLL_OBJECT_TYPE_INT:
			*((sll_integer_t*)p)=v->dt.i;
			p=(void*)(((uint64_t)p)+sizeof(sll_integer_t));
			break;
		case SLL_OBJECT_TYPE_FLOAT:
			*((sll_float_t*)p)=v->dt.f;
			p=(void*)(((uint64_t)p)+sizeof(sll_float_t));
			break;
		case SLL_OBJECT_TYPE_CHAR:
			*((sll_integer_t*)p)=v->dt.c;
			p=(void*)(((uint64_t)p)+sizeof(sll_integer_t));
			break;
		case SLL_OBJECT_TYPE_STRING:
			*((sll_string_t*)p)=v->dt.s;
			p=(void*)(((uint64_t)p)+sizeof(sll_string_t));
			break;
		case SLL_OBJECT_TYPE_ARRAY:
		case SLL_OBJECT_TYPE_MAP_KEYS:
		case SLL_OBJECT_TYPE_MAP_VALUES:
			*((sll_array_t*)p)=v->dt.a;
			p=(void*)(((uint64_t)p)+sizeof(sll_array_t));
			break;
		case SLL_OBJECT_TYPE_HANDLE:
			*((sll_handle_data_t*)p)=v->dt.h;
			p=(void*)(((uint64_t)p)+sizeof(sll_handle_data_t));
			break;
		case SLL_OBJECT_TYPE_MAP:
			*((sll_map_t*)p)=v->dt.m;
			p=(void*)(((uint64_t)p)+sizeof(sll_map_t));
			break;
		default:
			p=_init_struct(tt,t,p,v->dt.a.v,v->dt.a.l);
			break;
	}
	SLL_RELEASE(v);
	return p;
}



static void* _init_struct(const sll_object_type_table_t* tt,sll_object_type_t t,void* op,sll_object_t*const* p,sll_arg_count_t l){
	const sll_object_type_data_t* dt=*(tt->dt+t-SLL_MAX_OBJECT_TYPE-1);
	for (sll_arg_count_t i=0;i<dt->l;i++){
		if (!l){
			op=_zero_struct(tt,op,t,i);
			break;
		}
		op=_set_field(tt,op,dt->dt[i].t,*p);
		l--;
		p++;
	}
	return op;
}



static void* _get_offset(const sll_object_t* o,const sll_string_t* f,sll_object_type_t* t){
	const sll_object_type_data_t* dt=*(sll_current_runtime_data->tt->dt+SLL_OBJECT_GET_TYPE(o)-SLL_MAX_OBJECT_TYPE-1);
	void* p=o->dt.p;
	for (sll_arg_count_t i=0;i<dt->l;i++){
		if (sll_string_equal(f,&(dt->dt[i].nm))){
			*t=dt->dt[i].t;
			return p;
		}
		switch (SLL_OBJECT_GET_TYPE_MASK(dt->dt[i].t)){
			case SLL_OBJECT_TYPE_INT:
			case SLL_OBJECT_TYPE_CHAR:
				p=(void*)(((uint64_t)p)+sizeof(sll_integer_t));
				break;
			case SLL_OBJECT_TYPE_FLOAT:
				p=(void*)(((uint64_t)p)+sizeof(sll_float_t));
				break;
			case SLL_OBJECT_TYPE_STRING:
				p=(void*)(((uint64_t)p)+sizeof(sll_string_t));
				break;
			case SLL_OBJECT_TYPE_ARRAY:
			case SLL_OBJECT_TYPE_MAP_KEYS:
			case SLL_OBJECT_TYPE_MAP_VALUES:
				p=(void*)(((uint64_t)p)+sizeof(sll_array_t));
				break;
			case SLL_OBJECT_TYPE_HANDLE:
				p=(void*)(((uint64_t)p)+sizeof(sll_handle_data_t));
				break;
			case SLL_OBJECT_TYPE_MAP:
				p=(void*)(((uint64_t)p)+sizeof(sll_map_t));
				break;
			default:
				p=(void*)(((uint64_t)p)+(*(sll_current_runtime_data->tt->dt+SLL_OBJECT_GET_TYPE_MASK(dt->dt[i].t)-SLL_MAX_OBJECT_TYPE-1))->sz);
				break;
		}
	}
	return NULL;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_type_t sll_add_type(sll_object_type_table_t* tt,const sll_object_t*const* p,sll_arg_count_t l){
	tt->l++;
	SLL_ASSERT(tt->l+SLL_MAX_OBJECT_TYPE<SLL_OBJECT_TYPE_RESERVED0);
	tt->dt=sll_reallocate((void*)(tt->dt),tt->l*sizeof(const sll_object_type_data_t*));
	SLL_CHECK_NO_MEMORY(tt->dt);
	sll_object_type_data_t* n=sll_allocate(sizeof(sll_object_type_data_t)+l*sizeof(sll_object_type_data_entry_t));
	SLL_CHECK_NO_MEMORY(n);
	n->sz=0;
	n->l=l;
	for (sll_arg_count_t i=0;i<l;i++){
		sll_object_t* v=sll_operator_cast((sll_object_t*)(*p),sll_static_int[SLL_OBJECT_TYPE_INT]);
		sll_integer_t vv=v->dt.i;
		SLL_RELEASE(v);
		sll_object_type_t fl=0;
		if (vv<0){
			fl|=SLL_OBJECT_FLAG_CONSTANT;
			vv=~vv;
		}
		SLL_ASSERT(v>=0);
		n->dt[i].t=(vv>tt->l+SLL_MAX_OBJECT_TYPE-1?SLL_OBJECT_TYPE_INT:(sll_object_type_t)vv);
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
		n->dt[i].t|=fl;
		p++;
		v=sll_operator_cast((sll_object_t*)(*p),sll_static_int[SLL_OBJECT_TYPE_STRING]);
		SLL_CHECK_NO_MEMORY(sll_string_clone(&(v->dt.s),&(n->dt[i].nm)));
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
	SLL_CHECK_NO_MEMORY(tt->dt);
	sll_object_type_data_t* n=sll_allocate(sizeof(sll_object_type_data_t));
	SLL_CHECK_NO_MEMORY(n);
	n->sz=0;
	n->l=0;
	*(tt->dt+tt->l-1)=n;
	sll_object_t* o=SLL_CREATE();
	o->t=tt->l+SLL_MAX_OBJECT_TYPE;
	o->dt.p=NULL;
	return o;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_create_object_type(const sll_object_type_table_t* tt,sll_object_type_t t,sll_object_t*const* p,sll_arg_count_t l){
	t=SLL_OBJECT_GET_TYPE_MASK(t);
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
					SLL_CHECK_NO_MEMORY(sll_array_create(l,&(o->dt.a)));
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
					SLL_CHECK_NO_MEMORY(sll_map_create((l+1)>>1,&(o->dt.m)));
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
	SLL_CHECK_NO_MEMORY(o->dt.p);
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



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_object_get_field(const sll_object_t* o,const sll_string_t* f){
	sll_object_type_t t;
	void* p=_get_offset(o,f,&t);
	if (!p){
		return SLL_ACQUIRE_STATIC_INT(0);
	}
	t=SLL_OBJECT_GET_TYPE_MASK(t);
	switch (t){
		case SLL_OBJECT_TYPE_INT:
			return SLL_FROM_INT(*((sll_integer_t*)p));
		case SLL_OBJECT_TYPE_FLOAT:
			return SLL_FROM_FLOAT(*((sll_float_t*)p));
		case SLL_OBJECT_TYPE_CHAR:
			return SLL_FROM_CHAR(*((sll_float_t*)p));
		case SLL_OBJECT_TYPE_STRING:
			{
				sll_object_t* n=SLL_CREATE();
				n->t=SLL_OBJECT_TYPE_STRING;
				SLL_CHECK_NO_MEMORY(sll_string_clone((sll_string_t*)p,&(n->dt.s)));
				return n;
			}
		case SLL_OBJECT_TYPE_ARRAY:
		case SLL_OBJECT_TYPE_MAP_KEYS:
		case SLL_OBJECT_TYPE_MAP_VALUES:
			{
				sll_object_t* n=SLL_CREATE();
				n->t=SLL_OBJECT_TYPE_ARRAY;
				SLL_CHECK_NO_MEMORY(sll_array_clone((sll_array_t*)p,&(n->dt.a)));
				return n;
			}
		case SLL_OBJECT_TYPE_HANDLE:
			{
				sll_object_t* n=SLL_CREATE();
				n->t=SLL_OBJECT_TYPE_HANDLE;
				n->dt.h=*((sll_handle_data_t*)p);
				return n;
			}
		case SLL_OBJECT_TYPE_MAP:
			{
				sll_object_t* n=SLL_CREATE();
				n->t=SLL_OBJECT_TYPE_MAP;
				SLL_CHECK_NO_MEMORY(sll_map_clone((sll_map_t*)p,&(n->dt.m)));
				return n;
			}
	}
	sll_object_t* n=SLL_CREATE();
	n->t=t;
	const sll_object_type_data_t* dt=*(sll_current_runtime_data->tt->dt+t-SLL_MAX_OBJECT_TYPE-1);
	n->dt.p=sll_allocate(dt->sz);
	SLL_CHECK_NO_MEMORY(n->dt.p);
	sll_copy_data(p,dt->sz,n->dt.p);
	return n;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_type_t sll_object_get_type(const sll_object_t* o){
	return SLL_OBJECT_GET_TYPE(o);
}



__SLL_EXTERNAL void sll_object_set_field(sll_object_t* o,const sll_string_t* f,sll_object_t* v){
	sll_object_type_t t;
	void* n=_get_offset(o,f,&t);
	if (!n||(t&SLL_OBJECT_FLAG_CONSTANT)){
		return;
	}
	_set_field(sll_current_runtime_data->tt,n,SLL_OBJECT_GET_TYPE_MASK(t),v);
}
