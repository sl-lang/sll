#include <sll/_internal/api/serial.h>
#include <sll/_internal/common.h>
#include <sll/_internal/error.h>
#include <sll/_internal/static_object.h>
#include <sll/_internal/static_string.h>
#include <sll/api/file.h>
#include <sll/api/serial.h>
#include <sll/array.h>
#include <sll/audit.h>
#include <sll/common.h>
#include <sll/data.h>
#include <sll/error.h>
#include <sll/file.h>
#include <sll/gc.h>
#include <sll/map.h>
#include <sll/new_object.h>
#include <sll/object.h>
#include <sll/sandbox.h>
#include <sll/static_object.h>
#include <sll/string.h>
#include <sll/types.h>
#include <sll/vm.h>



static __STATIC_STRING(_serial_object_type_key,"$type");



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_error_t sll_api_serial_decode_float(sll_file_handle_t handle,sll_float_t* out){
	if (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_SERIAL)){
		return SLL_ERROR_FROM_SANDBOX(SLL_SANDBOX_FLAG_DISABLE_SERIAL);
	}
	sll_file_t* file=sll_file_from_handle(handle);
	if (!file){
		return SLL_ERROR_UNKNOWN_FD;
	}
	sll_error_t err;
	return (sll_file_read(file,out,sizeof(sll_float_t),&err)==sizeof(sll_float_t)?SLL_NO_ERROR:(err==SLL_NO_ERROR?SLL_ERROR_EOF:err));
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_error_t sll_api_serial_decode_integer(sll_file_handle_t handle,sll_array_t* out){
	if (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_SERIAL)){
		return SLL_ERROR_FROM_SANDBOX(SLL_SANDBOX_FLAG_DISABLE_SERIAL);
	}
	sll_file_t* file=sll_file_from_handle(handle);
	if (!file){
		return SLL_ERROR_UNKNOWN_FD;
	}
	sll_error_t err;
	sll_integer_t o=sll_decode_integer(file,&err);
	if (err==SLL_NO_ERROR){
		sll_new_object_array(SLL_CHAR("i"),out,o);
	}
	return err;
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_error_t sll_api_serial_decode_object(sll_file_handle_t handle,sll_array_t* out){
	if (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_SERIAL)){
		return SLL_ERROR_FROM_SANDBOX(SLL_SANDBOX_FLAG_DISABLE_SERIAL);
	}
	sll_audit(SLL_CHAR("sll.serial.object.decode"),SLL_CHAR("i"),handle);
	sll_file_t* file=sll_file_from_handle(handle);
	if (!file){
		return SLL_ERROR_UNKNOWN_FD;
	}
	sll_error_t err;
	sll_object_t o=sll_decode_object(file,&err);
	if (err==SLL_NO_ERROR){
		sll_new_object_array(SLL_CHAR("O!"),out,o);
	}
	return err;
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_error_t sll_api_serial_decode_signed_integer(sll_file_handle_t handle,sll_array_t* out){
	if (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_SERIAL)){
		return SLL_ERROR_FROM_SANDBOX(SLL_SANDBOX_FLAG_DISABLE_SERIAL);
	}
	sll_file_t* file=sll_file_from_handle(handle);
	if (!file){
		return SLL_ERROR_UNKNOWN_FD;
	}
	sll_error_t err;
	sll_integer_t o=sll_decode_signed_integer(file,&err);
	if (err==SLL_NO_ERROR){
		sll_new_object_array(SLL_CHAR("i"),out,o);
	}
	return err;
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_error_t sll_api_serial_decode_string(sll_file_handle_t handle,sll_string_t* out){
	if (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_SERIAL)){
		return SLL_ERROR_FROM_SANDBOX(SLL_SANDBOX_FLAG_DISABLE_SERIAL);
	}
	sll_file_t* file=sll_file_from_handle(handle);
	if (!file){
		return SLL_ERROR_UNKNOWN_FD;
	}
	return sll_decode_string(file,out);
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_error_t sll_api_serial_encode_float(sll_file_handle_t handle,sll_float_t v){
	if (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_SERIAL)){
		return SLL_ERROR_FROM_SANDBOX(SLL_SANDBOX_FLAG_DISABLE_SERIAL);
	}
	sll_file_t* file=sll_file_from_handle(handle);
	if (!file){
		return SLL_ERROR_UNKNOWN_FD;
	}
	sll_error_t err;
	return (sll_file_write(file,&v,sizeof(sll_float_t),&err)!=sizeof(sll_float_t)&&err==SLL_NO_ERROR?SLL_ERROR_EOF:err);
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_error_t sll_api_serial_encode_integer(sll_file_handle_t handle,sll_size_t v){
	if (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_SERIAL)){
		return SLL_ERROR_FROM_SANDBOX(SLL_SANDBOX_FLAG_DISABLE_SERIAL);
	}
	sll_file_t* file=sll_file_from_handle(handle);
	if (!file){
		return SLL_ERROR_UNKNOWN_FD;
	}
	return sll_encode_integer(file,v);
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_error_t sll_api_serial_encode_object(sll_file_handle_t handle,const sll_array_t* args){
	if (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_SERIAL)){
		return SLL_ERROR_FROM_SANDBOX(SLL_SANDBOX_FLAG_DISABLE_SERIAL);
	}
	sll_audit(SLL_CHAR("sll.serial.object.encode"),SLL_CHAR("ia"),handle,args);
	sll_file_t* file=sll_file_from_handle(handle);
	if (!file){
		return SLL_ERROR_UNKNOWN_FD;
	}
	return sll_encode_object(file,args->data,args->length);
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_error_t sll_api_serial_encode_signed_integer(sll_file_handle_t handle,sll_integer_t v){
	if (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_SERIAL)){
		return SLL_ERROR_FROM_SANDBOX(SLL_SANDBOX_FLAG_DISABLE_SERIAL);
	}
	sll_file_t* file=sll_file_from_handle(handle);
	if (!file){
		return SLL_ERROR_UNKNOWN_FD;
	}
	return sll_encode_signed_integer(file,v);
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_error_t sll_api_serial_encode_string(sll_file_handle_t handle,const sll_string_t* str){
	if (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_SERIAL)){
		return SLL_ERROR_FROM_SANDBOX(SLL_SANDBOX_FLAG_DISABLE_SERIAL);
	}
	sll_file_t* file=sll_file_from_handle(handle);
	if (!file){
		return SLL_ERROR_UNKNOWN_FD;
	}
	return sll_encode_string(file,str);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_size_t sll_decode_integer(sll_file_t* file,sll_error_t* err){
	ERROR_PTR_RESET;
	sll_read_char_t c=sll_file_read_char(file,err);
	if (err&&*err!=SLL_NO_ERROR){
		return 0;
	}
	if (c==SLL_END_OF_DATA){
		ERROR_PTR(SLL_ERROR_EOF);
		return 0;
	}
	sll_size_t v=0;
	sll_string_length_t s=0;
	while ((c&0x80)&&s<56){
		v|=((sll_size_t)(c&0x7f))<<s;
		s+=7;
		c=sll_file_read_char(file,err);
		if (err&&*err!=SLL_NO_ERROR){
			return 0;
		}
		if (c==SLL_END_OF_DATA){
			ERROR_PTR(SLL_ERROR_EOF);
			return 0;
		}
	}
	return v|(((sll_size_t)c)<<s);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_integer_t sll_decode_signed_integer(sll_file_t* file,sll_error_t* err){
	sll_size_t v=sll_decode_integer(file,err);
	return SLL_DECODE_SIGNED_INTEGER(v);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t sll_decode_object(sll_file_t* file,sll_error_t* err){
	ERROR_PTR_RESET;
	if (!file){
		if (err){
			*err=SLL_ERROR_UNKNOWN_FD;
			return NULL;
		}
		return SLL_ACQUIRE_STATIC_INT(0);
	}
	sll_read_char_t chr=sll_file_read_char(file,err);
	if (err&&*err!=SLL_NO_ERROR){
		return NULL;
	}
	if (chr==SLL_END_OF_DATA){
		if (err){
			*err=SLL_ERROR_EOF;
			return NULL;
		}
		return SLL_ACQUIRE_STATIC_INT(0);
	}
	switch (chr){
		case SLL_OBJECT_TYPE_INT:
			{
				sll_integer_t v=sll_decode_signed_integer(file,err);
				return (err&&*err!=SLL_NO_ERROR?NULL:sll_int_to_object(v));
			}
		case SLL_OBJECT_TYPE_FLOAT:
			{
				sll_float_t v=0;
				if (sll_file_read(file,&v,sizeof(sll_float_t),err)!=sizeof(sll_float_t)){
					if (err&&*err==SLL_NO_ERROR){
						*err=SLL_ERROR_EOF;
					}
					return NULL;
				}
				return (err&&*err!=SLL_NO_ERROR?NULL:sll_float_to_object(v));
			}
		case SLL_OBJECT_TYPE_CHAR:
			{
				sll_read_char_t v=sll_file_read_char(file,err);
				if (err&&*err!=SLL_NO_ERROR){
					return NULL;
				}
				if (v==SLL_END_OF_DATA){
					if (err){
						*err=SLL_ERROR_EOF;
						return NULL;
					}
					v=0;
				}
				return SLL_FROM_CHAR(v);
			}
		case SLL_OBJECT_TYPE_COMPLEX:
			SLL_UNIMPLEMENTED();
		case SLL_OBJECT_TYPE_STRING:
			{
				sll_string_t str;
				sll_error_t str_err=sll_decode_string(file,&str);
				if (str_err==SLL_NO_ERROR){
					return STRING_TO_OBJECT_NOCOPY(&str);
				}
				if (err){
					*err=str_err;
					return NULL;
				}
				return EMPTY_STRING_TO_OBJECT();
			}
		case SLL_OBJECT_TYPE_ARRAY:
		case SLL_OBJECT_TYPE_MAP_KEYS:
		case SLL_OBJECT_TYPE_MAP_VALUES:
			{
				sll_error_t len_err;
				sll_array_length_t l=(sll_array_length_t)sll_decode_integer(file,&len_err);
				if (len_err!=SLL_NO_ERROR){
					if (err){
						*err=len_err;
						return NULL;
					}
					return sll_array_to_object(NULL);
				}
				sll_object_t o=sll_array_length_to_object(l);
				for (sll_array_length_t i=0;i<l;i++){
					o->data.array.data[i]=sll_decode_object(file,err);
					if (err&&*err!=SLL_NO_ERROR){
						SLL_UNIMPLEMENTED();
					}
				}
				return o;
			}
		case SLL_OBJECT_TYPE_MAP:
			{
				sll_error_t len_err;
				sll_map_length_t l=(sll_map_length_t)sll_decode_integer(file,&len_err);
				if (len_err!=SLL_NO_ERROR){
					if (err){
						*err=len_err;
						return NULL;
					}
					return sll_map_to_object(NULL);
				}
				sll_object_t o=sll_map_length_to_object(l);
				for (sll_map_length_t i=0;i<(l<<1);i++){
					o->data.map.data[i]=sll_decode_object(file,err);
					if (err&&*err!=SLL_NO_ERROR){
						SLL_UNIMPLEMENTED();
					}
				}
				return o;
			}
		default:
			SLL_UNREACHABLE();
	}
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_error_t sll_decode_string(sll_file_t* file,sll_string_t* out){
	SLL_INIT_STRING(out);
	if (!file){
		return SLL_ERROR_UNKNOWN_FD;
	}
	sll_error_t err;
	out->length=(sll_string_length_t)sll_decode_integer(file,&err);
	if (err!=SLL_NO_ERROR){
		return err;
	}
	sll_string_create(out->length,out);
	if (out->length<STRING_COMPRESSION_MIN_LENGTH){
		if (sll_file_read(file,out->data,out->length*sizeof(sll_char_t),&err)!=out->length*sizeof(sll_char_t)){
			goto _error;
		}
	}
	else{
		sll_char_t buffer[1<<STRING_COMPRESSION_OFFSET_BIT_COUNT];
		sll_zero_memory(buffer,((1<<STRING_COMPRESSION_OFFSET_BIT_COUNT)-(1<<STRING_COMPRESSION_LENGTH_BIT_COUNT)-1));
		wide_data_t v;
		if (sll_file_read(file,&v,sizeof(wide_data_t),&err)!=sizeof(wide_data_t)){
			goto _error;
		}
		unsigned int bc=64;
		sll_string_length_t i=0;
		unsigned int r=((1<<STRING_COMPRESSION_OFFSET_BIT_COUNT)-(1<<STRING_COMPRESSION_LENGTH_BIT_COUNT)-1);
		do{
			if (!bc){
				if (sll_file_read(file,&v,sizeof(wide_data_t),&err)!=sizeof(wide_data_t)){
					goto _error;
				}
				bc=64;
			}
			bc--;
			unsigned int e;
			unsigned int el=((v&(1ull<<bc))?8:STRING_COMPRESSION_OFFSET_BIT_COUNT+STRING_COMPRESSION_LENGTH_BIT_COUNT);
			if (bc<el){
				e=(v&((1ull<<bc)-1))<<(el-bc);
				if (sll_file_read(file,&v,sizeof(wide_data_t),&err)!=sizeof(wide_data_t)){
					goto _error;
				}
				bc+=64-el;
				e|=v>>bc;
			}
			else{
				bc-=el;
				e=(v>>bc)&((1ull<<el)-1);
			}
			if (el==8){
				out->data[i]=(sll_char_t)e;
				buffer[r]=(sll_char_t)e;
				i++;
				r=(r+1)&((1<<STRING_COMPRESSION_OFFSET_BIT_COUNT)-1);
			}
			else{
				unsigned int k=e>>STRING_COMPRESSION_LENGTH_BIT_COUNT;
				unsigned int l=k+(e&((1<<STRING_COMPRESSION_LENGTH_BIT_COUNT)-1))+2;
				do{
					buffer[r]=buffer[k&((1<<STRING_COMPRESSION_OFFSET_BIT_COUNT)-1)];
					out->data[i]=buffer[r];
					i++;
					r=(r+1)&((1<<STRING_COMPRESSION_OFFSET_BIT_COUNT)-1);
					k++;
				} while (k<l);
			}
		} while (i<out->length);
	}
	sll_string_calculate_checksum(out);
	return SLL_NO_ERROR;
_error:
	sll_free_string(out);
	return err;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_error_t sll_encode_integer(sll_file_t* file,sll_size_t v){
	sll_char_t buffer[9];
	sll_string_length_t i=0;
	while (i<8&&v>0x7f){
		buffer[i]=(v&0x7f)|0x80;
		v>>=7;
		i++;
	}
	buffer[i]=(sll_char_t)v;
	sll_error_t err;
	sll_file_write(file,buffer,i+1,&err);
	return err;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_error_t sll_encode_signed_integer(sll_file_t* file,sll_integer_t v){
	return sll_encode_integer(file,SLL_ENCODE_SIGNED_INTEGER(v));
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_error_t sll_encode_object(sll_file_t* file,const sll_object_t* args,sll_arg_count_t arg_count){
	if (!file){
		return SLL_ERROR_UNKNOWN_FD;
	}
	while (arg_count){
		sll_object_t k=*args;
		args++;
		arg_count--;
		if (k->type>sll_current_runtime_data->type_table->length+SLL_MAX_OBJECT_TYPE){
			k=sll_static_int[0];
		}
		else if (k->type>SLL_MAX_OBJECT_TYPE){
			const sll_object_type_data_t* dt=*(sll_current_runtime_data->type_table->data+k->type-SLL_MAX_OBJECT_TYPE-1);
			sll_error_t err;
			sll_file_write_char(file,SLL_OBJECT_TYPE_MAP,&err);
			if (err!=SLL_NO_ERROR){
				return err;
			}
			err=sll_encode_integer(file,dt->field_count+1);
			if (err!=SLL_NO_ERROR){
				return err;
			}
			sll_file_write_char(file,SLL_OBJECT_TYPE_STRING,&err);
			if (err!=SLL_NO_ERROR){
				return err;
			}
			err=sll_encode_string(file,&_serial_object_type_key);
			if (err!=SLL_NO_ERROR){
				return err;
			}
			sll_file_write_char(file,SLL_OBJECT_TYPE_STRING,&err);
			if (err!=SLL_NO_ERROR){
				return err;
			}
			err=sll_encode_string(file,&(dt->name));
			if (err!=SLL_NO_ERROR){
				return err;
			}
			sll_object_field_t* p=k->data.fields;
			for (sll_arg_count_t i=0;i<dt->field_count;i++){
				sll_file_write_char(file,SLL_OBJECT_TYPE_STRING,&err);
				if (err!=SLL_NO_ERROR){
					return err;
				}
				err=sll_encode_string(file,&(dt->fields[i].name));
				if (err!=SLL_NO_ERROR){
					return err;
				}
				if (dt->fields[i].type>SLL_OBJECT_TYPE_CHAR){
					err=sll_encode_object(file,&(p->any),1);
					if (err!=SLL_NO_ERROR){
						return err;
					}
				}
				else{
					sll_file_write_char(file,dt->fields[i].type,&err);
					if (err!=SLL_NO_ERROR){
						return err;
					}
					if (dt->fields[i].type==SLL_OBJECT_TYPE_INT){
						err=sll_encode_signed_integer(file,p->int_);
					}
					else if (dt->fields[i].type==SLL_OBJECT_TYPE_FLOAT){
						sll_file_write(file,&(p->float_),sizeof(sll_float_t),&err);
					}
					else{
						sll_file_write_char(file,p->char_,&err);
					}
					if (err!=SLL_NO_ERROR){
						return err;
					}
				}
				p++;
			}
			continue;
		}
		sll_error_t err;
		sll_file_write_char(file,k->type,&err);
		if (err!=SLL_NO_ERROR){
			return err;
		}
		switch (k->type){
			case SLL_OBJECT_TYPE_INT:
				err=sll_encode_signed_integer(file,k->data.int_);
				break;
			case SLL_OBJECT_TYPE_FLOAT:
				sll_file_write(file,&(k->data.float_),sizeof(sll_float_t),&err);
				break;
			case SLL_OBJECT_TYPE_CHAR:
				sll_file_write_char(file,k->data.char_,&err);
				break;
			case SLL_OBJECT_TYPE_COMPLEX:
				sll_file_write(file,&(k->data.complex_),sizeof(sll_complex_t),&err);
				break;
			case SLL_OBJECT_TYPE_STRING:
				err=sll_encode_string(file,&(k->data.string));
				break;
			case SLL_OBJECT_TYPE_ARRAY:
			case SLL_OBJECT_TYPE_MAP_KEYS:
			case SLL_OBJECT_TYPE_MAP_VALUES:
				err=sll_encode_integer(file,k->data.array.length);
				if (err!=SLL_NO_ERROR){
					return err;
				}
				err=sll_encode_object(file,k->data.array.data,k->data.array.length);
				break;
			case SLL_OBJECT_TYPE_MAP:
				err=sll_encode_integer(file,k->data.map.length);
				if (err!=SLL_NO_ERROR){
					return err;
				}
				err=sll_encode_object(file,k->data.map.data,k->data.map.length<<1);
				break;
			default:
				SLL_UNIMPLEMENTED();
		}
		if (err!=SLL_NO_ERROR){
			return err;
		}
	}
	return SLL_NO_ERROR;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_error_t sll_encode_string(sll_file_t* file,const sll_string_t* string){
	if (!file){
		return SLL_ERROR_UNKNOWN_FD;
	}
	sll_error_t err=sll_encode_integer(file,string->length);
	if (err!=SLL_NO_ERROR){
		return err;
	}
	if (string->length<STRING_COMPRESSION_MIN_LENGTH){
		sll_file_write(file,string->data,string->length*sizeof(sll_char_t),&err);
		return err;
	}
	wide_data_t v=0;
	unsigned int bc=64;
	sll_char_t buffer[1<<(STRING_COMPRESSION_OFFSET_BIT_COUNT+1)];
	sll_zero_memory(buffer,((1<<STRING_COMPRESSION_OFFSET_BIT_COUNT)-(1<<STRING_COMPRESSION_LENGTH_BIT_COUNT)-1));
	sll_string_length_t si=0;
	unsigned int i=((1<<STRING_COMPRESSION_OFFSET_BIT_COUNT)-(1<<STRING_COMPRESSION_LENGTH_BIT_COUNT)-1);
	do{
		buffer[i]=string->data[si];
		i++;
		si++;
	} while (si<string->length&&i<(1<<(STRING_COMPRESSION_OFFSET_BIT_COUNT+1)));
	unsigned int r=((1<<STRING_COMPRESSION_OFFSET_BIT_COUNT)-(1<<STRING_COMPRESSION_LENGTH_BIT_COUNT)-1);
	do{
		unsigned int st=0;
		unsigned int l=1;
		unsigned int mn=i-r;
		if (mn>(1<<STRING_COMPRESSION_LENGTH_BIT_COUNT)+1){
			mn=(1<<STRING_COMPRESSION_LENGTH_BIT_COUNT)+1;
		}
		sll_char_t c=buffer[r];
		for (unsigned int j=r-(((1<<STRING_COMPRESSION_OFFSET_BIT_COUNT)-(1<<STRING_COMPRESSION_LENGTH_BIT_COUNT)-1));j<r;j++){
			if (buffer[j]==c){
				unsigned int k=1;
				while (k<mn&&buffer[j+k]==buffer[r+k]){
					k++;
				}
				if (k>l){
					st=j;
					l=k;
				}
			}
		}
		unsigned int e;
		unsigned int el;
		if (l==1){
			e=256|c;
			el=9;
		}
		else{
			e=((st&((1<<STRING_COMPRESSION_OFFSET_BIT_COUNT)-1))<<STRING_COMPRESSION_LENGTH_BIT_COUNT)|(l-2);
			el=STRING_COMPRESSION_OFFSET_BIT_COUNT+STRING_COMPRESSION_LENGTH_BIT_COUNT+1;
		}
		SLL_ASSERT(el<=15);
		if (bc<el){
			v=(v<<bc)|(e>>(el-bc));
			sll_file_write(file,&v,sizeof(wide_data_t),&err);
			if (err!=SLL_NO_ERROR){
				return err;
			}
			v=e;
			bc+=64-el;
		}
		else{
			v=(v<<el)|e;
			bc-=el;
		}
		r+=l;
		if (r>=(1<<(STRING_COMPRESSION_OFFSET_BIT_COUNT+1))-(1<<STRING_COMPRESSION_LENGTH_BIT_COUNT)-1){
			for (unsigned int j=0;j<(1<<STRING_COMPRESSION_OFFSET_BIT_COUNT);j++){
				buffer[j]=buffer[j+(1<<STRING_COMPRESSION_OFFSET_BIT_COUNT)];
			}
			i-=1<<STRING_COMPRESSION_OFFSET_BIT_COUNT;
			r-=1<<STRING_COMPRESSION_OFFSET_BIT_COUNT;
			while (i<(1<<(STRING_COMPRESSION_OFFSET_BIT_COUNT+1))&&si<string->length){
				buffer[i]=string->data[si];
				i++;
				si++;
			}
		}
	} while (r<i);
	if (bc!=64){
		v<<=bc;
		sll_file_write(file,&v,sizeof(wide_data_t),&err);
	}
	return err;
}
