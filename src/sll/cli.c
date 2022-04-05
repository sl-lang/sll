#include <sll/_internal/cli.h>
#include <sll/_internal/common.h>
#include <sll/_internal/static_string.h>
#include <sll/abi.h>
#include <sll/api/date.h>
#include <sll/api/math.h>
#include <sll/api/path.h>
#include <sll/api/sys.h>
#include <sll/assembly.h>
#include <sll/audit.h>
#include <sll/bundle.h>
#include <sll/data.h>
#include <sll/error.h>
#include <sll/file.h>
#include <sll/generated/help_text.h>
#include <sll/ift.h>
#include <sll/init.h>
#include <sll/io.h>
#include <sll/log.h>
#include <sll/memory.h>
#include <sll/node.h>
#include <sll/platform/library.h>
#include <sll/platform/path.h>
#include <sll/platform/util.h>
#include <sll/sandbox.h>
#include <sll/string.h>
#include <sll/types.h>
#include <sll/version.h>
#include <sll/vm.h>



static __STATIC_STRING(slc_end,".slc");
static unsigned int fl;
static sll_char_t* i_fp;
static sll_string_length_t i_fpl;
static cli_bundle_source_t** i_b;
static sll_array_length_t i_bl;
static sll_char_t l_fp[SLL_API_MAX_FILE_PATH_LENGTH];
static sll_string_length_t l_fpl;
static sll_internal_function_table_t i_ft;



static void _load_file(const sll_char_t* f_nm,sll_assembly_data_t* a_dt,sll_compilation_data_t* c_dt,sll_char_t* f_fp);



static sll_bool_t _import_file(const sll_string_t* nm,sll_compilation_data_t* o){
	sll_assembly_data_t a_dt=SLL_INIT_ASSEMBLY_DATA_STRUCT;
	sll_char_t f_fp[SLL_API_MAX_FILE_PATH_LENGTH];
	_load_file(nm->v,&a_dt,o,f_fp);
	if (fl&CLI_FLAG_ASSEMBLY_GENERATED){
		sll_free_assembly_data(&a_dt);
		SLL_WARN(SLL_CHAR("Importing assembly into compiled programs is not allowed"));
		return 0;
	}
	return 1;
}



static void _load_file(const sll_char_t* f_nm,sll_assembly_data_t* a_dt,sll_compilation_data_t* c_dt,sll_char_t* f_fp){
	sll_string_length_t f_nm_l=sll_string_length_unaligned(f_nm);
	sll_char_t bf[SLL_API_MAX_FILE_PATH_LENGTH];
	if (i_bl){
		sll_string_t f_nm_str;
		sll_string_from_pointer_length(f_nm,f_nm_l,&f_nm_str);
		sll_array_length_t i=i_bl;
		do{
			i--;
			cli_bundle_source_t* b_dt=*(i_b+i);
			CLI_LOG_IF_VERBOSE("Trying to open file '%s/%s'...",b_dt->nm,f_nm);
			if (sll_bundle_fetch(&(b_dt->b),&f_nm_str,c_dt)){
				CLI_LOG_IF_VERBOSE("File successfully read.");
				sll_free_string(&f_nm_str);
				return;
			}
		} while(i);
		sll_free_string(&f_nm_str);
	}
	sll_string_length_t i=0;
	while (i<i_fpl){
		sll_string_length_t j=sll_string_length_unaligned(i_fp+i);
		sll_copy_data(i_fp+i,j,bf);
		i+=j+1;
		sll_copy_data(f_nm,f_nm_l,bf+j);
		j+=f_nm_l;
		SLL_COPY_STRING_NULL(SLL_CHAR(".slc"),bf+j);
		CLI_LOG_IF_VERBOSE("Trying to open file '%s'...",bf);
		sll_file_t f;
		if (sll_platform_path_exists(bf)){
			sll_file_open(bf,SLL_FILE_FLAG_READ,&f);
			CLI_EXPAND_PATH(bf,f_fp);
			CLI_LOG_IF_VERBOSE("Found file '%s'",f_fp);
			if (sll_load_compiled_node(&f,c_dt)){
				sll_file_close(&f);
				CLI_LOG_IF_VERBOSE("File successfully read.");
				return;
			}
			sll_free_compilation_data(c_dt);
			sll_file_close(&f);
			CLI_LOG_IF_VERBOSE("File is not a compiled program");
		}
		bf[j]=0;
		CLI_LOG_IF_VERBOSE("Trying to open file '%s'...",bf);
		if (!sll_platform_path_exists(bf)){
			continue;
		}
		sll_file_open(bf,SLL_FILE_FLAG_READ,&f);
		CLI_EXPAND_PATH(bf,f_fp);
		CLI_LOG_IF_VERBOSE("Found file '%s'",f_fp);
		if (sll_load_assembly(&f,a_dt)){
			sll_file_close(&f);
			fl|=CLI_FLAG_ASSEMBLY_GENERATED;
			CLI_LOG_IF_VERBOSE("File successfully read.");
			return;
		}
		sll_free_assembly_data(a_dt);
		sll_file_reset(&f);
		if (sll_load_compiled_node(&f,c_dt)){
			sll_file_close(&f);
			CLI_LOG_IF_VERBOSE("File successfully read.");
			return;
		}
		sll_free_compilation_data(c_dt);
		sll_file_reset(&f);
		sll_init_compilation_data(f_fp,c_dt);
		sll_parse_nodes(&f,c_dt,&i_ft,_import_file);
		sll_file_close(&f);
		CLI_LOG_IF_VERBOSE("File successfully read.");
		return;
	}
	if (l_fpl){
		sll_copy_data(f_nm,f_nm_l,l_fp+l_fpl);
		SLL_COPY_STRING_NULL(SLL_CHAR(".slc"),l_fp+l_fpl+f_nm_l);
		CLI_LOG_IF_VERBOSE("Trying to open file '%s'...",l_fp);
		sll_file_t f;
		if (sll_platform_path_exists(l_fp)){
			sll_file_open(l_fp,SLL_FILE_FLAG_READ,&f);
			CLI_EXPAND_PATH(l_fp,f_fp);
			CLI_LOG_IF_VERBOSE("Found file '%s'",f_fp);
			if (sll_load_compiled_node(&f,c_dt)){
				sll_file_close(&f);
				CLI_LOG_IF_VERBOSE("File successfully read.");
				return;
			}
			sll_free_compilation_data(c_dt);
			sll_file_close(&f);
			CLI_LOG_IF_VERBOSE("File is not a compiled program");
		}
	}
	SLL_WARN("Unable to find file '%s'",f_nm);
	sll_init_compilation_data(f_nm,c_dt);
}



static void _load_bundle(const sll_char_t* nm,sll_file_t* rf){
	sll_bundle_t b_dt;
	if (!sll_load_bundle(rf,&b_dt)){
		return;
	}
	i_bl++;
	i_b=sll_reallocate(i_b,i_bl*sizeof(cli_bundle_source_t*));
	cli_bundle_source_t* b=sll_allocate(sizeof(cli_bundle_source_t));
	sll_string_length_t nml=sll_string_length_unaligned(nm)+1;
	b->nm=sll_allocate(nml);
	sll_copy_data(nm,nml,b->nm);
	b->b=b_dt;
	*(i_b+i_bl-1)=b;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_return_code_t sll_cli_main(sll_array_length_t argc,const sll_char_t*const* argv){
	if (!argc){
		return 0;
	}
	sll_init();
	sll_return_code_t ec=0;
	fl=0;
	i_b=NULL;
	i_bl=0;
	i_fp=sll_allocate(sizeof(sll_char_t));
	*i_fp=0;
	i_fpl=1;
	SLL_ASSERT(sll_library_file_path->l<SLL_API_MAX_FILE_PATH_LENGTH+5);
	l_fpl=sll_library_file_path->l;
	while (sll_library_file_path->v[l_fpl]!='/'&&sll_library_file_path->v[l_fpl]!='\\'){
		if (!l_fpl){
			break;
		}
		l_fpl--;
	}
	l_fpl++;
	sll_copy_data(sll_library_file_path->v,l_fpl,l_fp);
	SLL_COPY_STRING_NULL(SLL_CHAR("lib/"),l_fp+l_fpl);
	l_fpl+=4;
	sll_string_t* b_fl=NULL;
	sll_array_length_t b_fll=sll_platform_list_directory(l_fp,&b_fl);
	while (b_fll){
		b_fll--;
		sll_string_t* str=b_fl+b_fll;
		sll_copy_data(str->v,str->l+1,l_fp+l_fpl);
		sll_free_string(str);
		if (!sll_platform_path_is_directory(l_fp)){
			sll_file_t b_f;
			if (sll_file_open(l_fp,SLL_FILE_FLAG_READ,&b_f)==SLL_NO_ERROR){
				_load_bundle(l_fp,&b_f);
				sll_file_close(&b_f);
			}
		}
	}
	l_fp[l_fpl]=0;
	sll_deallocate(b_fl);
	sll_array_length_t* fp=NULL;
	sll_string_length_t fpl=0;
	sll_array_length_t* sl=NULL;
	sll_string_length_t sll=0;
	sll_library_handle_t* ll=NULL;
	sll_array_length_t lll=0;
	sll_create_internal_function_table(&i_ft);
	sll_register_builtin_internal_functions(&i_ft);
	const sll_char_t* b_nm=NULL;
	const sll_char_t* b_o_fp=NULL;
	const sll_char_t* o_fp=NULL;
	sll_set_argument_count(1);
	sll_array_length_t i=0;
	do{
		const sll_char_t* e=argv[i];
		sll_char_t nm=(*e=='-'&&*(e+1)&&!*(e+2)?*(e+1):0);
		if (nm=='a'||sll_string_compare_pointer(e,SLL_CHAR("--generate-assembly"))==SLL_COMPARE_RESULT_EQUAL){
			fl|=CLI_FLAG_GENERATE_ASSEMBLY;
		}
		else if (nm=='A'||sll_string_compare_pointer(e,SLL_CHAR("--args"))==SLL_COMPARE_RESULT_EQUAL){
			sll_set_argument_count(argc-i);
			for (sll_array_length_t j=0;j<argc-i-1;j++){
				sll_set_argument(j+1,argv[i+j+1]);
			}
			break;
		}
		else if (nm=='b'||sll_string_compare_pointer(e,SLL_CHAR("--bundle"))==SLL_COMPARE_RESULT_EQUAL){
			fl|=CLI_FLAG_GENERATE_BUNDLE;
		}
		else if (nm=='c'||sll_string_compare_pointer(e,SLL_CHAR("--generate-compiled-object"))==SLL_COMPARE_RESULT_EQUAL){
			fl|=CLI_FLAG_GENERATE_COMPILED_OBJECT;
		}
		else if (nm=='d'||sll_string_compare_pointer(e,SLL_CHAR("--strip-names"))==SLL_COMPARE_RESULT_EQUAL){
			fl|=CLI_FLAG_STRIP_NAMES;
		}
		else if (nm=='D'||sll_string_compare_pointer(e,SLL_CHAR("--strip-debug"))==SLL_COMPARE_RESULT_EQUAL){
			fl|=CLI_FLAG_STRIP_DEBUG;
		}
		else if (nm=='e'||sll_string_compare_pointer(e,SLL_CHAR("--expand-file-paths"))==SLL_COMPARE_RESULT_EQUAL){
			fl|=CLI_FLAG_EXPAND_PATH;
		}
		else if (nm=='f'||sll_string_compare_pointer(e,SLL_CHAR("--file"))==SLL_COMPARE_RESULT_EQUAL){
			i++;
			if (i==argc){
				break;
			}
			goto _read_file_argument;
		}
		else if (nm=='h'||sll_string_compare_pointer(e,SLL_CHAR("--help"))==SLL_COMPARE_RESULT_EQUAL){
			fl|=CLI_FLAG_HELP;
		}
		else if (nm=='I'||sll_string_compare_pointer(e,SLL_CHAR("--include"))==SLL_COMPARE_RESULT_EQUAL){
			i++;
			if (i==argc){
				break;
			}
			e=argv[i];
			sll_string_length_t sz=sll_string_length_unaligned(e);
			if (sz){
				if (sll_platform_path_is_directory(e)){
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
				else{
					sll_file_t b_f;
					if (sll_file_open(e,SLL_FILE_FLAG_READ,&b_f)==SLL_NO_ERROR){
						_load_bundle(e,&b_f);
						sll_file_close(&b_f);
					}
				}
			}
		}
		else if (nm=='L'||sll_string_compare_pointer(e,SLL_CHAR("--audit-library"))==SLL_COMPARE_RESULT_EQUAL){
			i++;
			if (i==argc){
				break;
			}
			e=argv[i];
			sll_library_handle_t* lh=sll_platform_load_library(e);
			if (lh!=SLL_UNKNOWN_LIBRARY_HANDLE){
				sll_audit_callback_t cb=sll_platform_lookup_symbol(lh,SLL_ABI_NAME(SLL_ABI_AUDIT_CALL));
				if (!cb){
					sll_platform_unload_library(lh);
				}
				else{
					void (*init)(void)=sll_platform_lookup_symbol(lh,SLL_ABI_NAME(SLL_ABI_AUDIT_INIT));
					if (init){
						init();
					}
					lll++;
					ll=sll_reallocate(ll,lll*sizeof(sll_library_handle_t));
					*(ll+lll-1)=lh;
					sll_audit_register_callback(cb);
				}
			}
		}
		else if (nm=='n'||sll_string_compare_pointer(e,SLL_CHAR("--names-only"))==SLL_COMPARE_RESULT_EQUAL){
			fl|=CLI_FLAG_NO_PATHS;
		}
		else if (nm=='N'||sll_string_compare_pointer(e,SLL_CHAR("--bundle-name"))==SLL_COMPARE_RESULT_EQUAL){
			i++;
			if (i==argc){
				break;
			}
			b_nm=argv[i];
		}
		else if (nm=='o'||sll_string_compare_pointer(e,SLL_CHAR("--output"))==SLL_COMPARE_RESULT_EQUAL){
			i++;
			if (i==argc){
				break;
			}
			o_fp=argv[i];
		}
		else if (nm=='O'||sll_string_compare_pointer(e,SLL_CHAR("--bundle-output"))==SLL_COMPARE_RESULT_EQUAL){
			i++;
			if (i==argc){
				break;
			}
			b_o_fp=argv[i];
		}
		else if (nm=='p'||sll_string_compare_pointer(e,SLL_CHAR("--print-objects"))==SLL_COMPARE_RESULT_EQUAL){
			fl|=CLI_FLAG_PRINT_NODES;
		}
		else if (nm=='P'||sll_string_compare_pointer(e,SLL_CHAR("--print-assembly"))==SLL_COMPARE_RESULT_EQUAL){
			fl|=CLI_FLAG_PRINT_ASSEMBLY;
		}
		else if (nm=='R'||sll_string_compare_pointer(e,SLL_CHAR("--no-run"))==SLL_COMPARE_RESULT_EQUAL){
			fl|=CLI_FLAG_NO_RUN;
		}
		else if (nm=='s'||sll_string_compare_pointer(e,SLL_CHAR("--source"))==SLL_COMPARE_RESULT_EQUAL){
			i++;
			if (i==argc){
				break;
			}
			sll++;
			sl=sll_reallocate(sl,sll*sizeof(sll_array_length_t));
			*(sl+sll-1)=i;
		}
		else if (nm=='S'||sll_string_compare_pointer(e,SLL_CHAR("--sandbox"))==SLL_COMPARE_RESULT_EQUAL){
			i++;
			if (i==argc){
				break;
			}
			e=argv[i];
			if (sll_string_compare_pointer(e,SLL_CHAR("no-file-io"))==SLL_COMPARE_RESULT_EQUAL){
				sll_set_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_FILE_IO);
			}
			else if (sll_string_compare_pointer(e,SLL_CHAR("stdin-io"))==SLL_COMPARE_RESULT_EQUAL){
				sll_set_sandbox_flag(SLL_SANDBOX_FLAG_ENABLE_STDIN_IO);
			}
			else if (sll_string_compare_pointer(e,SLL_CHAR("stdout-io"))==SLL_COMPARE_RESULT_EQUAL){
				sll_set_sandbox_flag(SLL_SANDBOX_FLAG_ENABLE_STDOUT_IO);
			}
			else if (sll_string_compare_pointer(e,SLL_CHAR("no-path-api"))==SLL_COMPARE_RESULT_EQUAL){
				sll_set_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_PATH_API);
			}
			else if (sll_string_compare_pointer(e,SLL_CHAR("no-process-api"))==SLL_COMPARE_RESULT_EQUAL){
				sll_set_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_PROCESS_API);
			}
			else if (sll_string_compare_pointer(e,SLL_CHAR("no-environment"))==SLL_COMPARE_RESULT_EQUAL){
				sll_set_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_ENVIRONMENT);
			}
			else if (sll_string_compare_pointer(e,SLL_CHAR("no-load-library"))==SLL_COMPARE_RESULT_EQUAL){
				sll_set_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_LOAD_LIBRARY);
			}
			else if (sll_string_compare_pointer(e,SLL_CHAR("buffer-files"))==SLL_COMPARE_RESULT_EQUAL){
				sll_set_sandbox_flag(SLL_SANDBOX_FLAG_ENABLE_BUFFER_FILES);
			}
			else if (sll_string_compare_pointer(e,SLL_CHAR("file-rename"))==SLL_COMPARE_RESULT_EQUAL){
				sll_set_sandbox_flag(SLL_SANDBOX_FLAG_ENABLE_FILE_RENAME);
			}
			else if (sll_string_compare_pointer(e,SLL_CHAR("file-copy"))==SLL_COMPARE_RESULT_EQUAL){
				sll_set_sandbox_flag(SLL_SANDBOX_FLAG_ENABLE_FILE_COPY);
			}
			else if (sll_string_compare_pointer(e,SLL_CHAR("no-float-error-change"))==SLL_COMPARE_RESULT_EQUAL){
				sll_set_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_FLOAT_COMPARE_ERROR_CHANGE);
			}
			else if (sll_string_compare_pointer(e,SLL_CHAR("file-delete"))==SLL_COMPARE_RESULT_EQUAL){
				sll_set_sandbox_flag(SLL_SANDBOX_FLAG_ENABLE_FILE_DELETE);
			}
			else if (sll_string_compare_pointer(e,SLL_CHAR("no-thread"))==SLL_COMPARE_RESULT_EQUAL){
				sll_set_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_THREADS);
			}
			else if (sll_string_compare_pointer(e,SLL_CHAR("no-random"))==SLL_COMPARE_RESULT_EQUAL){
				sll_set_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_RANDOM);
			}
			else if (sll_string_compare_pointer(e,SLL_CHAR("no-serial"))==SLL_COMPARE_RESULT_EQUAL){
				sll_set_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_SERIAL);
			}
			else{
				SLL_WARN(SLL_CHAR("Ignoring unknown Sandbox Flag '%s'"),e);
			}
		}
		else if (nm=='v'||sll_string_compare_pointer(e,SLL_CHAR("--verbose"))==SLL_COMPARE_RESULT_EQUAL){
			fl|=CLI_FLAG_VERBOSE;
		}
		else if (nm=='V'||sll_string_compare_pointer(e,SLL_CHAR("--version"))==SLL_COMPARE_RESULT_EQUAL){
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
		i++;
	} while (i<argc);
	if (fl&CLI_FLAG_VERBOSE){
		sll_set_log_default(SLL_LOG_FLAG_SHOW,1);
		sll_set_log_file(SLL_CHAR(__FILE__),SLL_LOG_FLAG_NO_HEADER,1);
	}
	sll_audit(SLL_CHAR("sll.cli.init"),SLL_CHAR("x"),argv,argc);
	if (fl&CLI_FLAG_VERSION){
		sll_date_t d;
		sll_date_from_time_ns(SLL_VERSION_BUILD_TIME,sll_platform_time_zone,&d);
		sll_file_write_format(sll_stdout,SLL_CHAR("sll "SLL_VERSION_STRING" ("CLI_BUILD_TYPE_STRING", %.4u/%.2u/%.2u %.2u:%.2u:%.2u)\n"),NULL,d.y,d.m+1,d.d+1,d.h,d.mn,sll_api_math_floor(d.s));
		goto _cleanup;
	}
	if (fl&CLI_FLAG_HELP){
		sll_file_write(sll_stdout,HELP_TEXT,HELP_TEXT_SIZE,NULL);
		goto _cleanup;
	}
	if (fl&CLI_FLAG_VERBOSE){
		SLL_LOG("Configuration:");
		if (fl&CLI_FLAG_EXPAND_PATH){
			SLL_LOG("  Path expansion");
		}
		if (fl&CLI_FLAG_GENERATE_ASSEMBLY){
			SLL_LOG("  Assembly generation");
		}
		if (fl&CLI_FLAG_GENERATE_BUNDLE){
			SLL_LOG("  Bundle generation");
		}
		if (fl&CLI_FLAG_GENERATE_COMPILED_OBJECT){
			SLL_LOG("  Compiled program generation");
		}
		if (fl&CLI_FLAG_NO_PATHS){
			SLL_LOG("  No bundle file paths");
		}
		if (!(fl&CLI_FLAG_NO_RUN)){
			SLL_LOG("  Execution");
		}
		if (fl&CLI_FLAG_PRINT_ASSEMBLY){
			SLL_LOG("  Assembly printing");
		}
		if (fl&CLI_FLAG_PRINT_NODES){
			SLL_LOG("  Compiled program printing");
		}
		if (fl&CLI_FLAG_STRIP_DEBUG){
			SLL_LOG("  Debug data stripping");
		}
		if (fl&CLI_FLAG_STRIP_NAMES){
			SLL_LOG("  Function name stripping");
		}
		SLL_LOG("Include path:");
		for (sll_array_length_t j=0;j<i_bl;j++){
			SLL_LOG("  '%s' (bundle)",(*(i_b+j))->nm);
		}
		sll_string_length_t j=0;
		while (j<i_fpl){
			SLL_LOG("  '%s'",i_fp+j);
			j+=sll_string_length_unaligned(i_fp+j)+1;
		}
		SLL_LOG("Library path: '%s'",l_fp);
	}
	if (fpl+sll==1){
		fl|=CLI_FLAG_SINGLE_OUTPUT;
	}
	sll_bundle_t bundle;
	if (fl&CLI_FLAG_GENERATE_BUNDLE){
		sll_bundle_create((!b_nm||((b_nm[0]=='\\'||b_nm[0]=='/')&&!b_nm[1])?NULL:b_nm),&bundle);
	}
	for (sll_string_length_t j=0;j<fpl+sll;j++){
		sll_assembly_data_t a_dt=SLL_INIT_ASSEMBLY_DATA_STRUCT;
		sll_compilation_data_t c_dt=SLL_INIT_COMPILATION_DATA_STRUCT;
		sll_char_t f_fp[SLL_API_MAX_FILE_PATH_LENGTH];
		sll_source_file_t* a_dt_sf=NULL;
		if (j<fpl){
			_load_file(argv[*(fp+j)],&a_dt,&c_dt,f_fp);
			sll_char_t bf[SLL_API_MAX_FILE_PATH_LENGTH];
			CLI_EXPAND_PATH(argv[*(fp+j)],bf);
			sll_set_argument(0,bf);
		}
		else{
			CLI_LOG_IF_VERBOSE("Compiling console input...");
			sll_init_compilation_data(SLL_CHAR("@console"),&c_dt);
			sll_file_t f;
			sll_file_from_data(PTR(argv[*(sl+j-fpl)]),sll_string_length_unaligned(argv[*(sl+j-fpl)]),SLL_FILE_FLAG_READ,&f);
			sll_parse_nodes(&f,&c_dt,&i_ft,_import_file);
			sll_file_close(&f);
			CLI_LOG_IF_VERBOSE("Input successfully read.");
			sll_set_argument(0,SLL_CHAR("@console"));
		}
		if (!(fl&CLI_FLAG_ASSEMBLY_GENERATED)){
			if ((fl&(CLI_FLAG_GENERATE_ASSEMBLY|CLI_FLAG_PRINT_ASSEMBLY))||!(fl&CLI_FLAG_NO_RUN)){
				CLI_LOG_IF_VERBOSE("Combining source files...");
				a_dt_sf=sll_allocate(sizeof(sll_source_file_t));
				sll_unify_compilation_data(&c_dt,a_dt_sf);
				sll_free_compilation_data(&c_dt);
				c_dt.dt=sll_allocate(sizeof(sll_source_file_t*));
				*(c_dt.dt)=a_dt_sf;
				c_dt.l=1;
			}
			if (fl&CLI_FLAG_STRIP_DEBUG){
				CLI_LOG_IF_VERBOSE("Removing debugging data...");
				sll_remove_debug_data(&c_dt);
			}
			if (fl&CLI_FLAG_STRIP_NAMES){
				CLI_LOG_IF_VERBOSE("Removing name data...");
				sll_remove_debug_names(&c_dt);
			}
			CLI_LOG_IF_VERBOSE("Removing node padding...");
			sll_remove_node_padding(&c_dt);
			CLI_LOG_IF_VERBOSE("Optimizing node metadata...");
			sll_optimize_metadata(&c_dt);
			if (fl&CLI_FLAG_PRINT_NODES){
				sll_print_node(*(c_dt.dt),&i_ft,NULL,sll_stdout);
				sll_file_write_char(sll_stdout,'\n',NULL);
			}
			if ((fl&(CLI_FLAG_GENERATE_ASSEMBLY|CLI_FLAG_PRINT_ASSEMBLY))||!(fl&CLI_FLAG_NO_RUN)){
				CLI_LOG_IF_VERBOSE("Generating assembly...");
				sll_generate_assembly(a_dt_sf,&a_dt);
			}
		}
		if (fl&CLI_FLAG_PRINT_ASSEMBLY){
			sll_print_assembly(&a_dt,sll_stdout);
			sll_file_write_char(sll_stdout,'\n',NULL);
		}
		if (j<fpl&&(fl&(CLI_FLAG_GENERATE_ASSEMBLY|CLI_FLAG_GENERATE_COMPILED_OBJECT))){
			sll_char_t bf[SLL_API_MAX_FILE_PATH_LENGTH];
			sll_string_length_t k=0;
			sll_string_length_t f_fp_l=sll_string_length_unaligned(f_fp);
			if (!o_fp){
				sll_copy_data(f_fp,f_fp_l,bf);
				k=f_fp_l-1;
			}
			else{
				if (fl&CLI_FLAG_SINGLE_OUTPUT){
					k=sll_string_length_unaligned(o_fp);
					sll_copy_data(o_fp,k,bf);
					k-=1;
				}
				else{
					k=sll_string_length_unaligned(o_fp);
					while (*(o_fp+k)!='\\'&&*(o_fp+k)!='/'){
						if (!k){
							k--;
							break;
						}
						k--;
					}
					k++;
					sll_string_length_t l=f_fp_l;
					while (l&&*(f_fp+l-1)!='\\'&&*(f_fp+l-1)!='/'){
						l--;
					}
					sll_copy_data(f_fp+l,f_fp_l-l,bf+k);
					k+=f_fp_l-l-1;
				}
			}
			bf[k+1]='.';
			bf[k+2]='s';
			bf[k+3]='l';
			bf[k+5]=0;
			k+=4;
			if (fl&CLI_FLAG_GENERATE_ASSEMBLY){
				bf[k]='a';
				CLI_LOG_IF_VERBOSE("Writing assembly to file '%s'...",bf);
				sll_file_t of;
				sll_file_open(bf,SLL_FILE_FLAG_WRITE,&of);
				sll_write_assembly(&of,&a_dt);
				CLI_LOG_IF_VERBOSE("File written successfully.");
				sll_file_close(&of);
			}
			if (fl&CLI_FLAG_GENERATE_COMPILED_OBJECT){
				bf[k]='c';
				CLI_LOG_IF_VERBOSE("Writing compiled program to file '%s'...",bf);
				sll_file_t of;
				sll_file_open(bf,SLL_FILE_FLAG_WRITE,&of);
				sll_write_compiled_node(&of,&c_dt);
				CLI_LOG_IF_VERBOSE("File written successfully.");
				sll_file_close(&of);
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
			sll_file_flush(sll_stdout);
			sll_file_flush(sll_stderr);
			ec=sll_execute_assembly(&a_dt,&cfg);
			sll_file_flush(sll_stdout);
			sll_file_flush(sll_stderr);
		}
		sll_free_assembly_data(&a_dt);
		if (j<fpl&&(fl&(CLI_FLAG_ASSEMBLY_GENERATED|CLI_FLAG_GENERATE_BUNDLE))==CLI_FLAG_GENERATE_BUNDLE){
			sll_string_t b_f_nm;
			sll_string_from_pointer(argv[*(fp+j)],&b_f_nm);
			sll_string_length_t off=((fl&CLI_FLAG_NO_PATHS)?sll_path_split(&b_f_nm):0);
			if (sll_string_ends(&b_f_nm,&slc_end)){
				sll_string_set_char(0,b_f_nm.l-slc_end.l,&b_f_nm);
			}
			CLI_LOG_IF_VERBOSE("Adding file '%s' as '%s' to bundle...",f_fp,b_f_nm.v+off);
			sll_bundle_add_file(b_f_nm.v+off,&c_dt,&bundle);
			sll_free_string(&b_f_nm);
		}
		else{
			sll_free_compilation_data(&c_dt);
		}
		fl&=~CLI_FLAG_ASSEMBLY_GENERATED;
	}
	if (fl&CLI_FLAG_GENERATE_BUNDLE){
		sll_char_t bf[SLL_API_MAX_FILE_PATH_LENGTH];
		if (b_o_fp){
			sll_string_length_t l=sll_string_length_unaligned(b_o_fp);
			if (l>SLL_API_MAX_FILE_PATH_LENGTH-1){
				l=SLL_API_MAX_FILE_PATH_LENGTH-1;
			}
			sll_copy_data(b_o_fp,l,bf);
			bf[l]=0;
		}
		else{
			sll_string_length_t l=bundle.nm.l;
			if (l>SLL_API_MAX_FILE_PATH_LENGTH-1){
				l=SLL_API_MAX_FILE_PATH_LENGTH-1;
			}
			sll_copy_data(bundle.nm.v,l,bf);
			bf[l]=0;
		}
		if (!bf[0]){
			SLL_WARN(SLL_CHAR("No bundle output path supplied"));
			sll_free_bundle(&bundle);
			goto _cleanup;
		}
		CLI_LOG_IF_VERBOSE("Writing bundle to '%s'...",bf);
		sll_file_t of;
		sll_file_open(bf,SLL_FILE_FLAG_WRITE,&of);
		sll_write_bundle(&of,&bundle);
		CLI_LOG_IF_VERBOSE("File written successfully.");
		sll_file_close(&of);
		sll_free_bundle(&bundle);
	}
_cleanup:
	sll_deallocate(i_fp);
	while (i_bl){
		i_bl--;
		cli_bundle_source_t* b=*(i_b+i_bl);
		sll_deallocate(b->nm);
		sll_free_bundle(&(b->b));
		sll_deallocate(b);
	}
	while (lll){
		lll--;
		void (*deinit)(void)=sll_platform_lookup_symbol(*(ll+lll),SLL_ABI_NAME(SLL_ABI_AUDIT_DEINIT));
		if (deinit){
			deinit();
		}
		sll_platform_unload_library(*(ll+lll));
	}
	sll_deallocate(i_b);
	sll_deallocate(fp);
	sll_deallocate(sl);
	sll_deallocate(ll);
	sll_free_internal_function_table(&i_ft);
	sll_deinit();
	return ec;
}
