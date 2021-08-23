#include <windows.h>
#include <util.h>
#include <platform.h>
#include <stdint.h>
#include <string.h>



typedef struct __DIRECTORY_LIST{
	char** dl;
	uint32_t dll;
} directory_list_t;



void _list_files(char* fp,uint16_t fpl,list_files_callback_t d_cb,list_files_callback_t f_cb,void* ctx){
	WIN32_FIND_DATAA dt;
	*(fp+fpl)='*';
	*(fp+fpl+1)=0;
	HANDLE fh=FindFirstFileA(fp,&dt);
	if (fh!=INVALID_HANDLE_VALUE){
		do{
			if (dt.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY){
				if (d_cb==LIST_FILES_ONLY_FILES||(*(dt.cFileName)=='.'&&(*(dt.cFileName+1)==0||(*(dt.cFileName+1)=='.'&&*(dt.cFileName+2)==0)))){
					continue;
				}
				uint16_t i=fpl;
				fpl+=copy_str(fp+fpl,dt.cFileName);
				*(fp+fpl)=0;
				if (d_cb(fp,ctx)){
					fpl=i;
					continue;
				}
				*(fp+fpl)='/';
				_list_files(fp,fpl+1,d_cb,f_cb,ctx);
				fpl=i;
			}
			else{
				*(fp+fpl+copy_str(fp+fpl,dt.cFileName))=0;
				f_cb(fp,ctx);
			}
		} while (FindNextFileA(fh,&dt));
		FindClose(fh);
	}
}



uint8_t _add_directory(const char* fp,void* ctx){
	if (!strcmp(fp+6,"builder")){
		return 1;
	}
	if (!strcmp(fp+6,"lib")||!strcmp(fp+6,"objects")){
		return 0;
	}
	directory_list_t* dl=(directory_list_t*)ctx;
	dl->dll++;
	dl->dl=realloc(dl->dl,dl->dll*sizeof(char*));
	uint16_t i=0;
	while (*(fp+i)){
		i++;
	}
	char* d=malloc((i+1)*sizeof(char));
	memcpy(d,fp,(i+1)*sizeof(char));
	*(dl->dl+dl->dll-1)=d;
	return 0;
}



uint8_t _delete_file(const char* fp,void* ctx){
	DeleteFile(fp);
	return 0;
}



void copy_file(const char* s,const char* d){
	CopyFileA(s,d,FALSE);
}



void create_build(void){
	if (GetFileAttributesA("build")==INVALID_FILE_ATTRIBUTES){
		CreateDirectoryA("build",NULL);
		CreateDirectoryA("build/lib",NULL);
		CreateDirectoryA("build/objects",NULL);
	}
	else{
		directory_list_t dl={
			NULL,
			0
		};
		list_files("build/",_add_directory,_delete_file,&dl);
		uint32_t i=dl.dll;
		while (i){
			i--;
			RemoveDirectoryA(*(dl.dl+i));
			free(*(dl.dl+i));
		}
		if (dl.dl){
			free(dl.dl);
		}
		if (GetFileAttributesA("build/lib")==INVALID_FILE_ATTRIBUTES){
			CreateDirectoryA("build/lib",NULL);
		}
		if (GetFileAttributesA("build/objects")==INVALID_FILE_ATTRIBUTES){
			CreateDirectoryA("build/objects",NULL);
		}
	}
}



void list_files(const char* fp,list_files_callback_t d_cb,list_files_callback_t f_cb,void* ctx){
	char bf[MAX_PATH];
	uint16_t i=0;
	while (*(fp+i)){
		bf[i]=*(fp+i);
		if (bf[i]=='\\'){
			bf[i]='/';
		}
		i++;
	}
	if (bf[i-1]!='/'){
		bf[i]='/';
		i++;
	}
	_list_files(bf,i,d_cb,f_cb,ctx);
}
