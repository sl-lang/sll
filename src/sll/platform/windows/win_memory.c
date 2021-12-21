#include <windows.h>
#include <sll/_sll_internal.h>
#include <sll/common.h>
#include <sll/platform.h>
#include <sll/types.h>
#pragma comment(lib,"advapi32.lib")



static char _win_large_page=0;



__SLL_EXTERNAL __SLL_CHECK_OUTPUT void* sll_platform_allocate_page(sll_size_t sz,sll_bool_t l){
	if (l&&_win_large_page!=2){
		if (!_win_large_page){
			_win_large_page=1;
			HANDLE h;
			TOKEN_PRIVILEGES tp;
			if (!OpenProcessToken(GetCurrentProcess(),TOKEN_ADJUST_PRIVILEGES|TOKEN_QUERY,&h)||!LookupPrivilegeValueA(NULL,"SeLockMemoryPrivilege",&tp.Privileges[0].Luid)){
				_win_large_page=2;
			}
			else{
				tp.PrivilegeCount=1;
				tp.Privileges[0].Attributes=SE_PRIVILEGE_ENABLED;
				if (!AdjustTokenPrivileges(h,FALSE,&tp,0,NULL,0)||(GetLastError()!=ERROR_SUCCESS)||!CloseHandle(h)||!GetLargePageMinimum()!=SLL_LARGE_PAGE_SIZE){
					_win_large_page=2;
				}
			}
		}
		SLL_ASSERT(SLL_ROUND_LARGE_PAGE(sz)==sz);
		void* o=VirtualAlloc(NULL,sz,MEM_COMMIT|MEM_RESERVE|MEM_LARGE_PAGES,PAGE_READWRITE);
		if (o){
			return o;
		}
	}
	SLL_ASSERT(SLL_ROUND_PAGE(sz)==sz);
	return VirtualAlloc(NULL,sz,MEM_COMMIT|MEM_RESERVE,PAGE_READWRITE);
}



__SLL_EXTERNAL void sll_platform_free_page(void* pg,sll_size_t sz){
	VirtualFree(pg,0,MEM_RELEASE);
}
