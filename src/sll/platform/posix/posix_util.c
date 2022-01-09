#include <sll/_sll_internal.h>
#include <sll/common.h>
#include <sll/string.h>
#include <sll/types.h>
#include <sll/util.h>
#include <dlfcn.h>
#include <errno.h>
#include <signal.h>
#include <sys/random.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>



__SLL_EXTERNAL const sll_char_t* sll_platform_string=SLL_CHAR("posix");



void _fix_load_mode(void){
	Dl_info dt;
	SLL_ASSERT(dladdr(_fix_load_mode,&dt));
	dlclose(dlopen(dt.dli_fname,RTLD_NOW|RTLD_GLOBAL|RTLD_NOLOAD));
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
