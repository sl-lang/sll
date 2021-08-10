#ifdef _MSC_VER
#define WIN32_LEAN_AND_MEAN 1
#include <windows.h>
#else
#include <linux/limits.h>
#include <unistd.h>
#endif
#ifdef STANDALONE_BUILD
#include <sll_standalone.h>
#include <compiled_modules.h>
#else
#include <sll.h>
#endif
#include <help_text.h>
#include <stdio.h>
#include <stdlib.h>



#ifdef _MSC_VER
#define MAX_PATH_LENGTH MAX_PATH
#define EXPAND_FILE_PATH(s,d) GetFullPathNameA((s),MAX_PATH,(d),NULL)
#define GET_EXECUATBLE_FILE_PATH(bf,l,o) \
	do{ \
		(o)=GetModuleFileNameA(NULL,(bf),(l)); \
	} while (0)
#else
#define MAX_PATH_LENGTH PATH_MAX
#define EXPAND_FILE_PATH(s,d) realpath((s),(d))
#define GET_EXECUATBLE_FILE_PATH(bf,l,o) \
	do{ \
		ssize_t __o=readlink("/proc/self/exe",(bf),(l)); \
		if (__o!=-1){ \
			(o)=__o; \
			*((bf)+(o))=0; \
		} \
		else{ \
			(o)=0; \
			*(bf)=0; \
		} \
	} while (0)
#endif



#define PRINT_STATIC_STR(s) fwrite((void*)(s),sizeof(char),sizeof((s))/sizeof(char)-1,stdout)
#define _STR(x) #x
#define STR(x) _STR(x)
#define ASSEMBLY_STACK_SIZE 16384
#define COMPILER_STACK_SIZE 65536
#define VM_STACK_SIZE 65536
#define FLAG_EXPAND_PATH 1
#define FLAG_GENERATE_ASSEMBLY 2
#define FLAG_GENERATE_COMPILED_OBJECT 4
#define FLAG_HELP 8
#define FLAG_NO_LOGO 16
#define FLAG_NO_RUN 32
#define FLAG_PRINT_ASSEMBLY 64
#define FLAG_PRINT_OBJECT 128
#define FLAG_VERBOSE 256
#define _FLAG_ASSEMBLY_GENERATED 512
#define OPTIMIZE_LEVEL_NO_OPTIMIZE 0
#define OPTIMIZE_LEVEL_REMOVE_PADDING 1
#define OPTIMIZE_LEVEL_STRIP_DEBUG_DATA 2
#define OPTIMIZE_LEVEL_STRIP_GLOBAL_OPTIMIZE 3



uint8_t a_st[ASSEMBLY_STACK_SIZE];
uint8_t c_st[COMPILER_STACK_SIZE];
uint8_t vm_st[VM_STACK_SIZE];
uint8_t ol;
uint16_t fl;
char* i_fp;
uint32_t i_fpl;
char** fp;
uint32_t fpl;
#ifndef STANDALONE_BUILD
char l_fp[MAX_PATH_LENGTH];
uint32_t l_fpl;
#else
sll_input_buffer_t i_bf;
#endif
sll_internal_function_table_t i_ft;



uint8_t load_file(const char* f_nm,sll_assembly_data_t* a_dt,sll_compilation_data_t* c_dt,FILE** f,sll_input_data_stream_t* is,char* f_fp);



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



sll_return_t load_import(const sll_string_t* nm,sll_compilation_data_t* o,sll_error_t* e){
	char bf[MAX_PATH_LENGTH];
	for (sll_string_length_t m=0;m<nm->l;m++){
		bf[m]=nm->v[m];
	}
	bf[nm->l]=0;
	FILE* f=NULL;
	sll_assembly_data_t a_dt={0};
	sll_input_data_stream_t is;
	char f_fp[MAX_PATH_LENGTH];
	uint8_t st[COMPILER_STACK_SIZE];
	sll_set_assembly_data_stack(&a_dt,a_st,ASSEMBLY_STACK_SIZE);
	sll_init_compilation_data(bf,&is,o);
	sll_set_compilation_data_stack(o,st,COMPILER_STACK_SIZE);
	if (!load_file(bf,&a_dt,o,&f,&is,f_fp)){
		if (f){
			fclose(f);
		}
		sll_free_compilation_data(o);
		e->t=SLL_ERROR_UNKNOWN;
		return SLL_RETURN_ERROR;
	}
	if (fl&_FLAG_ASSEMBLY_GENERATED){
		sll_free_assembly_function_table(&(a_dt.ft));
		sll_free_string_table(&(a_dt.st));
		PRINT_STATIC_STR("Importing Assembly into Compiled Objects is Not Allowed\n");
		e->t=SLL_ERROR_UNKNOWN;
		return SLL_RETURN_ERROR;
	}
	if (f){
		fclose(f);
	}
	return SLL_RETURN_NO_ERROR;
}



uint8_t load_file(const char* f_nm,sll_assembly_data_t* a_dt,sll_compilation_data_t* c_dt,FILE** f,sll_input_data_stream_t* is,char* f_fp){
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
			*(bf+j)='.';
			*(bf+j+1)='s';
			*(bf+j+2)='l';
			*(bf+j+3)='l';
			*(bf+j+4)='c';
			*(bf+j+5)=0;
			if (fl&FLAG_VERBOSE){
				PRINT_STATIC_STR("Trying to Open File '");
				print_str(bf);
				PRINT_STATIC_STR("'...\n");
			}
			FILE* nf=fopen(bf,"rb");// lgtm [cpp/path-injection]
			if (nf){
				if (!(fl&FLAG_EXPAND_PATH)||!EXPAND_FILE_PATH(bf,f_fp)){
					*(f_fp+j)=0;
					while (j){
						j--;
						*(f_fp+j)=*(bf+j);
					}
				}
				if (fl&FLAG_VERBOSE){
					PRINT_STATIC_STR("Found File '");
					print_str(f_fp);
					PRINT_STATIC_STR("'\n");
				}
				*f=nf;
				sll_stream_create_input_from_file(nf,is);
				sll_error_t e;
				if (!sll_load_compiled_object(is,c_dt,&e)){
					sll_free_identifier_table(&(c_dt->idt));
					sll_free_export_table(&(c_dt->et));
					sll_free_function_table(&(c_dt->ft));
					sll_free_string_table(&(c_dt->st));
					if (e.t==SLL_ERROR_INVALID_FILE_FORMAT){
						PRINT_STATIC_STR("File '");
						print_str(f_fp);
						PRINT_STATIC_STR("'is not a Compiled Object.\n");
					}
					else{
						sll_print_error(is,&e);
					}
					return 0;
				}
				if (fl&FLAG_PRINT_OBJECT){
					sll_output_data_stream_t os;
					sll_stream_create_output_from_file(stdout,&os);
					sll_print_object(c_dt,c_dt->h,&os);
					putchar('\n');
				}
				if (fl&FLAG_VERBOSE){
					PRINT_STATIC_STR("File Successfully Read.\n");
				}
				return 1;
			}
			*(bf+j)=0;
			if (fl&FLAG_VERBOSE){
				PRINT_STATIC_STR("Trying to Open File '");
				print_str(bf);
				PRINT_STATIC_STR("'...\n");
			}
			nf=fopen(bf,"rb");// lgtm [cpp/path-injection]
			if (nf){
				if (!(fl&FLAG_EXPAND_PATH)||!EXPAND_FILE_PATH(bf,f_fp)){
					*(f_fp+j)=0;
					while (j){
						j--;
						*(f_fp+j)=*(bf+j);
					}
				}
				if (fl&FLAG_VERBOSE){
					PRINT_STATIC_STR("Found File '");
					print_str(f_fp);
					PRINT_STATIC_STR("'\n");
				}
				*f=nf;
				sll_stream_create_input_from_file(nf,is);
				if (fl&FLAG_VERBOSE){
					PRINT_STATIC_STR("Trying to Load File as Assembly...\n");
				}
				sll_error_t e;
				if (!sll_load_assembly(is,a_dt,&e)){
					sll_free_assembly_function_table(&(a_dt->ft));
					sll_free_string_table(&(a_dt->st));
					if (e.t==SLL_ERROR_INVALID_FILE_FORMAT){
						if (fl&FLAG_VERBOSE){
							PRINT_STATIC_STR("File is not an Assembly. Falling Back to Compiled Object...\n");
						}
						sll_stream_create_input_from_file(nf,is);
						if (!sll_load_compiled_object(is,c_dt,&e)){
							sll_free_identifier_table(&(c_dt->idt));
							sll_free_export_table(&(c_dt->et));
							sll_free_function_table(&(c_dt->ft));
							sll_free_string_table(&(c_dt->st));
							if (e.t==SLL_ERROR_INVALID_FILE_FORMAT){
								if (fl&FLAG_VERBOSE){
									PRINT_STATIC_STR("File is not a Compiled Object. Falling Back to Standard Compilation...\n");
								}
								sll_stream_create_input_from_file(nf,is);
								sll_init_compilation_data(f_fp,is,c_dt);
								if (!sll_parse_all_objects(c_dt,&i_ft,load_import,&e)){
									if (e.t!=SLL_ERROR_UNKNOWN){
										sll_print_error(is,&e);
									}
									return 0;
								}
								if (fl&FLAG_PRINT_OBJECT){
									sll_output_data_stream_t os;
									sll_stream_create_output_from_file(stdout,&os);
									sll_print_object(c_dt,c_dt->h,&os);
									putchar('\n');
								}
								if (fl&FLAG_VERBOSE){
									PRINT_STATIC_STR("File Successfully Read.\n");
								}
							}
							else{
								sll_print_error(is,&e);
								return 0;
							}
						}
						else{
							if (fl&FLAG_PRINT_OBJECT){
								sll_output_data_stream_t os;
								sll_stream_create_output_from_file(stdout,&os);
								sll_print_object(c_dt,c_dt->h,&os);
								putchar('\n');
							}
							if (fl&FLAG_VERBOSE){
								PRINT_STATIC_STR("File Successfully Read.\n");
							}
						}
					}
					else{
						sll_print_error(is,&e);
						return 0;
					}
				}
				else{
					fl|=_FLAG_ASSEMBLY_GENERATED;
					if (fl&FLAG_VERBOSE){
						PRINT_STATIC_STR("File Successfully Read.\n");
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
#ifdef STANDALONE_BUILD
	uint32_t i=0;
	uint8_t c=0;
	while (*(f_nm+i)){
		c^=*(f_nm+i);
		i++;
	}
	for (uint32_t k=0;k<COMPILED_MODULE_COUNT;k++){
		const module_t* m=m_dt+k;
		if (m->c==c&&m->nml==i){
			for (uint32_t l=0;l<i;l++){
				if (*(m->nm+l)!=*(f_nm+l)){
					goto _check_next_module;
				}
			}
			if (fl&FLAG_VERBOSE){
				PRINT_STATIC_STR("Found Internal Module '");
				print_str(f_nm);
				PRINT_STATIC_STR(".sllc'\n");
			}
			*f=NULL;
			i_bf.bf=m->dt;
			i_bf.sz=m->sz;
			sll_stream_create_input_from_buffer(&i_bf,is);
			sll_error_t e;
			if (!sll_load_compiled_object(is,c_dt,&e)){
				sll_free_identifier_table(&(c_dt->idt));
				sll_free_export_table(&(c_dt->et));
				sll_free_function_table(&(c_dt->ft));
				sll_free_string_table(&(c_dt->st));
				if (e.t==SLL_ERROR_INVALID_FILE_FORMAT){
					PRINT_STATIC_STR("Module '");
					print_str(f_nm);
					PRINT_STATIC_STR(".sllc' is not a Compiled Object.\n");
				}
				else{
					sll_print_error(is,&e);
				}
				return 0;
			}
			if (fl&FLAG_PRINT_OBJECT){
				sll_output_data_stream_t os;
				sll_stream_create_output_from_file(stdout,&os);
				sll_print_object(c_dt,c_dt->h,&os);
				putchar('\n');
			}
			if (fl&FLAG_VERBOSE){
				PRINT_STATIC_STR("File Successfully Read.\n");
			}
			return 1;
		}
_check_next_module:;
	}
#else
	if (l_fpl){
		uint32_t i=l_fpl;
		uint32_t k=0;
		while (*(f_nm+k)){
			*(l_fp+i)=*(f_nm+k);
			i++;
			k++;
		}
		*(l_fp+i)='.';
		*(l_fp+i+1)='s';
		*(l_fp+i+2)='l';
		*(l_fp+i+3)='l';
		*(l_fp+i+4)='c';
		*(l_fp+i+5)=0;
		if (fl&FLAG_VERBOSE){
			PRINT_STATIC_STR("Trying to Open File '");
			print_str(l_fp);
			PRINT_STATIC_STR("'...\n");
		}
		FILE* nf=fopen(l_fp,"rb");// lgtm [cpp/path-injection]
		if (nf){
			if (!(fl&FLAG_EXPAND_PATH)||!EXPAND_FILE_PATH(l_fp,f_fp)){
				i+=5;
				*(f_fp+i)=0;
				while (i){
					i--;
					*(f_fp+i)=*(l_fp+i);
				}
			}
			if (fl&FLAG_VERBOSE){
				PRINT_STATIC_STR("Found File '");
				print_str(f_fp);
				PRINT_STATIC_STR("'\n");
			}
			*f=nf;
			sll_stream_create_input_from_file(nf,is);
			sll_error_t e;
			if (!sll_load_compiled_object(is,c_dt,&e)){
				sll_free_identifier_table(&(c_dt->idt));
				sll_free_export_table(&(c_dt->et));
				sll_free_function_table(&(c_dt->ft));
				sll_free_string_table(&(c_dt->st));
				if (e.t==SLL_ERROR_INVALID_FILE_FORMAT){
					PRINT_STATIC_STR("File '");
					print_str(f_fp);
					PRINT_STATIC_STR("' is not a Compiled Object.\n");
				}
				else{
					sll_print_error(is,&e);
				}
				return 0;
			}
			if (fl&FLAG_PRINT_OBJECT){
				sll_output_data_stream_t os;
				sll_stream_create_output_from_file(stdout,&os);
				sll_print_object(c_dt,c_dt->h,&os);
				putchar('\n');
			}
			if (fl&FLAG_VERBOSE){
				PRINT_STATIC_STR("File Successfully Read.\n");
			}
			return 1;
		}
	}
#endif
	PRINT_STATIC_STR("Unable to Find File '");
	print_str(f_nm);
	PRINT_STATIC_STR("'\n");
	return 0;
}



uint8_t write_assembly(char* o_fp,const sll_assembly_data_t* a_dt){
	uint16_t i=0;
	while (*(o_fp+i)){
		i++;
	}
	*(o_fp+i)='.';
	*(o_fp+i+1)='s';
	*(o_fp+i+2)='l';
	*(o_fp+i+3)='l';
	*(o_fp+i+4)='a';
	*(o_fp+i+5)=0;
	if (fl&FLAG_VERBOSE){
		PRINT_STATIC_STR("Writing Assembly to File '");
		print_str(o_fp);
		PRINT_STATIC_STR("'...\n");
	}
	FILE* f=fopen(o_fp,"wb");// lgtm [cpp/path-injection]
	if (!f){
		PRINT_STATIC_STR("Unable to Open Output File '");
		print_str(o_fp);
		PRINT_STATIC_STR("'\n");
		return 0;
	}
	sll_output_data_stream_t os;
	sll_stream_create_output_from_file(f,&os);
	sll_write_assembly(&os,a_dt);
	if (fl&FLAG_VERBOSE){
		PRINT_STATIC_STR("File Written Successfully.\n");
	}
	fclose(f);
	return 1;
}



uint8_t write_compiled(char* o_fp,const sll_compilation_data_t* c_dt){
	uint16_t i=0;
	while (*(o_fp+i)){
		i++;
	}
	*(o_fp+i)='.';
	*(o_fp+i+1)='s';
	*(o_fp+i+2)='l';
	*(o_fp+i+3)='l';
	*(o_fp+i+4)='c';
	*(o_fp+i+5)=0;
	if (fl&FLAG_VERBOSE){
		PRINT_STATIC_STR("Writing Compiled Object to File '");
		print_str(o_fp);
		PRINT_STATIC_STR("'...\n");
	}
	FILE* f=fopen(o_fp,"wb");// lgtm [cpp/path-injection]
	if (!f){
		PRINT_STATIC_STR("Unable to Open Output File '");
		print_str(o_fp);
		PRINT_STATIC_STR("'\n");
		return 0;
	}
	sll_output_data_stream_t os;
	sll_stream_create_output_from_file(f,&os);
	sll_write_compiled_object(&os,c_dt);
	if (fl&FLAG_VERBOSE){
		PRINT_STATIC_STR("File Written Successfully.\n");
	}
	fclose(f);
	return 1;
}



int main(int argc,const char** argv){
	int32_t ec=1;
	ol=OPTIMIZE_LEVEL_STRIP_DEBUG_DATA;
	fl=0;
	i_fp=malloc(sizeof(char));
	if (!i_fp){
		PRINT_STATIC_STR("Unable to Allocate Space for Include File Path Array\n");
		return 1;
	}
	*i_fp=0;
	i_fpl=1;
#ifndef STANDALONE_BUILD
	GET_EXECUATBLE_FILE_PATH(l_fp,MAX_PATH_LENGTH,l_fpl);
	while (l_fp[l_fpl]!='/'&&l_fp[l_fpl]!='\\'){
		if (!l_fpl){
			goto _skip_std_lib_path;
		}
		l_fpl--;
	}
	l_fp[l_fpl+1]='l';
	l_fp[l_fpl+2]='i';
	l_fp[l_fpl+3]='b';
	l_fp[l_fpl+4]='/';
	l_fp[l_fpl+5]=0;
	l_fpl+=5;
_skip_std_lib_path:
#endif
	fp=NULL;
	fpl=0;
	sll_create_internal_function_table(&i_ft);
	sll_register_standard_internal_functions(&i_ft);
	const char* o_fp=NULL;
	FILE* f=NULL;
	sll_assembly_data_t a_dt={0};
	sll_compilation_data_t c_dt={0};
	uint32_t im_fpl=UINT32_MAX;
	sll_set_argument_count(1);
	for (int i=1;i<argc;i++){
		const char* e=argv[i];
		if ((*e=='-'&&*(e+1)=='a'&&*(e+2)==0)||cmp_str(e,"--generate-assembly")){
			fl|=FLAG_GENERATE_ASSEMBLY;
		}
		else if ((*e=='-'&&*(e+1)=='A'&&*(e+2)==0)||cmp_str(e,"--args")){
			sll_set_argument_count(argc-i);
			for (sll_sys_arg_count_t j=0;j<(sll_sys_arg_count_t)(argc-i-1);j++){
				sll_set_argument(j+1,*(argv+i+j+1));
			}
			break;
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
					PRINT_STATIC_STR("Unable to Allocate Space for Include File Path Array\n");
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
		else if ((*e=='-'&&*(e+1)=='o'&&*(e+2)==0)||cmp_str(e,"--output")){
			if (o_fp){
				PRINT_STATIC_STR("Multplie Output Files Supplied\n");
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
				ol=OPTIMIZE_LEVEL_STRIP_DEBUG_DATA;
			}
			else if (*(e+2)=='3'){
				ol=OPTIMIZE_LEVEL_STRIP_GLOBAL_OPTIMIZE;
			}
			else{
				goto _unkown_switch;
			}
		}
		else if ((*e=='-'&&*(e+1)=='P'&&*(e+2)==0)||cmp_str(e,"--print-assembly")){
			fl|=FLAG_PRINT_ASSEMBLY;
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
			PRINT_STATIC_STR("Unknown Switch '");
			print_str((char*)e);
			PRINT_STATIC_STR("'\n");
			goto _help;
		}
		else{
_read_file_argument:
			fpl++;
			void* tmp=realloc(fp,fpl*sizeof(char*));
			if (!tmp){
				PRINT_STATIC_STR("Unable to Allocate Space for File Path Array\n");
				goto _error;
			}
			fp=tmp;
			*(fp+fpl-1)=(char*)e;
		}
	}
	im_fpl=fpl;
	if (!(fl&FLAG_NO_LOGO)){
#ifdef STANDALONE_BUILD
		PRINT_STATIC_STR("sll (standalone, v"STR(SLL_VERSION_MAJOR)"."STR(SLL_VERSION_MINOR)"."STR(SLL_VERSION_PATCH)") ("SLL_VERSION_BUILD_DATE", "SLL_VERSION_BUILD_TIME")\n");
#else
		PRINT_STATIC_STR("sll (v"STR(SLL_VERSION_MAJOR)"."STR(SLL_VERSION_MINOR)"."STR(SLL_VERSION_PATCH)") ("SLL_VERSION_BUILD_DATE", "SLL_VERSION_BUILD_TIME")\n");
#endif
	}
	if (fl&FLAG_VERBOSE){
		PRINT_STATIC_STR("Configuration:\n  Optimization Level:\n");
		if (ol==OPTIMIZE_LEVEL_NO_OPTIMIZE){
			PRINT_STATIC_STR("    No Optimization\n");
		}
		if (ol>=OPTIMIZE_LEVEL_REMOVE_PADDING){
			PRINT_STATIC_STR("    Padding Reduction\n");
		}
		if (ol>=OPTIMIZE_LEVEL_STRIP_DEBUG_DATA){
			PRINT_STATIC_STR("    Debug Data Stripping\n");
		}
		if (ol>=OPTIMIZE_LEVEL_STRIP_GLOBAL_OPTIMIZE){
			PRINT_STATIC_STR("    Global Optimization\n");
		}
		if (fl&FLAG_GENERATE_ASSEMBLY){
			PRINT_STATIC_STR("  Assembly Generation Mode\n");
		}
		if (fl&FLAG_GENERATE_COMPILED_OBJECT){
			PRINT_STATIC_STR("  Compiled Object Generation Mode\n");
		}
		if (fl&FLAG_EXPAND_PATH){
			PRINT_STATIC_STR("  Path Expand Mode\n");
		}
		if (fl&FLAG_HELP){
			PRINT_STATIC_STR("  Help Print Mode\n");
		}
		if (!(fl&FLAG_NO_LOGO)){
			PRINT_STATIC_STR("  Compiler Logo Mode\n");
		}
		if (fl&FLAG_PRINT_ASSEMBLY){
			PRINT_STATIC_STR("  Assembly Print Mode\n");
		}
		if (fl&FLAG_PRINT_OBJECT){
			PRINT_STATIC_STR("  Object Print Mode\n");
		}
		if (!(fl&FLAG_NO_RUN)){
			PRINT_STATIC_STR("  Program Run Mode\n");
		}
		if (fl&FLAG_VERBOSE){
			PRINT_STATIC_STR("  Verbose Mode\n");
		}
		PRINT_STATIC_STR("Include Path: \n  - '");
		for (uint32_t i=0;i<i_fpl;i++){
			if (*(i_fp+i)){
				putchar(*(i_fp+i));
			}
			else{
				if (i+1<i_fpl){
					PRINT_STATIC_STR("'\n  - '");
				}
				else{
					PRINT_STATIC_STR("'\n");
				}
			}
		}
#ifndef STANDALONE_BUILD
		PRINT_STATIC_STR("Library Path: ");
		print_str(l_fp);
		putchar('\n');
#endif
	}
	if (fl&FLAG_HELP){
		goto _help;
	}
	if (!fpl){
		PRINT_STATIC_STR("No Input Files Supplied\n");
		goto _help;
	}
	for (uint32_t j=0;j<fpl;j++){
		sll_set_assembly_data_stack(&a_dt,a_st,ASSEMBLY_STACK_SIZE);
		sll_set_compilation_data_stack(&c_dt,c_st,COMPILER_STACK_SIZE);
		char f_fp[MAX_PATH_LENGTH];
		sll_input_data_stream_t is;
		fl&=~_FLAG_ASSEMBLY_GENERATED;
		if (!load_file(*(fp+j),&a_dt,&c_dt,&f,&is,f_fp)){
			goto _error;
		}
		if (!(fl&_FLAG_ASSEMBLY_GENERATED)){
			if (ol>=OPTIMIZE_LEVEL_STRIP_GLOBAL_OPTIMIZE){
				if (fl&FLAG_VERBOSE){
					PRINT_STATIC_STR("Performing Global Optimization...\n");
				}
				sll_optimize_object(&c_dt,c_dt.h);
			}
			if (ol>=OPTIMIZE_LEVEL_STRIP_DEBUG_DATA){
				if (fl&FLAG_VERBOSE){
					PRINT_STATIC_STR("Removing Debug Data...\n");
				}
				sll_remove_object_debug_data(c_dt.h);
			}
			if (ol>=OPTIMIZE_LEVEL_REMOVE_PADDING){
				if (fl&FLAG_VERBOSE){
					PRINT_STATIC_STR("Removing Object Padding...\n");
				}
				sll_remove_object_padding(&c_dt,c_dt.h);
			}
			if (fl&FLAG_PRINT_OBJECT){
				sll_output_data_stream_t os;
				sll_stream_create_output_from_file(stdout,&os);
				sll_print_object(&c_dt,c_dt.h,&os);
				putchar('\n');
			}
			if (fl&FLAG_VERBOSE){
				PRINT_STATIC_STR("Optimizing Object Metadata...\n");
			}
			sll_optimize_metadata(&c_dt);
		}
		if (!(fl&_FLAG_ASSEMBLY_GENERATED)&&((fl&(FLAG_GENERATE_ASSEMBLY|FLAG_PRINT_ASSEMBLY))||!(fl&FLAG_NO_RUN))){
			if (fl&FLAG_VERBOSE){
				PRINT_STATIC_STR("Generating Assembly...\n");
			}
			sll_error_t e;
			if (!sll_generate_assembly(&c_dt,&a_dt,&e)){
				sll_print_error(&is,&e);
				goto _error;
			}
		}
		if (fl&FLAG_PRINT_ASSEMBLY){
			sll_output_data_stream_t os;
			sll_stream_create_output_from_file(stdout,&os);
			sll_print_assembly(&a_dt,&os);
			putchar('\n');
		}
		if (fl&(FLAG_GENERATE_ASSEMBLY|FLAG_GENERATE_COMPILED_OBJECT)){
			char bf[MAX_PATH_LENGTH];
			uint16_t i=0;
			if (!o_fp){
				while (*(f_fp+i)){
					*(bf+i)=*(f_fp+i);
					i++;
				}
				i--;
			}
			else{
				if (fpl==1){
					while (*(o_fp+i)){
						bf[i]=*(o_fp+i);
						i++;
					}
					if (fl&FLAG_GENERATE_ASSEMBLY){
						bf[i]=0;
						if (!write_assembly(bf,&a_dt)){// lgtm [cpp/path-injection]
							goto _error;
						}
					}
					if (fl&FLAG_GENERATE_COMPILED_OBJECT){
						bf[i]=0;
						if (!write_compiled(bf,&c_dt)){// lgtm [cpp/path-injection]
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
					uint32_t k=0;
					uint32_t l=0;
					while (*(f_fp+k)&&*(f_fp+k)!='.'){
						if (*(f_fp+k)=='\\'||*(f_fp+k)=='/'){
							l=k+1;
						}
						k++;
					}
					while (l<k){
						bf[i]=*(f_fp+l);
						i++;
						l++;
					}
					bf[i]='.';
				}
			}
			i++;
			if (fl&FLAG_GENERATE_ASSEMBLY){
				bf[i]=0;
				if (!write_assembly(bf,&a_dt)){// lgtm [cpp/path-injection]
					goto _error;
				}
			}
			if (fl&FLAG_GENERATE_COMPILED_OBJECT){
				bf[i]=0;
				if (!write_compiled(bf,&c_dt)){// lgtm [cpp/path-injection]
					goto _error;
				}
			}
_skip_write:;
		}
		if (!(fl&FLAG_NO_RUN)){
			const char* c_fp=*(fp+j);
			char bf[MAX_PATH_LENGTH];
			if (!EXPAND_FILE_PATH(c_fp,bf)){
				uint16_t k=0;
				while (*(c_fp+k)){
					bf[k]=*(c_fp+k);
					k++;
				}
				bf[k]=0;
			}
			sll_set_argument(0,bf);
			sll_input_data_stream_t ris;
			sll_output_data_stream_t ros;
			sll_stream_create_input_from_file(stdin,&ris);
			sll_stream_create_output_from_file(stdout,&ros);
			sll_stack_data_t st;
			sll_setup_stack(&st,vm_st,VM_STACK_SIZE);
			sll_error_t e={
				SLL_ERROR_UNKNOWN
			};
			sll_return_code_t r=sll_execute_assembly(&a_dt,&st,&i_ft,&ris,&ros,&e);
			if (e.t!=SLL_ERROR_UNKNOWN){
				sll_print_error(NULL,&e);
				goto _error;
			}
			if (r){
				ec=r;
				goto _error;
			}
		}
		if (f){
			fclose(f);
			f=NULL;
		}
		sll_free_assembly_data(&a_dt);
		sll_free_compilation_data(&c_dt);
	}
	while (im_fpl<fpl){
		free(*(fp+im_fpl));
		im_fpl++;
	}
	free(i_fp);
	if (fp){
		free(fp);
	}
	sll_free_internal_function_table(&i_ft);
	return 0;
_help:
	PRINT_STATIC_STR(HELP_TEXT);
_error:
	while (im_fpl<fpl){
		free(*(fp+im_fpl));
		im_fpl++;
	}
	free(i_fp);
	if (fp){
		free(fp);
	}
	sll_free_assembly_data(&a_dt);
	sll_free_compilation_data(&c_dt);
	if (f){
		fclose(f);
	}
	sll_free_internal_function_table(&i_ft);
	return ec;
}
