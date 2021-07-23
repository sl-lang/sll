#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS 1
#define WIN32_LEAN_AND_MEAN 1
#include <windows.h>
#else
#include <linux/limits.h>
#endif
#include <lll_lib.h>
#include <generated.h>
#include <stdio.h>
#include <stdlib.h>



#ifdef _MSC_VER
#define MAX_PATH_LENGTH MAX_PATH
#define EXPAND_FILE_PATH(s,d) GetFullPathNameA((s),MAX_PATH,(d),NULL)
#else
#define MAX_PATH_LENGTH PATH_MAX
#define EXPAND_FILE_PATH(s,d) realpath((s),(d))
#endif



#define PRINT_STR_CONST(s) fwrite((void*)(s),sizeof(char),sizeof((s))/sizeof(char)-1,stdout)
#define _STR(x) #x
#define STR(x) _STR(x)
#define COMPILER_STACK_SIZE 65536
#define FLAG_EXPAND_PATH 1
#define FLAG_GENERATE_ASSEMBLY 2
#define FLAG_GENERATE_C 4
#define FLAG_GENERATE_COMPILED_OBJECT 8
#define FLAG_HELP 16
#define FLAG_MERGE_IMPORTS 32
#define FLAG_NO_LOGO 64
#define FLAG_NO_RUN 128
#define FLAG_PRINT_OBJECT 256
#define FLAG_VERBOSE 512
#define OPTIMIZE_LEVEL_GLOBAL_OPTIMIZE 2
#define OPTIMIZE_LEVEL_NO_OPTIMIZE 0
#define OPTIMIZE_LEVEL_REMOVE_PADDING 1
#define OPTIMIZE_LEVEL_STRIP_DEBUG_DATA 3



uint8_t st[COMPILER_STACK_SIZE];
uint8_t ol;
uint16_t fl;
char* i_fp;
uint32_t i_fpl;
char** fp;
uint32_t fpl;



uint8_t cmp_str(const char* a,const char* b){
	while (1){
		if (*a!=*b){
			return 0;
		}
		if (!(*a)){
			return 1;
		}
		a++;
		b++;
	}
}



void print_str(const char* s){
	while (*s){
		putchar(*s);
		s++;
	}
}



void print_int(int64_t v){
	if (!v){
		putchar('0');
		return;
	}
	if (v<0){
		v=-v;
		putchar('-');
	}
	char bf[20];
	uint8_t i=0;
	while (v){
		bf[i]=v%10;
		v/=10;
		i++;
	};
	while (i){
		i--;
		putchar(bf[i]+48);
	}
}



uint8_t load_file(const char* f_nm,lll_compilation_data_t* c_dt,FILE** f,lll_input_data_stream_t* is,char* f_fp){
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
				PRINT_STR_CONST("Trying to Open File '");
				print_str(bf);
				PRINT_STR_CONST("'...\n");
			}
			FILE* tf=fopen(bf,"rb");// lgtm [cpp/path-injection]
			if (tf){
				if (!(fl&FLAG_EXPAND_PATH)||!EXPAND_FILE_PATH(bf,f_fp)){
					*(f_fp+j)=0;
					while (j){
						j--;
						*(f_fp+j)=*(bf+j);
					}
				}
				if (fl&FLAG_VERBOSE){
					PRINT_STR_CONST("Found File '");
					print_str(f_fp);
					PRINT_STR_CONST("'\n");
				}
				*f=tf;
				lll_create_input_data_stream(*f,is);
				if (fl&FLAG_VERBOSE){
					PRINT_STR_CONST("Trying to Load File as Compiled Object...\n");
				}
				lll_error_t e;
				if (!lll_load_compiled_object(is,c_dt,&e)){
					lll_free_identifier_data(&(c_dt->i_dt));
					lll_free_import_data(&(c_dt->im));
					if (e.t==LLL_ERROR_INVALID_FILE_FORMAT){
						if (fl&FLAG_VERBOSE){
							PRINT_STR_CONST("File is not a Compiled Object. Falling Back to Standard Compilation...\n");
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
							PRINT_STR_CONST("File Successfully Read.\n");
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
								if (!load_file(nm,&n_c_dt,&n_f,&n_is,n_f_fp)){
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
									PRINT_STR_CONST("Merging Module '");
									print_str(n_f_fp);
									PRINT_STR_CONST("' ('");
									print_str(nm);
									PRINT_STR_CONST("', index ");
									print_int(l);
									PRINT_STR_CONST(") into '");
									print_str(f_fp);
									PRINT_STR_CONST("'...\n");
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
									PRINT_STR_CONST("Unable to Allocate Space for File Path Array\n");
									return 0;
								}
								fp=tmp;
								char* d=malloc(((c_dt->im.dt+l)->sz+1)*sizeof(char));
								if (!d){
									PRINT_STR_CONST("Unable to Allocate Space for Module Name\n");
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
						PRINT_STR_CONST("File Successfully Read.\n");
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
	PRINT_STR_CONST("Unable to Find File '");
	print_str(f_nm);
	PRINT_STR_CONST("'\n");
	return 0;
}



uint8_t write_asm(const char* fp,lll_compilation_data_t* c_dt,lll_error_t* e){
	if (fl&FLAG_VERBOSE){
		PRINT_STR_CONST("Writing Object to File '");
		print_str(fp);
		PRINT_STR_CONST("'...\n");
	}
	FILE* f=fopen(fp,"wb");
	if (!f){// lgtm [cpp/path-injection]
		PRINT_STR_CONST("Unable to Open Output File '");
		print_str(fp);
		PRINT_STR_CONST("'\n");
		return 0;
	}
	lll_output_data_stream_t os;
	lll_create_output_data_stream(f,&os);
	if (!lll_write_compiled_object(&os,c_dt,LLL_WRITE_MODE_ASSEMBLY,e)){
		fclose(f);
		return 0;
	}
	if (fl&FLAG_VERBOSE){
		PRINT_STR_CONST("File Written Successfully.\n");
	}
	fclose(f);
	return 1;
}



uint8_t write_c(const char* fp,lll_compilation_data_t* c_dt,lll_error_t* e){
	PRINT_STR_CONST("C Code Generation Unimplemented\n");
	return 0;
}



uint8_t write_lllc(const char* fp,lll_compilation_data_t* c_dt,lll_error_t* e){
	if (fl&FLAG_VERBOSE){
		PRINT_STR_CONST("Writing Object to File '");
		print_str(fp);
		PRINT_STR_CONST("'...\n");
	}
	FILE* f=fopen(fp,"wb");
	if (!f){// lgtm [cpp/path-injection]
		PRINT_STR_CONST("Unable to Open Output File '");
		print_str(fp);
		PRINT_STR_CONST("'\n");
		return 0;
	}
	lll_output_data_stream_t os;
	lll_create_output_data_stream(f,&os);
	if (!lll_write_compiled_object(&os,c_dt,LLL_WRITE_MODE_RAW,e)){
		fclose(f);
		return 0;
	}
	if (fl&FLAG_VERBOSE){
		PRINT_STR_CONST("File Written Successfully.\n");
	}
	fclose(f);
	return 1;
}



int main(int argc,const char** argv){
	int32_t ec=1;
	ol=OPTIMIZE_LEVEL_GLOBAL_OPTIMIZE;
	fl=0;
	i_fp=malloc(sizeof(char));
	if (!i_fp){
		PRINT_STR_CONST("Unable to Allocate Space for Include File Path Array\n");
		return 1;
	}
	*i_fp=0;
	i_fpl=1;
	fp=NULL;
	fpl=0;
	const char* o_fp=NULL;
	FILE* f=NULL;
	lll_compilation_data_t c_dt={0};
	uint32_t im_fpl=UINT32_MAX;
	for (int i=1;i<argc;i++){
		const char* e=argv[i];
		if ((*e=='-'&&*(e+1)=='a'&&*(e+2)==0)||cmp_str(e,"--generate-assembly")){
			fl|=FLAG_GENERATE_ASSEMBLY;
		}
		else if ((*e=='-'&&*(e+1)=='b'&&*(e+2)==0)||cmp_str(e,"--generate-c")){
			fl|=FLAG_GENERATE_C;
		}
		else if ((*e=='-'&&*(e+1)=='c'&&*(e+2)==0)||cmp_str(e,"--generate-compiled-object")){
			fl|=FLAG_GENERATE_COMPILED_OBJECT;
		}
		else if ((*e=='-'&&*(e+1)=='e'&&*(e+2)==0)||cmp_str(e,"--expand-file-paths")){
			fl|=FLAG_EXPAND_PATH;
		}
		else if ((*e=='-'&&*(e+1)=='f'&&*(e+2)==0)||cmp_str(e,"--file")){
			i++;
			if (i==argc){
				break;
			}
			goto _read_file_argument;
		}
		else if ((*e=='-'&&*(e+1)=='h'&&*(e+2)==0)||cmp_str(e,"--help")){
			fl|=FLAG_HELP;
		}
		else if ((*e=='-'&&*(e+1)=='I'&&*(e+2)==0)||cmp_str(e,"--include")){
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
					PRINT_STR_CONST("Unable to Allocate Space for Include File Path Array\n");
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
		else if ((*e=='-'&&*(e+1)=='L'&&*(e+2)==0)||cmp_str(e,"--no-logo")){
			fl|=FLAG_NO_LOGO;
		}
		else if ((*e=='-'&&*(e+1)=='m'&&*(e+2)==0)||cmp_str(e,"--merge-imports")){
			fl|=FLAG_MERGE_IMPORTS;
		}
		else if ((*e=='-'&&*(e+1)=='o'&&*(e+2)==0)||cmp_str(e,"--output")){
			if (o_fp){
				PRINT_STR_CONST("Multplie Output Files Supplied\n");
				goto _help;
			}
			i++;
			if (i==argc){
				break;
			}
			o_fp=argv[i];
		}
		else if (*e=='-'&&*(e+1)=='O'&&*(e+3)==0){
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
		else if ((*e=='-'&&*(e+1)=='p'&&*(e+2)==0)||cmp_str(e,"--print-objects")){
			fl|=FLAG_PRINT_OBJECT;
		}
		else if ((*e=='-'&&*(e+1)=='R'&&*(e+2)==0)||cmp_str(e,"--no-run")){
			fl|=FLAG_NO_RUN;
		}
		else if ((*e=='-'&&*(e+1)=='v'&&*(e+2)==0)||cmp_str(e,"--verbose")){
			fl|=FLAG_VERBOSE;
		}
		else if (*e=='-'){
_unkown_switch:
			PRINT_STR_CONST("Unknown Switch '");
			print_str((char*)e);
			PRINT_STR_CONST("'\n");
			goto _help;
		}
		else{
_read_file_argument:
			fpl++;
			void* tmp=realloc(fp,fpl*sizeof(char*));
			if (!tmp){
				PRINT_STR_CONST("Unable to Allocate Space for File Path Array\n");
				goto _error;
			}
			fp=tmp;
			*(fp+fpl-1)=(char*)e;
		}
	}
	im_fpl=fpl;
	if (!(fl&FLAG_NO_LOGO)){
		PRINT_STR_CONST("lll (Lisp Like Language) "STR(LLL_VERSION_MAJOR)"."STR(LLL_VERSION_MINOR)"."STR(LLL_VERSION_PATCH)" ("LLL_VERSION_BUILD_DATE", "LLL_VERSION_BUILD_TIME")\n");
	}
	if (fl&FLAG_VERBOSE){
		PRINT_STR_CONST("Configuration:\n  Optimization Level:\n");
		if (ol==OPTIMIZE_LEVEL_NO_OPTIMIZE){
			PRINT_STR_CONST("    No Optimization\n");
		}
		if (ol>=OPTIMIZE_LEVEL_REMOVE_PADDING){
			PRINT_STR_CONST("    Padding Reduction\n");
		}
		if (ol>=OPTIMIZE_LEVEL_GLOBAL_OPTIMIZE){
			PRINT_STR_CONST("    Global Optimization\n");
		}
		if (ol>=OPTIMIZE_LEVEL_STRIP_DEBUG_DATA){
			PRINT_STR_CONST("    Debug Data Stripping\n");
		}
		if (fl&FLAG_GENERATE_ASSEMBLY){
			PRINT_STR_CONST("  Assembly Generation Mode\n");
		}
		if (fl&FLAG_GENERATE_C){
			PRINT_STR_CONST("  C Soruce Code Generation Mode\n");
		}
		if (fl&FLAG_GENERATE_COMPILED_OBJECT){
			PRINT_STR_CONST("  Compiled Object Generation Mode\n");
		}
		if (fl&FLAG_EXPAND_PATH){
			PRINT_STR_CONST("  Path Expand Mode\n");
		}
		if (fl&FLAG_HELP){
			PRINT_STR_CONST("  Help Print Mode\n");
		}
		if (!(fl&FLAG_NO_LOGO)){
			PRINT_STR_CONST("  Compiler Logo Mode\n");
		}
		if (fl&FLAG_MERGE_IMPORTS){
			PRINT_STR_CONST("  Import Merge Mode\n");
		}
		if (fl&FLAG_PRINT_OBJECT){
			PRINT_STR_CONST("  Object Print Mode\n");
		}
		if (!(fl&FLAG_NO_RUN)){
			PRINT_STR_CONST("  Program Run Mode\n");
		}
		if (fl&FLAG_VERBOSE){
			PRINT_STR_CONST("  Verbose Mode\n");
		}
		PRINT_STR_CONST("Include Path: \n  - '");
		for (uint32_t i=0;i<i_fpl;i++){
			if (*(i_fp+i)){
				putchar(*(i_fp+i));
			}
			else{
				if (i+1<i_fpl){
					PRINT_STR_CONST("'\n  - '");
				}
				else{
					PRINT_STR_CONST("'\n");
				}
			}
		}
	}
	if (fl&FLAG_HELP){
		goto _help;
	}
	if (!fpl){
		PRINT_STR_CONST("No Input Files Supplied\n");
		goto _help;
	}
	for (uint32_t j=0;j<fpl;j++){
		lll_set_internal_stack(st,COMPILER_STACK_SIZE);
		char f_fp[MAX_PATH_LENGTH];
		lll_input_data_stream_t is;
		if (!load_file(*(fp+j),&c_dt,&f,&is,f_fp)){
			goto _error;
		}
		if (ol>=OPTIMIZE_LEVEL_GLOBAL_OPTIMIZE){
			if (fl&FLAG_VERBOSE){
				PRINT_STR_CONST("Performing Global Optimization...\n");
			}
			lll_error_t e;
			if (!lll_optimize_object(c_dt.h,&e)){
				lll_print_error(&is,&e);
				goto _error;
			}
		}
		if (ol>=OPTIMIZE_LEVEL_STRIP_DEBUG_DATA){
			if (fl&FLAG_VERBOSE){
				PRINT_STR_CONST("Removing Debug Data...\n");
			}
			lll_error_t e;
			if (!lll_remove_object_debug_data(c_dt.h,&e)){
				lll_print_error(&is,&e);
				goto _error;
			}
		}
		if (ol>=OPTIMIZE_LEVEL_REMOVE_PADDING){
			if (fl&FLAG_VERBOSE){
				PRINT_STR_CONST("Removing Object Padding...\n");
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
		if (fl&(FLAG_GENERATE_ASSEMBLY|FLAG_GENERATE_C|FLAG_GENERATE_COMPILED_OBJECT)){
			char bf[MAX_PATH_LENGTH];
			uint16_t i=0;
			if (!o_fp){
				while (*(f_fp+i)&&*(f_fp+i)!='.'){
					*(bf+i)=*(f_fp+i);
					i++;
				}
				bf[i]='.';
			}
			else{
				if (fpl==1&&!((fl&(FLAG_GENERATE_ASSEMBLY|FLAG_GENERATE_C|FLAG_GENERATE_COMPILED_OBJECT))&((fl&(FLAG_GENERATE_ASSEMBLY|FLAG_GENERATE_C|FLAG_GENERATE_COMPILED_OBJECT))-1))){
					if (fl&FLAG_GENERATE_ASSEMBLY){
						lll_error_t e={
							LLL_ERROR_UNKNOWN
						};
						if (!write_asm(o_fp,&c_dt,&e)){
							if (e.t!=LLL_ERROR_UNKNOWN){
								lll_print_error(&is,&e);
							}
							goto _error;
						}
					}
					else if (fl&FLAG_GENERATE_C){
						lll_error_t e;
						if (!write_c(o_fp,&c_dt,&e)){
							if (e.t!=LLL_ERROR_UNKNOWN){
								lll_print_error(&is,&e);
							}
							goto _error;
						}
					}
					else{
						lll_error_t e;
						if (!write_lllc(o_fp,&c_dt,&e)){
							if (e.t!=LLL_ERROR_UNKNOWN){
								lll_print_error(&is,&e);
							}
							goto _error;
						}
					}
					goto _skip_write;
				}
				else{
					while (*(o_fp+i)&&*(o_fp+i)!='.'){
						bf[i]=*(o_fp+i);
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
						bf[i]=*(f_fp+k);
						i++;
						k++;
					}
					bf[i]='.';
				}
			}
			if (fl&FLAG_GENERATE_ASSEMBLY){
				bf[i+1]='a';
				bf[i+2]='s';
				bf[i+3]='m';
				bf[i+4]=0;
				lll_error_t e={
					LLL_ERROR_UNKNOWN
				};
				if (!write_asm(bf,&c_dt,&e)){
					if (e.t!=LLL_ERROR_UNKNOWN){
						lll_print_error(&is,&e);
					}
					goto _error;
				}
			}
			if (fl&FLAG_GENERATE_C){
				bf[i+1]='c';
				bf[i+2]=0;
				lll_error_t e={
					LLL_ERROR_UNKNOWN
				};
				if (!write_c(bf,&c_dt,&e)){
					if (e.t!=LLL_ERROR_UNKNOWN){
						lll_print_error(&is,&e);
					}
					goto _error;
				}
			}
			if (fl&FLAG_GENERATE_COMPILED_OBJECT){
				bf[i+1]='l';
				bf[i+2]='l';
				bf[i+3]='l';
				bf[i+4]='c';
				bf[i+5]=0;
				lll_error_t e={
					LLL_ERROR_UNKNOWN
				};
				if (!write_lllc(bf,&c_dt,&e)){
					if (e.t!=LLL_ERROR_UNKNOWN){
						lll_print_error(&is,&e);
					}
					goto _error;
				}
			}
_skip_write:;
		}
		if (!(fl&FLAG_NO_RUN)){
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
	PRINT_STR_CONST(HELP_TEXT);
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
	return ec;
}
