#include <sll/_internal/cli.h>
#include <sll/_internal/common.h>
#include <sll/_internal/static_string.h>
#include <sll/_size_types.h>
#include <sll/abi.h>
#include <sll/api/date.h>
#include <sll/api/path.h>
#include <sll/api/sys.h>
#include <sll/assembly.h>
#include <sll/audit.h>
#include <sll/bundle.h>
#include <sll/cli.h>
#include <sll/common.h>
#include <sll/data.h>
#include <sll/error.h>
#include <sll/file.h>
#include <sll/generated/help_text.h>
#include <sll/ift.h>
#include <sll/init.h>
#include <sll/io.h>
#include <sll/log.h>
#include <sll/memory.h>
#include <sll/new_object.h>
#include <sll/node.h>
#include <sll/optimizer.h>
#include <sll/platform/library.h>
#include <sll/platform/path.h>
#include <sll/platform/process.h>
#include <sll/platform/time.h>
#include <sll/sandbox.h>
#include <sll/string.h>
#include <sll/types.h>
#include <sll/version.h>
#include <sll/vm.h>
#include <math.h>



static __STATIC_STRING(_cli_slc_suffix,".slc");
static __SLL_U32 _cli_flags;
static cli_include_dir_t** _cli_include_list;
static sll_string_length_t _cli_include_list_len;
static cli_bundle_source_t** _cli_bundle_list;
static sll_array_length_t _cli_bundle_list_len;
static sll_char_t _cli_lib_path[SLL_API_MAX_FILE_PATH_LENGTH];
static sll_string_length_t _cli_lib_path_len;
static sll_internal_function_table_t _cli_ift;
static sll_bool_t _cli_enable_file_lookup=0;
static sll_cli_path_resolver_t _cli_resolver_table[CLI_PATH_RESOLVER_TABLE_SIZE];
static sll_array_length_t _cli_resolver_table_size=0;
static sll_optimization_round_count_t _cli_optimization_count;
static sll_bool_t _cli_had_warning;



static sll_bool_t _import_file(const sll_string_t* path,sll_compilation_data_t* out){
	sll_cli_lookup_data_t res_data;
	sll_cli_lookup_result_t res=sll_cli_lookup_file(path,1,&res_data);
	if (res==SLL_LOOKUP_RESULT_ASSEMBLY){
		sll_free_assembly_data(&(res_data.data.assembly_data));
		SLL_WARN(SLL_CHAR("Importing assembly into compiled programs is not allowed"));
		_cli_had_warning=1;
		return 0;
	}
	if (res==SLL_LOOKUP_RESULT_EMPTY){
		SLL_WARN("Unable to find file '%s'",path->data);
		_cli_had_warning=1;
		return 0;
	}
	*out=res_data.data.compilation_data;
	return 1;
}



static void _load_bundle(const sll_char_t* nm,sll_file_t* rf){
	sll_bundle_t b_dt;
	if (!sll_load_bundle(rf,&b_dt)){
		return;
	}
	_cli_bundle_list_len++;
	_cli_bundle_list=sll_reallocate(_cli_bundle_list,_cli_bundle_list_len*sizeof(cli_bundle_source_t*));
	cli_bundle_source_t* b=sll_allocate(sizeof(cli_bundle_source_t));
	sll_string_length_t nml=sll_string_length(nm)+1;
	b->name=sll_allocate(nml);
	sll_copy_data(nm,nml,b->name);
	b->bundle=b_dt;
	*(_cli_bundle_list+_cli_bundle_list_len-1)=b;
}



static void _check_release_mode(sll_array_length_t argc,const sll_char_t*const*argv){
	do{
		const sll_char_t* e=*argv;
		argc--;
		argv++;
		sll_char_t nm=(*e=='-'&&*(e+1)&&!(*(e+2))?*(e+1):0);
		if (nm=='A'||sll_string_compare_pointer(e,SLL_CHAR("--args"))==SLL_COMPARE_RESULT_EQUAL){
			break;
		}
		else if (nm=='B'||nm=='f'||nm=='F'||nm=='i'||nm=='I'||nm=='L'||nm=='N'||nm=='o'||nm=='s'||nm=='S'||nm=='x'||sll_string_compare_pointer(e,SLL_CHAR("--file"))==SLL_COMPARE_RESULT_EQUAL||sll_string_compare_pointer(e,SLL_CHAR("--file-path-resolver"))==SLL_COMPARE_RESULT_EQUAL||sll_string_compare_pointer(e,SLL_CHAR("--install-path"))==SLL_COMPARE_RESULT_EQUAL||sll_string_compare_pointer(e,SLL_CHAR("--include"))==SLL_COMPARE_RESULT_EQUAL||sll_string_compare_pointer(e,SLL_CHAR("--audit-library"))==SLL_COMPARE_RESULT_EQUAL||sll_string_compare_pointer(e,SLL_CHAR("--bundle-name"))==SLL_COMPARE_RESULT_EQUAL||sll_string_compare_pointer(e,SLL_CHAR("--output"))==SLL_COMPARE_RESULT_EQUAL||sll_string_compare_pointer(e,SLL_CHAR("--bundle-output"))==SLL_COMPARE_RESULT_EQUAL||sll_string_compare_pointer(e,SLL_CHAR("--source"))==SLL_COMPARE_RESULT_EQUAL||sll_string_compare_pointer(e,SLL_CHAR("--sandbox"))==SLL_COMPARE_RESULT_EQUAL||sll_string_compare_pointer(e,SLL_CHAR("--optimization-rounds"))==SLL_COMPARE_RESULT_EQUAL){
			if (argc){
				argc--;
				argv++;
			}
		}
		else if (nm=='r'||sll_string_compare_pointer(e,SLL_CHAR("--release-mode"))==SLL_COMPARE_RESULT_EQUAL){
			_cli_flags|=SLL_CLI_FLAG_RELEASE_MODE;
		}
	} while (argc);
}



static cli_include_dir_t* _alloc_include_dir(void){
	_cli_include_list_len++;
	_cli_include_list=sll_reallocate(_cli_include_list,_cli_include_list_len*sizeof(cli_include_dir_t*));
	cli_include_dir_t* o=sll_allocate(sizeof(cli_include_dir_t));
	*(_cli_include_list+_cli_include_list_len-1)=o;
	return o;
}



static void _ensure_path_separator(const sll_char_t* str,sll_string_length_t len,sll_string_t* out){
	sll_bool_t add_separator=(*(str+len-1)!='\\'&&*(str+len-1)!='/');
	sll_string_create(len+add_separator,out);
	sll_copy_data(str,len,out->data);
	if (add_separator){
		out->data[len]=SLL_API_FILE_PATH_SEPARATOR;
	}
	sll_string_calculate_checksum(out);
}



static sll_bool_t _starts_with_path_prefix(const sll_string_t* path,const sll_string_t* prefix){
	if (prefix->length>path->length){
		return 0;
	}
	for (sll_string_length_t i=0;i<prefix->length;i++){
		sll_char_t a=path->data[i];
		sll_char_t b=prefix->data[i];
		if ((a=='\\'?'/':a)!=(b=='\\'?'/':b)){
			return 0;
		}
	}
	return 1;
}



__SLL_EXTERNAL void sll_cli_expand_path(const sll_char_t* path,sll_char_t* out){
	if (_cli_flags&SLL_CLI_FLAG_EXPAND_PATH){
		sll_platform_absolute_path(path,out,SLL_API_MAX_FILE_PATH_LENGTH);
	}
	else{
		SLL_COPY_STRING_NULL(path,out);
	}
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_cli_lookup_result_t sll_cli_lookup_file(const sll_string_t* path,sll_bool_t use_custom_resolvers,sll_cli_lookup_data_t* out){
	if (!_cli_enable_file_lookup){
		return SLL_LOOKUP_RESULT_EMPTY;
	}
	if (use_custom_resolvers){
		for (sll_array_length_t i=0;i<_cli_resolver_table_size;i++){
			sll_cli_lookup_result_t res=_cli_resolver_table[i](path,out);
			if (res!=SLL_LOOKUP_RESULT_EMPTY){
				return res;
			}
		}
	}
	sll_audit(SLL_CHAR("sll.cli.find"),SLL_CHAR("S"),path);
	sll_char_t bf[SLL_API_MAX_FILE_PATH_LENGTH];
	if (_cli_bundle_list_len){
		sll_array_length_t i=_cli_bundle_list_len;
		do{
			i--;
			cli_bundle_source_t* b_dt=*(_cli_bundle_list+i);
			SLL_LOG("Trying to open file '%s/%s'...",b_dt->name,path->data);
			if (sll_bundle_fetch(&(b_dt->bundle),path,&(out->data.compilation_data))){
				SLL_LOG("File successfully read.");
				return SLL_LOOKUP_RESULT_COMPILED_OBJECT;
			}
		} while(i);
	}
	for (sll_string_length_t i=0;i<_cli_include_list_len;i++){
		cli_include_dir_t* inc=*(_cli_include_list+i);
		if (!_starts_with_path_prefix(path,&(inc->name))){
			continue;
		}
		sll_copy_data(inc->path.data,inc->path.length,bf);
		sll_copy_data(path->data+inc->name.length,path->length-inc->name.length,bf+inc->path.length);
		sll_string_length_t j=inc->path.length+path->length-inc->name.length;
		sll_copy_data(_cli_slc_suffix.data,_cli_slc_suffix.length+1,bf+j);
		SLL_LOG("Trying to open file '%s'...",bf);
		sll_file_t f;
		if (sll_platform_path_exists(bf)){
			sll_file_open(bf,SLL_FILE_FLAG_READ,&f);
			sll_cli_expand_path(bf,out->path);
			SLL_LOG("Found file '%s'",out->path);
			if (sll_load_compiled_node(&f,&(out->data.compilation_data))){
				sll_file_close(&f);
				SLL_LOG("File successfully read.");
				return SLL_LOOKUP_RESULT_COMPILED_OBJECT;
			}
			sll_file_close(&f);
			SLL_LOG("File is not a compiled program");
		}
		bf[j]=0;
		SLL_LOG("Trying to open file '%s'...",bf);
		if (!sll_platform_path_exists(bf)){
			continue;
		}
		sll_file_open(bf,SLL_FILE_FLAG_READ,&f);
		sll_cli_expand_path(bf,out->path);
		SLL_LOG("Found file '%s'",out->path);
		if (sll_load_assembly(&f,&(out->data.assembly_data))){
			sll_file_close(&f);
			SLL_LOG("File successfully read.");
			return SLL_LOOKUP_RESULT_ASSEMBLY;
		}
		sll_file_reset(&f);
		if (sll_load_compiled_node(&f,&(out->data.compilation_data))){
			sll_file_close(&f);
			SLL_LOG("File successfully read.");
			return SLL_LOOKUP_RESULT_COMPILED_OBJECT;
		}
		sll_file_reset(&f);
		sll_init_compilation_data(out->path,&(out->data.compilation_data));
		sll_parse_nodes(&f,&(out->data.compilation_data),&_cli_ift,_import_file);
		sll_file_close(&f);
		SLL_LOG("File successfully read.");
		return SLL_LOOKUP_RESULT_COMPILED_OBJECT;
	}
	if (_cli_lib_path_len){
		sll_copy_data(path->data,path->length,_cli_lib_path+_cli_lib_path_len);
		sll_copy_data(_cli_slc_suffix.data,_cli_slc_suffix.length+1,_cli_lib_path+_cli_lib_path_len+path->length);
		SLL_LOG("Trying to open file '%s'...",_cli_lib_path);
		sll_file_t f;
		if (sll_platform_path_exists(_cli_lib_path)){
			sll_file_open(_cli_lib_path,SLL_FILE_FLAG_READ,&f);
			sll_cli_expand_path(_cli_lib_path,out->path);
			SLL_LOG("Found file '%s'",out->path);
			if (sll_load_compiled_node(&f,&(out->data.compilation_data))){
				sll_file_close(&f);
				SLL_LOG("File successfully read.");
				return SLL_LOOKUP_RESULT_COMPILED_OBJECT;
			}
			sll_file_close(&f);
			SLL_LOG("File is not a compiled program");
		}
	}
	return SLL_LOOKUP_RESULT_EMPTY;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_return_code_t sll_cli_main(sll_array_length_t argc,const sll_char_t*const* argv){
	if (!argc){
		return 0;
	}
	sll_init();
	sll_return_code_t ec=0;
	_cli_flags=0;
	_check_release_mode(argc,argv);
	_cli_include_list=NULL;
	_cli_include_list_len=0;
	_cli_bundle_list=NULL;
	_cli_bundle_list_len=0;
	_cli_optimization_count=1;
	_cli_had_warning=0;
	cli_include_dir_t* inc=_alloc_include_dir();
	SLL_INIT_STRING(&(inc->name));
	SLL_INIT_STRING(&(inc->path));
	SLL_ASSERT(sll_library_file_path->length<SLL_API_MAX_FILE_PATH_LENGTH);
	_cli_lib_path_len=sll_library_file_path->length;
	while (_cli_lib_path_len&&sll_library_file_path->data[_cli_lib_path_len]!='/'&&sll_library_file_path->data[_cli_lib_path_len]!='\\'){
		_cli_lib_path_len--;
	}
	_cli_lib_path_len++;
	sll_copy_data(sll_library_file_path->data,_cli_lib_path_len,_cli_lib_path);
	if (_cli_flags&SLL_CLI_FLAG_RELEASE_MODE){
		SLL_COPY_STRING_NULL(SLL_CHAR("lib/"),_cli_lib_path+_cli_lib_path_len);
		_cli_lib_path_len+=4;
	}
	else{
		SLL_COPY_STRING_NULL(SLL_CHAR("lib_debug/"),_cli_lib_path+_cli_lib_path_len);
		_cli_lib_path_len+=10;
	}
	sll_string_t* b_fl=NULL;
	sll_array_length_t b_fll=sll_platform_list_directory(_cli_lib_path,&b_fl,NULL);
	while (b_fll){
		b_fll--;
		sll_string_t* str=b_fl+b_fll;
		sll_copy_data(str->data,str->length+1,_cli_lib_path+_cli_lib_path_len);
		sll_free_string(str);
		if (!sll_platform_path_is_directory(_cli_lib_path)){
			sll_file_t bundle_fh;
			if (sll_file_open(_cli_lib_path,SLL_FILE_FLAG_READ,&bundle_fh)==SLL_NO_ERROR){
				_load_bundle(_cli_lib_path,&bundle_fh);
				sll_file_close(&bundle_fh);
			}
		}
	}
	_cli_lib_path[_cli_lib_path_len]=0;
	sll_deallocate(b_fl);
	sll_array_length_t* fp=NULL;
	sll_string_length_t fpl=0;
	sll_array_length_t* sl=NULL;
	sll_string_length_t sll=0;
	cli_audit_library_t* audit_library_list=NULL;
	sll_array_length_t audit_library_list_len=0;
	sll_library_handle_t* path_resolver_list=NULL;
	sll_array_length_t path_resolver_list_len=0;
	sll_create_internal_function_table(&_cli_ift);
	sll_register_internal_functions(&_cli_ift,sll_builtin_internal_functions->data,sll_builtin_internal_functions->length);
	const sll_char_t* b_nm=NULL;
	const sll_char_t* b_o_fp=NULL;
	const sll_char_t* o_fp=NULL;
	sll_set_argument_count(1);
	sll_array_length_t i=0;
	do{
		const sll_char_t* e=argv[i];
		sll_char_t nm=(*e=='-'&&*(e+1)&&!(*(e+2))?*(e+1):0);
		if (nm=='a'||sll_string_compare_pointer(e,SLL_CHAR("--generate-assembly"))==SLL_COMPARE_RESULT_EQUAL){
			_cli_flags|=SLL_CLI_FLAG_GENERATE_ASSEMBLY;
		}
		else if (nm=='A'||sll_string_compare_pointer(e,SLL_CHAR("--args"))==SLL_COMPARE_RESULT_EQUAL){
			sll_set_argument_count(argc-i);
			for (sll_array_length_t j=0;j<argc-i-1;j++){
				sll_set_argument(j+1,argv[i+j+1]);
			}
			break;
		}
		else if (nm=='b'||sll_string_compare_pointer(e,SLL_CHAR("--bundle"))==SLL_COMPARE_RESULT_EQUAL){
			_cli_flags|=SLL_CLI_FLAG_GENERATE_BUNDLE;
		}
		else if (nm=='B'||sll_string_compare_pointer(e,SLL_CHAR("--bundle-output"))==SLL_COMPARE_RESULT_EQUAL){
			i++;
			if (i==argc){
				break;
			}
			b_o_fp=argv[i];
		}
		else if (nm=='c'||sll_string_compare_pointer(e,SLL_CHAR("--generate-compiled-object"))==SLL_COMPARE_RESULT_EQUAL){
			_cli_flags|=SLL_CLI_FLAG_GENERATE_COMPILED_OBJECT;
		}
		else if (nm=='d'||sll_string_compare_pointer(e,SLL_CHAR("--strip-names"))==SLL_COMPARE_RESULT_EQUAL){
			_cli_flags|=SLL_CLI_FLAG_STRIP_NAMES;
		}
		else if (nm=='D'||sll_string_compare_pointer(e,SLL_CHAR("--strip-debug"))==SLL_COMPARE_RESULT_EQUAL){
			_cli_flags|=SLL_CLI_FLAG_STRIP_DEBUG;
		}
		else if (nm=='e'||sll_string_compare_pointer(e,SLL_CHAR("--expand-file-paths"))==SLL_COMPARE_RESULT_EQUAL){
			_cli_flags|=SLL_CLI_FLAG_EXPAND_PATH;
		}
		else if (nm=='E'||sll_string_compare_pointer(e,SLL_CHAR("--enable-errors"))==SLL_COMPARE_RESULT_EQUAL){
			_cli_flags|=SLL_CLI_FLAG_ENABLE_ERRORS;
		}
		else if (nm=='f'||sll_string_compare_pointer(e,SLL_CHAR("--file"))==SLL_COMPARE_RESULT_EQUAL){
			i++;
			if (i==argc){
				break;
			}
			goto _read_file_argument;
		}
		else if (nm=='F'||sll_string_compare_pointer(e,SLL_CHAR("--file-path-resolver"))==SLL_COMPARE_RESULT_EQUAL){
			i++;
			if (i==argc){
				break;
			}
			e=argv[i];
			sll_library_handle_t lh=sll_platform_load_library(e,NULL);
			if (lh){
				sll_cli_path_resolver_t cb=sll_platform_lookup_symbol(lh,SLL_ABI_NAME(SLL_ABI_PATH_RESOLVER_RESOLVE));
				if (!cb){
					SLL_WARN("Unable to load library '%s'",e);
					_cli_had_warning=1;
					SLL_CRITICAL_ERROR(sll_platform_unload_library(lh));
				}
				else{
					void (*init)(void)=sll_platform_lookup_symbol(lh,SLL_ABI_NAME(SLL_ABI_PATH_RESOLVER_INIT));
					if (init){
						init();
					}
					path_resolver_list_len++;
					path_resolver_list=sll_reallocate(path_resolver_list,path_resolver_list_len*sizeof(sll_library_handle_t));
					*(path_resolver_list+path_resolver_list_len-1)=lh;
					sll_cli_register_path_resolver(cb);
				}
			}
			else{
				SLL_WARN("Unable to load library '%s'",e);
				_cli_had_warning=1;
			}
		}
		else if (nm=='h'||sll_string_compare_pointer(e,SLL_CHAR("--help"))==SLL_COMPARE_RESULT_EQUAL){
			_cli_flags|=SLL_CLI_FLAG_HELP;
		}
		else if (nm=='i'||sll_string_compare_pointer(e,SLL_CHAR("--install-path"))==SLL_COMPARE_RESULT_EQUAL){
			i++;
			sll_char_t out[SLL_API_MAX_FILE_PATH_LENGTH]={'/',0};
			const sll_string_t* src=NULL;
			if (i<argc){
				e=argv[i];
				if (sll_string_compare_pointer(e,SLL_CHAR("executable"))==SLL_COMPARE_RESULT_EQUAL){
					src=sll_executable_file_path;
				}
				else if (sll_string_compare_pointer(e,SLL_CHAR("install-path"))==SLL_COMPARE_RESULT_EQUAL){
					SLL_COPY_STRING_NULL(SLL_CHAR("/"),out);
				}
				else if (sll_string_compare_pointer(e,SLL_CHAR("library"))==SLL_COMPARE_RESULT_EQUAL){
					src=sll_library_file_path;
				}
				else if (sll_string_compare_pointer(e,SLL_CHAR("source-path"))==SLL_COMPARE_RESULT_EQUAL){
					SLL_COPY_STRING_NULL(((_cli_flags&SLL_CLI_FLAG_RELEASE_MODE)?SLL_CHAR("/lib/"):SLL_CHAR("/lib_debug/")),out);
				}
			}
			if (src){
				sll_string_length_t sz=(src->length>=SLL_API_MAX_FILE_PATH_LENGTH?SLL_API_MAX_FILE_PATH_LENGTH-1:src->length);
				sll_copy_data(src->data,sz,out);
				out[sz]=0;
			}
			else{
				sll_string_length_t pfx_sz=sll_library_file_path->length;
				while (pfx_sz&&sll_library_file_path->data[pfx_sz]!='/'&&sll_library_file_path->data[pfx_sz]!='\\'){
					pfx_sz--;
				}
				pfx_sz++;
				sll_string_length_t sz=sll_string_length(out);
				SLL_ASSERT(sz+pfx_sz<SLL_API_MAX_FILE_PATH_LENGTH);
				sll_char_t tmp[SLL_API_MAX_FILE_PATH_LENGTH];
				sll_copy_data(sll_library_file_path->data,pfx_sz,tmp);
				sll_copy_data(out,sz,tmp+pfx_sz);
				tmp[pfx_sz+sz]=0;
				sz=sll_platform_absolute_path(tmp,out,SLL_API_MAX_FILE_PATH_LENGTH);
				if (sz<SLL_API_MAX_FILE_PATH_LENGTH-1&&(!sz||out[sz-1]!=SLL_API_FILE_PATH_SEPARATOR)){
					out[sz]=SLL_API_FILE_PATH_SEPARATOR;
					out[sz+1]=0;
				}
			}
			sll_file_write_format(sll_stdout,SLL_CHAR("%s\n"),NULL,out);
			goto _cleanup;
		}
		else if (nm=='I'||sll_string_compare_pointer(e,SLL_CHAR("--include"))==SLL_COMPARE_RESULT_EQUAL){
			i++;
			if (i==argc){
				break;
			}
			e=argv[i];
			sll_string_t tmp;
			sll_string_from_pointer(e,&tmp);
			if (tmp.length){
				sll_string_length_t split=sll_string_index_char(&tmp,'|',0,0);
				if (split&&split!=SLL_MAX_STRING_LENGTH){
					if (!sll_platform_path_is_directory(tmp.data+split+1)){
						SLL_UNIMPLEMENTED();
					}
					inc=_alloc_include_dir();
					_ensure_path_separator(tmp.data,split,&(inc->name));
					_ensure_path_separator(tmp.data+split+1,tmp.length-split-1,&(inc->path));
				}
				else if (sll_platform_path_is_directory(e)){
					inc=_alloc_include_dir();
					SLL_INIT_STRING(&(inc->name));
					_ensure_path_separator(tmp.data,tmp.length,&(inc->path));
				}
				else{
					sll_file_t bundle_fh;
					if (sll_file_open(e,SLL_FILE_FLAG_READ,&bundle_fh)==SLL_NO_ERROR){
						_load_bundle(e,&bundle_fh);
						sll_file_close(&bundle_fh);
					}
				}
			}
			sll_free_string(&tmp);
		}
		else if (nm=='L'||sll_string_compare_pointer(e,SLL_CHAR("--audit-library"))==SLL_COMPARE_RESULT_EQUAL){
			i++;
			if (i==argc){
				break;
			}
			e=argv[i];
			sll_library_handle_t lh=sll_platform_load_library(e,NULL);
			if (lh){
				sll_audit_callback_t cb=sll_platform_lookup_symbol(lh,SLL_ABI_NAME(SLL_ABI_AUDIT_CALL));
				if (!cb){
					SLL_WARN("Unable to load library '%s'",e);
					_cli_had_warning=1;
					SLL_CRITICAL_ERROR(sll_platform_unload_library(lh));
				}
				else{
					void (*init)(void)=sll_platform_lookup_symbol(lh,SLL_ABI_NAME(SLL_ABI_AUDIT_INIT));
					if (init){
						init();
					}
					audit_library_list_len++;
					audit_library_list=sll_reallocate(audit_library_list,audit_library_list_len*sizeof(cli_audit_library_t));
					(audit_library_list+audit_library_list_len-1)->name=e;
					(audit_library_list+audit_library_list_len-1)->handle=lh;
					sll_audit_register_callback(cb);
				}
			}
			else{
				SLL_WARN("Unable to load library '%s'",e);
				_cli_had_warning=1;
			}
		}
		else if (nm=='n'||sll_string_compare_pointer(e,SLL_CHAR("--names-only"))==SLL_COMPARE_RESULT_EQUAL){
			_cli_flags|=SLL_CLI_FLAG_NO_PATHS;
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
		else if (nm=='O'||sll_string_compare_pointer(e,SLL_CHAR("--optimize"))==SLL_COMPARE_RESULT_EQUAL){
			_cli_flags|=SLL_CLI_FLAG_OPTIMIZE;
		}
		else if (nm=='p'||sll_string_compare_pointer(e,SLL_CHAR("--print-objects"))==SLL_COMPARE_RESULT_EQUAL){
			_cli_flags|=SLL_CLI_FLAG_PRINT_NODES;
		}
		else if (nm=='P'||sll_string_compare_pointer(e,SLL_CHAR("--print-assembly"))==SLL_COMPARE_RESULT_EQUAL){
			_cli_flags|=SLL_CLI_FLAG_PRINT_ASSEMBLY;
		}
		else if (nm=='R'||sll_string_compare_pointer(e,SLL_CHAR("--no-run"))==SLL_COMPARE_RESULT_EQUAL){
			_cli_flags|=SLL_CLI_FLAG_NO_RUN;
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
			else if (sll_string_compare_pointer(e,SLL_CHAR("no-ref"))==SLL_COMPARE_RESULT_EQUAL){
				sll_set_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_REFERENCE_COUNTER);
			}
			else{
				SLL_WARN(SLL_CHAR("Ignoring unknown Sandbox Flag '%s'"),e);
				_cli_had_warning=1;
			}
		}
		else if (nm=='v'||sll_string_compare_pointer(e,SLL_CHAR("--verbose"))==SLL_COMPARE_RESULT_EQUAL){
			_cli_flags|=SLL_CLI_FLAG_VERBOSE;
		}
		else if (nm=='V'||sll_string_compare_pointer(e,SLL_CHAR("--version"))==SLL_COMPARE_RESULT_EQUAL){
			_cli_flags|=SLL_CLI_FLAG_VERSION;
		}
		else if (nm=='x'||sll_string_compare_pointer(e,SLL_CHAR("--optimization-rounds"))==SLL_COMPARE_RESULT_EQUAL){
			i++;
			if (i==argc){
				break;
			}
			e=argv[i];
			__SLL_U16 rounds=0;
			while (*e){
				if (*e<48||*e>57){
					goto _skip_int_read;
				}
				rounds=rounds*10+(*e)-48;
				if (rounds>255){
					rounds=255;
					break;
				}
				e++;
			}
			if (!rounds){
				rounds=1;
			}
			_cli_optimization_count=(sll_optimization_round_count_t)rounds;
			goto _skip_warning;
_skip_int_read:;
			SLL_WARN("Ignoring non-integer count '%s'",argv[i]);
			_cli_had_warning=1;
_skip_warning:;
		}
		else if (nm=='X'||sll_string_compare_pointer(e,SLL_CHAR("--disable-audit"))==SLL_COMPARE_RESULT_EQUAL){
			sll_audit_enable(0);
		}
		else if (nm!='r'&&sll_string_compare_pointer(e,SLL_CHAR("--release-mode"))!=SLL_COMPARE_RESULT_EQUAL){
			if (*e=='-'){
				SLL_WARN("Ignroing unknown switch '%s'",e);
				_cli_had_warning=1;
			}
			else{
_read_file_argument:
				fpl++;
				fp=sll_reallocate(fp,fpl*sizeof(sll_array_length_t));
				*(fp+fpl-1)=i;
			}
		}
		i++;
	} while (i<argc);
	sll_audit(SLL_CHAR("sll.cli.init.raw"),SLL_CHAR("S+"),argv,argc);
	sll_audit(SLL_CHAR("sll.cli.init"),SLL_CHAR("uS{ss}{Si}{Sp}Si"),_cli_flags,o_fp,_cli_include_list,_cli_include_list_len,0ull,SLL_OFFSETOF(cli_include_dir_t,name),SLL_OFFSETOF(cli_include_dir_t,path),audit_library_list,audit_library_list_len,sizeof(cli_audit_library_t),SLL_OFFSETOF(cli_audit_library_t,name),SLL_OFFSETOF(cli_audit_library_t,handle),_cli_bundle_list,_cli_bundle_list_len,0ull,SLL_OFFSETOF(cli_bundle_source_t,name),SLL_OFFSETOF(cli_bundle_source_t,bundle),b_nm,sll_get_sandbox_flags());
	if (_cli_flags&SLL_CLI_FLAG_VERBOSE){
		SLL_CRITICAL(sll_set_log_flags(NULL,NULL,SLL_LOG_FLAG_SHOW,1));
		SLL_CRITICAL(sll_set_log_flags(SLL_CHAR(__FILE__),NULL,SLL_LOG_FLAG_NO_HEADER,1));
	}
	if (_cli_flags&SLL_CLI_FLAG_VERSION){
		sll_date_t d;
		sll_date_from_time_ns(SLL_VERSION_BUILD_TIME,sll_platform_time_zone,&d);
		sll_file_write_format(sll_stdout,SLL_CHAR("sll "SLL_VERSION_STRING" ("CLI_BUILD_TYPE_STRING", %.4u/%.2u/%.2u %.2u:%.2u:%.2u)\n"),NULL,d.year,d.month+1,d.day+1,d.hour,d.minute,floor(d.second));
		goto _cleanup;
	}
	if (_cli_flags&SLL_CLI_FLAG_HELP){
		sll_file_write(sll_stdout,HELP_TEXT,HELP_TEXT_SIZE,NULL);
		goto _cleanup;
	}
	if (_cli_flags&SLL_CLI_FLAG_VERBOSE){
		SLL_LOG("Configuration:");
		if (_cli_flags&SLL_CLI_FLAG_EXPAND_PATH){
			SLL_LOG("  Path expansion");
		}
		if (_cli_flags&SLL_CLI_FLAG_GENERATE_ASSEMBLY){
			SLL_LOG("  Assembly generation");
		}
		if (_cli_flags&SLL_CLI_FLAG_GENERATE_BUNDLE){
			SLL_LOG("  Bundle generation");
		}
		if (_cli_flags&SLL_CLI_FLAG_GENERATE_COMPILED_OBJECT){
			SLL_LOG("  Compiled program generation");
		}
		if (_cli_flags&SLL_CLI_FLAG_NO_PATHS){
			SLL_LOG("  No bundle file paths");
		}
		if (!(_cli_flags&SLL_CLI_FLAG_NO_RUN)){
			SLL_LOG("  Execution");
		}
		if (_cli_flags&SLL_CLI_FLAG_OPTIMIZE){
			SLL_LOG("  Optimizations");
		}
		if (_cli_flags&SLL_CLI_FLAG_PRINT_ASSEMBLY){
			SLL_LOG("  Assembly printing");
		}
		if (_cli_flags&SLL_CLI_FLAG_PRINT_NODES){
			SLL_LOG("  Compiled program printing");
		}
		if (_cli_flags&SLL_CLI_FLAG_RELEASE_MODE){
			SLL_LOG("  Release mode");
		}
		if (_cli_flags&SLL_CLI_FLAG_STRIP_DEBUG){
			SLL_LOG("  Debug data stripping");
		}
		if (_cli_flags&SLL_CLI_FLAG_STRIP_NAMES){
			SLL_LOG("  Function name stripping");
		}
		if (_cli_flags&SLL_CLI_FLAG_ENABLE_ERRORS){
			SLL_LOG("  Errors");
		}
		SLL_LOG("Include path:");
		for (sll_array_length_t j=0;j<_cli_bundle_list_len;j++){
			SLL_LOG("  '%s' (bundle)",(*(_cli_bundle_list+j))->name);
		}
		for (sll_string_length_t j=0;j<_cli_include_list_len;j++){
			inc=*(_cli_include_list+j);
			if (inc->name.length){
				SLL_LOG("  '%s' -> '%s'",inc->name.data,inc->path.data);
			}
			else{
				SLL_LOG("  '%s'",inc->path.data);
			}
		}
		SLL_LOG("Library path: '%s'",_cli_lib_path);
		if (o_fp){
			SLL_LOG("Output path: '%s'",o_fp);
		}
		SLL_LOG("Audit libraries:");
		for (sll_array_length_t j=0;j<audit_library_list_len;j++){
			SLL_LOG("  '%s'",(audit_library_list+j)->name);
		}
	}
	if (fpl+sll==1){
		if (!o_fp){
			_cli_flags|=CLI_FLAG_SINGLE_OUTPUT;
		}
		else{
			sll_char_t last=o_fp[sll_string_length(o_fp)-1];
			if (last!='\\'&&last!='/'){
				_cli_flags|=CLI_FLAG_SINGLE_OUTPUT;
			}
		}
	}
	sll_bundle_t bundle;
	if (_cli_flags&SLL_CLI_FLAG_GENERATE_BUNDLE){
		sll_bundle_create((!b_nm||((b_nm[0]=='\\'||b_nm[0]=='/')&&!b_nm[1])?NULL:b_nm),&bundle);
	}
	for (sll_string_length_t j=0;j<fpl+sll;j++){
		sll_assembly_data_t assembly_data=SLL_INIT_ASSEMBLY_DATA_STRUCT;
		sll_compilation_data_t compilation_data=SLL_INIT_COMPILATION_DATA_STRUCT;
		sll_char_t f_fp[SLL_API_MAX_FILE_PATH_LENGTH];
		sll_cli_lookup_result_t generated_type=SLL_LOOKUP_RESULT_COMPILED_OBJECT;
		_cli_enable_file_lookup=1;
		if (j<fpl){
			sll_audit(SLL_CHAR("sll.cli.load"),SLL_CHAR("S"),argv[*(fp+j)]);
			sll_string_t tmp;
			sll_string_from_pointer(argv[*(fp+j)],&tmp);
			sll_cli_lookup_data_t res_data;
			generated_type=sll_cli_lookup_file(&tmp,1,&res_data);
			if (generated_type==SLL_LOOKUP_RESULT_COMPILED_OBJECT){
				compilation_data=res_data.data.compilation_data;
			}
			else if (generated_type==SLL_LOOKUP_RESULT_ASSEMBLY){
				assembly_data=res_data.data.assembly_data;
			}
			else{
				SLL_WARN("Unable to find file '%s'",tmp.data);
				if (_cli_flags&SLL_CLI_FLAG_ENABLE_ERRORS){
					ec=1;
					goto _cleanup;
				}
				sll_init_compilation_data(tmp.data,&compilation_data);
			}
			sll_free_string(&tmp);
			sll_copy_data(res_data.path,SLL_API_FILE_PATH_SEPARATOR,f_fp);
			sll_char_t bf[SLL_API_MAX_FILE_PATH_LENGTH];
			sll_cli_expand_path(argv[*(fp+j)],bf);
			sll_set_argument(0,bf);
		}
		else{
			sll_audit(SLL_CHAR("sll.cli.load.source"),SLL_CHAR("S"),argv[*(sl+j-fpl)]);
			SLL_LOG("Compiling console input...");
			sll_init_compilation_data(SLL_CHAR("@console"),&compilation_data);
			sll_file_t f;
			sll_file_from_data(PTR(argv[*(sl+j-fpl)]),sll_string_length(argv[*(sl+j-fpl)]),SLL_FILE_FLAG_READ,&f);
			sll_parse_nodes(&f,&compilation_data,&_cli_ift,_import_file);
			sll_file_close(&f);
			SLL_LOG("Input successfully read.");
			sll_set_argument(0,SLL_CHAR("@console"));
		}
		if (_cli_had_warning&&(_cli_flags&SLL_CLI_FLAG_ENABLE_ERRORS)){
			if (generated_type==SLL_LOOKUP_RESULT_COMPILED_OBJECT){
				sll_free_compilation_data(&compilation_data);
			}
			else{
				sll_free_assembly_data(&assembly_data);
			}
			ec=1;
			goto _cleanup;
		}
		_cli_enable_file_lookup=0;
		if (generated_type==SLL_LOOKUP_RESULT_COMPILED_OBJECT){
			sll_source_file_t a_dt_sf;
			if ((_cli_flags&(SLL_CLI_FLAG_GENERATE_ASSEMBLY|SLL_CLI_FLAG_PRINT_ASSEMBLY))||!(_cli_flags&SLL_CLI_FLAG_NO_RUN)){
				SLL_LOG("Combining source files...");
				sll_unify_compilation_data(&compilation_data,&a_dt_sf);
				sll_free_compilation_data(&compilation_data);
				sll_compilation_data_from_source_file(&a_dt_sf,&compilation_data);
			}
			if (_cli_flags&SLL_CLI_FLAG_OPTIMIZE){
				SLL_LOG("Optimizing compilation data...");
				for (sll_source_file_index_t k=0;k<compilation_data.length;k++){
					sll_optimize_source_file(*(compilation_data.data+k),_cli_optimization_count);
				}
			}
			if (_cli_flags&SLL_CLI_FLAG_STRIP_DEBUG){
				SLL_LOG("Removing debugging data...");
				sll_remove_debug_data(&compilation_data);
			}
			if (_cli_flags&SLL_CLI_FLAG_STRIP_NAMES){
				SLL_LOG("Removing name data...");
				sll_remove_debug_names(&compilation_data);
			}
			SLL_LOG("Removing node padding...");
			sll_remove_node_padding(&compilation_data);
			SLL_LOG("Optimizing node metadata...");
			sll_optimize_metadata(&compilation_data);
			if (_cli_flags&SLL_CLI_FLAG_PRINT_NODES){
				sll_print_node(*(compilation_data.data),&_cli_ift,NULL,sll_stdout);
				sll_file_write_char(sll_stdout,'\n',NULL);
			}
			if ((_cli_flags&(SLL_CLI_FLAG_GENERATE_ASSEMBLY|SLL_CLI_FLAG_PRINT_ASSEMBLY))||!(_cli_flags&SLL_CLI_FLAG_NO_RUN)){
				SLL_LOG("Generating assembly...");
				SLL_ASSERT(compilation_data.length==1);
				sll_generate_assembly(*(compilation_data.data),&assembly_data);
			}
		}
		if (_cli_flags&SLL_CLI_FLAG_PRINT_ASSEMBLY){
			sll_print_assembly(&assembly_data,sll_stdout);
			sll_file_write_char(sll_stdout,'\n',NULL);
		}
		if (j<fpl&&(_cli_flags&(SLL_CLI_FLAG_GENERATE_ASSEMBLY|SLL_CLI_FLAG_GENERATE_COMPILED_OBJECT))){
			sll_char_t bf[SLL_API_MAX_FILE_PATH_LENGTH];
			sll_string_length_t k=0;
			sll_string_length_t f_fp_l=sll_string_length(f_fp);
			if (!o_fp){
				sll_copy_data(f_fp,f_fp_l,bf);
				k=f_fp_l-1;
			}
			else{
				k=sll_string_length(o_fp);
				sll_copy_data(o_fp,k,bf);
				if (_cli_flags&CLI_FLAG_SINGLE_OUTPUT){
					k-=1;
				}
				else{
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
			if (_cli_flags&SLL_CLI_FLAG_GENERATE_ASSEMBLY){
				bf[k]='a';
				sll_audit(SLL_CHAR("sll.cli.save.assembly"),SLL_CHAR("S"),bf);
				SLL_LOG("Writing assembly to file '%s'...",bf);
				sll_file_t of;
				SLL_CRITICAL_ERROR(sll_file_open(bf,SLL_FILE_FLAG_WRITE,&of));
				sll_write_assembly(&assembly_data,&of);
				SLL_LOG("File written successfully.");
				sll_file_close(&of);
			}
			if (_cli_flags&SLL_CLI_FLAG_GENERATE_COMPILED_OBJECT){
				bf[k]='c';
				sll_audit(SLL_CHAR("sll.cli.save.compiled"),SLL_CHAR("S"),bf);
				SLL_LOG("Writing compiled program to file '%s'...",bf);
				sll_file_t of;
				SLL_CRITICAL_ERROR(sll_file_open(bf,SLL_FILE_FLAG_WRITE,&of));
				sll_write_compiled_node(&compilation_data,&of);
				SLL_LOG("File written successfully.");
				sll_file_close(&of);
			}
		}
		if (!(_cli_flags&SLL_CLI_FLAG_NO_RUN)){
			sll_vm_config_t cfg={
				CLI_VM_STACK_SIZE,
				CLI_VM_CALL_STACK_SIZE,
				&_cli_ift,
				sll_stdin,
				sll_stdout,
				sll_stderr
			};
			sll_file_flush(sll_stdout);
			sll_file_flush(sll_stderr);
			ec=sll_execute_assembly(&assembly_data,&cfg);
			sll_file_flush(sll_stdout);
			sll_file_flush(sll_stderr);
		}
		sll_free_assembly_data(&assembly_data);
		if (j<fpl&&generated_type==SLL_LOOKUP_RESULT_COMPILED_OBJECT&&(_cli_flags&SLL_CLI_FLAG_GENERATE_BUNDLE)){
			sll_string_t b_f_nm;
			sll_string_from_pointer(argv[*(fp+j)],&b_f_nm);
			sll_string_length_t off=((_cli_flags&SLL_CLI_FLAG_NO_PATHS)?sll_path_split(&b_f_nm):0);
			if (sll_string_ends(&b_f_nm,&_cli_slc_suffix)){
				sll_string_set_char(&b_f_nm,0,b_f_nm.length-_cli_slc_suffix.length);
			}
			SLL_LOG("Adding file '%s' as '%s' to bundle...",f_fp,b_f_nm.data+off);
			sll_bundle_add_file(b_f_nm.data+off,&compilation_data,&bundle);
			sll_free_string(&b_f_nm);
		}
		else{
			sll_free_compilation_data(&compilation_data);
		}
	}
	if (_cli_flags&SLL_CLI_FLAG_GENERATE_BUNDLE){
		sll_char_t bf[SLL_API_MAX_FILE_PATH_LENGTH];
		if (b_o_fp){
			sll_string_length_t l=sll_string_length(b_o_fp);
			if (l>SLL_API_MAX_FILE_PATH_LENGTH-1){
				l=SLL_API_MAX_FILE_PATH_LENGTH-1;
			}
			sll_copy_data(b_o_fp,l,bf);
			bf[l]=0;
		}
		else{
			sll_string_length_t l=bundle.name.length;
			if (l>SLL_API_MAX_FILE_PATH_LENGTH-1){
				l=SLL_API_MAX_FILE_PATH_LENGTH-1;
			}
			sll_copy_data(bundle.name.data,l,bf);
			bf[l]=0;
		}
		if (!bf[0]){
			SLL_WARN(SLL_CHAR("No bundle output path supplied"));
			_cli_had_warning=1;
			sll_free_bundle(&bundle);
			goto _cleanup;
		}
		SLL_LOG("Writing bundle to '%s'...",bf);
		sll_file_t of;
		sll_file_open(bf,SLL_FILE_FLAG_WRITE,&of);
		sll_write_bundle(&bundle,&of);
		SLL_LOG("File written successfully.");
		sll_file_close(&of);
		sll_free_bundle(&bundle);
	}
_cleanup:
	while (_cli_include_list_len){
		_cli_include_list_len--;
		inc=*(_cli_include_list+_cli_include_list_len);
		sll_free_string(&(inc->name));
		sll_free_string(&(inc->path));
		sll_deallocate(inc);
	}
	sll_deallocate(_cli_include_list);
	sll_audit(SLL_CHAR("sll.cli.deinit"),SLL_CHAR(""));
	while (_cli_bundle_list_len){
		_cli_bundle_list_len--;
		cli_bundle_source_t* b=*(_cli_bundle_list+_cli_bundle_list_len);
		sll_deallocate(b->name);
		sll_free_bundle(&(b->bundle));
		sll_deallocate(b);
	}
	while (audit_library_list_len){
		audit_library_list_len--;
		void (*deinit)(void)=sll_platform_lookup_symbol((audit_library_list+audit_library_list_len)->handle,SLL_ABI_NAME(SLL_ABI_AUDIT_DEINIT));
		if (deinit){
			deinit();
		}
		SLL_CRITICAL_ERROR(sll_platform_unload_library((audit_library_list+audit_library_list_len)->handle));
	}
	while (path_resolver_list_len){
		path_resolver_list_len--;
		void (*deinit)(void)=sll_platform_lookup_symbol(*(path_resolver_list+path_resolver_list_len),SLL_ABI_NAME(SLL_ABI_PATH_RESOLVER_DEINIT));
		if (deinit){
			deinit();
		}
		SLL_CRITICAL_ERROR(sll_platform_unload_library(*(path_resolver_list+path_resolver_list_len)));
	}
	sll_deallocate(_cli_bundle_list);
	sll_deallocate(fp);
	sll_deallocate(sl);
	sll_deallocate(audit_library_list);
	sll_deallocate(path_resolver_list);
	sll_free_internal_function_table(&_cli_ift);
	sll_deinit();
	_cli_resolver_table_size=0;
	return ec;
}



__SLL_EXTERNAL void sll_cli_register_path_resolver(sll_cli_path_resolver_t function){
	if (_cli_resolver_table_size==CLI_PATH_RESOLVER_TABLE_SIZE){
		return;
	}
	for (sll_array_length_t i=0;i<_cli_resolver_table_size;i++){
		if (_cli_resolver_table[i]==function){
			return;
		}
	}
	_cli_resolver_table[_cli_resolver_table_size]=function;
	_cli_resolver_table_size++;
}



__SLL_EXTERNAL sll_bool_t sll_cli_unregister_path_resolver(sll_cli_path_resolver_t function){
	sll_array_length_t i=0;
	while (i<_cli_resolver_table_size){
		if (_cli_resolver_table[i]==function){
			i++;
			while (i<_cli_resolver_table_size){
				_cli_resolver_table[i-1]=_cli_resolver_table[i];
				i++;
			}
			_cli_resolver_table_size--;
			return 1;
		}
		i++;
	}
	return 0;
}
