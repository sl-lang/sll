#include <sll/_internal/api.h>
#include <sll/_internal/common.h>
#include <sll/_internal/serial.h>
#include <sll/api/file.h>
#include <sll/api/serial.h>
#include <sll/array.h>
#include <sll/audit.h>
#include <sll/common.h>
#include <sll/data.h>
#include <sll/file.h>
#include <sll/gc.h>
#include <sll/map.h>
#include <sll/object.h>
#include <sll/sandbox.h>
#include <sll/static_object.h>
#include <sll/string.h>
#include <sll/types.h>



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_size_t sll_decode_integer(sll_file_t* f,sll_bool_t* err){
	sll_read_char_t c=sll_file_read_char(f,NULL);
	if (c==SLL_END_OF_DATA){
		*err=1;
		return 0;
	}
	sll_size_t v=0;
	sll_string_length_t s=0;
	while ((c&0x80)&&s<56){
		v|=((sll_size_t)(c&0x7f))<<s;
		s+=7;
		c=sll_file_read_char(f,NULL);
		if (c==SLL_END_OF_DATA){
			*err=1;
			return 0;
		}
	}
	return v|(((sll_size_t)c)<<s);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_integer_t sll_decode_signed_integer(sll_file_t* f,sll_bool_t* err){
	sll_size_t v=sll_decode_integer(f,err);
	return (v>>1)^(-((sll_integer_t)(v&1)));
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_decode_object(sll_file_t* f){
	if (!f){
		return SLL_ACQUIRE_STATIC_INT(0);
	}
	switch (sll_file_read_char(f,NULL)){
		case SLL_OBJECT_TYPE_INT:
			{
				sll_bool_t err=0;
				sll_integer_t v=sll_decode_signed_integer(f,&err);
				return (err?SLL_ACQUIRE_STATIC_INT(0):sll_int_to_object(v));
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
		case SLL_OBJECT_TYPE_STRING:
			{
				sll_string_t str;
				return (sll_decode_string(f,&str)?sll_string_to_object_nocopy(&str):sll_string_to_object(NULL));
			}
		case SLL_OBJECT_TYPE_ARRAY:
		case SLL_OBJECT_TYPE_MAP_KEYS:
		case SLL_OBJECT_TYPE_MAP_VALUES:
			{
				sll_bool_t err=0;
				sll_array_length_t l=(sll_array_length_t)sll_decode_integer(f,&err);
				if (err){
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
				sll_bool_t err=0;
				sll_map_length_t l=(sll_map_length_t)sll_decode_integer(f,&err);
				if (err){
					return sll_map_to_object(NULL);
				}
				sll_object_t* o=sll_map_length_to_object(l);
				l<<=1;
				for (sll_map_length_t i=0;i<l;i++){
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



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_decode_string(sll_file_t* f,sll_string_t* o){
	if (!f){
		goto _error;
	}
	sll_bool_t err=0;
	o->l=(sll_string_length_t)sll_decode_integer(f,&err);
	if (err){
		goto _error;
	}
	sll_string_create(o->l,o);
	if (o->l<STRING_COMPRESSION_MIN_LENGTH){
		if (sll_file_read(f,o->v,o->l*sizeof(sll_char_t),NULL)!=o->l*sizeof(sll_char_t)){
			goto _error_free;
		}
	}
	else{
		sll_char_t bf[1<<STRING_COMPRESSION_OFFSET_BIT_COUNT];
		sll_zero_memory(bf,((1<<STRING_COMPRESSION_OFFSET_BIT_COUNT)-(1<<STRING_COMPRESSION_LENGTH_BIT_COUNT)-1));
		wide_data_t v;
		if (sll_file_read(f,&v,sizeof(wide_data_t),NULL)!=sizeof(wide_data_t)){
			goto _error_free;
		}
		unsigned int bc=64;
		sll_string_length_t i=0;
		unsigned int r=((1<<STRING_COMPRESSION_OFFSET_BIT_COUNT)-(1<<STRING_COMPRESSION_LENGTH_BIT_COUNT)-1);
		do{
			if (!bc){
				if (sll_file_read(f,&v,sizeof(wide_data_t),NULL)!=sizeof(wide_data_t)){
					goto _error_free;
				}
				bc=64;
			}
			bc--;
			unsigned int e;
			unsigned int el=((v&(1ull<<bc))?8:STRING_COMPRESSION_OFFSET_BIT_COUNT+STRING_COMPRESSION_LENGTH_BIT_COUNT);
			if (bc<el){
				e=(v&((1<<bc)-1))<<(el-bc);
				if (sll_file_read(f,&v,sizeof(wide_data_t),NULL)!=sizeof(wide_data_t)){
					goto _error_free;
				}
				bc+=64-el;
				e|=v>>bc;
			}
			else{
				bc-=el;
				e=(v>>bc)&((1<<el)-1);
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
	return 1;
_error_free:
	sll_free_string(o);
_error:
	SLL_INIT_STRING(o);
	return 0;
}



__SLL_EXTERNAL void sll_encode_integer(sll_file_t* f,sll_size_t v){
	sll_char_t bf[9];
	sll_string_length_t i=0;
	while (i<8&&v>0x7f){
		bf[i]=(v&0x7f)|0x80;
		v>>=7;
		i++;
	}
	bf[i]=(sll_char_t)v;
	sll_file_write(f,bf,i+1,NULL);
}



__SLL_EXTERNAL void sll_encode_signed_integer(sll_file_t* f,sll_integer_t v){
	sll_encode_integer(f,((v<0?~v:v)<<1)|(v<0));
}



__SLL_EXTERNAL void sll_encode_object(sll_file_t* f,sll_object_t*const* a,sll_arg_count_t ac){
	if (!f){
		return;
	}
	while (ac){
		sll_object_t* k=*a;
		a++;
		ac--;
		if (k->t>SLL_MAX_OBJECT_TYPE){
			SLL_UNIMPLEMENTED();
		}
		sll_file_write_char(f,k->t,NULL);
		switch (k->t){
			case SLL_OBJECT_TYPE_INT:
				sll_encode_signed_integer(f,k->dt.i);
				break;
			case SLL_OBJECT_TYPE_FLOAT:
				sll_file_write(f,&(k->dt.f),sizeof(sll_float_t),NULL);
				break;
			case SLL_OBJECT_TYPE_CHAR:
				sll_file_write_char(f,k->dt.c,NULL);
				break;
			case SLL_OBJECT_TYPE_STRING:
				sll_encode_string(f,&(k->dt.s));
				break;
			case SLL_OBJECT_TYPE_ARRAY:
			case SLL_OBJECT_TYPE_MAP_KEYS:
			case SLL_OBJECT_TYPE_MAP_VALUES:
				sll_encode_integer(f,k->dt.a.l);
				sll_encode_object(f,k->dt.a.v,k->dt.a.l);
				break;
			case SLL_OBJECT_TYPE_MAP:
				sll_encode_integer(f,k->dt.m.l);
				sll_encode_object(f,k->dt.m.v,k->dt.m.l<<1);
				break;
			default:
				SLL_UNIMPLEMENTED();
		}
	}
}



__SLL_EXTERNAL void sll_encode_string(sll_file_t* f,const sll_string_t* s){
	if (!f){
		return;
	}
	sll_encode_integer(f,s->l);
	if (s->l<STRING_COMPRESSION_MIN_LENGTH){
		sll_file_write(f,s->v,s->l*sizeof(sll_char_t),NULL);
		return;
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
			sll_file_write(f,&v,sizeof(wide_data_t),NULL);
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
		sll_file_write(f,&v,sizeof(wide_data_t),NULL);
	}
}



__API_FUNC(serial_decode_float){
	if (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_SERIAL)){
		return 0;
	}
	sll_file_t* f=sll_file_from_handle(a);
	if (!f){
		return 0;
	}
	sll_float_t v;
	return (sll_file_read(f,&v,sizeof(sll_float_t),NULL)==sizeof(sll_float_t)?v:0);
}



__API_FUNC(serial_decode_integer){
	if (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_SERIAL)){
		return 0;
	}
	sll_bool_t err=0;
	sll_size_t o=sll_decode_integer(sll_file_from_handle(a),&err);
	return (err?0:(sll_integer_t)o);
}



__API_FUNC(serial_decode_signed_integer){
	if (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_SERIAL)){
		return 0;
	}
	sll_bool_t err=0;
	sll_integer_t o=sll_decode_signed_integer(sll_file_from_handle(a),&err);
	return (err?0:o);
}



__API_FUNC(serial_decode_object){
	if (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_SERIAL)){
		return SLL_ACQUIRE_STATIC_INT(0);
	}
	sll_audit(SLL_CHAR("sll.serial.object.decode"),SLL_CHAR("i"),a);
	return sll_decode_object(sll_file_from_handle(a));
}



__API_FUNC(serial_decode_string){
	if (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_SERIAL)){
		return SLL_ACQUIRE_STATIC_INT(0);
	}
	sll_string_t str;
	if (!sll_decode_string(sll_file_from_handle(a),&str)){
		return SLL_ACQUIRE_STATIC_INT(0);
	}
	return sll_string_to_object_nocopy(&str);
}



__API_FUNC(serial_encode_float){
	if (!sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_SERIAL)){
		sll_file_write(sll_file_from_handle(a),&b,sizeof(sll_float_t),NULL);
	}
}



__API_FUNC(serial_encode_integer){
	if (!sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_SERIAL)){
		sll_encode_integer(sll_file_from_handle(a),(sll_size_t)b);
	}
}



__API_FUNC(serial_encode_signed_integer){
	if (!sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_SERIAL)){
		sll_encode_signed_integer(sll_file_from_handle(a),b);
	}
}



__API_FUNC(serial_encode_object){
	if (!sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_SERIAL)){
		sll_audit(SLL_CHAR("sll.serial.object.encode"),SLL_CHAR("iO*"),a,b,bc);
		sll_encode_object(sll_file_from_handle(a),b,bc);
	}
}



__API_FUNC(serial_encode_string){
	if (!sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_SERIAL)){
		sll_encode_string(sll_file_from_handle(a),b);
	}
}
