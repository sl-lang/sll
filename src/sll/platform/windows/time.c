#include <windows.h>
#include <bcrypt.h>
#include <immintrin.h>
#include <io.h>
#include <signal.h>
#include <sll/api/date.h>
#include <sll/api/time.h>
#include <sll/common.h>
#include <sll/types.h>



static HANDLE _win_wh=INVALID_HANDLE_VALUE;
static sll_time_zone_t _win_platform_time_zone={"GMT",0};
__SLL_EXTERNAL const sll_time_zone_t* sll_platform_time_zone=&_win_platform_time_zone;



void _deinit_platform_time(void){
	if (_win_wh!=INVALID_HANDLE_VALUE){
		CloseHandle(_win_wh);
		_win_wh=INVALID_HANDLE_VALUE;
	}
	_win_platform_time_zone=*sll_utc_time_zone;
}



void _init_platform_time(void){
	TIME_ZONE_INFORMATION tz;
	DWORD tz_st=GetTimeZoneInformation(&tz);
	WCHAR* nm;
	_win_platform_time_zone.off=-tz.Bias;
	if (tz_st==TIME_ZONE_ID_DAYLIGHT){
		nm=tz.DaylightName;
		_win_platform_time_zone.off-=tz.DaylightBias;
	}
	else{
		nm=tz.StandardName;
		_win_platform_time_zone.off-=tz.StandardBias;
	}
	sll_string_length_t i=0;
	do{
		i++;
	} while (*(nm+i-1));
	WideCharToMultiByte(CP_UTF8,0,nm,i,_win_platform_time_zone.nm,32,NULL,NULL);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_time_t sll_platform_get_current_time(void){
	FILETIME ft;
	GetSystemTimeAsFileTime(&ft);
	return ((((sll_time_t)ft.dwHighDateTime)<<32)|ft.dwLowDateTime)*100-11644473600000000000;
}



__SLL_EXTERNAL void sll_platform_sleep(sll_time_t tm){
	if (_win_wh==INVALID_HANDLE_VALUE){
		_win_wh=CreateEventA(NULL,TRUE,FALSE,FALSE);
	}
	sll_time_t e=GetTickCount64()*1000000+tm;
	while (1){
		ResetEvent(_win_wh);
		if (WaitForSingleObjectEx(_win_wh,(DWORD)((tm+999999)/1000000),FALSE)!=WAIT_OBJECT_0){
			return;
		}
		sll_time_t c=GetTickCount64()*1000000;
		if (c>=e){
			break;
		}
		tm=c-e;
	}
}
