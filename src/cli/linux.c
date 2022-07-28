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
	char buffer[PATH_MAX+STATIC_STRLEN(LIBRARY_NAME)];
#ifdef __SLL_BUILD_DARWIN
	uint32_t buffer_length=PATH_MAX;
	if (!_NSGetExecutablePath(buffer,&buffer_length)){
		buffer_length=0;
		while (buffer[buffer_length]){
			buffer_length++;
		}
#else
	ssize_t buffer_length=readlink("/proc/self/exe",buffer,PATH_MAX);
	if (buffer_length!=-1){
#endif
		while (buffer_length&&buffer[buffer_length]!='/'){
			buffer_length--;
		}
		if (!buffer_length){
			buffer[0]='/';
		}
	}
	else{
		buffer[0]='/';
		buffer_length=0;
	}
	buffer_length++;
	if (buffer_length>=STATIC_STRLEN(APT_INSTALL_PATH)){
		for (unsigned int i=0;i<STATIC_STRLEN(APT_INSTALL_PATH);i++){
			if (buffer[i]!=APT_INSTALL_PATH[i]){
				goto _not_apt;
			}
		}
		for (unsigned int i=0;i<STATIC_STRLEN(APT_LIB_PATH);i++){
			buffer[i]=APT_LIB_PATH[i];
		}
		buffer[STATIC_STRLEN(APT_LIB_PATH)]=0;
	}
	else{
_not_apt:
		for (unsigned int i=0;i<STATIC_STRLEN(LIBRARY_NAME);i++){
			buffer[buffer_length+i]=LIBRARY_NAME[i];
		}
		buffer[buffer_length+STATIC_STRLEN(LIBRARY_NAME)]=0;
	}
	void* library_handle=dlopen(buffer,RTLD_LAZY);
	if (!library_handle){
		return 0;
	}
	sll_version_t (*ver)(void)=dlsym(library_handle,"sll_version");
	if (!ver||ver()!=SLL_VERSION){
		dlclose(library_handle);
		return 0;
	}
	sll_return_code_t (*cli)(sll_array_length_t,const sll_char_t*const*)=dlsym(library_handle,"sll_cli_main");
	sll_return_code_t out=(cli?cli(argc-1,(const sll_char_t*const*)(argv+1)):0);
	dlclose(library_handle);
	return out;
}
