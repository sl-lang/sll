#include <windows.h>
#include <sll/_sll_internal.h>
#include <sll/common.h>
#include <sll/init.h>
#include <sll/types.h>
#include <fcntl.h>
#include <io.h>
#include <stdio.h>



static int _win_stdout_m=0;
static int _win_stderr_m=0;
static DWORD _win_stdout_cm=0xffffffff;
static DWORD _win_stderr_cm=0xffffffff;



static void _reset_console_color(void){
	if (_win_stdout_cm!=0xffffffff){
		SetConsoleMode(GetStdHandle(STD_OUTPUT_HANDLE),_win_stdout_cm);
		_win_stdout_cm=0xffffffff;
	}
	if (_win_stderr_cm!=0xffffffff){
		SetConsoleMode(GetStdHandle(STD_ERROR_HANDLE),_win_stderr_cm);
		_win_stderr_cm=0xffffffff;
	}
}



__SLL_EXTERNAL void sll_platform_create_console(void){
	if (!AttachConsole(ATTACH_PARENT_PROCESS)){
		AllocConsole();
	}
	if (!freopen("CONIN$","r",stdin)||!freopen("CONOUT$","w",stderr)||!freopen("CONOUT$","w",stdout)){
		SLL_UNIMPLEMENTED();
	}
	_file_release_std_streams();
	_file_init_std_streams();
}



__SLL_EXTERNAL void sll_platform_enable_console_color(void){
	sll_bool_t st=0;
	if (_win_stdout_cm==0xffffffff&&_isatty(1)){
		GetConsoleMode(GetStdHandle(STD_OUTPUT_HANDLE),&_win_stdout_cm);
		SetConsoleMode(GetStdHandle(STD_OUTPUT_HANDLE),_win_stdout_cm|ENABLE_PROCESSED_OUTPUT|ENABLE_WRAP_AT_EOL_OUTPUT|ENABLE_VIRTUAL_TERMINAL_PROCESSING);
		st=1;
	}
	if (_win_stderr_cm==0xffffffff&&_isatty(2)){
		GetConsoleMode(GetStdHandle(STD_ERROR_HANDLE),&_win_stderr_cm);
		SetConsoleMode(GetStdHandle(STD_ERROR_HANDLE),_win_stderr_cm|ENABLE_PROCESSED_OUTPUT|ENABLE_WRAP_AT_EOL_OUTPUT|ENABLE_VIRTUAL_TERMINAL_PROCESSING);
		st=1;
	}
	if (st){
		sll_register_cleanup(_reset_console_color);
	}
}
