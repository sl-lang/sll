#include <sll.h>
#include <dlfcn.h>
#ifdef __SLL_BUILD_DARWIN
#include <sys/syslimits.h>
#else
#include <linux/limits.h>
#endif
#include <unistd.h>



#define APT_INSTALL_PATH "/usr/bin/"
#define APT_LIB_PATH "/usr/lib/sll_"SLL_VERSION_STRING"/"LIBRARY_NAME

#define LIBRARY_NAME "sll-"SLL_VERSION_STRING".so"
#define STATIC_STRLEN(x) (sizeof(x)/sizeof(char)-1)



int main(int argc,const char*const* argv){
	if (!argc){
		return 0;
	}
	char bf[PATH_MAX+STATIC_STRLEN(LIBRARY_NAME)];
#ifdef __SLL_BUILD_DARWIN
	uint32_t bfl=PATH_MAX;
	if (!_NSGetExecutablePath(bf,&bfl)){
		bfl=0;
		while (bf[bfl]){
			bfl++;
		}
#else
	ssize_t bfl=readlink("/proc/self/exe",bf,PATH_MAX);
	if (bfl!=-1){
#endif
		while (bfl&&bf[bfl]!='/'){
			bfl--;
		}
		if (!bfl){
			bf[0]='/';
		}
	}
	else{
		bf[0]='/';
		bfl=0;
	}
	bfl++;
	if (bfl>=STATIC_STRLEN(APT_INSTALL_PATH)){
		for (unsigned int i=0;i<STATIC_STRLEN(APT_INSTALL_PATH);i++){
			if (bf[i]!=APT_INSTALL_PATH[i]){
				goto _not_apt;
			}
		}
		for (unsigned int i=0;i<STATIC_STRLEN(APT_LIB_PATH);i++){
			bf[i]=APT_LIB_PATH[i];
		}
		bf[STATIC_STRLEN(APT_LIB_PATH)]=0;
	}
	else{
_not_apt:
		for (unsigned int i=0;i<STATIC_STRLEN(LIBRARY_NAME);i++){
			bf[bfl+i]=LIBRARY_NAME[i];
		}
		bf[bfl+STATIC_STRLEN(LIBRARY_NAME)]=0;
	}
	void* lh=dlopen(bf,RTLD_LAZY);
	if (!lh){
		return 0;
	}
	sll_version_t (*ver)(void)=dlsym(lh,"sll_version");
	if (!ver||ver()!=SLL_VERSION){
		dlclose(lh);
		return 0;
	}
	sll_return_code_t (*cli)(sll_array_length_t,const sll_char_t*const*)=dlsym(lh,"sll_cli_main");
	sll_return_code_t o=(cli?cli(argc-1,(const sll_char_t*const*)(argv+1)):0);
	dlclose(lh);
	return o;
}
