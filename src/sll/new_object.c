#include <sll/_internal/common.h>
#include <sll/_internal/new_object.h>
#include <sll/_internal/static_object.h>
#include <sll/_internal/var_arg.h>
#include <sll/_size_types.h>
#include <sll/allocator.h>
#include <sll/array.h>
#include <sll/common.h>
#include <sll/gc.h>
#include <sll/log.h>
#include <sll/map.h>
#include <sll/memory.h>
#include <sll/new_object.h>
#include <sll/object.h>
#include <sll/operator.h>
#include <sll/static_object.h>
#include <sll/string.h>
#include <sll/types.h>
#include <sll/var_arg.h>
#include <stdarg.h>



#define BUILD_CLONE_TYPE(type_) \
	do{ \
		if (fl&NEW_OBJECT_FLAG_ARRAY){ \
			SLL_UNIMPLEMENTED(); \
		} \
		sll_object_t* obj=sll_var_arg_get_object(va); \
		sll_object_t* o=(obj->type==(type_)?sll_operator_copy(obj,0):sll_operator_cast(obj,sll_static_int[(type_)])); \
		SLL_RELEASE(obj); \
		return o; \
	} while (0)

#define SKIP_MODIFIERS \
	while (format_length&&(*format==' '||(*format>8&&*format<14)||*format=='!'||*format=='+'||*format=='*')){ \
		format_length--; \
		format++; \
	}
#define SKIP_WHITESPACE_VAR(format,format_length) \
	while ((format_length)&&(*(format)==' '||(*(format)>8&&*(format)<14))){ \
		(format_length)--; \
		(format)++; \
	}
#define SKIP_WHITESPACE SKIP_WHITESPACE_VAR(*format,*format_length)



static sll_object_t* _build_single(const sll_char_t** format,sll_string_length_t* format_length,sll_var_arg_list_t* va);



static void _build_struct_offsets(const sll_char_t** format,sll_string_length_t* format_length,sll_var_arg_list_t* va,struct_offset_builder_t* out){
	SKIP_WHITESPACE;
	if (!(*format_length)){
		return;
	}
	(*format_length)--;
	sll_char_t st=**format;
	(*format)++;
	sll_arg_count_t cnt=1;
	while (*format_length&&(**format==' '||(**format>8&&**format<14)||**format=='!'||**format=='+'||**format=='*')){
		if (**format=='+'){
			cnt=2;
		}
		(*format_length)--;
		(*format)++;
	}
	switch (st){
		case '#':
			out->converter_function_count++;
			out->converter_function_data=sll_reallocate(out->converter_function_data,out->converter_function_count*sizeof(void*));
			*(out->converter_function_data+out->converter_function_count-1)=sll_var_arg_get(va);
		case 'p':
			cnt=1;
		case 'h':
		case 'u':
		case 'i':
		case 'f':
		case 'c':
		case 'C':
		case 'd':
		case 's':
		case 'S':
		case 'l':
		case 'a':
		case 'm':
		case 'O':
			out->offset_count+=cnt;
			out->offset_data=sll_reallocate(out->offset_data,out->offset_count*sizeof(sll_size_t));
			*(out->offset_data+out->offset_count-1)=sll_var_arg_get_int(va);
			if (cnt==2){
				*(out->offset_data+out->offset_count-2)=sll_var_arg_get_int(va);
			}
			return;
		case '(':
		case '[':
		case '<':
			{
				sll_char_t ec=(st=='('?')':(st=='['?']':'>'));
				SKIP_WHITESPACE;
				while (*format_length&&**format!=ec){
					_build_struct_offsets(format,format_length,va,out);
					SKIP_WHITESPACE;
				}
				if (*format_length){
					(*format_length)--;
					(*format)++;
				}
				return;
			}
	}
}



static sll_object_t* _build_struct(const sll_char_t** format,sll_string_length_t* format_length,sll_var_arg_list_t* va,object_creation_flags_t flags){
	addr_t ptr=sll_var_arg_get_int(va);
	sll_array_length_t len=(sll_array_length_t)sll_var_arg_get_int(va);
	sll_size_t sz=sll_var_arg_get_int(va);
	SKIP_WHITESPACE;
	const sll_char_t* base_t=*format;
	sll_string_length_t base_tl=*format_length;
	struct_offset_builder_t off_dt={
		NULL,
		0,
		NULL,
		0
	};
	while (*format_length&&**format!='}'){
		_build_struct_offsets(format,format_length,va,&off_dt);
		SKIP_WHITESPACE;
	}
	if (*format_length){
		(*format_length)--;
		(*format)++;
	}
	sll_object_t* o=sll_array_length_to_object(len);
	sll_bool_t deref=0;
	if (!sz){
		sz=sizeof(const void*);
		deref=1;
	}
	ptr+=len*sz;
	while (len){
		len--;
		ptr-=sz;
		const sll_char_t* arg_t=base_t;
		sll_string_length_t arg_tl=base_tl;
		sll_var_arg_list_t arg_va={
			VAR_ARG_LIST_TYPE_STRUCT,
			{
				.struct_={
					(deref?*((const void**)ptr):(const void*)ptr),
					off_dt.offset_data,
					off_dt.offset_count,
					off_dt.converter_function_data,
					off_dt.converter_function_count
				}
			}
		};
		sll_object_t* arg=sll_array_to_object(NULL);
		o->data.array.data[len]=arg;
		while (arg_tl&&*arg_t!='}'){
			arg->data.array.length++;
			sll_allocator_resize((void**)(&(arg->data.array.data)),arg->data.array.length*sizeof(sll_object_t*));
			arg->data.array.data[arg->data.array.length-1]=_build_single(&arg_t,&arg_tl,&arg_va);
			SKIP_WHITESPACE_VAR(arg_t,arg_tl);
		}
		sll_allocator_collapse((void**)(&(arg->data.array.data)),arg->data.array.length*sizeof(sll_object_t*));
	}
	sll_deallocate(off_dt.offset_data);
	sll_deallocate(off_dt.converter_function_data);
	return o;
}



static sll_object_t* _build_single(const sll_char_t** format,sll_string_length_t* format_length,sll_var_arg_list_t* va){
	SKIP_WHITESPACE;
	if (!(*format_length)){
		return SLL_ACQUIRE_STATIC_INT(0);
	}
	const sll_char_t* src_t=*format;
	(*format_length)--;
	sll_char_t st=**format;
	(*format)++;
	object_creation_flags_t fl=0;
	while (1){
		SKIP_WHITESPACE;
		if (**format=='!'){
			fl|=NEW_OBJECT_FLAG_NO_ACQUIRE;
		}
		else if (**format=='+'){
			fl|=NEW_OBJECT_FLAG_ARRAY;
		}
		else if (**format=='*'){
			fl|=NEW_OBJECT_FLAG_DEREF;
		}
		else{
			break;
		}
		(*format)++;
		(*format_length)--;
	}
	if (va->type!=SLL_VAR_ARG_LIST_TYPE_SLL){
		switch (st){
			case 'h':
				if (fl&NEW_OBJECT_FLAG_ARRAY){
					SLL_UNIMPLEMENTED();
				}
				return sll_int_to_object((__SLL_S32)sll_var_arg_get_int(va));
			case 'u':
				if (fl&NEW_OBJECT_FLAG_ARRAY){
					SLL_UNIMPLEMENTED();
				}
				return sll_int_to_object((__SLL_U32)sll_var_arg_get_int(va));
			case 'p':
				return sll_int_to_object(_var_arg_get_pointer(va));
			case 's':
				{
					if (!(fl&NEW_OBJECT_FLAG_ARRAY)){
						sll_string_t* ptr=PTR(_var_arg_get_pointer(va));
						return (ptr?STRING_TO_OBJECT(ptr):EMPTY_STRING_TO_OBJECT());
					}
					const void* ptr=PTR(_var_arg_get_pointer(va));
					sll_array_length_t len=(sll_array_length_t)sll_var_arg_get_int(va);
					if (!ptr||!len){
						return sll_array_to_object(NULL);
					}
					sll_object_t* o=sll_array_length_to_object(len);
					while (len){
						len--;
						o->data.array.data[len]=STRING_TO_OBJECT(((fl&NEW_OBJECT_FLAG_DEREF)?*(((const sll_string_t*const*)ptr)+len):((const sll_string_t*)ptr)+len));
					}
					return o;
				}
			case 'S':
				{
					if (!(fl&NEW_OBJECT_FLAG_ARRAY)){
						const sll_char_t* ptr=PTR(_var_arg_get_pointer(va));
						return (ptr?STRING_POINTER_TO_OBJECT(ptr):EMPTY_STRING_TO_OBJECT());
					}
					const sll_char_t*const* ptr=PTR(_var_arg_get_pointer(va));
					sll_array_length_t len=(sll_array_length_t)sll_var_arg_get_int(va);
					if (!ptr||!len){
						return sll_array_to_object(NULL);
					}
					sll_object_t* o=sll_array_length_to_object(len);
					while (len){
						len--;
						o->data.array.data[len]=(*(ptr+len)?STRING_POINTER_TO_OBJECT(*(ptr+len)):EMPTY_STRING_TO_OBJECT());
					}
					return o;
				}
			case 'l':
				{
					if (fl&NEW_OBJECT_FLAG_ARRAY){
						SLL_UNIMPLEMENTED();
					}
					const sll_char_t* ptr=sll_var_arg_get(va);
					sll_string_length_t len=(sll_string_length_t)sll_var_arg_get_int(va);
					return (ptr&&len?STRING_POINTER_LENGTH_TO_OBJECT(ptr,len):EMPTY_STRING_TO_OBJECT());
				}
			case 'a':
				if (fl&NEW_OBJECT_FLAG_ARRAY){
					SLL_UNIMPLEMENTED();
				}
				return sll_array_to_object(sll_var_arg_get(va));
			case 'm':
				if (fl&NEW_OBJECT_FLAG_ARRAY){
					SLL_UNIMPLEMENTED();
				}
				return sll_map_to_object(sll_var_arg_get(va));
			case '#':
				return _var_arg_converter(va);
			case '{':
				return _build_struct(format,format_length,va,fl);
		}
	}
	else{
		fl=0;
	}
	switch (st){
		case '1':
			return SLL_ACQUIRE_STATIC_INT(1);
		case 'i':
			if (fl&NEW_OBJECT_FLAG_ARRAY){
				SLL_UNIMPLEMENTED();
			}
			return sll_int_to_object(sll_var_arg_get_int(va));
		case 'f':
			if (fl&NEW_OBJECT_FLAG_ARRAY){
				SLL_UNIMPLEMENTED();
			}
			return sll_float_to_object(sll_var_arg_get_float(va));
		case 'c':
			if (fl&NEW_OBJECT_FLAG_ARRAY){
				SLL_UNIMPLEMENTED();
			}
			return sll_char_to_object(sll_var_arg_get_char(va));
		case 'C':
			if (fl&NEW_OBJECT_FLAG_ARRAY){
				SLL_UNIMPLEMENTED();
			}
			return sll_char_to_string_object(sll_var_arg_get_char(va));
		case 'd':
			{
				if (fl&NEW_OBJECT_FLAG_ARRAY){
					SLL_UNIMPLEMENTED();
				}
				sll_complex_t n;
				sll_var_arg_get_complex(va,&n);
				return sll_complex_to_object(&n);
			}
		case 's':
			BUILD_CLONE_TYPE(SLL_OBJECT_TYPE_STRING);
		case 'z':
			return SLL_FROM_CHAR(0);
		case 'Z':
			return EMPTY_STRING_TO_OBJECT();
		case 'a':
			BUILD_CLONE_TYPE(SLL_OBJECT_TYPE_ARRAY);
		case 'A':
			return sll_array_to_object(NULL);
		case 'm':
			BUILD_CLONE_TYPE(SLL_OBJECT_TYPE_MAP);
		case 'M':
			return sll_map_to_object(NULL);
		case 'O':
			{
				if (fl&NEW_OBJECT_FLAG_ARRAY){
					sll_object_t*const* ptr=sll_var_arg_get(va);
					sll_array_length_t len=(sll_array_length_t)sll_var_arg_get_int(va);
					if (!ptr||!len){
						return sll_array_to_object(NULL);
					}
					sll_object_t* o=sll_array_length_to_object(len);
					while (len){
						len--;
						if (!(fl&NEW_OBJECT_FLAG_NO_ACQUIRE)){
							SLL_ACQUIRE(*(ptr+len));
						}
						o->data.array.data[len]=*(ptr+len);
					}
					return o;
				}
				sll_object_t* o=sll_var_arg_get_object(va);
				if (fl&NEW_OBJECT_FLAG_NO_ACQUIRE){
					SLL_RELEASE(o);
				}
				return o;
			}
		case '(':
		case '[':
			{
				sll_char_t ec=(st=='('?')':']');
				sll_object_t* o=sll_array_to_object(NULL);
				SKIP_WHITESPACE;
				while (*format_length&&**format!=ec){
					o->data.array.length++;
					sll_allocator_resize((void**)(&(o->data.array.data)),o->data.array.length*sizeof(sll_object_t*));
					o->data.array.data[o->data.array.length-1]=_build_single(format,format_length,va);
					SKIP_WHITESPACE;
				}
				if (st=='('){
					sll_allocator_collapse((void**)(&(o->data.array.data)),o->data.array.length*sizeof(sll_object_t*));
				}
				if (*format_length){
					(*format_length)--;
					(*format)++;
				}
				return o;
			}
		case '<':
			{
				sll_object_t* o=sll_map_to_object(NULL);
				sll_bool_t val=0;
				SKIP_WHITESPACE;
				while (*format_length&&**format!='>'){
					if (!val){
						o->data.map.length++;
						o->data.map.data=sll_reallocate(o->data.map.data,(o->data.map.length<<1)*sizeof(sll_object_t*));
					}
					o->data.map.data[((o->data.map.length-1)<<1)+val]=_build_single(format,format_length,va);
					val=!val;
					SKIP_WHITESPACE;
				}
				if (val){
					o->data.map.data[(o->data.map.length<<1)-1]=SLL_ACQUIRE_STATIC_INT(0);
				}
				if (*format_length){
					(*format_length)--;
					(*format)++;
				}
				return o;
			}
	}
	if (st!='0'){
		SLL_WARN("Ignoring unknown format code '%c': %s",st,src_t);
	}
	return SLL_ACQUIRE_STATIC_INT(0);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_new_object(const sll_char_t* format,...){
	va_list va;
	va_start(va,format);
	sll_var_arg_list_t dt;
	SLL_VAR_ARG_INIT_C(&dt,&va);
	sll_object_t* o=sll_new_object_list(format,sll_string_length(format),&dt);
	va_end(va);
	return o;
}



__SLL_EXTERNAL void sll_new_object_array(const sll_char_t* format,sll_array_t* out,...){
	va_list va;
	va_start(va,out);
	sll_var_arg_list_t dt;
	SLL_VAR_ARG_INIT_C(&dt,&va);
	sll_new_object_array_list(format,sll_string_length(format),&dt,out);
	va_end(va);
}



__SLL_EXTERNAL void sll_new_object_array_list(const sll_char_t* format,sll_string_length_t format_length,sll_var_arg_list_t* va,sll_array_t* out){
	sll_array_create(0,out);
	SKIP_MODIFIERS;
	if (!format_length){
		return;
	}
	while (format_length){
		out->length++;
		sll_allocator_resize((void**)(&(out->data)),out->length*sizeof(sll_object_t*));
		out->data[out->length-1]=_build_single(&format,&format_length,va);
	}
	sll_allocator_collapse((void**)(&(out->data)),out->length*sizeof(sll_object_t*));
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_new_object_list(const sll_char_t* format,sll_string_length_t format_length,sll_var_arg_list_t* va){
	if (!format_length){
		return SLL_ACQUIRE_STATIC_INT(0);
	}
	SKIP_MODIFIERS;
	sll_object_t* e=_build_single(&format,&format_length,va);
	if (!format_length){
		return e;
	}
	sll_object_t* o=sll_array_length_to_object(1);
	o->data.array.data[0]=e;
	do{
		o->data.array.length++;
		sll_allocator_resize((void**)(&(o->data.array.data)),o->data.array.length*sizeof(sll_object_t*));
		o->data.array.data[o->data.array.length-1]=_build_single(&format,&format_length,va);
	} while (format_length);
	sll_allocator_collapse((void**)(&(o->data.array.data)),o->data.array.length*sizeof(sll_object_t*));
	return o;
}
