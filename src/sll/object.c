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



#define GET_HASH_TABLE_OFFSET(type_descriptor,field_name) (ROTATE_BITS_RIGHT64((field_name)->checksum*(type_descriptor)->_rng,(type_descriptor)->_rng&63)&(type_descriptor)->_hash_table_bit_mask)



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



static void _init_struct(const sll_object_type_table_t* type_table,sll_object_t* object,sll_object_t*const* object_data,sll_arg_count_t object_count);



static void _zero_struct(const sll_object_type_table_t* type_table,const sll_object_type_data_t* type_descriptor,sll_object_field_t* field_pointer,sll_arg_count_t field_count){
	while (field_count<type_descriptor->field_count){
		switch (type_descriptor->fields[field_count].type){
			case SLL_OBJECT_TYPE_INT:
				field_pointer->int_=0;
				break;
			case SLL_OBJECT_TYPE_FLOAT:
				field_pointer->float_=0;
				break;
			case SLL_OBJECT_TYPE_CHAR:
				field_pointer->char_=0;
				break;
			case SLL_OBJECT_TYPE_COMPLEX:
				field_pointer->any=SLL_ACQUIRE_STATIC(complex_zero);
				break;
			case SLL_OBJECT_TYPE_STRING:
				field_pointer->any=EMPTY_STRING_TO_OBJECT();
				break;
			case SLL_OBJECT_TYPE_ARRAY:
			case SLL_OBJECT_TYPE_MAP_KEYS:
			case SLL_OBJECT_TYPE_MAP_VALUES:
				field_pointer->any=sll_array_to_object(NULL);
				break;
			case SLL_OBJECT_TYPE_MAP:
				field_pointer->any=sll_map_to_object(NULL);
				break;
			case SLL_OBJECT_TYPE_OBJECT:
				field_pointer->any=SLL_ACQUIRE_STATIC_INT(0);
				break;
			default:
				{
					sll_object_t* n=sll_create_object(type_descriptor->fields[field_count].type);
					const sll_object_type_data_t* n_dt=*(type_table->data+n->type-SLL_MAX_OBJECT_TYPE-1);
					n->data.fields=sll_allocate(n_dt->field_count*sizeof(sll_object_field_t));
					_zero_struct(type_table,n_dt,n->data.fields,0);
					if (n_dt->functions[SLL_OBJECT_FUNC_INIT]){
						SLL_RELEASE(sll_execute_function(n_dt->functions[SLL_OBJECT_FUNC_INIT],&n,1,0));
					}
					field_pointer->any=n;
					break;
				}
		}
		field_count++;
		field_pointer++;
	}
}



static void _set_field(const sll_object_type_table_t* type_table,sll_object_field_t* field,sll_object_type_t type,sll_object_t* value){
	if ((type>SLL_MAX_OBJECT_TYPE&&value->type==type)||type==SLL_OBJECT_TYPE_OBJECT){
		SLL_ACQUIRE(value);
		field->any=value;
		return;
	}
	value=sll_operator_cast(value,sll_static_int[(type<=SLL_MAX_OBJECT_TYPE?type:SLL_OBJECT_TYPE_ARRAY)]);
	switch (type){
		case SLL_OBJECT_TYPE_INT:
			field->int_=value->data.int_;
			break;
		case SLL_OBJECT_TYPE_FLOAT:
			field->float_=value->data.float_;
			break;
		case SLL_OBJECT_TYPE_CHAR:
			field->char_=value->data.char_;
			break;
		case SLL_OBJECT_TYPE_COMPLEX:
		case SLL_OBJECT_TYPE_STRING:
		case SLL_OBJECT_TYPE_ARRAY:
		case SLL_OBJECT_TYPE_MAP:
		case SLL_OBJECT_TYPE_MAP_KEYS:
		case SLL_OBJECT_TYPE_MAP_VALUES:
			field->any=value;
			return;
		default:
			{
				sll_object_t* n=sll_create_object(type);
				const sll_object_type_data_t* dt=*(type_table->data+type-SLL_MAX_OBJECT_TYPE-1);
				n->data.fields=sll_allocate(dt->field_count*sizeof(sll_object_field_t));
				_init_struct(type_table,n,value->data.array.data,value->data.array.length);
				field->any=n;
				break;
			}
	}
	SLL_RELEASE(value);
}



static void _init_struct(const sll_object_type_table_t* type_table,sll_object_t* object,sll_object_t*const* object_data,sll_arg_count_t object_count){
	SLL_ASSERT(object->type>SLL_MAX_OBJECT_TYPE);
	const sll_object_type_data_t* dt=*(type_table->data+object->type-SLL_MAX_OBJECT_TYPE-1);
	sll_object_field_t* d=object->data.fields;
	for (sll_arg_count_t i=0;i<dt->field_count;i++){
		if (!object_count){
			_zero_struct(type_table,dt,d,i);
			break;
		}
		_set_field(type_table,d,dt->fields[i].type,*object_data);
		object_count--;
		object_data++;
		d++;
	}
	if (dt->functions[SLL_OBJECT_FUNC_INIT]){
		SLL_RELEASE(sll_execute_function(dt->functions[SLL_OBJECT_FUNC_INIT],&object,1,0));
	}
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_type_t sll_add_type(sll_object_type_table_t* type_table,sll_object_t*const* object_data,sll_arg_count_t field_count,const sll_string_t* name){
	sll_object_type_t current_type=type_table->length+SLL_MAX_OBJECT_TYPE+1;
	sll_object_type_data_t* n=sll_allocate(sizeof(sll_object_type_data_t)+field_count*sizeof(sll_object_type_data_field_t));
	if (name){
		sll_string_clone(name,(sll_string_t*)(&(n->name)));
	}
	else{
		SLL_INIT_STRING((sll_string_t*)(&(n->name)));
	}
	sll_zero_memory(n->functions,(SLL_MAX_OBJECT_FUNC+1)*sizeof(sll_integer_t));
	sll_arg_count_t i=0;
	while (i<field_count){
		sll_object_t* v=sll_operator_cast((sll_object_t*)(*object_data),sll_static_int[SLL_OBJECT_TYPE_INT]);
		sll_integer_t vv=v->data.int_;
		SLL_RELEASE(v);
		n->fields[i].read_only=0;
		if (vv<0){
			n->fields[i].read_only=1;
			vv=~vv;
		}
		SLL_ASSERT(vv>=0);
		if (vv==SLL_OBJECT_TYPE_SELF){
			n->fields[i].type=current_type;
		}
		else if (vv!=SLL_OBJECT_TYPE_OBJECT&&vv>type_table->length+SLL_MAX_OBJECT_TYPE){
			n->fields[i].type=SLL_OBJECT_TYPE_INT;
		}
		else{
			n->fields[i].type=(sll_object_type_t)vv;
		}
		object_data++;
		v=sll_operator_cast((sll_object_t*)(*object_data),sll_static_int[SLL_OBJECT_TYPE_STRING]);
		object_data++;
		sll_string_t str=v->data.string;
		SLL_RELEASE(v);
		if (str.length>4&&str.data[0]=='@'&&str.data[1]=='@'&&str.data[str.length-2]=='@'&&str.data[str.length-1]=='@'){
			for (sll_object_function_index_t j=0;j<=SLL_MAX_OBJECT_FUNC;j++){
				if (STRING_EQUAL(&str,_object_fn_list[j])){
					n->functions[j]=(n->fields[i].read_only?~vv:vv);
					field_count--;
					goto _skip_next;
				}
			}
		}
		sll_string_clone(&str,&(n->fields[i].name));
		i++;
_skip_next:;
	}
	n->field_count=field_count;
	n->_hash_table_bit_mask=(field_count<2?0:1<<(FIND_LAST_SET_BIT(field_count)+(!!(field_count&(field_count-1)))));
	n=sll_reallocate(n,sizeof(sll_object_type_data_t)+field_count*sizeof(sll_object_type_data_field_t)+n->_hash_table_bit_mask*sizeof(sll_arg_count_t));
	if (n->_hash_table_bit_mask){
		n->_hash_table=PTR(ADDR(n)+sizeof(sll_object_type_data_t)+field_count*sizeof(sll_object_type_data_field_t));
		sll_set_memory(n->_hash_table,n->_hash_table_bit_mask*sizeof(sll_arg_count_t),0xff);
		n->_hash_table_bit_mask--;
		SLL_RANDOM_BITS(n->_rng);
		if (!(n->_rng>>6)){
			n->_rng|=64;
		}
		sll_bool_t extra=0;
		for (i=0;i<field_count;i++){
			sll_arg_count_t j=GET_HASH_TABLE_OFFSET(n,&(n->fields[i].name));
			if (n->_hash_table[j]==SLL_MAX_ARG_COUNT){
				n->_hash_table[j]=i;
			}
			else{
				extra=1;
			}
		}
		if (extra){
			for (i=0;i<field_count;i++){
				sll_arg_count_t j=GET_HASH_TABLE_OFFSET(n,&(n->fields[i].name));
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
	type_table->length++;
	SLL_ASSERT(type_table->length+SLL_MAX_OBJECT_TYPE<SLL_OBJECT_TYPE_SELF);
	type_table->data=sll_reallocate(PTR(type_table->data),type_table->length*sizeof(const sll_object_type_data_t*));
	*(type_table->data+type_table->length-1)=n;
	return type_table->length+SLL_MAX_OBJECT_TYPE;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_create_new_object_type(sll_object_type_table_t* type_table){
	type_table->length++;
	SLL_ASSERT(type_table->length+SLL_MAX_OBJECT_TYPE<SLL_OBJECT_TYPE_SELF);
	type_table->data=sll_reallocate(PTR(type_table->data),type_table->length*sizeof(const sll_object_type_data_t*));
	sll_object_type_data_t* n=sll_allocate(sizeof(sll_object_type_data_t));
	SLL_INIT_STRING((sll_string_t*)(&(n->name)));
	n->field_count=0;
	sll_zero_memory(n->functions,(SLL_MAX_OBJECT_FUNC+1)*sizeof(sll_integer_t));
	*(type_table->data+type_table->length-1)=n;
	sll_object_t* o=sll_create_object(type_table->length+SLL_MAX_OBJECT_TYPE);
	o->data.fields=NULL;
	return o;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_create_object_type(const sll_object_type_table_t* type_table,sll_object_type_t type,sll_object_t*const* args,sll_arg_count_t arg_count){
	switch (type){
		case SLL_OBJECT_TYPE_INT:
			return (arg_count?sll_operator_cast(*args,sll_static_int[SLL_OBJECT_TYPE_INT]):SLL_ACQUIRE_STATIC_INT(0));
		case SLL_OBJECT_TYPE_FLOAT:
			return (arg_count?sll_operator_cast(*args,sll_static_int[SLL_OBJECT_TYPE_FLOAT]):SLL_ACQUIRE_STATIC(float_zero));
		case SLL_OBJECT_TYPE_CHAR:
			return (arg_count?sll_operator_cast(*args,sll_static_int[SLL_OBJECT_TYPE_CHAR]):SLL_FROM_CHAR(0));
		case SLL_OBJECT_TYPE_COMPLEX:
			{
				if (!arg_count){
					return SLL_ACQUIRE_STATIC(complex_zero);
				}
				if (arg_count==1){
					return sll_operator_cast(*args,sll_static_int[SLL_OBJECT_TYPE_COMPLEX]);
				}
				sll_object_t* real=sll_operator_cast(*args,sll_static_int[SLL_OBJECT_TYPE_FLOAT]);
				sll_object_t* imag=sll_operator_cast(*(args+1),sll_static_int[SLL_OBJECT_TYPE_FLOAT]);
				sll_complex_t v={
					real->data.float_,
					imag->data.float_
				};
				SLL_RELEASE(real);
				SLL_RELEASE(imag);
				return sll_complex_to_object(&v);
			}
		case SLL_OBJECT_TYPE_STRING:
			return (arg_count?sll_operator_cast(*args,sll_static_int[SLL_OBJECT_TYPE_STRING]):EMPTY_STRING_TO_OBJECT());
		case SLL_OBJECT_TYPE_ARRAY:
		case SLL_OBJECT_TYPE_MAP_KEYS:
		case SLL_OBJECT_TYPE_MAP_VALUES:
			{
				sll_object_t* o=sll_array_length_to_object(arg_count);
				sll_copy_objects(args,arg_count,o->data.array.data);
				return o;
			}
		case SLL_OBJECT_TYPE_MAP:
			{
				if (!arg_count){
					return sll_map_to_object(NULL);
				}
				sll_object_t* o=sll_map_length_to_object((arg_count+1)>>1);
				sll_copy_objects(args,arg_count,o->data.map.data);
				if (arg_count&1){
					o->data.map.data[arg_count]=SLL_ACQUIRE_STATIC_INT(0);
				}
				return o;
			}
	}
	if (!type_table){
		return SLL_ACQUIRE_STATIC_INT(0);
	}
	SLL_ASSERT(type-SLL_MAX_OBJECT_TYPE-1<type_table->length);
	sll_object_t* o=sll_create_object(type);
	o->data.fields=sll_allocate((*(type_table->data+type-SLL_MAX_OBJECT_TYPE-1))->field_count*sizeof(sll_object_field_t));
	_init_struct(type_table,o,args,arg_count);
	return o;
}



__SLL_EXTERNAL void sll_free_object_type_list(sll_object_type_table_t* type_table){
	if (type_table->length){
		for (sll_object_type_table_length_t i=0;i<type_table->length;i++){
			const sll_object_type_data_t* k=*(type_table->data+i);
			sll_free_string((sll_string_t*)(&(k->name)));
			for (sll_arg_count_t j=0;j<k->field_count;j++){
				sll_free_string((sll_string_t*)(&(k->fields[j].name)));
			}
			sll_deallocate(PTR(k));
		}
		sll_deallocate(PTR(type_table->data));
		type_table->data=NULL;
		type_table->length=0;
	}
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_arg_count_t sll_get_offset(const sll_object_type_table_t* type_table,sll_object_type_t type,const sll_string_t* field_name){
	SLL_ASSERT(type>SLL_MAX_OBJECT_TYPE&&type-SLL_MAX_OBJECT_TYPE-1<type_table->length);
	const sll_object_type_data_t* type_descriptor=*(type_table->data+type-SLL_MAX_OBJECT_TYPE-1);
	if (!type_descriptor->field_count){
		return SLL_MAX_ARG_COUNT;
	}
	if (type_descriptor->field_count==1){
		return (STRING_EQUAL(field_name,&(type_descriptor->fields->name))?0:SLL_MAX_ARG_COUNT);
	}
	sll_arg_count_t i=GET_HASH_TABLE_OFFSET(type_descriptor,field_name);
	sll_arg_count_t l=type_descriptor->_hash_table_bit_mask+1;
	do{
		sll_arg_count_t j=type_descriptor->_hash_table[i];
		if (j!=SLL_MAX_ARG_COUNT&&STRING_EQUAL(field_name,&(type_descriptor->fields[j].name))){
			return j;
		}
		l--;
		if (!i){
			i=type_descriptor->_hash_table_bit_mask;
		}
		else{
			i--;
		}
	} while (l);
	return SLL_MAX_ARG_COUNT;
}



__SLL_EXTERNAL void sll_get_type_name(const sll_object_type_table_t* type_table,sll_object_type_t type,sll_string_t* out){
	switch (type){
		case SLL_OBJECT_TYPE_INT:
			sll_string_clone(&_object_int_type_str,out);
			return;
		case SLL_OBJECT_TYPE_FLOAT:
			sll_string_clone(&_object_float_type_str,out);
			return;
		case SLL_OBJECT_TYPE_CHAR:
			sll_string_clone(&_object_char_type_str,out);
			return;
		case SLL_OBJECT_TYPE_COMPLEX:
			sll_string_clone(&_object_complex_type_str,out);
			break;
		case SLL_OBJECT_TYPE_STRING:
			sll_string_clone(&_object_string_type_str,out);
			return;
		case SLL_OBJECT_TYPE_ARRAY:
		case SLL_OBJECT_TYPE_MAP_KEYS:
		case SLL_OBJECT_TYPE_MAP_VALUES:
			sll_string_clone(&_object_array_type_str,out);
			return;
		case SLL_OBJECT_TYPE_MAP:
			sll_string_clone(&_object_map_type_str,out);
			return;
	}
	if (!type_table||type-SLL_MAX_OBJECT_TYPE-1>=type_table->length){
		SLL_INIT_STRING(out);
		return;
	}
	sll_string_clone(&((*(type_table->data+type-SLL_MAX_OBJECT_TYPE-1))->name),out);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_object_clone(const sll_object_type_table_t* type_table,sll_object_t* object,sll_bool_t deep){
	const sll_object_type_data_t* dt=*(type_table->data+object->type-SLL_MAX_OBJECT_TYPE-1);
	sll_object_t* n=sll_create_object(object->type);
	n->data.fields=sll_allocate(dt->field_count*sizeof(sll_object_field_t));
	const sll_object_field_t* src=object->data.fields;
	sll_object_field_t* dst=n->data.fields;
	for (sll_arg_count_t i=0;i<dt->field_count;i++){
		switch (dt->fields[i].type){
			case SLL_OBJECT_TYPE_INT:
			case SLL_OBJECT_TYPE_CHAR:
				dst->int_=src->int_;
				break;
			case SLL_OBJECT_TYPE_FLOAT:
				dst->float_=src->float_;
				break;
			default:
				if (deep){
					dst->any=sll_operator_copy(src->any,deep);
					break;
				}
			case SLL_OBJECT_TYPE_COMPLEX:
				SLL_ACQUIRE(src->any);
				dst->any=src->any;
				break;
		}
		src++;
		dst++;
	}
	if (dt->functions[SLL_OBJECT_FUNC_COPY]){
		SLL_RELEASE(sll_execute_function(dt->functions[SLL_OBJECT_FUNC_COPY],&n,1,0));
	}
	return n;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_object_get_field(const sll_object_type_table_t* type_table,sll_object_t* object,const sll_string_t* field_name){
	sll_arg_count_t off=sll_get_offset(type_table,object->type,field_name);
	if (off==SLL_MAX_ARG_COUNT){
		return SLL_ACQUIRE_STATIC_INT(0);
	}
	sll_object_field_t* v=object->data.fields+off;
	switch ((*(type_table->data+object->type-SLL_MAX_OBJECT_TYPE-1))->fields[off].type){
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



__SLL_EXTERNAL void sll_object_set_field(const sll_object_type_table_t* type_table,sll_object_t* object,const sll_string_t* field_name,sll_object_t* value){
	sll_arg_count_t off=sll_get_offset(type_table,object->type,field_name);
	if (off==SLL_MAX_ARG_COUNT){
		return;
	}
	const sll_object_type_data_t* dt=*(type_table->data+object->type-SLL_MAX_OBJECT_TYPE-1);
	if (dt->fields[off].read_only){
		return;
	}
	if (dt->fields[off].type>SLL_OBJECT_TYPE_CHAR){
		SLL_RELEASE((object->data.fields+off)->any);
	}
	_set_field(type_table,object->data.fields+off,dt->fields[off].type,value);
}



__SLL_EXTERNAL void sll_object_to_array(const sll_object_type_table_t* type_table,sll_object_t* object,sll_array_t* out){
	SLL_ASSERT(object->type>SLL_MAX_OBJECT_TYPE&&object->type-SLL_MAX_OBJECT_TYPE-1<type_table->length);
	const sll_object_type_data_t* dt=*(type_table->data+object->type-SLL_MAX_OBJECT_TYPE-1);
	sll_array_create(dt->field_count,out);
	sll_object_field_t* v=object->data.fields;
	for (sll_arg_count_t i=0;i<dt->field_count;i++){
		switch (dt->fields[i].type){
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



__SLL_EXTERNAL void sll_object_to_map(const sll_object_type_table_t* type_table,sll_object_t* object,sll_map_t* out){
	SLL_ASSERT(object->type>SLL_MAX_OBJECT_TYPE&&object->type-SLL_MAX_OBJECT_TYPE-1<type_table->length);
	const sll_object_type_data_t* dt=*(type_table->data+object->type-SLL_MAX_OBJECT_TYPE-1);
	sll_map_create(dt->field_count,out);
	sll_object_field_t* p=object->data.fields;
	for (sll_arg_count_t i=0;i<dt->field_count;i++){
		out->data[i<<1]=sll_string_to_object(&(dt->fields[i].name));
		if (dt->fields[i].type==SLL_OBJECT_TYPE_INT){
			out->data[(i<<1)+1]=sll_int_to_object(p->int_);
		}
		else if (dt->fields[i].type==SLL_OBJECT_TYPE_FLOAT){
			out->data[(i<<1)+1]=sll_float_to_object(p->float_);
		}
		else if (dt->fields[i].type==SLL_OBJECT_TYPE_CHAR){
			out->data[(i<<1)+1]=SLL_FROM_CHAR(p->char_);
		}
		else{
			SLL_ACQUIRE(p->any);
			out->data[(i<<1)+1]=p->any;
		}
		p++;
	}
}
