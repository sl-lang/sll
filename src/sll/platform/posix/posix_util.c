#include <sll/_sll_internal.h>
#include <sll/common.h>
#include <sll/data.h>
#include <sll/memory.h>
#include <sll/string.h>
#include <sll/types.h>
#include <sll/util.h>
#include <dlfcn.h>
#include <errno.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/random.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>



static sll_environment_t _posix_env={NULL,0};
static __STATIC_STRING(_posix_platform_str,"posix");



__SLL_EXTERNAL const sll_environment_t* sll_environment=&_posix_env;
__SLL_EXTERNAL const sll_string_t* sll_platform_string=&_posix_platform_str;



extern char** environ;



static void _cleanup_env_data(void){
	for (sll_array_length_t i=0;i<_posix_env.l;i++){
		const sll_environment_variable_t* kv=*(_posix_env.dt+i);
		sll_free_string((sll_string_t*)(&(kv->k)));
		sll_free_string((sll_string_t*)(&(kv->v)));
		sll_deallocate((void*)kv);
	}
	*((sll_array_length_t*)(&(_posix_env.l)))=0;
	_posix_env.dt=NULL;
	sll_deallocate((void*)(_posix_env.dt));
}



void _init_platform(void){
	Dl_info fn_dt;
	SLL_ASSERT(dladdr(_init_platform,&fn_dt));
	dlclose(dlopen(fn_dt.dli_fname,RTLD_NOW|RTLD_GLOBAL|RTLD_NOLOAD));
	sll_array_length_t l=0;
	char** dt=environ;
	while (*dt){
		l++;
		dt++;
	}
	_posix_env.dt=sll_allocate(l*sizeof(sll_environment_variable_t*));
	sll_array_length_t i=0;
	dt=environ;
	while (*dt){
		sll_char_t* e=SLL_CHAR(*dt);
		dt++;
		if (*e=='='){
			continue;
		}
		sll_string_index_t j=0;
		while (*e&&*e!='='){
			j++;
			e++;
		}
		if (!(*e)){
			continue;
		}
		sll_environment_variable_t* n=sll_allocate(sizeof(sll_environment_variable_t));
		sll_string_from_pointer_length(e-j,j,(sll_string_t*)(&(n->k)));
		sll_string_from_pointer(e+1,(sll_string_t*)(&(n->v)));
		*(((const sll_environment_variable_t**)(_posix_env.dt))+i)=n;
		i++;
	}
	if (i!=l){
		l=i;
		_posix_env.dt=sll_reallocate((const sll_environment_variable_t**)(_posix_env.dt),l*sizeof(sll_environment_variable_t*));
	}
	*((sll_array_length_t*)(&(_posix_env.l)))=l;
	sll_register_cleanup(_cleanup_env_data);
}



__SLL_NO_RETURN void _force_exit_platform(void){
	_exit(SIGABRT);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_time_t sll_platform_get_current_time(void){
	struct timespec tm;
	clock_gettime(CLOCK_REALTIME,&tm);
	return tm.tv_sec*1000000000+tm.tv_nsec;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_size_t sll_platform_get_cpu_count(void){
	return sysconf(_SC_NPROCESSORS_ONLN);
}



__SLL_EXTERNAL void sll_platform_random(void* bf,sll_size_t l){
	while (l){
		ssize_t n=getrandom(bf,l,0);
		if (n==-1){
			if (errno==EINTR){
				continue;
			}
			sll_set_memory(bf,l,0);
			return;
		}
		l-=n;
		bf=(void*)(((uint64_t)bf)+n);
	}
}



__SLL_EXTERNAL void sll_platform_remove_environment_variable(const sll_char_t* k){
	unsetenv((char*)k);
}



__SLL_EXTERNAL void sll_platform_set_environment_variable(const sll_char_t* k,const sll_char_t* v){
	setenv((char*)k,(char*)v,1);
}



__SLL_EXTERNAL void sll_platform_sleep(sll_time_t tm){
	struct timespec ts;
	clock_gettime(CLOCK_REALTIME,&ts);
	sll_time_t e=ts.tv_sec*1000000000+ts.tv_nsec+tm;
	while (1){
		struct timeval tv;
		tv.tv_sec=tm/1000000000;
		tv.tv_usec=(tm/1000)%1000000;
		if (!select(0,NULL,NULL,NULL,&tv)){
			return;
		}
		clock_gettime(CLOCK_REALTIME,&ts);
		sll_time_t c=ts.tv_sec*1000000000+ts.tv_nsec;
		if (c>=e){
			break;
		}
		tm=c-e;
	}
}
