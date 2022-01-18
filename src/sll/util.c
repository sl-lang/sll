#include <sll/_sll_internal.h>
#include <sll/assembly.h>
#include <sll/common.h>
#include <sll/file.h>
#include <sll/gc.h>
#include <sll/memory.h>
#include <sll/object.h>
#include <sll/platform.h>
#include <sll/string.h>
#include <sll/types.h>
#include <sll/util.h>



STATIC_STRING_SETUP;



static sll_cleanup_function_t _util_exit_table[MAX_CLEANUP_TABLE_SIZE];
static sll_bool_t _util_init=0;
static unsigned int _util_exit_table_size=0;
static sll_sandbox_flags_t _util_sandbox_flags=0;



__SLL_NO_RETURN void _force_exit(const sll_char_t* a,const sll_char_t* b,const sll_char_t* c){
	sll_file_flush(sll_stdout);
	sll_file_flush(sll_stderr);
	sll_file_descriptor_t fd=sll_platform_get_default_stream_descriptor(SLL_PLATFORM_STREAM_ERROR);
	sll_platform_file_write(fd,a,sll_string_length_unaligned(a));
	sll_platform_file_write(fd,b,sll_string_length_unaligned(b));
	sll_platform_file_write(fd,c,sll_string_length_unaligned(c));
	_force_exit_platform();
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_string_index_t sll_add_string(sll_string_table_t* st,sll_string_t* s,sll_bool_t d){
	for (sll_string_index_t i=0;i<st->l;i++){
		if (sll_string_equal(st->dt+i,s)){
			if (d){
				sll_free_string(s);
			}
			return i;
		}
	}
	st->l++;
	st->dt=sll_reallocate(st->dt,st->l*sizeof(sll_string_t));
	*(st->dt+st->l-1)=*s;
	return st->l-1;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_string_index_t sll_add_string_object(sll_string_table_t* st,sll_object_t* v){
	SLL_ASSERT(SLL_OBJECT_GET_TYPE(v)==SLL_OBJECT_TYPE_STRING);
	for (sll_string_index_t i=0;i<st->l;i++){
		if (sll_string_equal(st->dt+i,&(v->dt.s))){
			return i;
		}
	}
	st->l++;
	st->dt=sll_reallocate(st->dt,st->l*sizeof(sll_string_t));
	v->t|=OBJECT_EXTERNAL_STRING;
	*(st->dt+st->l-1)=v->dt.s;
	return st->l-1;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_string_index_t sll_create_string(sll_string_table_t* st,const sll_char_t* dt,sll_string_length_t l){
	sll_string_t n;
	sll_string_from_pointer_length(dt,l,&n);
	for (sll_string_index_t i=0;i<st->l;i++){
		if (sll_string_equal(st->dt+i,&n)){
			sll_free_string(&n);
			return i;
		}
	}
	st->l++;
	st->dt=sll_reallocate(st->dt,st->l*sizeof(sll_string_t));
	*(st->dt+st->l-1)=n;
	return st->l-1;
}


__SLL_EXTERNAL void sll_deinit(void){
	if (!_util_init){
		return;
	}
	while (_util_exit_table_size){
		_util_exit_table_size--;
		_util_exit_table[_util_exit_table_size]();
	}
	const static_string_t*const* l=&__s_str_start;
	while (l<&__s_str_end){
		const static_string_t* k=*l;
		if (k){
			sll_free_string(k->p);
		}
		l++;
	}
	_gc_release_data();
	_file_release_std_streams();
	_log_release_data();
	_memory_release_data();
	sll_platform_reset_console();
	_util_sandbox_flags=0;
	_util_init=0;
}



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



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_file_offset_t sll_get_location(const sll_assembly_data_t* a_dt,sll_instruction_index_t ii,sll_string_index_t* fp,sll_string_index_t* fn){
	if (ii>=a_dt->ic){
		*fp=0;
		return 0;
	}
	sll_instruction_index_t c=0;
	sll_file_offset_t o_ln=0;
	sll_string_index_t o_fn=SLL_MAX_STRING_INDEX;
	sll_string_index_t o_fp=0;
	for (sll_instruction_index_t i=0;i<a_dt->dbg.l;i++){
		c+=(a_dt->dbg.dt+i)->ii;
		if (c>ii){
			break;
		}
		if ((a_dt->dbg.dt+i)->ln&SLL_DEBUG_LINE_DATA_FLAG_FILE){
			o_fp=(sll_string_index_t)SLL_DEBUG_LINE_DATA_GET_DATA(a_dt->dbg.dt+i);
		}
		else if ((a_dt->dbg.dt+i)->ln&SLL_DEBUG_LINE_DATA_FLAG_FUNC){
			o_fn=(sll_string_index_t)SLL_DEBUG_LINE_DATA_GET_DATA(a_dt->dbg.dt+i);
		}
		else{
			o_ln=SLL_DEBUG_LINE_DATA_GET_DATA(a_dt->dbg.dt+i);
		}
	}
	*fp=o_fp;
	*fn=o_fn;
	return o_ln+1;
}


__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_get_sandbox_flag(sll_sandbox_flags_t f){
	return !!(_util_sandbox_flags&f);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_sandbox_flags_t sll_get_sandbox_flags(void){
	return _util_sandbox_flags;
}



__SLL_EXTERNAL void sll_init(void){
	if (_util_init){
		return;
	}
	sll_platform_setup_console();
	_file_init_std_streams();
	_init_platform();
	const static_string_t*const* l=&__s_str_start;
	while (l<&__s_str_end){
		const static_string_t* k=*l;
		if (k){
			if (k->dtl==SLL_MAX_STRING_LENGTH){
				k->dt.fn(k->p);
			}
			else{
				sll_string_from_pointer_length(k->dt.s,k->dtl,k->p);
			}
		}
		l++;
	}
}



__SLL_EXTERNAL void sll_remove_environment_variable(const sll_string_t* k){
	for (sll_array_length_t i=0;i<sll_environment->l;i++){
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
			*((const sll_environment_variable_t*const**)(&(sll_environment->dt)))=sll_reallocate((void*)(sll_environment->dt),sll_environment->l*sizeof(sll_environment_variable_t*));
			return;
		}
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
	*((const sll_environment_variable_t*const**)(&(sll_environment->dt)))=sll_reallocate((void*)(sll_environment->dt),sll_environment->l*sizeof(sll_environment_variable_t*));
	sll_environment_variable_t* n=sll_allocate(sizeof(sll_environment_variable_t));
	sll_string_clone(k,(sll_string_t*)(&(n->k)));
	sll_string_clone(v,(sll_string_t*)(&(n->v)));
	*(((const sll_environment_variable_t**)(sll_environment->dt))+sll_environment->l-1)=n;
}



__SLL_EXTERNAL void sll_register_cleanup(sll_cleanup_function_t f){
	SLL_ASSERT(_util_exit_table_size<MAX_CLEANUP_TABLE_SIZE);
	_util_exit_table[_util_exit_table_size]=f;
	_util_exit_table_size++;
}



__SLL_EXTERNAL sll_sandbox_flags_t sll_set_sandbox_flags(sll_sandbox_flags_t f){
	_util_sandbox_flags|=f;
	return _util_sandbox_flags;
}
