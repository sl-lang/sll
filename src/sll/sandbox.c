#include <sll/_sll_internal.h>
#include <sll/array.h>
#include <sll/common.h>
#include <sll/gc.h>
#include <sll/object.h>
#include <sll/sandbox.h>
#include <sll/types.h>



#define FLAG(f,nm) __STATIC_STRING(__flag_str_##f,(nm))
#define CHECK_FLAG(f) \
	if (_util_sandbox_flags&(1ull<<SLL_SANDBOX_FLAG_##f)){ \
		o->v[i]=SLL_CREATE(); \
		o->v[i]->t=SLL_OBJECT_TYPE_STRING; \
		sll_string_clone(&__flag_str_##f,&(o->v[i]->dt.s)); \
		i++; \
	}



FLAG(DISABLE_FILE_IO,"no-file-io");
FLAG(ENABLE_STDIN_IO,"stdin-io");
FLAG(ENABLE_STDOUT_IO,"stdout-io");
FLAG(DISABLE_PATH_API,"no-path-api");
FLAG(DISABLE_PROCESS_API,"no-process-api");
FLAG(DISABLE_ENVIRONMENT,"no-environment");
FLAG(DISABLE_LOAD_LIBRARY,"no-load-library");
FLAG(ENABLE_BUFFER_FILES,"buffer-files");
FLAG(ENABLE_FILE_RENAME,"file-rename");
FLAG(ENABLE_FILE_COPY,"file-copy");
static wide_data_t _util_sandbox_flags=0;



void _reset_sandbox(void){
	_util_sandbox_flags=0;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_get_sandbox_flag(sll_sandbox_flags_t f){
	return !!(_util_sandbox_flags&(1ull<<f));
}



__SLL_EXTERNAL void sll_get_sandbox_flags(sll_array_t* o){
	sll_array_length_t l=(sll_array_length_t)POPULATION_COUNT(_util_sandbox_flags);
	if (!sll_array_create(l,o)){
		SLL_UNIMPLEMENTED();
	}
	sll_array_length_t i=0;
	CHECK_FLAG(DISABLE_FILE_IO);
	CHECK_FLAG(ENABLE_STDIN_IO);
	CHECK_FLAG(ENABLE_STDOUT_IO);
	CHECK_FLAG(DISABLE_PATH_API);
	CHECK_FLAG(DISABLE_PROCESS_API);
	CHECK_FLAG(DISABLE_ENVIRONMENT);
	CHECK_FLAG(DISABLE_LOAD_LIBRARY);
	CHECK_FLAG(ENABLE_BUFFER_FILES);
	CHECK_FLAG(ENABLE_FILE_RENAME);
	CHECK_FLAG(ENABLE_FILE_COPY);
	SLL_ASSERT(i==l);
}



__SLL_EXTERNAL void sll_set_sandbox_flag(sll_sandbox_flags_t f){
	_util_sandbox_flags|=1ull<<f;
}
