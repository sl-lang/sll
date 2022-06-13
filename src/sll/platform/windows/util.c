#include <windows.h>
#include <bcrypt.h>
#include <immintrin.h>
#include <io.h>
#include <signal.h>
#include <sll/_internal/common.h>
#include <sll/_internal/platform.h>
#include <sll/_internal/static_string.h>
#include <sll/_size_types.h>
#include <sll/common.h>
#include <sll/error.h>
#include <sll/types.h>



static sll_cpu_t _win_cpu=0;
static __STATIC_STRING(_win_platform_str,"windows");
static unsigned int _win_csr=0;
static DWORD _win_stdin_cm;
static DWORD _win_stdout_cm;
static DWORD _win_stderr_cm;



void* _win_dll_handle=NULL;



__SLL_EXTERNAL const sll_cpu_t* sll_platform_cpu_count=&_win_cpu;
__SLL_EXTERNAL const sll_string_t* sll_platform_string=&_win_platform_str;



void _deinit_platform(void){
	_mm_setcsr(_win_csr);
	SetConsoleMode(GetStdHandle(STD_INPUT_HANDLE),_win_stdin_cm);
	SetConsoleMode(GetStdHandle(STD_OUTPUT_HANDLE),_win_stdout_cm);
	SetConsoleMode(GetStdHandle(STD_ERROR_HANDLE),_win_stderr_cm);
}



__SLL_NO_RETURN void _force_exit_platform(void){
	TerminateProcess(GetCurrentProcess(),SIGABRT);
}



void _init_platform(void){
	HANDLE h=GetStdHandle(STD_INPUT_HANDLE);
	GetConsoleMode(h,&_win_stdin_cm);
	SetConsoleMode(h,_win_stdin_cm&(~(ENABLE_LINE_INPUT|ENABLE_ECHO_INPUT)));
	h=GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleMode(h,&_win_stdout_cm);
	SetConsoleMode(h,_win_stdout_cm|ENABLE_PROCESSED_OUTPUT|ENABLE_WRAP_AT_EOL_OUTPUT|ENABLE_VIRTUAL_TERMINAL_PROCESSING);
	h=GetStdHandle(STD_ERROR_HANDLE);
	GetConsoleMode(h,&_win_stderr_cm);
	SetConsoleMode(h,_win_stderr_cm|ENABLE_PROCESSED_OUTPUT|ENABLE_WRAP_AT_EOL_OUTPUT|ENABLE_VIRTUAL_TERMINAL_PROCESSING);
	_win_csr=_mm_getcsr();
	_mm_setcsr(_win_csr|CSR_REGISTER_FLAGS);
	SYSTEM_INFO si;
	GetSystemInfo(&si);
	_win_cpu=(sll_cpu_t)si.dwNumberOfProcessors;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_error_t sll_platform_get_error(void){
	sll_error_t err=(GetLastError()<<9)|SLL_ERROR_FLAG_SYSTEM;
	switch (GetLastError()){
		case ERROR_FILE_NOT_FOUND:
		case ERROR_PATH_NOT_FOUND:
		case ERROR_INVALID_DRIVE:
		case ERROR_NO_MORE_FILES:
		case ERROR_MOD_NOT_FOUND:
		case ERROR_BAD_PATHNAME:
		case ERROR_FILENAME_EXCED_RANGE:
			return ENOENT|err;
		case ERROR_BAD_ENVIRONMENT:
			return E2BIG|err;
		case ERROR_BAD_FORMAT:
		case ERROR_INVALID_STARTING_CODESEG:
		case ERROR_INVALID_STACKSEG:
		case ERROR_INVALID_MODULETYPE:
		case ERROR_INVALID_EXE_SIGNATURE:
		case ERROR_EXE_MARKED_INVALID:
		case ERROR_BAD_EXE_FORMAT:
		case ERROR_ITERATED_DATA_EXCEEDS_64k:
		case ERROR_INVALID_MINALLOCSIZE:
		case ERROR_INFLOOP_IN_RELOC_CHAIN:
			return ENOEXEC|err;
		case ERROR_INVALID_HANDLE:
		case ERROR_INVALID_TARGET_HANDLE:
		case ERROR_DIRECT_ACCESS_HANDLE:
			return EBADF|err;
		case ERROR_WAIT_NO_CHILDREN:
		case ERROR_CHILD_NOT_COMPLETE:
			return ECHILD|err;
		case ERROR_NO_PROC_SLOTS:
		case ERROR_MAX_THRDS_REACHED:
		case ERROR_NESTING_NOT_ALLOWED:
			return EAGAIN|err;
		case ERROR_ARENA_TRASHED:
		case ERROR_NOT_ENOUGH_MEMORY:
		case ERROR_INVALID_BLOCK:
			return ENOMEM|err;
		case ERROR_ACCESS_DENIED:
		case ERROR_CURRENT_DIRECTORY:
		case ERROR_WRITE_PROTECT:
		case ERROR_BAD_UNIT:
		case ERROR_NOT_READY:
		case ERROR_BAD_COMMAND:
		case ERROR_CRC:
		case ERROR_BAD_LENGTH:
		case ERROR_SEEK:
		case ERROR_NOT_DOS_DISK:
		case ERROR_SECTOR_NOT_FOUND:
		case ERROR_OUT_OF_PAPER:
		case ERROR_WRITE_FAULT:
		case ERROR_READ_FAULT:
		case ERROR_GEN_FAILURE:
		case ERROR_SHARING_VIOLATION:
		case ERROR_LOCK_VIOLATION:
		case ERROR_WRONG_DISK:
		case ERROR_SHARING_BUFFER_EXCEEDED:
		case ERROR_NETWORK_ACCESS_DENIED:
		case ERROR_CANNOT_MAKE:
		case ERROR_FAIL_I24:
		case ERROR_DRIVE_LOCKED:
		case ERROR_SEEK_ON_DEVICE:
		case ERROR_NOT_LOCKED:
		case ERROR_LOCK_FAILED:
			return EACCES|err;
		case ERROR_FILE_EXISTS:
		case ERROR_ALREADY_EXISTS:
			return EEXIST|err;
		case ERROR_NOT_SAME_DEVICE:
			return EXDEV|err;
		case ERROR_DIRECTORY:
			return ENOTDIR|err;
		case ERROR_TOO_MANY_OPEN_FILES:
			return EMFILE|err;
		case ERROR_DISK_FULL:
			return ENOSPC|err;
		case ERROR_BROKEN_PIPE:
		case ERROR_NO_DATA:
			return EPIPE|err;
		case ERROR_DIR_NOT_EMPTY:
			return ENOTEMPTY|err;
		case ERROR_INVALID_FUNCTION:
		case ERROR_INVALID_ACCESS:
		case ERROR_INVALID_DATA:
		case ERROR_INVALID_PARAMETER:
		case ERROR_NEGATIVE_SEEK:
			return EINVAL|err;
		default:
			return SLL_UNMAPPED_SYSTEM_ERROR|err;
	}
}



__SLL_EXTERNAL void sll_platform_random(void* bf,sll_size_t l){
	while (l){
		ULONG n=(l>__SLL_U32_MAX?__SLL_U32_MAX:(ULONG)l);
		if (BCryptGenRandom(NULL,bf,n,BCRYPT_USE_SYSTEM_PREFERRED_RNG)<0){
			sll_set_memory(bf,l,0);
		}
		l-=n;
		bf=PTR(ADDR(bf)+n);
	}
}



BOOL WINAPI DllMain(HINSTANCE dll_h,DWORD t,LPVOID r){
	if (!_win_dll_handle){
		_win_dll_handle=dll_h;
	}
	return TRUE;
}
