#include <sll/_sll_internal.h>
#include <sll/_generated_help_text.h>
#include <sll/api/path.h>
#include <sll/api/sys.h>
#include <sll/assembly.h>
#include <sll/file.h>
#include <sll/ift.h>
#include <sll/io.h>
#include <sll/log.h>
#include <sll/memory.h>
#include <sll/node.h>
#include <sll/platform.h>
#include <sll/string.h>
#include <sll/types.h>
#include <sll/util.h>
#include <sll/vm.h>
#include <stdint.h>



static uint16_t fl;
static sll_char_t* i_fp;
static sll_string_length_t i_fpl;
static sll_char_t l_fp[SLL_API_MAX_FILE_PATH_LENGTH];
static sll_string_length_t l_fpl;
static sll_internal_function_table_t i_ft;



static void _load_file(const sll_char_t* f_nm,sll_assembly_data_t* a_dt,sll_compilation_data_t* c_dt,sll_file_t* f,sll_char_t* f_fp);



static sll_bool_t _import_file(const sll_string_t* nm,sll_compilation_data_t* o){
	sll_assembly_data_t a_dt=SLL_INIT_ASSEMBLY_DATA_STRUCT;
	sll_file_t f;
	sll_char_t f_fp[SLL_API_MAX_FILE_PATH_LENGTH];
	_load_file(nm->v,&a_dt,o,&f,f_fp);
	sll_file_close(&f);
	if (fl&CLI_FLAG_ASSEMBLY_GENERATED){
		sll_free_assembly_data(&a_dt);
		SLL_WARN(SLL_CHAR("Importing assembly into compiled programs is not allowed"));
		return 0;
	}
	return 1;
}



static void _parse_file(sll_compilation_data_t* c_dt,sll_file_t* f,const sll_char_t* f_fp){
	sll_init_compilation_data(f_fp,f,c_dt);
	sll_parse_all_nodes(c_dt,&i_ft,_import_file);
	if (fl&CLI_FLAG_PRINT_OBJECT){
		sll_print_node(c_dt,&i_ft,NULL,sll_stdout);
		sll_file_write_char(sll_stdout,'\n');
	}
	if (fl&CLI_FLAG_VERBOSE){
		SLL_LOG(SLL_CHAR("File successfully read."));
	}
}



static void _load_file(const sll_char_t* f_nm,sll_assembly_data_t* a_dt,sll_compilation_data_t* c_dt,sll_file_t* f,sll_char_t* f_fp){
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
		if (fl&CLI_FLAG_VERBOSE){
			SLL_LOG(SLL_CHAR("Trying to open file '%s'..."),bf);
		}
		if (sll_file_open(bf,SLL_FILE_FLAG_READ,f)){
			if (!(fl&CLI_FLAG_EXPAND_PATH)){
				sll_copy_data(bf,j+1,f_fp);
			}
			else{
				sll_platform_absolute_path(bf,f_fp,SLL_API_MAX_FILE_PATH_LENGTH);
			}
			if (fl&CLI_FLAG_VERBOSE){
				SLL_LOG(SLL_CHAR("Found file '%s'"),f_fp);
			}
			if (sll_load_compiled_node(f,c_dt)){
				if (fl&CLI_FLAG_PRINT_OBJECT){
					sll_print_node(c_dt,&i_ft,NULL,sll_stdout);
					sll_file_write_char(sll_stdout,'\n');
				}
				if (fl&CLI_FLAG_VERBOSE){
					SLL_LOG(SLL_CHAR("File successfully read."));
				}
				return;
			}
			sll_free_compilation_data(c_dt);
			sll_file_close(f);
			if (fl&CLI_FLAG_VERBOSE){
				SLL_LOG(SLL_CHAR("File is not a compiled program"));
			}
		}
		bf[j]=0;
		if (fl&CLI_FLAG_VERBOSE){
			SLL_LOG(SLL_CHAR("Trying to open file '%s'..."),bf);
		}
		if (!sll_file_open(bf,SLL_FILE_FLAG_READ,f)){
			continue;
		}
		if (!(fl&CLI_FLAG_EXPAND_PATH)){
			sll_copy_data(bf,j+1,f_fp);
		}
		else{
			sll_platform_absolute_path(bf,f_fp,SLL_API_MAX_FILE_PATH_LENGTH);
		}
		if (fl&CLI_FLAG_VERBOSE){
			SLL_LOG(SLL_CHAR("Found file '%s'"),f_fp);
		}
		if (sll_load_assembly(f,a_dt)){
			fl|=CLI_FLAG_ASSEMBLY_GENERATED;
			if (fl&CLI_FLAG_VERBOSE){
				SLL_LOG(SLL_CHAR("File successfully read."));
			}
			return;
		}
		sll_free_assembly_data(a_dt);
		sll_file_reset(f);
		if (sll_load_compiled_node(f,c_dt)){
			if (fl&CLI_FLAG_PRINT_OBJECT){
				sll_print_node(c_dt,&i_ft,NULL,sll_stdout);
				sll_file_write_char(sll_stdout,'\n');
			}
			if (fl&CLI_FLAG_VERBOSE){
				SLL_LOG(SLL_CHAR("File successfully read."));
			}
			return;
		}
		sll_free_compilation_data(c_dt);
		sll_file_reset(f);
		_parse_file(c_dt,f,f_fp);
		return;
	}
	if (l_fpl){
		i=l_fpl+f_nm_l;
		sll_copy_data(f_nm,f_nm_l,l_fp+l_fpl);
		SLL_COPY_STRING_NULL(SLL_CHAR(".slc"),l_fp+i);
		if (fl&CLI_FLAG_VERBOSE){
			SLL_LOG(SLL_CHAR("Trying to open file '%s'..."),l_fp);
		}
		if (sll_file_open(l_fp,SLL_FILE_FLAG_READ,f)){
			if (!(fl&CLI_FLAG_EXPAND_PATH)){
				sll_copy_data(l_fp,i+5,f_fp);
			}
			else{
				sll_platform_absolute_path(l_fp,f_fp,SLL_API_MAX_FILE_PATH_LENGTH);
			}
			if (fl&CLI_FLAG_VERBOSE){
				SLL_LOG(SLL_CHAR("Found file '%s'"),f_fp);
			}
			if (sll_load_compiled_node(f,c_dt)){
				if (fl&CLI_FLAG_PRINT_OBJECT){
					sll_print_node(c_dt,&i_ft,NULL,sll_stdout);
					sll_file_write_char(sll_stdout,'\n');
				}
				if (fl&CLI_FLAG_VERBOSE){
					SLL_LOG(SLL_CHAR("File successfully read."));
				}
				return;
			}
			sll_free_compilation_data(c_dt);
			sll_file_close(f);
			if (fl&CLI_FLAG_VERBOSE){
				SLL_LOG(SLL_CHAR("File is not a compiled program"));
			}
		}
		*(l_fp+i)=0;
		if (fl&CLI_FLAG_VERBOSE){
			SLL_LOG(SLL_CHAR("Trying to open file '%s'..."),l_fp);
		}
		if (sll_file_open(l_fp,SLL_FILE_FLAG_READ,f)){
			if (!(fl&CLI_FLAG_EXPAND_PATH)){
				sll_copy_data(l_fp,l_fpl+5,f_fp);
			}
			else{
				sll_platform_absolute_path(l_fp,f_fp,SLL_API_MAX_FILE_PATH_LENGTH);
			}
			if (fl&CLI_FLAG_VERBOSE){
				SLL_LOG(SLL_CHAR("Found file '%s'"),f_fp);
			}
			_parse_file(c_dt,f,l_fp);
			return;
		}
	}
	SLL_WARN(SLL_CHAR("Unable to find file '%s'"),f_nm);
	sll_init_compilation_data(f_nm,NULL,c_dt);
}



static void _execute(const sll_char_t* f_fp,sll_compilation_data_t* c_dt,sll_assembly_data_t* a_dt,sll_file_t* f,const sll_char_t* o_fp,int* ec){
	if (!(fl&CLI_FLAG_ASSEMBLY_GENERATED)){
		if (fl&CLI_FLAG_OPTIMIZE){
			if (fl&CLI_FLAG_VERBOSE){
				SLL_LOG(SLL_CHAR("Performing global optimization..."));
			}
			sll_optimize_node(c_dt,&i_ft);
		}
		if (fl&CLI_FLAG_STRIP_DEBUG){
			if (fl&CLI_FLAG_VERBOSE){
				SLL_LOG(SLL_CHAR("Removing debugging data..."));
			}
			sll_remove_debug_data(c_dt);
		}
		if (fl&CLI_FLAG_VERBOSE){
			SLL_LOG(SLL_CHAR("Removing node padding..."));
		}
		sll_remove_node_padding(c_dt,c_dt->h);
		if (fl&CLI_FLAG_VERBOSE){
			SLL_LOG(SLL_CHAR("Optimizing node metadata..."));
		}
		sll_optimize_metadata(c_dt);
		if (fl&CLI_FLAG_PRINT_OBJECT){
			sll_print_node(c_dt,&i_ft,NULL,sll_stdout);
			sll_file_write_char(sll_stdout,'\n');
		}
	}
	if (!(fl&CLI_FLAG_ASSEMBLY_GENERATED)&&((fl&(CLI_FLAG_GENERATE_ASSEMBLY|CLI_FLAG_PRINT_ASSEMBLY))||!(fl&CLI_FLAG_NO_RUN))){
		if (fl&CLI_FLAG_VERBOSE){
			SLL_LOG(SLL_CHAR("Generating assembly..."));
		}
		sll_generate_assembly(c_dt,a_dt);
	}
	if (fl&CLI_FLAG_PRINT_ASSEMBLY){
		sll_print_assembly(a_dt,sll_stdout);
		sll_file_write_char(sll_stdout,'\n');
	}
	if (f_fp&&(fl&(CLI_FLAG_GENERATE_ASSEMBLY|CLI_FLAG_GENERATE_COMPILED_OBJECT|CLI_FLAG_GENERATE_SLL))){
		sll_char_t bf[SLL_API_MAX_FILE_PATH_LENGTH];
		sll_string_length_t i=0;
		sll_string_length_t f_fp_l=sll_string_length_unaligned(f_fp);
		if (!o_fp){
			sll_copy_data(f_fp,f_fp_l,bf);
			i=f_fp_l-1;
		}
		else{
			if (fl&CLI_FLAG_SINGLE_OUTPUT){
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
		if (fl&CLI_FLAG_GENERATE_ASSEMBLY){
			SLL_COPY_STRING_NULL(SLL_CHAR(".sla"),bf+i);
			if (fl&CLI_FLAG_VERBOSE){
				SLL_LOG(SLL_CHAR("Writing assembly to file '%s'..."),bf);
			}
			sll_file_t of;
			if (!sll_file_open(bf,SLL_FILE_FLAG_WRITE,&of)){
				SLL_WARN(SLL_CHAR("Unable to open output file '%s'"),bf);
			}
			else{
				sll_write_assembly(&of,a_dt);
				if (fl&CLI_FLAG_VERBOSE){
					SLL_LOG(SLL_CHAR("File written successfully."));
				}
				sll_file_close(&of);
			}
		}
		if (fl&CLI_FLAG_GENERATE_COMPILED_OBJECT){
			SLL_COPY_STRING_NULL(SLL_CHAR(".slc"),bf+i);
			if (fl&CLI_FLAG_VERBOSE){
				SLL_LOG(SLL_CHAR("Writing compiled program to file '%s'..."),bf);
			}
			sll_file_t of;
			if (!sll_file_open(bf,SLL_FILE_FLAG_WRITE,&of)){
				SLL_WARN(SLL_CHAR("Unable to open output file '%s'"),bf);
			}
			else{
				sll_write_compiled_node(&of,c_dt);
				if (fl&CLI_FLAG_VERBOSE){
					SLL_LOG(SLL_CHAR("File written successfully."));
				}
				sll_file_close(&of);
			}
		}
		if (fl&CLI_FLAG_GENERATE_SLL){
			SLL_COPY_STRING_NULL(SLL_CHAR(".sll"),bf+i);
			if (fl&CLI_FLAG_VERBOSE){
				SLL_LOG(SLL_CHAR("Writing sll code to file '%s'..."),bf);
			}
			sll_file_t of;
			if (!sll_file_open(bf,SLL_FILE_FLAG_WRITE,&of)){
				SLL_WARN(SLL_CHAR("Unable to open output file '%s'"),bf);
			}
			else{
				sll_write_sll_code(c_dt,&i_ft,1,&of);
				if (fl&CLI_FLAG_VERBOSE){
					SLL_LOG(SLL_CHAR("File written successfully."));
				}
				sll_file_close(&of);
			}
		}
	}
	if (!(fl&CLI_FLAG_NO_RUN)){
		sll_vm_config_t cfg={
			CLI_VM_STACK_SIZE,
			CLI_VM_CALL_STACK_SIZE,
			&i_ft,
			sll_stdin,
			sll_stdout,
			sll_stderr
		};
		*ec=sll_execute_assembly(a_dt,&cfg);
		sll_file_flush(sll_stdout);
		sll_file_flush(sll_stderr);
	}
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_return_code_t sll_cli_main(sll_array_length_t argc,const sll_char_t*const* argv){
	sll_init();
	sll_return_code_t ec=1;
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
	sll_array_length_t* fp=NULL;
	sll_string_length_t fpl=0;
	sll_array_length_t* sl=NULL;
	sll_string_length_t sll=0;
	sll_create_internal_function_table(&i_ft);
	sll_register_builtin_internal_functions(&i_ft);
	const sll_char_t* o_fp=NULL;
	sll_file_t f={0};
	sll_assembly_data_t a_dt={0};
	sll_compilation_data_t c_dt={0};
	sll_sandbox_flags_t s_fl=0;
	sll_set_argument_count(1);
	for (sll_array_length_t i=1;i<argc;i++){
		const sll_char_t* e=SLL_CHAR(argv[i]);
		if ((*e=='-'&&*(e+1)=='a'&&*(e+2)==0)||sll_string_compare_pointer(e,SLL_CHAR("--generate-assembly"))==SLL_COMPARE_RESULT_EQUAL){
			fl|=CLI_FLAG_GENERATE_ASSEMBLY;
		}
		else if ((*e=='-'&&*(e+1)=='A'&&*(e+2)==0)||sll_string_compare_pointer(e,SLL_CHAR("--args"))==SLL_COMPARE_RESULT_EQUAL){
			sll_set_argument_count(argc-i);
			for (sll_array_length_t j=0;j<argc-i-1;j++){
				sll_set_argument(j+1,SLL_CHAR(*(argv+i+j+1)));
			}
			break;
		}
		else if ((*e=='-'&&*(e+1)=='c'&&*(e+2)==0)||sll_string_compare_pointer(e,SLL_CHAR("--generate-compiled-object"))==SLL_COMPARE_RESULT_EQUAL){
			fl|=CLI_FLAG_GENERATE_COMPILED_OBJECT;
		}
		else if ((*e=='-'&&*(e+1)=='D'&&*(e+2)==0)||sll_string_compare_pointer(e,SLL_CHAR("--string-debug"))==SLL_COMPARE_RESULT_EQUAL){
			fl|=CLI_FLAG_STRIP_DEBUG;
		}
		else if ((*e=='-'&&*(e+1)=='e'&&*(e+2)==0)||sll_string_compare_pointer(e,SLL_CHAR("--expand-file-paths"))==SLL_COMPARE_RESULT_EQUAL){
			fl|=CLI_FLAG_EXPAND_PATH;
		}
		else if ((*e=='-'&&*(e+1)=='f'&&*(e+2)==0)||sll_string_compare_pointer(e,SLL_CHAR("--file"))==SLL_COMPARE_RESULT_EQUAL){
			i++;
			if (i==argc){
				break;
			}
			goto _read_file_argument;
		}
		else if ((*e=='-'&&*(e+1)=='F'&&*(e+2)==0)||sll_string_compare_pointer(e,SLL_CHAR("--generate-sll"))==SLL_COMPARE_RESULT_EQUAL){
			fl|=CLI_FLAG_GENERATE_SLL;
		}
		else if ((*e=='-'&&*(e+1)=='h'&&*(e+2)==0)||sll_string_compare_pointer(e,SLL_CHAR("--help"))==SLL_COMPARE_RESULT_EQUAL){
			fl|=CLI_FLAG_HELP;
		}
		else if ((*e=='-'&&*(e+1)=='I'&&*(e+2)==0)||sll_string_compare_pointer(e,SLL_CHAR("--include"))==SLL_COMPARE_RESULT_EQUAL){
			i++;
			if (i==argc){
				break;
			}
			e=SLL_CHAR(argv[i]);
			sll_string_length_t sz=sll_string_length_unaligned(e);
			if (sz){
				sll_string_length_t j=i_fpl;
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
			i++;
			if (i==argc){
				break;
			}
			o_fp=(const sll_char_t*)(argv[i]);
		}
		else if ((*e=='-'&&*(e+1)=='O'&&*(e+2)==0)||sll_string_compare_pointer(e,SLL_CHAR("--optimize"))==SLL_COMPARE_RESULT_EQUAL){
			fl|=CLI_FLAG_OPTIMIZE;
		}
		else if ((*e=='-'&&*(e+1)=='p'&&*(e+2)==0)||sll_string_compare_pointer(e,SLL_CHAR("--print-objects"))==SLL_COMPARE_RESULT_EQUAL){
			fl|=CLI_FLAG_PRINT_OBJECT;
		}
		else if ((*e=='-'&&*(e+1)=='P'&&*(e+2)==0)||sll_string_compare_pointer(e,SLL_CHAR("--print-assembly"))==SLL_COMPARE_RESULT_EQUAL){
			fl|=CLI_FLAG_PRINT_ASSEMBLY;
		}
		else if ((*e=='-'&&*(e+1)=='R'&&*(e+2)==0)||sll_string_compare_pointer(e,SLL_CHAR("--no-run"))==SLL_COMPARE_RESULT_EQUAL){
			fl|=CLI_FLAG_NO_RUN;
		}
		else if ((*e=='-'&&*(e+1)=='s'&&*(e+2)==0)||sll_string_compare_pointer(e,SLL_CHAR("--source"))==SLL_COMPARE_RESULT_EQUAL){
			i++;
			if (i==argc){
				break;
			}
			sll++;
			sl=sll_reallocate(sl,sll*sizeof(sll_array_length_t));
			*(sl+sll-1)=i;
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
				SLL_WARN(SLL_CHAR("Ignoring unknown Sandbox Flag '%s'"),e);
			}
		}
		else if ((*e=='-'&&*(e+1)=='v'&&*(e+2)==0)||sll_string_compare_pointer(e,SLL_CHAR("--verbose"))==SLL_COMPARE_RESULT_EQUAL){
			fl|=CLI_FLAG_VERBOSE;
		}
		else if ((*e=='-'&&*(e+1)=='V'&&*(e+2)==0)||sll_string_compare_pointer(e,SLL_CHAR("--version"))==SLL_COMPARE_RESULT_EQUAL){
			fl|=CLI_FLAG_VERSION;
		}
		else if (*e=='-'){
			SLL_WARN(SLL_CHAR("Ignroing unknown Switch '%s'"),e);
		}
		else{
_read_file_argument:
			fpl++;
			fp=sll_reallocate(fp,fpl*sizeof(sll_array_length_t));
			*(fp+fpl-1)=i;
		}
	}
	if (fl&CLI_FLAG_VERBOSE){
		sll_set_log_default(SLL_LOG_FLAG_SHOW,1);
		sll_set_log_file(SLL_CHAR(__FILE__),SLL_LOG_FLAG_NO_HEADER,1);
	}
	if (fl&CLI_FLAG_VERSION){
		SLL_LOG(SLL_CHAR("sll "SLL_VERSION_STRING" ("CLI_BUILD_TYPE_STRING", "SLL_VERSION_BUILD_DATE", "SLL_VERSION_BUILD_TIME")"));
		sll_deinit();
		return 0;
	}
	if (fl&CLI_FLAG_HELP){
		sll_file_write(sll_stdout,HELP_TEXT,HELP_TEXT_SIZE);
		goto _error;
	}
	sll_set_sandbox_flags(s_fl);
	if (fl&CLI_FLAG_VERBOSE){
		SLL_LOG(SLL_CHAR("Configuration:"));
		if (fl&CLI_FLAG_EXPAND_PATH){
			SLL_LOG(SLL_CHAR("  Path expansion"));
		}
		if (fl&CLI_FLAG_GENERATE_ASSEMBLY){
			SLL_LOG(SLL_CHAR("  Assembly generation"));
		}
		if (fl&CLI_FLAG_GENERATE_COMPILED_OBJECT){
			SLL_LOG(SLL_CHAR("  Compiled program generation"));
		}
		if (fl&CLI_FLAG_GENERATE_SLL){
			SLL_LOG(SLL_CHAR("  Sll code generation mode"));
		}
		if (!(fl&CLI_FLAG_NO_RUN)){
			SLL_LOG(SLL_CHAR("  Execution"));
		}
		if (fl&CLI_FLAG_OPTIMIZE){
			SLL_LOG(SLL_CHAR("  Optimization"));
		}
		if (fl&CLI_FLAG_PRINT_ASSEMBLY){
			SLL_LOG(SLL_CHAR("  Assembly printing"));
		}
		if (fl&CLI_FLAG_PRINT_OBJECT){
			SLL_LOG(SLL_CHAR("  Compiled program printing"));
		}
		if (fl&CLI_FLAG_STRIP_DEBUG){
			SLL_LOG(SLL_CHAR("  Debug data stripping"));
		}
		SLL_LOG(SLL_CHAR("Include path:"));
		sll_string_length_t i=0;
		while (i<i_fpl){
			SLL_LOG(SLL_CHAR("  '%s'"),i_fp+i);
			i+=sll_string_length_unaligned(i_fp+i)+1;
		}
		SLL_LOG(SLL_CHAR("Library path: '%s'"),l_fp);
	}
	if (fl&CLI_FLAG_OPTIMIZE){
		SLL_WARN(SLL_CHAR("WARNING: Optimizer is currently an alpha feature!"));
	}
	if (fpl+sll==1){
		fl|=CLI_FLAG_SINGLE_OUTPUT;
	}
	for (sll_string_length_t j=0;j<fpl;j++){
		sll_char_t f_fp[SLL_API_MAX_FILE_PATH_LENGTH];
		fl&=~CLI_FLAG_ASSEMBLY_GENERATED;
		_load_file(SLL_CHAR(argv[*(fp+j)]),&a_dt,&c_dt,&f,f_fp);
		sll_char_t bf[SLL_API_MAX_FILE_PATH_LENGTH];
		sll_platform_absolute_path(SLL_CHAR(argv[*(fp+j)]),bf,SLL_API_MAX_FILE_PATH_LENGTH);
		sll_set_argument(0,bf);
		_execute(f_fp,&c_dt,&a_dt,&f,o_fp,&ec);
		if (f.f){
			sll_file_close(&f);
		}
		sll_free_assembly_data(&a_dt);
		sll_free_compilation_data(&c_dt);
	}
	for (sll_string_length_t j=0;j<sll;j++){
		sll_set_argument(0,SLL_CHAR("<console>"));
		sll_file_from_data(SLL_CHAR(argv[*(fp+j)]),sll_string_length_unaligned(SLL_CHAR(argv[*(fp+j)])),SLL_FILE_FLAG_READ,&f);
		if (fl&CLI_FLAG_VERBOSE){
			SLL_LOG(SLL_CHAR("Compiling console input..."));
		}
		sll_init_compilation_data(SLL_CHAR("@console"),&f,&c_dt);
		sll_parse_all_nodes(&c_dt,&i_ft,_import_file);
		if (fl&CLI_FLAG_PRINT_OBJECT){
			sll_print_node(&c_dt,&i_ft,NULL,sll_stdout);
			sll_file_write_char(sll_stdout,'\n');
		}
		if (fl&CLI_FLAG_VERBOSE){
			SLL_LOG(SLL_CHAR("Input successfully read."));
		}
		_execute(NULL,&c_dt,&a_dt,&f,o_fp,&ec);
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
