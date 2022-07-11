#include <sll/_internal/common.h>
#include <sll/_internal/platform.h>
#include <sll/_internal/static_string.h>
#include <sll/_internal/string.h>
#include <sll/common.h>
#include <sll/environment.h>
#include <sll/memory.h>
#include <sll/platform/environment.h>
#include <sll/platform/lock.h>
#include <sll/search_path.h>
#include <sll/string.h>
#include <sll/types.h>



#define LOCK_ENV SLL_CRITICAL_ERROR(sll_platform_lock_acquire(_env_lock))
#define UNLOCK_ENV SLL_CRITICAL(sll_platform_lock_release(_env_lock))



static sll_environment_t _env_data={NULL,0};
static sll_lock_handle_t _env_lock;
static __STATIC_STRING(_env_path_var_name,"PATH");
static sll_search_path_t _env_path;



__SLL_EXTERNAL const sll_environment_t* sll_environment=&_env_data;
__SLL_EXTERNAL const sll_search_path_t* sll_environment_path=&_env_path;



void _deinit_environment(void){
	SLL_CRITICAL(sll_platform_lock_delete(_env_lock));
	sll_free_search_path(&_env_path);
	for (sll_environment_length_t i=0;i<sll_environment->length;i++){
		const sll_environment_variable_t* key_value=*(sll_environment->data+i);
		sll_free_string((sll_string_t*)(&(key_value->key)));
		sll_free_string((sll_string_t*)(&(key_value->value)));
		sll_deallocate(PTR(key_value));
	}
	*((sll_environment_length_t*)(&(sll_environment->length)))=0;
	sll_deallocate(PTR(sll_environment->data));
}



void _init_environment(void){
	_init_platform_env();
	for (sll_environment_length_t i=0;i<sll_environment->length;i++){
		sll_environment_variable_t* key_value=(sll_environment_variable_t*)(*(sll_environment->data+i));
		sll_string_upper_case(NULL,(sll_string_t*)(&(key_value->key)));
	}
	_env_lock=sll_platform_lock_create(NULL);
	sll_string_t tmp;
	sll_string_from_pointer(SLL_CHAR("path"),&tmp);
	sll_search_path_from_environment(&tmp,&_env_path);
	sll_free_string(&tmp);
}



__SLL_EXTERNAL sll_bool_t sll_expand_environment_variable(const sll_string_t* key,sll_string_t* out){
	if (!sll_get_environment_variable(key,out)){
		return 0;
	}
	if (!out->length){
		return 1;
	}
	sll_string_length_t i=0;
	while (1){
		i=sll_string_index_char(out,'%',0,i);
		if (i==SLL_MAX_STRING_LENGTH){
			break;
		}
		sll_string_length_t j=sll_string_index_char(out,'%',0,i);
		if (j==SLL_MAX_STRING_LENGTH){
			break;
		}
		SLL_UNIMPLEMENTED();
	}
	return 1;
}



__SLL_EXTERNAL sll_bool_t sll_get_environment_variable(const sll_string_t* key,sll_string_t* out){
	sll_string_t lowercase_key;
	sll_string_upper_case(key,&lowercase_key);
	LOCK_ENV;
	for (sll_environment_length_t i=0;i<sll_environment->length;i++){
		const sll_environment_variable_t* key_value=*(sll_environment->data+i);
		if (STRING_EQUAL(&lowercase_key,&(key_value->key))){
			if (out){
				sll_string_clone(&(key_value->value),out);
			}
			UNLOCK_ENV;
			sll_free_string(&lowercase_key);
			return 1;
		}
	}
	UNLOCK_ENV;
	sll_free_string(&lowercase_key);
	SLL_INIT_STRING(out);
	return 0;
}



__SLL_EXTERNAL void sll_remove_environment_variable(const sll_string_t* key){
	sll_string_t lowercase_key;
	sll_string_upper_case(key,&lowercase_key);
	LOCK_ENV;
	sll_environment_length_t i=0;
	while (i<sll_environment->length){
		sll_environment_variable_t* key_value=(sll_environment_variable_t*)(*(sll_environment->data+i));
		if (STRING_EQUAL(&lowercase_key,&(key_value->key))){
			sll_platform_remove_environment_variable(key->data);
			sll_free_string((sll_string_t*)(&(key_value->key)));
			sll_free_string((sll_string_t*)(&(key_value->value)));
			sll_deallocate(key_value);
			i++;
			while (i<sll_environment->length){
				*(((const sll_environment_variable_t**)(sll_environment->data))+i-1)=*(sll_environment->data+i);
				i++;
			}
			(*((sll_environment_length_t*)(&(sll_environment->length))))--;
			*((const sll_environment_variable_t*const**)(&(sll_environment->data)))=sll_reallocate(PTR(sll_environment->data),sll_environment->length*sizeof(sll_environment_variable_t*));
			goto _cleanup;
		}
		i++;
	}
_cleanup:
	if (sll_string_equal(&lowercase_key,&_env_path_var_name)){
		sll_free_search_path(&_env_path);
		sll_search_path_from_environment(&_env_path_var_name,&_env_path);
	}
	UNLOCK_ENV;
	sll_free_string(&lowercase_key);
}



__SLL_EXTERNAL void sll_set_environment_variable(const sll_string_t* key,const sll_string_t* value){
	sll_string_t lowercase_key;
	sll_string_upper_case(key,&lowercase_key);
	LOCK_ENV;
	sll_platform_set_environment_variable(key->data,value->data);
	for (sll_environment_length_t i=0;i<sll_environment->length;i++){
		sll_environment_variable_t* key_value=(sll_environment_variable_t*)(*(sll_environment->data+i));
		if (STRING_EQUAL(&lowercase_key,&(key_value->key))){
			sll_free_string((sll_string_t*)(&(key_value->value)));
			sll_string_clone(value,(sll_string_t*)(&(key_value->value)));
			goto _end;
		}
	}
	(*((sll_environment_length_t*)(&(sll_environment->length))))++;
	*((const sll_environment_variable_t*const**)(&(sll_environment->data)))=sll_reallocate(PTR(sll_environment->data),sll_environment->length*sizeof(sll_environment_variable_t*));
	sll_environment_variable_t* n=sll_allocate(sizeof(sll_environment_variable_t));
	sll_string_clone(key,(sll_string_t*)(&(n->key)));
	sll_string_clone(value,(sll_string_t*)(&(n->value)));
	*(((const sll_environment_variable_t**)(sll_environment->data))+sll_environment->length-1)=n;
_end:
	UNLOCK_ENV;
	if (sll_string_equal(&lowercase_key,&_env_path_var_name)){
		sll_free_search_path(&_env_path);
		sll_search_path_from_environment(&_env_path_var_name,&_env_path);
	}
	sll_free_string(&lowercase_key);
}
