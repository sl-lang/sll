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
	sll_cli_lookup_data_t data;
	sll_cli_lookup_result_t res=sll_cli_lookup_file(path,1,&data);
	if (res==SLL_LOOKUP_RESULT_ASSEMBLY){
		sll_free_assembly_data(&(data.data.assembly_data));
		SLL_WARN(SLL_CHAR("Importing assembly into compiled programs is not allowed"));
		_cli_had_warning=1;
		return 0;
	}
	if (res==SLL_LOOKUP_RESULT_EMPTY){
		SLL_WARN("Unable to find file '%s'",path->data);
		_cli_had_warning=1;
		return 0;
	}
	*out=data.data.compilation_data;
	return 1;
}



static void _load_bundle(const sll_char_t* name,sll_file_t* rf){
	sll_bundle_t bundle;
	if (!sll_load_bundle(rf,&bundle)){
		return;
	}
	_cli_bundle_list_len++;
	_cli_bundle_list=sll_reallocate(_cli_bundle_list,_cli_bundle_list_len*sizeof(cli_bundle_source_t*));
	cli_bundle_source_t* bundle_data=sll_allocate(sizeof(cli_bundle_source_t));
	sll_string_length_t name_length=sll_string_length(name)+1;
	bundle_data->name=sll_allocate(name_length);
	sll_copy_data(name,name_length,bundle_data->name);
	bundle_data->bundle=bundle;
	*(_cli_bundle_list+_cli_bundle_list_len-1)=bundle_data;
}



static void _check_release_mode(sll_array_length_t argc,const sll_char_t*const*argv){
	do{
		const sll_char_t* flag=*argv;
		argc--;
		argv++;
		sll_char_t short_flag=(*flag=='-'&&*(flag+1)&&!(*(flag+2))?*(flag+1):0);
		if (short_flag=='A'||sll_string_compare_pointer(flag,SLL_CHAR("--args"))==SLL_COMPARE_RESULT_EQUAL){
			break;
		}
		else if (short_flag=='B'||short_flag=='f'||short_flag=='F'||short_flag=='i'||short_flag=='I'||short_flag=='L'||short_flag=='N'||short_flag=='o'||short_flag=='s'||short_flag=='S'||short_flag=='x'||sll_string_compare_pointer(flag,SLL_CHAR("--file"))==SLL_COMPARE_RESULT_EQUAL||sll_string_compare_pointer(flag,SLL_CHAR("--file-path-resolver"))==SLL_COMPARE_RESULT_EQUAL||sll_string_compare_pointer(flag,SLL_CHAR("--install-path"))==SLL_COMPARE_RESULT_EQUAL||sll_string_compare_pointer(flag,SLL_CHAR("--include"))==SLL_COMPARE_RESULT_EQUAL||sll_string_compare_pointer(flag,SLL_CHAR("--audit-library"))==SLL_COMPARE_RESULT_EQUAL||sll_string_compare_pointer(flag,SLL_CHAR("--bundle-name"))==SLL_COMPARE_RESULT_EQUAL||sll_string_compare_pointer(flag,SLL_CHAR("--output"))==SLL_COMPARE_RESULT_EQUAL||sll_string_compare_pointer(flag,SLL_CHAR("--bundle-output"))==SLL_COMPARE_RESULT_EQUAL||sll_string_compare_pointer(flag,SLL_CHAR("--source"))==SLL_COMPARE_RESULT_EQUAL||sll_string_compare_pointer(flag,SLL_CHAR("--sandbox"))==SLL_COMPARE_RESULT_EQUAL||sll_string_compare_pointer(flag,SLL_CHAR("--optimization-rounds"))==SLL_COMPARE_RESULT_EQUAL){
			if (argc){
				argc--;
				argv++;
			}
		}
		else if (short_flag=='r'||sll_string_compare_pointer(flag,SLL_CHAR("--release-mode"))==SLL_COMPARE_RESULT_EQUAL){
			_cli_flags|=SLL_CLI_FLAG_RELEASE_MODE;
		}
	} while (argc);
}



static cli_include_dir_t* _alloc_include_dir(void){
	_cli_include_list_len++;
	_cli_include_list=sll_reallocate(_cli_include_list,_cli_include_list_len*sizeof(cli_include_dir_t*));
	cli_include_dir_t* out=sll_allocate(sizeof(cli_include_dir_t));
	*(_cli_include_list+_cli_include_list_len-1)=out;
	return out;
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
	sll_audit(SLL_CHAR("console_code_list_length.cli.find"),SLL_CHAR("S"),path);
	sll_char_t buffer[SLL_API_MAX_FILE_PATH_LENGTH];
	if (_cli_bundle_list_len){
		sll_array_length_t i=_cli_bundle_list_len;
		do{
			i--;
			cli_bundle_source_t* bundle_data=*(_cli_bundle_list+i);
			SLL_LOG("Trying to open file '%s/%s'...",bundle_data->name,path->data);
			if (sll_bundle_fetch(&(bundle_data->bundle),path,&(out->data.compilation_data))){
				SLL_LOG("File successfully read.");
				return SLL_LOOKUP_RESULT_COMPILED_OBJECT;
			}
		} while(i);
	}
	for (sll_string_length_t i=0;i<_cli_include_list_len;i++){
		cli_include_dir_t* include_directory=*(_cli_include_list+i);
		if (!_starts_with_path_prefix(path,&(include_directory->name))){
			continue;
		}
		sll_copy_data(include_directory->path.data,include_directory->path.length,buffer);
		sll_copy_data(path->data+include_directory->name.length,path->length-include_directory->name.length,buffer+include_directory->path.length);
		sll_string_length_t j=include_directory->path.length+path->length-include_directory->name.length;
		sll_copy_data(_cli_slc_suffix.data,_cli_slc_suffix.length+1,buffer+j);
		SLL_LOG("Trying to open file '%s'...",buffer);
		sll_file_t fh;
		if (sll_platform_path_exists(buffer)){
			sll_file_open(buffer,SLL_FILE_FLAG_READ,&fh);
			sll_cli_expand_path(buffer,out->path);
			SLL_LOG("Found file '%s'",out->path);
			if (sll_load_compiled_node(&fh,&(out->data.compilation_data))){
				sll_file_close(&fh);
				SLL_LOG("File successfully read.");
				return SLL_LOOKUP_RESULT_COMPILED_OBJECT;
			}
			sll_file_close(&fh);
			SLL_LOG("File is not a compiled program");
		}
		buffer[j]=0;
		SLL_LOG("Trying to open file '%s'...",buffer);
		if (!sll_platform_path_exists(buffer)){
			continue;
		}
		sll_file_open(buffer,SLL_FILE_FLAG_READ,&fh);
		sll_cli_expand_path(buffer,out->path);
		SLL_LOG("Found file '%s'",out->path);
		if (sll_load_assembly(&fh,&(out->data.assembly_data))){
			sll_file_close(&fh);
			SLL_LOG("File successfully read.");
			return SLL_LOOKUP_RESULT_ASSEMBLY;
		}
		sll_file_reset(&fh);
		if (sll_load_compiled_node(&fh,&(out->data.compilation_data))){
			sll_file_close(&fh);
			SLL_LOG("File successfully read.");
			return SLL_LOOKUP_RESULT_COMPILED_OBJECT;
		}
		sll_file_reset(&fh);
		sll_init_compilation_data(out->path,&(out->data.compilation_data));
		sll_parse_nodes(&fh,&(out->data.compilation_data),&_cli_ift,_import_file);
		sll_file_close(&fh);
		SLL_LOG("File successfully read.");
		return SLL_LOOKUP_RESULT_COMPILED_OBJECT;
	}
	if (_cli_lib_path_len){
		sll_copy_data(path->data,path->length,_cli_lib_path+_cli_lib_path_len);
		sll_copy_data(_cli_slc_suffix.data,_cli_slc_suffix.length+1,_cli_lib_path+_cli_lib_path_len+path->length);
		SLL_LOG("Trying to open file '%s'...",_cli_lib_path);
		if (sll_platform_path_exists(_cli_lib_path)){
			sll_file_t fh;
			sll_file_open(_cli_lib_path,SLL_FILE_FLAG_READ,&fh);
			sll_cli_expand_path(_cli_lib_path,out->path);
			SLL_LOG("Found file '%s'",out->path);
			if (sll_load_compiled_node(&fh,&(out->data.compilation_data))){
				sll_file_close(&fh);
				SLL_LOG("File successfully read.");
				return SLL_LOOKUP_RESULT_COMPILED_OBJECT;
			}
			sll_file_close(&fh);
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
	sll_return_code_t return_code=0;
	_cli_flags=0;
	_check_release_mode(argc,argv);
	_cli_include_list=NULL;
	_cli_include_list_len=0;
	_cli_bundle_list=NULL;
	_cli_bundle_list_len=0;
	_cli_optimization_count=1;
	_cli_had_warning=0;
	cli_include_dir_t* include_directory=_alloc_include_dir();
	SLL_INIT_STRING(&(include_directory->name));
	SLL_INIT_STRING(&(include_directory->path));
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
	sll_string_t* bundle_file_list=NULL;
	sll_array_length_t bundle_file_list_length=sll_platform_list_directory(_cli_lib_path,&bundle_file_list,NULL);
	while (bundle_file_list_length){
		bundle_file_list_length--;
		sll_string_t* str=bundle_file_list+bundle_file_list_length;
		sll_copy_data(str->data,str->length+1,_cli_lib_path+_cli_lib_path_len);
		sll_free_string(str);
		if (!sll_platform_path_is_directory(_cli_lib_path)){
			sll_file_t bundle_file;
			if (sll_file_open(_cli_lib_path,SLL_FILE_FLAG_READ,&bundle_file)==SLL_NO_ERROR){
				_load_bundle(_cli_lib_path,&bundle_file);
				sll_file_close(&bundle_file);
			}
		}
	}
	_cli_lib_path[_cli_lib_path_len]=0;
	sll_deallocate(bundle_file_list);
	sll_array_length_t* file_list=NULL;
	sll_string_length_t file_list_length=0;
	sll_array_length_t* console_code_list=NULL;
	sll_string_length_t console_code_list_length=0;
	cli_audit_library_t* audit_library_list=NULL;
	sll_array_length_t audit_library_list_len=0;
	sll_library_handle_t* path_resolver_list=NULL;
	sll_array_length_t path_resolver_list_len=0;
	sll_create_internal_function_table(&_cli_ift);
	sll_register_internal_functions(&_cli_ift,sll_builtin_internal_functions->data,sll_builtin_internal_functions->length);
	const sll_char_t* bundle_name=NULL;
	const sll_char_t* bundle_output_file_path=NULL;
	const sll_char_t* output_file_path=NULL;
	sll_set_argument_count(1);
	sll_array_length_t i=0;
	do{
		const sll_char_t* flag=argv[i];
		sll_char_t short_flag=(*flag=='-'&&*(flag+1)&&!(*(flag+2))?*(flag+1):0);
		if (short_flag=='a'||sll_string_compare_pointer(flag,SLL_CHAR("--generate-assembly"))==SLL_COMPARE_RESULT_EQUAL){
			_cli_flags|=SLL_CLI_FLAG_GENERATE_ASSEMBLY;
		}
		else if (short_flag=='A'||sll_string_compare_pointer(flag,SLL_CHAR("--args"))==SLL_COMPARE_RESULT_EQUAL){
			sll_set_argument_count(argc-i);
			for (sll_array_length_t j=0;j<argc-i-1;j++){
				sll_set_argument(j+1,argv[i+j+1]);
			}
			break;
		}
		else if (short_flag=='b'||sll_string_compare_pointer(flag,SLL_CHAR("--bundle"))==SLL_COMPARE_RESULT_EQUAL){
			_cli_flags|=SLL_CLI_FLAG_GENERATE_BUNDLE;
		}
		else if (short_flag=='B'||sll_string_compare_pointer(flag,SLL_CHAR("--bundle-output"))==SLL_COMPARE_RESULT_EQUAL){
			i++;
			if (i==argc){
				break;
			}
			bundle_output_file_path=argv[i];
		}
		else if (short_flag=='c'||sll_string_compare_pointer(flag,SLL_CHAR("--generate-compiled-object"))==SLL_COMPARE_RESULT_EQUAL){
			_cli_flags|=SLL_CLI_FLAG_GENERATE_COMPILED_OBJECT;
		}
		else if (short_flag=='d'||sll_string_compare_pointer(flag,SLL_CHAR("--strip-names"))==SLL_COMPARE_RESULT_EQUAL){
			_cli_flags|=SLL_CLI_FLAG_STRIP_NAMES;
		}
		else if (short_flag=='D'||sll_string_compare_pointer(flag,SLL_CHAR("--strip-debug"))==SLL_COMPARE_RESULT_EQUAL){
			_cli_flags|=SLL_CLI_FLAG_STRIP_DEBUG;
		}
		else if (short_flag=='e'||sll_string_compare_pointer(flag,SLL_CHAR("--expand-file-paths"))==SLL_COMPARE_RESULT_EQUAL){
			_cli_flags|=SLL_CLI_FLAG_EXPAND_PATH;
		}
		else if (short_flag=='E'||sll_string_compare_pointer(flag,SLL_CHAR("--enable-errors"))==SLL_COMPARE_RESULT_EQUAL){
			_cli_flags|=SLL_CLI_FLAG_ENABLE_ERRORS;
		}
		else if (short_flag=='f'||sll_string_compare_pointer(flag,SLL_CHAR("--file"))==SLL_COMPARE_RESULT_EQUAL){
			i++;
			if (i==argc){
				break;
			}
			goto _read_file_argument;
		}
		else if (short_flag=='F'||sll_string_compare_pointer(flag,SLL_CHAR("--file-path-resolver"))==SLL_COMPARE_RESULT_EQUAL){
			i++;
			if (i==argc){
				break;
			}
			flag=argv[i];
			sll_library_handle_t library_handle=sll_platform_load_library(flag,NULL);
			if (library_handle){
				sll_cli_path_resolver_t callback=sll_platform_lookup_symbol(library_handle,SLL_ABI_NAME(SLL_ABI_PATH_RESOLVER_RESOLVE));
				if (!callback){
					SLL_WARN("Unable to load library '%s'",flag);
					_cli_had_warning=1;
					SLL_CRITICAL_ERROR(sll_platform_unload_library(library_handle));
				}
				else{
					void (*init)(void)=sll_platform_lookup_symbol(library_handle,SLL_ABI_NAME(SLL_ABI_PATH_RESOLVER_INIT));
					if (init){
						init();
					}
					path_resolver_list_len++;
					path_resolver_list=sll_reallocate(path_resolver_list,path_resolver_list_len*sizeof(sll_library_handle_t));
					*(path_resolver_list+path_resolver_list_len-1)=library_handle;
					sll_cli_register_path_resolver(callback);
				}
			}
			else{
				SLL_WARN("Unable to load library '%s'",flag);
				_cli_had_warning=1;
			}
		}
		else if (short_flag=='h'||sll_string_compare_pointer(flag,SLL_CHAR("--help"))==SLL_COMPARE_RESULT_EQUAL){
			_cli_flags|=SLL_CLI_FLAG_HELP;
		}
		else if (short_flag=='i'||sll_string_compare_pointer(flag,SLL_CHAR("--install-path"))==SLL_COMPARE_RESULT_EQUAL){
			i++;
			sll_char_t out[SLL_API_MAX_FILE_PATH_LENGTH]={'/',0};
			const sll_string_t* base=NULL;
			if (i<argc){
				flag=argv[i];
				if (sll_string_compare_pointer(flag,SLL_CHAR("executable"))==SLL_COMPARE_RESULT_EQUAL){
					base=sll_executable_file_path;
				}
				else if (sll_string_compare_pointer(flag,SLL_CHAR("install-path"))==SLL_COMPARE_RESULT_EQUAL){
					SLL_COPY_STRING_NULL(SLL_CHAR("/"),out);
				}
				else if (sll_string_compare_pointer(flag,SLL_CHAR("library"))==SLL_COMPARE_RESULT_EQUAL){
					base=sll_library_file_path;
				}
				else if (sll_string_compare_pointer(flag,SLL_CHAR("source-path"))==SLL_COMPARE_RESULT_EQUAL){
					SLL_COPY_STRING_NULL(((_cli_flags&SLL_CLI_FLAG_RELEASE_MODE)?SLL_CHAR("/lib/"):SLL_CHAR("/lib_debug/")),out);
				}
			}
			if (base){
				sll_string_length_t length=(base->length>=SLL_API_MAX_FILE_PATH_LENGTH?SLL_API_MAX_FILE_PATH_LENGTH-1:base->length);
				sll_copy_data(base->data,length,out);
				out[length]=0;
			}
			else{
				sll_string_length_t prefix_size=sll_library_file_path->length;
				while (prefix_size&&sll_library_file_path->data[prefix_size]!='/'&&sll_library_file_path->data[prefix_size]!='\\'){
					prefix_size--;
				}
				prefix_size++;
				sll_string_length_t length=sll_string_length(out);
				SLL_ASSERT(length+prefix_size<SLL_API_MAX_FILE_PATH_LENGTH);
				sll_char_t tmp[SLL_API_MAX_FILE_PATH_LENGTH];
				sll_copy_data(sll_library_file_path->data,prefix_size,tmp);
				sll_copy_data(out,length,tmp+prefix_size);
				tmp[prefix_size+length]=0;
				length=sll_platform_absolute_path(tmp,out,SLL_API_MAX_FILE_PATH_LENGTH);
				if (length<SLL_API_MAX_FILE_PATH_LENGTH-1&&(!length||out[length-1]!=SLL_API_FILE_PATH_SEPARATOR)){
					out[length]=SLL_API_FILE_PATH_SEPARATOR;
					out[length+1]=0;
				}
			}
			sll_file_write_format(sll_stdout,SLL_CHAR("%s\n"),NULL,out);
			goto _cleanup;
		}
		else if (short_flag=='I'||sll_string_compare_pointer(flag,SLL_CHAR("--include"))==SLL_COMPARE_RESULT_EQUAL){
			i++;
			if (i==argc){
				break;
			}
			flag=argv[i];
			sll_string_t tmp;
			sll_string_from_pointer(flag,&tmp);
			if (tmp.length){
				sll_string_length_t split=sll_string_index_char(&tmp,'|',0,0);
				if (split&&split!=SLL_MAX_STRING_LENGTH){
					if (!sll_platform_path_is_directory(tmp.data+split+1)){
						SLL_UNIMPLEMENTED();
					}
					include_directory=_alloc_include_dir();
					_ensure_path_separator(tmp.data,split,&(include_directory->name));
					_ensure_path_separator(tmp.data+split+1,tmp.length-split-1,&(include_directory->path));
				}
				else if (sll_platform_path_is_directory(flag)){
					include_directory=_alloc_include_dir();
					SLL_INIT_STRING(&(include_directory->name));
					_ensure_path_separator(tmp.data,tmp.length,&(include_directory->path));
				}
				else{
					sll_file_t bundle_fh;
					if (sll_file_open(flag,SLL_FILE_FLAG_READ,&bundle_fh)==SLL_NO_ERROR){
						_load_bundle(flag,&bundle_fh);
						sll_file_close(&bundle_fh);
					}
				}
			}
			sll_free_string(&tmp);
		}
		else if (short_flag=='L'||sll_string_compare_pointer(flag,SLL_CHAR("--audit-library"))==SLL_COMPARE_RESULT_EQUAL){
			i++;
			if (i==argc){
				break;
			}
			flag=argv[i];
			sll_library_handle_t library_handle=sll_platform_load_library(flag,NULL);
			if (library_handle){
				sll_audit_callback_t callback=sll_platform_lookup_symbol(library_handle,SLL_ABI_NAME(SLL_ABI_AUDIT_CALL));
				if (!callback){
					SLL_WARN("Unable to load library '%s'",flag);
					_cli_had_warning=1;
					SLL_CRITICAL_ERROR(sll_platform_unload_library(library_handle));
				}
				else{
					void (*init)(void)=sll_platform_lookup_symbol(library_handle,SLL_ABI_NAME(SLL_ABI_AUDIT_INIT));
					if (init){
						init();
					}
					audit_library_list_len++;
					audit_library_list=sll_reallocate(audit_library_list,audit_library_list_len*sizeof(cli_audit_library_t));
					(audit_library_list+audit_library_list_len-1)->name=flag;
					(audit_library_list+audit_library_list_len-1)->handle=library_handle;
					sll_audit_register_callback(callback);
				}
			}
			else{
				SLL_WARN("Unable to load library '%s'",flag);
				_cli_had_warning=1;
			}
		}
		else if (short_flag=='n'||sll_string_compare_pointer(flag,SLL_CHAR("--names-only"))==SLL_COMPARE_RESULT_EQUAL){
			_cli_flags|=SLL_CLI_FLAG_NO_PATHS;
		}
		else if (short_flag=='N'||sll_string_compare_pointer(flag,SLL_CHAR("--bundle-name"))==SLL_COMPARE_RESULT_EQUAL){
			i++;
			if (i==argc){
				break;
			}
			bundle_name=argv[i];
		}
		else if (short_flag=='o'||sll_string_compare_pointer(flag,SLL_CHAR("--output"))==SLL_COMPARE_RESULT_EQUAL){
			i++;
			if (i==argc){
				break;
			}
			output_file_path=argv[i];
		}
		else if (short_flag=='O'||sll_string_compare_pointer(flag,SLL_CHAR("--optimize"))==SLL_COMPARE_RESULT_EQUAL){
			_cli_flags|=SLL_CLI_FLAG_OPTIMIZE;
		}
		else if (short_flag=='p'||sll_string_compare_pointer(flag,SLL_CHAR("--print-objects"))==SLL_COMPARE_RESULT_EQUAL){
			_cli_flags|=SLL_CLI_FLAG_PRINT_NODES;
		}
		else if (short_flag=='P'||sll_string_compare_pointer(flag,SLL_CHAR("--print-assembly"))==SLL_COMPARE_RESULT_EQUAL){
			_cli_flags|=SLL_CLI_FLAG_PRINT_ASSEMBLY;
		}
		else if (short_flag=='R'||sll_string_compare_pointer(flag,SLL_CHAR("--no-run"))==SLL_COMPARE_RESULT_EQUAL){
			_cli_flags|=SLL_CLI_FLAG_NO_RUN;
		}
		else if (short_flag=='s'||sll_string_compare_pointer(flag,SLL_CHAR("--source"))==SLL_COMPARE_RESULT_EQUAL){
			i++;
			if (i==argc){
				break;
			}
			console_code_list_length++;
			console_code_list=sll_reallocate(console_code_list,console_code_list_length*sizeof(sll_array_length_t));
			*(console_code_list+console_code_list_length-1)=i;
		}
		else if (short_flag=='S'||sll_string_compare_pointer(flag,SLL_CHAR("--sandbox"))==SLL_COMPARE_RESULT_EQUAL){
			i++;
			if (i==argc){
				break;
			}
			flag=argv[i];
			if (sll_string_compare_pointer(flag,SLL_CHAR("no-file-io"))==SLL_COMPARE_RESULT_EQUAL){
				sll_set_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_FILE_IO);
			}
			else if (sll_string_compare_pointer(flag,SLL_CHAR("stdin-io"))==SLL_COMPARE_RESULT_EQUAL){
				sll_set_sandbox_flag(SLL_SANDBOX_FLAG_ENABLE_STDIN_IO);
			}
			else if (sll_string_compare_pointer(flag,SLL_CHAR("stdout-io"))==SLL_COMPARE_RESULT_EQUAL){
				sll_set_sandbox_flag(SLL_SANDBOX_FLAG_ENABLE_STDOUT_IO);
			}
			else if (sll_string_compare_pointer(flag,SLL_CHAR("no-path-api"))==SLL_COMPARE_RESULT_EQUAL){
				sll_set_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_PATH_API);
			}
			else if (sll_string_compare_pointer(flag,SLL_CHAR("no-process-api"))==SLL_COMPARE_RESULT_EQUAL){
				sll_set_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_PROCESS_API);
			}
			else if (sll_string_compare_pointer(flag,SLL_CHAR("no-environment"))==SLL_COMPARE_RESULT_EQUAL){
				sll_set_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_ENVIRONMENT);
			}
			else if (sll_string_compare_pointer(flag,SLL_CHAR("no-load-library"))==SLL_COMPARE_RESULT_EQUAL){
				sll_set_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_LOAD_LIBRARY);
			}
			else if (sll_string_compare_pointer(flag,SLL_CHAR("buffer-files"))==SLL_COMPARE_RESULT_EQUAL){
				sll_set_sandbox_flag(SLL_SANDBOX_FLAG_ENABLE_BUFFER_FILES);
			}
			else if (sll_string_compare_pointer(flag,SLL_CHAR("file-rename"))==SLL_COMPARE_RESULT_EQUAL){
				sll_set_sandbox_flag(SLL_SANDBOX_FLAG_ENABLE_FILE_RENAME);
			}
			else if (sll_string_compare_pointer(flag,SLL_CHAR("file-copy"))==SLL_COMPARE_RESULT_EQUAL){
				sll_set_sandbox_flag(SLL_SANDBOX_FLAG_ENABLE_FILE_COPY);
			}
			else if (sll_string_compare_pointer(flag,SLL_CHAR("no-float-error-change"))==SLL_COMPARE_RESULT_EQUAL){
				sll_set_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_FLOAT_COMPARE_ERROR_CHANGE);
			}
			else if (sll_string_compare_pointer(flag,SLL_CHAR("file-delete"))==SLL_COMPARE_RESULT_EQUAL){
				sll_set_sandbox_flag(SLL_SANDBOX_FLAG_ENABLE_FILE_DELETE);
			}
			else if (sll_string_compare_pointer(flag,SLL_CHAR("no-thread"))==SLL_COMPARE_RESULT_EQUAL){
				sll_set_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_THREADS);
			}
			else if (sll_string_compare_pointer(flag,SLL_CHAR("no-random"))==SLL_COMPARE_RESULT_EQUAL){
				sll_set_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_RANDOM);
			}
			else if (sll_string_compare_pointer(flag,SLL_CHAR("no-serial"))==SLL_COMPARE_RESULT_EQUAL){
				sll_set_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_SERIAL);
			}
			else if (sll_string_compare_pointer(flag,SLL_CHAR("no-ref"))==SLL_COMPARE_RESULT_EQUAL){
				sll_set_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_REFERENCE_COUNTER);
			}
			else{
				SLL_WARN(SLL_CHAR("Ignoring unknown Sandbox Flag '%s'"),flag);
				_cli_had_warning=1;
			}
		}
		else if (short_flag=='v'||sll_string_compare_pointer(flag,SLL_CHAR("--verbose"))==SLL_COMPARE_RESULT_EQUAL){
			_cli_flags|=SLL_CLI_FLAG_VERBOSE;
		}
		else if (short_flag=='V'||sll_string_compare_pointer(flag,SLL_CHAR("--version"))==SLL_COMPARE_RESULT_EQUAL){
			_cli_flags|=SLL_CLI_FLAG_VERSION;
		}
		else if (short_flag=='x'||sll_string_compare_pointer(flag,SLL_CHAR("--optimization-rounds"))==SLL_COMPARE_RESULT_EQUAL){
			i++;
			if (i==argc){
				break;
			}
			flag=argv[i];
			__SLL_U16 rounds=0;
			while (*flag){
				if (*flag<48||*flag>57){
					goto _skip_int_read;
				}
				rounds=rounds*10+(*flag)-48;
				if (rounds>255){
					rounds=255;
					break;
				}
				flag++;
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
		else if (short_flag=='X'||sll_string_compare_pointer(flag,SLL_CHAR("--disable-audit"))==SLL_COMPARE_RESULT_EQUAL){
			sll_audit_enable(0);
		}
		else if (short_flag!='r'&&sll_string_compare_pointer(flag,SLL_CHAR("--release-mode"))!=SLL_COMPARE_RESULT_EQUAL){
			if (*flag=='-'){
				SLL_WARN("Ignroing unknown switch '%s'",flag);
				_cli_had_warning=1;
			}
			else{
_read_file_argument:
				file_list_length++;
				file_list=sll_reallocate(file_list,file_list_length*sizeof(sll_array_length_t));
				*(file_list+file_list_length-1)=i;
			}
		}
		i++;
	} while (i<argc);
	sll_audit(SLL_CHAR("console_code_list_length.cli.init.raw"),SLL_CHAR("S+"),argv,argc);
	sll_audit(SLL_CHAR("console_code_list_length.cli.init"),SLL_CHAR("uS{ss}{Si}{Sp}Si"),_cli_flags,output_file_path,_cli_include_list,_cli_include_list_len,0ull,SLL_OFFSETOF(cli_include_dir_t,name),SLL_OFFSETOF(cli_include_dir_t,path),audit_library_list,audit_library_list_len,sizeof(cli_audit_library_t),SLL_OFFSETOF(cli_audit_library_t,name),SLL_OFFSETOF(cli_audit_library_t,handle),_cli_bundle_list,_cli_bundle_list_len,0ull,SLL_OFFSETOF(cli_bundle_source_t,name),SLL_OFFSETOF(cli_bundle_source_t,bundle),bundle_name,sll_get_sandbox_flags());
	if (_cli_flags&SLL_CLI_FLAG_VERBOSE){
		SLL_CRITICAL(sll_set_log_flags(NULL,NULL,SLL_LOG_FLAG_SHOW,1));
		SLL_CRITICAL(sll_set_log_flags(SLL_CHAR(__FILE__),NULL,SLL_LOG_FLAG_NO_HEADER,1));
	}
	if (_cli_flags&SLL_CLI_FLAG_VERSION){
		sll_date_t date;
		sll_date_from_time_ns(SLL_VERSION_BUILD_TIME,sll_platform_time_zone,&date);
		sll_file_write_format(sll_stdout,SLL_CHAR("console_code_list_length "SLL_VERSION_STRING" ("CLI_BUILD_TYPE_STRING", %.4u/%.2u/%.2u %.2u:%.2u:%.2u)\n"),NULL,date.year,date.month+1,date.day+1,date.hour,date.minute,floor(date.second));
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
			include_directory=*(_cli_include_list+j);
			if (include_directory->name.length){
				SLL_LOG("  '%s' -> '%s'",include_directory->name.data,include_directory->path.data);
			}
			else{
				SLL_LOG("  '%s'",include_directory->path.data);
			}
		}
		SLL_LOG("Library path: '%s'",_cli_lib_path);
		if (output_file_path){
			SLL_LOG("Output path: '%s'",output_file_path);
		}
		SLL_LOG("Audit libraries:");
		for (sll_array_length_t j=0;j<audit_library_list_len;j++){
			SLL_LOG("  '%s'",(audit_library_list+j)->name);
		}
	}
	if (file_list_length+console_code_list_length==1){
		if (!output_file_path){
			_cli_flags|=CLI_FLAG_SINGLE_OUTPUT;
		}
		else{
			sll_char_t last_char=output_file_path[sll_string_length(output_file_path)-1];
			if (last_char!='\\'&&last_char!='/'){
				_cli_flags|=CLI_FLAG_SINGLE_OUTPUT;
			}
		}
	}
	sll_bundle_t bundle;
	if (_cli_flags&SLL_CLI_FLAG_GENERATE_BUNDLE){
		sll_bundle_create((!bundle_name||((bundle_name[0]=='\\'||bundle_name[0]=='/')&&!bundle_name[1])?NULL:bundle_name),&bundle);
	}
	for (sll_string_length_t j=0;j<file_list_length+console_code_list_length;j++){
		sll_assembly_data_t assembly_data=SLL_INIT_ASSEMBLY_DATA_STRUCT;
		sll_compilation_data_t compilation_data=SLL_INIT_COMPILATION_DATA_STRUCT;
		sll_char_t file_path[SLL_API_MAX_FILE_PATH_LENGTH];
		sll_cli_lookup_result_t generated_type=SLL_LOOKUP_RESULT_COMPILED_OBJECT;
		_cli_enable_file_lookup=1;
		if (j<file_list_length){
			sll_audit(SLL_CHAR("console_code_list_length.cli.load"),SLL_CHAR("S"),argv[*(file_list+j)]);
			sll_string_t tmp;
			sll_string_from_pointer(argv[*(file_list+j)],&tmp);
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
					return_code=1;
					goto _cleanup;
				}
				sll_init_compilation_data(tmp.data,&compilation_data);
			}
			sll_free_string(&tmp);
			sll_copy_data(res_data.path,SLL_API_FILE_PATH_SEPARATOR,file_path);
			sll_char_t buffer[SLL_API_MAX_FILE_PATH_LENGTH];
			sll_cli_expand_path(argv[*(file_list+j)],buffer);
			sll_set_argument(0,buffer);
		}
		else{
			sll_audit(SLL_CHAR("console_code_list_length.cli.load.source"),SLL_CHAR("S"),argv[*(console_code_list+j-file_list_length)]);
			SLL_LOG("Compiling console input...");
			sll_init_compilation_data(SLL_CHAR("@console"),&compilation_data);
			sll_file_t f;
			sll_file_from_data(PTR(argv[*(console_code_list+j-file_list_length)]),sll_string_length(argv[*(console_code_list+j-file_list_length)]),SLL_FILE_FLAG_READ,&f);
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
			return_code=1;
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
		if (j<file_list_length&&(_cli_flags&(SLL_CLI_FLAG_GENERATE_ASSEMBLY|SLL_CLI_FLAG_GENERATE_COMPILED_OBJECT))){
			sll_char_t buffer[SLL_API_MAX_FILE_PATH_LENGTH];
			sll_string_length_t k=0;
			sll_string_length_t file_path_length=sll_string_length(file_path);
			if (!output_file_path){
				sll_copy_data(file_path,file_path_length,buffer);
				k=file_path_length-1;
			}
			else{
				k=sll_string_length(output_file_path);
				sll_copy_data(output_file_path,k,buffer);
				if (_cli_flags&CLI_FLAG_SINGLE_OUTPUT){
					k-=1;
				}
				else{
					while (*(output_file_path+k)!='\\'&&*(output_file_path+k)!='/'){
						if (!k){
							k--;
							break;
						}
						k--;
					}
					k++;
					sll_string_length_t l=file_path_length;
					while (l&&*(file_path+l-1)!='\\'&&*(file_path+l-1)!='/'){
						l--;
					}
					sll_copy_data(file_path+l,file_path_length-l,buffer+k);
					k+=file_path_length-l-1;
				}
			}
			buffer[k+1]='.';
			buffer[k+2]='s';
			buffer[k+3]='l';
			buffer[k+5]=0;
			k+=4;
			if (_cli_flags&SLL_CLI_FLAG_GENERATE_ASSEMBLY){
				buffer[k]='a';
				sll_audit(SLL_CHAR("console_code_list_length.cli.save.assembly"),SLL_CHAR("S"),buffer);
				SLL_LOG("Writing assembly to file '%s'...",buffer);
				sll_file_t of;
				SLL_CRITICAL_ERROR(sll_file_open(buffer,SLL_FILE_FLAG_WRITE,&of));
				sll_write_assembly(&assembly_data,&of);
				SLL_LOG("File written successfully.");
				sll_file_close(&of);
			}
			if (_cli_flags&SLL_CLI_FLAG_GENERATE_COMPILED_OBJECT){
				buffer[k]='c';
				sll_audit(SLL_CHAR("console_code_list_length.cli.save.compiled"),SLL_CHAR("S"),buffer);
				SLL_LOG("Writing compiled program to file '%s'...",buffer);
				sll_file_t of;
				SLL_CRITICAL_ERROR(sll_file_open(buffer,SLL_FILE_FLAG_WRITE,&of));
				sll_write_compiled_node(&compilation_data,&of);
				SLL_LOG("File written successfully.");
				sll_file_close(&of);
			}
		}
		if (!(_cli_flags&SLL_CLI_FLAG_NO_RUN)){
			sll_vm_config_t config={
				CLI_VM_STACK_SIZE,
				CLI_VM_CALL_STACK_SIZE,
				&_cli_ift,
				sll_stdin,
				sll_stdout,
				sll_stderr
			};
			sll_file_flush(sll_stdout);
			sll_file_flush(sll_stderr);
			return_code=sll_execute_assembly(&assembly_data,&config);
			sll_file_flush(sll_stdout);
			sll_file_flush(sll_stderr);
		}
		sll_free_assembly_data(&assembly_data);
		if (j<file_list_length&&generated_type==SLL_LOOKUP_RESULT_COMPILED_OBJECT&&(_cli_flags&SLL_CLI_FLAG_GENERATE_BUNDLE)){
			sll_string_t b_f_nm;
			sll_string_from_pointer(argv[*(file_list+j)],&b_f_nm);
			sll_string_length_t off=((_cli_flags&SLL_CLI_FLAG_NO_PATHS)?sll_path_split(&b_f_nm):0);
			if (sll_string_ends(&b_f_nm,&_cli_slc_suffix)){
				sll_string_set_char(&b_f_nm,0,b_f_nm.length-_cli_slc_suffix.length);
			}
			SLL_LOG("Adding file '%s' as '%s' to bundle...",file_path,b_f_nm.data+off);
			sll_bundle_add_file(b_f_nm.data+off,&compilation_data,&bundle);
			sll_free_string(&b_f_nm);
		}
		else{
			sll_free_compilation_data(&compilation_data);
		}
	}
	if (_cli_flags&SLL_CLI_FLAG_GENERATE_BUNDLE){
		sll_char_t buffer[SLL_API_MAX_FILE_PATH_LENGTH];
		if (bundle_output_file_path){
			sll_string_length_t l=sll_string_length(bundle_output_file_path);
			if (l>SLL_API_MAX_FILE_PATH_LENGTH-1){
				l=SLL_API_MAX_FILE_PATH_LENGTH-1;
			}
			sll_copy_data(bundle_output_file_path,l,buffer);
			buffer[l]=0;
		}
		else{
			sll_string_length_t l=bundle.name.length;
			if (l>SLL_API_MAX_FILE_PATH_LENGTH-1){
				l=SLL_API_MAX_FILE_PATH_LENGTH-1;
			}
			sll_copy_data(bundle.name.data,l,buffer);
			buffer[l]=0;
		}
		if (!buffer[0]){
			SLL_WARN(SLL_CHAR("No bundle output path supplied"));
			_cli_had_warning=1;
			sll_free_bundle(&bundle);
			goto _cleanup;
		}
		SLL_LOG("Writing bundle to '%s'...",buffer);
		sll_file_t of;
		sll_file_open(buffer,SLL_FILE_FLAG_WRITE,&of);
		sll_write_bundle(&bundle,&of);
		SLL_LOG("File written successfully.");
		sll_file_close(&of);
		sll_free_bundle(&bundle);
	}
_cleanup:
	while (_cli_include_list_len){
		_cli_include_list_len--;
		include_directory=*(_cli_include_list+_cli_include_list_len);
		sll_free_string(&(include_directory->name));
		sll_free_string(&(include_directory->path));
		sll_deallocate(include_directory);
	}
	sll_deallocate(_cli_include_list);
	sll_audit(SLL_CHAR("console_code_list_length.cli.deinit"),SLL_CHAR(""));
	while (_cli_bundle_list_len){
		_cli_bundle_list_len--;
		cli_bundle_source_t* bundle_data=*(_cli_bundle_list+_cli_bundle_list_len);
		sll_deallocate(bundle_data->name);
		sll_free_bundle(&(bundle_data->bundle));
		sll_deallocate(bundle_data);
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
	sll_deallocate(file_list);
	sll_deallocate(console_code_list);
	sll_deallocate(audit_library_list);
	sll_deallocate(path_resolver_list);
	sll_free_internal_function_table(&_cli_ift);
	sll_deinit();
	_cli_resolver_table_size=0;
	return return_code;
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
