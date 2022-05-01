#include <sll/_internal/common.h>
#include <sll/_internal/platform.h>
#include <sll/_internal/static_string.h>
#include <sll/common.h>
#include <sll/environment.h>
#include <sll/memory.h>
#include <sll/string.h>
#include <sll/types.h>
#include <dlfcn.h>
#include <errno.h>
#include <immintrin.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/random.h>
#include <termios.h>
#include <unistd.h>



static sll_cpu_t _linux_cpu=0;
static sll_environment_t _linux_env={NULL,0};
static __STATIC_STRING(_linux_platform_str,
#ifdef __SLL_BUILD_DARWIN
	"darwin"
#else
	"linux"
#endif
);
static unsigned int _linux_csr;
static struct termios _linux_stdin_cfg;



__SLL_EXTERNAL const sll_cpu_t* sll_platform_cpu_count=&_linux_cpu;
__SLL_EXTERNAL const sll_environment_t* sll_environment=&_linux_env;
__SLL_EXTERNAL const sll_string_t* sll_platform_string=&_linux_platform_str;



static void _reset_critical(void){
	_mm_setcsr(_linux_csr);
	tcsetattr(STDIN_FILENO,TCSANOW,&_linux_stdin_cfg);
}



static void _exception_handler(int sig,siginfo_t* si,void* arg){
	_force_exit_platform();
}



void _deinit_platform(void){
	for (sll_environment_length_t i=0;i<_linux_env.l;i++){
		const sll_environment_variable_t* kv=*(_linux_env.dt+i);
		sll_free_string((sll_string_t*)(&(kv->k)));
		sll_free_string((sll_string_t*)(&(kv->v)));
		sll_deallocate(PTR(kv));
	}
	*((sll_environment_length_t*)(&(_linux_env.l)))=0;
	sll_deallocate(PTR(_linux_env.dt));
	_linux_env.dt=NULL;
	_reset_critical();
}



__SLL_NO_RETURN void _force_exit_platform(void){
	_reset_critical();
#ifdef DEBUG_BUILD
	raise(SIGABRT);
#endif
	_exit(SIGABRT);
}



void _init_platform(void){
	Dl_info fn_dt;
	SLL_ASSERT(dladdr(_init_platform,&fn_dt));
	dlclose(dlopen(fn_dt.dli_fname,RTLD_NOW|RTLD_GLOBAL|RTLD_NOLOAD));
	_linux_csr=_mm_getcsr();
	_mm_setcsr(_linux_csr|CSR_REGISTER_FLAGS);
	if (!tcgetattr(STDIN_FILENO,&_linux_stdin_cfg)){
		struct termios bf=_linux_stdin_cfg;
		bf.c_iflag=(bf.c_iflag&(~INLCR))|ICRNL;
		bf.c_lflag=(bf.c_lflag&(~(ICANON|ICRNL|ECHO|ECHOCTL|ECHOPRT)))|ISIG;
		tcsetattr(STDIN_FILENO,TCSANOW,&bf);
	}
	struct sigaction sa={
		.sa_sigaction=_exception_handler,
		.sa_flags=SA_SIGINFO|SA_NODEFER|SA_RESTART
	};
	sigaction(SIGSEGV,&sa,NULL);
	sll_environment_length_t l=0;
	char** dt=environ;
	while (*dt){
		l++;
		dt++;
	}
	_linux_env.dt=sll_allocate(l*sizeof(sll_environment_variable_t*));
	sll_environment_length_t i=0;
	dt=environ;
	while (*dt){
		sll_char_t* e=SLL_CHAR(*dt);
		dt++;
		if (*e=='='){
			continue;
		}
		sll_string_length_t j=0;
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
	*((sll_environment_length_t*)(&(_linux_env.l)))=l;
	_linux_cpu=sysconf(_SC_NPROCESSORS_ONLN);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_error_t sll_platform_get_error(void){
	return errno|SLL_ERROR_FLAG_SYSTEM;
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
