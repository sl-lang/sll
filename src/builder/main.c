#include <stdio.h>
#include <stdint.h>
#include <string.h>



#define PRINT_VERBOSE(s) \
	do{ \
		if (fl&FLAG_VERBOSE){ \
			fputs(s,stdout); \
		} \
	} while (0)
#define FLAG_DO_NO_RUN_TEST 1
#define FLAG_RELEASE 2
#define FLAG_RUN 4
#define FLAG_TEST 8
#define FLAG_VERBOSE 16



int main(int argc,const char** argv){
	uint8_t fl=0;
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
	return 0;
}
