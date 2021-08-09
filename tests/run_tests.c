#ifdef _MSC_VER
#define WIN32_LEAN_AND_MEAN 1
#include <windows.h>
#else
#include <dirent.h>
#endif
#include <sll_lib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>



typedef void (*file_callback_t)(const char* fp);



uint32_t list_files(char* fp,uint16_t fpl,file_callback_t cb){
	uint32_t o=0;
#ifdef _MSC_VER
	WIN32_FIND_DATAA dt;
	HANDLE fh=FindFirstFileA(fp,&dt);
	if (fh!=INVALID_HANDLE_VALUE){
		do{
			if (!(dt.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)){
				o++;
				cb(dt.cFileName);
			}
		} while (FindNextFileA(fh,&dt));
		FindClose(fh);
	}
#else
	DIR* d=opendir(fp);
	if (d){
		struct dirent* dt;
		while (dt=readdir(d)){
			if (dt->d_type==DT_REG){
				o++;
				cb(dt->d_name);
			}
		}
		closedir(d);
	}
#endif
	return o;
}



void run_parser_test(const char* nm){
	printf("Test Path: %s\n",nm);
}



int main(int argc,const char** argv){
	char bf[4096]=__TEST_ROOT_DIR__;
	uint16_t bfl=0;
	while (bf[bfl]){
		bfl++;
	}
	while (bfl&&bf[bfl]!='/'&&bf[bfl]!='\\'){
		bfl--;
	}
	bf[bfl+1]='p';
	bf[bfl+2]='a';
	bf[bfl+3]='r';
	bf[bfl+4]='s';
	bf[bfl+5]='e';
	bf[bfl+6]='/';
	bf[bfl+7]=0;
	list_files(bf,bfl+7,run_parser_test);
	return 0;
}
