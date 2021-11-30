#include <sll/_sll_internal.h>
#include <sll/api.h>
#include <sll/api/string.h>
#include <sll/assembly.h>
#include <sll/common.h>
#include <sll/gc.h>
#include <sll/handle.h>
#include <sll/memory.h>
#include <sll/object.h>
#include <sll/static_object.h>
#include <sll/string.h>
#include <sll/types.h>
#include <sll/util.h>
#include <sll/var_arg.h>
#include <stdint.h>
#include <stdio.h>



static sll_bool_t _object_to_string(const sll_object_t* a,sll_bool_t q,sll_string_t* o);



static sll_bool_t _write_int(uint64_t v,sll_string_t* o){
	uint8_t i=0;
	char bf[20];
	do{
		bf[i]=v%10;
		i++;
		v/=10;
	} while (v);
	if (!sll_string_increase(o,i)){
		return 0;
	}
	while (i){
		i--;
		o->v[o->l]=bf[i]+48;
		o->l++;
	}
	return 1;
}


static sll_bool_t _write_char(sll_char_t c,sll_string_t* o){
	if (c=='\"'||c=='\''||c=='\\'){
		if (!sll_string_increase(o,2)){
			return 0;
		}
		o->v[o->l]='\\';
		o->v[o->l+1]=c;
		o->l+=2;
	}
	else if (c=='\t'){
		if (!sll_string_increase(o,2)){
			return 0;
		}
		o->v[o->l]='\\';
		o->v[o->l+1]='t';
		o->l+=2;
	}
	else if (c=='\n'){
		if (!sll_string_increase(o,2)){
			return 0;
		}
		o->v[o->l]='\\';
		o->v[o->l+1]='n';
		o->l+=2;
	}
	else if (c=='\v'){
		if (!sll_string_increase(o,2)){
			return 0;
		}
		o->v[o->l]='\\';
		o->v[o->l+1]='v';
		o->l+=2;
	}
	else if (c=='\f'){
		if (!sll_string_increase(o,2)){
			return 0;
		}
		o->v[o->l]='\\';
		o->v[o->l+1]='f';
		o->l+=2;
	}
	else if (c=='\r'){
		if (!sll_string_increase(o,2)){
			return 0;
		}
		o->v[o->l]='\\';
		o->v[o->l+1]='r';
		o->l+=2;
	}
	else if (SLL_STRING_HEX_ESCAPE(c)){
		if (!sll_string_increase(o,4)){
			return 0;
		}
		o->v[o->l]='\\';
		o->v[o->l+1]='x';
		o->v[o->l+2]=(c>>4)+((c>>4)>9?87:48);
		o->v[o->l+3]=(c&15)+((c&15)>9?87:48);
		o->l+=4;
	}
	else{
		if (!sll_string_increase(o,1)){
			return 0;
		}
		o->v[o->l]=c;
		o->l++;
	}
	return 1;
}



static void* _print_custom_type(void* p,sll_object_type_t t,sll_string_t* o){
	if (!sll_string_increase(o,3)){
		return NULL;
	}
	sll_copy_string(SLL_CHAR("<&:"),o->v+o->l);
	o->l+=3;
	if (!_write_int(t,o)){
		return NULL;
	}
	sll_object_t tmp={1};
	const sll_object_type_data_t* dt=*(sll_current_runtime_data->tt->dt+t-SLL_MAX_OBJECT_TYPE-1);
	for (sll_arg_count_t i=0;i<dt->l;i++){
		sll_string_t s=dt->dt[i].nm;
		if (!sll_string_increase(o,2)){
			return NULL;
		}
		o->v[o->l]=' ';
		o->v[o->l+1]='\"';
		o->l+=2;
		for (sll_string_length_t j=0;j<s.l;j++){
			if (!_write_char(s.v[j],o)){
				return NULL;
			}
		}
		if (!sll_string_increase(o,2)){
			return NULL;
		}
		o->v[o->l]='\"';
		o->v[o->l+1]=' ';
		o->l+=2;
		switch (SLL_OBJECT_GET_TYPE_MASK(dt->dt[i].t)){
			case SLL_OBJECT_TYPE_INT:
				tmp.dt.i=*((sll_integer_t*)p);
				p=(void*)(((uint64_t)p)+sizeof(sll_integer_t));
				break;
			case SLL_OBJECT_TYPE_FLOAT:
				tmp.dt.f=*((sll_float_t*)p);
				p=(void*)(((uint64_t)p)+sizeof(sll_float_t));
				break;
			case SLL_OBJECT_TYPE_CHAR:
				tmp.dt.c=(sll_char_t)(*((sll_integer_t*)p));
				p=(void*)(((uint64_t)p)+sizeof(sll_integer_t));
				break;
			case SLL_OBJECT_TYPE_STRING:
				tmp.dt.s=*((sll_string_t*)p);
				p=(void*)(((uint64_t)p)+sizeof(sll_string_t));
				break;
			case SLL_OBJECT_TYPE_ARRAY:
			case SLL_OBJECT_TYPE_MAP_KEYS:
			case SLL_OBJECT_TYPE_MAP_VALUES:
				tmp.dt.a=*((sll_array_t*)p);
				p=(void*)(((uint64_t)p)+sizeof(sll_array_t));
				break;
			case SLL_OBJECT_TYPE_HANDLE:
				tmp.dt.h=*((sll_handle_data_t*)p);
				p=(void*)(((uint64_t)p)+sizeof(sll_handle_data_t));
				break;
			case SLL_OBJECT_TYPE_MAP:
				tmp.dt.m=*((sll_map_t*)p);
				p=(void*)(((uint64_t)p)+sizeof(sll_map_t));
				break;
			default:
				p=_print_custom_type(p,SLL_OBJECT_GET_TYPE_MASK(dt->dt[i].t),o);
				if (!p){
					return NULL;
				}
				continue;
		}
		tmp.t=SLL_OBJECT_GET_TYPE_MASK(dt->dt[i].t);
		if (!_object_to_string(&tmp,1,o)){
			return NULL;
		}
	}
	if (!sll_string_increase(o,1)){
		return NULL;
	}
	o->v[o->l]='>';
	o->l++;
	return p;
}



static sll_bool_t _object_to_string(const sll_object_t* a,sll_bool_t q,sll_string_t* o){
	if (!a->rc){
		if (!sll_string_increase(o,19)){
			return 0;
		}
		sll_copy_string(SLL_CHAR("<released object>"),o->v+o->l);
		o->l+=19;
		return 1;
	}
	switch (SLL_OBJECT_GET_TYPE(a)){
		case SLL_OBJECT_TYPE_INT:
			{
				sll_integer_t v=a->dt.i;
				if (v<0){
					if (!sll_string_increase(o,1)){
						return 0;
					}
					o->v[o->l]='-';
					v=-v;
					o->l++;
				}
				return _write_int(v,o);
			}
		case SLL_OBJECT_TYPE_FLOAT:
			{
				sll_char_t bf[256];
				sll_string_length_t bfl=snprintf((char*)bf,256,"%.16lf",a->dt.f);
				if (!sll_string_increase(o,bfl)){
					return 0;
				}
				sll_copy_data(bf,bfl,o->v+o->l);
				o->l+=bfl;
				return 1;
			}
		case SLL_OBJECT_TYPE_CHAR:
			if (q){
				if (!sll_string_increase(o,1)){
					return 0;
				}
				o->v[o->l]='\'';
				o->l++;
				if (!_write_char(a->dt.c,o)||!sll_string_increase(o,1)){
					return 0;
				}
				o->v[o->l]='\'';
				o->l++;
				return 1;
			}
			if (!sll_string_increase(o,1)){
				return 0;
			}
			o->v[o->l]=a->dt.c;
			o->l++;
			return 1;
		case SLL_OBJECT_TYPE_STRING:
			if (q){
				if (!sll_string_increase(o,1)){
					return 0;
				}
				o->v[o->l]='\"';
				o->l++;
				for (sll_string_length_t i=0;i<a->dt.s.l;i++){
					if (!_write_char(a->dt.s.v[i],o)){
						return 0;
					}
				}
				if (!sll_string_increase(o,1)){
					return 0;
				}
				o->v[o->l]='\"';
				o->l++;
				return 1;
			}
			if (!sll_string_increase(o,a->dt.s.l)){
				return 0;
			}
			sll_copy_data(a->dt.s.v,a->dt.s.l,o->v+o->l);
			o->l+=a->dt.s.l;
			return 1;
		case SLL_OBJECT_TYPE_ARRAY:
			if (!sll_string_increase(o,1)){
				return 0;
			}
			o->v[o->l]='[';
			o->l++;
			for (sll_array_length_t i=0;i<a->dt.a.l;i++){
				if (i){
					if (!sll_string_increase(o,1)){
						return 0;
					}
					o->v[o->l]=' ';
					o->l++;
				}
				if (!_object_to_string(*(a->dt.a.v+i),1,o)){
					return 0;
				}
			}
			if (!sll_string_increase(o,1)){
				return 0;
			}
			o->v[o->l]=']';
			o->l++;
			return 1;
		case SLL_OBJECT_TYPE_HANDLE:
			{
				sll_handle_descriptor_t* hd=(sll_current_runtime_data?SLL_HANDLE_LOOKUP_DESCRIPTOR(sll_current_runtime_data->hl,a->dt.h.t):NULL);
				if (hd&&hd->sf){
					hd->sf(a->dt.h.h,o);
					return 1;
				}
				if (!_write_int(a->dt.h.h,o)||!sll_string_increase(o,1)){
					return 0;
				}
				o->v[o->l]='$';
				o->l++;
				if (hd){
					if (!sll_string_increase(o,hd->nml)){
						return 0;
					}
					sll_copy_data(hd->nm,hd->nml,o->v+o->l);
					o->l+=hd->nml;
					return 1;
				}
				return _write_int(a->dt.h.t,o);
			}
		case SLL_OBJECT_TYPE_MAP:
			if (!sll_string_increase(o,1)){
				return 0;
			}
			o->v[o->l]='<';
			o->l++;
			for (sll_array_length_t i=0;i<(a->dt.m.l<<1);i++){
				if (i){
					if (!sll_string_increase(o,1)){
						return 0;
					}
					o->v[o->l]=' ';
					o->l++;
				}
				if (!_object_to_string(*(a->dt.m.v+i),1,o)){
					return 0;
				}
			}
			if (!sll_string_increase(o,1)){
				return 0;
			}
			o->v[o->l]='>';
			o->l++;
			return 1;
		case OBJECT_TYPE_FUNCTION_ID:
			if (!sll_string_increase(o,1)){
				return 0;
			}
			o->v[o->l]='#';
			o->l++;
			return _write_int(a->dt.i,o);
		case OBJECT_TYPE_UNKNOWN:
_print_unknown:
			if (!sll_string_increase(o,9)){
				return 0;
			}
			sll_copy_string(SLL_CHAR("<unknown>"),o->v+o->l);
			o->l+=9;
			return 1;
	}
	if (!sll_current_runtime_data||SLL_OBJECT_GET_TYPE(a)>sll_current_runtime_data->tt->l+SLL_MAX_OBJECT_TYPE){
		goto _print_unknown;
	}
	return (!!_print_custom_type(a->dt.p,SLL_OBJECT_GET_TYPE(a),o)||!a->dt.p);
}



__API_FUNC(string_convert){
	if (!ac){
		SLL_INIT_STRING(out);
		return;
	}
	SLL_CHECK_NO_MEMORY(sll_string_create(0,out));
	out->v=sll_memory_move(out->v,SLL_MEMORY_MOVE_DIRECTION_TO_STACK);
	for (sll_array_length_t i=0;i<ac;i++){
		SLL_CHECK_NO_MEMORY(_object_to_string(*(a+i),0,out));
	}
	out->v=sll_memory_move(out->v,SLL_MEMORY_MOVE_DIRECTION_FROM_STACK);
	sll_string_calculate_checksum(out);
}



__API_FUNC(string_format){
	sll_var_arg_list_t dt={
		SLL_VAR_ARG_LIST_TYPE_SLL,
		{
			.sll={
				b,
				bc
			}
		}
	};
	SLL_CHECK_NO_MEMORY(sll_string_format_list(a->v,a->l,&dt,out));
}



__API_FUNC(string_flip_case){
	SLL_CHECK_NO_MEMORY(sll_string_flip_case(a,out));
}



__API_FUNC(string_replace){
	if (!b){
		SLL_CHECK_NO_MEMORY(sll_string_clone(a,out));
	}
	else if (!c){
		if (SLL_OBJECT_GET_TYPE(b)==SLL_OBJECT_TYPE_STRING){
			SLL_CHECK_NO_MEMORY(sll_string_remove(a,&(b->dt.s),out));
		}
		else{
			SLL_UNIMPLEMENTED();
			sll_string_t s;
			SLL_CHECK_NO_MEMORY(sll_string_from_char(b->dt.c,&s));
			SLL_CHECK_NO_MEMORY(sll_string_remove(a,&s,out));
			sll_free_string(&s);
		}
	}
	else{
		if (SLL_OBJECT_GET_TYPE(b)==SLL_OBJECT_TYPE_CHAR&&SLL_OBJECT_GET_TYPE(c)==SLL_OBJECT_TYPE_CHAR){
			SLL_CHECK_NO_MEMORY(sll_string_replace_char(a,b->dt.c,c->dt.c,out));
		}
		else if (SLL_OBJECT_GET_TYPE(b)==SLL_OBJECT_TYPE_CHAR){
			sll_string_t s;
			SLL_CHECK_NO_MEMORY(sll_string_from_char(b->dt.c,&s));
			SLL_CHECK_NO_MEMORY(sll_string_replace(a,&s,&(c->dt.s),out));
			sll_free_string(&s);
		}
		else if (SLL_OBJECT_GET_TYPE(c)==SLL_OBJECT_TYPE_CHAR){
			sll_string_t s;
			SLL_CHECK_NO_MEMORY(sll_string_from_char(c->dt.c,&s));
			SLL_CHECK_NO_MEMORY(sll_string_replace(a,&(b->dt.s),&s,out));
			sll_free_string(&s);
		}
		else{
			SLL_CHECK_NO_MEMORY(sll_string_replace(a,&(b->dt.s),&(c->dt.s),out));
		}
	}
}



__API_FUNC(string_to_lower_case){
	SLL_CHECK_NO_MEMORY(sll_string_lower_case(a,out));
}



__API_FUNC(string_to_title_case){
	SLL_CHECK_NO_MEMORY(sll_string_title_case(a,out));
}



__API_FUNC(string_to_upper_case){
	SLL_CHECK_NO_MEMORY(sll_string_upper_case(a,out));
}
