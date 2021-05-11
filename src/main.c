#ifdef _MSC_VER
#include <windows.h>
#else
#include <linux/limits.h>
#include <stdlib.h>
#endif
#include <language.h>
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



int main(int argc,const char** argv){
	uint8_t ol=DEFAULT_OPTIMIZE_LEVEL;
	uint8_t fl=0;
	const char* fp=NULL;
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
	char tmp[MAX_PATH_LENGTH];
	if (fl&FLAG_FULL_PATH){
#ifdef _MSC_VER
		if (GetFullPathNameA(fp,MAX_PATH_LENGTH,tmp,NULL)){
#else
		if (realpath(fp,tmp)){
#endif
			fp=tmp;
		}
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
			printf("  Non-Compilation Only Mode Not Supported\n");
			return 1;
		}
		if (fl&FLAG_COMPILE_ONLY){
			printf("  Full Path Mode\n");
		}
		printf("Compiling File '%s'...\n",fp);
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
	input_data_stream_t is;
	create_input_data_stream(f,&is);
	compilation_data_t c_dt;
	init_compilation_data(fp,&is,&c_dt);
	error_t e=read_all_objects(&c_dt);
	if (IS_ERROR(e)){
		print_error(&is,GET_ERROR(e));
	}
	else{
		print_object(c_dt.h,stdout);
		if (ol>=OPTIMIZE_LEVEL_GLOBAL_OPTIMIZE){
			error_t e=optimize_object(c_dt.h);
			if (IS_ERROR(e)){
				print_error(&is,GET_ERROR(e));
				return 1;
			}
		}
		if (ol>=OPTIMIZE_LEVEL_STRIP_DEBUG_DATA){
			remove_object_debug_data(c_dt.h);
		}
		if (ol>=OPTIMIZE_LEVEL_REMOVE_PADDING){
			remove_object_padding(c_dt.h);
		}
		print_object(c_dt.h,stdout);
		FILE* of=NULL;
		char o_fp[512];
		uint16_t i=0;
		while (*fp&&*fp!='.'){
			*(o_fp+i)=*fp;
			fp++;
			i++;
		}
		o_fp[i]='.';
		o_fp[i+1]='l';
		o_fp[i+2]='l';
		o_fp[i+3]='l';
		o_fp[i+4]='c';
		o_fp[i+5]=0;
#ifdef _MSC_VER
		if (fopen_s(&of,o_fp,"wb")){// lgtm [cpp/path-injection]
#else
		if (!(of=fopen(o_fp,"wb"))){// lgtm [cpp/path-injection]
#endif
			printf("Unable to Open File '%s'!\n",o_fp);
			return 1;
		}
		output_data_stream_t os;
		create_output_data_stream(of,&os);
		write_object(&os,c_dt.h);
		fclose(of);
	}
	fclose(f);
	return 0;
}
