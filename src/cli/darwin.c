#include <sll.h>
#include <string.h>
#include <unistd.h>
#include <dlfcn.h>



#define APT_INSTALL_PATH "/usr/bin/"
#define APT_LIB_PATH "/usr/lib/sll_"SLL_VERSION_STRING"/"LIBRARY_NAME

#define LIBRARY_NAME "sll-"SLL_VERSION_STRING".so"
#define STRLEN(x) (sizeof(x)/sizeof(char)-1)



int main(int argc,const char*const* argv){
	if (!argc){
		return 0;
	}
	char bf[1024+STRLEN(LIBRARY_NAME)];
	ssize_t bfl=1024;
	if (_NSGetExecutablePath(bf,&bfl)==-1){
		bf[0]='/';
		bfl=0;
	}
	else{
		bfl=strlen(bf)-1;
		while (bfl&&bf[bfl]!='/'){
			bfl--;
		}
		if (!bfl){
			bf[0]='/';
		}
	}
	bfl++;
	if (bfl>=STRLEN(APT_INSTALL_PATH)){
		for (unsigned int i=0;i<STRLEN(APT_INSTALL_PATH);i++){
			if (bf[i]!=APT_INSTALL_PATH[i]){
				goto _not_apt;
			}
		}
		for (unsigned int i=0;i<STRLEN(APT_LIB_PATH);i++){
			bf[i]=APT_LIB_PATH[i];
		}
		bf[STRLEN(APT_LIB_PATH)]=0;
	}
	else{
_not_apt:
		for (unsigned int i=0;i<STRLEN(LIBRARY_NAME);i++){
			bf[bfl+i]=LIBRARY_NAME[i];
		}
		bf[bfl+STRLEN(LIBRARY_NAME)]=0;
	}
	void* lh=dlopen(bf,RTLD_LAZY);
	if (!lh){
		return 0;
	}
	sll_return_code_t o=0;
	void* ver=dlsym(lh,"sll_version");
	if (!ver||((sll_version_t(*)(void))ver)()!=SLL_VERSION){
		goto _end;
	}
	void* cli=dlsym(lh,"sll_cli_main");
	if (!cli){
		goto _end;
	}
	o=((sll_return_code_t(*)(sll_array_length_t,const sll_char_t*const*))cli)(argc-1,(const sll_char_t*const*)(argv+1));
_end:
	dlclose(lh);
	return o;
}
