#include <sll/_internal/common.h>
#include <sll/_internal/static_string.h>
#include <sll/_internal/string.h>
#include <sll/common.h>
#include <sll/env.h>
#include <sll/init.h>
#include <sll/memory.h>
#include <sll/platform/lock.h>
#include <sll/platform/util.h>
#include <sll/search_path.h>
#include <sll/string.h>
#include <sll/types.h>



#define LOCK_ENV \
	do{ \
		if (!_env_lock){ \
			_env_lock=sll_platform_lock_create(); \
			sll_register_cleanup(_cleanup_env); \
		} \
		SLL_CRITICAL(sll_platform_lock_acquire(_env_lock)); \
	} while (0)
#define UNLOCK_ENV SLL_CRITICAL(sll_platform_lock_release(_env_lock))



static sll_lock_handle_t _env_lock=NULL;
static __STATIC_STRING(_env_path_var_name,"path");
static sll_search_path_t _env_path;



__SLL_EXTERNAL const sll_search_path_t* sll_env_path=&_env_path;



static void _cleanup_env(void){
	SLL_CRITICAL(sll_platform_lock_delete(_env_lock));
	_env_lock=NULL;
	sll_free_search_path(&_env_path);
}



static void _expand_all(sll_string_t* o,const sll_string_t* key){
	if (!sll_get_environment_variable(key,o)){
		SLL_INIT_STRING(o);
		return;
	}
	if (!o->l){
		return;
	}
	sll_string_length_t i=0;
	while (1){
		i=sll_string_index_char(o,'%',0,i);
		if (i==SLL_MAX_STRING_LENGTH){
			break;
		}
		sll_string_length_t e=sll_string_index_char(o,'%',0,i);
		if (e==SLL_MAX_STRING_LENGTH){
			break;
		}
		SLL_UNIMPLEMENTED();
	}
}



static void _rebuild_global_search_path(void){
	sll_string_t tmp;
	_expand_all(&tmp,&_env_path_var_name);
	sll_create_search_path(&tmp,&_env_path);
	sll_free_string(&tmp);
}



void _init_env(void){
	for (sll_array_length_t i=0;i<sll_environment->l;i++){
		sll_environment_variable_t* kv=(sll_environment_variable_t*)(*(sll_environment->dt+i));
		sll_string_lower_case(NULL,(sll_string_t*)(&(kv->k)));
	}
	_rebuild_global_search_path();
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_get_environment_variable(const sll_string_t* k,sll_string_t* o){
	sll_string_t k_low;
	sll_string_lower_case(k,&k_low);
	LOCK_ENV;
	for (sll_array_length_t i=0;i<sll_environment->l;i++){
		const sll_environment_variable_t* kv=*(sll_environment->dt+i);
		if (STRING_EQUAL(&k_low,&(kv->k))){
			if (o){
				sll_string_clone(&(kv->v),o);
			}
			UNLOCK_ENV;
			sll_free_string(&k_low);
			return 1;
		}
	}
	UNLOCK_ENV;
	sll_free_string(&k_low);
	SLL_INIT_STRING(o);
	return 0;
}



__SLL_EXTERNAL void sll_remove_environment_variable(const sll_string_t* k){
	sll_string_t k_low;
	sll_string_lower_case(k,&k_low);
	LOCK_ENV;
	sll_array_length_t i=0;
	while (i<sll_environment->l){
		sll_environment_variable_t* kv=(sll_environment_variable_t*)(*(sll_environment->dt+i));
		if (STRING_EQUAL(&k_low,&(kv->k))){
			sll_platform_remove_environment_variable(k->v);
			sll_free_string((sll_string_t*)(&(kv->k)));
			sll_free_string((sll_string_t*)(&(kv->v)));
			sll_deallocate(kv);
			i++;
			while (i<sll_environment->l){
				*(((const sll_environment_variable_t**)(sll_environment->dt))+i-1)=*(sll_environment->dt+i);
			}
			(*((sll_array_length_t*)(&(sll_environment->l))))--;
			*((const sll_environment_variable_t*const**)(&(sll_environment->dt)))=sll_reallocate(PTR(sll_environment->dt),sll_environment->l*sizeof(sll_environment_variable_t*));
			goto _cleanup;
		}
		i++;
	}
_cleanup:
	if (sll_string_equal(&k_low,&_env_path_var_name)){
		_rebuild_global_search_path();
	}
	UNLOCK_ENV;
	sll_free_string(&k_low);
}



__SLL_EXTERNAL void sll_set_environment_variable(const sll_string_t* k,const sll_string_t* v){
	sll_string_t k_low;
	sll_string_lower_case(k,&k_low);
	LOCK_ENV;
	sll_platform_set_environment_variable(k->v,v->v);
	for (sll_array_length_t i=0;i<sll_environment->l;i++){
		sll_environment_variable_t* kv=(sll_environment_variable_t*)(*(sll_environment->dt+i));
		if (STRING_EQUAL(&k_low,&(kv->k))){
			sll_free_string((sll_string_t*)(&(kv->v)));
			sll_string_clone(v,(sll_string_t*)(&(kv->v)));
			goto _end;
		}
	}
	(*((sll_array_length_t*)(&(sll_environment->l))))++;
	*((const sll_environment_variable_t*const**)(&(sll_environment->dt)))=sll_reallocate(PTR(sll_environment->dt),sll_environment->l*sizeof(sll_environment_variable_t*));
	sll_environment_variable_t* n=sll_allocate(sizeof(sll_environment_variable_t));
	sll_string_clone(k,(sll_string_t*)(&(n->k)));
	sll_string_clone(v,(sll_string_t*)(&(n->v)));
	*(((const sll_environment_variable_t**)(sll_environment->dt))+sll_environment->l-1)=n;
_end:
	UNLOCK_ENV;
	if (sll_string_equal(&k_low,&_env_path_var_name)){
		_rebuild_global_search_path();
	}
	sll_free_string(&k_low);
}
