#include <sll/_sll_internal.h>
#include <sll/api/date.h>
#include <sll/common.h>
#include <sll/data.h>
#include <sll/init.h>
#include <sll/memory.h>
#include <sll/string.h>
#include <sll/types.h>
#include <dlfcn.h>
#include <errno.h>
#include <immintrin.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/random.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>



static sll_environment_t _linux_env={NULL,0};
static __STATIC_STRING(_linux_platform_str,
#ifdef __SLL_BUILD_DARWIN
	"darwin"
#else
	"linux"
#endif
);
static sll_time_zone_t _linux_platform_time_zone={"GMT",0};
static unsigned int _linux_csr;



__SLL_EXTERNAL const sll_environment_t* sll_environment=&_linux_env;
__SLL_EXTERNAL const sll_string_t* sll_platform_string=&_linux_platform_str;
__SLL_EXTERNAL const sll_time_zone_t* sll_platform_time_zone=&_linux_platform_time_zone;



extern char** environ;
extern int daylight;
extern long timezone;
extern char* tzname[2];



static void _cleanup_env_data(void){
	for (sll_array_length_t i=0;i<_linux_env.l;i++){
		const sll_environment_variable_t* kv=*(_linux_env.dt+i);
		sll_free_string((sll_string_t*)(&(kv->k)));
		sll_free_string((sll_string_t*)(&(kv->v)));
		sll_deallocate(PTR(kv));
	}
	*((sll_array_length_t*)(&(_linux_env.l)))=0;
	_linux_env.dt=NULL;
	sll_deallocate(PTR(_linux_env.dt));
	_linux_platform_time_zone=*sll_utc_time_zone;
	_mm_setcsr(_linux_csr);
}



__SLL_NO_RETURN void _force_exit_platform(void){
	_exit(SIGABRT);
}



void _init_platform(void){
	_linux_csr=_mm_getcsr();
	_mm_setcsr(_linux_csr|CSR_REGISTER_FLAGS);
	Dl_info fn_dt;
	SLL_ASSERT(dladdr(_init_platform,&fn_dt));
	dlclose(dlopen(fn_dt.dli_fname,RTLD_NOW|RTLD_GLOBAL|RTLD_NOLOAD));
	sll_array_length_t l=0;
	char** dt=environ;
	while (*dt){
		l++;
		dt++;
	}
	_linux_env.dt=sll_allocate(l*sizeof(sll_environment_variable_t*));
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
		*(((const sll_environment_variable_t**)(_linux_env.dt))+i)=n;
		i++;
	}
	if (i!=l){
		l=i;
		_linux_env.dt=sll_reallocate((const sll_environment_variable_t**)(_linux_env.dt),l*sizeof(sll_environment_variable_t*));
	}
	*((sll_array_length_t*)(&(_linux_env.l)))=l;
	sll_register_cleanup(_cleanup_env_data);
	tzset();
	const sll_char_t* nm=SLL_CHAR(tzname[!!daylight]);
	sll_string_length_t sz=sll_string_length_unaligned(nm);
	if (sz>31){
		sz=31;
	}
	sll_copy_data(nm,sz,_linux_platform_time_zone.nm);
	_linux_platform_time_zone.nm[sz]=0;
	_linux_platform_time_zone.off=-timezone/60;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_size_t sll_platform_get_cpu_count(void){
	return sysconf(_SC_NPROCESSORS_ONLN);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_time_t sll_platform_get_current_time(void){
	struct timespec tm;
	clock_gettime(CLOCK_REALTIME,&tm);
	return tm.tv_sec*1000000000+tm.tv_nsec;
}



__SLL_EXTERNAL void sll_platform_random(void* bf,sll_size_t l){
	while (l){
#ifdef __SLL_BUILD_DARWIN
		sll_size_t n=(l>256?256:l);
		if (getentropy(bf,n)<0){
#else
		ssize_t n=getrandom(bf,l,0);
		if (n==-1){
#endif
			if (errno==EINTR){
				continue;
			}
			sll_set_memory(bf,l,0);
			return;
		}
		l-=n;
		bf=PTR(ADDR(bf)+n);
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
