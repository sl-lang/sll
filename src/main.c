#ifdef _MSC_VER
#include <windows.h>
#else
#include <linux/limits.h>
#include <stdlib.h>
#endif
#include <lll.h>
#include <stdio.h>



#ifdef _MSC_VER
#define MAX_PATH_LENGTH MAX_PATH
#else
#define MAX_PATH_LENGTH PATH_MAX
#endif
#define OPTIMIZE_LEVEL_NO_OPTIMIZE 0
#define OPTIMIZE_LEVEL_REMOVE_PADDING 1
#define OPTIMIZE_LEVEL_GLOBAL_OPTIMIZE 2
#define OPTIMIZE_LEVEL_STRIP_DEBUG_DATA 3
#define FLAG_VERBOSE 1
#define FLAG_COMPILE_ONLY 2
#define FLAG_FULL_PATH 4
#define DEFAULT_OPTIMIZE_LEVEL OPTIMIZE_LEVEL_GLOBAL_OPTIMIZE
#define COMPILER_STACK_SIZE 65536



uint8_t st[COMPILER_STACK_SIZE];



int main(int argc,const char** argv){
	uint8_t ol=DEFAULT_OPTIMIZE_LEVEL;
	uint8_t fl=0;
	const char* fp=NULL;
	const char* o_fp=NULL;
	for (int i=1;i<argc;i++){
		const char* e=argv[i];
		if (*e=='-'&&*(e+1)=='O'&&*(e+3)==0){
			if (*(e+2)=='0'){
				ol=OPTIMIZE_LEVEL_NO_OPTIMIZE;
			}
			else if (*(e+2)=='1'){
				ol=OPTIMIZE_LEVEL_REMOVE_PADDING;
			}
			else if (*(e+2)=='2'){
				ol=OPTIMIZE_LEVEL_GLOBAL_OPTIMIZE;
			}
			else if (*(e+2)=='3'){
				ol=OPTIMIZE_LEVEL_STRIP_DEBUG_DATA;
			}
			else{
				goto _unkown_switch;
			}
		}
		else if (*e=='-'&&*(e+1)=='v'&&*(e+2)==0){
			fl|=FLAG_VERBOSE;
		}
		else if (*e=='-'&&*(e+1)=='c'&&*(e+2)==0){
			fl|=FLAG_COMPILE_ONLY;
		}
		else if (*e=='-'&&*(e+1)=='f'&&*(e+2)=='p'&&*(e+3)==0){
			fl|=FLAG_FULL_PATH;
		}
		else if (*e=='-'&&*(e+1)=='o'&&*(e+2)==0){
			if (o_fp){
				printf("Multplie Output Files Supplied\n");
				return 1;
			}
			i++;
			if (i==argc){
				break;
			}
			o_fp=argv[i];
		}
		else if (*e=='-'){
_unkown_switch:
			printf("Unknown Switch: '%s'\n",e);
			return 1;
		}
		else{
			if (fp!=NULL){
				printf("Multplie Input Files not Supported\n");
				return 1;
			}
			fp=e;
		}
	}
	if (fp==NULL){
		printf("Not Input Files Supplied\n");
		return 1;
	}
	char tmp0[MAX_PATH_LENGTH];
	if (fl&FLAG_FULL_PATH){
#ifdef _MSC_VER
		if (GetFullPathNameA(fp,MAX_PATH_LENGTH,tmp0,NULL)){
#else
		if (realpath(fp,tmp0)){
#endif
			fp=tmp0;
		}
	}
	char tmp1[MAX_PATH_LENGTH];
	if (!o_fp){
		uint16_t i=0;
		while (*(fp+i)&&*(fp+i)!='.'){
			*(tmp1+i)=*(fp+i);
			i++;
		}
		if (fl&FLAG_COMPILE_ONLY){
			tmp1[i]='.';
			tmp1[i+1]='l';
			tmp1[i+2]='l';
			tmp1[i+3]='l';
			tmp1[i+4]='c';
			tmp1[i+5]=0;
		}
		else{
			tmp1[i]='.';
			tmp1[i+1]='c';
			tmp1[i+2]=0;
		}
		o_fp=tmp1;
	}
	if (fl&FLAG_VERBOSE){
		printf("Configuration:\n  Optimization Level: %c (",ol+48);
		if (ol==OPTIMIZE_LEVEL_NO_OPTIMIZE){
			printf("OPTIMIZE_LEVEL_NO_OPTIMIZE");
		}
		else if (ol==OPTIMIZE_LEVEL_REMOVE_PADDING){
			printf("OPTIMIZE_LEVEL_REMOVE_PADDING");
		}
		else if (ol==OPTIMIZE_LEVEL_GLOBAL_OPTIMIZE){
			printf("OPTIMIZE_LEVEL_REMOVE_PADDING, OPTIMIZE_LEVEL_GLOBAL_OPTIMIZE");
		}
		else{
			printf("OPTIMIZE_LEVEL_REMOVE_PADDING, OPTIMIZE_LEVEL_GLOBAL_OPTIMIZE, OPTIMIZE_LEVEL_STRIP_DEBUG_DATA");
		}
		printf(")\n");
		if (fl&FLAG_COMPILE_ONLY){
			printf("  Compile Only Mode\n");
		}
		else{
			printf("  Compilation And Code Generation Mode\n");
		}
		if (fl&FLAG_FULL_PATH){
			printf("  Full Path Mode\n");
		}
		printf("Compiling File '%s'...\n",fp);
	}
	lll_error_t e;
	if (!lll_set_internal_stack(st,COMPILER_STACK_SIZE,&e)){
		lll_print_error(NULL,&e);
		return 1;
	}
	FILE* f=NULL;
#ifdef _MSC_VER
	if (fopen_s(&f,fp,"rb")){// lgtm [cpp/path-injection]
#else
	if (!(f=fopen(fp,"rb"))){// lgtm [cpp/path-injection]
#endif
		printf("Unable to Open File '%s'!\n",fp);
		return 1;
	}
	lll_input_data_stream_t is;
	lll_create_input_data_stream(f,&is);
	lll_compilation_data_t c_dt;
	lll_init_compilation_data(fp,&is,&c_dt);
	if (!lll_read_all_objects(&c_dt,&e)){
		lll_print_error(&is,&e);
	}
	else{
		lll_print_object(c_dt.h,stdout);
		putchar('\n');
		if (ol>=OPTIMIZE_LEVEL_GLOBAL_OPTIMIZE){
			if (!lll_optimize_object(c_dt.h,&e)){
				lll_print_error(&is,&e);
				return 1;
			}
		}
		if (ol>=OPTIMIZE_LEVEL_STRIP_DEBUG_DATA){
			if (!lll_remove_object_debug_data(c_dt.h,&e)){
				lll_print_error(&is,&e);
				return 1;
			}
		}
		if (ol>=OPTIMIZE_LEVEL_REMOVE_PADDING){
			if (!lll_remove_object_padding(c_dt.h,&e)){
				lll_print_error(&is,&e);
				return 1;
			}
		}
		lll_print_object(c_dt.h,stdout);
		putchar('\n');
		FILE* of=NULL;
#ifdef _MSC_VER
		if (fopen_s(&of,o_fp,"wb")){// lgtm [cpp/path-injection]
#else
		if (!(of=fopen(o_fp,"wb"))){// lgtm [cpp/path-injection]
#endif
			printf("Unable to Open File '%s'!\n",o_fp);
			return 1;
		}
		lll_output_data_stream_t os;
		lll_create_output_data_stream(of,&os);
		if (!lll_write_object(&os,c_dt.h,((fl&FLAG_COMPILE_ONLY)?LLL_WRITE_MODE_RAW:LLL_WRITE_MODE_C),&e)){
			lll_print_error(&is,&e);
			return 1;
		}
		fclose(of);
	}
	fclose(f);
	return 0;
}
