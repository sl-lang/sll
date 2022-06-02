#include <sll/_internal/api_string.h>
#include <sll/_internal/common.h>
#include <sll/_internal/string.h>
#include <sll/allocator.h>
#include <sll/api/string.h>
#include <sll/common.h>
#include <sll/data.h>
#include <sll/gc.h>
#include <sll/memory.h>
#include <sll/object.h>
#include <sll/operator.h>
#include <sll/parse_args.h>
#include <sll/static_object.h>
#include <sll/string.h>
#include <sll/string_table.h>
#include <sll/types.h>
#include <sll/var_arg.h>
#include <sll/vm.h>
#include <stdio.h>



static void _write_int(sll_size_t v,sll_string_t* o){
	sll_string_length_t i=0;
	sll_char_t bf[20];
	do{
		bf[i]=v%10;
		i++;
		v/=10;
	} while (v);
	sll_string_increase(o,i);
	while (i){
		i--;
		o->data[o->length]=bf[i]+48;
		o->length++;
	}
}



static void _write_char(sll_char_t c,sll_string_t* o){
	if (c=='\"'||c=='\''||c=='\\'){
		sll_string_increase(o,2);
		o->data[o->length]='\\';
		o->data[o->length+1]=c;
		o->length+=2;
	}
	else if (c=='\t'){
		sll_string_increase(o,2);
		o->data[o->length]='\\';
		o->data[o->length+1]='t';
		o->length+=2;
	}
	else if (c=='\n'){
		sll_string_increase(o,2);
		o->data[o->length]='\\';
		o->data[o->length+1]='n';
		o->length+=2;
	}
	else if (c=='\v'){
		sll_string_increase(o,2);
		o->data[o->length]='\\';
		o->data[o->length+1]='v';
		o->length+=2;
	}
	else if (c=='\f'){
		sll_string_increase(o,2);
		o->data[o->length]='\\';
		o->data[o->length+1]='f';
		o->length+=2;
	}
	else if (c=='\r'){
		sll_string_increase(o,2);
		o->data[o->length]='\\';
		o->data[o->length+1]='r';
		o->length+=2;
	}
	else if (SLL_STRING_HEX_ESCAPE(c)){
		sll_string_increase(o,4);
		o->data[o->length]='\\';
		o->data[o->length+1]='x';
		o->data[o->length+2]=(c>>4)+((c>>4)>9?87:48);
		o->data[o->length+3]=(c&15)+((c&15)>9?87:48);
		o->length+=4;
	}
	else{
		sll_string_increase(o,1);
		o->data[o->length]=c;
		o->length++;
	}
}



static void _object_to_string(sll_object_t* a,sll_string_t* o,address_list_t* addr_list){
	if (!a->rc){
		sll_string_increase(o,17);
		sll_copy_string(SLL_CHAR("<released object>"),o->data+o->length);
		o->length+=17;
		return;
	}
	address_list_t new_addr_list={
		ADDR(a),
		addr_list
	};
	while (addr_list){
		if (ADDR(a)==addr_list->addr){
			sll_string_increase(o,20);
			o->data[o->length]='%';
			o->length++;
			o->data[o->length]='%';
			o->length++;
			__SLL_U32 i=64;
			do{
				i-=4;
				sll_char_t v=(ADDR(a)>>i)&15;
				o->data[o->length]=v+(v>9?87:48);
				o->length++;
			} while (i);
			o->data[o->length]='%';
			o->length++;
			o->data[o->length]='%';
			o->length++;
			return;
		}
		addr_list=addr_list->next;
	}
	switch (a->type){
		case SLL_OBJECT_TYPE_INT:
			{
				sll_integer_t v=a->data.int_;
				if (v<0){
					sll_string_increase(o,1);
					o->data[o->length]='-';
					v=-v;
					o->length++;
				}
				_write_int(v,o);
				return;
			}
		case SLL_OBJECT_TYPE_FLOAT:
			{
				sll_char_t bf[256];
				sll_string_length_t bfl=snprintf((char*)bf,256,"%.16lf",a->data.float_);
				sll_string_increase(o,bfl);
				sll_copy_data(bf,bfl,o->data+o->length);
				o->length+=bfl;
				return;
			}
		case SLL_OBJECT_TYPE_CHAR:
			sll_string_increase(o,1);
			o->data[o->length]='\'';
			o->length++;
			_write_char(a->data.char_,o);
			sll_string_increase(o,1);
			o->data[o->length]='\'';
			o->length++;
			return;
		case SLL_OBJECT_TYPE_COMPLEX:
			{
				sll_char_t bf[256];
				if (a->data.complex_.real){
					sll_string_length_t bfl=snprintf((char*)bf,256,"%.16lf",a->data.complex_.real);
					sll_string_increase(o,bfl);
					sll_copy_data(bf,bfl,o->data+o->length);
					o->length+=bfl;
					if (a->data.complex_.imag){
						sll_float_t v=a->data.complex_.imag;
						sll_string_increase(o,1);
						if (v<0){
							o->data[o->length]='-';
							v=-v;
						}
						else{
							o->data[o->length]='+';
						}
						o->length++;
						bfl=snprintf((char*)bf,256,"%.16lfi",v);
						sll_string_increase(o,bfl);
						sll_copy_data(bf,bfl,o->data+o->length);
						o->length+=bfl;
					}
				}
				else if (a->data.complex_.imag){
					sll_string_length_t bfl=snprintf((char*)bf,256,"%.16lfi",a->data.complex_.imag);
					sll_string_increase(o,bfl);
					sll_copy_data(bf,bfl,o->data+o->length);
					o->length+=bfl;
				}
				else{
					sll_string_increase(o,1);
					o->data[o->length]='0';
					o->length++;
				}
				return;
			}
		case SLL_OBJECT_TYPE_STRING:
			sll_string_increase(o,1);
			o->data[o->length]='\"';
			o->length++;
			for (sll_string_length_t i=0;i<a->data.string.length;i++){
				_write_char(a->data.string.data[i],o);
			}
			sll_string_increase(o,1);
			o->data[o->length]='\"';
			o->length++;
			return;
		case SLL_OBJECT_TYPE_ARRAY:
			sll_string_increase(o,1);
			o->data[o->length]='[';
			o->length++;
			for (sll_array_length_t i=0;i<a->data.array.length;i++){
				if (i){
					sll_string_increase(o,1);
					o->data[o->length]=' ';
					o->length++;
				}
				_object_to_string(*(a->data.array.data+i),o,&new_addr_list);
			}
			sll_string_increase(o,1);
			o->data[o->length]=']';
			o->length++;
			return;
		case SLL_OBJECT_TYPE_MAP:
			sll_string_increase(o,1);
			o->data[o->length]='<';
			o->length++;
			for (sll_array_length_t i=0;i<(a->data.map.length<<1);i++){
				if (i){
					sll_string_increase(o,1);
					o->data[o->length]=' ';
					o->length++;
				}
				_object_to_string(*(a->data.map.data+i),o,&new_addr_list);
			}
			sll_string_increase(o,1);
			o->data[o->length]='>';
			o->length++;
			return;
		default:
			{
				if (!sll_current_runtime_data||a->type>sll_current_runtime_data->type_table->length+SLL_MAX_OBJECT_TYPE){
					sll_string_increase(o,13);
					sll_copy_string(SLL_CHAR("<custom-type>"),o->data+o->length);
					o->length+=13;
					return;
				}
				const sll_object_type_data_t* dt=*(sll_current_runtime_data->type_table->data+a->type-SLL_MAX_OBJECT_TYPE-1);
				if (dt->functions[SLL_OBJECT_FUNC_STRING]){
					sll_object_t* v=sll_execute_function(dt->functions[SLL_OBJECT_FUNC_STRING],&a,1,0);
					if (v){
						sll_object_t* str=sll_operator_cast(v,sll_static_int[SLL_OBJECT_TYPE_STRING]);
						SLL_RELEASE(v);
						sll_string_increase(o,str->data.string.length);
						sll_copy_data(str->data.string.data,str->data.string.length,o->data+o->length);
						o->length+=str->data.string.length;
						SLL_RELEASE(str);
						return;
					}
				}
				sll_string_increase(o,3);
				sll_copy_string(SLL_CHAR("<&:"),o->data+o->length);
				o->length+=3;
				if (!dt->name.length){
					_write_int(a->type,o);
				}
				else{
					sll_string_increase(o,dt->name.length);
					sll_copy_data(dt->name.data,dt->name.length,o->data+o->length);
					o->length+=dt->name.length;
				}
				sll_object_field_t* p=a->data.fields;
				for (sll_arg_count_t i=0;i<dt->field_count;i++){
					sll_string_t s=dt->fields[i].name;
					sll_string_increase(o,1);
					o->data[o->length]=' ';
					o->length++;
					for (sll_string_length_t j=0;j<s.length;j++){
						_write_char(s.data[j],o);
					}
					sll_string_increase(o,1);
					o->data[o->length]=' ';
					o->length++;
					sll_object_t* tmp;
					switch (dt->fields[i].type){
						case SLL_OBJECT_TYPE_INT:
							tmp=sll_int_to_object(p->int_);
							break;
						case SLL_OBJECT_TYPE_FLOAT:
							tmp=sll_float_to_object(p->float_);
							break;
						case SLL_OBJECT_TYPE_CHAR:
							tmp=SLL_FROM_CHAR(p->char_);
							break;
						default:
							tmp=p->any;
							SLL_ACQUIRE(tmp);
							break;
					}
					_object_to_string(tmp,o,&new_addr_list);
					SLL_RELEASE(tmp);
					p++;
				}
				sll_string_increase(o,1);
				o->data[o->length]='>';
				o->length++;
				return;
			}
	}
	SLL_UNREACHABLE();
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_string_checksum_t sll_api_string_checksum(const sll_string_t* str){
	return str->checksum;
}



__SLL_EXTERNAL __SLL_API_CALL void sll_api_string_convert(sll_object_t*const* args,sll_arg_count_t len,sll_string_t* out){
	STRING_INIT_STACK(out);
	for (sll_array_length_t i=0;i<len;i++){
		sll_object_t* v=*(args+i);
		if (v->type==SLL_OBJECT_TYPE_CHAR){
			sll_string_increase(out,1);
			out->data[out->length]=v->data.char_;
			out->length++;
		}
		else if (v->type==SLL_OBJECT_TYPE_STRING){
			sll_string_increase(out,v->data.string.length);
			sll_copy_data(v->data.string.data,v->data.string.length,out->data+out->length);
			out->length+=v->data.string.length;
		}
		else{
			_object_to_string(*(args+i),out,NULL);
		}
	}
	sll_allocator_move((void**)(&(out->data)),SLL_MEMORY_MOVE_DIRECTION_FROM_STACK);
	sll_string_calculate_checksum(out);
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_integer_t sll_api_string_count(const sll_string_t* str,const sll_char_string_t* elem){
	return (elem->type==SLL_PARSE_ARGS_TYPE_CHAR?sll_string_count_char(str,elem->data.char_):sll_string_count(str,elem->data.string));
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_integer_t sll_api_string_count_left(const sll_string_t* str,sll_char_t chr){
	return sll_string_count_left(str,chr);
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_integer_t sll_api_string_count_right(const sll_string_t* str,sll_char_t chr){
	return sll_string_count_right(str,chr);
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_bool_t sll_api_string_ends(const sll_string_t* str,const sll_char_string_t* end){
	if (end->type==SLL_PARSE_ARGS_TYPE_CHAR){
		return (str->length&&str->data[str->length-1]==end->data.char_);
	}
	return sll_string_ends(str,end->data.string);
}



__SLL_EXTERNAL __SLL_API_CALL void sll_api_string_flip_case(const sll_string_t* str,sll_string_t* out){
	sll_string_flip_case(str,out);
}



__SLL_EXTERNAL __SLL_API_CALL void sll_api_string_format(const sll_string_t* fmt,sll_object_t*const* args,sll_arg_count_t len,sll_string_t* out){
	sll_var_arg_list_t dt;
	SLL_VAR_ARG_INIT_SLL(&dt,args,len);
	sll_string_format_list(fmt->data,fmt->length,&dt,out);
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_integer_t sll_api_string_index(const sll_string_t* str,const sll_char_string_t* substr,sll_string_length_t start){
	sll_string_length_t o=(substr->type==SLL_PARSE_ARGS_TYPE_CHAR?sll_string_index_char(str,substr->data.char_,0,start):sll_string_index(str,substr->data.string,start));
	return (o==SLL_MAX_STRING_LENGTH?(sll_integer_t)-1:o);
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_integer_t sll_api_string_index_list(const sll_string_t* str,const sll_char_string_t* substr,sll_bool_t inv){
	sll_string_length_t o=(substr->type==SLL_PARSE_ARGS_TYPE_CHAR?sll_string_index_char(str,substr->data.char_,inv,0):sll_string_index_multiple(str,substr->data.string->data,substr->data.string->length,inv));
	return (o==SLL_MAX_STRING_LENGTH?(sll_integer_t)-1:o);
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_integer_t sll_api_string_index_reverse(const sll_string_t* str,const sll_char_string_t* substr){
	sll_string_length_t o=(substr->type==SLL_PARSE_ARGS_TYPE_CHAR?sll_string_index_reverse_char(str,substr->data.char_,0):sll_string_index_reverse(str,substr->data.string));
	return (o==SLL_MAX_STRING_LENGTH?(sll_integer_t)-1:o);
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_integer_t sll_api_string_index_reverse_list(const sll_string_t* str,const sll_char_string_t* substr,sll_bool_t inv){
	sll_string_length_t o=(substr->type==SLL_PARSE_ARGS_TYPE_CHAR?sll_string_index_reverse_char(str,substr->data.char_,inv):sll_string_index_reverse_multiple(str,substr->data.string->data,substr->data.string->length,inv));
	return (o==SLL_MAX_STRING_LENGTH?(sll_integer_t)-1:o);
}



__SLL_EXTERNAL __SLL_API_CALL void sll_api_string_join(const sll_char_string_t* infix,const sll_array_t* arr,sll_string_t* out){
	if (infix->type==SLL_PARSE_ARGS_TYPE_CHAR){
		sll_string_join_char(infix->data.char_,arr->data,arr->length,out);
	}
	else{
		sll_string_join(infix->data.string,arr->data,arr->length,out);
	}
}



__SLL_EXTERNAL __SLL_API_CALL void sll_api_string_pad(const sll_string_t* str,sll_string_length_t len,sll_char_t chr,sll_string_t* out){
	sll_string_pad(str,len,chr,out);
}



__SLL_EXTERNAL __SLL_API_CALL void sll_api_string_pad_left(const sll_string_t* str,sll_string_length_t len,sll_char_t chr,sll_string_t* out){
	sll_string_pad_left(str,len,chr,out);
}



__SLL_EXTERNAL __SLL_API_CALL void sll_api_string_pad_right(const sll_string_t* str,sll_string_length_t len,sll_char_t chr,sll_string_t* out){
	sll_string_pad_right(str,len,chr,out);
}



__SLL_EXTERNAL __SLL_API_CALL void sll_api_string_replace(const sll_string_t* str,const sll_char_string_t* old,const sll_char_string_t* new,sll_string_t* out){
	if (old->type==SLL_PARSE_ARGS_TYPE_STRING&&!old->data.string->length){
		sll_string_clone(str,out);
	}
	else if (new->type==SLL_PARSE_ARGS_TYPE_STRING&&!new->data.string->length){
		if (old->type==SLL_PARSE_ARGS_TYPE_STRING){
			sll_string_remove(str,old->data.string,out);
		}
		else{
			sll_string_t s;
			sll_string_from_char(old->data.char_,&s);
			sll_string_remove(str,&s,out);
			sll_free_string(&s);
		}
	}
	else{
		if (old->type==SLL_PARSE_ARGS_TYPE_CHAR&&new->type==SLL_PARSE_ARGS_TYPE_CHAR){
			sll_string_replace_char(str,old->data.char_,new->data.char_,out);
		}
		else if (old->type==SLL_PARSE_ARGS_TYPE_CHAR){
			sll_string_t s;
			sll_string_from_char(old->data.char_,&s);
			sll_string_replace(str,&s,new->data.string,out);
			sll_free_string(&s);
		}
		else if (new->type==SLL_PARSE_ARGS_TYPE_CHAR){
			sll_string_t s;
			sll_string_from_char(new->data.char_,&s);
			sll_string_replace(str,old->data.string,&s,out);
			sll_free_string(&s);
		}
		else{
			sll_string_replace(str,old->data.string,new->data.string,out);
		}
	}
}



__SLL_EXTERNAL __SLL_API_CALL void sll_api_string_reverse(const sll_string_t* str,sll_string_t* out){
	sll_string_reverse(str,out);
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_bool_t sll_api_string_secure_equal(const sll_string_t* a,const sll_string_t* b){
	return sll_string_secure_equal(a,b);
}



__SLL_EXTERNAL __SLL_API_CALL void sll_api_string_split(const sll_string_t* str,const sll_char_string_t* infix,sll_array_t* out){
	if (infix->type==SLL_PARSE_ARGS_TYPE_CHAR){
		sll_string_split_char(str,infix->data.char_,out);
	}
	else{
		sll_string_split(str,infix->data.string,out);
	}
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_bool_t sll_api_string_starts(const sll_string_t* str,const sll_char_string_t* start){
	if (start->type==SLL_PARSE_ARGS_TYPE_CHAR){
		return (str->length&&str->data[0]==start->data.char_);
	}
	return sll_string_starts(str,start->data.string);
}



__SLL_EXTERNAL __SLL_API_CALL void sll_api_string_to_lower_case(const sll_string_t* str,sll_string_t* out){
	sll_string_lower_case(str,out);
}



__SLL_EXTERNAL __SLL_API_CALL void sll_api_string_to_title_case(const sll_string_t* str,sll_string_t* out){
	sll_string_title_case(str,out);
}



__SLL_EXTERNAL __SLL_API_CALL void sll_api_string_to_upper_case(const sll_string_t* str,sll_string_t* out){
	sll_string_upper_case(str,out);
}



__SLL_EXTERNAL __SLL_API_CALL void sll_api_string_trim(const sll_string_t* str,sll_string_t* out){
	sll_string_trim(str,out);
}



__SLL_EXTERNAL __SLL_API_CALL void sll_api_string_trim_left(const sll_string_t* str,sll_string_t* out){
	sll_string_trim_left(str,out);
}



__SLL_EXTERNAL __SLL_API_CALL void sll_api_string_trim_right(const sll_string_t* str,sll_string_t* out){
	sll_string_trim_right(str,out);
}
