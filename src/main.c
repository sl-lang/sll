#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS 1
#define WIN32_LEAN_AND_MEAN 1
#include <windows.h>
#else
#include <linux/limits.h>
#endif
#include <lll_lib.h>
#include <help_generated.h>
#include <stdio.h>
#include <stdlib.h>



#ifdef _MSC_VER
#define MAX_PATH_LENGTH MAX_PATH
#define EXPAND_FILE_PATH(s,d) GetFullPathNameA((s),MAX_PATH,(d),NULL)
#else
#define MAX_PATH_LENGTH PATH_MAX
#define EXPAND_FILE_PATH(s,d) realpath((s),(d))
#endif
#define OPTIMIZE_LEVEL_NO_OPTIMIZE 0
#define OPTIMIZE_LEVEL_REMOVE_PADDING 1
#define OPTIMIZE_LEVEL_GLOBAL_OPTIMIZE 2
#define OPTIMIZE_LEVEL_STRIP_DEBUG_DATA 3
#define FLAG_VERBOSE 1
#define FLAG_COMPILE_ONLY 2
#define FLAG_FULL_PATH 4
#define FLAG_PRINT_OBJECT 8
#define FLAG_MERGE_IMPORTS 16
#define FLAG_HELP 32
#define FLAG_LOGO 64
#define FLAG_RUN 128
#define FLAG_GENERATE_OUTPUT 256
#define DEFAULT_OPTIMIZE_LEVEL OPTIMIZE_LEVEL_GLOBAL_OPTIMIZE
#define COMPILER_STACK_SIZE 65536
#define _STR(x) #x
#define STR(x) _STR(x)



uint8_t st[COMPILER_STACK_SIZE];
uint8_t ol;
uint16_t fl;
char* i_fp;
uint32_t i_fpl;
char** fp;
uint32_t fpl;



uint8_t _load_file(char* f_nm,lll_compilation_data_t* c_dt,FILE** f,lll_input_data_stream_t* is,char* f_fp){
	char bf[MAX_PATH_LENGTH];
	uint32_t j=0;
	for (uint32_t i=0;i<i_fpl;i++){
		if (!(*(i_fp+i))){
			uint32_t k=0;
			while (*(f_nm+k)){
				*(bf+j)=*(f_nm+k);
				j++;
				k++;
			}
			*(bf+j)=0;
			if (fl&FLAG_VERBOSE){
				printf("Trying to Open File '%s'...\n",bf);
			}
			FILE* tf=fopen(bf,"rb");// lgtm [cpp/path-injection]
			if (tf){
				if (!(fl&FLAG_FULL_PATH)||!EXPAND_FILE_PATH(bf,f_fp)){
					*(f_fp+j)=0;
					while (j){
						j--;
						*(f_fp+j)=*(bf+j);
					}
				}
				if (fl&FLAG_VERBOSE){
					printf("Found File '%s'\n",f_fp);
				}
				*f=tf;
				lll_create_input_data_stream(*f,is);
				if (fl&FLAG_VERBOSE){
					printf("Trying to Load File as Compiled Object...\n");
				}
				lll_error_t e;
				if (!lll_load_compiled_object(is,c_dt,&e)){
					lll_free_identifier_data(&(c_dt->i_dt));
					lll_free_import_data(&(c_dt->im));
					if (e.t==LLL_ERROR_INVALID_FILE_FORMAT){
						if (fl&FLAG_VERBOSE){
							printf("File is not a Compiled Object. Falling Back to Standard Compilation...\n");
						}
						lll_create_input_data_stream(*f,is);
						lll_init_compilation_data(f_fp,is,c_dt);
						if (!lll_read_all_objects(c_dt,&e)){
							lll_print_error(is,&e);
							return 0;
						}
						if (fl&FLAG_PRINT_OBJECT){
							lll_print_object(c_dt,c_dt->h,stdout);
							putchar('\n');
						}
						if (fl&FLAG_VERBOSE){
							printf("File Successfully Read.\n");
						}
						if (fl&FLAG_MERGE_IMPORTS){
							for (uint32_t l=0;l<c_dt->im.l;l++){
								char nm[MAX_PATH_LENGTH];
								char* s=(c_dt->im.dt+l)->nm;
								uint32_t m=0;
								for (;m<(c_dt->im.dt+l)->sz;m++){
									*(nm+m)=*(s+m);
								}
								*(nm+m)=0;
								lll_stack_context_t s_ctx;
								lll_save_stack_context(&s_ctx);
								uint8_t n_st[COMPILER_STACK_SIZE];
								lll_compilation_data_t n_c_dt={0};
								FILE* n_f=NULL;
								lll_input_data_stream_t n_is;
								char n_f_fp[MAX_PATH_LENGTH];
								lll_set_internal_stack(n_st,COMPILER_STACK_SIZE);
								if (!_load_file(nm,&n_c_dt,&n_f,&n_is,n_f_fp)){
									if (n_f){
										fclose(n_f);
									}
									lll_load_stack_context(&s_ctx);
									lll_free_file_path_data(&(n_c_dt.fp_dt));
									lll_free_identifier_data(&(n_c_dt.i_dt));
									lll_free_import_data(&(n_c_dt.im));
									lll_free_function_data(&(n_c_dt.f_dt));
									return 0;
								}
								if (n_f){
									fclose(n_f);
								}
								lll_load_stack_context(&s_ctx);
								if (fl&FLAG_VERBOSE){
									printf("Merging Module '%s' ('%s', index %u) into '%s'...\n",n_f_fp,nm,l,f_fp);
								}
								if (!lll_merge_import(c_dt,l,&n_c_dt,&e)){
									lll_free_file_path_data(&(n_c_dt.fp_dt));
									lll_free_identifier_data(&(n_c_dt.i_dt));
									lll_free_import_data(&(n_c_dt.im));
									lll_free_function_data(&(n_c_dt.f_dt));
									lll_print_error(is,&e);
									return 0;
								}
								lll_free_file_path_data(&(n_c_dt.fp_dt));
								lll_free_identifier_data(&(n_c_dt.i_dt));
								lll_free_import_data(&(n_c_dt.im));
								lll_free_function_data(&(n_c_dt.f_dt));
							}
						}
						else{
							for (uint32_t l=0;l<c_dt->im.l;l++){
								fpl++;
								void* tmp=realloc(fp,fpl*sizeof(char*));
								if (!tmp){
									printf("Unable to Allocate Space for File Path Array\n");
									return 0;
								}
								fp=tmp;
								char* d=malloc(((c_dt->im.dt+l)->sz+1)*sizeof(char));
								if (!d){
									printf("Unable to Allocate Space for Module Name\n");
									return 0;
								}
								char* s=(c_dt->im.dt+l)->nm;
								uint32_t m=0;
								for (;m<(c_dt->im.dt+l)->sz;m++){
									*(d+m)=*(s+m);
								}
								*(d+m)=0;
								*(fp+fpl-1)=d;
							}
						}
					}
					else{
						lll_print_error(is,&e);
						return 0;
					}
				}
				else{
					if (fl&FLAG_PRINT_OBJECT){
						lll_print_object(c_dt,c_dt->h,stdout);
						putchar('\n');
					}
					if (fl&FLAG_VERBOSE){
						printf("File Successfully Read.\n");
					}
				}
				return 1;
			}
			j=0;
			continue;
		}
		*(bf+j)=*(i_fp+i);
		j++;
	}
	printf("Unable to Find File '%s'\n",f_nm);
	return 0;
}



int main(int argc,const char** argv){
	int32_t ec=1;
	ol=DEFAULT_OPTIMIZE_LEVEL;
	fl=FLAG_LOGO|FLAG_GENERATE_OUTPUT;
	i_fp=malloc(sizeof(char));
	if (!i_fp){
		printf("Unable to Allocate Space for Include File Path Array\n");
		return 1;
	}
	*i_fp=0;
	i_fpl=1;
	fp=NULL;
	fpl=0;
	const char* o_fp=NULL;
	FILE* f=NULL;
	FILE* of=NULL;
	lll_compilation_data_t c_dt={0};
	uint32_t im_fpl=UINT32_MAX;
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
		else if (*e=='-'&&*(e+1)=='I'&&*(e+2)==0){
			i++;
			if (i==argc){
				break;
			}
			e=argv[i];
			uint32_t sz=0;
			while (*(e+sz)){
				sz++;
			}
			if (sz){
				uint32_t j=i_fpl;
				i_fpl+=sz+(*(e+sz-1)!='\\'&&*(e+sz-1)!='/'?2:1);
				void* tmp=realloc(i_fp,i_fpl*sizeof(char));
				if (!tmp){
					printf("Unable to Allocate Space for Include File Path Array\n");
					goto _error;
				}
				i_fp=tmp;
				for (uint32_t k=0;k<sz;k++){
					*(i_fp+j)=*(e+k);
					j++;
				}
				if (*(e+sz-1)!='\\'&&*(e+sz-1)!='/'){
					*(i_fp+j)='/';
					j++;
				}
				*(i_fp+j)=0;
			}
		}
		else if (*e=='-'&&*(e+1)=='f'&&*(e+2)==0){
			i++;
			if (i==argc){
				break;
			}
			goto _read_file_argument;
		}
		else if (*e=='-'&&*(e+1)=='o'&&*(e+2)==0){
			if (o_fp){
				printf("Multplie Output Files Supplied\n");
				goto _help;
			}
			i++;
			if (i==argc){
				break;
			}
			o_fp=argv[i];
		}
		else if (*e=='-'&&*(e+1)=='o'&&*(e+2)=='0'&&*(e+3)==0){
			o_fp=NULL;
		}
		else if (*e=='-'&&*(e+1)=='h'&&*(e+2)==0){
			fl|=FLAG_HELP;
		}
		else if (*e=='-'&&*(e+1)=='L'&&*(e+2)==0){
			fl|=FLAG_LOGO;
		}
		else if (*e=='-'&&*(e+1)=='L'&&*(e+2)=='0'&&*(e+3)==0){
			fl&=~FLAG_LOGO;
		}
		else if (*e=='-'&&*(e+1)=='v'&&*(e+2)==0){
			fl|=FLAG_VERBOSE;
		}
		else if (*e=='-'&&*(e+1)=='v'&&*(e+2)=='0'&&*(e+3)==0){
			fl&=~FLAG_VERBOSE;
		}
		else if (*e=='-'&&*(e+1)=='m'&&*(e+2)==0){
			fl|=FLAG_MERGE_IMPORTS;
		}
		else if (*e=='-'&&*(e+1)=='m'&&*(e+2)=='0'&&*(e+3)==0){
			fl&=~FLAG_MERGE_IMPORTS;
		}
		else if (*e=='-'&&*(e+1)=='p'&&*(e+2)==0){
			fl|=FLAG_PRINT_OBJECT;
		}
		else if (*e=='-'&&*(e+1)=='p'&&*(e+2)=='0'&&*(e+3)==0){
			fl&=~FLAG_PRINT_OBJECT;
		}
		else if (*e=='-'&&*(e+1)=='c'&&*(e+2)==0){
			fl|=FLAG_COMPILE_ONLY;
		}
		else if (*e=='-'&&*(e+1)=='c'&&*(e+2)=='0'&&*(e+3)==0){
			fl&=~FLAG_COMPILE_ONLY;
		}
		else if (*e=='-'&&*(e+1)=='r'&&*(e+2)==0){
			fl|=FLAG_RUN;
		}
		else if (*e=='-'&&*(e+1)=='r'&&*(e+2)=='0'&&*(e+3)==0){
			fl&=~FLAG_RUN;
		}
		else if (*e=='-'&&*(e+1)=='f'&&*(e+2)=='p'&&*(e+3)==0){
			fl|=FLAG_FULL_PATH;
		}
		else if (*e=='-'&&*(e+1)=='f'&&*(e+2)=='p'&&*(e+3)=='0'&&*(e+4)==0){
			fl&=~FLAG_FULL_PATH;
		}
		else if (*e=='-'&&*(e+1)=='g'&&*(e+2)==0){
			fl|=FLAG_GENERATE_OUTPUT;
		}
		else if (*e=='-'&&*(e+1)=='g'&&*(e+2)=='0'&&*(e+3)==0){
			fl&=~FLAG_GENERATE_OUTPUT;
		}
		else if (*e=='-'){
_unkown_switch:
			printf("Unknown Switch: '%s'\n",e);
			goto _help;
		}
		else{
_read_file_argument:
			fpl++;
			void* tmp=realloc(fp,fpl*sizeof(char*));
			if (!tmp){
				printf("Unable to Allocate Space for File Path Array\n");
				goto _error;
			}
			fp=tmp;
			*(fp+fpl-1)=(char*)e;
		}
	}
	im_fpl=fpl;
	if (fl&FLAG_LOGO){
		printf("lll (Lisp Like Language) "STR(LLL_VERSION_MAJOR)"."STR(LLL_VERSION_MINOR)"."STR(LLL_VERSION_PATCH)" ("LLL_VERSION_BUILD_DATE", "LLL_VERSION_BUILD_TIME")\n");
	}
	if (fl&FLAG_HELP){
		goto _help;
	}
	if ((fl&FLAG_COMPILE_ONLY)&&(fl&FLAG_RUN)){
		printf("Option -c is not compatible with option -r\n");
		goto _help;
	}
	if (!fpl){
		printf("No Input Files Supplied\n");
		goto _help;
	}
	if (fl&FLAG_VERBOSE){
		printf("Configuration:\n  Optimization Level:\n");
		if (ol==OPTIMIZE_LEVEL_NO_OPTIMIZE){
			printf("    No Optimization\n");
		}
		if (ol>=OPTIMIZE_LEVEL_REMOVE_PADDING){
			printf("    Padding Reduction (OPTIMIZE_LEVEL_REMOVE_PADDING)\n");
		}
		if (ol>=OPTIMIZE_LEVEL_GLOBAL_OPTIMIZE){
			printf("    Global Optimization (OPTIMIZE_LEVEL_GLOBAL_OPTIMIZE)\n");
		}
		if (ol>=OPTIMIZE_LEVEL_STRIP_DEBUG_DATA){
			printf("    Debug Data Stripping (OPTIMIZE_LEVEL_STRIP_DEBUG_DATA)\n");
		}
		if (fl&FLAG_VERBOSE){
			printf("  Verbose Mode (FLAG_VERBOSE)\n");
		}
		if (fl&FLAG_COMPILE_ONLY){
			printf("  Compile Only Mode (FLAG_COMPILE_ONLY)\n");
		}
		if (fl&FLAG_FULL_PATH){
			printf("  Full Path Mode (FLAG_FULL_PATH)\n");
		}
		if (fl&FLAG_PRINT_OBJECT){
			printf("  Object Print Mode (FLAG_PRINT_OBJECT)\n");
		}
		if (fl&FLAG_MERGE_IMPORTS){
			printf("  Import Merge Mode (FLAG_MERGE_IMPORTS)\n");
		}
		if (fl&FLAG_HELP){
			printf("  Help Print Mode (FLAG_HELP)\n");
		}
		if (fl&FLAG_LOGO){
			printf("  Compiler Logo Mode (FLAG_LOGO)\n");
		}
		if (fl&FLAG_RUN){
			printf("  Program Run Mode (FLAG_RUN)\n");
		}
		if (fl&FLAG_GENERATE_OUTPUT){
			printf("  Output Generation Mode (FLAG_GENERATE_OUTPUT)\n");
		}
		printf("Include Path: \n  - '");
		for (uint32_t i=0;i<i_fpl;i++){
			if (*(i_fp+i)){
				putchar(*(i_fp+i));
			}
			else{
				if (i+1<i_fpl){
					printf("'\n  - '");
				}
				else{
					printf("'\n");
				}
			}
		}
	}
	for (uint32_t j=0;j<fpl;j++){
		lll_set_internal_stack(st,COMPILER_STACK_SIZE);
		char f_fp[MAX_PATH_LENGTH];
		lll_input_data_stream_t is;
		if (!_load_file(*(fp+j),&c_dt,&f,&is,f_fp)){
			goto _error;
		}
		if (ol>=OPTIMIZE_LEVEL_GLOBAL_OPTIMIZE){
			if (fl&FLAG_VERBOSE){
				printf("Performing Global Optimization...\n");
			}
			lll_error_t e;
			if (!lll_optimize_object(c_dt.h,&e)){
				lll_print_error(&is,&e);
				goto _error;
			}
		}
		if (ol>=OPTIMIZE_LEVEL_STRIP_DEBUG_DATA){
			if (fl&FLAG_VERBOSE){
				printf("Removing Debug Data...\n");
			}
			lll_error_t e;
			if (!lll_remove_object_debug_data(c_dt.h,&e)){
				lll_print_error(&is,&e);
				goto _error;
			}
		}
		if (ol>=OPTIMIZE_LEVEL_REMOVE_PADDING){
			if (fl&FLAG_VERBOSE){
				printf("Removing Object Padding...\n");
			}
			lll_error_t e;
			if (!lll_remove_object_padding(&c_dt,c_dt.h,&e)){
				lll_print_error(&is,&e);
				goto _error;
			}
		}
		if (fl&FLAG_PRINT_OBJECT){
			lll_print_object(&c_dt,c_dt.h,stdout);
			putchar('\n');
		}
		if (fl&FLAG_GENERATE_OUTPUT){
			if (!o_fp){
				char bf[MAX_PATH_LENGTH];
				uint16_t i=0;
				while (*(f_fp+i)&&*(f_fp+i)!='.'){
					*(bf+i)=*(f_fp+i);
					i++;
				}
				bf[i]='.';
				if (fl&FLAG_COMPILE_ONLY){
					bf[i+1]='l';
					bf[i+2]='l';
					bf[i+3]='l';
					bf[i+4]='c';
					bf[i+5]=0;
				}
				else{
					bf[i+1]='a';
					bf[i+2]='s';
					bf[i+3]='m';
					bf[i+4]=0;
				}
				if (fl&FLAG_VERBOSE){
					printf("Writing Object to File '%s'...\n",bf);
				}
				if (!(of=fopen(bf,"wb"))){// lgtm [cpp/path-injection]
					printf("Unable to Open Output File '%s'\n",bf);
					goto _error;
				}
			}
			else{
				if (fpl==1){
					if (fl&FLAG_VERBOSE){
						printf("Writing Object to File '%s'...\n",o_fp);
					}
					if (!(of=fopen(o_fp,"wb"))){// lgtm [cpp/path-injection]
						printf("Unable to Open Output File '%s'\n",o_fp);
						goto _error;
					}
				}
				else{
					char bf[MAX_PATH_LENGTH];
					uint16_t i=0;
					while (*(o_fp+i)&&*(o_fp+i)!='.'){
						*(bf+i)=*(o_fp+i);
						i++;
					}
					i--;
					while (*(o_fp+i)!='\\'&&*(o_fp+i)!='/'){
						if (i==0){
							i--;
							break;
						}
						i--;
					}
					i++;
					uint32_t j=0;
					uint32_t k=0;
					while (*(f_fp+j)&&*(f_fp+j)!='.'){
						if (*(f_fp+j)=='\\'||*(f_fp+j)=='/'){
							k=j+1;
						}
						j++;
					}
					while (k<j){
						*(bf+i)=*(f_fp+k);
						i++;
						k++;
					}
					*(bf+i)=0;
					*(bf+i)='.';
					if (fl&FLAG_COMPILE_ONLY){
						*(bf+i+1)='l';
						*(bf+i+2)='l';
						*(bf+i+3)='l';
						*(bf+i+4)='c';
						*(bf+i+5)=0;
					}
					else{
						*(bf+i+1)='a';
						*(bf+i+2)='s';
						*(bf+i+3)='m';
						*(bf+i+4)=0;
					}
					if (fl&FLAG_VERBOSE){
						printf("Writing Object to File '%s'...\n",bf);
					}
					if (!(of=fopen(bf,"wb"))){// lgtm [cpp/path-injection]
						printf("Unable to Open Output File '%s'\n",bf);
						goto _error;
					}
				}
			}
			lll_output_data_stream_t os;
			lll_create_output_data_stream(of,&os);
			lll_error_t e;
			if (!lll_write_compiled_object(&os,&c_dt,(fl&FLAG_COMPILE_ONLY?LLL_WRITE_MODE_RAW:LLL_WRITE_MODE_ASSEMBLY),&e)){
				lll_print_error(&is,&e);
				goto _error;
			}
			if (fl&FLAG_VERBOSE){
				printf("File Successfully Written.\n");
			}
			fclose(of);
			of=NULL;
		}
		if (fl&FLAG_RUN){
			lll_input_data_stream_t ris;
			lll_output_data_stream_t ros;
			lll_create_input_data_stream(stdin,&ris);
			lll_create_output_data_stream(stdout,&ros);
			lll_return_code_t r=lll_run_compiled_object(&c_dt,&ris,&ros);
			if (r){
				ec=r;
				goto _error;
			}
		}
		fclose(f);
		f=NULL;
		lll_free_file_path_data(&(c_dt.fp_dt));
		lll_free_identifier_data(&(c_dt.i_dt));
		lll_free_import_data(&(c_dt.im));
		lll_free_function_data(&(c_dt.f_dt));
	}
	while (im_fpl<fpl){
		free(*(fp+im_fpl));
		im_fpl++;
	}
	free(i_fp);
	if (fp){
		free(fp);
	}
	return 0;
_help:
	printf(HELP_TEXT);
_error:
	while (im_fpl<fpl){
		free(*(fp+im_fpl));
		im_fpl++;
	}
	free(i_fp);
	if (fp){
		free(fp);
	}
	lll_free_file_path_data(&(c_dt.fp_dt));
	lll_free_identifier_data(&(c_dt.i_dt));
	lll_free_import_data(&(c_dt.im));
	lll_free_function_data(&(c_dt.f_dt));
	if (f){
		fclose(f);
	}
	if (of){
		fclose(of);
	}
	return ec;
}
