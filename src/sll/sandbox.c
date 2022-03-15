#include <sll/_internal/intrinsics.h>
#include <sll/_internal/sandbox.h>
#include <sll/_internal/scheduler.h>
#include <sll/_internal/static_string.h>
#include <sll/array.h>
#include <sll/common.h>
#include <sll/gc.h>
#include <sll/object.h>
#include <sll/sandbox.h>
#include <sll/thread.h>
#include <sll/types.h>



#define FLAG(f,nm) __STATIC_STRING(__flag_str_##f,(nm))
#define CHECK_FLAG(f) \
	if (fl&(1ull<<SLL_SANDBOX_FLAG_##f)){ \
		o->v[i]=sll_create_object(); \
		o->v[i]->t=SLL_OBJECT_TYPE_STRING; \
		sll_string_clone(&__flag_str_##f,&(o->v[i]->dt.s)); \
		i++; \
	}
#define CHECK_SET_FLAG(f) \
	if (sll_string_equal(&__flag_str_##f,nm)){ \
		sll_set_sandbox_flag(SLL_SANDBOX_FLAG_##f); \
		return 1; \
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
FLAG(DISABLE_FLOAT_COMPARE_ERROR_CHANGE,"no-float-error-change");
FLAG(ENABLE_FILE_DELETE,"file-delete");



sandbox_t _sandbox_flags=0;



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_get_sandbox_flag(sll_sandbox_flag_t f){
	return !!((sll_current_thread_index==SLL_UNKNOWN_THREAD_INDEX?_sandbox_flags:_scheduler_current_thread->sandbox)&(1ull<<f));
}



__SLL_EXTERNAL void sll_get_sandbox_flags(sll_array_t* o){
	sandbox_t fl=(sll_current_thread_index==SLL_UNKNOWN_THREAD_INDEX?_sandbox_flags:_scheduler_current_thread->sandbox);
	sll_array_length_t l=(sll_array_length_t)POPULATION_COUNT(fl);
	sll_array_create(l,o);
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
	CHECK_FLAG(DISABLE_FLOAT_COMPARE_ERROR_CHANGE);
	CHECK_FLAG(ENABLE_FILE_DELETE);
	SLL_ASSERT(i==l);
}



__SLL_EXTERNAL void sll_set_sandbox_flag(sll_sandbox_flag_t f){
	if (sll_current_thread_index==SLL_UNKNOWN_THREAD_INDEX){
		_sandbox_flags|=1ull<<f;
	}
	else{
		_scheduler_current_thread->sandbox|=1ull<<f;
	}
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_set_sandbox_flag_string(const sll_string_t* nm){
	CHECK_SET_FLAG(DISABLE_FILE_IO);
	CHECK_SET_FLAG(ENABLE_STDIN_IO);
	CHECK_SET_FLAG(ENABLE_STDOUT_IO);
	CHECK_SET_FLAG(DISABLE_PATH_API);
	CHECK_SET_FLAG(DISABLE_PROCESS_API);
	CHECK_SET_FLAG(DISABLE_ENVIRONMENT);
	CHECK_SET_FLAG(DISABLE_LOAD_LIBRARY);
	CHECK_SET_FLAG(ENABLE_BUFFER_FILES);
	CHECK_SET_FLAG(ENABLE_FILE_RENAME);
	CHECK_SET_FLAG(ENABLE_FILE_COPY);
	CHECK_SET_FLAG(DISABLE_FLOAT_COMPARE_ERROR_CHANGE);
	CHECK_SET_FLAG(ENABLE_FILE_DELETE);
	return 0;
}
