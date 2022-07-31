#include <windows.h>
#include <sll/_internal/common.h>
#include <sll/_internal/error.h>
#include <sll/platform/memory.h>
#include <sll/platform/util.h>
#include <sll/common.h>
#include <sll/error.h>
#include <sll/types.h>



static char _win_large_page=0;
static sll_size_t _win_system_align=0;



__SLL_EXTERNAL __SLL_CHECK_OUTPUT void* sll_platform_allocate_page(sll_size_t sz,sll_bool_t l,sll_error_t* err){
	ERROR_PTR_RESET;
	if (l&&_win_large_page!=2){
		if (!_win_large_page){
			_win_large_page=1;
			HANDLE h;
			TOKEN_PRIVILEGES tp;
			if (!OpenProcessToken(GetCurrentProcess(),TOKEN_ADJUST_PRIVILEGES|TOKEN_QUERY,&h)||!LookupPrivilegeValueA(NULL,"SeLockMemoryPrivilege",&(tp.Privileges[0].Luid))){
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
	void* o=VirtualAlloc(NULL,sz,MEM_COMMIT|MEM_RESERVE,PAGE_READWRITE);
	if (!o){
		ERROR_PTR_SYSTEM;
	}
	return o;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT void* sll_platform_allocate_page_aligned(sll_size_t sz,sll_size_t align,sll_error_t* err){
	ERROR_PTR_RESET;
	if (!_win_system_align){
		SYSTEM_INFO si;
		GetSystemInfo(&si);
		_win_system_align=si.dwAllocationGranularity;
	}
	SLL_ASSERT(!(align&(align-1)));
	SLL_ASSERT(SLL_ROUND_PAGE(sz)==sz);
	if (align<=_win_system_align){
		void* o=VirtualAlloc(NULL,sz,MEM_COMMIT|MEM_RESERVE,PAGE_READWRITE);
		if (!o){
			ERROR_PTR_SYSTEM;
		}
		return o;
	}
	SLL_UNIMPLEMENTED();
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_error_t sll_platform_free_page(void* page,sll_size_t sz){
	return (VirtualFree(page,0,MEM_RELEASE)?SLL_NO_ERROR:sll_platform_get_error());
}
