#include <sll.h>
#include <linux/limits.h>
#include <string.h>
#include <unistd.h>
#include <dlfcn.h>
#include <stdio.h>



#define APT_INSTALL_PATH "/usr/bin/"
#define APT_LIB_PATH "/usr/lib/sll_"SLL_VERSION_STRING"/"LIBRARY_NAME

#define LIBRARY_NAME "sll-"SLL_VERSION_STRING".so"
#define STRLEN(x) (sizeof(x)/sizeof(char)-1)



int main(int argc,const char** argv){
	char bf[PATH_MAX+STRLEN(LIBRARY_NAME)];
	ssize_t bfl=readlink("/proc/self/exe",bf,PATH_MAX);
	if (bfl==-1){
		return -1;
	}
	while (bfl&&bf[bfl]!='/'){
		bfl--;
	}
	if (!bfl){
		return -1;
	}
	bfl++;
	if (bfl==STRLEN(APT_INSTALL_PATH)&&!memcmp(bf,APT_INSTALL_PATH,STRLEN(APT_INSTALL_PATH))){
		bf=APT_LIB_PATH;
	}
	else{
		memcpy(bf+bfl,LIBRARY_NAME,STRLEN(LIBRARY_NAME)+1);
	}
	void* lh=dlopen(bf,RTLD_NOW|RTLD_GLOBAL);
	if (!lh){
		return -1;
	}
	void* cli=dlsym(lh,"sll_cli_main");
	if (!cli){
		dlclose(lh);
		return -1;
	}
	sll_return_code_t o=((sll_return_code_t(*)(sll_array_length_t,const sll_char_t*const*))cli)(argc,(const sll_char_t*const*)argv);
	dlclose(lh);
	return o;
}
