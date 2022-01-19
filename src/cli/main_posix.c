#include <sll.h>
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
	char bf[4096+STRLEN(LIBRARY_NAME)];
	ssize_t bfl=readlink("/proc/self/exe",bf,4096);
	if (bfl==-1){
		bf[0]='/';
		bfl=0;
	}
	else{
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
	void* cli=dlsym(lh,"sll_cli_main");
	if (!cli){
		dlclose(lh);
		return 0;
	}
	sll_return_code_t o=((sll_return_code_t(*)(sll_array_length_t,const sll_char_t*const*))cli)(argc-1,(const sll_char_t*const*)(argv+1));
	dlclose(lh);
	return o;
}
