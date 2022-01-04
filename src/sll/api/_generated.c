// WARNING: This is an auto-generated file. Any changes made to this file might be lost at any moment. Do Not Edit!
#include <sll/_sll_internal.h>
#include <sll/api.h>
#include <sll/api/_generated.h>
#include <sll/array.h>
#include <sll/common.h>
#include <sll/ift.h>
#include <sll/memory.h>
#include <sll/object.h>
#include <sll/operator.h>
#include <sll/static_object.h>
#include <sll/types.h>



__SLL_API_TYPE_sll_api_array_count sll_api_array_count(__SLL_API_ARGS_sll_api_array_count);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_array_count_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_object_t* a=NULL;
	if (all>0){
		a=*(al+0);
		if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_ARRAY){
			SLL_ACQUIRE(a);
		}
		else{
			a=sll_operator_cast(a,sll_static_int[SLL_OBJECT_TYPE_ARRAY]);
		}
	}
	else{
		a=SLL_CREATE();
		a->t=SLL_OBJECT_TYPE_ARRAY;
		if (!sll_array_create(0,&(a->dt.a))){
			SLL_UNIMPLEMENTED();
		}
	}
	sll_object_t* b=NULL;
	if (all>1){
		b=*(al+1);
		SLL_ACQUIRE(b);
	}
	else{
		b=SLL_ACQUIRE_STATIC_INT(0);
	}
	sll_integer_t out=sll_api_array_count(&(a->dt.a),b);
	SLL_RELEASE(b);
	SLL_RELEASE(a);
	return SLL_FROM_INT(out);
}



__SLL_API_TYPE_sll_api_array_create sll_api_array_create(__SLL_API_ARGS_sll_api_array_create);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_array_create_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_object_t* a=NULL;
	if (all>0){
		a=*(al+0);
		if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_INT){
			SLL_ACQUIRE(a);
		}
		else{
			a=sll_operator_cast(a,sll_static_int[SLL_OBJECT_TYPE_INT]);
		}
	}
	else{
		a=SLL_ACQUIRE_STATIC_INT(0);
	}
	sll_array_t out;
	sll_api_array_create(a->dt.i,&out);
	SLL_RELEASE(a);
	sll_object_t* out_o=SLL_CREATE();
	out_o->t=SLL_OBJECT_TYPE_ARRAY;
	out_o->dt.a=out;
	return out_o;
}



__SLL_API_TYPE_sll_api_array_replace sll_api_array_replace(__SLL_API_ARGS_sll_api_array_replace);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_array_replace_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_object_t* a=NULL;
	if (all>0){
		a=*(al+0);
		if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_ARRAY){
			SLL_ACQUIRE(a);
		}
		else{
			a=sll_operator_cast(a,sll_static_int[SLL_OBJECT_TYPE_ARRAY]);
		}
	}
	else{
		a=SLL_CREATE();
		a->t=SLL_OBJECT_TYPE_ARRAY;
		if (!sll_array_create(0,&(a->dt.a))){
			SLL_UNIMPLEMENTED();
		}
	}
	sll_object_t* b=NULL;
	if (all>1){
		b=*(al+1);
		SLL_ACQUIRE(b);
	}
	else{
		b=SLL_ACQUIRE_STATIC_INT(0);
	}
	sll_object_t* c=NULL;
	if (all>2){
		c=*(al+2);
		SLL_ACQUIRE(c);
	}
	else{
		c=SLL_ACQUIRE_STATIC_INT(0);
	}
	sll_array_t out;
	sll_api_array_replace(&(a->dt.a),b,c,&out);
	SLL_RELEASE(c);
	SLL_RELEASE(b);
	SLL_RELEASE(a);
	sll_object_t* out_o=SLL_CREATE();
	out_o->t=SLL_OBJECT_TYPE_ARRAY;
	out_o->dt.a=out;
	return out_o;
}



__SLL_API_TYPE_sll_api_array_reverse sll_api_array_reverse(__SLL_API_ARGS_sll_api_array_reverse);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_array_reverse_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_object_t* a=NULL;
	if (all>0){
		a=*(al+0);
		if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_ARRAY){
			SLL_ACQUIRE(a);
		}
		else{
			a=sll_operator_cast(a,sll_static_int[SLL_OBJECT_TYPE_ARRAY]);
		}
	}
	else{
		a=SLL_CREATE();
		a->t=SLL_OBJECT_TYPE_ARRAY;
		if (!sll_array_create(0,&(a->dt.a))){
			SLL_UNIMPLEMENTED();
		}
	}
	sll_array_t out;
	sll_api_array_reverse(&(a->dt.a),&out);
	SLL_RELEASE(a);
	sll_object_t* out_o=SLL_CREATE();
	out_o->t=SLL_OBJECT_TYPE_ARRAY;
	out_o->dt.a=out;
	return out_o;
}



__SLL_API_TYPE_sll_api_base64_decode sll_api_base64_decode(__SLL_API_ARGS_sll_api_base64_decode);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_base64_decode_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_object_t* a=NULL;
	if (all>0){
		a=*(al+0);
		if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_STRING){
			SLL_ACQUIRE(a);
		}
		else{
			a=sll_operator_cast(a,sll_static_int[SLL_OBJECT_TYPE_STRING]);
		}
	}
	else{
		a=SLL_CREATE();
		a->t=SLL_OBJECT_TYPE_STRING;
		sll_string_create(0,&(a->dt.s));
	}
	sll_object_t* out=sll_api_base64_decode(&(a->dt.s));
	SLL_RELEASE(a);
	return out;
}



__SLL_API_TYPE_sll_api_base64_encode sll_api_base64_encode(__SLL_API_ARGS_sll_api_base64_encode);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_base64_encode_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_object_t* a=NULL;
	if (all>0){
		a=*(al+0);
		if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_STRING){
			SLL_ACQUIRE(a);
		}
		else{
			a=sll_operator_cast(a,sll_static_int[SLL_OBJECT_TYPE_STRING]);
		}
	}
	else{
		a=SLL_CREATE();
		a->t=SLL_OBJECT_TYPE_STRING;
		sll_string_create(0,&(a->dt.s));
	}
	sll_string_t out;
	sll_api_base64_encode(&(a->dt.s),&out);
	SLL_RELEASE(a);
	sll_object_t* out_o=SLL_CREATE();
	out_o->t=SLL_OBJECT_TYPE_STRING;
	out_o->dt.s=out;
	return out_o;
}



__SLL_API_TYPE_sll_api_date_merge sll_api_date_merge(__SLL_API_ARGS_sll_api_date_merge);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_date_merge_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_object_t* a=NULL;
	if (all>0){
		a=*(al+0);
		if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_INT){
			SLL_ACQUIRE(a);
		}
		else{
			a=sll_operator_cast(a,sll_static_int[SLL_OBJECT_TYPE_INT]);
		}
	}
	else{
		a=SLL_ACQUIRE_STATIC_INT(0);
	}
	sll_object_t* b=NULL;
	if (all>1){
		b=*(al+1);
		if (SLL_OBJECT_GET_TYPE(b)==SLL_OBJECT_TYPE_INT){
			SLL_ACQUIRE(b);
		}
		else{
			b=sll_operator_cast(b,sll_static_int[SLL_OBJECT_TYPE_INT]);
		}
	}
	else{
		b=SLL_ACQUIRE_STATIC_INT(0);
	}
	sll_object_t* c=NULL;
	if (all>2){
		c=*(al+2);
		if (SLL_OBJECT_GET_TYPE(c)==SLL_OBJECT_TYPE_INT){
			SLL_ACQUIRE(c);
		}
		else{
			c=sll_operator_cast(c,sll_static_int[SLL_OBJECT_TYPE_INT]);
		}
	}
	else{
		c=SLL_ACQUIRE_STATIC_INT(0);
	}
	sll_object_t* d=NULL;
	if (all>3){
		d=*(al+3);
		if (SLL_OBJECT_GET_TYPE(d)==SLL_OBJECT_TYPE_INT){
			SLL_ACQUIRE(d);
		}
		else{
			d=sll_operator_cast(d,sll_static_int[SLL_OBJECT_TYPE_INT]);
		}
	}
	else{
		d=SLL_ACQUIRE_STATIC_INT(0);
	}
	sll_object_t* e=NULL;
	if (all>4){
		e=*(al+4);
		if (SLL_OBJECT_GET_TYPE(e)==SLL_OBJECT_TYPE_INT){
			SLL_ACQUIRE(e);
		}
		else{
			e=sll_operator_cast(e,sll_static_int[SLL_OBJECT_TYPE_INT]);
		}
	}
	else{
		e=SLL_ACQUIRE_STATIC_INT(0);
	}
	sll_object_t* f=NULL;
	if (all>5){
		f=*(al+5);
		if (SLL_OBJECT_GET_TYPE(f)==SLL_OBJECT_TYPE_FLOAT){
			SLL_ACQUIRE(f);
		}
		else{
			f=sll_operator_cast(f,sll_static_int[SLL_OBJECT_TYPE_FLOAT]);
		}
	}
	else{
		f=SLL_ACQUIRE_STATIC(float_zero);
	}
	sll_float_t out=sll_api_date_merge(a->dt.i,b->dt.i,c->dt.i,d->dt.i,e->dt.i,f->dt.f);
	SLL_RELEASE(f);
	SLL_RELEASE(e);
	SLL_RELEASE(d);
	SLL_RELEASE(c);
	SLL_RELEASE(b);
	SLL_RELEASE(a);
	return SLL_FROM_FLOAT(out);
}



__SLL_API_TYPE_sll_api_date_split sll_api_date_split(__SLL_API_ARGS_sll_api_date_split);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_date_split_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_object_t* a=NULL;
	if (all>0){
		a=*(al+0);
		if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_FLOAT){
			SLL_ACQUIRE(a);
		}
		else{
			a=sll_operator_cast(a,sll_static_int[SLL_OBJECT_TYPE_FLOAT]);
		}
	}
	else{
		a=SLL_ACQUIRE_STATIC(float_zero);
	}
	sll_array_t out;
	sll_api_date_split(a->dt.f,&out);
	SLL_RELEASE(a);
	sll_object_t* out_o=SLL_CREATE();
	out_o->t=SLL_OBJECT_TYPE_ARRAY;
	out_o->dt.a=out;
	return out_o;
}



__SLL_API_TYPE_sll_api_file_close sll_api_file_close(__SLL_API_ARGS_sll_api_file_close);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_file_close_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_object_t* a=NULL;
	if (all>0){
		a=*(al+0);
		if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_INT){
			SLL_ACQUIRE(a);
		}
		else{
			a=sll_operator_cast(a,sll_static_int[SLL_OBJECT_TYPE_INT]);
		}
	}
	else{
		a=SLL_ACQUIRE_STATIC_INT(0);
	}
	sll_bool_t out=sll_api_file_close(a->dt.i);
	SLL_RELEASE(a);
	SLL_ACQUIRE(sll_static_int[out]);
	return sll_static_int[out];
}



__SLL_API_TYPE_sll_api_file_flush sll_api_file_flush(__SLL_API_ARGS_sll_api_file_flush);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_file_flush_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_object_t* a=NULL;
	if (all>0){
		a=*(al+0);
		if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_INT){
			SLL_ACQUIRE(a);
		}
		else{
			a=sll_operator_cast(a,sll_static_int[SLL_OBJECT_TYPE_INT]);
		}
	}
	else{
		a=SLL_ACQUIRE_STATIC_INT(0);
	}
	sll_bool_t out=sll_api_file_flush(a->dt.i);
	SLL_RELEASE(a);
	SLL_ACQUIRE(sll_static_int[out]);
	return sll_static_int[out];
}



__SLL_API_TYPE_sll_api_file_open sll_api_file_open(__SLL_API_ARGS_sll_api_file_open);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_file_open_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_object_t* a=NULL;
	if (all>0){
		a=*(al+0);
		if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_STRING){
			SLL_ACQUIRE(a);
		}
		else{
			a=sll_operator_cast(a,sll_static_int[SLL_OBJECT_TYPE_STRING]);
		}
	}
	else{
		a=SLL_CREATE();
		a->t=SLL_OBJECT_TYPE_STRING;
		sll_string_create(0,&(a->dt.s));
	}
	sll_object_t* b=NULL;
	if (all>1){
		b=*(al+1);
		if (SLL_OBJECT_GET_TYPE(b)==SLL_OBJECT_TYPE_INT){
			SLL_ACQUIRE(b);
		}
		else{
			b=sll_operator_cast(b,sll_static_int[SLL_OBJECT_TYPE_INT]);
		}
	}
	else{
		b=SLL_ACQUIRE_STATIC_INT(0);
	}
	sll_integer_t out=sll_api_file_open(&(a->dt.s),b->dt.i);
	SLL_RELEASE(b);
	SLL_RELEASE(a);
	return SLL_FROM_INT(out);
}



__SLL_API_TYPE_sll_api_file_read sll_api_file_read(__SLL_API_ARGS_sll_api_file_read);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_file_read_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_object_t* a=NULL;
	if (all>0){
		a=*(al+0);
		if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_INT){
			SLL_ACQUIRE(a);
		}
		else{
			a=sll_operator_cast(a,sll_static_int[SLL_OBJECT_TYPE_INT]);
		}
	}
	else{
		a=SLL_ACQUIRE_STATIC_INT(0);
	}
	sll_object_t* b=NULL;
	if (all>1){
		b=*(al+1);
		if (SLL_OBJECT_GET_TYPE(b)==SLL_OBJECT_TYPE_INT){
			SLL_ACQUIRE(b);
		}
		else{
			b=sll_operator_cast(b,sll_static_int[SLL_OBJECT_TYPE_INT]);
		}
	}
	else{
		b=SLL_ACQUIRE_STATIC_INT(0);
	}
	sll_string_t out;
	sll_api_file_read(a->dt.i,b->dt.i,&out);
	SLL_RELEASE(b);
	SLL_RELEASE(a);
	sll_object_t* out_o=SLL_CREATE();
	out_o->t=SLL_OBJECT_TYPE_STRING;
	out_o->dt.s=out;
	return out_o;
}



__SLL_API_TYPE_sll_api_file_std_handle sll_api_file_std_handle(__SLL_API_ARGS_sll_api_file_std_handle);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_file_std_handle_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_object_t* a=NULL;
	if (all>0){
		a=*(al+0);
		if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_INT){
			SLL_ACQUIRE(a);
		}
		else{
			a=sll_operator_cast(a,sll_static_int[SLL_OBJECT_TYPE_INT]);
		}
	}
	else{
		a=SLL_ACQUIRE_STATIC_INT(0);
	}
	sll_integer_t out=sll_api_file_std_handle(a->dt.i);
	SLL_RELEASE(a);
	return SLL_FROM_INT(out);
}



__SLL_API_TYPE_sll_api_file_write sll_api_file_write(__SLL_API_ARGS_sll_api_file_write);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_file_write_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_object_t* a=NULL;
	if (all>0){
		a=*(al+0);
		if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_INT){
			SLL_ACQUIRE(a);
		}
		else{
			a=sll_operator_cast(a,sll_static_int[SLL_OBJECT_TYPE_INT]);
		}
	}
	else{
		a=SLL_ACQUIRE_STATIC_INT(0);
	}
	sll_object_t*const* b=al+1;
	sll_arg_count_t bc=(all>1?all-1:0);
	sll_integer_t out=sll_api_file_write(a->dt.i,b,bc);
	SLL_RELEASE(a);
	return SLL_FROM_INT(out);
}



__SLL_API_TYPE_sll_api_hash_md5 sll_api_hash_md5(__SLL_API_ARGS_sll_api_hash_md5);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_hash_md5_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_object_t* a=NULL;
	if (all>0){
		a=*(al+0);
		if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_INT){
			SLL_ACQUIRE(a);
		}
		else{
			a=sll_operator_cast(a,sll_static_int[SLL_OBJECT_TYPE_INT]);
		}
	}
	else{
		a=SLL_ACQUIRE_STATIC_INT(0);
	}
	sll_object_t* b=NULL;
	if (all>1){
		b=*(al+1);
		if (SLL_OBJECT_GET_TYPE(b)==SLL_OBJECT_TYPE_INT){
			SLL_ACQUIRE(b);
		}
		else{
			b=sll_operator_cast(b,sll_static_int[SLL_OBJECT_TYPE_INT]);
		}
	}
	else{
		b=SLL_ACQUIRE_STATIC_INT(0);
	}
	sll_object_t* c=NULL;
	if (all>2){
		c=*(al+2);
		if (SLL_OBJECT_GET_TYPE(c)==SLL_OBJECT_TYPE_INT){
			SLL_ACQUIRE(c);
		}
		else{
			c=sll_operator_cast(c,sll_static_int[SLL_OBJECT_TYPE_INT]);
		}
	}
	else{
		c=SLL_ACQUIRE_STATIC_INT(0);
	}
	sll_object_t* d=NULL;
	if (all>3){
		d=*(al+3);
		if (SLL_OBJECT_GET_TYPE(d)==SLL_OBJECT_TYPE_INT){
			SLL_ACQUIRE(d);
		}
		else{
			d=sll_operator_cast(d,sll_static_int[SLL_OBJECT_TYPE_INT]);
		}
	}
	else{
		d=SLL_ACQUIRE_STATIC_INT(0);
	}
	sll_object_t* e=NULL;
	if (all>4){
		e=*(al+4);
		if (SLL_OBJECT_GET_TYPE(e)==SLL_OBJECT_TYPE_STRING){
			SLL_ACQUIRE(e);
		}
		else{
			e=sll_operator_cast(e,sll_static_int[SLL_OBJECT_TYPE_STRING]);
		}
	}
	else{
		e=SLL_CREATE();
		e->t=SLL_OBJECT_TYPE_STRING;
		sll_string_create(0,&(e->dt.s));
	}
	sll_array_t out;
	sll_api_hash_md5(a->dt.i,b->dt.i,c->dt.i,d->dt.i,&(e->dt.s),&out);
	SLL_RELEASE(e);
	SLL_RELEASE(d);
	SLL_RELEASE(c);
	SLL_RELEASE(b);
	SLL_RELEASE(a);
	sll_object_t* out_o=SLL_CREATE();
	out_o->t=SLL_OBJECT_TYPE_ARRAY;
	out_o->dt.a=out;
	return out_o;
}



__SLL_API_TYPE_sll_api_hash_sha1 sll_api_hash_sha1(__SLL_API_ARGS_sll_api_hash_sha1);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_hash_sha1_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_object_t* a=NULL;
	if (all>0){
		a=*(al+0);
		if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_INT){
			SLL_ACQUIRE(a);
		}
		else{
			a=sll_operator_cast(a,sll_static_int[SLL_OBJECT_TYPE_INT]);
		}
	}
	else{
		a=SLL_ACQUIRE_STATIC_INT(0);
	}
	sll_object_t* b=NULL;
	if (all>1){
		b=*(al+1);
		if (SLL_OBJECT_GET_TYPE(b)==SLL_OBJECT_TYPE_INT){
			SLL_ACQUIRE(b);
		}
		else{
			b=sll_operator_cast(b,sll_static_int[SLL_OBJECT_TYPE_INT]);
		}
	}
	else{
		b=SLL_ACQUIRE_STATIC_INT(0);
	}
	sll_object_t* c=NULL;
	if (all>2){
		c=*(al+2);
		if (SLL_OBJECT_GET_TYPE(c)==SLL_OBJECT_TYPE_INT){
			SLL_ACQUIRE(c);
		}
		else{
			c=sll_operator_cast(c,sll_static_int[SLL_OBJECT_TYPE_INT]);
		}
	}
	else{
		c=SLL_ACQUIRE_STATIC_INT(0);
	}
	sll_object_t* d=NULL;
	if (all>3){
		d=*(al+3);
		if (SLL_OBJECT_GET_TYPE(d)==SLL_OBJECT_TYPE_INT){
			SLL_ACQUIRE(d);
		}
		else{
			d=sll_operator_cast(d,sll_static_int[SLL_OBJECT_TYPE_INT]);
		}
	}
	else{
		d=SLL_ACQUIRE_STATIC_INT(0);
	}
	sll_object_t* e=NULL;
	if (all>4){
		e=*(al+4);
		if (SLL_OBJECT_GET_TYPE(e)==SLL_OBJECT_TYPE_INT){
			SLL_ACQUIRE(e);
		}
		else{
			e=sll_operator_cast(e,sll_static_int[SLL_OBJECT_TYPE_INT]);
		}
	}
	else{
		e=SLL_ACQUIRE_STATIC_INT(0);
	}
	sll_object_t* f=NULL;
	if (all>5){
		f=*(al+5);
		if (SLL_OBJECT_GET_TYPE(f)==SLL_OBJECT_TYPE_STRING){
			SLL_ACQUIRE(f);
		}
		else{
			f=sll_operator_cast(f,sll_static_int[SLL_OBJECT_TYPE_STRING]);
		}
	}
	else{
		f=SLL_CREATE();
		f->t=SLL_OBJECT_TYPE_STRING;
		sll_string_create(0,&(f->dt.s));
	}
	sll_array_t out;
	sll_api_hash_sha1(a->dt.i,b->dt.i,c->dt.i,d->dt.i,e->dt.i,&(f->dt.s),&out);
	SLL_RELEASE(f);
	SLL_RELEASE(e);
	SLL_RELEASE(d);
	SLL_RELEASE(c);
	SLL_RELEASE(b);
	SLL_RELEASE(a);
	sll_object_t* out_o=SLL_CREATE();
	out_o->t=SLL_OBJECT_TYPE_ARRAY;
	out_o->dt.a=out;
	return out_o;
}



__SLL_API_TYPE_sll_api_hash_sha256 sll_api_hash_sha256(__SLL_API_ARGS_sll_api_hash_sha256);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_hash_sha256_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_object_t* a=NULL;
	if (all>0){
		a=*(al+0);
		if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_INT){
			SLL_ACQUIRE(a);
		}
		else{
			a=sll_operator_cast(a,sll_static_int[SLL_OBJECT_TYPE_INT]);
		}
	}
	else{
		a=SLL_ACQUIRE_STATIC_INT(0);
	}
	sll_object_t* b=NULL;
	if (all>1){
		b=*(al+1);
		if (SLL_OBJECT_GET_TYPE(b)==SLL_OBJECT_TYPE_INT){
			SLL_ACQUIRE(b);
		}
		else{
			b=sll_operator_cast(b,sll_static_int[SLL_OBJECT_TYPE_INT]);
		}
	}
	else{
		b=SLL_ACQUIRE_STATIC_INT(0);
	}
	sll_object_t* c=NULL;
	if (all>2){
		c=*(al+2);
		if (SLL_OBJECT_GET_TYPE(c)==SLL_OBJECT_TYPE_INT){
			SLL_ACQUIRE(c);
		}
		else{
			c=sll_operator_cast(c,sll_static_int[SLL_OBJECT_TYPE_INT]);
		}
	}
	else{
		c=SLL_ACQUIRE_STATIC_INT(0);
	}
	sll_object_t* d=NULL;
	if (all>3){
		d=*(al+3);
		if (SLL_OBJECT_GET_TYPE(d)==SLL_OBJECT_TYPE_INT){
			SLL_ACQUIRE(d);
		}
		else{
			d=sll_operator_cast(d,sll_static_int[SLL_OBJECT_TYPE_INT]);
		}
	}
	else{
		d=SLL_ACQUIRE_STATIC_INT(0);
	}
	sll_object_t* e=NULL;
	if (all>4){
		e=*(al+4);
		if (SLL_OBJECT_GET_TYPE(e)==SLL_OBJECT_TYPE_INT){
			SLL_ACQUIRE(e);
		}
		else{
			e=sll_operator_cast(e,sll_static_int[SLL_OBJECT_TYPE_INT]);
		}
	}
	else{
		e=SLL_ACQUIRE_STATIC_INT(0);
	}
	sll_object_t* f=NULL;
	if (all>5){
		f=*(al+5);
		if (SLL_OBJECT_GET_TYPE(f)==SLL_OBJECT_TYPE_INT){
			SLL_ACQUIRE(f);
		}
		else{
			f=sll_operator_cast(f,sll_static_int[SLL_OBJECT_TYPE_INT]);
		}
	}
	else{
		f=SLL_ACQUIRE_STATIC_INT(0);
	}
	sll_object_t* g=NULL;
	if (all>6){
		g=*(al+6);
		if (SLL_OBJECT_GET_TYPE(g)==SLL_OBJECT_TYPE_INT){
			SLL_ACQUIRE(g);
		}
		else{
			g=sll_operator_cast(g,sll_static_int[SLL_OBJECT_TYPE_INT]);
		}
	}
	else{
		g=SLL_ACQUIRE_STATIC_INT(0);
	}
	sll_object_t* h=NULL;
	if (all>7){
		h=*(al+7);
		if (SLL_OBJECT_GET_TYPE(h)==SLL_OBJECT_TYPE_INT){
			SLL_ACQUIRE(h);
		}
		else{
			h=sll_operator_cast(h,sll_static_int[SLL_OBJECT_TYPE_INT]);
		}
	}
	else{
		h=SLL_ACQUIRE_STATIC_INT(0);
	}
	sll_object_t* i=NULL;
	if (all>8){
		i=*(al+8);
		if (SLL_OBJECT_GET_TYPE(i)==SLL_OBJECT_TYPE_STRING){
			SLL_ACQUIRE(i);
		}
		else{
			i=sll_operator_cast(i,sll_static_int[SLL_OBJECT_TYPE_STRING]);
		}
	}
	else{
		i=SLL_CREATE();
		i->t=SLL_OBJECT_TYPE_STRING;
		sll_string_create(0,&(i->dt.s));
	}
	sll_array_t out;
	sll_api_hash_sha256(a->dt.i,b->dt.i,c->dt.i,d->dt.i,e->dt.i,f->dt.i,g->dt.i,h->dt.i,&(i->dt.s),&out);
	SLL_RELEASE(i);
	SLL_RELEASE(h);
	SLL_RELEASE(g);
	SLL_RELEASE(f);
	SLL_RELEASE(e);
	SLL_RELEASE(d);
	SLL_RELEASE(c);
	SLL_RELEASE(b);
	SLL_RELEASE(a);
	sll_object_t* out_o=SLL_CREATE();
	out_o->t=SLL_OBJECT_TYPE_ARRAY;
	out_o->dt.a=out;
	return out_o;
}



__SLL_API_TYPE_sll_api_hash_sha512 sll_api_hash_sha512(__SLL_API_ARGS_sll_api_hash_sha512);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_hash_sha512_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_object_t* a=NULL;
	if (all>0){
		a=*(al+0);
		if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_INT){
			SLL_ACQUIRE(a);
		}
		else{
			a=sll_operator_cast(a,sll_static_int[SLL_OBJECT_TYPE_INT]);
		}
	}
	else{
		a=SLL_ACQUIRE_STATIC_INT(0);
	}
	sll_object_t* b=NULL;
	if (all>1){
		b=*(al+1);
		if (SLL_OBJECT_GET_TYPE(b)==SLL_OBJECT_TYPE_INT){
			SLL_ACQUIRE(b);
		}
		else{
			b=sll_operator_cast(b,sll_static_int[SLL_OBJECT_TYPE_INT]);
		}
	}
	else{
		b=SLL_ACQUIRE_STATIC_INT(0);
	}
	sll_object_t* c=NULL;
	if (all>2){
		c=*(al+2);
		if (SLL_OBJECT_GET_TYPE(c)==SLL_OBJECT_TYPE_INT){
			SLL_ACQUIRE(c);
		}
		else{
			c=sll_operator_cast(c,sll_static_int[SLL_OBJECT_TYPE_INT]);
		}
	}
	else{
		c=SLL_ACQUIRE_STATIC_INT(0);
	}
	sll_object_t* d=NULL;
	if (all>3){
		d=*(al+3);
		if (SLL_OBJECT_GET_TYPE(d)==SLL_OBJECT_TYPE_INT){
			SLL_ACQUIRE(d);
		}
		else{
			d=sll_operator_cast(d,sll_static_int[SLL_OBJECT_TYPE_INT]);
		}
	}
	else{
		d=SLL_ACQUIRE_STATIC_INT(0);
	}
	sll_object_t* e=NULL;
	if (all>4){
		e=*(al+4);
		if (SLL_OBJECT_GET_TYPE(e)==SLL_OBJECT_TYPE_INT){
			SLL_ACQUIRE(e);
		}
		else{
			e=sll_operator_cast(e,sll_static_int[SLL_OBJECT_TYPE_INT]);
		}
	}
	else{
		e=SLL_ACQUIRE_STATIC_INT(0);
	}
	sll_object_t* f=NULL;
	if (all>5){
		f=*(al+5);
		if (SLL_OBJECT_GET_TYPE(f)==SLL_OBJECT_TYPE_INT){
			SLL_ACQUIRE(f);
		}
		else{
			f=sll_operator_cast(f,sll_static_int[SLL_OBJECT_TYPE_INT]);
		}
	}
	else{
		f=SLL_ACQUIRE_STATIC_INT(0);
	}
	sll_object_t* g=NULL;
	if (all>6){
		g=*(al+6);
		if (SLL_OBJECT_GET_TYPE(g)==SLL_OBJECT_TYPE_INT){
			SLL_ACQUIRE(g);
		}
		else{
			g=sll_operator_cast(g,sll_static_int[SLL_OBJECT_TYPE_INT]);
		}
	}
	else{
		g=SLL_ACQUIRE_STATIC_INT(0);
	}
	sll_object_t* h=NULL;
	if (all>7){
		h=*(al+7);
		if (SLL_OBJECT_GET_TYPE(h)==SLL_OBJECT_TYPE_INT){
			SLL_ACQUIRE(h);
		}
		else{
			h=sll_operator_cast(h,sll_static_int[SLL_OBJECT_TYPE_INT]);
		}
	}
	else{
		h=SLL_ACQUIRE_STATIC_INT(0);
	}
	sll_object_t* i=NULL;
	if (all>8){
		i=*(al+8);
		if (SLL_OBJECT_GET_TYPE(i)==SLL_OBJECT_TYPE_STRING){
			SLL_ACQUIRE(i);
		}
		else{
			i=sll_operator_cast(i,sll_static_int[SLL_OBJECT_TYPE_STRING]);
		}
	}
	else{
		i=SLL_CREATE();
		i->t=SLL_OBJECT_TYPE_STRING;
		sll_string_create(0,&(i->dt.s));
	}
	sll_array_t out;
	sll_api_hash_sha512(a->dt.i,b->dt.i,c->dt.i,d->dt.i,e->dt.i,f->dt.i,g->dt.i,h->dt.i,&(i->dt.s),&out);
	SLL_RELEASE(i);
	SLL_RELEASE(h);
	SLL_RELEASE(g);
	SLL_RELEASE(f);
	SLL_RELEASE(e);
	SLL_RELEASE(d);
	SLL_RELEASE(c);
	SLL_RELEASE(b);
	SLL_RELEASE(a);
	sll_object_t* out_o=SLL_CREATE();
	out_o->t=SLL_OBJECT_TYPE_ARRAY;
	out_o->dt.a=out;
	return out_o;
}



__SLL_API_TYPE_sll_api_int_to_bin sll_api_int_to_bin(__SLL_API_ARGS_sll_api_int_to_bin);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_int_to_bin_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_object_t* a=NULL;
	if (all>0){
		a=*(al+0);
		if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_INT){
			SLL_ACQUIRE(a);
		}
		else{
			a=sll_operator_cast(a,sll_static_int[SLL_OBJECT_TYPE_INT]);
		}
	}
	else{
		a=SLL_ACQUIRE_STATIC_INT(0);
	}
	sll_string_t out;
	sll_api_int_to_bin(a->dt.i,&out);
	SLL_RELEASE(a);
	sll_object_t* out_o=SLL_CREATE();
	out_o->t=SLL_OBJECT_TYPE_STRING;
	out_o->dt.s=out;
	return out_o;
}



__SLL_API_TYPE_sll_api_int_to_hex sll_api_int_to_hex(__SLL_API_ARGS_sll_api_int_to_hex);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_int_to_hex_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_object_t* a=NULL;
	if (all>0){
		a=*(al+0);
		if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_INT){
			SLL_ACQUIRE(a);
		}
		else{
			a=sll_operator_cast(a,sll_static_int[SLL_OBJECT_TYPE_INT]);
		}
	}
	else{
		a=SLL_ACQUIRE_STATIC_INT(0);
	}
	sll_string_t out;
	sll_api_int_to_hex(a->dt.i,&out);
	SLL_RELEASE(a);
	sll_object_t* out_o=SLL_CREATE();
	out_o->t=SLL_OBJECT_TYPE_STRING;
	out_o->dt.s=out;
	return out_o;
}



__SLL_API_TYPE_sll_api_int_to_oct sll_api_int_to_oct(__SLL_API_ARGS_sll_api_int_to_oct);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_int_to_oct_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_object_t* a=NULL;
	if (all>0){
		a=*(al+0);
		if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_INT){
			SLL_ACQUIRE(a);
		}
		else{
			a=sll_operator_cast(a,sll_static_int[SLL_OBJECT_TYPE_INT]);
		}
	}
	else{
		a=SLL_ACQUIRE_STATIC_INT(0);
	}
	sll_string_t out;
	sll_api_int_to_oct(a->dt.i,&out);
	SLL_RELEASE(a);
	sll_object_t* out_o=SLL_CREATE();
	out_o->t=SLL_OBJECT_TYPE_STRING;
	out_o->dt.s=out;
	return out_o;
}



__SLL_API_TYPE_sll_api_json__init sll_api_json__init(__SLL_API_ARGS_sll_api_json__init);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_json__init_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_object_t* a=NULL;
	if (all>0){
		a=*(al+0);
		SLL_ACQUIRE(a);
	}
	else{
		a=SLL_ACQUIRE_STATIC_INT(0);
	}
	sll_object_t* b=NULL;
	if (all>1){
		b=*(al+1);
		SLL_ACQUIRE(b);
	}
	else{
		b=SLL_ACQUIRE_STATIC_INT(0);
	}
	sll_object_t* c=NULL;
	if (all>2){
		c=*(al+2);
		SLL_ACQUIRE(c);
	}
	else{
		c=SLL_ACQUIRE_STATIC_INT(0);
	}
	sll_api_json__init(a,b,c);
	SLL_RELEASE(c);
	SLL_RELEASE(b);
	SLL_RELEASE(a);
	return SLL_ACQUIRE_STATIC_INT(0);
}



__SLL_API_TYPE_sll_api_json_parse sll_api_json_parse(__SLL_API_ARGS_sll_api_json_parse);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_json_parse_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_object_t* a=NULL;
	if (all>0){
		a=*(al+0);
		if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_STRING){
			SLL_ACQUIRE(a);
		}
		else{
			a=sll_operator_cast(a,sll_static_int[SLL_OBJECT_TYPE_STRING]);
		}
	}
	else{
		a=SLL_CREATE();
		a->t=SLL_OBJECT_TYPE_STRING;
		sll_string_create(0,&(a->dt.s));
	}
	sll_object_t* out=sll_api_json_parse(&(a->dt.s));
	SLL_RELEASE(a);
	return out;
}



__SLL_API_TYPE_sll_api_json_stringify sll_api_json_stringify(__SLL_API_ARGS_sll_api_json_stringify);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_json_stringify_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_object_t* a=NULL;
	if (all>0){
		a=*(al+0);
		SLL_ACQUIRE(a);
	}
	else{
		a=SLL_ACQUIRE_STATIC_INT(0);
	}
	sll_string_t out;
	sll_api_json_stringify(a,&out);
	SLL_RELEASE(a);
	sll_object_t* out_o=SLL_CREATE();
	out_o->t=SLL_OBJECT_TYPE_STRING;
	out_o->dt.s=out;
	return out_o;
}



__SLL_API_TYPE_sll_api_log_log sll_api_log_log(__SLL_API_ARGS_sll_api_log_log);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_log_log_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_object_t*const* a=al+0;
	sll_arg_count_t ac=all;
	sll_api_log_log(a,ac);
	return SLL_ACQUIRE_STATIC_INT(0);
}



__SLL_API_TYPE_sll_api_log_set_default sll_api_log_set_default(__SLL_API_ARGS_sll_api_log_set_default);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_log_set_default_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_object_t* a=NULL;
	if (all>0){
		a=*(al+0);
		if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_INT){
			SLL_ACQUIRE(a);
		}
		else{
			a=sll_operator_cast(a,sll_static_int[SLL_OBJECT_TYPE_INT]);
		}
	}
	else{
		a=SLL_ACQUIRE_STATIC_INT(0);
	}
	sll_object_t* b=NULL;
	if (all>1){
		b=*(al+1);
		if (SLL_OBJECT_GET_TYPE(b)==SLL_OBJECT_TYPE_INT){
			SLL_ACQUIRE(b);
		}
		else{
			b=sll_operator_cast(b,sll_static_int[SLL_OBJECT_TYPE_INT]);
		}
	}
	else{
		b=SLL_ACQUIRE_STATIC_INT(0);
	}
	sll_api_log_set_default(a->dt.i,!!(b->dt.i));
	SLL_RELEASE(b);
	SLL_RELEASE(a);
	return SLL_ACQUIRE_STATIC_INT(0);
}



__SLL_API_TYPE_sll_api_log_set_file sll_api_log_set_file(__SLL_API_ARGS_sll_api_log_set_file);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_log_set_file_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_object_t* a=NULL;
	if (all>0){
		a=*(al+0);
		if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_STRING){
			SLL_ACQUIRE(a);
		}
		else{
			a=sll_operator_cast(a,sll_static_int[SLL_OBJECT_TYPE_STRING]);
		}
	}
	else{
		a=SLL_CREATE();
		a->t=SLL_OBJECT_TYPE_STRING;
		sll_string_create(0,&(a->dt.s));
	}
	sll_object_t* b=NULL;
	if (all>1){
		b=*(al+1);
		if (SLL_OBJECT_GET_TYPE(b)==SLL_OBJECT_TYPE_INT){
			SLL_ACQUIRE(b);
		}
		else{
			b=sll_operator_cast(b,sll_static_int[SLL_OBJECT_TYPE_INT]);
		}
	}
	else{
		b=SLL_ACQUIRE_STATIC_INT(0);
	}
	sll_object_t* c=NULL;
	if (all>2){
		c=*(al+2);
		if (SLL_OBJECT_GET_TYPE(c)==SLL_OBJECT_TYPE_INT){
			SLL_ACQUIRE(c);
		}
		else{
			c=sll_operator_cast(c,sll_static_int[SLL_OBJECT_TYPE_INT]);
		}
	}
	else{
		c=SLL_ACQUIRE_STATIC_INT(0);
	}
	sll_api_log_set_file(&(a->dt.s),b->dt.i,!!(c->dt.i));
	SLL_RELEASE(c);
	SLL_RELEASE(b);
	SLL_RELEASE(a);
	return SLL_ACQUIRE_STATIC_INT(0);
}



__SLL_API_TYPE_sll_api_log_set_function sll_api_log_set_function(__SLL_API_ARGS_sll_api_log_set_function);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_log_set_function_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_object_t* a=NULL;
	if (all>0){
		a=*(al+0);
		if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_STRING){
			SLL_ACQUIRE(a);
		}
		else{
			a=sll_operator_cast(a,sll_static_int[SLL_OBJECT_TYPE_STRING]);
		}
	}
	else{
		a=SLL_CREATE();
		a->t=SLL_OBJECT_TYPE_STRING;
		sll_string_create(0,&(a->dt.s));
	}
	sll_object_t* b=NULL;
	if (all>1){
		b=*(al+1);
		if (SLL_OBJECT_GET_TYPE(b)==SLL_OBJECT_TYPE_STRING){
			SLL_ACQUIRE(b);
		}
		else{
			b=sll_operator_cast(b,sll_static_int[SLL_OBJECT_TYPE_STRING]);
		}
	}
	else{
		b=SLL_CREATE();
		b->t=SLL_OBJECT_TYPE_STRING;
		sll_string_create(0,&(b->dt.s));
	}
	sll_object_t* c=NULL;
	if (all>2){
		c=*(al+2);
		if (SLL_OBJECT_GET_TYPE(c)==SLL_OBJECT_TYPE_INT){
			SLL_ACQUIRE(c);
		}
		else{
			c=sll_operator_cast(c,sll_static_int[SLL_OBJECT_TYPE_INT]);
		}
	}
	else{
		c=SLL_ACQUIRE_STATIC_INT(0);
	}
	sll_object_t* d=NULL;
	if (all>3){
		d=*(al+3);
		if (SLL_OBJECT_GET_TYPE(d)==SLL_OBJECT_TYPE_INT){
			SLL_ACQUIRE(d);
		}
		else{
			d=sll_operator_cast(d,sll_static_int[SLL_OBJECT_TYPE_INT]);
		}
	}
	else{
		d=SLL_ACQUIRE_STATIC_INT(0);
	}
	sll_api_log_set_function(&(a->dt.s),&(b->dt.s),c->dt.i,!!(d->dt.i));
	SLL_RELEASE(d);
	SLL_RELEASE(c);
	SLL_RELEASE(b);
	SLL_RELEASE(a);
	return SLL_ACQUIRE_STATIC_INT(0);
}



__SLL_API_TYPE_sll_api_math_abs sll_api_math_abs(__SLL_API_ARGS_sll_api_math_abs);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_math_abs_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_object_t* a=NULL;
	if (all>0){
		a=*(al+0);
		if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_FLOAT){
			SLL_ACQUIRE(a);
		}
		else{
			a=sll_operator_cast(a,sll_static_int[SLL_OBJECT_TYPE_FLOAT]);
		}
	}
	else{
		a=SLL_ACQUIRE_STATIC(float_zero);
	}
	sll_float_t out=sll_api_math_abs(a->dt.f);
	SLL_RELEASE(a);
	return SLL_FROM_FLOAT(out);
}



__SLL_API_TYPE_sll_api_math_acos sll_api_math_acos(__SLL_API_ARGS_sll_api_math_acos);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_math_acos_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_object_t* a=NULL;
	if (all>0){
		a=*(al+0);
		if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_FLOAT){
			SLL_ACQUIRE(a);
		}
		else{
			a=sll_operator_cast(a,sll_static_int[SLL_OBJECT_TYPE_FLOAT]);
		}
	}
	else{
		a=SLL_ACQUIRE_STATIC(float_zero);
	}
	sll_float_t out=sll_api_math_acos(a->dt.f);
	SLL_RELEASE(a);
	return SLL_FROM_FLOAT(out);
}



__SLL_API_TYPE_sll_api_math_acosh sll_api_math_acosh(__SLL_API_ARGS_sll_api_math_acosh);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_math_acosh_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_object_t* a=NULL;
	if (all>0){
		a=*(al+0);
		if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_FLOAT){
			SLL_ACQUIRE(a);
		}
		else{
			a=sll_operator_cast(a,sll_static_int[SLL_OBJECT_TYPE_FLOAT]);
		}
	}
	else{
		a=SLL_ACQUIRE_STATIC(float_zero);
	}
	sll_float_t out=sll_api_math_acosh(a->dt.f);
	SLL_RELEASE(a);
	return SLL_FROM_FLOAT(out);
}



__SLL_API_TYPE_sll_api_math_asin sll_api_math_asin(__SLL_API_ARGS_sll_api_math_asin);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_math_asin_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_object_t* a=NULL;
	if (all>0){
		a=*(al+0);
		if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_FLOAT){
			SLL_ACQUIRE(a);
		}
		else{
			a=sll_operator_cast(a,sll_static_int[SLL_OBJECT_TYPE_FLOAT]);
		}
	}
	else{
		a=SLL_ACQUIRE_STATIC(float_zero);
	}
	sll_float_t out=sll_api_math_asin(a->dt.f);
	SLL_RELEASE(a);
	return SLL_FROM_FLOAT(out);
}



__SLL_API_TYPE_sll_api_math_asinh sll_api_math_asinh(__SLL_API_ARGS_sll_api_math_asinh);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_math_asinh_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_object_t* a=NULL;
	if (all>0){
		a=*(al+0);
		if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_FLOAT){
			SLL_ACQUIRE(a);
		}
		else{
			a=sll_operator_cast(a,sll_static_int[SLL_OBJECT_TYPE_FLOAT]);
		}
	}
	else{
		a=SLL_ACQUIRE_STATIC(float_zero);
	}
	sll_float_t out=sll_api_math_asinh(a->dt.f);
	SLL_RELEASE(a);
	return SLL_FROM_FLOAT(out);
}



__SLL_API_TYPE_sll_api_math_atan sll_api_math_atan(__SLL_API_ARGS_sll_api_math_atan);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_math_atan_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_object_t* a=NULL;
	if (all>0){
		a=*(al+0);
		if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_FLOAT){
			SLL_ACQUIRE(a);
		}
		else{
			a=sll_operator_cast(a,sll_static_int[SLL_OBJECT_TYPE_FLOAT]);
		}
	}
	else{
		a=SLL_ACQUIRE_STATIC(float_zero);
	}
	sll_float_t out=sll_api_math_atan(a->dt.f);
	SLL_RELEASE(a);
	return SLL_FROM_FLOAT(out);
}



__SLL_API_TYPE_sll_api_math_atan2 sll_api_math_atan2(__SLL_API_ARGS_sll_api_math_atan2);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_math_atan2_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_object_t* a=NULL;
	if (all>0){
		a=*(al+0);
		if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_FLOAT){
			SLL_ACQUIRE(a);
		}
		else{
			a=sll_operator_cast(a,sll_static_int[SLL_OBJECT_TYPE_FLOAT]);
		}
	}
	else{
		a=SLL_ACQUIRE_STATIC(float_zero);
	}
	sll_object_t* b=NULL;
	if (all>1){
		b=*(al+1);
		if (SLL_OBJECT_GET_TYPE(b)==SLL_OBJECT_TYPE_FLOAT){
			SLL_ACQUIRE(b);
		}
		else{
			b=sll_operator_cast(b,sll_static_int[SLL_OBJECT_TYPE_FLOAT]);
		}
	}
	else{
		b=SLL_ACQUIRE_STATIC(float_zero);
	}
	sll_float_t out=sll_api_math_atan2(a->dt.f,b->dt.f);
	SLL_RELEASE(b);
	SLL_RELEASE(a);
	return SLL_FROM_FLOAT(out);
}



__SLL_API_TYPE_sll_api_math_atanh sll_api_math_atanh(__SLL_API_ARGS_sll_api_math_atanh);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_math_atanh_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_object_t* a=NULL;
	if (all>0){
		a=*(al+0);
		if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_FLOAT){
			SLL_ACQUIRE(a);
		}
		else{
			a=sll_operator_cast(a,sll_static_int[SLL_OBJECT_TYPE_FLOAT]);
		}
	}
	else{
		a=SLL_ACQUIRE_STATIC(float_zero);
	}
	sll_float_t out=sll_api_math_atanh(a->dt.f);
	SLL_RELEASE(a);
	return SLL_FROM_FLOAT(out);
}



__SLL_API_TYPE_sll_api_math_cbrt sll_api_math_cbrt(__SLL_API_ARGS_sll_api_math_cbrt);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_math_cbrt_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_object_t* a=NULL;
	if (all>0){
		a=*(al+0);
		if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_FLOAT){
			SLL_ACQUIRE(a);
		}
		else{
			a=sll_operator_cast(a,sll_static_int[SLL_OBJECT_TYPE_FLOAT]);
		}
	}
	else{
		a=SLL_ACQUIRE_STATIC(float_zero);
	}
	sll_float_t out=sll_api_math_cbrt(a->dt.f);
	SLL_RELEASE(a);
	return SLL_FROM_FLOAT(out);
}



__SLL_API_TYPE_sll_api_math_combinations sll_api_math_combinations(__SLL_API_ARGS_sll_api_math_combinations);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_math_combinations_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_object_t* a=NULL;
	if (all>0){
		a=*(al+0);
		if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_INT){
			SLL_ACQUIRE(a);
		}
		else{
			a=sll_operator_cast(a,sll_static_int[SLL_OBJECT_TYPE_INT]);
		}
	}
	else{
		a=SLL_ACQUIRE_STATIC_INT(0);
	}
	sll_object_t* b=NULL;
	if (all>1){
		b=*(al+1);
		if (SLL_OBJECT_GET_TYPE(b)==SLL_OBJECT_TYPE_INT){
			SLL_ACQUIRE(b);
		}
		else{
			b=sll_operator_cast(b,sll_static_int[SLL_OBJECT_TYPE_INT]);
		}
	}
	else{
		b=SLL_ACQUIRE_STATIC_INT(0);
	}
	sll_integer_t out=sll_api_math_combinations(a->dt.i,b->dt.i);
	SLL_RELEASE(b);
	SLL_RELEASE(a);
	return SLL_FROM_INT(out);
}



__SLL_API_TYPE_sll_api_math_cos sll_api_math_cos(__SLL_API_ARGS_sll_api_math_cos);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_math_cos_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_object_t* a=NULL;
	if (all>0){
		a=*(al+0);
		if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_FLOAT){
			SLL_ACQUIRE(a);
		}
		else{
			a=sll_operator_cast(a,sll_static_int[SLL_OBJECT_TYPE_FLOAT]);
		}
	}
	else{
		a=SLL_ACQUIRE_STATIC(float_zero);
	}
	sll_float_t out=sll_api_math_cos(a->dt.f);
	SLL_RELEASE(a);
	return SLL_FROM_FLOAT(out);
}



__SLL_API_TYPE_sll_api_math_cosh sll_api_math_cosh(__SLL_API_ARGS_sll_api_math_cosh);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_math_cosh_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_object_t* a=NULL;
	if (all>0){
		a=*(al+0);
		if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_FLOAT){
			SLL_ACQUIRE(a);
		}
		else{
			a=sll_operator_cast(a,sll_static_int[SLL_OBJECT_TYPE_FLOAT]);
		}
	}
	else{
		a=SLL_ACQUIRE_STATIC(float_zero);
	}
	sll_float_t out=sll_api_math_cosh(a->dt.f);
	SLL_RELEASE(a);
	return SLL_FROM_FLOAT(out);
}



__SLL_API_TYPE_sll_api_math_ceil sll_api_math_ceil(__SLL_API_ARGS_sll_api_math_ceil);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_math_ceil_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_object_t* a=NULL;
	if (all>0){
		a=*(al+0);
		if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_FLOAT){
			SLL_ACQUIRE(a);
		}
		else{
			a=sll_operator_cast(a,sll_static_int[SLL_OBJECT_TYPE_FLOAT]);
		}
	}
	else{
		a=SLL_ACQUIRE_STATIC(float_zero);
	}
	sll_float_t out=sll_api_math_ceil(a->dt.f);
	SLL_RELEASE(a);
	return SLL_FROM_FLOAT(out);
}



__SLL_API_TYPE_sll_api_math_copy_sign sll_api_math_copy_sign(__SLL_API_ARGS_sll_api_math_copy_sign);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_math_copy_sign_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_object_t* a=NULL;
	if (all>0){
		a=*(al+0);
		if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_FLOAT){
			SLL_ACQUIRE(a);
		}
		else{
			a=sll_operator_cast(a,sll_static_int[SLL_OBJECT_TYPE_FLOAT]);
		}
	}
	else{
		a=SLL_ACQUIRE_STATIC(float_zero);
	}
	sll_object_t* b=NULL;
	if (all>1){
		b=*(al+1);
		if (SLL_OBJECT_GET_TYPE(b)==SLL_OBJECT_TYPE_FLOAT){
			SLL_ACQUIRE(b);
		}
		else{
			b=sll_operator_cast(b,sll_static_int[SLL_OBJECT_TYPE_FLOAT]);
		}
	}
	else{
		b=SLL_ACQUIRE_STATIC(float_zero);
	}
	sll_float_t out=sll_api_math_copy_sign(a->dt.f,b->dt.f);
	SLL_RELEASE(b);
	SLL_RELEASE(a);
	return SLL_FROM_FLOAT(out);
}



__SLL_API_TYPE_sll_api_math_factorial sll_api_math_factorial(__SLL_API_ARGS_sll_api_math_factorial);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_math_factorial_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_object_t* a=NULL;
	if (all>0){
		a=*(al+0);
		if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_INT){
			SLL_ACQUIRE(a);
		}
		else{
			a=sll_operator_cast(a,sll_static_int[SLL_OBJECT_TYPE_INT]);
		}
	}
	else{
		a=SLL_ACQUIRE_STATIC_INT(0);
	}
	sll_integer_t out=sll_api_math_factorial(a->dt.i);
	SLL_RELEASE(a);
	return SLL_FROM_INT(out);
}



__SLL_API_TYPE_sll_api_math_gcd sll_api_math_gcd(__SLL_API_ARGS_sll_api_math_gcd);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_math_gcd_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_object_t* a=NULL;
	if (all>0){
		a=*(al+0);
		if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_INT){
			SLL_ACQUIRE(a);
		}
		else{
			a=sll_operator_cast(a,sll_static_int[SLL_OBJECT_TYPE_INT]);
		}
	}
	else{
		a=SLL_ACQUIRE_STATIC_INT(0);
	}
	sll_object_t* b=NULL;
	if (all>1){
		b=*(al+1);
		if (SLL_OBJECT_GET_TYPE(b)==SLL_OBJECT_TYPE_INT){
			SLL_ACQUIRE(b);
		}
		else{
			b=sll_operator_cast(b,sll_static_int[SLL_OBJECT_TYPE_INT]);
		}
	}
	else{
		b=SLL_ACQUIRE_STATIC_INT(0);
	}
	sll_integer_t out=sll_api_math_gcd(a->dt.i,b->dt.i);
	SLL_RELEASE(b);
	SLL_RELEASE(a);
	return SLL_FROM_INT(out);
}



__SLL_API_TYPE_sll_api_math_floor sll_api_math_floor(__SLL_API_ARGS_sll_api_math_floor);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_math_floor_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_object_t* a=NULL;
	if (all>0){
		a=*(al+0);
		if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_FLOAT){
			SLL_ACQUIRE(a);
		}
		else{
			a=sll_operator_cast(a,sll_static_int[SLL_OBJECT_TYPE_FLOAT]);
		}
	}
	else{
		a=SLL_ACQUIRE_STATIC(float_zero);
	}
	sll_float_t out=sll_api_math_floor(a->dt.f);
	SLL_RELEASE(a);
	return SLL_FROM_FLOAT(out);
}



__SLL_API_TYPE_sll_api_math_int_log2 sll_api_math_int_log2(__SLL_API_ARGS_sll_api_math_int_log2);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_math_int_log2_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_object_t* a=NULL;
	if (all>0){
		a=*(al+0);
		if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_INT){
			SLL_ACQUIRE(a);
		}
		else{
			a=sll_operator_cast(a,sll_static_int[SLL_OBJECT_TYPE_INT]);
		}
	}
	else{
		a=SLL_ACQUIRE_STATIC_INT(0);
	}
	sll_integer_t out=sll_api_math_int_log2(a->dt.i);
	SLL_RELEASE(a);
	return SLL_FROM_INT(out);
}



__SLL_API_TYPE_sll_api_math_int_pow sll_api_math_int_pow(__SLL_API_ARGS_sll_api_math_int_pow);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_math_int_pow_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_object_t* a=NULL;
	if (all>0){
		a=*(al+0);
		if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_INT){
			SLL_ACQUIRE(a);
		}
		else{
			a=sll_operator_cast(a,sll_static_int[SLL_OBJECT_TYPE_INT]);
		}
	}
	else{
		a=SLL_ACQUIRE_STATIC_INT(0);
	}
	sll_object_t* b=NULL;
	if (all>1){
		b=*(al+1);
		if (SLL_OBJECT_GET_TYPE(b)==SLL_OBJECT_TYPE_INT){
			SLL_ACQUIRE(b);
		}
		else{
			b=sll_operator_cast(b,sll_static_int[SLL_OBJECT_TYPE_INT]);
		}
	}
	else{
		b=SLL_ACQUIRE_STATIC_INT(0);
	}
	sll_integer_t out=sll_api_math_int_pow(a->dt.i,b->dt.i);
	SLL_RELEASE(b);
	SLL_RELEASE(a);
	return SLL_FROM_INT(out);
}



__SLL_API_TYPE_sll_api_math_int_sqrt sll_api_math_int_sqrt(__SLL_API_ARGS_sll_api_math_int_sqrt);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_math_int_sqrt_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_object_t* a=NULL;
	if (all>0){
		a=*(al+0);
		if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_INT){
			SLL_ACQUIRE(a);
		}
		else{
			a=sll_operator_cast(a,sll_static_int[SLL_OBJECT_TYPE_INT]);
		}
	}
	else{
		a=SLL_ACQUIRE_STATIC_INT(0);
	}
	sll_integer_t out=sll_api_math_int_sqrt(a->dt.i);
	SLL_RELEASE(a);
	return SLL_FROM_INT(out);
}



__SLL_API_TYPE_sll_api_math_log sll_api_math_log(__SLL_API_ARGS_sll_api_math_log);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_math_log_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_object_t* a=NULL;
	if (all>0){
		a=*(al+0);
		if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_FLOAT){
			SLL_ACQUIRE(a);
		}
		else{
			a=sll_operator_cast(a,sll_static_int[SLL_OBJECT_TYPE_FLOAT]);
		}
	}
	else{
		a=SLL_ACQUIRE_STATIC(float_zero);
	}
	sll_float_t out=sll_api_math_log(a->dt.f);
	SLL_RELEASE(a);
	return SLL_FROM_FLOAT(out);
}



__SLL_API_TYPE_sll_api_math_log2 sll_api_math_log2(__SLL_API_ARGS_sll_api_math_log2);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_math_log2_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_object_t* a=NULL;
	if (all>0){
		a=*(al+0);
		if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_FLOAT){
			SLL_ACQUIRE(a);
		}
		else{
			a=sll_operator_cast(a,sll_static_int[SLL_OBJECT_TYPE_FLOAT]);
		}
	}
	else{
		a=SLL_ACQUIRE_STATIC(float_zero);
	}
	sll_float_t out=sll_api_math_log2(a->dt.f);
	SLL_RELEASE(a);
	return SLL_FROM_FLOAT(out);
}



__SLL_API_TYPE_sll_api_math_log10 sll_api_math_log10(__SLL_API_ARGS_sll_api_math_log10);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_math_log10_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_object_t* a=NULL;
	if (all>0){
		a=*(al+0);
		if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_FLOAT){
			SLL_ACQUIRE(a);
		}
		else{
			a=sll_operator_cast(a,sll_static_int[SLL_OBJECT_TYPE_FLOAT]);
		}
	}
	else{
		a=SLL_ACQUIRE_STATIC(float_zero);
	}
	sll_float_t out=sll_api_math_log10(a->dt.f);
	SLL_RELEASE(a);
	return SLL_FROM_FLOAT(out);
}



__SLL_API_TYPE_sll_api_math_permutations sll_api_math_permutations(__SLL_API_ARGS_sll_api_math_permutations);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_math_permutations_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_object_t* a=NULL;
	if (all>0){
		a=*(al+0);
		if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_INT){
			SLL_ACQUIRE(a);
		}
		else{
			a=sll_operator_cast(a,sll_static_int[SLL_OBJECT_TYPE_INT]);
		}
	}
	else{
		a=SLL_ACQUIRE_STATIC_INT(0);
	}
	sll_object_t* b=NULL;
	if (all>1){
		b=*(al+1);
		if (SLL_OBJECT_GET_TYPE(b)==SLL_OBJECT_TYPE_INT){
			SLL_ACQUIRE(b);
		}
		else{
			b=sll_operator_cast(b,sll_static_int[SLL_OBJECT_TYPE_INT]);
		}
	}
	else{
		b=SLL_ACQUIRE_STATIC_INT(0);
	}
	sll_integer_t out=sll_api_math_permutations(a->dt.i,b->dt.i);
	SLL_RELEASE(b);
	SLL_RELEASE(a);
	return SLL_FROM_INT(out);
}



__SLL_API_TYPE_sll_api_math_pow sll_api_math_pow(__SLL_API_ARGS_sll_api_math_pow);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_math_pow_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_object_t* a=NULL;
	if (all>0){
		a=*(al+0);
		if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_FLOAT){
			SLL_ACQUIRE(a);
		}
		else{
			a=sll_operator_cast(a,sll_static_int[SLL_OBJECT_TYPE_FLOAT]);
		}
	}
	else{
		a=SLL_ACQUIRE_STATIC(float_zero);
	}
	sll_object_t* b=NULL;
	if (all>1){
		b=*(al+1);
		if (SLL_OBJECT_GET_TYPE(b)==SLL_OBJECT_TYPE_FLOAT){
			SLL_ACQUIRE(b);
		}
		else{
			b=sll_operator_cast(b,sll_static_int[SLL_OBJECT_TYPE_FLOAT]);
		}
	}
	else{
		b=SLL_ACQUIRE_STATIC(float_zero);
	}
	sll_float_t out=sll_api_math_pow(a->dt.f,b->dt.f);
	SLL_RELEASE(b);
	SLL_RELEASE(a);
	return SLL_FROM_FLOAT(out);
}



__SLL_API_TYPE_sll_api_math_round sll_api_math_round(__SLL_API_ARGS_sll_api_math_round);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_math_round_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_object_t* a=NULL;
	if (all>0){
		a=*(al+0);
		if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_FLOAT){
			SLL_ACQUIRE(a);
		}
		else{
			a=sll_operator_cast(a,sll_static_int[SLL_OBJECT_TYPE_FLOAT]);
		}
	}
	else{
		a=SLL_ACQUIRE_STATIC(float_zero);
	}
	sll_float_t out=sll_api_math_round(a->dt.f);
	SLL_RELEASE(a);
	return SLL_FROM_FLOAT(out);
}



__SLL_API_TYPE_sll_api_math_sin sll_api_math_sin(__SLL_API_ARGS_sll_api_math_sin);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_math_sin_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_object_t* a=NULL;
	if (all>0){
		a=*(al+0);
		if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_FLOAT){
			SLL_ACQUIRE(a);
		}
		else{
			a=sll_operator_cast(a,sll_static_int[SLL_OBJECT_TYPE_FLOAT]);
		}
	}
	else{
		a=SLL_ACQUIRE_STATIC(float_zero);
	}
	sll_float_t out=sll_api_math_sin(a->dt.f);
	SLL_RELEASE(a);
	return SLL_FROM_FLOAT(out);
}



__SLL_API_TYPE_sll_api_math_sinh sll_api_math_sinh(__SLL_API_ARGS_sll_api_math_sinh);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_math_sinh_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_object_t* a=NULL;
	if (all>0){
		a=*(al+0);
		if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_FLOAT){
			SLL_ACQUIRE(a);
		}
		else{
			a=sll_operator_cast(a,sll_static_int[SLL_OBJECT_TYPE_FLOAT]);
		}
	}
	else{
		a=SLL_ACQUIRE_STATIC(float_zero);
	}
	sll_float_t out=sll_api_math_sinh(a->dt.f);
	SLL_RELEASE(a);
	return SLL_FROM_FLOAT(out);
}



__SLL_API_TYPE_sll_api_math_sqrt sll_api_math_sqrt(__SLL_API_ARGS_sll_api_math_sqrt);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_math_sqrt_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_object_t* a=NULL;
	if (all>0){
		a=*(al+0);
		if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_FLOAT){
			SLL_ACQUIRE(a);
		}
		else{
			a=sll_operator_cast(a,sll_static_int[SLL_OBJECT_TYPE_FLOAT]);
		}
	}
	else{
		a=SLL_ACQUIRE_STATIC(float_zero);
	}
	sll_float_t out=sll_api_math_sqrt(a->dt.f);
	SLL_RELEASE(a);
	return SLL_FROM_FLOAT(out);
}



__SLL_API_TYPE_sll_api_math_tan sll_api_math_tan(__SLL_API_ARGS_sll_api_math_tan);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_math_tan_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_object_t* a=NULL;
	if (all>0){
		a=*(al+0);
		if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_FLOAT){
			SLL_ACQUIRE(a);
		}
		else{
			a=sll_operator_cast(a,sll_static_int[SLL_OBJECT_TYPE_FLOAT]);
		}
	}
	else{
		a=SLL_ACQUIRE_STATIC(float_zero);
	}
	sll_float_t out=sll_api_math_tan(a->dt.f);
	SLL_RELEASE(a);
	return SLL_FROM_FLOAT(out);
}



__SLL_API_TYPE_sll_api_math_tanh sll_api_math_tanh(__SLL_API_ARGS_sll_api_math_tanh);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_math_tanh_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_object_t* a=NULL;
	if (all>0){
		a=*(al+0);
		if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_FLOAT){
			SLL_ACQUIRE(a);
		}
		else{
			a=sll_operator_cast(a,sll_static_int[SLL_OBJECT_TYPE_FLOAT]);
		}
	}
	else{
		a=SLL_ACQUIRE_STATIC(float_zero);
	}
	sll_float_t out=sll_api_math_tanh(a->dt.f);
	SLL_RELEASE(a);
	return SLL_FROM_FLOAT(out);
}



__SLL_API_TYPE_sll_api_path_absolute sll_api_path_absolute(__SLL_API_ARGS_sll_api_path_absolute);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_path_absolute_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_object_t* a=NULL;
	if (all>0){
		a=*(al+0);
		if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_STRING){
			SLL_ACQUIRE(a);
		}
		else{
			a=sll_operator_cast(a,sll_static_int[SLL_OBJECT_TYPE_STRING]);
		}
	}
	else{
		a=SLL_CREATE();
		a->t=SLL_OBJECT_TYPE_STRING;
		sll_string_create(0,&(a->dt.s));
	}
	sll_string_t out;
	sll_api_path_absolute(&(a->dt.s),&out);
	SLL_RELEASE(a);
	sll_object_t* out_o=SLL_CREATE();
	out_o->t=SLL_OBJECT_TYPE_STRING;
	out_o->dt.s=out;
	return out_o;
}



__SLL_API_TYPE_sll_api_path_exists sll_api_path_exists(__SLL_API_ARGS_sll_api_path_exists);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_path_exists_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_object_t* a=NULL;
	if (all>0){
		a=*(al+0);
		if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_STRING){
			SLL_ACQUIRE(a);
		}
		else{
			a=sll_operator_cast(a,sll_static_int[SLL_OBJECT_TYPE_STRING]);
		}
	}
	else{
		a=SLL_CREATE();
		a->t=SLL_OBJECT_TYPE_STRING;
		sll_string_create(0,&(a->dt.s));
	}
	sll_integer_t out=sll_api_path_exists(&(a->dt.s));
	SLL_RELEASE(a);
	return SLL_FROM_INT(out);
}



__SLL_API_TYPE_sll_api_path_get_cwd sll_api_path_get_cwd(__SLL_API_ARGS_sll_api_path_get_cwd);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_path_get_cwd_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_string_t out;
	sll_api_path_get_cwd(&out);
	sll_object_t* out_o=SLL_CREATE();
	out_o->t=SLL_OBJECT_TYPE_STRING;
	out_o->dt.s=out;
	return out_o;
}



__SLL_API_TYPE_sll_api_path_is_dir sll_api_path_is_dir(__SLL_API_ARGS_sll_api_path_is_dir);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_path_is_dir_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_object_t* a=NULL;
	if (all>0){
		a=*(al+0);
		if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_STRING){
			SLL_ACQUIRE(a);
		}
		else{
			a=sll_operator_cast(a,sll_static_int[SLL_OBJECT_TYPE_STRING]);
		}
	}
	else{
		a=SLL_CREATE();
		a->t=SLL_OBJECT_TYPE_STRING;
		sll_string_create(0,&(a->dt.s));
	}
	sll_integer_t out=sll_api_path_is_dir(&(a->dt.s));
	SLL_RELEASE(a);
	return SLL_FROM_INT(out);
}



__SLL_API_TYPE_sll_api_path_join sll_api_path_join(__SLL_API_ARGS_sll_api_path_join);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_path_join_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_arg_count_t ac=all;
	sll_string_t** a=sll_allocate(ac*sizeof(sll_string_t*));
	for (sll_arg_count_t idx=0;idx<ac;idx++){
		sll_object_t* tmp=*(al+idx+0);
		if (!(SLL_OBJECT_GET_TYPE(tmp)==SLL_OBJECT_TYPE_STRING)){
			tmp=sll_operator_cast(tmp,sll_static_int[SLL_OBJECT_TYPE_STRING]);
			sll_string_clone(&(tmp->dt.s),*(a+idx));
			SLL_RELEASE(tmp);
		}
		else{
			*(a+idx)=&(tmp->dt.s);
		}
	}
	sll_string_t out;
	sll_api_path_join(a,ac,&out);
	sll_deallocate((void*)a);
	sll_object_t* out_o=SLL_CREATE();
	out_o->t=SLL_OBJECT_TYPE_STRING;
	out_o->dt.s=out;
	return out_o;
}



__SLL_API_TYPE_sll_api_path_list_dir sll_api_path_list_dir(__SLL_API_ARGS_sll_api_path_list_dir);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_path_list_dir_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_object_t* a=NULL;
	if (all>0){
		a=*(al+0);
		if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_STRING){
			SLL_ACQUIRE(a);
		}
		else{
			a=sll_operator_cast(a,sll_static_int[SLL_OBJECT_TYPE_STRING]);
		}
	}
	else{
		a=SLL_CREATE();
		a->t=SLL_OBJECT_TYPE_STRING;
		sll_string_create(0,&(a->dt.s));
	}
	sll_array_t out;
	sll_api_path_list_dir(&(a->dt.s),&out);
	SLL_RELEASE(a);
	sll_object_t* out_o=SLL_CREATE();
	out_o->t=SLL_OBJECT_TYPE_ARRAY;
	out_o->dt.a=out;
	return out_o;
}



__SLL_API_TYPE_sll_api_path_recursive_list_dir sll_api_path_recursive_list_dir(__SLL_API_ARGS_sll_api_path_recursive_list_dir);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_path_recursive_list_dir_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_object_t* a=NULL;
	if (all>0){
		a=*(al+0);
		if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_STRING){
			SLL_ACQUIRE(a);
		}
		else{
			a=sll_operator_cast(a,sll_static_int[SLL_OBJECT_TYPE_STRING]);
		}
	}
	else{
		a=SLL_CREATE();
		a->t=SLL_OBJECT_TYPE_STRING;
		sll_string_create(0,&(a->dt.s));
	}
	sll_array_t out;
	sll_api_path_recursive_list_dir(&(a->dt.s),&out);
	SLL_RELEASE(a);
	sll_object_t* out_o=SLL_CREATE();
	out_o->t=SLL_OBJECT_TYPE_ARRAY;
	out_o->dt.a=out;
	return out_o;
}



__SLL_API_TYPE_sll_api_path_relative sll_api_path_relative(__SLL_API_ARGS_sll_api_path_relative);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_path_relative_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_object_t* a=NULL;
	if (all>0){
		a=*(al+0);
		if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_STRING){
			SLL_ACQUIRE(a);
		}
		else{
			a=sll_operator_cast(a,sll_static_int[SLL_OBJECT_TYPE_STRING]);
		}
	}
	else{
		a=SLL_CREATE();
		a->t=SLL_OBJECT_TYPE_STRING;
		sll_string_create(0,&(a->dt.s));
	}
	sll_string_t out;
	sll_api_path_relative(&(a->dt.s),&out);
	SLL_RELEASE(a);
	sll_object_t* out_o=SLL_CREATE();
	out_o->t=SLL_OBJECT_TYPE_STRING;
	out_o->dt.s=out;
	return out_o;
}



__SLL_API_TYPE_sll_api_path_set_cwd sll_api_path_set_cwd(__SLL_API_ARGS_sll_api_path_set_cwd);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_path_set_cwd_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_object_t* a=NULL;
	if (all>0){
		a=*(al+0);
		if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_STRING){
			SLL_ACQUIRE(a);
		}
		else{
			a=sll_operator_cast(a,sll_static_int[SLL_OBJECT_TYPE_STRING]);
		}
	}
	else{
		a=SLL_CREATE();
		a->t=SLL_OBJECT_TYPE_STRING;
		sll_string_create(0,&(a->dt.s));
	}
	sll_integer_t out=sll_api_path_set_cwd(&(a->dt.s));
	SLL_RELEASE(a);
	return SLL_FROM_INT(out);
}



__SLL_API_TYPE_sll_api_path_size sll_api_path_size(__SLL_API_ARGS_sll_api_path_size);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_path_size_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_object_t* a=NULL;
	if (all>0){
		a=*(al+0);
		if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_STRING){
			SLL_ACQUIRE(a);
		}
		else{
			a=sll_operator_cast(a,sll_static_int[SLL_OBJECT_TYPE_STRING]);
		}
	}
	else{
		a=SLL_CREATE();
		a->t=SLL_OBJECT_TYPE_STRING;
		sll_string_create(0,&(a->dt.s));
	}
	sll_integer_t out=sll_api_path_size(&(a->dt.s));
	SLL_RELEASE(a);
	return SLL_FROM_INT(out);
}



__SLL_API_TYPE_sll_api_path_split sll_api_path_split(__SLL_API_ARGS_sll_api_path_split);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_path_split_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_object_t* a=NULL;
	if (all>0){
		a=*(al+0);
		if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_STRING){
			SLL_ACQUIRE(a);
		}
		else{
			a=sll_operator_cast(a,sll_static_int[SLL_OBJECT_TYPE_STRING]);
		}
	}
	else{
		a=SLL_CREATE();
		a->t=SLL_OBJECT_TYPE_STRING;
		sll_string_create(0,&(a->dt.s));
	}
	sll_array_t out;
	sll_api_path_split(&(a->dt.s),&out);
	SLL_RELEASE(a);
	sll_object_t* out_o=SLL_CREATE();
	out_o->t=SLL_OBJECT_TYPE_ARRAY;
	out_o->dt.a=out;
	return out_o;
}



__SLL_API_TYPE_sll_api_process__init sll_api_process__init(__SLL_API_ARGS_sll_api_process__init);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_process__init_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_object_t* a=NULL;
	if (all>0){
		a=*(al+0);
		if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_INT){
			SLL_ACQUIRE(a);
		}
		else{
			a=sll_operator_cast(a,sll_static_int[SLL_OBJECT_TYPE_INT]);
		}
	}
	else{
		a=SLL_ACQUIRE_STATIC_INT(0);
	}
	sll_api_process__init(a->dt.i);
	SLL_RELEASE(a);
	return SLL_ACQUIRE_STATIC_INT(0);
}



__SLL_API_TYPE_sll_api_process_join sll_api_process_join(__SLL_API_ARGS_sll_api_process_join);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_process_join_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_object_t* a=NULL;
	if (all>0){
		a=*(al+0);
		if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_ARRAY){
			SLL_ACQUIRE(a);
		}
		else{
			a=sll_operator_cast(a,sll_static_int[SLL_OBJECT_TYPE_ARRAY]);
		}
	}
	else{
		a=SLL_CREATE();
		a->t=SLL_OBJECT_TYPE_ARRAY;
		if (!sll_array_create(0,&(a->dt.a))){
			SLL_UNIMPLEMENTED();
		}
	}
	sll_string_t out;
	sll_api_process_join(&(a->dt.a),&out);
	SLL_RELEASE(a);
	sll_object_t* out_o=SLL_CREATE();
	out_o->t=SLL_OBJECT_TYPE_STRING;
	out_o->dt.s=out;
	return out_o;
}



__SLL_API_TYPE_sll_api_process_execute_shell sll_api_process_execute_shell(__SLL_API_ARGS_sll_api_process_execute_shell);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_process_execute_shell_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_object_t* a=NULL;
	if (all>0){
		a=*(al+0);
		if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_STRING){
			SLL_ACQUIRE(a);
		}
		else{
			a=sll_operator_cast(a,sll_static_int[SLL_OBJECT_TYPE_STRING]);
		}
	}
	else{
		a=SLL_CREATE();
		a->t=SLL_OBJECT_TYPE_STRING;
		sll_string_create(0,&(a->dt.s));
	}
	sll_bool_t out=sll_api_process_execute_shell(&(a->dt.s));
	SLL_RELEASE(a);
	SLL_ACQUIRE(sll_static_int[out]);
	return sll_static_int[out];
}



__SLL_API_TYPE_sll_api_process_get_pid sll_api_process_get_pid(__SLL_API_ARGS_sll_api_process_get_pid);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_process_get_pid_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_integer_t out=sll_api_process_get_pid();
	return SLL_FROM_INT(out);
}



__SLL_API_TYPE_sll_api_process_start sll_api_process_start(__SLL_API_ARGS_sll_api_process_start);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_process_start_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_object_t* a=NULL;
	if (all>0){
		a=*(al+0);
		if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_ARRAY){
			SLL_ACQUIRE(a);
		}
		else{
			a=sll_operator_cast(a,sll_static_int[SLL_OBJECT_TYPE_ARRAY]);
		}
	}
	else{
		a=SLL_CREATE();
		a->t=SLL_OBJECT_TYPE_ARRAY;
		if (!sll_array_create(0,&(a->dt.a))){
			SLL_UNIMPLEMENTED();
		}
	}
	sll_object_t* b=NULL;
	if (all>1){
		b=*(al+1);
		SLL_ACQUIRE(b);
	}
	else{
		b=SLL_ACQUIRE_STATIC_INT(0);
	}
	sll_object_t* c=NULL;
	if (all>2){
		c=*(al+2);
		if (SLL_OBJECT_GET_TYPE(c)==SLL_OBJECT_TYPE_STRING){
			SLL_ACQUIRE(c);
		}
		else{
			c=sll_operator_cast(c,sll_static_int[SLL_OBJECT_TYPE_STRING]);
		}
	}
	else{
		c=SLL_CREATE();
		c->t=SLL_OBJECT_TYPE_STRING;
		sll_string_create(0,&(c->dt.s));
	}
	sll_object_t* out=sll_api_process_start(&(a->dt.a),b,&(c->dt.s));
	SLL_RELEASE(c);
	SLL_RELEASE(b);
	SLL_RELEASE(a);
	return out;
}



__SLL_API_TYPE_sll_api_process_split sll_api_process_split(__SLL_API_ARGS_sll_api_process_split);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_process_split_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_object_t* a=NULL;
	if (all>0){
		a=*(al+0);
		if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_STRING){
			SLL_ACQUIRE(a);
		}
		else{
			a=sll_operator_cast(a,sll_static_int[SLL_OBJECT_TYPE_STRING]);
		}
	}
	else{
		a=SLL_CREATE();
		a->t=SLL_OBJECT_TYPE_STRING;
		sll_string_create(0,&(a->dt.s));
	}
	sll_array_t out;
	sll_api_process_split(&(a->dt.s),&out);
	SLL_RELEASE(a);
	sll_object_t* out_o=SLL_CREATE();
	out_o->t=SLL_OBJECT_TYPE_ARRAY;
	out_o->dt.a=out;
	return out_o;
}



__SLL_API_TYPE_sll_api_sort_quicksort sll_api_sort_quicksort(__SLL_API_ARGS_sll_api_sort_quicksort);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_sort_quicksort_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_object_t* a=NULL;
	if (all>0){
		a=*(al+0);
		if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_ARRAY){
			SLL_ACQUIRE(a);
		}
		else{
			a=sll_operator_cast(a,sll_static_int[SLL_OBJECT_TYPE_ARRAY]);
		}
	}
	else{
		a=SLL_CREATE();
		a->t=SLL_OBJECT_TYPE_ARRAY;
		if (!sll_array_create(0,&(a->dt.a))){
			SLL_UNIMPLEMENTED();
		}
	}
	sll_object_t* b=NULL;
	if (all>1){
		b=*(al+1);
		if (SLL_OBJECT_GET_TYPE(b)==SLL_OBJECT_TYPE_INT){
			SLL_ACQUIRE(b);
		}
		else{
			b=sll_operator_cast(b,sll_static_int[SLL_OBJECT_TYPE_INT]);
		}
	}
	else{
		b=SLL_ACQUIRE_STATIC_INT(0);
	}
	sll_array_t out;
	sll_api_sort_quicksort(&(a->dt.a),b->dt.i,&out);
	SLL_RELEASE(b);
	SLL_RELEASE(a);
	sll_object_t* out_o=SLL_CREATE();
	out_o->t=SLL_OBJECT_TYPE_ARRAY;
	out_o->dt.a=out;
	return out_o;
}



__SLL_API_TYPE_sll_api_string_checksum sll_api_string_checksum(__SLL_API_ARGS_sll_api_string_checksum);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_string_checksum_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_object_t* a=NULL;
	if (all>0){
		a=*(al+0);
		if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_STRING){
			SLL_ACQUIRE(a);
		}
		else{
			a=sll_operator_cast(a,sll_static_int[SLL_OBJECT_TYPE_STRING]);
		}
	}
	else{
		a=SLL_CREATE();
		a->t=SLL_OBJECT_TYPE_STRING;
		sll_string_create(0,&(a->dt.s));
	}
	sll_integer_t out=sll_api_string_checksum(&(a->dt.s));
	SLL_RELEASE(a);
	return SLL_FROM_INT(out);
}



__SLL_API_TYPE_sll_api_string_convert sll_api_string_convert(__SLL_API_ARGS_sll_api_string_convert);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_string_convert_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_object_t*const* a=al+0;
	sll_arg_count_t ac=all;
	sll_string_t out;
	sll_api_string_convert(a,ac,&out);
	sll_object_t* out_o=SLL_CREATE();
	out_o->t=SLL_OBJECT_TYPE_STRING;
	out_o->dt.s=out;
	return out_o;
}



__SLL_API_TYPE_sll_api_string_count sll_api_string_count(__SLL_API_ARGS_sll_api_string_count);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_string_count_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_object_t* a=NULL;
	if (all>0){
		a=*(al+0);
		if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_STRING){
			SLL_ACQUIRE(a);
		}
		else{
			a=sll_operator_cast(a,sll_static_int[SLL_OBJECT_TYPE_STRING]);
		}
	}
	else{
		a=SLL_CREATE();
		a->t=SLL_OBJECT_TYPE_STRING;
		sll_string_create(0,&(a->dt.s));
	}
	sll_object_t* b=NULL;
	if (all>1){
		b=*(al+1);
		if (SLL_OBJECT_GET_TYPE(b)==SLL_OBJECT_TYPE_STRING||SLL_OBJECT_GET_TYPE(b)==SLL_OBJECT_TYPE_CHAR){
			SLL_ACQUIRE(b);
		}
		else{
			b=sll_operator_cast(b,sll_static_int[SLL_OBJECT_TYPE_STRING]);
		}
	}
	else{
		b=SLL_CREATE();
		b->t=SLL_OBJECT_TYPE_STRING;
		sll_string_create(0,&(b->dt.s));
	}
	sll_integer_t out=sll_api_string_count(&(a->dt.s),b);
	SLL_RELEASE(b);
	SLL_RELEASE(a);
	return SLL_FROM_INT(out);
}



__SLL_API_TYPE_sll_api_string_count_left sll_api_string_count_left(__SLL_API_ARGS_sll_api_string_count_left);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_string_count_left_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_object_t* a=NULL;
	if (all>0){
		a=*(al+0);
		if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_STRING){
			SLL_ACQUIRE(a);
		}
		else{
			a=sll_operator_cast(a,sll_static_int[SLL_OBJECT_TYPE_STRING]);
		}
	}
	else{
		a=SLL_CREATE();
		a->t=SLL_OBJECT_TYPE_STRING;
		sll_string_create(0,&(a->dt.s));
	}
	sll_object_t* b=NULL;
	if (all>1){
		b=*(al+1);
		if (SLL_OBJECT_GET_TYPE(b)==SLL_OBJECT_TYPE_CHAR){
			SLL_ACQUIRE(b);
		}
		else{
			b=sll_operator_cast(b,sll_static_int[SLL_OBJECT_TYPE_CHAR]);
		}
	}
	else{
		b=SLL_ACQUIRE_STATIC_CHAR(0);
	}
	sll_integer_t out=sll_api_string_count_left(&(a->dt.s),b->dt.c);
	SLL_RELEASE(b);
	SLL_RELEASE(a);
	return SLL_FROM_INT(out);
}



__SLL_API_TYPE_sll_api_string_count_right sll_api_string_count_right(__SLL_API_ARGS_sll_api_string_count_right);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_string_count_right_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_object_t* a=NULL;
	if (all>0){
		a=*(al+0);
		if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_STRING){
			SLL_ACQUIRE(a);
		}
		else{
			a=sll_operator_cast(a,sll_static_int[SLL_OBJECT_TYPE_STRING]);
		}
	}
	else{
		a=SLL_CREATE();
		a->t=SLL_OBJECT_TYPE_STRING;
		sll_string_create(0,&(a->dt.s));
	}
	sll_object_t* b=NULL;
	if (all>1){
		b=*(al+1);
		if (SLL_OBJECT_GET_TYPE(b)==SLL_OBJECT_TYPE_CHAR){
			SLL_ACQUIRE(b);
		}
		else{
			b=sll_operator_cast(b,sll_static_int[SLL_OBJECT_TYPE_CHAR]);
		}
	}
	else{
		b=SLL_ACQUIRE_STATIC_CHAR(0);
	}
	sll_integer_t out=sll_api_string_count_right(&(a->dt.s),b->dt.c);
	SLL_RELEASE(b);
	SLL_RELEASE(a);
	return SLL_FROM_INT(out);
}



__SLL_API_TYPE_sll_api_string_ends sll_api_string_ends(__SLL_API_ARGS_sll_api_string_ends);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_string_ends_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_object_t* a=NULL;
	if (all>0){
		a=*(al+0);
		if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_STRING){
			SLL_ACQUIRE(a);
		}
		else{
			a=sll_operator_cast(a,sll_static_int[SLL_OBJECT_TYPE_STRING]);
		}
	}
	else{
		a=SLL_CREATE();
		a->t=SLL_OBJECT_TYPE_STRING;
		sll_string_create(0,&(a->dt.s));
	}
	sll_object_t* b=NULL;
	if (all>1){
		b=*(al+1);
		if (SLL_OBJECT_GET_TYPE(b)==SLL_OBJECT_TYPE_STRING||SLL_OBJECT_GET_TYPE(b)==SLL_OBJECT_TYPE_CHAR){
			SLL_ACQUIRE(b);
		}
		else{
			b=sll_operator_cast(b,sll_static_int[SLL_OBJECT_TYPE_STRING]);
		}
	}
	else{
		b=SLL_CREATE();
		b->t=SLL_OBJECT_TYPE_STRING;
		sll_string_create(0,&(b->dt.s));
	}
	sll_bool_t out=sll_api_string_ends(&(a->dt.s),b);
	SLL_RELEASE(b);
	SLL_RELEASE(a);
	SLL_ACQUIRE(sll_static_int[out]);
	return sll_static_int[out];
}



__SLL_API_TYPE_sll_api_string_format sll_api_string_format(__SLL_API_ARGS_sll_api_string_format);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_string_format_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_object_t* a=NULL;
	if (all>0){
		a=*(al+0);
		if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_STRING){
			SLL_ACQUIRE(a);
		}
		else{
			a=sll_operator_cast(a,sll_static_int[SLL_OBJECT_TYPE_STRING]);
		}
	}
	else{
		a=SLL_CREATE();
		a->t=SLL_OBJECT_TYPE_STRING;
		sll_string_create(0,&(a->dt.s));
	}
	sll_object_t*const* b=al+1;
	sll_arg_count_t bc=(all>1?all-1:0);
	sll_string_t out;
	sll_api_string_format(&(a->dt.s),b,bc,&out);
	SLL_RELEASE(a);
	sll_object_t* out_o=SLL_CREATE();
	out_o->t=SLL_OBJECT_TYPE_STRING;
	out_o->dt.s=out;
	return out_o;
}



__SLL_API_TYPE_sll_api_string_flip_case sll_api_string_flip_case(__SLL_API_ARGS_sll_api_string_flip_case);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_string_flip_case_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_object_t* a=NULL;
	if (all>0){
		a=*(al+0);
		if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_STRING){
			SLL_ACQUIRE(a);
		}
		else{
			a=sll_operator_cast(a,sll_static_int[SLL_OBJECT_TYPE_STRING]);
		}
	}
	else{
		a=SLL_CREATE();
		a->t=SLL_OBJECT_TYPE_STRING;
		sll_string_create(0,&(a->dt.s));
	}
	sll_string_t out;
	sll_api_string_flip_case(&(a->dt.s),&out);
	SLL_RELEASE(a);
	sll_object_t* out_o=SLL_CREATE();
	out_o->t=SLL_OBJECT_TYPE_STRING;
	out_o->dt.s=out;
	return out_o;
}



__SLL_API_TYPE_sll_api_string_index sll_api_string_index(__SLL_API_ARGS_sll_api_string_index);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_string_index_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_object_t* a=NULL;
	if (all>0){
		a=*(al+0);
		if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_STRING){
			SLL_ACQUIRE(a);
		}
		else{
			a=sll_operator_cast(a,sll_static_int[SLL_OBJECT_TYPE_STRING]);
		}
	}
	else{
		a=SLL_CREATE();
		a->t=SLL_OBJECT_TYPE_STRING;
		sll_string_create(0,&(a->dt.s));
	}
	sll_object_t* b=NULL;
	if (all>1){
		b=*(al+1);
		if (SLL_OBJECT_GET_TYPE(b)==SLL_OBJECT_TYPE_STRING||SLL_OBJECT_GET_TYPE(b)==SLL_OBJECT_TYPE_CHAR){
			SLL_ACQUIRE(b);
		}
		else{
			b=sll_operator_cast(b,sll_static_int[SLL_OBJECT_TYPE_STRING]);
		}
	}
	else{
		b=SLL_CREATE();
		b->t=SLL_OBJECT_TYPE_STRING;
		sll_string_create(0,&(b->dt.s));
	}
	sll_integer_t out=sll_api_string_index(&(a->dt.s),b);
	SLL_RELEASE(b);
	SLL_RELEASE(a);
	return SLL_FROM_INT(out);
}



__SLL_API_TYPE_sll_api_string_index_list sll_api_string_index_list(__SLL_API_ARGS_sll_api_string_index_list);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_string_index_list_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_object_t* a=NULL;
	if (all>0){
		a=*(al+0);
		if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_STRING){
			SLL_ACQUIRE(a);
		}
		else{
			a=sll_operator_cast(a,sll_static_int[SLL_OBJECT_TYPE_STRING]);
		}
	}
	else{
		a=SLL_CREATE();
		a->t=SLL_OBJECT_TYPE_STRING;
		sll_string_create(0,&(a->dt.s));
	}
	sll_object_t* b=NULL;
	if (all>1){
		b=*(al+1);
		if (SLL_OBJECT_GET_TYPE(b)==SLL_OBJECT_TYPE_STRING||SLL_OBJECT_GET_TYPE(b)==SLL_OBJECT_TYPE_CHAR){
			SLL_ACQUIRE(b);
		}
		else{
			b=sll_operator_cast(b,sll_static_int[SLL_OBJECT_TYPE_STRING]);
		}
	}
	else{
		b=SLL_CREATE();
		b->t=SLL_OBJECT_TYPE_STRING;
		sll_string_create(0,&(b->dt.s));
	}
	sll_integer_t out=sll_api_string_index_list(&(a->dt.s),b);
	SLL_RELEASE(b);
	SLL_RELEASE(a);
	return SLL_FROM_INT(out);
}



__SLL_API_TYPE_sll_api_string_index_reverse sll_api_string_index_reverse(__SLL_API_ARGS_sll_api_string_index_reverse);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_string_index_reverse_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_object_t* a=NULL;
	if (all>0){
		a=*(al+0);
		if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_STRING){
			SLL_ACQUIRE(a);
		}
		else{
			a=sll_operator_cast(a,sll_static_int[SLL_OBJECT_TYPE_STRING]);
		}
	}
	else{
		a=SLL_CREATE();
		a->t=SLL_OBJECT_TYPE_STRING;
		sll_string_create(0,&(a->dt.s));
	}
	sll_object_t* b=NULL;
	if (all>1){
		b=*(al+1);
		if (SLL_OBJECT_GET_TYPE(b)==SLL_OBJECT_TYPE_STRING||SLL_OBJECT_GET_TYPE(b)==SLL_OBJECT_TYPE_CHAR){
			SLL_ACQUIRE(b);
		}
		else{
			b=sll_operator_cast(b,sll_static_int[SLL_OBJECT_TYPE_STRING]);
		}
	}
	else{
		b=SLL_CREATE();
		b->t=SLL_OBJECT_TYPE_STRING;
		sll_string_create(0,&(b->dt.s));
	}
	sll_integer_t out=sll_api_string_index_reverse(&(a->dt.s),b);
	SLL_RELEASE(b);
	SLL_RELEASE(a);
	return SLL_FROM_INT(out);
}



__SLL_API_TYPE_sll_api_string_index_reverse_list sll_api_string_index_reverse_list(__SLL_API_ARGS_sll_api_string_index_reverse_list);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_string_index_reverse_list_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_object_t* a=NULL;
	if (all>0){
		a=*(al+0);
		if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_STRING){
			SLL_ACQUIRE(a);
		}
		else{
			a=sll_operator_cast(a,sll_static_int[SLL_OBJECT_TYPE_STRING]);
		}
	}
	else{
		a=SLL_CREATE();
		a->t=SLL_OBJECT_TYPE_STRING;
		sll_string_create(0,&(a->dt.s));
	}
	sll_object_t* b=NULL;
	if (all>1){
		b=*(al+1);
		if (SLL_OBJECT_GET_TYPE(b)==SLL_OBJECT_TYPE_STRING||SLL_OBJECT_GET_TYPE(b)==SLL_OBJECT_TYPE_CHAR){
			SLL_ACQUIRE(b);
		}
		else{
			b=sll_operator_cast(b,sll_static_int[SLL_OBJECT_TYPE_STRING]);
		}
	}
	else{
		b=SLL_CREATE();
		b->t=SLL_OBJECT_TYPE_STRING;
		sll_string_create(0,&(b->dt.s));
	}
	sll_integer_t out=sll_api_string_index_reverse_list(&(a->dt.s),b);
	SLL_RELEASE(b);
	SLL_RELEASE(a);
	return SLL_FROM_INT(out);
}



__SLL_API_TYPE_sll_api_string_join sll_api_string_join(__SLL_API_ARGS_sll_api_string_join);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_string_join_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_object_t* a=NULL;
	if (all>0){
		a=*(al+0);
		if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_STRING||SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_CHAR){
			SLL_ACQUIRE(a);
		}
		else{
			a=sll_operator_cast(a,sll_static_int[SLL_OBJECT_TYPE_STRING]);
		}
	}
	else{
		a=SLL_CREATE();
		a->t=SLL_OBJECT_TYPE_STRING;
		sll_string_create(0,&(a->dt.s));
	}
	sll_object_t* b=NULL;
	if (all>1){
		b=*(al+1);
		if (SLL_OBJECT_GET_TYPE(b)==SLL_OBJECT_TYPE_ARRAY){
			SLL_ACQUIRE(b);
		}
		else{
			b=sll_operator_cast(b,sll_static_int[SLL_OBJECT_TYPE_ARRAY]);
		}
	}
	else{
		b=SLL_CREATE();
		b->t=SLL_OBJECT_TYPE_ARRAY;
		if (!sll_array_create(0,&(b->dt.a))){
			SLL_UNIMPLEMENTED();
		}
	}
	sll_string_t out;
	sll_api_string_join(a,&(b->dt.a),&out);
	SLL_RELEASE(b);
	SLL_RELEASE(a);
	sll_object_t* out_o=SLL_CREATE();
	out_o->t=SLL_OBJECT_TYPE_STRING;
	out_o->dt.s=out;
	return out_o;
}



__SLL_API_TYPE_sll_api_string_pad_left sll_api_string_pad_left(__SLL_API_ARGS_sll_api_string_pad_left);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_string_pad_left_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_object_t* a=NULL;
	if (all>0){
		a=*(al+0);
		if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_STRING){
			SLL_ACQUIRE(a);
		}
		else{
			a=sll_operator_cast(a,sll_static_int[SLL_OBJECT_TYPE_STRING]);
		}
	}
	else{
		a=SLL_CREATE();
		a->t=SLL_OBJECT_TYPE_STRING;
		sll_string_create(0,&(a->dt.s));
	}
	sll_object_t* b=NULL;
	if (all>1){
		b=*(al+1);
		if (SLL_OBJECT_GET_TYPE(b)==SLL_OBJECT_TYPE_INT){
			SLL_ACQUIRE(b);
		}
		else{
			b=sll_operator_cast(b,sll_static_int[SLL_OBJECT_TYPE_INT]);
		}
	}
	else{
		b=SLL_ACQUIRE_STATIC_INT(0);
	}
	sll_object_t* c=NULL;
	if (all>2){
		c=*(al+2);
		if (SLL_OBJECT_GET_TYPE(c)==SLL_OBJECT_TYPE_CHAR){
			SLL_ACQUIRE(c);
		}
		else{
			c=sll_operator_cast(c,sll_static_int[SLL_OBJECT_TYPE_CHAR]);
		}
	}
	else{
		c=SLL_ACQUIRE_STATIC_CHAR(0);
	}
	sll_string_t out;
	sll_api_string_pad_left(&(a->dt.s),b->dt.i,c->dt.c,&out);
	SLL_RELEASE(c);
	SLL_RELEASE(b);
	SLL_RELEASE(a);
	sll_object_t* out_o=SLL_CREATE();
	out_o->t=SLL_OBJECT_TYPE_STRING;
	out_o->dt.s=out;
	return out_o;
}



__SLL_API_TYPE_sll_api_string_pad_right sll_api_string_pad_right(__SLL_API_ARGS_sll_api_string_pad_right);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_string_pad_right_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_object_t* a=NULL;
	if (all>0){
		a=*(al+0);
		if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_STRING){
			SLL_ACQUIRE(a);
		}
		else{
			a=sll_operator_cast(a,sll_static_int[SLL_OBJECT_TYPE_STRING]);
		}
	}
	else{
		a=SLL_CREATE();
		a->t=SLL_OBJECT_TYPE_STRING;
		sll_string_create(0,&(a->dt.s));
	}
	sll_object_t* b=NULL;
	if (all>1){
		b=*(al+1);
		if (SLL_OBJECT_GET_TYPE(b)==SLL_OBJECT_TYPE_INT){
			SLL_ACQUIRE(b);
		}
		else{
			b=sll_operator_cast(b,sll_static_int[SLL_OBJECT_TYPE_INT]);
		}
	}
	else{
		b=SLL_ACQUIRE_STATIC_INT(0);
	}
	sll_object_t* c=NULL;
	if (all>2){
		c=*(al+2);
		if (SLL_OBJECT_GET_TYPE(c)==SLL_OBJECT_TYPE_CHAR){
			SLL_ACQUIRE(c);
		}
		else{
			c=sll_operator_cast(c,sll_static_int[SLL_OBJECT_TYPE_CHAR]);
		}
	}
	else{
		c=SLL_ACQUIRE_STATIC_CHAR(0);
	}
	sll_string_t out;
	sll_api_string_pad_right(&(a->dt.s),b->dt.i,c->dt.c,&out);
	SLL_RELEASE(c);
	SLL_RELEASE(b);
	SLL_RELEASE(a);
	sll_object_t* out_o=SLL_CREATE();
	out_o->t=SLL_OBJECT_TYPE_STRING;
	out_o->dt.s=out;
	return out_o;
}



__SLL_API_TYPE_sll_api_string_replace sll_api_string_replace(__SLL_API_ARGS_sll_api_string_replace);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_string_replace_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_object_t* a=NULL;
	if (all>0){
		a=*(al+0);
		if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_STRING){
			SLL_ACQUIRE(a);
		}
		else{
			a=sll_operator_cast(a,sll_static_int[SLL_OBJECT_TYPE_STRING]);
		}
	}
	else{
		a=SLL_CREATE();
		a->t=SLL_OBJECT_TYPE_STRING;
		sll_string_create(0,&(a->dt.s));
	}
	sll_object_t* b=NULL;
	if (all>1){
		b=*(al+1);
		if (SLL_OBJECT_GET_TYPE(b)==SLL_OBJECT_TYPE_STRING||SLL_OBJECT_GET_TYPE(b)==SLL_OBJECT_TYPE_CHAR){
			SLL_ACQUIRE(b);
		}
		else{
			b=sll_operator_cast(b,sll_static_int[SLL_OBJECT_TYPE_STRING]);
		}
	}
	else{
		b=SLL_CREATE();
		b->t=SLL_OBJECT_TYPE_STRING;
		sll_string_create(0,&(b->dt.s));
	}
	sll_object_t* c=NULL;
	if (all>2){
		c=*(al+2);
		if (SLL_OBJECT_GET_TYPE(c)==SLL_OBJECT_TYPE_STRING||SLL_OBJECT_GET_TYPE(c)==SLL_OBJECT_TYPE_CHAR){
			SLL_ACQUIRE(c);
		}
		else{
			c=sll_operator_cast(c,sll_static_int[SLL_OBJECT_TYPE_STRING]);
		}
	}
	else{
		c=SLL_CREATE();
		c->t=SLL_OBJECT_TYPE_STRING;
		sll_string_create(0,&(c->dt.s));
	}
	sll_string_t out;
	sll_api_string_replace(&(a->dt.s),b,c,&out);
	SLL_RELEASE(c);
	SLL_RELEASE(b);
	SLL_RELEASE(a);
	sll_object_t* out_o=SLL_CREATE();
	out_o->t=SLL_OBJECT_TYPE_STRING;
	out_o->dt.s=out;
	return out_o;
}



__SLL_API_TYPE_sll_api_string_reverse sll_api_string_reverse(__SLL_API_ARGS_sll_api_string_reverse);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_string_reverse_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_object_t* a=NULL;
	if (all>0){
		a=*(al+0);
		if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_STRING){
			SLL_ACQUIRE(a);
		}
		else{
			a=sll_operator_cast(a,sll_static_int[SLL_OBJECT_TYPE_STRING]);
		}
	}
	else{
		a=SLL_CREATE();
		a->t=SLL_OBJECT_TYPE_STRING;
		sll_string_create(0,&(a->dt.s));
	}
	sll_string_t out;
	sll_api_string_reverse(&(a->dt.s),&out);
	SLL_RELEASE(a);
	sll_object_t* out_o=SLL_CREATE();
	out_o->t=SLL_OBJECT_TYPE_STRING;
	out_o->dt.s=out;
	return out_o;
}



__SLL_API_TYPE_sll_api_string_split sll_api_string_split(__SLL_API_ARGS_sll_api_string_split);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_string_split_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_object_t* a=NULL;
	if (all>0){
		a=*(al+0);
		if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_STRING){
			SLL_ACQUIRE(a);
		}
		else{
			a=sll_operator_cast(a,sll_static_int[SLL_OBJECT_TYPE_STRING]);
		}
	}
	else{
		a=SLL_CREATE();
		a->t=SLL_OBJECT_TYPE_STRING;
		sll_string_create(0,&(a->dt.s));
	}
	sll_object_t* b=NULL;
	if (all>1){
		b=*(al+1);
		if (SLL_OBJECT_GET_TYPE(b)==SLL_OBJECT_TYPE_STRING||SLL_OBJECT_GET_TYPE(b)==SLL_OBJECT_TYPE_CHAR){
			SLL_ACQUIRE(b);
		}
		else{
			b=sll_operator_cast(b,sll_static_int[SLL_OBJECT_TYPE_STRING]);
		}
	}
	else{
		b=SLL_CREATE();
		b->t=SLL_OBJECT_TYPE_STRING;
		sll_string_create(0,&(b->dt.s));
	}
	sll_array_t out;
	sll_api_string_split(&(a->dt.s),b,&out);
	SLL_RELEASE(b);
	SLL_RELEASE(a);
	sll_object_t* out_o=SLL_CREATE();
	out_o->t=SLL_OBJECT_TYPE_ARRAY;
	out_o->dt.a=out;
	return out_o;
}



__SLL_API_TYPE_sll_api_string_starts sll_api_string_starts(__SLL_API_ARGS_sll_api_string_starts);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_string_starts_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_object_t* a=NULL;
	if (all>0){
		a=*(al+0);
		if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_STRING){
			SLL_ACQUIRE(a);
		}
		else{
			a=sll_operator_cast(a,sll_static_int[SLL_OBJECT_TYPE_STRING]);
		}
	}
	else{
		a=SLL_CREATE();
		a->t=SLL_OBJECT_TYPE_STRING;
		sll_string_create(0,&(a->dt.s));
	}
	sll_object_t* b=NULL;
	if (all>1){
		b=*(al+1);
		if (SLL_OBJECT_GET_TYPE(b)==SLL_OBJECT_TYPE_STRING||SLL_OBJECT_GET_TYPE(b)==SLL_OBJECT_TYPE_CHAR){
			SLL_ACQUIRE(b);
		}
		else{
			b=sll_operator_cast(b,sll_static_int[SLL_OBJECT_TYPE_STRING]);
		}
	}
	else{
		b=SLL_CREATE();
		b->t=SLL_OBJECT_TYPE_STRING;
		sll_string_create(0,&(b->dt.s));
	}
	sll_bool_t out=sll_api_string_starts(&(a->dt.s),b);
	SLL_RELEASE(b);
	SLL_RELEASE(a);
	SLL_ACQUIRE(sll_static_int[out]);
	return sll_static_int[out];
}



__SLL_API_TYPE_sll_api_string_to_lower_case sll_api_string_to_lower_case(__SLL_API_ARGS_sll_api_string_to_lower_case);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_string_to_lower_case_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_object_t* a=NULL;
	if (all>0){
		a=*(al+0);
		if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_STRING){
			SLL_ACQUIRE(a);
		}
		else{
			a=sll_operator_cast(a,sll_static_int[SLL_OBJECT_TYPE_STRING]);
		}
	}
	else{
		a=SLL_CREATE();
		a->t=SLL_OBJECT_TYPE_STRING;
		sll_string_create(0,&(a->dt.s));
	}
	sll_string_t out;
	sll_api_string_to_lower_case(&(a->dt.s),&out);
	SLL_RELEASE(a);
	sll_object_t* out_o=SLL_CREATE();
	out_o->t=SLL_OBJECT_TYPE_STRING;
	out_o->dt.s=out;
	return out_o;
}



__SLL_API_TYPE_sll_api_string_to_title_case sll_api_string_to_title_case(__SLL_API_ARGS_sll_api_string_to_title_case);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_string_to_title_case_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_object_t* a=NULL;
	if (all>0){
		a=*(al+0);
		if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_STRING){
			SLL_ACQUIRE(a);
		}
		else{
			a=sll_operator_cast(a,sll_static_int[SLL_OBJECT_TYPE_STRING]);
		}
	}
	else{
		a=SLL_CREATE();
		a->t=SLL_OBJECT_TYPE_STRING;
		sll_string_create(0,&(a->dt.s));
	}
	sll_string_t out;
	sll_api_string_to_title_case(&(a->dt.s),&out);
	SLL_RELEASE(a);
	sll_object_t* out_o=SLL_CREATE();
	out_o->t=SLL_OBJECT_TYPE_STRING;
	out_o->dt.s=out;
	return out_o;
}



__SLL_API_TYPE_sll_api_string_to_upper_case sll_api_string_to_upper_case(__SLL_API_ARGS_sll_api_string_to_upper_case);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_string_to_upper_case_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_object_t* a=NULL;
	if (all>0){
		a=*(al+0);
		if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_STRING){
			SLL_ACQUIRE(a);
		}
		else{
			a=sll_operator_cast(a,sll_static_int[SLL_OBJECT_TYPE_STRING]);
		}
	}
	else{
		a=SLL_CREATE();
		a->t=SLL_OBJECT_TYPE_STRING;
		sll_string_create(0,&(a->dt.s));
	}
	sll_string_t out;
	sll_api_string_to_upper_case(&(a->dt.s),&out);
	SLL_RELEASE(a);
	sll_object_t* out_o=SLL_CREATE();
	out_o->t=SLL_OBJECT_TYPE_STRING;
	out_o->dt.s=out;
	return out_o;
}



__SLL_API_TYPE_sll_api_sys_get_args sll_api_sys_get_args(__SLL_API_ARGS_sll_api_sys_get_args);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_sys_get_args_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_array_t out;
	sll_api_sys_get_args(&out);
	sll_object_t* out_o=SLL_CREATE();
	out_o->t=SLL_OBJECT_TYPE_ARRAY;
	out_o->dt.a=out;
	return out_o;
}



__SLL_API_TYPE_sll_api_sys_get_executable sll_api_sys_get_executable(__SLL_API_ARGS_sll_api_sys_get_executable);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_sys_get_executable_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_string_t out;
	sll_api_sys_get_executable(&out);
	sll_object_t* out_o=SLL_CREATE();
	out_o->t=SLL_OBJECT_TYPE_STRING;
	out_o->dt.s=out;
	return out_o;
}



__SLL_API_TYPE_sll_api_sys_get_library sll_api_sys_get_library(__SLL_API_ARGS_sll_api_sys_get_library);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_sys_get_library_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_string_t out;
	sll_api_sys_get_library(&out);
	sll_object_t* out_o=SLL_CREATE();
	out_o->t=SLL_OBJECT_TYPE_STRING;
	out_o->dt.s=out;
	return out_o;
}



__SLL_API_TYPE_sll_api_sys_get_platform sll_api_sys_get_platform(__SLL_API_ARGS_sll_api_sys_get_platform);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_sys_get_platform_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_string_t out;
	sll_api_sys_get_platform(&out);
	sll_object_t* out_o=SLL_CREATE();
	out_o->t=SLL_OBJECT_TYPE_STRING;
	out_o->dt.s=out;
	return out_o;
}



__SLL_API_TYPE_sll_api_sys_get_version sll_api_sys_get_version(__SLL_API_ARGS_sll_api_sys_get_version);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_sys_get_version_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_object_t* a=NULL;
	if (all>0){
		a=*(al+0);
		if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_INT){
			SLL_ACQUIRE(a);
		}
		else{
			a=sll_operator_cast(a,sll_static_int[SLL_OBJECT_TYPE_INT]);
		}
	}
	else{
		a=SLL_ACQUIRE_STATIC_INT(0);
	}
	sll_object_t* out=sll_api_sys_get_version(a->dt.i);
	SLL_RELEASE(a);
	return out;
}



__SLL_API_TYPE_sll_api_sys_load_library sll_api_sys_load_library(__SLL_API_ARGS_sll_api_sys_load_library);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_sys_load_library_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_object_t* a=NULL;
	if (all>0){
		a=*(al+0);
		if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_STRING){
			SLL_ACQUIRE(a);
		}
		else{
			a=sll_operator_cast(a,sll_static_int[SLL_OBJECT_TYPE_STRING]);
		}
	}
	else{
		a=SLL_CREATE();
		a->t=SLL_OBJECT_TYPE_STRING;
		sll_string_create(0,&(a->dt.s));
	}
	sll_bool_t out=sll_api_sys_load_library(&(a->dt.s));
	SLL_RELEASE(a);
	SLL_ACQUIRE(sll_static_int[out]);
	return sll_static_int[out];
}



__SLL_API_TYPE_sll_api_time_current sll_api_time_current(__SLL_API_ARGS_sll_api_time_current);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_time_current_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_float_t out=sll_api_time_current();
	return SLL_FROM_FLOAT(out);
}



__SLL_API_TYPE_sll_api_time_current_nanos sll_api_time_current_nanos(__SLL_API_ARGS_sll_api_time_current_nanos);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_time_current_nanos_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_integer_t out=sll_api_time_current_nanos();
	return SLL_FROM_INT(out);
}



__SLL_API_TYPE_sll_api_time_sleep sll_api_time_sleep(__SLL_API_ARGS_sll_api_time_sleep);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_time_sleep_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_object_t* a=NULL;
	if (all>0){
		a=*(al+0);
		if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_FLOAT||SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_INT){
			SLL_ACQUIRE(a);
		}
		else{
			a=sll_operator_cast(a,sll_static_int[SLL_OBJECT_TYPE_FLOAT]);
		}
	}
	else{
		a=SLL_ACQUIRE_STATIC(float_zero);
	}
	sll_float_t out=sll_api_time_sleep(a);
	SLL_RELEASE(a);
	return SLL_FROM_FLOAT(out);
}



__SLL_API_TYPE_sll_api_time_sleep_nanos sll_api_time_sleep_nanos(__SLL_API_ARGS_sll_api_time_sleep_nanos);
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_time_sleep_nanos_raw(sll_object_t*const* al,sll_arg_count_t all){
	sll_object_t* a=NULL;
	if (all>0){
		a=*(al+0);
		if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_INT||SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_FLOAT||SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_INT){
			SLL_ACQUIRE(a);
		}
		else{
			a=sll_operator_cast(a,sll_static_int[SLL_OBJECT_TYPE_INT]);
		}
	}
	else{
		a=SLL_ACQUIRE_STATIC_INT(0);
	}
	sll_integer_t out=sll_api_time_sleep_nanos(a);
	SLL_RELEASE(a);
	return SLL_FROM_INT(out);
}



static const internal_function_t _ifunc_data_ptr[]={
	{
		"sll:array_count",
		sll_api_array_count_raw,
		SLL_INTERNAL_FUNCTION_FLAG_COMPILATION_CALL
	},
	{
		"sll:array_create",
		sll_api_array_create_raw,
		SLL_INTERNAL_FUNCTION_FLAG_COMPILATION_CALL
	},
	{
		"sll:array_replace",
		sll_api_array_replace_raw,
		SLL_INTERNAL_FUNCTION_FLAG_COMPILATION_CALL
	},
	{
		"sll:array_reverse",
		sll_api_array_reverse_raw,
		SLL_INTERNAL_FUNCTION_FLAG_COMPILATION_CALL
	},
	{
		"sll:base64_decode",
		sll_api_base64_decode_raw,
		SLL_INTERNAL_FUNCTION_FLAG_COMPILATION_CALL
	},
	{
		"sll:base64_encode",
		sll_api_base64_encode_raw,
		SLL_INTERNAL_FUNCTION_FLAG_COMPILATION_CALL
	},
	{
		"sll:date_merge",
		sll_api_date_merge_raw,
		SLL_INTERNAL_FUNCTION_FLAG_COMPILATION_CALL
	},
	{
		"sll:date_split",
		sll_api_date_split_raw,
		SLL_INTERNAL_FUNCTION_FLAG_COMPILATION_CALL
	},
	{
		"sll:file_close",
		sll_api_file_close_raw,
		SLL_INTERNAL_FUNCTION_FLAG_REQUIRED
	},
	{
		"sll:file_flush",
		sll_api_file_flush_raw,
		SLL_INTERNAL_FUNCTION_FLAG_REQUIRED
	},
	{
		"sll:file_open",
		sll_api_file_open_raw,
		0
	},
	{
		"sll:file_read",
		sll_api_file_read_raw,
		SLL_INTERNAL_FUNCTION_FLAG_REQUIRED
	},
	{
		"sll:file_std_handle",
		sll_api_file_std_handle_raw,
		0
	},
	{
		"sll:file_write",
		sll_api_file_write_raw,
		SLL_INTERNAL_FUNCTION_FLAG_REQUIRED
	},
	{
		"sll:hash_md5",
		sll_api_hash_md5_raw,
		SLL_INTERNAL_FUNCTION_FLAG_COMPILATION_CALL
	},
	{
		"sll:hash_sha1",
		sll_api_hash_sha1_raw,
		SLL_INTERNAL_FUNCTION_FLAG_COMPILATION_CALL
	},
	{
		"sll:hash_sha256",
		sll_api_hash_sha256_raw,
		SLL_INTERNAL_FUNCTION_FLAG_COMPILATION_CALL
	},
	{
		"sll:hash_sha512",
		sll_api_hash_sha512_raw,
		SLL_INTERNAL_FUNCTION_FLAG_COMPILATION_CALL
	},
	{
		"sll:int_to_bin",
		sll_api_int_to_bin_raw,
		SLL_INTERNAL_FUNCTION_FLAG_COMPILATION_CALL
	},
	{
		"sll:int_to_hex",
		sll_api_int_to_hex_raw,
		SLL_INTERNAL_FUNCTION_FLAG_COMPILATION_CALL
	},
	{
		"sll:int_to_oct",
		sll_api_int_to_oct_raw,
		SLL_INTERNAL_FUNCTION_FLAG_COMPILATION_CALL
	},
	{
		"sll:json__init",
		sll_api_json__init_raw,
		SLL_INTERNAL_FUNCTION_FLAG_REQUIRED
	},
	{
		"sll:json_parse",
		sll_api_json_parse_raw,
		0
	},
	{
		"sll:json_stringify",
		sll_api_json_stringify_raw,
		SLL_INTERNAL_FUNCTION_FLAG_COMPILATION_CALL
	},
	{
		"sll:log_log",
		sll_api_log_log_raw,
		0
	},
	{
		"sll:log_set_default",
		sll_api_log_set_default_raw,
		0
	},
	{
		"sll:log_set_file",
		sll_api_log_set_file_raw,
		0
	},
	{
		"sll:log_set_function",
		sll_api_log_set_function_raw,
		0
	},
	{
		"sll:math_abs",
		sll_api_math_abs_raw,
		SLL_INTERNAL_FUNCTION_FLAG_COMPILATION_CALL
	},
	{
		"sll:math_acos",
		sll_api_math_acos_raw,
		SLL_INTERNAL_FUNCTION_FLAG_COMPILATION_CALL
	},
	{
		"sll:math_acosh",
		sll_api_math_acosh_raw,
		SLL_INTERNAL_FUNCTION_FLAG_COMPILATION_CALL
	},
	{
		"sll:math_asin",
		sll_api_math_asin_raw,
		SLL_INTERNAL_FUNCTION_FLAG_COMPILATION_CALL
	},
	{
		"sll:math_asinh",
		sll_api_math_asinh_raw,
		SLL_INTERNAL_FUNCTION_FLAG_COMPILATION_CALL
	},
	{
		"sll:math_atan",
		sll_api_math_atan_raw,
		SLL_INTERNAL_FUNCTION_FLAG_COMPILATION_CALL
	},
	{
		"sll:math_atan2",
		sll_api_math_atan2_raw,
		SLL_INTERNAL_FUNCTION_FLAG_COMPILATION_CALL
	},
	{
		"sll:math_atanh",
		sll_api_math_atanh_raw,
		SLL_INTERNAL_FUNCTION_FLAG_COMPILATION_CALL
	},
	{
		"sll:math_cbrt",
		sll_api_math_cbrt_raw,
		SLL_INTERNAL_FUNCTION_FLAG_COMPILATION_CALL
	},
	{
		"sll:math_combinations",
		sll_api_math_combinations_raw,
		SLL_INTERNAL_FUNCTION_FLAG_COMPILATION_CALL
	},
	{
		"sll:math_cos",
		sll_api_math_cos_raw,
		SLL_INTERNAL_FUNCTION_FLAG_COMPILATION_CALL
	},
	{
		"sll:math_cosh",
		sll_api_math_cosh_raw,
		SLL_INTERNAL_FUNCTION_FLAG_COMPILATION_CALL
	},
	{
		"sll:math_ceil",
		sll_api_math_ceil_raw,
		SLL_INTERNAL_FUNCTION_FLAG_COMPILATION_CALL
	},
	{
		"sll:math_copy_sign",
		sll_api_math_copy_sign_raw,
		SLL_INTERNAL_FUNCTION_FLAG_COMPILATION_CALL
	},
	{
		"sll:math_factorial",
		sll_api_math_factorial_raw,
		SLL_INTERNAL_FUNCTION_FLAG_COMPILATION_CALL
	},
	{
		"sll:math_gcd",
		sll_api_math_gcd_raw,
		SLL_INTERNAL_FUNCTION_FLAG_COMPILATION_CALL
	},
	{
		"sll:math_floor",
		sll_api_math_floor_raw,
		SLL_INTERNAL_FUNCTION_FLAG_COMPILATION_CALL
	},
	{
		"sll:math_int_log2",
		sll_api_math_int_log2_raw,
		SLL_INTERNAL_FUNCTION_FLAG_COMPILATION_CALL
	},
	{
		"sll:math_int_pow",
		sll_api_math_int_pow_raw,
		SLL_INTERNAL_FUNCTION_FLAG_COMPILATION_CALL
	},
	{
		"sll:math_int_sqrt",
		sll_api_math_int_sqrt_raw,
		SLL_INTERNAL_FUNCTION_FLAG_COMPILATION_CALL
	},
	{
		"sll:math_log",
		sll_api_math_log_raw,
		SLL_INTERNAL_FUNCTION_FLAG_COMPILATION_CALL
	},
	{
		"sll:math_log2",
		sll_api_math_log2_raw,
		SLL_INTERNAL_FUNCTION_FLAG_COMPILATION_CALL
	},
	{
		"sll:math_log10",
		sll_api_math_log10_raw,
		SLL_INTERNAL_FUNCTION_FLAG_COMPILATION_CALL
	},
	{
		"sll:math_permutations",
		sll_api_math_permutations_raw,
		SLL_INTERNAL_FUNCTION_FLAG_COMPILATION_CALL
	},
	{
		"sll:math_pow",
		sll_api_math_pow_raw,
		SLL_INTERNAL_FUNCTION_FLAG_COMPILATION_CALL
	},
	{
		"sll:math_round",
		sll_api_math_round_raw,
		SLL_INTERNAL_FUNCTION_FLAG_COMPILATION_CALL
	},
	{
		"sll:math_sin",
		sll_api_math_sin_raw,
		SLL_INTERNAL_FUNCTION_FLAG_COMPILATION_CALL
	},
	{
		"sll:math_sinh",
		sll_api_math_sinh_raw,
		SLL_INTERNAL_FUNCTION_FLAG_COMPILATION_CALL
	},
	{
		"sll:math_sqrt",
		sll_api_math_sqrt_raw,
		SLL_INTERNAL_FUNCTION_FLAG_COMPILATION_CALL
	},
	{
		"sll:math_tan",
		sll_api_math_tan_raw,
		SLL_INTERNAL_FUNCTION_FLAG_COMPILATION_CALL
	},
	{
		"sll:math_tanh",
		sll_api_math_tanh_raw,
		SLL_INTERNAL_FUNCTION_FLAG_COMPILATION_CALL
	},
	{
		"sll:path_absolute",
		sll_api_path_absolute_raw,
		0
	},
	{
		"sll:path_exists",
		sll_api_path_exists_raw,
		0
	},
	{
		"sll:path_get_cwd",
		sll_api_path_get_cwd_raw,
		0
	},
	{
		"sll:path_is_dir",
		sll_api_path_is_dir_raw,
		0
	},
	{
		"sll:path_join",
		sll_api_path_join_raw,
		0
	},
	{
		"sll:path_list_dir",
		sll_api_path_list_dir_raw,
		0
	},
	{
		"sll:path_recursive_list_dir",
		sll_api_path_recursive_list_dir_raw,
		0
	},
	{
		"sll:path_relative",
		sll_api_path_relative_raw,
		0
	},
	{
		"sll:path_set_cwd",
		sll_api_path_set_cwd_raw,
		0
	},
	{
		"sll:path_size",
		sll_api_path_size_raw,
		0
	},
	{
		"sll:path_split",
		sll_api_path_split_raw,
		SLL_INTERNAL_FUNCTION_FLAG_COMPILATION_CALL
	},
	{
		"sll:process__init",
		sll_api_process__init_raw,
		SLL_INTERNAL_FUNCTION_FLAG_REQUIRED
	},
	{
		"sll:process_join",
		sll_api_process_join_raw,
		SLL_INTERNAL_FUNCTION_FLAG_REQUIRED
	},
	{
		"sll:process_execute_shell",
		sll_api_process_execute_shell_raw,
		SLL_INTERNAL_FUNCTION_FLAG_REQUIRED
	},
	{
		"sll:process_get_pid",
		sll_api_process_get_pid_raw,
		0
	},
	{
		"sll:process_start",
		sll_api_process_start_raw,
		SLL_INTERNAL_FUNCTION_FLAG_REQUIRED
	},
	{
		"sll:process_split",
		sll_api_process_split_raw,
		SLL_INTERNAL_FUNCTION_FLAG_REQUIRED
	},
	{
		"sll:sort_quicksort",
		sll_api_sort_quicksort_raw,
		SLL_INTERNAL_FUNCTION_FLAG_COMPILATION_CALL
	},
	{
		"sll:string_checksum",
		sll_api_string_checksum_raw,
		SLL_INTERNAL_FUNCTION_FLAG_COMPILATION_CALL
	},
	{
		"sll:string_convert",
		sll_api_string_convert_raw,
		SLL_INTERNAL_FUNCTION_FLAG_COMPILATION_CALL
	},
	{
		"sll:string_count",
		sll_api_string_count_raw,
		SLL_INTERNAL_FUNCTION_FLAG_COMPILATION_CALL
	},
	{
		"sll:string_count_left",
		sll_api_string_count_left_raw,
		SLL_INTERNAL_FUNCTION_FLAG_COMPILATION_CALL
	},
	{
		"sll:string_count_right",
		sll_api_string_count_right_raw,
		SLL_INTERNAL_FUNCTION_FLAG_COMPILATION_CALL
	},
	{
		"sll:string_ends",
		sll_api_string_ends_raw,
		SLL_INTERNAL_FUNCTION_FLAG_COMPILATION_CALL
	},
	{
		"sll:string_format",
		sll_api_string_format_raw,
		SLL_INTERNAL_FUNCTION_FLAG_COMPILATION_CALL
	},
	{
		"sll:string_flip_case",
		sll_api_string_flip_case_raw,
		SLL_INTERNAL_FUNCTION_FLAG_COMPILATION_CALL
	},
	{
		"sll:string_index",
		sll_api_string_index_raw,
		SLL_INTERNAL_FUNCTION_FLAG_COMPILATION_CALL
	},
	{
		"sll:string_index_list",
		sll_api_string_index_list_raw,
		SLL_INTERNAL_FUNCTION_FLAG_COMPILATION_CALL
	},
	{
		"sll:string_index_reverse",
		sll_api_string_index_reverse_raw,
		SLL_INTERNAL_FUNCTION_FLAG_COMPILATION_CALL
	},
	{
		"sll:string_index_reverse_list",
		sll_api_string_index_reverse_list_raw,
		SLL_INTERNAL_FUNCTION_FLAG_COMPILATION_CALL
	},
	{
		"sll:string_join",
		sll_api_string_join_raw,
		SLL_INTERNAL_FUNCTION_FLAG_COMPILATION_CALL
	},
	{
		"sll:string_pad_left",
		sll_api_string_pad_left_raw,
		SLL_INTERNAL_FUNCTION_FLAG_COMPILATION_CALL
	},
	{
		"sll:string_pad_right",
		sll_api_string_pad_right_raw,
		SLL_INTERNAL_FUNCTION_FLAG_COMPILATION_CALL
	},
	{
		"sll:string_replace",
		sll_api_string_replace_raw,
		SLL_INTERNAL_FUNCTION_FLAG_COMPILATION_CALL
	},
	{
		"sll:string_reverse",
		sll_api_string_reverse_raw,
		SLL_INTERNAL_FUNCTION_FLAG_COMPILATION_CALL
	},
	{
		"sll:string_split",
		sll_api_string_split_raw,
		SLL_INTERNAL_FUNCTION_FLAG_COMPILATION_CALL
	},
	{
		"sll:string_starts",
		sll_api_string_starts_raw,
		SLL_INTERNAL_FUNCTION_FLAG_COMPILATION_CALL
	},
	{
		"sll:string_to_lower_case",
		sll_api_string_to_lower_case_raw,
		SLL_INTERNAL_FUNCTION_FLAG_COMPILATION_CALL
	},
	{
		"sll:string_to_title_case",
		sll_api_string_to_title_case_raw,
		SLL_INTERNAL_FUNCTION_FLAG_COMPILATION_CALL
	},
	{
		"sll:string_to_upper_case",
		sll_api_string_to_upper_case_raw,
		SLL_INTERNAL_FUNCTION_FLAG_COMPILATION_CALL
	},
	{
		"sll:sys_get_args",
		sll_api_sys_get_args_raw,
		0
	},
	{
		"sll:sys_get_executable",
		sll_api_sys_get_executable_raw,
		0
	},
	{
		"sll:sys_get_library",
		sll_api_sys_get_library_raw,
		0
	},
	{
		"sll:sys_get_platform",
		sll_api_sys_get_platform_raw,
		0
	},
	{
		"sll:sys_get_version",
		sll_api_sys_get_version_raw,
		0
	},
	{
		"sll:sys_load_library",
		sll_api_sys_load_library_raw,
		0
	},
	{
		"sll:time_current",
		sll_api_time_current_raw,
		0
	},
	{
		"sll:time_current_nanos",
		sll_api_time_current_nanos_raw,
		0
	},
	{
		"sll:time_sleep",
		sll_api_time_sleep_raw,
		0
	},
	{
		"sll:time_sleep_nanos",
		sll_api_time_sleep_nanos_raw,
		0
	}
};



const sll_function_index_t _ifunc_size=109;
const internal_function_t* _ifunc_data=(const internal_function_t*)(&_ifunc_data_ptr);
