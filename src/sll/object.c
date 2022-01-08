#include <sll/_sll_internal.h>
#include <sll/array.h>
#include <sll/common.h>
#include <sll/map.h>
#include <sll/memory.h>
#include <sll/object.h>
#include <sll/operator.h>
#include <sll/static_object.h>
#include <sll/string.h>
#include <sll/types.h>
#include <sll/util.h>
#include <sll/vm.h>



static sll_string_t _object_copy_str=SLL_INIT_STRING_STRUCT;
static sll_string_t _object_delete_str=SLL_INIT_STRING_STRUCT;
static sll_string_t _object_init_str=SLL_INIT_STRING_STRUCT;



static void _init_struct(const sll_object_type_table_t* tt,sll_object_t* o,sll_object_t*const* s,sll_arg_count_t l);



static void _release_data(void){
	if (_object_copy_str.v){
		sll_free_string(&_object_copy_str);
		sll_free_string(&_object_delete_str);
		sll_free_string(&_object_init_str);
	}
}



static void _zero_struct(const sll_object_type_table_t* tt,const sll_object_type_data_t* dt,sll_object_field_t* p,sll_arg_count_t i){
	while (i<dt->l){
		switch (SLL_OBJECT_GET_TYPE_MASK(dt->dt[i].t)){
			case SLL_OBJECT_TYPE_INT:
				p->i=0;
				break;
			case SLL_OBJECT_TYPE_FLOAT:
				p->f=0;
				break;
			case SLL_OBJECT_TYPE_CHAR:
				p->c=0;
				break;
			case SLL_OBJECT_TYPE_STRING:
				{
					sll_object_t* n=SLL_CREATE();
					n->t=SLL_OBJECT_TYPE_STRING;
					SLL_INIT_STRING(&(n->dt.s));
					p->o=n;
					break;
				}
			case SLL_OBJECT_TYPE_ARRAY:
			case SLL_OBJECT_TYPE_MAP_KEYS:
			case SLL_OBJECT_TYPE_MAP_VALUES:
				{
					sll_object_t* n=SLL_CREATE();
					n->t=SLL_OBJECT_TYPE_ARRAY;
					SLL_INIT_ARRAY(&(n->dt.a));
					p->o=n;
					break;
				}
			case SLL_OBJECT_TYPE_MAP:
				{
					sll_object_t* n=SLL_CREATE();
					n->t=SLL_OBJECT_TYPE_MAP;
					SLL_INIT_MAP(&(n->dt.m));
					p->o=n;
					break;
				}
			default:
				{
					sll_object_t* n=SLL_CREATE();
					n->t=SLL_OBJECT_GET_TYPE_MASK(dt->dt[i].t);
					const sll_object_type_data_t* n_dt=*(tt->dt+n->t-SLL_MAX_OBJECT_TYPE-1);
					n->dt.p=sll_allocate(n_dt->l*sizeof(sll_object_field_t));
					_zero_struct(tt,n_dt,n->dt.p,0);
					if (n_dt->fn.init){
						SLL_RELEASE(sll_execute_function(n_dt->fn.init,&n,1));
					}
					p->o=n;
					break;
				}
		}
		i++;
		p++;
	}
}



static void _set_field(const sll_object_type_table_t* tt,sll_object_field_t* o,sll_object_type_t t,sll_object_t* v){
	t=SLL_OBJECT_GET_TYPE_MASK(t);
	if (t>SLL_MAX_OBJECT_TYPE&&SLL_OBJECT_GET_TYPE(v)==t){
		SLL_ACQUIRE(v);
		o->o=v;
		return;
	}
	v=sll_operator_cast(v,sll_static_int[(t<=SLL_MAX_OBJECT_TYPE?t:SLL_OBJECT_TYPE_ARRAY)]);
	switch (t){
		case SLL_OBJECT_TYPE_INT:
			o->i=v->dt.i;
			break;
		case SLL_OBJECT_TYPE_FLOAT:
			o->f=v->dt.f;
			break;
		case SLL_OBJECT_TYPE_CHAR:
			o->c=v->dt.c;
			break;
		case SLL_OBJECT_TYPE_STRING:
		case SLL_OBJECT_TYPE_ARRAY:
		case SLL_OBJECT_TYPE_MAP:
		case SLL_OBJECT_TYPE_MAP_KEYS:
		case SLL_OBJECT_TYPE_MAP_VALUES:
			o->o=v;
			return;
		default:
			{
				sll_object_t* n=SLL_CREATE();
				n->t=t;
				const sll_object_type_data_t* dt=*(tt->dt+t-SLL_MAX_OBJECT_TYPE-1);
				n->dt.p=sll_allocate(dt->l*sizeof(sll_object_field_t));
				_init_struct(tt,n,v->dt.a.v,v->dt.a.l);
				o->o=n;
				break;
			}
	}
	SLL_RELEASE(v);
}



static void _init_struct(const sll_object_type_table_t* tt,sll_object_t* o,sll_object_t*const* s,sll_arg_count_t l){
	SLL_ASSERT(SLL_OBJECT_GET_TYPE(o)>SLL_MAX_OBJECT_TYPE);
	const sll_object_type_data_t* dt=*(tt->dt+SLL_OBJECT_GET_TYPE(o)-SLL_MAX_OBJECT_TYPE-1);
	sll_object_field_t* d=o->dt.p;
	for (sll_arg_count_t i=0;i<dt->l;i++){
		if (!l){
			_zero_struct(tt,dt,d,i);
			break;
		}
		_set_field(tt,d,dt->dt[i].t,*s);
		l--;
		s++;
		d++;
	}
	if (dt->fn.init){
		SLL_RELEASE(sll_execute_function(dt->fn.init,&o,1));
	}
}



static sll_arg_count_t _get_offset(const sll_object_type_data_t* dt,const sll_string_t* f){
	for (sll_arg_count_t i=0;i<dt->l;i++){
		if (sll_string_equal(f,&(dt->dt[i].nm))){
			return i;
		}
	}
	return SLL_MAX_ARG_COUNT;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_type_t sll_add_initializer(sll_object_type_initializer_table_t* ot_it,const sll_object_type_field_t* p,sll_arg_count_t l){
	ot_it->l++;
	ot_it->dt=sll_reallocate(ot_it->dt,ot_it->l*sizeof(sll_object_type_initializer_t*));
	sll_object_type_initializer_t* n=sll_allocate(sizeof(sll_object_type_initializer_t)+l*sizeof(sll_object_type_field_t));
	n->l=l;
	SLL_ASSERT(l);
	for (sll_arg_count_t i=0;i<l;i++){
		n->dt[i].t=p->t;
		if (SLL_OBJECT_GET_TYPE_MASK(p->t)==SLL_OBJECT_TYPE_MAP_KEYS||SLL_OBJECT_GET_TYPE_MASK(p->t)==SLL_OBJECT_TYPE_MAP_VALUES){
			n->dt[i].t=SLL_OBJECT_TYPE_ARRAY|(p->t&SLL_OBJECT_FLAG_CONSTANT);
		}
		n->dt[i].f=p->f;
		p++;
	}
	*(ot_it->dt+ot_it->l-1)=n;
	return ot_it->l-1;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_type_t sll_add_type(sll_object_type_table_t* tt,const sll_object_t*const* p,sll_arg_count_t l,const sll_string_t* nm){
	tt->l++;
	SLL_ASSERT(tt->l+SLL_MAX_OBJECT_TYPE<SLL_OBJECT_TYPE_RESERVED0);
	tt->dt=sll_reallocate((void*)(tt->dt),tt->l*sizeof(const sll_object_type_data_t*));
	sll_object_type_data_t* n=sll_allocate(sizeof(sll_object_type_data_t)+l*sizeof(sll_object_type_data_entry_t));
	if (nm){
		sll_string_clone(nm,(sll_string_t*)(&(n->nm)));
	}
	else{
		SLL_INIT_STRING((sll_string_t*)(&(n->nm)));
	}
	n->fn.copy=0;
	n->fn.del=0;
	n->fn.init=0;
	for (sll_arg_count_t i=0;i<l;i++){
		sll_object_t* v=sll_operator_cast((sll_object_t*)(*p),sll_static_int[SLL_OBJECT_TYPE_INT]);
		sll_integer_t vv=v->dt.i;
		SLL_RELEASE(v);
		sll_object_type_t fl=0;
		if (vv<0){
			fl=SLL_OBJECT_FLAG_CONSTANT;
			vv=~vv;
		}
		SLL_ASSERT(vv>=0);
		n->dt[i].t=(vv>tt->l+SLL_MAX_OBJECT_TYPE-1?SLL_OBJECT_TYPE_INT:(sll_object_type_t)vv);
		if (n->dt[i].t>tt->l+SLL_MAX_OBJECT_TYPE-1){
			n->dt[i].t=SLL_OBJECT_TYPE_INT;
		}
		p++;
		v=sll_operator_cast((sll_object_t*)(*p),sll_static_int[SLL_OBJECT_TYPE_STRING]);
		p++;
		if (v->dt.s.l>4&&v->dt.s.v[0]=='@'&&v->dt.s.v[1]=='@'&&v->dt.s.v[v->dt.s.l-2]=='@'&&v->dt.s.v[v->dt.s.l-1]=='@'){
			if (!_object_copy_str.v){
				sll_string_from_pointer(SLL_CHAR("@@copy@@"),&_object_copy_str);
				sll_string_from_pointer(SLL_CHAR("@@delete@@"),&_object_delete_str);
				sll_string_from_pointer(SLL_CHAR("@@init@@"),&_object_init_str);
				sll_register_cleanup(_release_data);
			}
			if (sll_string_equal(&(v->dt.s),&_object_copy_str)){
				n->fn.copy=(fl?~vv:vv);
				i--;
				l--;
				continue;
			}
			else if (sll_string_equal(&(v->dt.s),&_object_delete_str)){
				n->fn.del=(fl?~vv:vv);
				i--;
				l--;
				continue;
			}
			else if (sll_string_equal(&(v->dt.s),&_object_init_str)){
				n->fn.init=(fl?~vv:vv);
				i--;
				l--;
				continue;
			}
		}
		sll_string_clone(&(v->dt.s),&(n->dt[i].nm));
		SLL_RELEASE(v);
		n->dt[i].t|=fl;
	}
	n->l=l;
	*(tt->dt+tt->l-1)=sll_reallocate(n,sizeof(sll_object_type_data_t)+l*sizeof(sll_object_type_data_entry_t));
	return tt->l+SLL_MAX_OBJECT_TYPE;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_create_new_object_type(sll_object_type_table_t* tt){
	tt->l++;
	SLL_ASSERT(tt->l+SLL_MAX_OBJECT_TYPE<SLL_OBJECT_TYPE_RESERVED0);
	tt->dt=sll_reallocate((void*)(tt->dt),tt->l*sizeof(const sll_object_type_data_t*));
	sll_object_type_data_t* n=sll_allocate(sizeof(sll_object_type_data_t));
	SLL_INIT_STRING((sll_string_t*)(&(n->nm)));
	n->l=0;
	n->fn.copy=0;
	n->fn.del=0;
	n->fn.init=0;
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
					sll_object_t* o=SLL_CREATE();
					o->t=SLL_OBJECT_TYPE_ARRAY;
					if (!sll_array_create(l,&(o->dt.a))){
						SLL_UNIMPLEMENTED();
					}
					for (sll_arg_count_t i=0;i<l;i++){
						o->dt.a.v[i]=*(p+i);
						SLL_ACQUIRE(*(p+i));
					}
					return o;
				}
			case SLL_OBJECT_TYPE_MAP:
				{
					sll_object_t* o=SLL_CREATE();
					o->t=SLL_OBJECT_TYPE_MAP;
					if (!l){
						sll_map_create(0,&(o->dt.m));
						return o;
					}
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
	if (!tt){
		return NULL;
	}
	SLL_ASSERT(t-SLL_MAX_OBJECT_TYPE-1<tt->l);
	sll_object_t* o=SLL_CREATE();
	o->t=t;
	o->dt.p=sll_allocate((*(tt->dt+t-SLL_MAX_OBJECT_TYPE-1))->l*sizeof(sll_object_field_t));
	_push_call_stack(SLL_CHAR("@sll_create_object_type"),SLL_MAX_STACK_OFFSET);
	_init_struct(tt,o,p,l);
	_pop_call_stack();
	return o;
}



__SLL_EXTERNAL void sll_free_object_type_list(sll_object_type_table_t* tt){
	if (tt->l){
		for (sll_object_type_t i=0;i<tt->l;i++){
			const sll_object_type_data_t* k=*(tt->dt+i);
			sll_free_string((sll_string_t*)(&(k->nm)));
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



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_object_clone(const sll_object_type_table_t* tt,const sll_object_t* o){
	const sll_object_type_data_t* dt=*(tt->dt+SLL_OBJECT_GET_TYPE(o)-SLL_MAX_OBJECT_TYPE-1);
	sll_object_t* n=SLL_CREATE();
	n->t=SLL_OBJECT_GET_TYPE(o);
	n->dt.p=sll_allocate(dt->l*sizeof(sll_object_field_t));
	const sll_object_field_t* s=o->dt.p;
	sll_object_field_t* d=n->dt.p;
	for (sll_arg_count_t i=0;i<dt->l;i++){
		switch (SLL_OBJECT_GET_TYPE_MASK(dt->dt[i].t)){
			case SLL_OBJECT_TYPE_INT:
			case SLL_OBJECT_TYPE_CHAR:
				d->i=s->i;
				break;
			case SLL_OBJECT_TYPE_FLOAT:
				d->f=s->f;
				break;
			default:
				d->o=sll_operator_dup(s->o);
				break;
		}
		s++;
		d++;
	}
	if (dt->fn.copy){
		_push_call_stack(SLL_CHAR("@sll_object_clone"),SLL_MAX_STACK_OFFSET);
		SLL_RELEASE(sll_execute_function(dt->fn.copy,&n,1));
		_pop_call_stack();
	}
	return n;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_object_get_field(const sll_object_type_table_t* tt,const sll_object_t* o,const sll_string_t* f){
	const sll_object_type_data_t* dt=*(sll_current_runtime_data->tt->dt+SLL_OBJECT_GET_TYPE(o)-SLL_MAX_OBJECT_TYPE-1);
	sll_arg_count_t off=_get_offset(dt,f);
	if (off==SLL_MAX_ARG_COUNT){
		return SLL_ACQUIRE_STATIC_INT(0);
	}
	sll_object_field_t* v=o->dt.p+off;
	switch (SLL_OBJECT_GET_TYPE_MASK(dt->dt[off].t)){
		case SLL_OBJECT_TYPE_INT:
			return SLL_FROM_INT(v->i);
		case SLL_OBJECT_TYPE_FLOAT:
			return SLL_FROM_FLOAT(v->f);
		case SLL_OBJECT_TYPE_CHAR:
			return SLL_FROM_CHAR(v->c);
	}
	SLL_ACQUIRE(v->o);
	return v->o;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_type_t sll_object_get_type(const sll_object_t* o){
	return SLL_OBJECT_GET_TYPE(o);
}



__SLL_EXTERNAL void sll_object_set_field(const sll_object_type_table_t* tt,sll_object_t* o,const sll_string_t* f,sll_object_t* v){
	const sll_object_type_data_t* dt=*(sll_current_runtime_data->tt->dt+SLL_OBJECT_GET_TYPE(o)-SLL_MAX_OBJECT_TYPE-1);
	sll_arg_count_t off=_get_offset(dt,f);
	if (off==SLL_MAX_ARG_COUNT){
		return;
	}
	sll_object_type_t t=dt->dt[off].t;
	if (t&SLL_OBJECT_FLAG_CONSTANT){
		return;
	}
	_set_field(tt,o->dt.p+off,SLL_OBJECT_GET_TYPE_MASK(t),v);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_type_t sll_type_from_initializer(sll_object_type_table_t* tt,const sll_string_table_t* st,const sll_object_type_initializer_t* oi,const sll_string_t* nm){
	tt->l++;
	SLL_ASSERT(tt->l+SLL_MAX_OBJECT_TYPE<SLL_OBJECT_TYPE_RESERVED0);
	tt->dt=sll_reallocate((void*)(tt->dt),tt->l*sizeof(const sll_object_type_data_t*));
	sll_object_type_data_t* n=sll_allocate(sizeof(sll_object_type_data_t)+oi->l*sizeof(sll_object_type_data_entry_t));
	if (nm){
		sll_string_clone(nm,(sll_string_t*)(&(n->nm)));
	}
	else{
		SLL_INIT_STRING((sll_string_t*)(&(n->nm)));
	}
	n->l=oi->l;
	n->fn.copy=0;
	n->fn.del=0;
	n->fn.init=0;
	for (sll_arg_count_t i=0;i<n->l;i++){
		n->dt[i].t=oi->dt[i].t;
		if (SLL_OBJECT_GET_TYPE_MASK(n->dt[i].t)>tt->l+SLL_MAX_OBJECT_TYPE-1){
			n->dt[i].t=SLL_OBJECT_TYPE_INT|(oi->dt[i].t&SLL_OBJECT_FLAG_CONSTANT);
		}
		sll_string_clone(st->dt+oi->dt[i].f,&(n->dt[i].nm));
	}
	*(tt->dt+tt->l-1)=n;
	return tt->l+SLL_MAX_OBJECT_TYPE;
}
