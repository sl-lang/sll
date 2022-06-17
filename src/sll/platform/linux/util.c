#include <sll/_internal/common.h>
#include <sll/_internal/platform.h>
#include <sll/_internal/static_string.h>
#include <sll/common.h>
#include <sll/types.h>
#include <dlfcn.h>
#include <errno.h>
#include <immintrin.h>
#include <signal.h>
#include <sys/random.h>
#include <termios.h>
#include <unistd.h>



static sll_cpu_t _linux_cpu=0;
static __STATIC_STRING(_linux_platform_str,
#ifdef __SLL_BUILD_DARWIN
	"darwin"
#else
	"linux"
#endif
);
#ifndef __SLL_BUILD_FUZZER
static unsigned int _linux_csr;
static struct termios _linux_stdin_cfg;
#endif



__SLL_EXTERNAL const sll_cpu_t* sll_platform_cpu_count=&_linux_cpu;
__SLL_EXTERNAL const sll_string_t* sll_platform_string=&_linux_platform_str;



#ifndef __SLL_BUILD_FUZZER
static void _exception_handler(int sig,siginfo_t* si,void* arg){
	_force_exit_platform();
}
#endif



void _deinit_platform(void){
#ifndef __SLL_BUILD_FUZZER
	_mm_setcsr(_linux_csr);
	tcsetattr(STDIN_FILENO,TCSANOW,&_linux_stdin_cfg);
#endif
}



__SLL_NO_RETURN void _force_exit_platform(void){
	_deinit_platform();
#ifdef DEBUG_BUILD
	raise(SIGABRT);
#endif
	_exit(SIGABRT);
}



void _init_platform(void){
	Dl_info fn_dt;
	SLL_ASSERT(dladdr(_init_platform,&fn_dt));
	dlclose(dlopen(fn_dt.dli_fname,RTLD_NOW|RTLD_GLOBAL|RTLD_NOLOAD));
#ifndef __SLL_BUILD_FUZZER
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
#endif
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
