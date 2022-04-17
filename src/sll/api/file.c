#include <sll/_internal/file.h>
#include <sll/_internal/static_object.h>
#include <sll/api/path.h>
#include <sll/api/string.h>
#include <sll/audit.h>
#include <sll/common.h>
#include <sll/data.h>
#include <sll/error.h>
#include <sll/file.h>
#include <sll/gc.h>
#include <sll/init.h>
#include <sll/memory.h>
#include <sll/object.h>
#include <sll/platform/path.h>
#include <sll/sandbox.h>
#include <sll/static_object.h>
#include <sll/string.h>
#include <sll/types.h>
#include <sll/vm.h>



static extended_file_t** _file_fl=NULL;
static sll_integer_t _file_fll=0;
static sll_bool_t _file_cleanup=0;



static void _release_data(void){
	for (sll_integer_t i=0;i<_file_fll;i++){
		if (*(_file_fl+i)){
			if ((*(_file_fl+i))->p){
				sll_file_close((*(_file_fl+i))->dt.p);
			}
			else{
				sll_file_close(&((*(_file_fl+i))->dt.f));
			}
			sll_deallocate(*(_file_fl+i));
		}
	}
	sll_deallocate(_file_fl);
	_file_fl=NULL;
	_file_fll=0;
	_file_cleanup=0;
}



static sll_integer_t _alloc_file(void){
	sll_integer_t o=0;
	while (o<_file_fll){
		if (!(_file_fl+o)){
			break;
		}
		o++;
	}
	if (o==_file_fll){
		_file_fll++;
		_file_fl=sll_reallocate(_file_fl,_file_fll*sizeof(extended_file_t*));
	}
	extended_file_t* n=sll_allocate(sizeof(extended_file_t));
	n->rc=1;
	*(_file_fl+o)=n;
	return o;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_file_t* sll_file_from_handle(sll_integer_t h){
	if (h<0||h>=_file_fll||sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_FILE_IO)||!(*(_file_fl+h))){
		return NULL;
	}
	extended_file_t* ef=*(_file_fl+h);
	return (ef->p?ef->dt.p:&(ef->dt.f));
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_integer_t sll_file_to_handle(sll_file_t* f){
	sll_integer_t o=_alloc_file();
	(*(_file_fl+o))->dt.p=f;
	(*(_file_fl+o))->p=1;
	return o;
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_bool_t sll_api_file_close(sll_integer_t a){
	if (a<0||a>=_file_fll||!(*(_file_fl+a))){
		return 0;
	}
	(*(_file_fl+a))->rc--;
	if ((*(_file_fl+a))->rc){
		return 1;
	}
	if ((*(_file_fl+a))->p){
		sll_file_close((*(_file_fl+a))->dt.p);
	}
	else{
		sll_file_close(&((*(_file_fl+a))->dt.f));
	}
	sll_deallocate(*(_file_fl+a));
	*(_file_fl+a)=NULL;
	if (a==_file_fll-1){
		do{
			_file_fll--;
		} while (_file_fll&&!(_file_fl+_file_fll-1));
		_file_fl=sll_reallocate(_file_fl,_file_fll*sizeof(extended_file_t*));
	}
	return 1;
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_integer_t sll_api_file_copy(sll_string_t* a,sll_string_t* b){
	if (a->l>SLL_API_MAX_FILE_PATH_LENGTH||b->l>SLL_API_MAX_FILE_PATH_LENGTH){
		return SLL_ERROR_TOO_LONG;
	}
	if (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_PATH_API)&&!sll_get_sandbox_flag(SLL_SANDBOX_FLAG_ENABLE_FILE_COPY)){
		return SLL_ERROR_FROM_SANDBOX(SLL_SANDBOX_FLAG_DISABLE_PATH_API);
	}
	sll_audit(SLL_CHAR("sll.file.copy"),SLL_CHAR("ss"),a,b);
	return sll_platform_path_copy(a->v,b->v);
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_integer_t sll_api_file_delete(sll_string_t* a){
	if (a->l>SLL_API_MAX_FILE_PATH_LENGTH){
		return SLL_ERROR_TOO_LONG;
	}
	if (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_PATH_API)&&!sll_get_sandbox_flag(SLL_SANDBOX_FLAG_ENABLE_FILE_DELETE)){
		return SLL_ERROR_FROM_SANDBOX(SLL_SANDBOX_FLAG_DISABLE_PATH_API);
	}
	sll_audit(SLL_CHAR("sll.file.delete"),SLL_CHAR("s"),a);
	return sll_platform_path_delete(a->v);
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_bool_t sll_api_file_flush(sll_integer_t a){
	if (a<0||a>=_file_fll||!(*(_file_fl+a))){
		return 0;
	}
	extended_file_t* ef=*(_file_fl+a);
	sll_file_flush((ef->p?ef->dt.p:&(ef->dt.f)));
	return 1;
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_integer_t sll_api_file_from_data(sll_string_t* a,sll_integer_t b){
	if (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_FILE_IO)&&!sll_get_sandbox_flag(SLL_SANDBOX_FLAG_ENABLE_BUFFER_FILES)){
		return ~SLL_ERROR_FROM_SANDBOX(SLL_SANDBOX_FLAG_DISABLE_FILE_IO);
	}
	sll_file_t f;
	sll_file_from_data(a->v,a->l,(sll_file_flags_t)(b&(SLL_FILE_FLAG_READ|SLL_FILE_FLAG_WRITE|SLL_FILE_FLAG_APPEND|SLL_FILE_FLAG_NO_BUFFER)),&f);
	sll_integer_t h=_alloc_file();
	sll_copy_data(&f,sizeof(sll_file_t),&((*(_file_fl+h))->dt.f));
	(*(_file_fl+h))->p=0;
	return h;
}



__SLL_EXTERNAL __SLL_API_CALL void sll_api_file_get_buffer(sll_integer_t a,sll_string_t* out){
	SLL_INIT_STRING(out);
	if (a<0||a>=_file_fll||!(*(_file_fl+a))){
		return;
	}
	extended_file_t* ef=*(_file_fl+a);
	sll_file_get_buffer((ef->p?ef->dt.p:&(ef->dt.f)),out);
}



__SLL_EXTERNAL __SLL_API_CALL void sll_api_file_get_temp_path(sll_string_t* out){
	sll_string_clone(sll_temporary_file_path,out);
}



__SLL_EXTERNAL __SLL_API_CALL void sll_api_file_inc_handle(sll_integer_t a){
	if (a<0||a>=_file_fll||!(*(_file_fl+a))){
		return;
	}
	(*(_file_fl+a))->rc++;
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_integer_t sll_api_file_open(sll_string_t* a,sll_integer_t b){
	if (a->l>SLL_API_MAX_FILE_PATH_LENGTH){
		return ~SLL_ERROR_TOO_LONG;
	}
	if (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_FILE_IO)){
		return ~SLL_ERROR_FROM_SANDBOX(SLL_SANDBOX_FLAG_DISABLE_FILE_IO);
	}
	sll_file_flags_t ff=(sll_file_flags_t)(b&(SLL_FILE_FLAG_READ|SLL_FILE_FLAG_WRITE|SLL_FILE_FLAG_APPEND|SLL_FILE_FLAG_NO_BUFFER));
	sll_audit(SLL_CHAR("sll.file.open"),SLL_CHAR("sh"),a,ff);
	sll_file_t f;
	sll_error_t err=sll_file_open(a->v,ff,&f);
	if (err!=SLL_NO_ERROR){
		return ~err;
	}
	sll_integer_t h=_alloc_file();
	sll_copy_data(&f,sizeof(sll_file_t),&((*(_file_fl+h))->dt.f));
	(*(_file_fl+h))->p=0;
	return h;
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_object_t* sll_api_file_peek(sll_integer_t a){
	if (a<0||a>=_file_fll||!(*(_file_fl+a))){
		return sll_int_to_object(~SLL_ERROR_UNKNOWN_FD);
	}
	extended_file_t* ef=*(_file_fl+a);
	sll_error_t err;
	sll_read_char_t o=sll_file_peek_char((ef->p?ef->dt.p:&(ef->dt.f)),&err);
	return (o==SLL_END_OF_DATA?sll_int_to_object(~err):SLL_FROM_CHAR(o));
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_object_t* sll_api_file_read(sll_integer_t a,sll_integer_t b){
	if (a<0||a>=_file_fll||!(*(_file_fl+a))){
		return sll_int_to_object(SLL_ERROR_UNKNOWN_FD);
	}
	if (!b){
		SLL_UNIMPLEMENTED();
	}
	extended_file_t* ef=*(_file_fl+a);
	sll_string_length_t l=(sll_string_length_t)b;
	sll_string_t o;
	sll_string_create(l,&o);
	sll_error_t err;
	sll_size_t sz=sll_file_read((ef->p?ef->dt.p:&(ef->dt.f)),o.v,l,&err);
	if (!sz&&err!=SLL_NO_ERROR){
		sll_free_string(&o);
		return sll_int_to_object(err);
	}
	sll_string_decrease(&o,(sll_string_length_t)sz);
	sll_string_calculate_checksum(&o);
	return STRING_TO_OBJECT_NOCOPY(&o);
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_object_t* sll_api_file_read_char(sll_integer_t a){
	if (a<0||a>=_file_fll||!(*(_file_fl+a))){
		return sll_int_to_object(SLL_ERROR_UNKNOWN_FD);
	}
	extended_file_t* ef=*(_file_fl+a);
	sll_error_t err;
	sll_read_char_t c=sll_file_read_char((ef->p?ef->dt.p:&(ef->dt.f)),&err);
	if (c==SLL_END_OF_DATA&&err!=SLL_NO_ERROR){
		return sll_int_to_object(err);
	}
	return (c==SLL_END_OF_DATA?SLL_ACQUIRE_STATIC_NEG_INT(1):SLL_FROM_CHAR(c));
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_integer_t sll_api_file_rename(sll_string_t* a,sll_string_t* b){
	if (a->l>SLL_API_MAX_FILE_PATH_LENGTH||b->l>SLL_API_MAX_FILE_PATH_LENGTH){
		return SLL_ERROR_TOO_LONG;
	}
	if (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_PATH_API)&&!sll_get_sandbox_flag(SLL_SANDBOX_FLAG_ENABLE_FILE_RENAME)){
		return SLL_ERROR_FROM_SANDBOX(SLL_SANDBOX_FLAG_DISABLE_PATH_API);
	}
	sll_audit(SLL_CHAR("sll.file.rename"),SLL_CHAR("ss"),a,b);
	return sll_platform_path_rename(a->v,b->v);
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_integer_t sll_api_file_std_handle(sll_integer_t a){
	if (!_file_cleanup){
		sll_register_cleanup(_release_data);
		_file_cleanup=1;
	}
	sll_file_t* p=NULL;
	if (!a){
		if (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_FILE_IO)&&!sll_get_sandbox_flag(SLL_SANDBOX_FLAG_ENABLE_STDIN_IO)){
			return ~SLL_ERROR_FROM_SANDBOX(SLL_SANDBOX_FLAG_DISABLE_FILE_IO);
		}
		p=sll_current_vm_config->in;
	}
	else if (a==1){
		if (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_FILE_IO)&&!sll_get_sandbox_flag(SLL_SANDBOX_FLAG_ENABLE_STDOUT_IO)){
			return ~SLL_ERROR_FROM_SANDBOX(SLL_SANDBOX_FLAG_DISABLE_FILE_IO);
		}
		p=sll_current_vm_config->out;
	}
	else{
		if (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_FILE_IO)){
			return ~SLL_ERROR_FROM_SANDBOX(SLL_SANDBOX_FLAG_DISABLE_FILE_IO);
		}
		p=sll_current_vm_config->err;
	}
	sll_integer_t h=_alloc_file();
	(*(_file_fl+h))->dt.p=p;
	(*(_file_fl+h))->p=1;
	return h;
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_integer_t sll_api_file_write(sll_integer_t a,sll_string_t* b){
	if (a<0||a>=_file_fll||!(*(_file_fl+a))){
		return ~SLL_ERROR_UNKNOWN_FD;
	}
	extended_file_t* ef=*(_file_fl+a);
	sll_error_t err;
	sll_size_t o=sll_file_write((ef->p?ef->dt.p:&(ef->dt.f)),b->v,b->l*sizeof(sll_char_t),&err);
	return (!o&&err!=SLL_NO_ERROR?~err:o*sizeof(sll_char_t));
}
