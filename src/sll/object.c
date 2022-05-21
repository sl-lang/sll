#include <sll/_internal/common.h>
#include <sll/_internal/static_object.h>
#include <sll/_internal/static_string.h>
#include <sll/_internal/string.h>
#include <sll/array.h>
#include <sll/common.h>
#include <sll/complex.h>
#include <sll/data.h>
#include <sll/gc.h>
#include <sll/map.h>
#include <sll/memory.h>
#include <sll/object.h>
#include <sll/operator.h>
#include <sll/platform/util.h>
#include <sll/static_object.h>
#include <sll/string.h>
#include <sll/types.h>
#include <sll/vm.h>



#define GET_HASH_TABLE_OFFSET(dt,str) (ROTATE_BITS_RIGHT64((str)->c*(dt)->_rng,(dt)->_rng&63)&(dt)->_hash_table_bit_mask)



static __STATIC_STRING(_object_copy_str,"@@copy@@");
static __STATIC_STRING(_object_delete_str,"@@delete@@");
static __STATIC_STRING(_object_init_str,"@@init@@");
static __STATIC_STRING(_object_string_str,"@@string@@");
static __STATIC_STRING(_object_int_type_str,"int_type");
static __STATIC_STRING(_object_float_type_str,"float_type");
static __STATIC_STRING(_object_char_type_str,"char_type");
static __STATIC_STRING(_object_complex_type_str,"complex_type");
static __STATIC_STRING(_object_string_type_str,"string_type");
static __STATIC_STRING(_object_array_type_str,"array_type");
static __STATIC_STRING(_object_map_type_str,"map_type");
static const sll_string_t* _object_fn_list[SLL_MAX_OBJECT_FUNC+1]={
	&_object_init_str,
	&_object_delete_str,
	&_object_copy_str,
	&_object_string_str
};



static void _init_struct(const sll_object_type_table_t* tt,sll_object_t* o,sll_object_t*const* s,sll_arg_count_t l);



static void _zero_struct(const sll_object_type_table_t* tt,const sll_object_type_data_t* dt,sll_object_field_t* p,sll_arg_count_t i){
	while (i<dt->l){
		switch (dt->dt[i].t){
			case SLL_OBJECT_TYPE_INT:
				p->int_=0;
				break;
			case SLL_OBJECT_TYPE_FLOAT:
				p->float_=0;
				break;
			case SLL_OBJECT_TYPE_CHAR:
				p->char_=0;
				break;
			case SLL_OBJECT_TYPE_COMPLEX:
				p->any=SLL_ACQUIRE_STATIC(complex_zero);
				break;
			case SLL_OBJECT_TYPE_STRING:
				p->any=EMPTY_STRING_TO_OBJECT();
				break;
			case SLL_OBJECT_TYPE_ARRAY:
			case SLL_OBJECT_TYPE_MAP_KEYS:
			case SLL_OBJECT_TYPE_MAP_VALUES:
				p->any=sll_array_to_object(NULL);
				break;
			case SLL_OBJECT_TYPE_MAP:
				p->any=sll_map_to_object(NULL);
				break;
			case SLL_OBJECT_TYPE_OBJECT:
				p->any=SLL_ACQUIRE_STATIC_INT(0);
				break;
			default:
				{
					sll_object_t* n=sll_create_object(dt->dt[i].t);
					const sll_object_type_data_t* n_dt=*(tt->dt+n->t-SLL_MAX_OBJECT_TYPE-1);
					n->dt.p=sll_allocate(n_dt->l*sizeof(sll_object_field_t));
					_zero_struct(tt,n_dt,n->dt.p,0);
					if (n_dt->fn[SLL_OBJECT_FUNC_INIT]){
						SLL_RELEASE(sll_execute_function(n_dt->fn[SLL_OBJECT_FUNC_INIT],&n,1,0));
					}
					p->any=n;
					break;
				}
		}
		i++;
		p++;
	}
}



static void _set_field(const sll_object_type_table_t* tt,sll_object_field_t* o,sll_object_type_t t,sll_object_t* v){
	if ((t>SLL_MAX_OBJECT_TYPE&&v->t==t)||t==SLL_OBJECT_TYPE_OBJECT){
		SLL_ACQUIRE(v);
		o->any=v;
		return;
	}
	v=sll_operator_cast(v,sll_static_int[(t<=SLL_MAX_OBJECT_TYPE?t:SLL_OBJECT_TYPE_ARRAY)]);
	switch (t){
		case SLL_OBJECT_TYPE_INT:
			o->int_=v->dt.i;
			break;
		case SLL_OBJECT_TYPE_FLOAT:
			o->float_=v->dt.f;
			break;
		case SLL_OBJECT_TYPE_CHAR:
			o->char_=v->dt.c;
			break;
		case SLL_OBJECT_TYPE_COMPLEX:
		case SLL_OBJECT_TYPE_STRING:
		case SLL_OBJECT_TYPE_ARRAY:
		case SLL_OBJECT_TYPE_MAP:
		case SLL_OBJECT_TYPE_MAP_KEYS:
		case SLL_OBJECT_TYPE_MAP_VALUES:
			o->any=v;
			return;
		default:
			{
				sll_object_t* n=sll_create_object(t);
				const sll_object_type_data_t* dt=*(tt->dt+t-SLL_MAX_OBJECT_TYPE-1);
				n->dt.p=sll_allocate(dt->l*sizeof(sll_object_field_t));
				_init_struct(tt,n,v->dt.a.data,v->dt.a.length);
				o->any=n;
				break;
			}
	}
	SLL_RELEASE(v);
}



static void _init_struct(const sll_object_type_table_t* tt,sll_object_t* o,sll_object_t*const* s,sll_arg_count_t l){
	SLL_ASSERT(o->t>SLL_MAX_OBJECT_TYPE);
	const sll_object_type_data_t* dt=*(tt->dt+o->t-SLL_MAX_OBJECT_TYPE-1);
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
	if (dt->fn[SLL_OBJECT_FUNC_INIT]){
		SLL_RELEASE(sll_execute_function(dt->fn[SLL_OBJECT_FUNC_INIT],&o,1,0));
	}
}



static sll_arg_count_t _get_offset(const sll_object_type_data_t* dt,const sll_string_t* f){
	if (!dt->l){
		return SLL_MAX_ARG_COUNT;
	}
	if (dt->l==1){
		return (STRING_EQUAL(f,&(dt->dt->nm))?0:SLL_MAX_ARG_COUNT);
	}
	sll_arg_count_t i=GET_HASH_TABLE_OFFSET(dt,f);
	sll_arg_count_t l=dt->_hash_table_bit_mask+1;
	do{
		sll_arg_count_t j=dt->_hash_table[i];
		if (j!=SLL_MAX_ARG_COUNT&&STRING_EQUAL(f,&(dt->dt[j].nm))){
			return j;
		}
		l--;
		if (!i){
			i=dt->_hash_table_bit_mask;
		}
		else{
			i--;
		}
	} while (l);
	return SLL_MAX_ARG_COUNT;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_type_t sll_add_type(sll_object_type_table_t* tt,sll_object_t*const* p,sll_arg_count_t l,const sll_string_t* nm){
	sll_object_type_data_t* n=sll_allocate(sizeof(sll_object_type_data_t)+l*sizeof(sll_object_type_data_entry_t));
	if (nm){
		sll_string_clone(nm,(sll_string_t*)(&(n->nm)));
	}
	else{
		SLL_INIT_STRING((sll_string_t*)(&(n->nm)));
	}
	sll_zero_memory(n->fn,(SLL_MAX_OBJECT_FUNC+1)*sizeof(sll_integer_t));
	sll_arg_count_t i=0;
	while (i<l){
		sll_object_t* v=sll_operator_cast((sll_object_t*)(*p),sll_static_int[SLL_OBJECT_TYPE_INT]);
		sll_integer_t vv=v->dt.i;
		SLL_RELEASE(v);
		n->dt[i].c=0;
		if (vv<0){
			n->dt[i].c=1;
			vv=~vv;
		}
		SLL_ASSERT(vv>=0);
		n->dt[i].t=(vv!=SLL_OBJECT_TYPE_OBJECT&&vv>tt->l+SLL_MAX_OBJECT_TYPE?SLL_OBJECT_TYPE_INT:(sll_object_type_t)vv);
		p++;
		v=sll_operator_cast((sll_object_t*)(*p),sll_static_int[SLL_OBJECT_TYPE_STRING]);
		p++;
		sll_string_t str=v->dt.s;
		SLL_RELEASE(v);
		if (str.l>4&&str.v[0]=='@'&&str.v[1]=='@'&&str.v[str.l-2]=='@'&&str.v[str.l-1]=='@'){
			for (sll_object_function_index_t j=0;j<=SLL_MAX_OBJECT_FUNC;j++){
				if (STRING_EQUAL(&str,_object_fn_list[j])){
					n->fn[j]=(n->dt[i].c?~vv:vv);
					l--;
					goto _skip_next;
				}
			}
		}
		sll_string_clone(&str,&(n->dt[i].nm));
		i++;
_skip_next:;
	}
	n->l=l;
	n->_hash_table_bit_mask=(l<2?0:1<<(FIND_LAST_SET_BIT(l)+(!!(l&(l-1)))));
	n=sll_reallocate(n,sizeof(sll_object_type_data_t)+l*sizeof(sll_object_type_data_entry_t)+n->_hash_table_bit_mask*sizeof(sll_arg_count_t));
	if (n->_hash_table_bit_mask){
		n->_hash_table=PTR(ADDR(n)+sizeof(sll_object_type_data_t)+l*sizeof(sll_object_type_data_entry_t));
		sll_set_memory(n->_hash_table,n->_hash_table_bit_mask*sizeof(sll_arg_count_t),0xff);
		n->_hash_table_bit_mask--;
		sll_platform_random(&(n->_rng),sizeof(__SLL_U64));
		if (!(n->_rng>>6)){
			n->_rng|=64;
		}
		sll_bool_t extra=0;
		for (i=0;i<l;i++){
			sll_arg_count_t j=GET_HASH_TABLE_OFFSET(n,&(n->dt[i].nm));
			if (n->_hash_table[j]==SLL_MAX_ARG_COUNT){
				n->_hash_table[j]=i;
			}
			else{
				extra=1;
			}
		}
		if (extra){
			for (i=0;i<l;i++){
				sll_arg_count_t j=GET_HASH_TABLE_OFFSET(n,&(n->dt[i].nm));
				if (n->_hash_table[j]==i){
					continue;
				}
				SLL_ASSERT(n->_hash_table[j]!=SLL_MAX_ARG_COUNT);
				do{
					j=(j?j-1:n->_hash_table_bit_mask);
				} while (n->_hash_table[j]!=SLL_MAX_ARG_COUNT);
				n->_hash_table[j]=i;
			}
		}
	}
	tt->l++;
	SLL_ASSERT(tt->l+SLL_MAX_OBJECT_TYPE<SLL_OBJECT_TYPE_OBJECT);
	tt->dt=sll_reallocate(PTR(tt->dt),tt->l*sizeof(const sll_object_type_data_t*));
	*(tt->dt+tt->l-1)=n;
	return tt->l+SLL_MAX_OBJECT_TYPE;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_create_new_object_type(sll_object_type_table_t* tt){
	tt->l++;
	SLL_ASSERT(tt->l+SLL_MAX_OBJECT_TYPE<SLL_OBJECT_TYPE_OBJECT);
	tt->dt=sll_reallocate(PTR(tt->dt),tt->l*sizeof(const sll_object_type_data_t*));
	sll_object_type_data_t* n=sll_allocate(sizeof(sll_object_type_data_t));
	SLL_INIT_STRING((sll_string_t*)(&(n->nm)));
	n->l=0;
	sll_zero_memory(n->fn,(SLL_MAX_OBJECT_FUNC+1)*sizeof(sll_integer_t));
	*(tt->dt+tt->l-1)=n;
	sll_object_t* o=sll_create_object(tt->l+SLL_MAX_OBJECT_TYPE);
	o->dt.p=NULL;
	return o;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_create_object_type(const sll_object_type_table_t* tt,sll_object_type_t t,sll_object_t*const* p,sll_arg_count_t l){
	switch (t){
		case SLL_OBJECT_TYPE_INT:
			return (l?sll_operator_cast(*p,sll_static_int[SLL_OBJECT_TYPE_INT]):SLL_ACQUIRE_STATIC_INT(0));
		case SLL_OBJECT_TYPE_FLOAT:
			return (l?sll_operator_cast(*p,sll_static_int[SLL_OBJECT_TYPE_FLOAT]):SLL_ACQUIRE_STATIC(float_zero));
		case SLL_OBJECT_TYPE_CHAR:
			return (l?sll_operator_cast(*p,sll_static_int[SLL_OBJECT_TYPE_CHAR]):SLL_FROM_CHAR(0));
		case SLL_OBJECT_TYPE_COMPLEX:
			SLL_UNIMPLEMENTED();
		case SLL_OBJECT_TYPE_STRING:
			return (l?sll_operator_cast(*p,sll_static_int[SLL_OBJECT_TYPE_STRING]):EMPTY_STRING_TO_OBJECT());
		case SLL_OBJECT_TYPE_ARRAY:
		case SLL_OBJECT_TYPE_MAP_KEYS:
		case SLL_OBJECT_TYPE_MAP_VALUES:
			{
				sll_object_t* o=sll_array_length_to_object(l);
				for (sll_arg_count_t i=0;i<l;i++){
					o->dt.a.data[i]=*(p+i);
					SLL_ACQUIRE(*(p+i));
				}
				return o;
			}
		case SLL_OBJECT_TYPE_MAP:
			{
				if (!l){
					return sll_map_to_object(NULL);
				}
				sll_object_t* o=sll_map_length_to_object((l+1)>>1);
				for (sll_arg_count_t i=0;i<l;i++){
					o->dt.m.data[i]=*(p+i);
					SLL_ACQUIRE(*(p+i));
				}
				if (l&1){
					o->dt.m.data[l]=SLL_ACQUIRE_STATIC_INT(0);
				}
				return o;
			}
	}
	if (!tt){
		return SLL_ACQUIRE_STATIC_INT(0);
	}
	SLL_ASSERT(t-SLL_MAX_OBJECT_TYPE-1<tt->l);
	sll_object_t* o=sll_create_object(t);
	o->dt.p=sll_allocate((*(tt->dt+t-SLL_MAX_OBJECT_TYPE-1))->l*sizeof(sll_object_field_t));
	_init_struct(tt,o,p,l);
	return o;
}



__SLL_EXTERNAL void sll_free_object_type_list(sll_object_type_table_t* tt){
	if (tt->l){
		for (sll_object_type_table_length_t i=0;i<tt->l;i++){
			const sll_object_type_data_t* k=*(tt->dt+i);
			sll_free_string((sll_string_t*)(&(k->nm)));
			for (sll_arg_count_t j=0;j<k->l;j++){
				sll_free_string((sll_string_t*)(&(k->dt[j].nm)));
			}
			sll_deallocate(PTR(k));
		}
		sll_deallocate(PTR(tt->dt));
		tt->dt=NULL;
		tt->l=0;
	}
}



__SLL_EXTERNAL void sll_get_type_name(sll_object_type_table_t* tt,sll_object_type_t t,sll_string_t* o){
	switch (t){
		case SLL_OBJECT_TYPE_INT:
			sll_string_clone(&_object_int_type_str,o);
			return;
		case SLL_OBJECT_TYPE_FLOAT:
			sll_string_clone(&_object_float_type_str,o);
			return;
		case SLL_OBJECT_TYPE_CHAR:
			sll_string_clone(&_object_char_type_str,o);
			return;
		case SLL_OBJECT_TYPE_COMPLEX:
			sll_string_clone(&_object_complex_type_str,o);
			break;
		case SLL_OBJECT_TYPE_STRING:
			sll_string_clone(&_object_string_type_str,o);
			return;
		case SLL_OBJECT_TYPE_ARRAY:
		case SLL_OBJECT_TYPE_MAP_KEYS:
		case SLL_OBJECT_TYPE_MAP_VALUES:
			sll_string_clone(&_object_array_type_str,o);
			return;
		case SLL_OBJECT_TYPE_MAP:
			sll_string_clone(&_object_map_type_str,o);
			return;
	}
	if (!tt||t-SLL_MAX_OBJECT_TYPE-1>=tt->l){
		SLL_INIT_STRING(o);
		return;
	}
	sll_string_clone(&((*(tt->dt+t-SLL_MAX_OBJECT_TYPE-1))->nm),o);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_object_clone(const sll_object_type_table_t* tt,sll_object_t* o,sll_bool_t d){
	const sll_object_type_data_t* dt=*(tt->dt+o->t-SLL_MAX_OBJECT_TYPE-1);
	sll_object_t* n=sll_create_object(o->t);
	n->dt.p=sll_allocate(dt->l*sizeof(sll_object_field_t));
	const sll_object_field_t* src=o->dt.p;
	sll_object_field_t* dst=n->dt.p;
	for (sll_arg_count_t i=0;i<dt->l;i++){
		switch (dt->dt[i].t){
			case SLL_OBJECT_TYPE_INT:
			case SLL_OBJECT_TYPE_CHAR:
				dst->int_=src->int_;
				break;
			case SLL_OBJECT_TYPE_FLOAT:
				dst->float_=src->float_;
				break;
			case SLL_OBJECT_TYPE_COMPLEX:
				SLL_ACQUIRE(src->any);
				dst->any=src->any;
				break;
			default:
				dst->any=sll_operator_copy(src->any,d);
				break;
		}
		src++;
		dst++;
	}
	if (dt->fn[SLL_OBJECT_FUNC_COPY]){
		SLL_RELEASE(sll_execute_function(dt->fn[SLL_OBJECT_FUNC_COPY],&n,1,0));
	}
	return n;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_object_get_field(const sll_object_type_table_t* tt,sll_object_t* o,const sll_string_t* f){
	const sll_object_type_data_t* dt=*(sll_current_runtime_data->tt->dt+o->t-SLL_MAX_OBJECT_TYPE-1);
	sll_arg_count_t off=_get_offset(dt,f);
	if (off==SLL_MAX_ARG_COUNT){
		return SLL_ACQUIRE_STATIC_INT(0);
	}
	sll_object_field_t* v=o->dt.p+off;
	switch (dt->dt[off].t){
		case SLL_OBJECT_TYPE_INT:
			return sll_int_to_object(v->int_);
		case SLL_OBJECT_TYPE_FLOAT:
			return sll_float_to_object(v->float_);
		case SLL_OBJECT_TYPE_CHAR:
			return SLL_FROM_CHAR(v->char_);
	}
	SLL_ACQUIRE(v->any);
	return v->any;
}



__SLL_EXTERNAL void sll_object_set_field(const sll_object_type_table_t* tt,sll_object_t* o,const sll_string_t* f,sll_object_t* v){
	const sll_object_type_data_t* dt=*(sll_current_runtime_data->tt->dt+o->t-SLL_MAX_OBJECT_TYPE-1);
	sll_arg_count_t off=_get_offset(dt,f);
	if (off==SLL_MAX_ARG_COUNT){
		return;
	}
	if (dt->dt[off].c){
		return;
	}
	if (dt->dt[off].t>SLL_OBJECT_TYPE_CHAR){
		SLL_RELEASE((o->dt.p+off)->any);
	}
	_set_field(tt,o->dt.p+off,dt->dt[off].t,v);
}



__SLL_EXTERNAL void sll_object_to_array(const sll_object_type_table_t* tt,sll_object_t* o,sll_array_t* out){
	sll_object_type_t t=o->t;
	SLL_ASSERT(t>SLL_MAX_OBJECT_TYPE&&t-SLL_MAX_OBJECT_TYPE-1<tt->l);
	const sll_object_type_data_t* dt=*(sll_current_runtime_data->tt->dt+o->t-SLL_MAX_OBJECT_TYPE-1);
	sll_array_create(dt->l,out);
	sll_object_field_t* v=o->dt.p;
	for (sll_arg_count_t i=0;i<dt->l;i++){
		switch (dt->dt[i].t){
			case SLL_OBJECT_TYPE_INT:
				out->data[i]=sll_int_to_object(v->int_);
				break;
			case SLL_OBJECT_TYPE_FLOAT:
				out->data[i]=sll_float_to_object(v->float_);
				break;
			case SLL_OBJECT_TYPE_CHAR:
				out->data[i]=SLL_FROM_CHAR(v->char_);
				break;
			default:
				SLL_ACQUIRE(v->any);
				out->data[i]=v->any;
				break;
		}
		v++;
	}
}



__SLL_EXTERNAL void sll_object_to_map(const sll_object_type_table_t* tt,sll_object_t* o,sll_map_t* out){
	sll_object_type_t t=o->t;
	SLL_ASSERT(t>SLL_MAX_OBJECT_TYPE&&t-SLL_MAX_OBJECT_TYPE-1<tt->l);
	const sll_object_type_data_t* dt=*(tt->dt+o->t-SLL_MAX_OBJECT_TYPE-1);
	sll_map_create(dt->l,out);
	sll_object_field_t* p=o->dt.p;
	for (sll_arg_count_t i=0;i<dt->l;i++){
		out->data[i<<1]=sll_string_to_object(&(dt->dt[i].nm));
		if (dt->dt[i].t==SLL_OBJECT_TYPE_INT){
			out->data[(i<<1)+1]=sll_int_to_object(p->int_);
		}
		else if (dt->dt[i].t==SLL_OBJECT_TYPE_FLOAT){
			out->data[(i<<1)+1]=sll_float_to_object(p->float_);
		}
		else if (dt->dt[i].t==SLL_OBJECT_TYPE_CHAR){
			out->data[(i<<1)+1]=SLL_FROM_CHAR(p->char_);
		}
		else{
			SLL_ACQUIRE(p->any);
			out->data[(i<<1)+1]=p->any;
		}
		p++;
	}
}
