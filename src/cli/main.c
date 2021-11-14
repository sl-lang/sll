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
#define FLAG_GENERATE_ASSEMBLY 2
#define FLAG_GENERATE_COMPILED_OBJECT 4
#define FLAG_HELP 8
#define FLAG_NO_RUN 16
#define FLAG_PRINT_ASSEMBLY 32
#define FLAG_PRINT_OBJECT 64
#define FLAG_USE_COLORS 128
#define FLAG_VERBOSE 256
#define FLAG_VERSION 512
#define _FLAG_ASSEMBLY_GENERATED 1024
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



static uint8_t ol;
static uint16_t fl;
static sll_char_t* i_fp;
static sll_string_length_t i_fpl;
static sll_char_t** fp;
static sll_string_length_t fpl;
static sll_char_t l_fp[SLL_API_MAX_FILE_PATH_LENGTH];
static sll_string_length_t l_fpl;
#ifdef STANDALONE_BUILD
static sll_input_buffer_t m_i_bf;
#endif
static sll_internal_function_table_t i_ft;



static sll_bool_t load_file(const sll_char_t* f_nm,sll_assembly_data_t* a_dt,sll_compilation_data_t* c_dt,sll_file_t* f,sll_char_t* f_fp);



static sll_return_t load_import(const sll_string_t* nm,sll_compilation_data_t* o,sll_error_t* e){
	sll_assembly_data_t a_dt=SLL_INIT_ASSEMBLY_DATA_STRUCT;
	sll_file_t f;
	sll_char_t f_fp[SLL_API_MAX_FILE_PATH_LENGTH];
	if (!load_file(nm->v,&a_dt,o,&f,f_fp)){
		e->t=SLL_ERROR_UNKNOWN;
		return SLL_RETURN_ERROR;
	}
	sll_file_close(&f);
	if (fl&_FLAG_ASSEMBLY_GENERATED){
		sll_deinit_assembly_data(&a_dt);
		COLOR_RED;
		PRINT_STATIC_STR("Importing Assembly into Compiled Objects is Not Allowed\n");
		COLOR_RESET;
		e->t=SLL_ERROR_UNKNOWN;
		return SLL_RETURN_ERROR;
	}
	return SLL_RETURN_NO_ERROR;
}



static sll_bool_t load_file(const sll_char_t* f_nm,sll_assembly_data_t* a_dt,sll_compilation_data_t* c_dt,sll_file_t* f,sll_char_t* f_fp){
	sll_string_length_t f_nm_l=sll_string_length_unaligned(f_nm);
	sll_char_t bf[SLL_API_MAX_FILE_PATH_LENGTH];
	sll_string_length_t i=0;
	while (i<i_fpl){
		sll_string_length_t j=sll_string_length_unaligned(i_fp+i);
		sll_copy_data(i_fp+i,j,bf);
		i+=j+1;
		sll_copy_data(f_nm,f_nm_l,bf+j);
		j+=f_nm_l;
		sll_copy_data(".slc",5,bf+j);
		if (fl&FLAG_VERBOSE){
			PRINT_STATIC_STR("Trying to Open File '");
			fputs((char*)bf,stdout);
			PRINT_STATIC_STR("'...\n");
		}
		if (sll_file_open(bf,SLL_FILE_FLAG_READ,f)){
			if (!(fl&FLAG_EXPAND_PATH)){
				sll_copy_data(bf,j+1,f_fp);
			}
			else{
				sll_path_absolute(bf,f_fp,SLL_API_MAX_FILE_PATH_LENGTH);
			}
			if (fl&FLAG_VERBOSE){
				PRINT_STATIC_STR("Found File '");
				fputs((char*)f_fp,stdout);
				PRINT_STATIC_STR("'\n");
			}
			sll_error_t e;
			if (!sll_load_compiled_object(f,c_dt,&e)){
				sll_deinit_compilation_data(c_dt);
				if (e.t==SLL_ERROR_INVALID_FILE_FORMAT){
					COLOR_RED;
					PRINT_STATIC_STR("File '");
					fputs((char*)f_fp,stdout);
					PRINT_STATIC_STR("' is not a Compiled Object.\n");
					COLOR_RESET;
				}
				else{
					sll_print_error(f,&e);
				}
				sll_file_close(f);
				return 0;
			}
			if (fl&FLAG_PRINT_OBJECT){
				sll_print_object(c_dt,&i_ft,c_dt->h,sll_stdout);
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
			fputs((char*)bf,stdout);
			PRINT_STATIC_STR("'...\n");
		}
		if (sll_file_open(bf,SLL_FILE_FLAG_READ,f)){
			if (!(fl&FLAG_EXPAND_PATH)){
				sll_copy_data(bf,j+1,f_fp);
			}
			else{
				sll_path_absolute(bf,f_fp,SLL_API_MAX_FILE_PATH_LENGTH);
			}
			if (fl&FLAG_VERBOSE){
				PRINT_STATIC_STR("Found File '");
				fputs((char*)f_fp,stdout);
				PRINT_STATIC_STR("'\n");
			}
			sll_error_t e;
			if (!sll_load_assembly(f,a_dt,&e)){
				sll_deinit_assembly_data(a_dt);
				if (e.t==SLL_ERROR_INVALID_FILE_FORMAT){
					sll_file_reset(f);
					if (!sll_load_compiled_object(f,c_dt,&e)){
						sll_deinit_compilation_data(c_dt);
						if (e.t==SLL_ERROR_INVALID_FILE_FORMAT){
							sll_file_reset(f);
							sll_init_compilation_data(f_fp,f,c_dt);
							if (!sll_parse_all_objects(c_dt,&i_ft,load_import,&e)){
								sll_deinit_compilation_data(c_dt);
								if (e.t!=SLL_ERROR_UNKNOWN){
									sll_print_error(f,&e);
								}
								sll_file_close(f);
								return 0;
							}
							if (fl&FLAG_PRINT_OBJECT){
								sll_print_object(c_dt,&i_ft,c_dt->h,sll_stdout);
								putchar('\n');
							}
							if (fl&FLAG_VERBOSE){
								PRINT_STATIC_STR("File Successfully Read.\n");
							}
						}
						else{
							sll_print_error(f,&e);
							sll_file_close(f);
							return 0;
						}
					}
					else{
						if (fl&FLAG_PRINT_OBJECT){
							sll_print_object(c_dt,&i_ft,c_dt->h,sll_stdout);
							putchar('\n');
						}
						if (fl&FLAG_VERBOSE){
							PRINT_STATIC_STR("File Successfully Read.\n");
						}
					}
				}
				else{
					sll_print_error(f,&e);
					sll_file_close(f);
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
		if (m->c==c&&m->nml==f_nm_l&&sll_compare_data(m->nm,f_nm,f_nm_l)==SLL_COMPARE_RESULT_EQUAL){
			if (fl&FLAG_VERBOSE){
				PRINT_STATIC_STR("Found Internal Module '");
				fputs((char*)f_nm,stdout);
				PRINT_STATIC_STR(".slc'\n");
			}
			sll_file_from_data((void*)(m->dt),m->sz,SLL_FILE_FLAG_READ,f);
			sll_error_t e;
			if (!sll_load_compiled_object(f,c_dt,&e)){
				sll_deinit_compilation_data(c_dt);
				if (e.t==SLL_ERROR_INVALID_FILE_FORMAT){
					COLOR_RED;
					PRINT_STATIC_STR("Module '");
					fputs((char*)f_nm,stdout);
					PRINT_STATIC_STR(".slc' is not a Compiled Object.\n");
					COLOR_RESET;
				}
				else{
					sll_print_error(f,&e);
				}
				sll_file_close(f);
				return 0;
			}
			if (fl&FLAG_PRINT_OBJECT){
				sll_print_object(c_dt,&i_ft,c_dt->h,sll_stdout);
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
		sll_copy_data(f_nm,f_nm_l,l_fp+l_fpl);
		sll_copy_data(".slc",5,l_fp+i);
		if (fl&FLAG_VERBOSE){
			PRINT_STATIC_STR("Trying to Open File '");
			fputs((char*)l_fp,stdout);
			PRINT_STATIC_STR("'...\n");
		}
		if (sll_file_open(l_fp,SLL_FILE_FLAG_READ,f)){
			if (!(fl&FLAG_EXPAND_PATH)){
				sll_copy_data(l_fp,i+5,f_fp);
			}
			else{
				sll_path_absolute(l_fp,f_fp,SLL_API_MAX_FILE_PATH_LENGTH);
			}
			if (fl&FLAG_VERBOSE){
				PRINT_STATIC_STR("Found File '");
				fputs((char*)f_fp,stdout);
				PRINT_STATIC_STR("'\n");
			}
			sll_error_t e;
			if (!sll_load_compiled_object(f,c_dt,&e)){
				sll_deinit_compilation_data(c_dt);
				if (e.t==SLL_ERROR_INVALID_FILE_FORMAT){
					COLOR_RED;
					PRINT_STATIC_STR("File '");
					fputs((char*)f_fp,stdout);
					PRINT_STATIC_STR("' is not a Compiled Object.\n");
					COLOR_RESET;
				}
				else{
					sll_print_error(f,&e);
				}
				sll_file_close(f);
				return 0;
			}
			if (fl&FLAG_PRINT_OBJECT){
				sll_print_object(c_dt,&i_ft,c_dt->h,sll_stdout);
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
			fputs((char*)l_fp,stdout);
			PRINT_STATIC_STR("'...\n");
		}
		if (sll_file_open(l_fp,SLL_FILE_FLAG_READ,f)){
			if (!(fl&FLAG_EXPAND_PATH)){
				sll_copy_data(l_fp,l_fpl+5,f_fp);
			}
			else{
				sll_path_absolute(l_fp,f_fp,SLL_API_MAX_FILE_PATH_LENGTH);
			}
			if (fl&FLAG_VERBOSE){
				PRINT_STATIC_STR("Found File '");
				fputs((char*)f_fp,stdout);
				PRINT_STATIC_STR("'\n");
			}
			sll_error_t e;
			sll_init_compilation_data(l_fp,f,c_dt);
			if (!sll_parse_all_objects(c_dt,&i_ft,load_import,&e)){
				sll_deinit_compilation_data(c_dt);
				if (e.t!=SLL_ERROR_UNKNOWN){
					sll_print_error(f,&e);
				}
				sll_file_close(f);
				return 0;
			}
			if (fl&FLAG_PRINT_OBJECT){
				sll_print_object(c_dt,&i_ft,c_dt->h,sll_stdout);
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
	fputs((char*)f_nm,stdout);
	PRINT_STATIC_STR("'\n");
	COLOR_RESET;
	return 0;
}



static sll_bool_t execute(const sll_char_t* f_fp,sll_compilation_data_t* c_dt,sll_assembly_data_t* a_dt,sll_file_t* f,const sll_char_t* o_fp,int* ec){
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
			sll_print_object(c_dt,&i_ft,c_dt->h,sll_stdout);
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
			sll_print_error(f,&e);
			return 0;
		}
	}
	if (fl&FLAG_PRINT_ASSEMBLY){
		sll_print_assembly(a_dt,sll_stdout);
		putchar('\n');
	}
	if (fl&(FLAG_GENERATE_ASSEMBLY|FLAG_GENERATE_COMPILED_OBJECT)){
		sll_char_t bf[SLL_API_MAX_FILE_PATH_LENGTH];
		uint16_t i=0;
		sll_string_length_t f_fp_l=sll_string_length_unaligned(f_fp);
		if (!o_fp){
			sll_copy_data(f_fp,f_fp_l,bf);
			i=f_fp_l-1;
		}
		else{
			if (fpl==1){
				i=sll_string_length_unaligned(o_fp);
				sll_copy_data(o_fp,i,bf);
				i-=1;
			}
			else{
				i=sll_string_length_unaligned(o_fp);
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
				sll_copy_data(f_fp+j,f_fp_l-j,bf+i);
				i+=f_fp_l-j-1;
			}
		}
		i++;
		if (fl&FLAG_GENERATE_ASSEMBLY){
			sll_copy_data(".sla",5,bf+i);
			if (fl&FLAG_VERBOSE){
				PRINT_STATIC_STR("Writing Assembly to File '");
				fputs((char*)bf,stdout);
				PRINT_STATIC_STR("'...\n");
			}
			sll_file_t of;
			if (!sll_file_open(bf,SLL_FILE_FLAG_WRITE,&of)){
				COLOR_RED;
				PRINT_STATIC_STR("Unable to Open Output File '");
				fputs((char*)bf,stdout);
				PRINT_STATIC_STR("'\n");
				COLOR_RESET;
				return 0;
			}
			sll_write_assembly(&of,a_dt);
			if (fl&FLAG_VERBOSE){
				PRINT_STATIC_STR("File Written Successfully.\n");
			}
			sll_file_close(&of);
		}
		if (fl&FLAG_GENERATE_COMPILED_OBJECT){
			sll_copy_data(".slc",5,bf+i);
			if (fl&FLAG_VERBOSE){
				PRINT_STATIC_STR("Writing Compiled Object to File '");
				fputs((char*)bf,stdout);
				PRINT_STATIC_STR("'...\n");
			}
			sll_file_t of;
			if (!sll_file_open(bf,SLL_FILE_FLAG_WRITE,&of)){
				COLOR_RED;
				PRINT_STATIC_STR("Unable to Open Output File '");
				fputs((char*)bf,stdout);
				PRINT_STATIC_STR("'\n");
				COLOR_RESET;
				return 0;
			}
			sll_write_compiled_object(&of,c_dt);
			if (fl&FLAG_VERBOSE){
				PRINT_STATIC_STR("File Written Successfully.\n");
			}
			sll_file_close(&of);
		}
	}
	if (!(fl&FLAG_NO_RUN)){
		sll_handle_list_t hl=SLL_INIT_HANDLE_LIST_STRUCT;
		sll_runtime_data_t r_dt={
			&i_ft,
			&hl,
			sll_stdin,
			sll_stdout
		};
		sll_error_t e={
			SLL_ERROR_UNKNOWN
		};
		sll_return_code_t r=sll_execute_assembly(a_dt,VM_STACK_SIZE,&r_dt,&e);
		sll_deinit_handle_list(&hl);
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
	atexit(sll_deinit);
	sll_platform_enable_console_color();
	int ec=1;
	ol=OPTIMIZE_LEVEL_STRIP_DEBUG_DATA;
	fl=0;
	i_fp=malloc(sizeof(sll_char_t));
	if (!i_fp){
		PRINT_STATIC_STR("Unable to Allocate Space for Include File Path Array\n");
		sll_deinit();
		return 1;
	}
	*i_fp=0;
	i_fpl=1;
	l_fpl=sll_platform_get_executable_file_path(l_fp,SLL_API_MAX_FILE_PATH_LENGTH);
#ifndef STANDALONE_BUILD
	while (l_fp[l_fpl]!='/'&&l_fp[l_fpl]!='\\'){
		if (!l_fpl){
			goto _skip_lib_path;
		}
		l_fpl--;
	}
	sll_copy_data("lib/",5,l_fp+l_fpl+1);
	l_fpl+=5;
_skip_lib_path:
#endif
	fp=NULL;
	fpl=0;
	sll_char_t** sl=NULL;
	uint32_t sll=0;
	sll_create_internal_function_table(&i_ft);
	sll_register_standard_internal_functions(&i_ft);
	const sll_char_t* o_fp=NULL;
	sll_file_t f={0};
	sll_assembly_data_t a_dt={0};
	sll_compilation_data_t c_dt={0};
	uint32_t im_fpl=UINT32_MAX;
	sll_set_argument_count(1);
	for (int i=1;i<argc;i++){
		const char* e=argv[i];
		if ((*e=='-'&&*(e+1)=='a'&&*(e+2)==0)||sll_string_compare_pointer(SLL_CHAR(e),SLL_CHAR("--generate-assembly"))==SLL_COMPARE_RESULT_EQUAL){
			fl|=FLAG_GENERATE_ASSEMBLY;
		}
		else if ((*e=='-'&&*(e+1)=='A'&&*(e+2)==0)||sll_string_compare_pointer(SLL_CHAR(e),SLL_CHAR("--args"))==SLL_COMPARE_RESULT_EQUAL){
			sll_set_argument_count(argc-i);
			for (sll_integer_t j=0;j<(sll_integer_t)(argc-i-1);j++){
				sll_set_argument(j+1,SLL_CHAR(*(argv+i+j+1)));
			}
			break;
		}
		else if ((*e=='-'&&*(e+1)=='c'&&*(e+2)==0)||sll_string_compare_pointer(SLL_CHAR(e),SLL_CHAR("--generate-compiled-object"))==SLL_COMPARE_RESULT_EQUAL){
			fl|=FLAG_GENERATE_COMPILED_OBJECT;
		}
		else if ((*e=='-'&&*(e+1)=='C'&&*(e+2)==0)||sll_string_compare_pointer(SLL_CHAR(e),SLL_CHAR("--use-colors"))==SLL_COMPARE_RESULT_EQUAL){
			fl|=FLAG_USE_COLORS;
		}
		else if ((*e=='-'&&*(e+1)=='e'&&*(e+2)==0)||sll_string_compare_pointer(SLL_CHAR(e),SLL_CHAR("--expand-file-paths"))==SLL_COMPARE_RESULT_EQUAL){
			fl|=FLAG_EXPAND_PATH;
		}
		else if ((*e=='-'&&*(e+1)=='f'&&*(e+2)==0)||sll_string_compare_pointer(SLL_CHAR(e),SLL_CHAR("--file"))==SLL_COMPARE_RESULT_EQUAL){
			i++;
			if (i==argc){
				break;
			}
			goto _read_file_argument;
		}
		else if ((*e=='-'&&*(e+1)=='h'&&*(e+2)==0)||sll_string_compare_pointer(SLL_CHAR(e),SLL_CHAR("--help"))==SLL_COMPARE_RESULT_EQUAL){
			fl|=FLAG_HELP;
		}
		else if ((*e=='-'&&*(e+1)=='I'&&*(e+2)==0)||sll_string_compare_pointer(SLL_CHAR(e),SLL_CHAR("--include"))==SLL_COMPARE_RESULT_EQUAL){
			i++;
			if (i==argc){
				break;
			}
			e=argv[i];
			sll_string_length_t sz=sll_string_length_unaligned(SLL_CHAR(e));
			if (sz){
				uint32_t j=i_fpl;
				i_fpl+=sz+(*(e+sz-1)!='\\'&&*(e+sz-1)!='/'?2:1);
				void* tmp=realloc(i_fp,i_fpl*sizeof(sll_char_t));
				if (!tmp){
					COLOR_RED;
					PRINT_STATIC_STR("Unable to Allocate Space for Include File Path Array\n");
					COLOR_RESET;
					goto _error;
				}
				i_fp=tmp;
				sll_copy_data(e,sz,i_fp+j);
				j+=sz;
				if (*(e+sz-1)!='\\'&&*(e+sz-1)!='/'){
					*(i_fp+j)='/';
					j++;
				}
				*(i_fp+j)=0;
			}
		}
		else if ((*e=='-'&&*(e+1)=='o'&&*(e+2)==0)||sll_string_compare_pointer(SLL_CHAR(e),SLL_CHAR("--output"))==SLL_COMPARE_RESULT_EQUAL){
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
			o_fp=(const sll_char_t*)(argv[i]);
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
		else if ((*e=='-'&&*(e+1)=='P'&&*(e+2)==0)||sll_string_compare_pointer(SLL_CHAR(e),SLL_CHAR("--print-assembly"))==SLL_COMPARE_RESULT_EQUAL){
			fl|=FLAG_PRINT_ASSEMBLY;
		}
		else if ((*e=='-'&&*(e+1)=='p'&&*(e+2)==0)||sll_string_compare_pointer(SLL_CHAR(e),SLL_CHAR("--print-objects"))==SLL_COMPARE_RESULT_EQUAL){
			fl|=FLAG_PRINT_OBJECT;
		}
		else if ((*e=='-'&&*(e+1)=='R'&&*(e+2)==0)||sll_string_compare_pointer(SLL_CHAR(e),SLL_CHAR("--no-run"))==SLL_COMPARE_RESULT_EQUAL){
			fl|=FLAG_NO_RUN;
		}
		else if ((*e=='-'&&*(e+1)=='s'&&*(e+2)==0)||sll_string_compare_pointer(SLL_CHAR(e),SLL_CHAR("--source"))==SLL_COMPARE_RESULT_EQUAL){
			i++;
			if (i==argc){
				break;
			}
			sll++;
			void* tmp=realloc(sl,sll*sizeof(sll_char_t*));
			if (!tmp){
				COLOR_RED;
				PRINT_STATIC_STR("Unable to Allocate Space for Source Code Array\n");
				COLOR_RESET;
				goto _error;
			}
			sl=tmp;
			*(sl+sll-1)=(sll_char_t*)argv[i];
		}
		else if ((*e=='-'&&*(e+1)=='v'&&*(e+2)==0)||sll_string_compare_pointer(SLL_CHAR(e),SLL_CHAR("--verbose"))==SLL_COMPARE_RESULT_EQUAL){
			fl|=FLAG_VERBOSE;
		}
		else if ((*e=='-'&&*(e+1)=='V'&&*(e+2)==0)||sll_string_compare_pointer(SLL_CHAR(e),SLL_CHAR("--version"))==SLL_COMPARE_RESULT_EQUAL){
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
			void* tmp=realloc(fp,fpl*sizeof(sll_char_t*));
			if (!tmp){
				COLOR_RED;
				PRINT_STATIC_STR("Unable to Allocate Space for File Path Array\n");
				COLOR_RESET;
				goto _error;
			}
			fp=tmp;
			*(fp+fpl-1)=(sll_char_t*)e;
		}
	}
	if (fl&FLAG_VERSION){
		PRINT_STATIC_STR("sll "STR(SLL_VERSION_MAJOR)"."STR(SLL_VERSION_MINOR)"."STR(SLL_VERSION_PATCH)" ("STANDALONE_STRING TYPE_STRING SLL_VERSION_BUILD_DATE", "SLL_VERSION_BUILD_TIME")\n");
		sll_deinit();
		return 0;
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
		if (fl&FLAG_VERBOSE){
			PRINT_STATIC_STR("  Verbose Mode\n");
		}
		PRINT_STATIC_STR("Include Path: \n  - '");
		uint32_t i=0;
		while (i<i_fpl){
			if (i){
				PRINT_STATIC_STR("'\n  - '");
			}
			sll_string_length_t sz=sll_string_length_unaligned(i_fp+i);
			fwrite(i_fp+i,sizeof(sll_char_t),sz,stdout);
			i+=sz+1;
		}
		PRINT_STATIC_STR("'\n");
#ifndef STANDALONE_BUILD
		PRINT_STATIC_STR("Library Path: ");
		fputs((char*)l_fp,stdout);
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
		sll_char_t f_fp[SLL_API_MAX_FILE_PATH_LENGTH];
		fl&=~_FLAG_ASSEMBLY_GENERATED;
		if (!load_file(*(fp+j),&a_dt,&c_dt,&f,f_fp)){
			goto _error;
		}
		sll_char_t bf[SLL_API_MAX_FILE_PATH_LENGTH];
		sll_path_absolute(*(fp+j),bf,SLL_API_MAX_FILE_PATH_LENGTH);
		sll_set_argument(0,bf);
		if (!execute(f_fp,&c_dt,&a_dt,&f,o_fp,&ec)){
			goto _error;
		}
		if (f.nm.v){
			sll_file_close(&f);
		}
		sll_deinit_assembly_data(&a_dt);
		sll_deinit_compilation_data(&c_dt);
	}
	for (uint32_t j=0;j<sll;j++){
		sll_set_argument(0,SLL_CHAR("<console>"));
		sll_char_t f_fp[SLL_API_MAX_FILE_PATH_LENGTH];
		sll_file_from_data(*(sl+j),sll_string_length_unaligned(*(sl+j)),SLL_FILE_FLAG_READ,&f);
		if (fl&FLAG_VERBOSE){
			PRINT_STATIC_STR("Trying to Load Input as Assembly...\n");
		}
		sll_error_t e;
		if (!sll_load_assembly(&f,&a_dt,&e)){
			sll_deinit_assembly_data(&(a_dt));
			if (e.t==SLL_ERROR_INVALID_FILE_FORMAT){
				sll_file_reset(&f);
				if (!sll_load_compiled_object(&f,&c_dt,&e)){
					sll_deinit_compilation_data(&c_dt);
					if (e.t==SLL_ERROR_INVALID_FILE_FORMAT){
						sll_file_reset(&f);
						sll_init_compilation_data(SLL_CHAR("<console>"),&f,&c_dt);
						if (!sll_parse_all_objects(&c_dt,&i_ft,load_import,&e)){
							if (e.t!=SLL_ERROR_UNKNOWN){
								sll_print_error(&f,&e);
							}
							goto _error;
						}
						if (fl&FLAG_PRINT_OBJECT){
							sll_print_object(&c_dt,&i_ft,c_dt.h,sll_stdout);
							putchar('\n');
						}
						if (fl&FLAG_VERBOSE){
							PRINT_STATIC_STR("Input Successfully Read.\n");
						}
					}
					else{
						sll_print_error(&f,&e);
						goto _error;
					}
				}
				else{
					if (fl&FLAG_PRINT_OBJECT){
						sll_print_object(&c_dt,&i_ft,c_dt.h,sll_stdout);
						putchar('\n');
					}
					if (fl&FLAG_VERBOSE){
						PRINT_STATIC_STR("Input Successfully Read.\n");
					}
				}
			}
			else{
				sll_print_error(&f,&e);
				goto _error;
			}
		}
		else if (fl&FLAG_VERBOSE){
			PRINT_STATIC_STR("Input Successfully Read.\n");
		}
		if (!execute(f_fp,&c_dt,&a_dt,&f,o_fp,&ec)){
			goto _error;
		}
		sll_deinit_assembly_data(&a_dt);
		sll_deinit_compilation_data(&c_dt);
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
	sll_deinit_internal_function_table(&i_ft);
	sll_deinit();
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
	sll_deinit_assembly_data(&a_dt);
	sll_deinit_compilation_data(&c_dt);
	if (f.nm.v){
		sll_file_close(&f);
	}
	sll_deinit_internal_function_table(&i_ft);
	sll_deinit();
	return ec;
}
