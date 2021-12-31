#ifdef _MSC_VER
#define WIN32_LEAN_AND_MEAN 1
#include <windows.h>
#else
#include <linux/limits.h>
#include <unistd.h>
#endif
#include <help_text.h>
#include <sll.h>
#include <stdlib.h>



#define PRINT_STATIC_STR(s) sll_file_write(sll_stdout,(s),sizeof(s)/sizeof(sll_char_t)-1)
#define _STR(x) #x
#define STR(x) _STR(x)
#define _PRINT_IF_USE_COLOR(x) \
	do{ \
		if (fl&FLAG_USE_COLORS){ \
			PRINT_STATIC_STR(x); \
		} \
	} while (0)
#define COLOR_RESET _PRINT_IF_USE_COLOR("\x1b[0m")
#define COLOR_RED _PRINT_IF_USE_COLOR("\x1b[91m")
#define COLOR_YELLOW _PRINT_IF_USE_COLOR("\x1b[33m")
#define VM_STACK_SIZE 65536
#define FLAG_EXPAND_PATH 1
#define FLAG_GENERATE_ASSEMBLY 2
#define FLAG_GENERATE_COMPILED_OBJECT 4
#define FLAG_GENERATE_SLL 8
#define FLAG_HELP 16
#define FLAG_NO_RUN 32
#define FLAG_OPTIMIZE 64
#define FLAG_PRINT_ASSEMBLY 128
#define FLAG_PRINT_OBJECT 256
#define FLAG_STRIP_DEBUG 512
#define FLAG_USE_COLORS 1024
#define FLAG_VERBOSE 2048
#define FLAG_VERSION 4096
#define _FLAG_ASSEMBLY_GENERATED 8192
#define _FLAG_SINGLE_OUTPUT 16384
#ifdef SLL_VERSION_HAS_SHA
#define TYPE_STRING "commit/"SLL_VERSION_SHA" [https://github.com/sl-lang/sll/tree/"SLL_VERSION_TAG"], "
#else
#define TYPE_STRING "local, "
#endif



static uint16_t fl;
static sll_char_t* i_fp;
static sll_string_length_t i_fpl;
static sll_char_t l_fp[SLL_API_MAX_FILE_PATH_LENGTH];
static sll_string_length_t l_fpl;
static sll_internal_function_table_t i_ft;



static sll_bool_t load_file(const sll_char_t* f_nm,sll_assembly_data_t* a_dt,sll_compilation_data_t* c_dt,sll_file_t* f,sll_char_t* f_fp);



static sll_bool_t import_file(const sll_string_t* nm,sll_compilation_data_t* o){
	sll_assembly_data_t a_dt=SLL_INIT_ASSEMBLY_DATA_STRUCT;
	sll_file_t f;
	sll_char_t f_fp[SLL_API_MAX_FILE_PATH_LENGTH];
	if (!load_file(nm->v,&a_dt,o,&f,f_fp)){
		return 0;
	}
	sll_file_close(&f);
	if (fl&_FLAG_ASSEMBLY_GENERATED){
		sll_free_assembly_data(&a_dt);
		COLOR_RED;
		PRINT_STATIC_STR("Importing assembly into compiled nodes is not allowed\n");
		COLOR_RESET;
		return 0;
	}
	return 1;
}



static sll_bool_t parse_file(sll_compilation_data_t* c_dt,sll_file_t* f,const sll_char_t* f_fp){
	sll_init_compilation_data(f_fp,f,c_dt);
	sll_error_t e;
	if (!sll_parse_all_nodes(c_dt,&i_ft,import_file,&e)){
		sll_free_compilation_data(c_dt);
		sll_print_error(f,&e);
		sll_file_close(f);
		return 0;
	}
	if (fl&FLAG_PRINT_OBJECT){
		sll_print_node(c_dt,&i_ft,c_dt->h,sll_stdout);
		sll_file_write_char(sll_stdout,'\n');
	}
	if (fl&FLAG_VERBOSE){
		PRINT_STATIC_STR("File successfully read.\n");
	}
	return 1;
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
		SLL_COPY_STRING_NULL(SLL_CHAR(".slc"),bf+j);
		if (fl&FLAG_VERBOSE){
			sll_file_write_format(sll_stdout,SLL_CHAR("Trying to open file '%s'...\n"),bf);
		}
		if (sll_file_open(bf,SLL_FILE_FLAG_READ,f)){
			if (!(fl&FLAG_EXPAND_PATH)){
				sll_copy_data(bf,j+1,f_fp);
			}
			else{
				sll_platform_absolute_path(bf,f_fp,SLL_API_MAX_FILE_PATH_LENGTH);
			}
			if (fl&FLAG_VERBOSE){
				sll_file_write_format(sll_stdout,SLL_CHAR("Found file '%s'\n"),f_fp);
			}
			if (!sll_load_compiled_node(f,c_dt)){
				sll_free_compilation_data(c_dt);
				sll_file_close(f);
				COLOR_RED;
				sll_file_write_format(sll_stdout,SLL_CHAR("File '%s' is not a compiled program.\n"),f_fp);
				COLOR_RESET;
				return 0;
			}
			if (fl&FLAG_PRINT_OBJECT){
				sll_print_node(c_dt,&i_ft,c_dt->h,sll_stdout);
				sll_file_write_char(sll_stdout,'\n');
			}
			if (fl&FLAG_VERBOSE){
				PRINT_STATIC_STR("File successfully read.\n");
			}
			return 1;
		}
		bf[j]=0;
		if (fl&FLAG_VERBOSE){
			sll_file_write_format(sll_stdout,SLL_CHAR("Trying to open file '%s'...\n"),bf);
		}
		if (!sll_file_open(bf,SLL_FILE_FLAG_READ,f)){
			continue;
		}
		if (!(fl&FLAG_EXPAND_PATH)){
			sll_copy_data(bf,j+1,f_fp);
		}
		else{
			sll_platform_absolute_path(bf,f_fp,SLL_API_MAX_FILE_PATH_LENGTH);
		}
		if (fl&FLAG_VERBOSE){
			sll_file_write_format(sll_stdout,SLL_CHAR("Found file '%s'\n"),f_fp);
		}
		if (sll_load_assembly(f,a_dt)){
			fl|=_FLAG_ASSEMBLY_GENERATED;
			if (fl&FLAG_VERBOSE){
				PRINT_STATIC_STR("File successfully read.\n");
			}
			return 1;
		}
		sll_free_assembly_data(a_dt);
		sll_file_reset(f);
		if (sll_load_compiled_node(f,c_dt)){
			if (fl&FLAG_PRINT_OBJECT){
				sll_print_node(c_dt,&i_ft,c_dt->h,sll_stdout);
				sll_file_write_char(sll_stdout,'\n');
			}
			if (fl&FLAG_VERBOSE){
				PRINT_STATIC_STR("File successfully read.\n");
			}
			return 1;
		}
		sll_free_compilation_data(c_dt);
		sll_file_reset(f);
		return parse_file(c_dt,f,f_fp);
	}
	if (l_fpl){
		i=l_fpl+f_nm_l;
		sll_copy_data(f_nm,f_nm_l,l_fp+l_fpl);
		SLL_COPY_STRING_NULL(SLL_CHAR(".slc"),l_fp+i);
		if (fl&FLAG_VERBOSE){
			sll_file_write_format(sll_stdout,SLL_CHAR("Trying to open file '%s'...\n"),l_fp);
		}
		if (sll_file_open(l_fp,SLL_FILE_FLAG_READ,f)){
			if (!(fl&FLAG_EXPAND_PATH)){
				sll_copy_data(l_fp,i+5,f_fp);
			}
			else{
				sll_platform_absolute_path(l_fp,f_fp,SLL_API_MAX_FILE_PATH_LENGTH);
			}
			if (fl&FLAG_VERBOSE){
				sll_file_write_format(sll_stdout,SLL_CHAR("Found file '%s'\n"),f_fp);
			}
			if (sll_load_compiled_node(f,c_dt)){
				if (fl&FLAG_PRINT_OBJECT){
					sll_print_node(c_dt,&i_ft,c_dt->h,sll_stdout);
					sll_file_write_char(sll_stdout,'\n');
				}
				if (fl&FLAG_VERBOSE){
					PRINT_STATIC_STR("File successfully read.\n");
				}
				return 1;
			}
			sll_free_compilation_data(c_dt);
			sll_file_close(f);
			COLOR_RED;
			sll_file_write_format(sll_stdout,SLL_CHAR("File '%s' is not a compiled program\n"),f_fp);
			COLOR_RESET;
			return 0;
		}
		*(l_fp+i)=0;
		if (fl&FLAG_VERBOSE){
			sll_file_write_format(sll_stdout,SLL_CHAR("Trying to open file '%s'...\n"),l_fp);
		}
		if (sll_file_open(l_fp,SLL_FILE_FLAG_READ,f)){
			if (!(fl&FLAG_EXPAND_PATH)){
				sll_copy_data(l_fp,l_fpl+5,f_fp);
			}
			else{
				sll_platform_absolute_path(l_fp,f_fp,SLL_API_MAX_FILE_PATH_LENGTH);
			}
			if (fl&FLAG_VERBOSE){
				sll_file_write_format(sll_stdout,SLL_CHAR("Found file '%s'\n"),f_fp);
			}
			return parse_file(c_dt,f,l_fp);
		}
	}
	COLOR_RED;
	sll_file_write_format(sll_stdout,SLL_CHAR("Unable to find file '%s'\n"),f_nm);
	COLOR_RESET;
	return 0;
}



static sll_bool_t execute(const sll_char_t* f_fp,sll_compilation_data_t* c_dt,sll_assembly_data_t* a_dt,sll_file_t* f,const sll_char_t* o_fp,int* ec){
	if (!(fl&_FLAG_ASSEMBLY_GENERATED)){
		if (fl&FLAG_OPTIMIZE){
			if (fl&FLAG_VERBOSE){
				PRINT_STATIC_STR("Performing global optimization...\n");
			}
			sll_optimize_node(c_dt,&i_ft);
		}
		if (fl&FLAG_STRIP_DEBUG){
			if (fl&FLAG_VERBOSE){
				PRINT_STATIC_STR("Removing debugging data...\n");
			}
			sll_remove_debug_data(c_dt);
		}
		if (fl&FLAG_VERBOSE){
			PRINT_STATIC_STR("Removing node padding...\n");
		}
		sll_remove_node_padding(c_dt,c_dt->h);
		if (fl&FLAG_VERBOSE){
			PRINT_STATIC_STR("Optimizing node metadata...\n");
		}
		sll_optimize_metadata(c_dt);
		if (fl&FLAG_PRINT_OBJECT){
			sll_print_node(c_dt,&i_ft,c_dt->h,sll_stdout);
			sll_file_write_char(sll_stdout,'\n');
		}
	}
	if (!(fl&_FLAG_ASSEMBLY_GENERATED)&&((fl&(FLAG_GENERATE_ASSEMBLY|FLAG_PRINT_ASSEMBLY))||!(fl&FLAG_NO_RUN))){
		if (fl&FLAG_VERBOSE){
			PRINT_STATIC_STR("Generating assembly...\n");
		}
		sll_error_t e;
		if (!sll_generate_assembly(c_dt,a_dt,&e)){
			sll_print_error(f,&e);
			return 0;
		}
	}
	if (fl&FLAG_PRINT_ASSEMBLY){
		sll_print_assembly(a_dt,sll_stdout);
		sll_file_write_char(sll_stdout,'\n');
	}
	if (fl&(FLAG_GENERATE_ASSEMBLY|FLAG_GENERATE_COMPILED_OBJECT|FLAG_GENERATE_SLL)){
		sll_char_t bf[SLL_API_MAX_FILE_PATH_LENGTH];
		uint16_t i=0;
		sll_string_length_t f_fp_l=sll_string_length_unaligned(f_fp);
		if (!o_fp){
			sll_copy_data(f_fp,f_fp_l,bf);
			i=f_fp_l-1;
		}
		else{
			if (fl&_FLAG_SINGLE_OUTPUT){
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
			SLL_COPY_STRING_NULL(SLL_CHAR(".sla"),bf+i);
			if (fl&FLAG_VERBOSE){
				sll_file_write_format(sll_stdout,SLL_CHAR("Writing assembly to file '%s'...\n"),bf);
			}
			sll_file_t of;
			if (!sll_file_open(bf,SLL_FILE_FLAG_WRITE,&of)){
				COLOR_RED;
				sll_file_write_format(sll_stdout,SLL_CHAR("Unable to open output file '%s'\n"),bf);
				COLOR_RESET;
				return 0;
			}
			sll_write_assembly(&of,a_dt);
			if (fl&FLAG_VERBOSE){
				PRINT_STATIC_STR("File written successfully.\n");
			}
			sll_file_close(&of);
		}
		if (fl&FLAG_GENERATE_COMPILED_OBJECT){
			SLL_COPY_STRING_NULL(SLL_CHAR(".slc"),bf+i);
			if (fl&FLAG_VERBOSE){
				sll_file_write_format(sll_stdout,SLL_CHAR("Writing compiled program to file '%s'...\n"),bf);
			}
			sll_file_t of;
			if (!sll_file_open(bf,SLL_FILE_FLAG_WRITE,&of)){
				COLOR_RED;
				sll_file_write_format(sll_stdout,SLL_CHAR("Unable to open output file '%s'\n"),bf);
				COLOR_RESET;
				return 0;
			}
			sll_write_compiled_node(&of,c_dt);
			if (fl&FLAG_VERBOSE){
				PRINT_STATIC_STR("File written successfully.\n");
			}
			sll_file_close(&of);
		}
		if (fl&FLAG_GENERATE_SLL){
			SLL_COPY_STRING_NULL(SLL_CHAR(".sll"),bf+i);
			if (fl&FLAG_VERBOSE){
				sll_file_write_format(sll_stdout,SLL_CHAR("Writing sll code to file '%s'...\n"),bf);
			}
			sll_file_t of;
			if (!sll_file_open(bf,SLL_FILE_FLAG_WRITE,&of)){
				COLOR_RED;
				sll_file_write_format(sll_stdout,SLL_CHAR("Unable to open output file '%s'\n"),bf);
				COLOR_RESET;
				return 0;
			}
			sll_write_sll_code(c_dt,&i_ft,1,&of);
			if (fl&FLAG_VERBOSE){
				PRINT_STATIC_STR("File written successfully.\n");
			}
			sll_file_close(&of);
		}
	}
	if (!(fl&FLAG_NO_RUN)){
		sll_vm_config_t cfg={
			VM_STACK_SIZE,
			sll_stdin,
			sll_stdout,
			sll_stderr
		};
		sll_return_code_t r=sll_execute_assembly(a_dt,&cfg);
		sll_file_flush(sll_stdout);
		sll_file_flush(sll_stderr);
		if (r){
			*ec=r;
			return 0;
		}
	}
	return 1;
}



int main(int argc,const char** argv){
	sll_init();
	int ec=1;
	fl=0;
	i_fp=sll_allocate(sizeof(sll_char_t));
	*i_fp=0;
	i_fpl=1;
	l_fpl=sll_platform_get_library_file_path(l_fp,SLL_API_MAX_FILE_PATH_LENGTH);
	while (l_fp[l_fpl]!='/'&&l_fp[l_fpl]!='\\'){
		if (!l_fpl){
			break;
		}
		l_fpl--;
	}
	SLL_COPY_STRING_NULL(SLL_CHAR("lib/"),l_fp+l_fpl+1);
	l_fpl+=5;
	const sll_char_t** fp=NULL;
	sll_string_length_t fpl=0;
	sll_char_t** sl=NULL;
	uint32_t sll=0;
	sll_create_internal_function_table(&i_ft);
	sll_register_builtin_internal_functions(&i_ft);
	const sll_char_t* o_fp=NULL;
	sll_file_t f={0};
	sll_assembly_data_t a_dt={0};
	sll_compilation_data_t c_dt={0};
	sll_sandbox_flags_t s_fl=0;
	sll_set_argument_count(1);
	for (int i=1;i<argc;i++){
		const sll_char_t* e=SLL_CHAR(argv[i]);
		if ((*e=='-'&&*(e+1)=='a'&&*(e+2)==0)||sll_string_compare_pointer(e,SLL_CHAR("--generate-assembly"))==SLL_COMPARE_RESULT_EQUAL){
			fl|=FLAG_GENERATE_ASSEMBLY;
		}
		else if ((*e=='-'&&*(e+1)=='A'&&*(e+2)==0)||sll_string_compare_pointer(e,SLL_CHAR("--args"))==SLL_COMPARE_RESULT_EQUAL){
			sll_set_argument_count(argc-i);
			for (sll_array_length_t j=0;j<(sll_array_length_t)(argc-i-1);j++){
				sll_set_argument(j+1,SLL_CHAR(*(argv+i+j+1)));
			}
			break;
		}
		else if ((*e=='-'&&*(e+1)=='c'&&*(e+2)==0)||sll_string_compare_pointer(e,SLL_CHAR("--generate-compiled-object"))==SLL_COMPARE_RESULT_EQUAL){
			fl|=FLAG_GENERATE_COMPILED_OBJECT;
		}
		else if ((*e=='-'&&*(e+1)=='C'&&*(e+2)==0)||sll_string_compare_pointer(e,SLL_CHAR("--use-colors"))==SLL_COMPARE_RESULT_EQUAL){
			fl|=FLAG_USE_COLORS;
		}
		else if ((*e=='-'&&*(e+1)=='D'&&*(e+2)==0)||sll_string_compare_pointer(e,SLL_CHAR("--string-debug"))==SLL_COMPARE_RESULT_EQUAL){
			fl|=FLAG_STRIP_DEBUG;
		}
		else if ((*e=='-'&&*(e+1)=='e'&&*(e+2)==0)||sll_string_compare_pointer(e,SLL_CHAR("--expand-file-paths"))==SLL_COMPARE_RESULT_EQUAL){
			fl|=FLAG_EXPAND_PATH;
		}
		else if ((*e=='-'&&*(e+1)=='f'&&*(e+2)==0)||sll_string_compare_pointer(e,SLL_CHAR("--file"))==SLL_COMPARE_RESULT_EQUAL){
			i++;
			if (i==argc){
				break;
			}
			goto _read_file_argument;
		}
		else if ((*e=='-'&&*(e+1)=='F'&&*(e+2)==0)||sll_string_compare_pointer(e,SLL_CHAR("--generate-sll"))==SLL_COMPARE_RESULT_EQUAL){
			fl|=FLAG_GENERATE_SLL;
		}
		else if ((*e=='-'&&*(e+1)=='h'&&*(e+2)==0)||sll_string_compare_pointer(e,SLL_CHAR("--help"))==SLL_COMPARE_RESULT_EQUAL){
			fl|=FLAG_HELP;
		}
		else if ((*e=='-'&&*(e+1)=='I'&&*(e+2)==0)||sll_string_compare_pointer(e,SLL_CHAR("--include"))==SLL_COMPARE_RESULT_EQUAL){
			i++;
			if (i==argc){
				break;
			}
			e=SLL_CHAR(argv[i]);
			sll_string_length_t sz=sll_string_length_unaligned(e);
			if (sz){
				uint32_t j=i_fpl;
				i_fpl+=sz+(*(e+sz-1)!='\\'&&*(e+sz-1)!='/'?2:1);
				i_fp=sll_reallocate(i_fp,i_fpl*sizeof(sll_char_t));
				sll_copy_data(e,sz,i_fp+j);
				j+=sz;
				if (*(e+sz-1)!='\\'&&*(e+sz-1)!='/'){
					*(i_fp+j)='/';
					j++;
				}
				*(i_fp+j)=0;
			}
		}
		else if ((*e=='-'&&*(e+1)=='o'&&*(e+2)==0)||sll_string_compare_pointer(e,SLL_CHAR("--output"))==SLL_COMPARE_RESULT_EQUAL){
			if (o_fp){
				COLOR_RED;
				PRINT_STATIC_STR("Multplie output files supplied\n");
				COLOR_RESET;
				goto _help;
			}
			i++;
			if (i==argc){
				break;
			}
			o_fp=(const sll_char_t*)(argv[i]);
		}
		else if ((*e=='-'&&*(e+1)=='O'&&*(e+2)==0)||sll_string_compare_pointer(e,SLL_CHAR("--optimize"))==SLL_COMPARE_RESULT_EQUAL){
			fl|=FLAG_OPTIMIZE;
		}
		else if ((*e=='-'&&*(e+1)=='p'&&*(e+2)==0)||sll_string_compare_pointer(e,SLL_CHAR("--print-objects"))==SLL_COMPARE_RESULT_EQUAL){
			fl|=FLAG_PRINT_OBJECT;
		}
		else if ((*e=='-'&&*(e+1)=='P'&&*(e+2)==0)||sll_string_compare_pointer(e,SLL_CHAR("--print-assembly"))==SLL_COMPARE_RESULT_EQUAL){
			fl|=FLAG_PRINT_ASSEMBLY;
		}
		else if ((*e=='-'&&*(e+1)=='R'&&*(e+2)==0)||sll_string_compare_pointer(e,SLL_CHAR("--no-run"))==SLL_COMPARE_RESULT_EQUAL){
			fl|=FLAG_NO_RUN;
		}
		else if ((*e=='-'&&*(e+1)=='s'&&*(e+2)==0)||sll_string_compare_pointer(e,SLL_CHAR("--source"))==SLL_COMPARE_RESULT_EQUAL){
			i++;
			if (i==argc){
				break;
			}
			sll++;
			sl=sll_reallocate(sl,sll*sizeof(sll_char_t*));
			*(sl+sll-1)=(sll_char_t*)argv[i];
		}
		else if ((*e=='-'&&*(e+1)=='S'&&*(e+2)==0)||sll_string_compare_pointer(e,SLL_CHAR("--sandbox"))==SLL_COMPARE_RESULT_EQUAL){
			i++;
			if (i==argc){
				break;
			}
			e=SLL_CHAR(argv[i]);
			if (sll_string_compare_pointer(e,SLL_CHAR("no-file-io"))==SLL_COMPARE_RESULT_EQUAL){
				s_fl|=SLL_SANDBOX_FLAG_DISABLE_FILE_IO;
			}
			else if (sll_string_compare_pointer(e,SLL_CHAR("enable-stdin-io"))==SLL_COMPARE_RESULT_EQUAL){
				s_fl|=SLL_SANDBOX_FLAG_ENABLE_STDIN_IO;
			}
			else if (sll_string_compare_pointer(e,SLL_CHAR("enable-stdout-io"))==SLL_COMPARE_RESULT_EQUAL){
				s_fl|=SLL_SANDBOX_FLAG_ENABLE_STDOUT_IO;
			}
			else if (sll_string_compare_pointer(e,SLL_CHAR("no-path-api"))==SLL_COMPARE_RESULT_EQUAL){
				s_fl|=SLL_SANDBOX_FLAG_DISABLE_PATH_API;
			}
			else{
				COLOR_RED;
				sll_file_write_format(sll_stdout,SLL_CHAR("Unknown Sandbox Flag '%s'\n"),e);
				COLOR_RESET;
				goto _error;
			}
		}
		else if ((*e=='-'&&*(e+1)=='v'&&*(e+2)==0)||sll_string_compare_pointer(e,SLL_CHAR("--verbose"))==SLL_COMPARE_RESULT_EQUAL){
			fl|=FLAG_VERBOSE;
		}
		else if ((*e=='-'&&*(e+1)=='V'&&*(e+2)==0)||sll_string_compare_pointer(e,SLL_CHAR("--version"))==SLL_COMPARE_RESULT_EQUAL){
			fl|=FLAG_VERSION;
		}
		else if (*e=='-'){
			COLOR_RED;
			sll_file_write_format(sll_stdout,SLL_CHAR("Unknown Switch '%s'\n"),e);
			COLOR_RESET;
			goto _help;
		}
		else{
_read_file_argument:
			fpl++;
			fp=sll_reallocate(fp,fpl*sizeof(sll_char_t*));
			*(fp+fpl-1)=e;
		}
	}
	if (fl&FLAG_VERBOSE){
		sll_set_log_default(1);
	}
	if (fl&FLAG_USE_COLORS){
		sll_platform_enable_console_color();
	}
	if (fl&FLAG_VERSION){
		PRINT_STATIC_STR("sll "STR(SLL_VERSION_MAJOR)"."STR(SLL_VERSION_MINOR)"."STR(SLL_VERSION_PATCH)" ("TYPE_STRING SLL_VERSION_BUILD_DATE", "SLL_VERSION_BUILD_TIME")\n");
		sll_deinit();
		return 0;
	}
	if (fl&FLAG_HELP){
		goto _help;
	}
	sll_set_sandbox_flags(s_fl);
	if (fl&FLAG_VERBOSE){
		PRINT_STATIC_STR("Configuration:\n");
		if (fl&FLAG_EXPAND_PATH){
			PRINT_STATIC_STR("  Path expansion\n");
		}
		if (fl&FLAG_GENERATE_ASSEMBLY){
			PRINT_STATIC_STR("  Assembly generation\n");
		}
		if (fl&FLAG_GENERATE_COMPILED_OBJECT){
			PRINT_STATIC_STR("  Compiled program generation\n");
		}
		if (fl&FLAG_GENERATE_SLL){
			PRINT_STATIC_STR("  Sll code generation mode\n");
		}
		if (!(fl&FLAG_NO_RUN)){
			PRINT_STATIC_STR("  Execution\n");
		}
		if (fl&FLAG_OPTIMIZE){
			PRINT_STATIC_STR("  Optimization\n");
		}
		if (fl&FLAG_PRINT_ASSEMBLY){
			PRINT_STATIC_STR("  Assembly printing\n");
		}
		if (fl&FLAG_PRINT_OBJECT){
			PRINT_STATIC_STR("  Compiled program printing\n");
		}
		if (fl&FLAG_STRIP_DEBUG){
			PRINT_STATIC_STR("  Debug data stripping\n");
		}
		if (fl&FLAG_USE_COLORS){
			PRINT_STATIC_STR("  Ansi escape codes\n");
		}
		PRINT_STATIC_STR("Include path: \n  - '");
		uint32_t i=0;
		while (i<i_fpl){
			if (i){
				PRINT_STATIC_STR("'\n  - '");
			}
			sll_string_length_t sz=sll_string_length_unaligned(i_fp+i);
			sll_file_write(sll_stdout,i_fp+i,sz);
			i+=sz+1;
		}
		sll_file_write_format(sll_stdout,SLL_CHAR("'\nLibrary path '%s'\n"),l_fp);
	}
	if (!fpl&&!sll){
		COLOR_RED;
		PRINT_STATIC_STR("No input files supplied\n");
		COLOR_RESET;
		goto _help;
	}
	if (fl&FLAG_OPTIMIZE){
		COLOR_YELLOW;
		PRINT_STATIC_STR("WARNING: Optimizer is currently an alpha feature!\n");
		COLOR_RESET;
	}
	if (fpl==1){
		fl|=_FLAG_SINGLE_OUTPUT;
	}
	for (uint32_t j=0;j<fpl;j++){
		sll_char_t f_fp[SLL_API_MAX_FILE_PATH_LENGTH];
		fl&=~_FLAG_ASSEMBLY_GENERATED;
		if (!load_file(*(fp+j),&a_dt,&c_dt,&f,f_fp)){
			goto _error;
		}
		sll_char_t bf[SLL_API_MAX_FILE_PATH_LENGTH];
		sll_platform_absolute_path(*(fp+j),bf,SLL_API_MAX_FILE_PATH_LENGTH);
		sll_set_argument(0,bf);
		if (!execute(f_fp,&c_dt,&a_dt,&f,o_fp,&ec)){
			goto _error;
		}
		if (f.f){
			sll_file_close(&f);
		}
		sll_free_assembly_data(&a_dt);
		sll_free_compilation_data(&c_dt);
	}
	for (uint32_t j=0;j<sll;j++){
		sll_set_argument(0,SLL_CHAR("<console>"));
		sll_char_t f_fp[SLL_API_MAX_FILE_PATH_LENGTH];
		sll_file_from_data(*(sl+j),sll_string_length_unaligned(*(sl+j)),SLL_FILE_FLAG_READ,&f);
		if (fl&FLAG_VERBOSE){
			PRINT_STATIC_STR("Compiling console input...\n");
		}
		sll_init_compilation_data(SLL_CHAR("@console"),&f,&c_dt);
		sll_error_t e;
		if (!sll_parse_all_nodes(&c_dt,&i_ft,import_file,&e)){
			sll_print_error(&f,&e);
			goto _error;
		}
		if (fl&FLAG_PRINT_OBJECT){
			sll_print_node(&c_dt,&i_ft,c_dt.h,sll_stdout);
			sll_file_write_char(sll_stdout,'\n');
		}
		if (fl&FLAG_VERBOSE){
			PRINT_STATIC_STR("Input successfully read.\n");
		}
		if (!execute(f_fp,&c_dt,&a_dt,&f,o_fp,&ec)){
			goto _error;
		}
		sll_free_assembly_data(&a_dt);
		sll_free_compilation_data(&c_dt);
	}
	sll_deallocate(i_fp);
	if (fp){
		sll_deallocate(fp);
	}
	if (sl){
		sll_deallocate(sl);
	}
	sll_free_internal_function_table(&i_ft);
	sll_deinit();
	return 0;
_help:
	sll_file_write(sll_stdout,HELP_TEXT,HELP_TEXT_SIZE);
_error:
	sll_deallocate(i_fp);
	if (fp){
		sll_deallocate(fp);
	}
	if (sl){
		sll_deallocate(sl);
	}
	sll_free_assembly_data(&a_dt);
	sll_free_compilation_data(&c_dt);
	if (f.f){
		sll_file_close(&f);
	}
	sll_free_internal_function_table(&i_ft);
	sll_deinit();
	return ec;
}
