#include <sll.h>
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
	unsigned int i=0;
	unsigned int j=0;
	const char* fp=*argv;
	while (*fp&&j<4096){
		bf[j]=*fp;
		if (bf[j]=='/'){
			i=j;
		}
		j++;
		fp++;
	}
	if (!i){
		bf[0]='/';
	}
	i++;
	if (i>=STRLEN(APT_INSTALL_PATH)){
		for (unsigned int k=0;k<STRLEN(APT_INSTALL_PATH);k++){
			if (bf[k]!=APT_INSTALL_PATH[k]){
				goto _not_apt;
			}
		}
		for (unsigned int k=0;k<STRLEN(APT_LIB_PATH);k++){
			bf[k]=APT_LIB_PATH[k];
		}
		bf[STRLEN(APT_LIB_PATH)]=0;
	}
	else{
_not_apt:
		for (unsigned int k=0;k<STRLEN(LIBRARY_NAME);k++){
			bf[i+k]=LIBRARY_NAME[k];
		}
		bf[i+STRLEN(LIBRARY_NAME)]=0;
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
