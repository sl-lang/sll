#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS 1
#define WIN32_LEAN_AND_MEAN 1
#include <windows.h>
#else
#include <linux/limits.h>
#endif
#include <lll_lib.h>
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
#define DEFAULT_OPTIMIZE_LEVEL OPTIMIZE_LEVEL_GLOBAL_OPTIMIZE
#define COMPILER_STACK_SIZE 65536



uint8_t st[COMPILER_STACK_SIZE];
uint8_t ol;
uint8_t fl;
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
							for (i=0;i<c_dt->im.l;i++){
								char nm[MAX_PATH_LENGTH];
								char* s=(c_dt->im.dt+i)->nm;
								j=0;
								for (;j<(c_dt->im.dt+i)->sz;j++){
									*(nm+j)=*(s+j);
								}
								*(nm+j)=0;
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
									lll_free_identifier_data(&(n_c_dt.i_dt));
									lll_free_import_data(&(n_c_dt.im));
									return 0;
								}
								if (n_f){
									fclose(n_f);
								}
								lll_load_stack_context(&s_ctx);
								if (fl&FLAG_VERBOSE){
									printf("Merging Module '%s' ('%s', index %u) into '%s'...\n",n_f_fp,nm,i,f_fp);
								}
								if (!lll_merge_import(c_dt,i,&n_c_dt,&e)){
									lll_free_identifier_data(&(n_c_dt.i_dt));
									lll_free_import_data(&(n_c_dt.im));
									lll_print_error(is,&e);
									return 0;
								}
								lll_free_identifier_data(&(n_c_dt.i_dt));
								lll_free_import_data(&(n_c_dt.im));
							}
						}
						else{
							for (i=0;i<c_dt->im.l;i++){
								fpl++;
								fp=realloc(fp,fpl*sizeof(char*));
								char* d=malloc(((c_dt->im.dt+i)->sz+1)*sizeof(char));
								char* s=(c_dt->im.dt+i)->nm;
								j=0;
								for (;j<(c_dt->im.dt+i)->sz;j++){
									*(d+j)=*(s+j);
								}
								*(d+j)=0;
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
	ol=DEFAULT_OPTIMIZE_LEVEL;
	fl=0;
	i_fp=malloc(sizeof(char));
	*i_fp=0;
	i_fpl=1;
	fp=NULL;
	fpl=0;
	const char* o_fp=NULL;
	FILE* f=NULL;
	FILE* of=NULL;
	lll_compilation_data_t c_dt={0};
	int i=1;
	uint32_t im_fpl=UINT32_MAX;
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
		else if (*e=='-'&&*(e+1)=='I'&&*(e+2)==0){
			i++;
			if (i<argc){
				e=argv[i];
				uint32_t sz=0;
				while (*(e+sz)){
					sz++;
				}
				if (sz){
					if (*(e+sz-1)!='\\'&&*(e+sz-1)!='/'){
						sz+=2;
						uint32_t j=i_fpl;
						i_fpl+=sz;
						i_fp=realloc(i_fp,i_fpl*sizeof(char));
						for (uint32_t k=0;k<sz-2;k++){
							*(i_fp+j)=*(e+k);
							j++;
						}
						*(i_fp+j)='/';
						*(i_fp+j+1)=0;
					}
					else{
						sz++;
						uint32_t j=i_fpl;
						i_fpl+=sz;
						i_fp=realloc(i_fp,i_fpl*sizeof(char));
						for (uint32_t k=0;k<sz;k++){
							*(i_fp+j)=*(e+k);
							j++;
						}
					}
				}
			}
		}
		else if (*e=='-'&&*(e+1)=='m'&&*(e+2)==0){
			fl|=FLAG_MERGE_IMPORTS;
		}
		else if (*e=='-'&&*(e+1)=='p'&&*(e+2)==0){
			fl|=FLAG_PRINT_OBJECT;
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
				goto _error;
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
			goto _error;
		}
		else{
			fpl++;
			void* tmp=realloc(fp,fpl*sizeof(char*));
			if (!tmp){
				printf("Unable to Allocate Space for File Path Array\n");
				goto _error;
			}
			fp=tmp;
			*(fp+fpl-1)=(char*)e;
		}
		i++;
	}
	if (!fpl){
		printf("Not Input Files Supplied\n");
		goto _error;
	}
	im_fpl=fpl;
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
		if (fl&FLAG_PRINT_OBJECT){
			printf("  Object Print Mode\n");
		}
		if (fl&FLAG_MERGE_IMPORTS){
			printf("  Import Merge Mode\n");
		}
		printf("Include Path: \n - '");
		for (uint32_t i=0;i<i_fpl;i++){
			if (*(i_fp+i)){
				putchar(*(i_fp+i));
			}
			else{
				if (i+1<i_fpl){
					printf("'\n - '");
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
			if (!lll_remove_object_padding(c_dt.h,&e)){
				lll_print_error(&is,&e);
				goto _error;
			}
		}
		if (fl&FLAG_PRINT_OBJECT){
			lll_print_object(&c_dt,c_dt.h,stdout);
			putchar('\n');
		}
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
		fclose(f);
		f=NULL;
		fclose(of);
		of=NULL;
		lll_free_identifier_data(&(c_dt.i_dt));
		lll_free_import_data(&(c_dt.im));
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
_error:
	while (im_fpl<fpl){
		free(*(fp+im_fpl));
		im_fpl++;
	}
	free(i_fp);
	if (fp){
		free(fp);
	}
	lll_free_identifier_data(&(c_dt.i_dt));
	lll_free_import_data(&(c_dt.im));
	if (f){
		fclose(f);
	}
	if (of){
		fclose(of);
	}
	return 1;
}
