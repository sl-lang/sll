#include <sll/_internal/common.h>
#include <sll/_internal/error.h>
#include <sll/_internal/serial.h>
#include <sll/_internal/static_object.h>
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
#include <sll/object.h>
#include <sll/sandbox.h>
#include <sll/static_object.h>
#include <sll/string.h>
#include <sll/types.h>



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_size_t sll_decode_integer(sll_file_t* f,sll_error_t* err){
	ERROR_PTR_RESET;
	sll_read_char_t c=sll_file_read_char(f,err);
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
		c=sll_file_read_char(f,err);
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



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_integer_t sll_decode_signed_integer(sll_file_t* f,sll_error_t* err){
	sll_size_t v=sll_decode_integer(f,err);
	return SLL_DECODE_SIGNED_INTEGER(v);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_decode_object(sll_file_t* f){
	if (!f){
		return SLL_ACQUIRE_STATIC_INT(0);
	}
	switch (sll_file_read_char(f,NULL)){
		case SLL_OBJECT_TYPE_INT:
			{
				sll_error_t err;
				sll_integer_t v=sll_decode_signed_integer(f,&err);
				return (err==SLL_NO_ERROR?SLL_ACQUIRE_STATIC_INT(0):sll_int_to_object(v));
			}
		case SLL_OBJECT_TYPE_FLOAT:
			{
				sll_float_t v;
				if (sll_file_read(f,&v,sizeof(sll_float_t),NULL)!=sizeof(sll_float_t)){
					return SLL_ACQUIRE_STATIC(float_zero);
				}
				return sll_float_to_object(v);
			}
		case SLL_OBJECT_TYPE_CHAR:
			{
				sll_read_char_t v=sll_file_read_char(f,NULL);
				if (v==SLL_END_OF_DATA){
					v=0;
				}
				return SLL_FROM_CHAR(v);
			}
		case SLL_OBJECT_TYPE_COMPLEX:
			SLL_UNIMPLEMENTED();
		case SLL_OBJECT_TYPE_STRING:
			{
				sll_string_t str;
				return (sll_decode_string(f,&str)==SLL_NO_ERROR?STRING_TO_OBJECT_NOCOPY(&str):STRING_TO_OBJECT(NULL));
			}
		case SLL_OBJECT_TYPE_ARRAY:
		case SLL_OBJECT_TYPE_MAP_KEYS:
		case SLL_OBJECT_TYPE_MAP_VALUES:
			{
				sll_error_t err;
				sll_array_length_t l=(sll_array_length_t)sll_decode_integer(f,&err);
				if (err!=SLL_NO_ERROR){
					return sll_array_to_object(NULL);
				}
				sll_object_t* o=sll_array_length_to_object(l);
				for (sll_array_length_t i=0;i<l;i++){
					o->dt.a.v[i]=sll_decode_object(f);
				}
				return o;
			}
		case SLL_OBJECT_TYPE_MAP:
			{
				sll_error_t err;
				sll_map_length_t l=(sll_map_length_t)sll_decode_integer(f,&err);
				if (err!=SLL_NO_ERROR){
					return sll_map_to_object(NULL);
				}
				sll_object_t* o=sll_map_length_to_object(l);
				for (sll_map_length_t i=0;i<(l<<1);i++){
					o->dt.m.v[i]=sll_decode_object(f);
				}
				return o;
			}
		case SLL_END_OF_DATA:
			return SLL_ACQUIRE_STATIC_INT(0);
		default:
			SLL_UNREACHABLE();
	}
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_error_t sll_decode_string(sll_file_t* f,sll_string_t* o){
	SLL_INIT_STRING(o);
	if (!f){
		return SLL_ERROR_UNKNOWN_FD;
	}
	sll_error_t err;
	o->l=(sll_string_length_t)sll_decode_integer(f,&err);
	if (err!=SLL_NO_ERROR){
		return err;
	}
	sll_string_create(o->l,o);
	if (o->l<STRING_COMPRESSION_MIN_LENGTH){
		if (sll_file_read(f,o->v,o->l*sizeof(sll_char_t),&err)!=o->l*sizeof(sll_char_t)){
			goto _error;
		}
	}
	else{
		sll_char_t bf[1<<STRING_COMPRESSION_OFFSET_BIT_COUNT];
		sll_zero_memory(bf,((1<<STRING_COMPRESSION_OFFSET_BIT_COUNT)-(1<<STRING_COMPRESSION_LENGTH_BIT_COUNT)-1));
		wide_data_t v;
		if (sll_file_read(f,&v,sizeof(wide_data_t),&err)!=sizeof(wide_data_t)){
			goto _error;
		}
		unsigned int bc=64;
		sll_string_length_t i=0;
		unsigned int r=((1<<STRING_COMPRESSION_OFFSET_BIT_COUNT)-(1<<STRING_COMPRESSION_LENGTH_BIT_COUNT)-1);
		do{
			if (!bc){
				if (sll_file_read(f,&v,sizeof(wide_data_t),&err)!=sizeof(wide_data_t)){
					goto _error;
				}
				bc=64;
			}
			bc--;
			unsigned int e;
			unsigned int el=((v&(1ull<<bc))?8:STRING_COMPRESSION_OFFSET_BIT_COUNT+STRING_COMPRESSION_LENGTH_BIT_COUNT);
			if (bc<el){
				e=(v&((1ull<<bc)-1))<<(el-bc);
				if (sll_file_read(f,&v,sizeof(wide_data_t),&err)!=sizeof(wide_data_t)){
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
				o->v[i]=(sll_char_t)e;
				bf[r]=(sll_char_t)e;
				i++;
				r=(r+1)&((1<<STRING_COMPRESSION_OFFSET_BIT_COUNT)-1);
			}
			else{
				unsigned int k=e>>STRING_COMPRESSION_LENGTH_BIT_COUNT;
				unsigned int l=k+(e&((1<<STRING_COMPRESSION_LENGTH_BIT_COUNT)-1))+2;
				do{
					bf[r]=bf[k&((1<<STRING_COMPRESSION_OFFSET_BIT_COUNT)-1)];
					o->v[i]=bf[r];
					i++;
					r=(r+1)&((1<<STRING_COMPRESSION_OFFSET_BIT_COUNT)-1);
					k++;
				} while (k<l);
			}
		} while (i<o->l);
	}
	sll_string_calculate_checksum(o);
	return SLL_NO_ERROR;
_error:
	sll_free_string(o);
	return err;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_error_t sll_encode_integer(sll_file_t* f,sll_size_t v){
	sll_char_t bf[9];
	sll_string_length_t i=0;
	while (i<8&&v>0x7f){
		bf[i]=(v&0x7f)|0x80;
		v>>=7;
		i++;
	}
	bf[i]=(sll_char_t)v;
	sll_error_t err;
	sll_file_write(f,bf,i+1,&err);
	return err;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_error_t sll_encode_signed_integer(sll_file_t* f,sll_integer_t v){
	return sll_encode_integer(f,SLL_ENCODE_SIGNED_INTEGER(v));
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_error_t sll_encode_object(sll_file_t* f,sll_object_t*const* a,sll_arg_count_t ac){
	if (!f){
		return SLL_ERROR_UNKNOWN_FD;
	}
	while (ac){
		sll_object_t* k=*a;
		a++;
		ac--;
		if (k->t>SLL_MAX_OBJECT_TYPE){
			SLL_UNIMPLEMENTED();
		}
		sll_error_t err;
		sll_file_write_char(f,k->t,&err);
		if (err!=SLL_NO_ERROR){
			return err;
		}
		switch (k->t){
			case SLL_OBJECT_TYPE_INT:
				err=sll_encode_signed_integer(f,k->dt.i);
				break;
			case SLL_OBJECT_TYPE_FLOAT:
				sll_file_write(f,&(k->dt.f),sizeof(sll_float_t),&err);
				break;
			case SLL_OBJECT_TYPE_CHAR:
				sll_file_write_char(f,k->dt.c,&err);
				break;
			case SLL_OBJECT_TYPE_COMPLEX:
				SLL_UNIMPLEMENTED();
			case SLL_OBJECT_TYPE_STRING:
				err=sll_encode_string(f,&(k->dt.s));
				break;
			case SLL_OBJECT_TYPE_ARRAY:
			case SLL_OBJECT_TYPE_MAP_KEYS:
			case SLL_OBJECT_TYPE_MAP_VALUES:
				err=sll_encode_integer(f,k->dt.a.l);
				if (err!=SLL_NO_ERROR){
					return err;
				}
				err=sll_encode_object(f,k->dt.a.v,k->dt.a.l);
				break;
			case SLL_OBJECT_TYPE_MAP:
				err=sll_encode_integer(f,k->dt.m.l);
				if (err!=SLL_NO_ERROR){
					return err;
				}
				err=sll_encode_object(f,k->dt.m.v,k->dt.m.l<<1);
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



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_error_t sll_encode_string(sll_file_t* f,const sll_string_t* s){
	if (!f){
		return SLL_ERROR_UNKNOWN_FD;
	}
	sll_error_t err=sll_encode_integer(f,s->l);
	if (err!=SLL_NO_ERROR){
		return err;
	}
	if (s->l<STRING_COMPRESSION_MIN_LENGTH){
		sll_file_write(f,s->v,s->l*sizeof(sll_char_t),&err);
		return err;
	}
	wide_data_t v=0;
	unsigned int bc=64;
	sll_char_t bf[1<<(STRING_COMPRESSION_OFFSET_BIT_COUNT+1)];
	sll_zero_memory(bf,((1<<STRING_COMPRESSION_OFFSET_BIT_COUNT)-(1<<STRING_COMPRESSION_LENGTH_BIT_COUNT)-1));
	sll_string_length_t si=0;
	unsigned int i=((1<<STRING_COMPRESSION_OFFSET_BIT_COUNT)-(1<<STRING_COMPRESSION_LENGTH_BIT_COUNT)-1);
	do{
		bf[i]=s->v[si];
		i++;
		si++;
	} while (si<s->l&&i<(1<<(STRING_COMPRESSION_OFFSET_BIT_COUNT+1)));
	unsigned int r=((1<<STRING_COMPRESSION_OFFSET_BIT_COUNT)-(1<<STRING_COMPRESSION_LENGTH_BIT_COUNT)-1);
	do{
		unsigned int st=0;
		unsigned int l=1;
		unsigned int mn=i-r;
		if (mn>(1<<STRING_COMPRESSION_LENGTH_BIT_COUNT)+1){
			mn=(1<<STRING_COMPRESSION_LENGTH_BIT_COUNT)+1;
		}
		sll_char_t c=bf[r];
		for (unsigned int j=r-(((1<<STRING_COMPRESSION_OFFSET_BIT_COUNT)-(1<<STRING_COMPRESSION_LENGTH_BIT_COUNT)-1));j<r;j++){
			if (bf[j]==c){
				unsigned int k=1;
				while (k<mn&&bf[j+k]==bf[r+k]){
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
			sll_file_write(f,&v,sizeof(wide_data_t),&err);
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
				bf[j]=bf[j+(1<<STRING_COMPRESSION_OFFSET_BIT_COUNT)];
			}
			i-=1<<STRING_COMPRESSION_OFFSET_BIT_COUNT;
			r-=1<<STRING_COMPRESSION_OFFSET_BIT_COUNT;
			while (i<(1<<(STRING_COMPRESSION_OFFSET_BIT_COUNT+1))&&si<s->l){
				bf[i]=s->v[si];
				i++;
				si++;
			}
		}
	} while (r<i);
	if (bc!=64){
		v<<=bc;
		sll_file_write(f,&v,sizeof(wide_data_t),&err);
	}
	return err;
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_error_t sll_api_serial_decode_float(sll_integer_t fh,sll_float_t* out){
	if (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_SERIAL)){
		return SLL_ERROR_FROM_SANDBOX(SLL_SANDBOX_FLAG_DISABLE_SERIAL);
	}
	sll_file_t* f=sll_file_from_handle(fh);
	if (!f){
		return SLL_ERROR_UNKNOWN_FD;
	}
	sll_error_t err;
	return (sll_file_read(f,out,sizeof(sll_float_t),&err)==sizeof(sll_float_t)?SLL_NO_ERROR:(err==SLL_NO_ERROR?SLL_ERROR_EOF:err));
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_size_t sll_api_serial_decode_integer(sll_integer_t fh){
	if (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_SERIAL)){
		return 0;
	}
	sll_error_t err;
	sll_size_t o=sll_decode_integer(sll_file_from_handle(fh),&err);
	return (err==SLL_NO_ERROR?o:0);
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_integer_t sll_api_serial_decode_signed_integer(sll_integer_t fh){
	if (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_SERIAL)){
		return 0;
	}
	sll_error_t err;
	sll_integer_t o=sll_decode_signed_integer(sll_file_from_handle(fh),&err);
	return (err==SLL_NO_ERROR?o:0);
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_object_t* sll_api_serial_decode_object(sll_integer_t fh){
	if (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_SERIAL)){
		return SLL_ACQUIRE_STATIC_INT(0);
	}
	sll_audit(SLL_CHAR("sll.serial.object.decode"),SLL_CHAR("i"),fh);
	return sll_decode_object(sll_file_from_handle(fh));
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_error_t sll_api_serial_decode_string(sll_integer_t fh,sll_string_t* out){
	if (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_SERIAL)){
		return SLL_ERROR_FROM_SANDBOX(SLL_SANDBOX_FLAG_DISABLE_SERIAL);
	}
	sll_error_t err=sll_decode_string(sll_file_from_handle(fh),out);
	if (err!=SLL_NO_ERROR){
		sll_free_string(out);
	}
	return err;
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_error_t sll_api_serial_encode_float(sll_integer_t fh,sll_float_t v){
	if (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_SERIAL)){
		return SLL_ERROR_FROM_SANDBOX(SLL_SANDBOX_FLAG_DISABLE_SERIAL);
	}
	sll_error_t err;
	sll_file_write(sll_file_from_handle(fh),&v,sizeof(sll_float_t),&err);
	return err;
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_error_t sll_api_serial_encode_integer(sll_integer_t fh,sll_size_t v){
	if (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_SERIAL)){
		return SLL_ERROR_FROM_SANDBOX(SLL_SANDBOX_FLAG_DISABLE_SERIAL);
	}
	return sll_encode_integer(sll_file_from_handle(fh),v);
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_error_t sll_api_serial_encode_signed_integer(sll_integer_t fh,sll_integer_t v){
	if (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_SERIAL)){
		return SLL_ERROR_FROM_SANDBOX(SLL_SANDBOX_FLAG_DISABLE_SERIAL);
	}
	return sll_encode_signed_integer(sll_file_from_handle(fh),v);
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_error_t sll_api_serial_encode_object(sll_integer_t fh,sll_object_t*const* args,sll_arg_count_t len){
	if (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_SERIAL)){
		return SLL_ERROR_FROM_SANDBOX(SLL_SANDBOX_FLAG_DISABLE_SERIAL);
	}
	sll_audit(SLL_CHAR("sll.serial.object.encode"),SLL_CHAR("iO+"),fh,args,len);
	return sll_encode_object(sll_file_from_handle(fh),args,len);
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_error_t sll_api_serial_encode_string(sll_integer_t fh,const sll_string_t* str){
	if (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_SERIAL)){
		return SLL_ERROR_FROM_SANDBOX(SLL_SANDBOX_FLAG_DISABLE_SERIAL);
	}
	return sll_encode_string(sll_file_from_handle(fh),str);
}
