#include <platform.h>
#include <util.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>



#define PRINT_VERBOSE(s,...) \
	do{ \
		if (fl&FLAG_VERBOSE){ \
			printf(s,__VA_ARGS__); \
		} \
	} while (0)
#define FLAG_DO_NO_RUN_TEST 1
#define FLAG_RELEASE 2
#define FLAG_RUN 4
#define FLAG_TEST 8
#define FLAG_VERBOSE 16



uint8_t fl=0;



uint8_t _copy_lib_file(const char* fp,void* ctx){
	PRINT_VERBOSE("  Copying Module '%s'...\n",fp);
	char bf[4096]="build/";
	bf[6+copy_str(bf+6,fp+8)]=0;
	copy_file(fp,bf);
	return 0;
}



int main(int argc,const char** argv){
	for (int i=0;i<argc;i++){
		const char* k=argv[i];
		if (*k!='-'||*(k+1)!='-'){
			continue;
		}
		k+=2;
		if (!strcmp(k,"do-not-run-test")){
			fl|=FLAG_DO_NO_RUN_TEST;
		}
		else if (!strcmp(k,"release")){
			fl|=FLAG_RELEASE;
		}
		else if (!strcmp(k,"run")){
			fl|=FLAG_RUN;
		}
		else if (!strcmp(k,"test")){
			fl|=FLAG_TEST;
		}
		else if (!strcmp(k,"verbose")){
			fl|=FLAG_VERBOSE;
		}
	}
	PRINT_VERBOSE("Generating Build Directory...\n");
	create_build();
	PRINT_VERBOSE("Copying Modules...\n");
	list_files("src/sll/lib/",LIST_FILES_ONLY_FILES,_copy_lib_file,NULL);
	return 0;
}
