#ifdef _MSC_VER
#include <windows.h>
#else
#include <linux/limits.h>
#endif
#include <lll_lib.h>
#include <stdio.h>
#include <stdlib.h>



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
	char** fp=NULL;
	uint32_t fpl=0;
	const char* o_fp=NULL;
	char* ffp=NULL;
	FILE* f=NULL;
	FILE* of=NULL;
	lll_compilation_data_t c_dt={0};
	int i=1;
	while (i<argc){
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
				goto _cleanup;
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
			goto _cleanup;
		}
		else{
			fpl++;
			fp=realloc(fp,fpl*sizeof(char*));
			*(fp+fpl-1)=(char*)e;
		}
		i++;
	}
	if (!fpl){
		printf("Not Input Files Supplied\n");
		return 1;
	}
	if (fl&FLAG_FULL_PATH){
		ffp=malloc(fpl*MAX_PATH_LENGTH*sizeof(char));
		for (uint32_t i=0;i<fpl;i++){
#ifdef _MSC_VER
			if (GetFullPathNameA(*(fp+i),MAX_PATH_LENGTH,ffp+i*MAX_PATH_LENGTH,NULL)){
#else
			if (realpath(*(fp+i),ffp+i*MAX_PATH_LENGTH)){
#endif
				*(fp+i)=ffp+i*MAX_PATH_LENGTH;
			}
		}
	}
	char tmp[MAX_PATH_LENGTH];
	if (!o_fp){
		uint16_t i=0;
		while (*(*fp+i)&&*(*fp+i)!='.'){
			*(tmp+i)=*(*fp+i);
			i++;
		}
		tmp[i]='.';
		if (fl&FLAG_COMPILE_ONLY){
			tmp[i+1]='l';
			tmp[i+2]='l';
			tmp[i+3]='l';
			tmp[i+4]='c';
			tmp[i+5]=0;
		}
		else{
			tmp[i+1]='a';
			tmp[i+2]='s';
			tmp[i+3]='m';
			tmp[i+4]=0;
		}
		o_fp=tmp;
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
	}
	lll_error_t e;
	if (!lll_set_internal_stack(st,COMPILER_STACK_SIZE,&e)){
		lll_print_error(NULL,&e);
		goto _cleanup;
	}
	lll_input_data_stream_t is;
	for (uint32_t i=0;i<fpl;i++){
		if (fl&FLAG_VERBOSE){
			printf("Opening File '%s'...\n",*(fp+i));
		}
#ifdef _MSC_VER
		if (fopen_s(&f,*(fp+i),"rb")){// lgtm [cpp/path-injection]
#else
		if (!(f=fopen(*(fp+i),"rb"))){// lgtm [cpp/path-injection]
#endif
			printf("Unable to Open File '%s'!\n",*(fp+i));
			goto _cleanup;
		}
		lll_create_input_data_stream(f,&is);
		if (fl&FLAG_VERBOSE){
			printf("Trying to Load File as Compiled Object...\n");
		}
		if (!lll_load_compiled_object(&is,&c_dt,&e)){
			lll_free_identifier_data(&(c_dt.i_dt));
			if (e.t==LLL_ERROR_INVALID_FILE_FORMAT){
				if (fl&FLAG_VERBOSE){
					printf("File is not a Compiled Object. Falling Back to Standard Compilation...\n");
				}
				lll_create_input_data_stream(f,&is);
				lll_init_compilation_data(*(fp+i),&is,&c_dt);
				if (!lll_read_all_objects(&c_dt,&e)){
					lll_print_error(&is,&e);
					goto _cleanup;
				}
			}
			else{
				lll_print_error(&is,&e);
				goto _cleanup;
			}
		}
		if (fl&FLAG_VERBOSE){
			printf("File Successfully Read.\n");
		}
		if (ol>=OPTIMIZE_LEVEL_GLOBAL_OPTIMIZE){
			if (fl&FLAG_VERBOSE){
				printf("Performing Global Optimization...\n");
			}
			if (!lll_optimize_object(c_dt.h,&e)){
				lll_print_error(&is,&e);
				goto _cleanup;
			}
		}
		if (ol>=OPTIMIZE_LEVEL_STRIP_DEBUG_DATA){
			if (fl&FLAG_VERBOSE){
				printf("Removing Debug Data...\n");
			}
			if (!lll_remove_object_debug_data(c_dt.h,&e)){
				lll_print_error(&is,&e);
				goto _cleanup;
			}
		}
		if (ol>=OPTIMIZE_LEVEL_REMOVE_PADDING){
			if (fl&FLAG_VERBOSE){
				printf("Removing Object Padding...\n");
			}
			if (!lll_remove_object_padding(c_dt.h,&e)){
				lll_print_error(&is,&e);
				goto _cleanup;
			}
		}
#ifdef _MSC_VER
		if (fopen_s(&of,o_fp,"wb")){// lgtm [cpp/path-injection]
#else
		if (!(of=fopen(o_fp,"wb"))){// lgtm [cpp/path-injection]
#endif
			printf("Unable to Open File '%s'!\n",o_fp);
			goto _cleanup;
		}
		lll_output_data_stream_t os;
		lll_create_output_data_stream(of,&os);
		if (!lll_write_compiled_object(&os,&c_dt,(fl&FLAG_COMPILE_ONLY?LLL_WRITE_MODE_RAW:LLL_WRITE_MODE_ASSEMBLY),&e)){
			lll_print_error(&is,&e);
			goto _cleanup;
		}
		lll_free_identifier_data(&(c_dt.i_dt));
		fclose(of);
		of=NULL;
		fclose(f);
		f=NULL;
	}
	if (fp){
		free(fp);
	}
	return 0;
_cleanup:
	if (fp){
		free(fp);
	}
	lll_free_identifier_data(&(c_dt.i_dt));
	if (f){
		fclose(f);
	}
	if (of){
		fclose(of);
	}
	if (ffp){
		free(ffp);
	}
	return 1;
}
