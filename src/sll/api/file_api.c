#include <sll/_sll_internal.h>
#include <sll/api.h>
#include <sll/api/path.h>
#include <sll/api/string.h>
#include <sll/common.h>
#include <sll/data.h>
#include <sll/file.h>
#include <sll/gc.h>
#include <sll/init.h>
#include <sll/memory.h>
#include <sll/object.h>
#include <sll/platform.h>
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
			goto _found_index;
		}
		o++;
	}
	_file_fll++;
	_file_fl=sll_reallocate(_file_fl,_file_fll*sizeof(extended_file_t*));
_found_index:;
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



__API_FUNC(file_close){
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
		if (_file_fll){
			_file_fl=sll_reallocate(_file_fl,_file_fll*sizeof(extended_file_t*));
		}
		else{
			sll_deallocate(_file_fl);
			_file_fl=NULL;
		}
	}
	return 1;
}



__API_FUNC(file_copy){
	if (a->l>SLL_API_MAX_FILE_PATH_LENGTH||b->l>SLL_API_MAX_FILE_PATH_LENGTH||(sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_PATH_API)&&!sll_get_sandbox_flag(SLL_SANDBOX_FLAG_ENABLE_FILE_COPY))){
		return 0;
	}
	return sll_platform_path_copy(a->v,b->v);
}



__API_FUNC(file_flush){
	if (a<0||a>=_file_fll||!(*(_file_fl+a))){
		return 0;
	}
	extended_file_t* ef=*(_file_fl+a);
	sll_file_flush((ef->p?ef->dt.p:&(ef->dt.f)));
	return 1;
}



__API_FUNC(file_from_data){
	if (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_FILE_IO)&&!sll_get_sandbox_flag(SLL_SANDBOX_FLAG_ENABLE_BUFFER_FILES)){
		return -1;
	}
	sll_file_t f;
	sll_file_from_data(a->v,a->l,(sll_file_flags_t)(b&(SLL_FILE_FLAG_READ|SLL_FILE_FLAG_WRITE|SLL_FILE_FLAG_APPEND|SLL_FILE_FLAG_NO_BUFFER)),&f);
	sll_integer_t h=_alloc_file();
	sll_copy_data(&f,sizeof(sll_file_t),&((*(_file_fl+h))->dt.f));
	(*(_file_fl+h))->p=0;
	return h;
}



__API_FUNC(file_get_buffer){
	SLL_INIT_STRING(out);
	if (a<0||a>=_file_fll||!(*(_file_fl+a))){
		return;
	}
	extended_file_t* ef=*(_file_fl+a);
	sll_file_get_buffer((ef->p?ef->dt.p:&(ef->dt.f)),out);
}



__API_FUNC(file_get_temp_path){
	sll_char_t bf[SLL_API_MAX_FILE_PATH_LENGTH];
	sll_string_from_pointer_length(bf,sll_platform_get_temporary_file_path(bf,SLL_API_MAX_FILE_PATH_LENGTH),out);
}



__API_FUNC(file_inc_handle){
	if (a<0||a>=_file_fll||!(*(_file_fl+a))){
		return;
	}
	(*(_file_fl+a))->rc++;
}



__API_FUNC(file_open){
	if (a->l>SLL_API_MAX_FILE_PATH_LENGTH||sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_FILE_IO)){
		return -1;
	}
	sll_file_t f;
	if (!sll_file_open(a->v,(sll_file_flags_t)(b&(SLL_FILE_FLAG_READ|SLL_FILE_FLAG_WRITE|SLL_FILE_FLAG_APPEND|SLL_FILE_FLAG_NO_BUFFER)),&f)){
		return -1;
	}
	sll_integer_t h=_alloc_file();
	sll_copy_data(&f,sizeof(sll_file_t),&((*(_file_fl+h))->dt.f));
	(*(_file_fl+h))->p=0;
	return h;
}



__API_FUNC(file_peek){
	if (a<0||a>=_file_fll||!(*(_file_fl+a))){
		return SLL_ACQUIRE_STATIC_INT(0);
	}
	extended_file_t* ef=*(_file_fl+a);
	sll_read_char_t o=sll_file_peek_char((ef->p?ef->dt.p:&(ef->dt.f)));
	return (o==SLL_END_OF_DATA?SLL_ACQUIRE_STATIC_INT(0):SLL_FROM_CHAR(o));
}



__API_FUNC(file_read){
	SLL_INIT_STRING(out);
	if (a<0||a>=_file_fll||!(*(_file_fl+a))){
		return;
	}
	if (!b){
		SLL_UNIMPLEMENTED();
	}
	sll_string_length_t l=(sll_string_length_t)b;
	sll_string_create(l,out);
	extended_file_t* ef=*(_file_fl+a);
	sll_string_decrease(out,(sll_string_length_t)sll_file_read((ef->p?ef->dt.p:&(ef->dt.f)),out->v,l));
	sll_string_calculate_checksum(out);
}



__API_FUNC(file_rename){
	if (a->l>SLL_API_MAX_FILE_PATH_LENGTH||b->l>SLL_API_MAX_FILE_PATH_LENGTH||(sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_PATH_API)&&!sll_get_sandbox_flag(SLL_SANDBOX_FLAG_ENABLE_FILE_RENAME))){
		return 0;
	}
	return sll_platform_path_rename(a->v,b->v);
}



__API_FUNC(file_std_handle){
	if (!_file_cleanup){
		sll_register_cleanup(_release_data);
		_file_cleanup=1;
	}
	sll_file_t* p=NULL;
	if (!a){
		if (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_FILE_IO)&&!sll_get_sandbox_flag(SLL_SANDBOX_FLAG_ENABLE_STDIN_IO)){
			return -1;
		}
		p=sll_current_vm_config->in;
	}
	else if (a==1){
		if (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_FILE_IO)&&!sll_get_sandbox_flag(SLL_SANDBOX_FLAG_ENABLE_STDOUT_IO)){
			return -1;
		}
		p=sll_current_vm_config->out;
	}
	else{
		if (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_FILE_IO)){
			return -1;
		}
		p=sll_current_vm_config->err;
	}
	sll_integer_t h=_alloc_file();
	(*(_file_fl+h))->dt.p=p;
	(*(_file_fl+h))->p=1;
	return h;
}



__API_FUNC(file_write){
	if (a<0||a>=_file_fll||!(*(_file_fl+a))){
		return 0;
	}
	sll_string_t s;
	sll_api_string_convert(b,bc,&s);
	extended_file_t* ef=*(_file_fl+a);
	sll_size_t o=sll_file_write((ef->p?ef->dt.p:&(ef->dt.f)),s.v,s.l*sizeof(sll_char_t));
	sll_free_string(&s);
	return o*sizeof(sll_char_t);
}
