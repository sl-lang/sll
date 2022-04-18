#include <sll/_internal/common.h>
#include <sll/_internal/gc.h>
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
		o->v[o->l]=bf[i]+48;
		o->l++;
	}
}



static void _write_char(sll_char_t c,sll_string_t* o){
	if (c=='\"'||c=='\''||c=='\\'){
		sll_string_increase(o,2);
		o->v[o->l]='\\';
		o->v[o->l+1]=c;
		o->l+=2;
	}
	else if (c=='\t'){
		sll_string_increase(o,2);
		o->v[o->l]='\\';
		o->v[o->l+1]='t';
		o->l+=2;
	}
	else if (c=='\n'){
		sll_string_increase(o,2);
		o->v[o->l]='\\';
		o->v[o->l+1]='n';
		o->l+=2;
	}
	else if (c=='\v'){
		sll_string_increase(o,2);
		o->v[o->l]='\\';
		o->v[o->l+1]='v';
		o->l+=2;
	}
	else if (c=='\f'){
		sll_string_increase(o,2);
		o->v[o->l]='\\';
		o->v[o->l+1]='f';
		o->l+=2;
	}
	else if (c=='\r'){
		sll_string_increase(o,2);
		o->v[o->l]='\\';
		o->v[o->l+1]='r';
		o->l+=2;
	}
	else if (SLL_STRING_HEX_ESCAPE(c)){
		sll_string_increase(o,4);
		o->v[o->l]='\\';
		o->v[o->l+1]='x';
		o->v[o->l+2]=(c>>4)+((c>>4)>9?87:48);
		o->v[o->l+3]=(c&15)+((c&15)>9?87:48);
		o->l+=4;
	}
	else{
		sll_string_increase(o,1);
		o->v[o->l]=c;
		o->l++;
	}
}



static void _object_to_string(sll_object_t* a,sll_string_t* o){
	if (!a->rc){
		sll_string_increase(o,17);
		sll_copy_string(SLL_CHAR("<released object>"),o->v+o->l);
		o->l+=17;
		return;
	}
	switch (a->t){
		case SLL_OBJECT_TYPE_INT:
			{
				sll_integer_t v=a->dt.i;
				if (v<0){
					sll_string_increase(o,1);
					o->v[o->l]='-';
					v=-v;
					o->l++;
				}
				_write_int(v,o);
				return;
			}
		case SLL_OBJECT_TYPE_FLOAT:
			{
				sll_char_t bf[256];
				sll_string_length_t bfl=snprintf((char*)bf,256,"%.16lf",a->dt.f);
				sll_string_increase(o,bfl);
				sll_copy_data(bf,bfl,o->v+o->l);
				o->l+=bfl;
				return;
			}
		case SLL_OBJECT_TYPE_CHAR:
			sll_string_increase(o,1);
			o->v[o->l]='\'';
			o->l++;
			_write_char(a->dt.c,o);
			sll_string_increase(o,1);
			o->v[o->l]='\'';
			o->l++;
			return;
		case SLL_OBJECT_TYPE_COMPLEX:
			{
				sll_char_t bf[256];
				if (a->dt.d.real){
					sll_string_length_t bfl=snprintf((char*)bf,256,"%.16lf",a->dt.d.real);
					sll_string_increase(o,bfl);
					sll_copy_data(bf,bfl,o->v+o->l);
					o->l+=bfl;
					if (a->dt.d.imag){
						sll_float_t v=a->dt.d.imag;
						sll_string_increase(o,1);
						if (v<0){
							o->v[o->l]='-';
							v=-v;
						}
						else{
							o->v[o->l]='+';
						}
						o->l++;
						bfl=snprintf((char*)bf,256,"%.16lfi",v);
						sll_string_increase(o,bfl);
						sll_copy_data(bf,bfl,o->v+o->l);
						o->l+=bfl;
					}
				}
				else if (a->dt.d.imag){
					sll_string_length_t bfl=snprintf((char*)bf,256,"%.16lfi",a->dt.d.imag);
					sll_string_increase(o,bfl);
					sll_copy_data(bf,bfl,o->v+o->l);
					o->l+=bfl;
				}
				else{
					sll_string_increase(o,1);
					o->v[o->l]='0';
					o->l++;
				}
				return;
			}
		case SLL_OBJECT_TYPE_STRING:
			sll_string_increase(o,1);
			o->v[o->l]='\"';
			o->l++;
			for (sll_string_length_t i=0;i<a->dt.s.l;i++){
				_write_char(a->dt.s.v[i],o);
			}
			sll_string_increase(o,1);
			o->v[o->l]='\"';
			o->l++;
			return;
		case SLL_OBJECT_TYPE_ARRAY:
			sll_string_increase(o,1);
			o->v[o->l]='[';
			o->l++;
			for (sll_array_length_t i=0;i<a->dt.a.l;i++){
				if (i){
					sll_string_increase(o,1);
					o->v[o->l]=' ';
					o->l++;
				}
				_object_to_string(*(a->dt.a.v+i),o);
			}
			sll_string_increase(o,1);
			o->v[o->l]=']';
			o->l++;
			return;
		case SLL_OBJECT_TYPE_MAP:
			sll_string_increase(o,1);
			o->v[o->l]='<';
			o->l++;
			for (sll_array_length_t i=0;i<(a->dt.m.l<<1);i++){
				if (i){
					sll_string_increase(o,1);
					o->v[o->l]=' ';
					o->l++;
				}
				_object_to_string(*(a->dt.m.v+i),o);
			}
			sll_string_increase(o,1);
			o->v[o->l]='>';
			o->l++;
			return;
		default:
			{
				if (!sll_current_runtime_data||a->t>sll_current_runtime_data->tt->l+SLL_MAX_OBJECT_TYPE){
					sll_string_increase(o,13);
					sll_copy_string(SLL_CHAR("<custom-type>"),o->v+o->l);
					o->l+=13;
					return;
				}
				const sll_object_type_data_t* dt=*(sll_current_runtime_data->tt->dt+a->t-SLL_MAX_OBJECT_TYPE-1);
				if (dt->fn[SLL_OBJECT_FUNC_STRING]){
					sll_object_t* v=sll_execute_function(dt->fn[SLL_OBJECT_FUNC_STRING],&a,1,0);
					sll_object_t* str=sll_operator_cast(v,sll_static_int[SLL_OBJECT_TYPE_STRING]);
					GC_RELEASE(v);
					sll_string_increase(o,str->dt.s.l);
					sll_copy_data(str->dt.s.v,str->dt.s.l,o->v+o->l);
					o->l+=str->dt.s.l;
					GC_RELEASE(str);
					return;
				}
				sll_string_increase(o,3);
				sll_copy_string(SLL_CHAR("<&:"),o->v+o->l);
				o->l+=3;
				if (!dt->nm.l){
					_write_int(a->t,o);
				}
				else{
					sll_string_increase(o,dt->nm.l);
					sll_copy_data(dt->nm.v,dt->nm.l,o->v+o->l);
					o->l+=dt->nm.l;
				}
				sll_object_field_t* p=a->dt.p;
				for (sll_arg_count_t i=0;i<dt->l;i++){
					sll_string_t s=dt->dt[i].nm;
					sll_string_increase(o,1);
					o->v[o->l]=' ';
					o->l++;
					for (sll_string_length_t j=0;j<s.l;j++){
						_write_char(s.v[j],o);
					}
					sll_string_increase(o,1);
					o->v[o->l]=' ';
					o->l++;
					sll_object_t* tmp;
					switch (dt->dt[i].t){
						case SLL_OBJECT_TYPE_INT:
							tmp=sll_int_to_object(p->i);
							break;
						case SLL_OBJECT_TYPE_FLOAT:
							tmp=sll_float_to_object(p->f);
							break;
						case SLL_OBJECT_TYPE_CHAR:
							tmp=SLL_FROM_CHAR(p->c);
							break;
						default:
							tmp=p->o;
							SLL_ACQUIRE(tmp);
							break;
					}
					_object_to_string(tmp,o);
					GC_RELEASE(tmp);
					p++;
				}
				sll_string_increase(o,1);
				o->v[o->l]='>';
				o->l++;
				return;
			}
	}
	SLL_UNREACHABLE();
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_string_checksum_t sll_api_string_checksum(sll_string_t* str){
	return str->c;
}



__SLL_EXTERNAL __SLL_API_CALL void sll_api_string_convert(sll_object_t*const* args,sll_arg_count_t len,sll_string_t* out){
	STRING_INIT_STACK(out);
	for (sll_array_length_t i=0;i<len;i++){
		sll_object_t* v=*(args+i);
		if (v->t==SLL_OBJECT_TYPE_CHAR){
			sll_string_increase(out,1);
			out->v[out->l]=v->dt.c;
			out->l++;
		}
		else if (v->t==SLL_OBJECT_TYPE_STRING){
			sll_string_increase(out,v->dt.s.l);
			sll_copy_data(v->dt.s.v,v->dt.s.l,out->v+out->l);
			out->l+=v->dt.s.l;
		}
		else{
			_object_to_string(*(args+i),out);
		}
	}
	sll_allocator_move((void**)(&(out->v)),SLL_MEMORY_MOVE_DIRECTION_FROM_STACK);
	sll_string_calculate_checksum(out);
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_integer_t sll_api_string_count(sll_string_t* str,sll_char_string_t* elem){
	return (elem->t==SLL_PARSE_ARGS_TYPE_CHAR?sll_string_count_char(str,elem->dt.c):sll_string_count(str,elem->dt.s));
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_integer_t sll_api_string_count_left(sll_string_t* str,sll_char_t chr){
	return sll_string_count_left(str,chr);
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_integer_t sll_api_string_count_right(sll_string_t* str,sll_char_t chr){
	return sll_string_count_right(str,chr);
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_bool_t sll_api_string_ends(sll_string_t* str,sll_char_string_t* end){
	if (end->t==SLL_PARSE_ARGS_TYPE_CHAR){
		return (str->l&&str->v[str->l-1]==end->dt.c);
	}
	return sll_string_ends(str,end->dt.s);
}



__SLL_EXTERNAL __SLL_API_CALL void sll_api_string_format(sll_string_t* fmt,sll_object_t*const* args,sll_arg_count_t len,sll_string_t* out){
	sll_var_arg_list_t dt={
		SLL_VAR_ARG_LIST_TYPE_SLL,
		{
			.sll={
				args,
				len
			}
		}
	};
	sll_string_format_list(fmt->v,fmt->l,&dt,out);
}



__SLL_EXTERNAL __SLL_API_CALL void sll_api_string_flip_case(sll_string_t* str,sll_string_t* out){
	sll_string_flip_case(str,out);
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_integer_t sll_api_string_index(sll_string_t* str,sll_char_string_t* substr,sll_string_length_t start){
	sll_string_length_t o=(substr->t==SLL_PARSE_ARGS_TYPE_CHAR?sll_string_index_char(str,substr->dt.c,0,start):sll_string_index(str,substr->dt.s,start));
	return (o==SLL_MAX_STRING_INDEX?(sll_integer_t)-1:o);
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_integer_t sll_api_string_index_list(sll_string_t* str,sll_char_string_t* substr,sll_bool_t inv){
	sll_string_length_t o=(substr->t==SLL_PARSE_ARGS_TYPE_CHAR?sll_string_index_char(str,substr->dt.c,inv,0):sll_string_index_multiple(str,substr->dt.s->v,substr->dt.s->l,inv));
	return (o==SLL_MAX_STRING_INDEX?(sll_integer_t)-1:o);
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_integer_t sll_api_string_index_reverse(sll_string_t* str,sll_char_string_t* substr){
	sll_string_length_t o=(substr->t==SLL_PARSE_ARGS_TYPE_CHAR?sll_string_index_reverse_char(str,substr->dt.c,0):sll_string_index_reverse(str,substr->dt.s));
	return (o==SLL_MAX_STRING_INDEX?(sll_integer_t)-1:o);
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_integer_t sll_api_string_index_reverse_list(sll_string_t* str,sll_char_string_t* substr,sll_bool_t inv){
	sll_string_length_t o=(substr->t==SLL_PARSE_ARGS_TYPE_CHAR?sll_string_index_reverse_char(str,substr->dt.c,inv):sll_string_index_reverse_multiple(str,substr->dt.s->v,substr->dt.s->l,inv));
	return (o==SLL_MAX_STRING_INDEX?(sll_integer_t)-1:o);
}



__SLL_EXTERNAL __SLL_API_CALL void sll_api_string_join(sll_char_string_t* infix,sll_array_t* arr,sll_string_t* out){
	if (infix->t==SLL_PARSE_ARGS_TYPE_CHAR){
		sll_string_join_char(infix->dt.c,arr->v,arr->l,out);
	}
	else{
		sll_string_join(infix->dt.s,arr->v,arr->l,out);
	}
}



__SLL_EXTERNAL __SLL_API_CALL void sll_api_string_pad(sll_string_t* str,sll_string_length_t len,sll_char_t chr,sll_string_t* out){
	sll_string_pad(str,len,chr,out);
}



__SLL_EXTERNAL __SLL_API_CALL void sll_api_string_pad_left(sll_string_t* str,sll_string_length_t len,sll_char_t chr,sll_string_t* out){
	sll_string_pad_left(str,len,chr,out);
}



__SLL_EXTERNAL __SLL_API_CALL void sll_api_string_pad_right(sll_string_t* str,sll_string_length_t len,sll_char_t chr,sll_string_t* out){
	sll_string_pad_right(str,len,chr,out);
}



__SLL_EXTERNAL __SLL_API_CALL void sll_api_string_replace(sll_string_t* str,sll_char_string_t* old,sll_char_string_t* new,sll_string_t* out){
	if (old->t==SLL_PARSE_ARGS_TYPE_STRING&&!old->dt.s->l){
		sll_string_clone(str,out);
	}
	else if (new->t==SLL_PARSE_ARGS_TYPE_STRING&&!new->dt.s->l){
		if (old->t==SLL_PARSE_ARGS_TYPE_STRING){
			sll_string_remove(str,old->dt.s,out);
		}
		else{
			sll_string_t s;
			sll_string_from_char(old->dt.c,&s);
			sll_string_remove(str,&s,out);
			sll_free_string(&s);
		}
	}
	else{
		if (old->t==SLL_PARSE_ARGS_TYPE_CHAR&&new->t==SLL_PARSE_ARGS_TYPE_CHAR){
			sll_string_replace_char(str,old->dt.c,new->dt.c,out);
		}
		else if (old->t==SLL_PARSE_ARGS_TYPE_CHAR){
			sll_string_t s;
			sll_string_from_char(old->dt.c,&s);
			sll_string_replace(str,&s,new->dt.s,out);
			sll_free_string(&s);
		}
		else if (new->t==SLL_PARSE_ARGS_TYPE_CHAR){
			sll_string_t s;
			sll_string_from_char(new->dt.c,&s);
			sll_string_replace(str,old->dt.s,&s,out);
			sll_free_string(&s);
		}
		else{
			sll_string_replace(str,old->dt.s,new->dt.s,out);
		}
	}
}



__SLL_EXTERNAL __SLL_API_CALL void sll_api_string_reverse(sll_string_t* str,sll_string_t* out){
	sll_string_reverse(str,out);
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_bool_t sll_api_string_secure_equal(sll_string_t* a,sll_string_t* b){
	return sll_string_secure_equal(a,b);
}



__SLL_EXTERNAL __SLL_API_CALL void sll_api_string_split(sll_string_t* str,sll_char_string_t* infix,sll_array_t* out){
	if (infix->t==SLL_PARSE_ARGS_TYPE_CHAR){
		sll_string_split_char(str,infix->dt.c,out);
	}
	else{
		sll_string_split(str,infix->dt.s,out);
	}
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_bool_t sll_api_string_starts(sll_string_t* str,sll_char_string_t* start){
	if (start->t==SLL_PARSE_ARGS_TYPE_CHAR){
		return (str->l&&str->v[0]==start->dt.c);
	}
	return sll_string_starts(str,start->dt.s);
}



__SLL_EXTERNAL __SLL_API_CALL void sll_api_string_to_lower_case(sll_string_t* str,sll_string_t* out){
	sll_string_lower_case(str,out);
}



__SLL_EXTERNAL __SLL_API_CALL void sll_api_string_to_title_case(sll_string_t* a,sll_string_t* out){
	sll_string_title_case(a,out);
}



__SLL_EXTERNAL __SLL_API_CALL void sll_api_string_trim(sll_string_t* a,sll_string_t* out){
	sll_string_trim(a,out);
}



__SLL_EXTERNAL __SLL_API_CALL void sll_api_string_trim_left(sll_string_t* a,sll_string_t* out){
	sll_string_trim_left(a,out);
}



__SLL_EXTERNAL __SLL_API_CALL void sll_api_string_trim_right(sll_string_t* a,sll_string_t* out){
	sll_string_trim_right(a,out);
}



__SLL_EXTERNAL __SLL_API_CALL void sll_api_string_to_upper_case(sll_string_t* a,sll_string_t* out){
	sll_string_upper_case(a,out);
}
