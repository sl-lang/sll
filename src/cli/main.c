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
#include <string.h>



#ifdef _MSC_VER
#define MAX_PATH_LENGTH (MAX_PATH+1)
#define EXPAND_FILE_PATH(s,d) GetFullPathNameA((s),MAX_PATH+1,(d),NULL)
#define GET_EXECUATBLE_FILE_PATH(bf,l,o) \
	do{ \
		(o)=GetModuleFileNameA(NULL,(char*)(bf),(l)); \
	} while (0)
#define RESET_CONSOLE SetConsoleMode(GetStdHandle(STD_OUTPUT_HANDLE),cm)
#ifdef SLL_VERSION_STANDALONE
#define SLL_TYPE "win-standalone"
#else
#define SLL_TYPE "win"
#endif
#define DYNAMIC_LIBRARY_EXTENSION ".dll"
#else
#define MAX_PATH_LENGTH (PATH_MAX+1)
#define EXPAND_FILE_PATH(s,d) realpath((s),(d))
#define GET_EXECUATBLE_FILE_PATH(bf,l,o) \
	do{ \
		ssize_t __o=readlink("/proc/self/exe",(char*)(bf),(l)); \
		if (__o!=-1){ \
			(o)=__o; \
			*((bf)+(o))=0; \
		} \
		else{ \
			(o)=0; \
			*(bf)=0; \
		} \
	} while (0)
#define RESET_CONSOLE
#ifdef SLL_VERSION_STANDALONE
#define SLL_TYPE "posix-standalone"
#else
#define SLL_TYPE "posix"
#endif
#define DYNAMIC_LIBRARY_EXTENSION ".so"
#endif



#define PRINT_STATIC_STR(s) fwrite((void*)(s),sizeof(char),sizeof((s))/sizeof(char)-1,stdout)
#define _STR(x) #x
#define STR(x) _STR(x)
#define _IF_USE_COLORS(x) \
	do{ \
		if (fl&FLAG_USE_COLORS){ \
			PRINT_STATIC_STR(x); \
		} \
	} while (0)
#define COLOR_RESET _IF_USE_COLORS("\x1b[0m")
#define COLOR_RED _IF_USE_COLORS("\x1b[91m")
#define VM_STACK_SIZE 65536
#define FLAG_EXPAND_PATH 1
#define FLAG_FORCE_UPDATE 2
#define FLAG_GENERATE_ASSEMBLY 4
#define FLAG_GENERATE_COMPILED_OBJECT 8
#define FLAG_HELP 16
#define FLAG_NO_RUN 32
#define FLAG_PRINT_ASSEMBLY 64
#define FLAG_PRINT_OBJECT 128
#define FLAG_UPDATE 256
#define FLAG_USE_COLORS 512
#define FLAG_VERBOSE 1024
#define FLAG_VERSION 2048
#define _FLAG_ASSEMBLY_GENERATED 4096
#define OPTIMIZE_LEVEL_NO_OPTIMIZE 0
#define OPTIMIZE_LEVEL_REMOVE_PADDING 1
#define OPTIMIZE_LEVEL_STRIP_DEBUG_DATA 2
#define OPTIMIZE_LEVEL_STRIP_GLOBAL_OPTIMIZE 3
#ifdef SLL_VERSION_STANDALONE
#define STANDALONE_STRING "standalone, "
#else
#define STANDALONE_STRING ""
#endif
#ifdef SLL_VERSION_HAS_SHA
#define TYPE_STRING "commit/"SLL_VERSION_SHA" [https://github.com/sl-lang/sll/tree/"SLL_VERSION_FULL_SHA"], "
#else
#define TYPE_STRING "local, "
#endif



static uint8_t vm_st[VM_STACK_SIZE];
static uint8_t ol;
static uint16_t fl;
static char* i_fp;
static uint32_t i_fpl;
static char** fp;
static uint32_t fpl;
static char l_fp[MAX_PATH_LENGTH];
static uint32_t l_fpl;
#ifdef STANDALONE_BUILD
static sll_input_buffer_t m_i_bf;
#endif
static sll_internal_function_table_t i_ft;
#ifdef _MSC_VER
static DWORD cm;
#endif



static uint8_t load_file(const char* f_nm,sll_assembly_data_t* a_dt,sll_compilation_data_t* c_dt,FILE** f,sll_input_data_stream_t* is,char* f_fp);



static sll_return_t load_import(const sll_string_t* nm,sll_compilation_data_t* o,sll_error_t* e){
	FILE* f=NULL;
	sll_assembly_data_t a_dt={0};
	sll_input_data_stream_t is;
	char f_fp[MAX_PATH_LENGTH];
	sll_init_compilation_data(nm->v,&is,o);
	if (!load_file((char*)(nm->v),&a_dt,o,&f,&is,f_fp)){
		if (f){
			fclose(f);
		}
		sll_free_compilation_data(o);
		e->t=SLL_ERROR_UNKNOWN;
		return SLL_RETURN_ERROR;
	}
	if (fl&_FLAG_ASSEMBLY_GENERATED){
		sll_free_assembly_data(&a_dt);
		COLOR_RED;
		PRINT_STATIC_STR("Importing Assembly into Compiled Objects is Not Allowed\n");
		COLOR_RESET;
		e->t=SLL_ERROR_UNKNOWN;
		return SLL_RETURN_ERROR;
	}
	if (f){
		fclose(f);
	}
	return SLL_RETURN_NO_ERROR;
}



static uint8_t load_file(const char* f_nm,sll_assembly_data_t* a_dt,sll_compilation_data_t* c_dt,FILE** f,sll_input_data_stream_t* is,char* f_fp){
	sll_string_length_t f_nm_l=sll_string_length_unaligned(SLL_CHAR(f_nm));
	char bf[MAX_PATH_LENGTH];
	sll_string_length_t i=0;
	while (i<i_fpl){
		sll_string_length_t j=sll_string_length_unaligned(SLL_CHAR(i_fp+i));
		memcpy(bf,i_fp+i,j);
		i+=j+1;
		memcpy(bf+j,f_nm,f_nm_l);
		j+=f_nm_l;
		memcpy(bf+j,".slc",5);
		if (fl&FLAG_VERBOSE){
			PRINT_STATIC_STR("Trying to Open File '");
			fputs(bf,stdout);
			PRINT_STATIC_STR("'...\n");
		}
		FILE* nf=fopen(bf,"rb");
		if (nf){
			if (!(fl&FLAG_EXPAND_PATH)||!EXPAND_FILE_PATH(bf,f_fp)){
				memcpy(f_fp,bf,j+1);
			}
			if (fl&FLAG_VERBOSE){
				PRINT_STATIC_STR("Found File '");
				fputs(f_fp,stdout);
				PRINT_STATIC_STR("'\n");
			}
			*f=nf;
			sll_stream_create_input_from_file(nf,is);
			sll_error_t e;
			if (!sll_load_compiled_object(is,c_dt,&e)){
				sll_free_compilation_data(c_dt);
				if (e.t==SLL_ERROR_INVALID_FILE_FORMAT){
					COLOR_RED;
					PRINT_STATIC_STR("File '");
					fputs(f_fp,stdout);
					PRINT_STATIC_STR("'is not a Compiled Object.\n");
					COLOR_RESET;
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
		bf[j]=0;
		if (fl&FLAG_VERBOSE){
			PRINT_STATIC_STR("Trying to Open File '");
			fputs(bf,stdout);
			PRINT_STATIC_STR("'...\n");
		}
		nf=fopen(bf,"rb");
		if (nf){
			if (!(fl&FLAG_EXPAND_PATH)||!EXPAND_FILE_PATH(bf,f_fp)){
				memcpy(f_fp,bf,j+1);
			}
			if (fl&FLAG_VERBOSE){
				PRINT_STATIC_STR("Found File '");
				fputs(f_fp,stdout);
				PRINT_STATIC_STR("'\n");
			}
			*f=nf;
			sll_stream_create_input_from_file(nf,is);
			sll_error_t e;
			if (!sll_load_assembly(is,a_dt,&e)){
				sll_free_assembly_data(a_dt);
				if (e.t==SLL_ERROR_INVALID_FILE_FORMAT){
					sll_stream_create_input_from_file(nf,is);
					if (!sll_load_compiled_object(is,c_dt,&e)){
						sll_free_compilation_data(c_dt);
						if (e.t==SLL_ERROR_INVALID_FILE_FORMAT){
							sll_stream_create_input_from_file(nf,is);
							sll_init_compilation_data(SLL_CHAR(f_fp),is,c_dt);
							if (!sll_parse_all_objects(c_dt,&i_ft,load_import,&e)){
								sll_free_compilation_data(c_dt);
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
	}
#ifdef STANDALONE_BUILD
	uint8_t c=0;
	for (i=0;i<f_nm_l;i++){
		c^=*(f_nm+i);
	}
	for (uint32_t j=0;j<COMPILED_MODULE_COUNT;j++){
		const module_t* m=m_dt+j;
		if (m->c==c&&m->nml==f_nm_l&&!memcmp(m->nm,f_nm,f_nm_l)){
			if (fl&FLAG_VERBOSE){
				PRINT_STATIC_STR("Found Internal Module '");
				fputs(f_nm,stdout);
				PRINT_STATIC_STR(".slc'\n");
			}
			*f=NULL;
			m_i_bf.bf=m->dt;
			m_i_bf.sz=m->sz;
			sll_stream_create_input_from_buffer(&m_i_bf,is);
			sll_error_t e;
			if (!sll_load_compiled_object(is,c_dt,&e)){
				sll_free_identifier_table(&(c_dt->idt));
				sll_free_export_table(&(c_dt->et));
				sll_free_function_table(&(c_dt->ft));
				sll_free_string_table(&(c_dt->st));
				if (e.t==SLL_ERROR_INVALID_FILE_FORMAT){
					COLOR_RED;
					PRINT_STATIC_STR("Module '");
					fputs(f_nm,stdout);
					PRINT_STATIC_STR(".slc' is not a Compiled Object.\n");
					COLOR_RESET;
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
#else
	if (l_fpl){
		i=l_fpl+f_nm_l;
		memcpy(l_fp+l_fpl,f_nm,f_nm_l);
		memcpy(l_fp+i,".slc",5);
		if (fl&FLAG_VERBOSE){
			PRINT_STATIC_STR("Trying to Open File '");
			fputs(l_fp,stdout);
			PRINT_STATIC_STR("'...\n");
		}
		FILE* nf=fopen(l_fp,"rb");
		if (nf){
			if (!(fl&FLAG_EXPAND_PATH)||!EXPAND_FILE_PATH(l_fp,f_fp)){
				memcpy(f_fp,l_fp,i+5);
			}
			if (fl&FLAG_VERBOSE){
				PRINT_STATIC_STR("Found File '");
				fputs(f_fp,stdout);
				PRINT_STATIC_STR("'\n");
			}
			*f=nf;
			sll_stream_create_input_from_file(nf,is);
			sll_error_t e;
			if (!sll_load_compiled_object(is,c_dt,&e)){
				sll_free_compilation_data(c_dt);
				if (e.t==SLL_ERROR_INVALID_FILE_FORMAT){
					COLOR_RED;
					PRINT_STATIC_STR("File '");
					fputs(f_fp,stdout);
					PRINT_STATIC_STR("' is not a Compiled Object.\n");
					COLOR_RESET;
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
		*(l_fp+i)=0;
		if (fl&FLAG_VERBOSE){
			PRINT_STATIC_STR("Trying to Open File '");
			fputs(l_fp,stdout);
			PRINT_STATIC_STR("'...\n");
		}
		nf=fopen(l_fp,"rb");
		if (nf){
			if (!(fl&FLAG_EXPAND_PATH)||!EXPAND_FILE_PATH(l_fp,f_fp)){
				memcpy(f_fp,l_fp,l_fpl+5);
			}
			if (fl&FLAG_VERBOSE){
				PRINT_STATIC_STR("Found File '");
				fputs(f_fp,stdout);
				PRINT_STATIC_STR("'\n");
			}
			*f=nf;
			sll_stream_create_input_from_file(nf,is);
			sll_error_t e;
			if (!sll_parse_all_objects(c_dt,&i_ft,load_import,&e)){
				sll_free_compilation_data(c_dt);
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
			return 1;
		}
	}
#endif
	COLOR_RED;
	PRINT_STATIC_STR("Unable to Find File '");
	fputs(f_nm,stdout);
	PRINT_STATIC_STR("'\n");
	COLOR_RESET;
	return 0;
}



static uint8_t write_assembly(char* o_fp,const sll_assembly_data_t* a_dt){
	memcpy(o_fp+sll_string_length_unaligned(SLL_CHAR(o_fp)),".sla",5);
	if (fl&FLAG_VERBOSE){
		PRINT_STATIC_STR("Writing Assembly to File '");
		fputs(o_fp,stdout);
		PRINT_STATIC_STR("'...\n");
	}
	FILE* f=fopen(o_fp,"wb");
	if (!f){
		COLOR_RED;
		PRINT_STATIC_STR("Unable to Open Output File '");
		fputs(o_fp,stdout);
		PRINT_STATIC_STR("'\n");
		COLOR_RESET;
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



static uint8_t write_compiled(char* o_fp,const sll_compilation_data_t* c_dt){
	memcpy(o_fp+sll_string_length_unaligned(SLL_CHAR(o_fp)),".slc",5);
	if (fl&FLAG_VERBOSE){
		PRINT_STATIC_STR("Writing Compiled Object to File '");
		fputs(o_fp,stdout);
		PRINT_STATIC_STR("'...\n");
	}
	FILE* f=fopen(o_fp,"wb");
	if (!f){
		COLOR_RED;
		PRINT_STATIC_STR("Unable to Open Output File '");
		fputs(o_fp,stdout);
		PRINT_STATIC_STR("'\n");
		COLOR_RESET;
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



static uint8_t execute(const char* f_fp,sll_compilation_data_t* c_dt,sll_assembly_data_t* a_dt,sll_input_data_stream_t* is,const char* o_fp,int* ec){
	if (!(fl&_FLAG_ASSEMBLY_GENERATED)){
		if (ol>=OPTIMIZE_LEVEL_STRIP_GLOBAL_OPTIMIZE){
			if (fl&FLAG_VERBOSE){
				PRINT_STATIC_STR("Performing Global Optimization...\n");
			}
			sll_optimize_object(c_dt,&i_ft);
		}
		if (ol>=OPTIMIZE_LEVEL_STRIP_DEBUG_DATA){
			if (fl&FLAG_VERBOSE){
				PRINT_STATIC_STR("Removing Debug Data...\n");
			}
			sll_remove_object_debug_data(c_dt->h);
		}
		if (ol>=OPTIMIZE_LEVEL_REMOVE_PADDING){
			if (fl&FLAG_VERBOSE){
				PRINT_STATIC_STR("Removing Object Padding...\n");
			}
			sll_remove_object_padding(c_dt,c_dt->h);
		}
		if (fl&FLAG_PRINT_OBJECT){
			sll_output_data_stream_t os;
			sll_stream_create_output_from_file(stdout,&os);
			sll_print_object(c_dt,c_dt->h,&os);
			putchar('\n');
		}
		if (fl&FLAG_VERBOSE){
			PRINT_STATIC_STR("Optimizing Object Metadata...\n");
		}
		sll_optimize_metadata(c_dt);
	}
	if (!(fl&_FLAG_ASSEMBLY_GENERATED)&&((fl&(FLAG_GENERATE_ASSEMBLY|FLAG_PRINT_ASSEMBLY))||!(fl&FLAG_NO_RUN))){
		if (fl&FLAG_VERBOSE){
			PRINT_STATIC_STR("Generating Assembly...\n");
		}
		sll_error_t e;
		if (!sll_generate_assembly(c_dt,a_dt,&e)){
			sll_print_error(is,&e);
			return 0;
		}
	}
	if (fl&FLAG_PRINT_ASSEMBLY){
		sll_output_data_stream_t os;
		sll_stream_create_output_from_file(stdout,&os);
		sll_print_assembly(a_dt,&os);
		putchar('\n');
	}
	if (fl&(FLAG_GENERATE_ASSEMBLY|FLAG_GENERATE_COMPILED_OBJECT)){
		char bf[MAX_PATH_LENGTH];
		uint16_t i=0;
		sll_string_length_t f_fp_l=sll_string_length_unaligned(SLL_CHAR(f_fp));
		if (!o_fp){
			memcpy(bf,f_fp,f_fp_l);
			i=f_fp_l-1;
		}
		else{
			if (fpl==1){
				i=sll_string_length_unaligned(SLL_CHAR(o_fp));
				memcpy(bf,o_fp,i);
				i-=1;
			}
			else{
				i=sll_string_length_unaligned(SLL_CHAR(o_fp));
				while (*(o_fp+i)!='\\'&&*(o_fp+i)!='/'){
					if (i==0){
						i--;
						break;
					}
					i--;
				}
				i++;
				sll_string_length_t j=f_fp_l;
				while (j&&*(f_fp+j-1)!='\\'&&*(f_fp+j-1)!='/'){
					j--;
				}
				memcpy(bf+i,f_fp+j,f_fp_l-j);
				i+=f_fp_l-j-1;
			}
		}
		i++;
		if (fl&FLAG_GENERATE_ASSEMBLY){
			bf[i]=0;
			if (!write_assembly(bf,a_dt)){
				return 0;
			}
		}
		if (fl&FLAG_GENERATE_COMPILED_OBJECT){
			bf[i]=0;
			if (!write_compiled(bf,c_dt)){
				return 0;
			}
		}
	}
	if (!(fl&FLAG_NO_RUN)){
		sll_stack_data_t st;
		sll_setup_stack(&st,vm_st,VM_STACK_SIZE);
		sll_handle_list_t hl;
		sll_init_handle_list(&hl);
		sll_input_data_stream_t ris;
		sll_output_data_stream_t ros;
		sll_stream_create_input_from_file(stdin,&ris);
		sll_stream_create_output_from_file(stdout,&ros);
		sll_runtime_data_t r_dt={
			&i_ft,
			&hl,
			&ris,
			&ros
		};
		sll_error_t e={
			SLL_ERROR_UNKNOWN
		};
		sll_return_code_t r=sll_execute_assembly(a_dt,&st,&r_dt,&e);
		sll_free_handle_list(&hl);
		if (e.t!=SLL_ERROR_UNKNOWN){
			sll_print_error(NULL,&e);
			return 0;
		}
		if (r){
			*ec=r;
			return 0;
		}
	}
	return 1;
}



int main(int argc,const char** argv){
	sll_init();
#ifdef _MSC_VER
	GetConsoleMode(GetStdHandle(STD_OUTPUT_HANDLE),&cm);
	SetConsoleMode(GetStdHandle(STD_OUTPUT_HANDLE),cm|ENABLE_PROCESSED_OUTPUT|ENABLE_WRAP_AT_EOL_OUTPUT|ENABLE_VIRTUAL_TERMINAL_PROCESSING);
#endif
	int ec=1;
	ol=OPTIMIZE_LEVEL_STRIP_DEBUG_DATA;
	fl=0;
	i_fp=malloc(sizeof(char));
	if (!i_fp){
		PRINT_STATIC_STR("Unable to Allocate Space for Include File Path Array\n");
		RESET_CONSOLE;
		return 1;
	}
	*i_fp=0;
	i_fpl=1;
	GET_EXECUATBLE_FILE_PATH(l_fp,MAX_PATH_LENGTH,l_fpl);
#ifndef STANDALONE_BUILD
	while (l_fp[l_fpl]!='/'&&l_fp[l_fpl]!='\\'){
		if (!l_fpl){
			goto _skip_lib_path;
		}
		l_fpl--;
	}
	memcpy(l_fp+l_fpl+1,"lib/",5);
	l_fpl+=5;
_skip_lib_path:
#endif
	fp=NULL;
	fpl=0;
	char** sl=NULL;
	uint32_t sll=0;
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
		if ((*e=='-'&&*(e+1)=='a'&&*(e+2)==0)||!strcmp(e,"--generate-assembly")){
			fl|=FLAG_GENERATE_ASSEMBLY;
		}
		else if ((*e=='-'&&*(e+1)=='A'&&*(e+2)==0)||!strcmp(e,"--args")){
			sll_set_argument_count(argc-i);
			for (sll_integer_t j=0;j<(sll_integer_t)(argc-i-1);j++){
				sll_set_argument(j+1,SLL_CHAR(*(argv+i+j+1)));
			}
			break;
		}
		else if ((*e=='-'&&*(e+1)=='c'&&*(e+2)==0)||!strcmp(e,"--generate-compiled-object")){
			fl|=FLAG_GENERATE_COMPILED_OBJECT;
		}
		else if ((*e=='-'&&*(e+1)=='C'&&*(e+2)==0)||!strcmp(e,"--use-colors")){
			fl|=FLAG_USE_COLORS;
		}
		else if ((*e=='-'&&*(e+1)=='e'&&*(e+2)==0)||!strcmp(e,"--expand-file-paths")){
			fl|=FLAG_EXPAND_PATH;
		}
		else if ((*e=='-'&&*(e+1)=='f'&&*(e+2)==0)||!strcmp(e,"--file")){
			i++;
			if (i==argc){
				break;
			}
			goto _read_file_argument;
		}
		else if (!strcmp(e,"--force-update")){
			fl|=FLAG_FORCE_UPDATE;
		}
		else if ((*e=='-'&&*(e+1)=='h'&&*(e+2)==0)||!strcmp(e,"--help")){
			fl|=FLAG_HELP;
		}
		else if ((*e=='-'&&*(e+1)=='I'&&*(e+2)==0)||!strcmp(e,"--include")){
			i++;
			if (i==argc){
				break;
			}
			e=argv[i];
			sll_string_length_t sz=sll_string_length_unaligned(SLL_CHAR(e));
			if (sz){
				uint32_t j=i_fpl;
				i_fpl+=sz+(*(e+sz-1)!='\\'&&*(e+sz-1)!='/'?2:1);
				void* tmp=realloc(i_fp,i_fpl*sizeof(char));
				if (!tmp){
					COLOR_RED;
					PRINT_STATIC_STR("Unable to Allocate Space for Include File Path Array\n");
					COLOR_RESET;
					goto _error;
				}
				i_fp=tmp;
				memcpy(i_fp+j,e,sz);
				j+=sz;
				if (*(e+sz-1)!='\\'&&*(e+sz-1)!='/'){
					*(i_fp+j)='/';
					j++;
				}
				*(i_fp+j)=0;
			}
		}
		else if ((*e=='-'&&*(e+1)=='o'&&*(e+2)==0)||!strcmp(e,"--output")){
			if (o_fp){
				COLOR_RED;
				PRINT_STATIC_STR("Multplie Output Files Supplied\n");
				COLOR_RESET;
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
		else if ((*e=='-'&&*(e+1)=='P'&&*(e+2)==0)||!strcmp(e,"--print-assembly")){
			fl|=FLAG_PRINT_ASSEMBLY;
		}
		else if ((*e=='-'&&*(e+1)=='p'&&*(e+2)==0)||!strcmp(e,"--print-objects")){
			fl|=FLAG_PRINT_OBJECT;
		}
		else if ((*e=='-'&&*(e+1)=='R'&&*(e+2)==0)||!strcmp(e,"--no-run")){
			fl|=FLAG_NO_RUN;
		}
		else if ((*e=='-'&&*(e+1)=='s'&&*(e+2)==0)||!strcmp(e,"--source")){
			i++;
			if (i==argc){
				break;
			}
			sll++;
			void* tmp=realloc(sl,sll*sizeof(char*));
			if (!tmp){
				COLOR_RED;
				PRINT_STATIC_STR("Unable to Allocate Space for Source Code Array\n");
				COLOR_RESET;
				goto _error;
			}
			sl=tmp;
			*(sl+sll-1)=(char*)argv[i];
		}
		else if ((*e=='-'&&*(e+1)=='U'&&*(e+2)==0)||!strcmp(e,"--update")){
			fl|=FLAG_UPDATE;
		}
		else if ((*e=='-'&&*(e+1)=='v'&&*(e+2)==0)||!strcmp(e,"--verbose")){
			fl|=FLAG_VERBOSE;
		}
		else if ((*e=='-'&&*(e+1)=='V'&&*(e+2)==0)||!strcmp(e,"--version")){
			fl|=FLAG_VERSION;
		}
		else if (*e=='-'){
_unkown_switch:
			COLOR_RED;
			PRINT_STATIC_STR("Unknown Switch '");
			fputs((char*)e,stdout);
			PRINT_STATIC_STR("'\n");
			COLOR_RESET;
			goto _help;
		}
		else{
_read_file_argument:
			fpl++;
			void* tmp=realloc(fp,fpl*sizeof(char*));
			if (!tmp){
				COLOR_RED;
				PRINT_STATIC_STR("Unable to Allocate Space for File Path Array\n");
				COLOR_RESET;
				goto _error;
			}
			fp=tmp;
			*(fp+fpl-1)=(char*)e;
		}
	}
	if (fl&FLAG_VERSION){
		PRINT_STATIC_STR("sll "STR(SLL_VERSION_MAJOR)"."STR(SLL_VERSION_MINOR)"."STR(SLL_VERSION_PATCH)" ("STANDALONE_STRING TYPE_STRING SLL_VERSION_BUILD_DATE", "SLL_VERSION_BUILD_TIME")\n");
		RESET_CONSOLE;
		return 0;
	}
	if (fl&FLAG_UPDATE){
		if (fl&FLAG_VERBOSE){
			PRINT_STATIC_STR("Fetching Version Data...\n");
		}
		sll_string_t m;
		sll_string_from_pointer(SLL_CHAR("GET"),&m);
		sll_string_t h;
		sll_string_from_pointer(SLL_CHAR("sll.krzem.workers.dev"),&h);
		sll_string_t p;
		sll_string_from_pointer(SLL_CHAR("/version"),&p);
		sll_string_t dt=SLL_ZERO_STRING_STRUCT;
		sll_header_list_t hl=SLL_ZERO_HEADER_LIST_STRUCT;
		sll_string_t r_dt;
		sll_http_response_t r={
			NULL,
			NULL,
			&r_dt
		};
		sll_return_code_t rc=sll_url_http_request(&m,&h,&p,&hl,&dt,&r);
		if (rc!=200){
			COLOR_RED;
			PRINT_STATIC_STR("Unexpected HTTP Status Code: ");
			sll_string_t tmp;
			sll_string_from_int(rc,&tmp);
			fwrite(tmp.v,sizeof(sll_char_t),tmp.l,stdout);
			free(tmp.v);
			PRINT_STATIC_STR("\n");
			COLOR_RESET;
			sll_free_http_response(&r);
			RESET_CONSOLE;
			return 1;
		}
		sll_json_object_t json;
		sll_json_parser_state_t json_p=r_dt.v;
		if (!sll_json_parse(&json_p,&json)){
			goto _json_error;
		}
		sll_string_t tmp;
		sll_string_from_pointer(SLL_CHAR("version"),&tmp);
		sll_json_object_t* v=sll_json_get_by_key(&json,&tmp);
		if (!v||v->t!=SLL_JSON_OBJECT_TYPE_INTEGER){
			goto _json_error;
		}
		sll_free_http_response(&r);
		if (!(fl&FLAG_FORCE_UPDATE)&&v->dt.i<=SLL_VERSION){
			PRINT_STATIC_STR("No New Versions Avaible, sll is Up To Date\n");
		}
		else{
			if (fl&FLAG_VERBOSE){
				PRINT_STATIC_STR("Updating sll from "SLL_VERSION_STRING" to ");
				sll_string_from_int(SLL_GET_MAJOR(v->dt.i),&tmp);
				fwrite(tmp.v,sizeof(sll_char_t),tmp.l,stdout);
				free(tmp.v);
				putchar('.');
				sll_string_from_int(SLL_GET_MINOR(v->dt.i),&tmp);
				fwrite(tmp.v,sizeof(sll_char_t),tmp.l,stdout);
				free(tmp.v);
				putchar('.');
				sll_string_from_int(SLL_GET_PATCH(v->dt.i),&tmp);
				fwrite(tmp.v,sizeof(sll_char_t),tmp.l,stdout);
				free(tmp.v);
				putchar('\n');
			}
			sll_char_t bf[MAX_PATH_LENGTH+1];
			uint32_t i;
			GET_EXECUATBLE_FILE_PATH(bf,MAX_PATH_LENGTH,i);
			sll_char_t bf2[MAX_PATH_LENGTH];
			memcpy(bf2,bf,i);
			bf2[i]='.';
			bf2[i+1]='o';
			bf2[i+2]='l';
			bf2[i+3]='d';
			bf2[i+4]=0;
			rename((char*)bf,(char*)bf2);
#ifndef STANDALONE_BUILD
			while (bf[i]!='\\'&&bf[i]!='/'){
				i--;
			}
			memcpy(bf+i+1,"sll-"SLL_VERSION_STRING DYNAMIC_LIBRARY_EXTENSION,sizeof("sll-"SLL_VERSION_STRING DYNAMIC_LIBRARY_EXTENSION));
			i+=sizeof("sll-"SLL_VERSION_STRING DYNAMIC_LIBRARY_EXTENSION);
			memcpy(bf2,bf,i);
			bf2[i]='.';
			bf2[i+1]='o';
			bf2[i+2]='l';
			bf2[i+3]='d';
			bf2[i+4]=0;
			rename((char*)bf,(char*)bf2);
			while (bf[i]!='\\'&&bf[i]!='/'){
				i--;
			}
			memcpy(bf+i+1,"lib/",sizeof("lib/"));
			sll_string_t* dl;
			sll_array_length_t dll=sll_platform_list_directory_recursive(bf,&dl);
			for (sll_array_length_t j=0;j<dll;j++){
				sll_string_t* f_fp=dl+j;
				remove((char*)f_fp->v);
				free(f_fp->v);
			}
			free(dl);
#endif
			sll_string_from_pointer(SLL_CHAR("/data/"SLL_TYPE),&p);
			r.dt=&r_dt;
			if (fl&FLAG_VERBOSE){
				PRINT_STATIC_STR("Downloading '");
				fputs((char*)h.v,stdout);
				fputs((char*)p.v,stdout);
				PRINT_STATIC_STR("'...\n");
			}
			rc=sll_url_http_request(&m,&h,&p,&hl,&dt,&r);
			if (rc!=200){
				COLOR_RED;
				PRINT_STATIC_STR("Unexpected HTTP Status Code: ");
				sll_string_from_int(rc,&tmp);
				fwrite(tmp.v,sizeof(sll_char_t),tmp.l,stdout);
				free(tmp.v);
				PRINT_STATIC_STR("\n");
				COLOR_RESET;
				sll_free_http_response(&r);
				sll_free_json_object(&json);
				RESET_CONSOLE;
				return 1;
			}
			while (bf[i]!='\\'&&bf[i]!='/'){
				i--;
			}
			i++;
			sll_string_length_t j=0;
			while (j<r_dt.l){
				uint8_t nml=r_dt.v[j];
				uint32_t sz=(((uint32_t)r_dt.v[j+1])<<24)|(((uint32_t)r_dt.v[j+2])<<16)|(((uint32_t)r_dt.v[j+3])<<8)|r_dt.v[j+4];
				j+=5;
				memcpy(bf+i,r_dt.v+j,nml);
				bf[i+nml]=0;
				if (fl&FLAG_VERBOSE){
					PRINT_STATIC_STR("Extracting '");
					fputs((char*)(bf+i),stdout);
					PRINT_STATIC_STR("' into '");
					fputs((char*)bf,stdout);
					PRINT_STATIC_STR("' (");
					sll_string_from_int(sz,&tmp);
					fwrite(tmp.v,sizeof(sll_char_t),tmp.l,stdout);
					free(tmp.v);
					PRINT_STATIC_STR(" bytes)\n");
				}
				j+=nml;
				FILE* wf=fopen((char*)bf,"wb");
				if (!wf){
					COLOR_RED;
					PRINT_STATIC_STR("Unable to Open File '");
					fputs((char*)bf,stdout);
					PRINT_STATIC_STR("'. Installation is now in a corrupted state.");
					COLOR_RESET;
					sll_free_http_response(&r);
					sll_free_json_object(&json);
					RESET_CONSOLE;
					return 1;
				}
				if (fwrite(r_dt.v+j,sizeof(sll_char_t),sz,wf)!=sz){
					COLOR_RED;
					PRINT_STATIC_STR("Unable to Write Data to File '");
					fputs((char*)bf,stdout);
					PRINT_STATIC_STR("'. Installation is now in a corrupted state.");
					COLOR_RESET;
					fclose(wf);
					sll_free_http_response(&r);
					sll_free_json_object(&json);
					RESET_CONSOLE;
					return 1;
				}
				fclose(wf);
				j+=sz;
			}
			sll_free_http_response(&r);
		}
		sll_free_json_object(&json);
		RESET_CONSOLE;
		return 0;
_json_error:
		sll_free_json_object(&json);
		COLOR_RED;
		PRINT_STATIC_STR("Malformated JSON: ");
		fputs((char*)r_dt.v,stdout);
		putchar('\n');
		COLOR_RESET;
		sll_free_http_response(&r);
		RESET_CONSOLE;
		return 1;
	}
	im_fpl=fpl;
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
		if (fl&FLAG_PRINT_ASSEMBLY){
			PRINT_STATIC_STR("  Assembly Print Mode\n");
		}
		if (fl&FLAG_PRINT_OBJECT){
			PRINT_STATIC_STR("  Object Print Mode\n");
		}
		if (!(fl&FLAG_NO_RUN)){
			PRINT_STATIC_STR("  Program Run Mode\n");
		}
		if (fl&FLAG_USE_COLORS){
			PRINT_STATIC_STR("  Use Colors\n");
		}
		if (fl&FLAG_UPDATE){
			PRINT_STATIC_STR("  Update Mode\n");
		}
		if (fl&FLAG_VERBOSE){
			PRINT_STATIC_STR("  Verbose Mode\n");
		}
		PRINT_STATIC_STR("Include Path: \n  - '");
		uint32_t i=0;
		while (i<i_fpl){
			if (i){
				PRINT_STATIC_STR("'\n  - '");
			}
			sll_string_length_t sz=sll_string_length_unaligned(SLL_CHAR(i_fp+i));
			fwrite(i_fp+i,sizeof(char),sz,stdout);
			i+=sz+1;
		}
		PRINT_STATIC_STR("'\n");
#ifndef STANDALONE_BUILD
		PRINT_STATIC_STR("Library Path: ");
		fputs(l_fp,stdout);
		putchar('\n');
#endif
	}
	if (fl&FLAG_HELP){
		goto _help;
	}
	if (!fpl&&!sll){
		COLOR_RED;
		PRINT_STATIC_STR("No Input Files Supplied\n");
		COLOR_RESET;
		goto _help;
	}
	for (uint32_t j=0;j<fpl;j++){
		char f_fp[MAX_PATH_LENGTH];
		sll_input_data_stream_t is;
		fl&=~_FLAG_ASSEMBLY_GENERATED;
		if (!load_file(*(fp+j),&a_dt,&c_dt,&f,&is,f_fp)){
			goto _error;
		}
		char bf[MAX_PATH_LENGTH];
		sll_set_argument(0,SLL_CHAR((!EXPAND_FILE_PATH(*(fp+j),bf)?*(fp+j):bf)));
		if (!execute(f_fp,&c_dt,&a_dt,&is,o_fp,&ec)){
			goto _error;
		}
		if (f){
			fclose(f);
			f=NULL;
		}
		sll_free_assembly_data(&a_dt);
		sll_free_compilation_data(&c_dt);
	}
	for (uint32_t j=0;j<sll;j++){
		sll_set_argument(0,SLL_CHAR("<console>"));
		char f_fp[MAX_PATH_LENGTH];
		sll_input_data_stream_t is;
		sll_input_buffer_t i_bf={
			SLL_CHAR(*(sl+j)),
			sll_string_length_unaligned(SLL_CHAR(*(sl+j)))
		};
		sll_stream_create_input_from_buffer(&i_bf,&is);
		if (fl&FLAG_VERBOSE){
			PRINT_STATIC_STR("Trying to Load Input as Assembly...\n");
		}
		sll_error_t e;
		if (!sll_load_assembly(&is,&a_dt,&e)){
			sll_free_assembly_data(&(a_dt));
			if (e.t==SLL_ERROR_INVALID_FILE_FORMAT){
				sll_stream_create_input_from_buffer(&i_bf,&is);
				if (!sll_load_compiled_object(&is,&c_dt,&e)){
					sll_free_compilation_data(&c_dt);
					if (e.t==SLL_ERROR_INVALID_FILE_FORMAT){
						sll_stream_create_input_from_buffer(&i_bf,&is);
						sll_init_compilation_data(SLL_CHAR("<console>"),&is,&c_dt);
						if (!sll_parse_all_objects(&c_dt,&i_ft,load_import,&e)){
							if (e.t!=SLL_ERROR_UNKNOWN){
								sll_print_error(&is,&e);
							}
							goto _error;
						}
						if (fl&FLAG_PRINT_OBJECT){
							sll_output_data_stream_t os;
							sll_stream_create_output_from_file(stdout,&os);
							sll_print_object(&c_dt,c_dt.h,&os);
							putchar('\n');
						}
						if (fl&FLAG_VERBOSE){
							PRINT_STATIC_STR("Input Successfully Read.\n");
						}
					}
					else{
						sll_print_error(&is,&e);
						goto _error;
					}
				}
				else{
					if (fl&FLAG_PRINT_OBJECT){
						sll_output_data_stream_t os;
						sll_stream_create_output_from_file(stdout,&os);
						sll_print_object(&c_dt,c_dt.h,&os);
						putchar('\n');
					}
					if (fl&FLAG_VERBOSE){
						PRINT_STATIC_STR("Input Successfully Read.\n");
					}
				}
			}
			else{
				sll_print_error(&is,&e);
				goto _error;
			}
		}
		else if (fl&FLAG_VERBOSE){
			PRINT_STATIC_STR("Input Successfully Read.\n");
		}
		if (!execute(f_fp,&c_dt,&a_dt,&is,o_fp,&ec)){
			goto _error;
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
	if (sll){
		free(sl);
	}
	sll_free_internal_function_table(&i_ft);
	RESET_CONSOLE;
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
	if (sll){
		free(sl);
	}
	sll_free_assembly_data(&a_dt);
	sll_free_compilation_data(&c_dt);
	if (f){
		fclose(f);
	}
	sll_free_internal_function_table(&i_ft);
	RESET_CONSOLE;
	return ec;
}
