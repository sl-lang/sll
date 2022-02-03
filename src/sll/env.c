#include <sll/_sll_internal.h>
#include <sll/common.h>
#include <sll/memory.h>
#include <sll/platform.h>
#include <sll/string.h>
#include <sll/types.h>



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_get_environment_variable(const sll_string_t* k,sll_string_t* o){
	for (sll_array_length_t i=0;i<sll_environment->l;i++){
		const sll_environment_variable_t* kv=*(sll_environment->dt+i);
		if (sll_string_equal(&(kv->k),k)){
			if (o){
				sll_string_clone(&(kv->v),o);
			}
			return 1;
		}
	}
	return 0;
}



__SLL_EXTERNAL void sll_remove_environment_variable(const sll_string_t* k){
	sll_array_length_t i=0;
	while (i<sll_environment->l){
		sll_environment_variable_t* kv=(sll_environment_variable_t*)(*(sll_environment->dt+i));
		if (sll_string_equal(k,&(kv->k))){
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
			return;
		}
		i++;
	}
}



__SLL_EXTERNAL void sll_set_environment_variable(const sll_string_t* k,const sll_string_t* v){
	sll_platform_set_environment_variable(k->v,v->v);
	for (sll_array_length_t i=0;i<sll_environment->l;i++){
		sll_environment_variable_t* kv=(sll_environment_variable_t*)(*(sll_environment->dt+i));
		if (sll_string_equal(k,&(kv->k))){
			sll_free_string((sll_string_t*)(&(kv->v)));
			sll_string_clone(v,(sll_string_t*)(&(kv->v)));
			return;
		}
	}
	(*((sll_array_length_t*)(&(sll_environment->l))))++;
	*((const sll_environment_variable_t*const**)(&(sll_environment->dt)))=sll_reallocate(PTR(sll_environment->dt),sll_environment->l*sizeof(sll_environment_variable_t*));
	sll_environment_variable_t* n=sll_allocate(sizeof(sll_environment_variable_t));
	sll_string_clone(k,(sll_string_t*)(&(n->k)));
	sll_string_clone(v,(sll_string_t*)(&(n->v)));
	*(((const sll_environment_variable_t**)(sll_environment->dt))+sll_environment->l-1)=n;
}
