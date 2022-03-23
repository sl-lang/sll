# The Sl Programing Language Change Log

## [0.7.10] - Ongoing

### Added

- Implemented `sll_get_cpu_index` and `sll_get_thread_index`
- Implemented `SLL_IDENTIFIER_GET_STRING_INDEX`, `SLL_IDENTIFIER_IS_TLS`, `SLL_IDENTIFIER_UPDATE_STRING_INDEX`, `SLL_IDENTIFIER_SET_STRING_INDEX`, `sll_identifier_get_string_index`, `sll_identifier_is_tls`, `sll_identifier_set_string_index` and `sll_identifier_update_string_index` ([#254])
- Implemented `sll_new_object`, `sll_new_object_array` and `sll_new_object_list` ([#255])
- Implemented proper parsing of unknown escape sequences
- Operator parser is now generated from [`src/sll/data/operator_parser.txt`][0.7.10/src/sll/data/operator_parser.txt]
- String format that converts objects to string: `S`
- Thread-local variables can be marked with an exclamation mark (`!`) ([#254])
- Threads, semaphores, locks, barriers and environmental variables are now multithreading-compatible ([#243])

### Changed

- Fixed `sll_file_peek_char`
- *\[Windows Only\]* Fixed `sll_platform_set_cpu`
- Renamed `sll_identifier_create_identifier` to `sll_identifier_create`
- Reworked the assembly optimizer

## [0.7.9] - 2022-03-19

### Added

- Ability to execute functions asynchronously via `sll_execute_function`
- Allocator API: `sll_allocator_from_memory`, `sll_allocator_init`, `sll_allocator_move`, `sll_allocator_release` and `sll_allocator_resize` ([#224])
- Implemented `input.sll`, `input$char`, `input$int` and `input$string` ([#244])
- Implemented `sll_cpu_t`, `SLL_CPU_ANY`, `sll_platform_current_thread` and `sll_platform_set_cpu` ([#243])
- Implemented `sll_event_handle_t`, `sll_platform_event_create`, `sll_platform_event_delete`, `sll_platform_event_set` and `sll_platform_event_wait`
- Implemented `sll_lock_handle_t`, `sll_platform_lock_acquire`, `sll_platform_lock_create`, `sll_platform_lock_delete` and `sll_platform_lock_release`
- Implemented `sll_set_sandbox_flag_string` and `sys$set_sandbox_flags`
- Implemented `sll_string_to_object`, `sll_string_to_object_nocopy` and `sll_string_to_object_pointer`
- Object modification blocks should be enclosed by calls to `sll_lock_object` and `sll_unlock_object` to prevent race conditions
- Thread count limit: `SLL_SCHEDULER_MAX_THREADS`

### Changed

- Arrays and strings now use the allocator API ([#224])
- *\[Windows Only\]* Combined code for CLI console and window targets in the same file
- Files are now protected by locks to prevent racing conditions
- Fixed declaration of raw API functions
- Fixed invalid memory access bug in `sll_copy_data` and `sll_optimize_metadata`
- Objects types can no longer be changed and can only by initialized by a call to `sll_create_object`
- *\[Windows Only\]* Reduced the number of Windows headers included during compilation
- Rename `sll_sandbox_flags_t` to `sll_sandbox_flag_t`
- Reworked the thread and scheduler APIs ([#243])
- Sandbox flags are now thread-based and are inherited by child threads
- Split the [`src/sll/include/sll/_sll_internal.h`][0.7.8/src/sll/include/sll/_sll_internal.h] header into multiple headers located in [`src/sll/include/sll/_internal`][0.7.9/src/sll/include/sll/_internal]
- The maximum number of objects is now `2^24-1`

### Removed

- All constants references to objects (`const sll_object_t*`)
- CPU count getter function `sll_platform_get_cpu_count` (in favor of global constant `sll_platform_cpu_count`)
- Current thread index (`sll_current_thread_index`) from the public interface
- Unused function `sll_cli_main_raw`

## [0.7.8] - 2022-03-12

### Added

- All bundles from the `lib/` folder are automatically loaded ([#247])
- Asynchronous reading operations flag (read-only): `SLL_FILE_FLAG_ASYNC`
- Bundles can be added to the search path via the include (`-I` or `--include`) CLI option ([#245])
- CLI option to exclude full file paths from bundles: `-n` or `--names-only` ([#245])
- CLI option to generate bundles: `-b` or `--bundle` ([#245])
- CLI option to set bundle output file path: `-O` or `--bundle-output` ([#245])
- CLI option to specify a bundle name: `-N` or `--name-bundle` ([#245])
- Extra parameter in `file$read` to allow nonblocking I/O reading operations
- Functions related to asynchronous file reading I/O: `sll_platform_file_async_read`, `sll_platform_file_data_available` and `sll_file_data_available`
- Implemented `file$read_char`
- Implemented `random$choice`, `random$gauss`, `random$gauss2` and `random$triangular`
- Implemented `sll_bundle_add_file`, `sll_bundle_create`, `sll_bundle_fetch`, `sll_free_bundle`, `sll_load_bundle` and `sll_write_bundle` ([#245])
- Implemented `thread$internal_thread_data_type` and `thread$get_internal_data`
- Implemented `thread_type$suspended` and `thread$suspend`
- Internal I/O operation dispatcher thread ([#243])
- Raw strings (strings with no escapes)
- System thread API: `sll_internal_thread_index_t`, `SLL_UNKNOWN_INTERNAL_THREAD_INDEX`, `sll_platform_join_thread` and `sll_platform_start_thread` ([#243])

### Changed

- Cross-requesting call stacks is now enabled by an extra argument in `error$get_call_stack`
- Fixed internal initialization and deinitialization order
- Fixed memory overflow bugs in `sll_array_join_arrays` and `sll_string_select`
- Renamed barrier, lock, semaphore and thread functions
- Renamed platform source files from `linux_xxx.c` and `windows_xxx.c` to `xxx.c`
- Replaced VM global variables (`sll_current_instruction_count` and `sll_current_instruction_index`) with functions (`sll_vm_get_instruction_count` and `sll_thread_get_instruction_index`)
- Reversed the bundle search order ([#246])
- Split the [`src/sll/include/sll/platform.h`][0.7.7/src/sll/include/sll/platform.h] header into multiple headers located in [`src/sll/include/sll/platform`][0.7.8/src/sll/include/sll/platform]
- Standard input is now opened in non-blocking mode
- Storage of functions in the internal function table

### Removed

- Debug functions: `sll_add_debug_data`, `sll_remove_object_debug_data` and `sll_verify_object_stack_cleanup`
- Debugging data from objects
- Object macros: `SLL_ACQUIRE_NO_DEBUG`, `SLL_CREATE` and `SLL_RELEASE`
- Static object macros: `SLL_FROM_INT` and `SLL_FROM_FLOAT`
- Unused debug extension
- Unused function `sll_platform_enable_console_color` (Executed internally by `sll_init`)
- Unused macro `SLL_ACQUIRE_STATIC_CHAR`

## [0.7.7] - 2022-03-05

### Added

- C error API: `sll_error_t`, `SLL_ERROR_NO_FILE_PATH`, `SLL_ERROR_UNKNOWN_FD`, `SLL_ERROR_SANDBOX`, `SLL_ERROR_TOO_LONG`, `SLL_ERROR_FLAG_WINAPI`, `SLL_ERROR_FLAG_LIBC`, `SLL_ERROR_GET_TYPE`, `SLL_NO_ERROR` ([#238])
- Exposed the `hash_type` type from `hash.sll`
- Implemented `float$float_data_type`, `float$get_data` and `float$set_data`
- Implemented `hmac.sll` and `hmac$hmac` ([#230])
- Implemented `random$shuffle`
- Implemented `sll_instruction_to_location`
- Implemented `sll_remove_debug_names` and CLI flag to remove debug names (`-d`)
- Implemented `sll_var_arg_get_object`
- Implemented `sll_version_string`
- Library verification parameter can be passed to `sys$load_library` to verify the SHA-256 of the loaded file
- Name-of operator (`(.?)`): `sll_get_name`
- New error types: `error$ERROR_ANY`, `error$ERROR_INTERNAL_ERROR`, `error$ERROR_INVALID_FILE_DESCRIPTOR`, `error$ERROR_PATH_TOO_LONG` and `error$ERROR_SANDBOX` ([#238])
- Path size limit: `path$MAX_PATH_SIZE`
- System error codes: `error$from_internal`, `error_codes.sll`, `error_codes$ERROR_INTERNAL_ERROR`, `error_codes$LIBC_ERROR_CODES`, `error_codes$WINDOWS_ERROR_CODES` and `error_codes$get_by_name` ([#238])
- Type name-of operator (`(&:?)`): `sll_get_type_name`
- Variables evaluating to the current file (`@@file@@`) and line number (`@@line@@`)

### Changed

- Adjusted platform and file function to allow for error return values ([#238])
- Fixed bit-shifting logic in `sll_load_assembly` and `sll_write_assembly`
- Fixed bugs in `hash$xxx$update` and `hash$xxx$digest` functions
- Fixed critical error in file hash calcultion
- *\[Darwin only\]* Fixed `SLL_API_MAX_FILE_PATH_LENGTH`
- Fixed `sll_object_field_t` to be a union instead of a structure
- Getting the file size via `sll_platform_file_size` now returns `SLL_NO_FILE_SIZE` instead of `0` on error
- Libraries loaded via `sys$load_library` can no longer be located outside of the `sys_lib` directory
- Moved call stack types and functions (`call_stack_type` and `get_call_stack`) from the extension to the `error.sll` module
- Moved VM-related types and functions (`location_type`, `vm_config_type`, `get_config`, `get_instruction_count`, `get_instruction_index`, `get_location` and `get_ref_count`) from the extension to the `vm.sll` module
- Renamed `sll_platform_lookup_function` to `sll_platform_lookup_symbol`
- Renamed API source files from `xxx_api.c` to `xxx.c`
- Replaced `sll_operator_deep_copy` with an extra argument in `sll_operator_copy`
- Replaced `sll_platform_get_temporary_file_path` by `sll_temporary_file_path`

### Removed

- Ability to generate Sll code: `sll_write_sll_code`, `--generate-sll` and `-F`
- Final call stack frame from `error$create` and `error$from_internal`
- Sll type exporting to C code in extensions
- Unused functions `sll_add_string_object` and `sll_api_error_get_backtrace`
- Unused object flags: `SLL_OBJECT_FLAG_RESERVED0` (`OBJECT_CHANGE_IN_LOOP`) and `SLL_OBJECT_FLAG_RESERVED1` (`OBJECT_EXTERNAL_STRING`)
- Unused object type: `SLL_OBJECT_TYPE_RESERVED0` (`OBJECT_TYPE_FUNCTION_ID`)

## [0.7.6] - 2022-02-25

### Added

- Argument `no_flush_io` in `process$shell` and `process$start` to prevent flushing of `file$stdout` and `file$stderr` before the creation of a process
- Barriers: `sll_create_barrier`, `sll_delete_barrier`, `sll_increase_barrier`, `sll_reset_barrier`, `thread$barrier_type`, `thread$STATE_WAITING_BARRIER`, `thread$create_barrier`, `thread$if_equal`, `thread$if_greater_equal`, `thread$increase_barrier` and `thread$release_barrier` ([#237])
- Constant string representing the current executable (`sll_executable_file_path`) and the Sll library (`sll_library_file_path`) file paths
- Field for structure data size without padding: `struct_type$data_size`
- Full MacOS support
- More tests ([#8])
- New error types: `error$ERROR_INVALID_BASE64_CHARACTER` and `error$ERROR_INVALID_BASE64_PADDING`
- Semaphores: `sll_semaphore_counter_t`, `sll_semaphore_index_t`, `sll_create_semaphore`, `sll_delete_semaphore`, `sll_release_semaphore`, `thread$semaphore_type`, `thread$STATE_WAITING_SEMAPHORE`, `thread$acquire_semaphore`, `thread$create_semaphore` and `thread$release_semaphore` ([#237])

### Changed

- Control/Status Register has the `DAZ` and `FTZ` flags automatically set
- Fixed `sll_platform_path_copy`
- Moved all `ERROR_xxx` constants to `error.sll`
- Program crashes now print the Sll backtrace (if available)
- Reworked the error API

### Removed

- Executable and library getter function: `sll_platform_get_executable_file_path` and `sll_platform_get_library_file_path` (in favor of global strings)

## [0.7.5] - 2022-02-20

### Added

- Deep copy operator: `(:!)` ([#233])
- Exposed `sll_current_thread_index`, `sll_create_lock`, `sll_create_thread`, `sll_delete_lock`, `sll_delete_thread`, `sll_release_lock` and `sll_start_thread` ([#173] and [#177])
- Extra parameter in `sll_object_clone` to enable deep value copy
- *\[Windows only\]* Icons to CLI executables
- Implemented `data_file.sll` ([#235])
- Implemented `sll_api_error_get_backtrace`, `error.sll`, `error$error_type` and `error$create` ([#196])
- Implemented `sll_api_struct_float_from_bits`, `sll_api_struct_double_from_bits` and `struct$decode` ([#227] and [#228])
- Implemented `sll_api_struct_float_to_bits`, `sll_api_struct_double_to_bits` and `struct$encode` ([#227] and [#229])
- Implemented `sll_operator_deep_copy` ([#233])
- Implemented `sll_platform_create_directory` and `path$mkdir`
- Implemented `SLL_SANDBOX_FLAG_ENABLE_FILE_DELETE`, `sll_platform_path_delete` and `file$delete`
- Implemented `struct.sll`, `struct$ERROR_OUT_OF_RANGE`, `struct$ERROR_UNKNOWN_TYPE` and `struct$create` ([#227])
- MacOS support (alpha stage)
- String resize function: `sll_string_resize`

### Changed

- Builder script no longer recompiles unchanged files (works across configurations)
- Calling `sll_file_flush` on files with empty buffers no longer generates system calls
- Factorizing a negative number via `math$factors` correctly returns a factor of `-1` raised to the 1st power
- Fixed another bug in `sll_string_compare_pointer`
- Fixed edge cases in `sll_string_duplicate`
- Improved performance of the scheduler
- Renamed `sll_operator_dup` to `sll_operator_copy`
- Split `SLL_ACQUIRE_STATIC_INT` macro to `SLL_ACQUIRE_STATIC_INT` and `SLL_ACQUIRE_STATIC_NEG_INT`
- Thread objects are now recycled instead of released immediately to the OS
- Updated CLI help string
- VM stack size now specifies the number of Sll objects instead of the raw memory size

### Removed

- Extension version macros

## [0.7.4] - 2022-02-15

### Added

- Assembly instruction to reserve space on the stack: `SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_STACK` ([#26])
- Implemented `sll_api_thread_create` ([#173] and [#174])
- Implemented `sll_get_call_stack` ([#173])
- Implemented `thread$lock_type`, `thread$acquire_lock`, `thread$create_lock` and `thread$release_lock` ([#173] and [#175])
- Implemented `thread$thread_type`, `thread$STATE_RUNNING`, `thread$STATE_WAITING`, `thread$STATE_TERMINATED`, `thread$current`, `thread$get`, `thread$join` and `thread$start` ([#173] and [#175])
- Implemented `thread.sll` ([#173] and [#175])
- Multithreading support ([#173] and [#177])
- Tests can be executed by running `sll tests/_runner.sll` or by passing the `--test` flag to the builder script ([#8])
- Thread waiting operator (`(!<<)`) and current thread identification getter (`(!.)`) ([#173] and [#175])
- Variable argument function call operator (`(<-*)`) ([#232])

### Changed

- Array functions now use `sll_operator_strict_equal` instead of `sll_operator_equal`
- Fixed `sll_process_join_args`
- Fixed incorrect results returned by `sll_string_compare_pointer`
- Fixed memory leaks related to unreleased objects (invalidated [#25])
- Fixed multiple inconsistencies with different operators
- Fixed reference counting and double-free in `file.sll`
- Fixed variable names related to compilation data and source files
- Function `time$time_function` now accepts arguments passed to the timed function
- Function variables are now stack based and not register-based ([#26])
- If (`(?)`), inline if (`(?:)`) and switch (`(??)`) operators now assign new scopes for every condition and code block pair
- JSON API functions now encode and decode the 4-character escape (`\u00{N}{N}`) instead of the invalid 2-character escape (`\x{N}{N}`)
- *\[Windows only\]* Separate executables for console mode (`sll.exe`) and window mode (`sllw.exe`) are now generated
- Variable arguments are now properly handled in `file$write`
- VM now stores execution data based on threads ([#173] and [#177])

### Removed

- Console creation function (`sll_platform_create_console`) and CLI option to suppress window creation
- Console initialization functions: `sll_platform_setup_console` and `sll_platform_reset_console`
- Memory fail text is no longer present in the exported header file
- Recursive calls to `sll_execute_assembly` are no longer allowed

## [0.7.3] - 2022-02-09

### Added

- Base memory allocation function (`sll_allocate`, `sll_allocate_stack`, `sll_reallocate`, `sll_zero_allocate` and `sll_zero_allocate_stack`) raise critical errors when memory requests cannot be fulfilled ([#83])
- C hashing API: `sll_md5_data_t`, `sll_sha1_data_t`, `sll_sha256_data_t`, `sll_sha512_data_t`, `SLL_INIT_MD5`, `SLL_INIT_SHA1`, `SLL_INIT_SHA256`, `SLL_INIT_SHA512`, `SLL_INIT_MD5_STRUCT`, `SLL_INIT_SHA1_STRUCT`, `SLL_INIT_SHA256_STRUCT`, `SLL_INIT_SHA512_STRUCT`, `sll_hash_md5`, `sll_hash_sha1`, `sll_hash_sha256` and `sll_hash_sha512`
- CLI loader now checks the internal library version via a `sll_version` call (i.e. renaming the dynamic library will not change the version)
- Implemented `float.sll`, `float$get_compare_error`, `float$set_compare_error` and `SLL_SANDBOX_FLAG_DISABLE_FLOAT_COMPARE_ERROR_CHANGE`
- Implemented `sll_free_source_file` and `sll_unify_compilation_data` ([#55])
- Implemented `sll_path_split_drive` and `path$split_drive`
- Implemented `sll_time_zone_t`, `sll_utc_time_zone`, `sll_platform_time_zone` and `date$LOCAL_TIME_ZONE`
- Rewritten large part of the codebase to execute modules only once ([#55])

### Changed

- Compiled modules no longer contain full file paths from the host file system
- Creating a file from an invalid file descriptor (`SLL_UNKNOWN_FILE_DESCRIPTOR`) with `sll_file_open_descriptor` now creates a null file
- Date functions (`date$current`, `date$from_time` and `date$from_time_ns`) now default to the local time zone (`date$LOCAL_TIME_ZONE`) instead of UTC (`date$UTC_TIME_ZONE`)
- Date functions (`sll_date_from_time` and `sll_date_from_time_ns`) now expect a time zone argument
- File creation functions (`sll_file_from_data`, `sll_file_open` and `sll_file_open_descriptor`) now remove invalid file flags
- File path table now stores file hashes and lengths (`sll_file_path_t` instead `sll_string_length_t`) ([#55])
- Fixed `sll_compare_data`
- Fixed variable function calling via `sll_execute_function`
- Removing node padding via `sll_remove_node_padding` no longer requires a node argument
- Renamed `sll_allocate`, `sll_allocate_stack`, `sll_reallocate`, `sll_zero_allocate` and `sll_zero_allocate_stack` to `sll_allocate_fail`, `sll_allocate_stack_fail`, `sll_reallocate_fail`, `sll_zero_allocate_fail` and `sll_zero_allocate_stack_fail` ([#83])
- Renamed `sll_parse_all_nodes` to `sll_parse_nodes`
- Rewritten CLI code

### Removed

- Array functions no longer return value based on memory allocation functions ([#83])
- Single node reading and writing functions: `sll_load_node` and `sll_write_node`
- Unused file APIs: `SLL_FILE_GET_LINE_OFFSET` and `sll_file_reset_line`

## [0.7.2] - 2022-02-04

### Added

- Implemented `file$FLAG_NO_BUFFER`
- Implemented `SLL_ASSEMBLY_FUNCTION_GET_ARGUMENT_COUNT`, `SLL_ASSEMBLY_FUNCTION_IS_VAR_ARG`, `SLL_FUNCTION_GET_ARGUMENT_COUNT`, `SLL_FUNCTION_GET_ARGUMENT_COUNT_RAW`, `SLL_FUNCTION_IS_VAR_ARG` and variable argument functions (`@@last-argument-name@@`)
- Implemented `sll_object_to_array` and casting between custom types
- New sandbox flags: `SLL_SANDBOX_FLAG_DISABLE_ENVIRONMENT`, `SLL_SANDBOX_FLAG_DISABLE_LOAD_LIBRARY`, `SLL_SANDBOX_FLAG_ENABLE_BUFFER_FILES`, `SLL_SANDBOX_FLAG_ENABLE_FILE_RENAME` and `SLL_SANDBOX_FLAG_ENABLE_FILE_COPY`

### Changed

- Fixed constant string modification
- Flags returned by `sll_get_sandbox_flags` and `sys$get_sandbox_flags` are a list of strings
- Incorrect return code when printing help or version in the CLI
- Renamed `sll_set_sandbox_flags` to `sll_set_sandbox_flag`
- Setting a sandbox flag via `sll_set_sandbox_flag` no longer returns the current flags

### Removed

- Call to an internal initializer function in `process.sll`
- Internal sandbox flag values: `FLAG_DISABLE_FILE_IO`, `FLAG_ENABLE_STDIN_IO`, `FLAG_ENABLE_STDOUT_IO`, `FLAG_DISABLE_PATH_API` and `FLAG_DISABLE_PROCESS_API`
- Unused `sort$default_sort_func`

## [0.7.1] - 2022-01-29

### Added

- Implemented `args$add_option` ([#179] and [#180])
- Implemented `args$parse` ([#179] and [#181])
- Implemented `args.sll` ([#179])
- Implemented `time$time_function` ([#195])
- Object type support: `SLL_OBJECT_TYPE_OBJECT` and `object_type`

### Changed

- Custom ELF sections have been renamed from `s_str` and `s_obj` to `.initstr` and `.sobject`
- Page splitting assembly (`SLL_ASSEMBLY_INSTRUCTION_TYPE_CHANGE_STACK`) and node (`SLL_NODE_TYPE_CHANGE_STACK`) types are no longer internal
- Recursive calls to `sll_execute_assembly` no longer corrupt internal data
- Renamed everything related to `posix` to `linux` (to allow future transition into MacOS support)
- `sll_add_debug_data` now uses `const sll_char_t*` instead of `const char*`

### Removed

- Broken optimizer
- Internal function flags: `SLL_INTERNAL_FUNCTION_FLAG_REQUIRED`, `SLL_INTERNAL_FUNCTION_FLAG_COMPILATION_CALL` and `sll_internal_function_type_t`
- Internal function table name length limit: `SLL_INTERNAL_FUNCTION_MAX_LENGTH`
- Various code duplications

## [0.7.0] - 2022-01-21

### Added

- Console option to do not create a console: `-w` (Windows only)
- Implemented `date$from_time_ns`
- Implemented `debug$backtrace`
- Implemented `serial$decode_float`, `serial$decode_int`, `serial$decode_unsigned_int`, `serial$encode_float`, `serial$encode_int` and `serial$encode_unsigned_int`
- Implemented `serial.sll` ([#199])
- Implemented `sll_date_from_time_ns`
- Implemented `sll_decode_integer`, `sll_decode_signed_integer`, `sll_encode_integer` and `sll_encode_signed_integer`
- Implemented `sll_decode_object` and `serial$decode` ([#199] and [#200])
- Implemented `sll_decode_string` and `serial$decode_string` ([#199] and [#201])
- Implemented `sll_encode_object` and `serial$encode` ([#199] and [#202])
- Implemented `sll_encode_string` and `serial$encode_string` ([#199] and [#203])
- Implemented `sll_file_from_handle`
- Implemented `sll_file_get_buffer` and `file$get_buffer`
- Implemented `sll_platform_create_console` and `sll_cli_main_raw`
- Implemented `sll_platform_path_copy` and `file$copy` ([#192])
- Implemented `sll_platform_path_rename` and `file$rename` ([#192])
- Implemented `sys$version_type$commit`, `sys$version_type$full_commit`, `sys$version_type$url` and `sys$version_type$time`

### Changed

- Build script now removes symbols from the generated executables
- Changed internal executable section names
- Increased `sll_file_flags_t` to 16 bits
- Moved `SLL_COPY_STRING_NULL`, `SLL_COMPARE_RESULT_BELOW`, `SLL_COMPARE_RESULT_EQUAL`, `SLL_COMPARE_RESULT_ABOVE` types, `sll_compare_data`, `sll_copy_data`, `sll_copy_string`, `sll_copy_string_null`, `sll_set_memory` and `sll_zero_memory` from [`src/sll/util.c`][0.6.37/src/sll/util.c] and [`src/sll/include/sll/util.h`][0.6.37/src/sll/include/sll/util.h] to [`src/sll/data.c`][0.7.0/src/sll/data.c] and [`src/sll/include/sll/data.h`][0.7.0/src/sll/include/sll/data.h]
- Moved `SLL_DEBUG_LINE_DATA_GET_DATA`, `SLL_DEBUG_LINE_DATA_FLAG_FILE` and `SLL_DEBUG_LINE_DATA_FLAG_FUNC` from [`src/sll/include/sll/assembly.h`][0.6.37/src/sll/include/sll/assembly.h] to [`src/sll/location.c`][0.7.0/src/sll/location.c] and [`src/sll/include/sll/location.h`][0.7.0/src/sll/include/sll/location.h]
- Moved `sll_deinit`, `sll_init` and `sll_register_cleanup` from [`src/sll/util.c`][0.6.37/src/sll/util.c] and [`src/sll/include/sll/util.h`][0.6.37/src/sll/include/sll/util.h] to [`src/sll/init.c`][0.7.0/src/sll/init.c] and [`src/sll/include/sll/init.h`][0.7.0/src/sll/include/sll/init.h]
- Moved `sll_get_environment_variable`, `sll_remove_environment_variable` and `sll_set_environment_variable` from [`src/sll/util.c`][0.6.37/src/sll/util.c] and [`src/sll/include/sll/util.h`][0.6.37/src/sll/include/sll/util.h] to [`src/sll/env.c`][0.7.0/src/sll/env.c] and [`src/sll/include/sll/env.h`][0.7.0/src/sll/include/sll/env.h]
- Moved `sll_get_location` from [`src/sll/util.c`][0.6.37/src/sll/util.c] and [`src/sll/include/sll/util.h`][0.6.37/src/sll/include/sll/util.h] to [`src/sll/location.c`][0.7.0/src/sll/location.c] and [`src/sll/include/sll/location.h`][0.7.0/src/sll/include/sll/location.h]
- Moved `SLL_SANDBOX_FLAG_xxx` flags, `sll_get_sandbox_flag`, `sll_get_sandbox_flags` and `sll_set_sandbox_flags` from [`src/sll/util.c`][0.6.37/src/sll/util.c] and [`src/sll/include/sll/util.h`][0.6.37/src/sll/include/sll/util.h] to [`src/sll/sandbox.c`][0.7.0/src/sll/sandbox.c] and [`src/sll/include/sll/sandbox.h`][0.7.0/src/sll/include/sll/sandbox.h]
- Moved `sll_version` from [`src/sll/util.c`][0.6.37/src/sll/util.c] and [`src/sll/include/sll/util.h`][0.6.37/src/sll/include/sll/util.h] to [`src/sll/version.c`][0.7.0/src/sll/version.c] and [`src/sll/include/sll/version.h`][0.7.0/src/sll/include/sll/version.h]
- Moved generated header files to the [`src/sll/include/sll/generated`][0.7.0/src/sll/include/sll/generated] directory
- Platform identifier string (`sll_platform_string`) is now a `const sll_string_t*` instead of `const sll_char_t*`
- Renamed [`src/sll/api/_generated.c`][0.6.37/src/sll/api/_generated.c] and [`src/sll/include/sll/api/_generated.h`][0.6.37/src/sll/include/sll/api/_generated.h] to [`src/sll/api/_generated_raw.c`][0.7.0/src/sll/api/_generated_raw.c] and [`src/sll/include/sll/generated/api.h`][0.7.0/src/sll/include/sll/generated/api.h]
- Renamed [`src/sll/util.c`][0.6.37/src/sll/util.c] and [`src/sll/include/sll/util.h`][0.6.37/src/sll/include/sll/util.h] to [`src/sll/string_table.c`][0.7.0/src/sll/string_table.c] and [`src/sll/include/sll/string_table.h`][0.7.0/src/sll/include/sll/string_table.h]
- Renamed `sll_api_time_current_nanos`, `sll_api_time_sleep_nanos`, `time$time_nanos` and `time$sleep_nanos` to `sll_api_time_current_ns`, `sll_api_time_sleep_ns`, `time$time_ns` and `time$sleep_ns`
- Split [`src/sll/node.c`][0.6.37/src/sll/node.c] into [`src/sll/compilation_data.c`][0.7.0/src/sll/compilation_data.c], [`src/sll/debug_data.c`][0.7.0/src/sll/debug_data.c], [`src/sll/skip.c`][0.7.0/src/sll/skip.c] and [`src/sll/size.c`][0.7.0/src/sll/size.c]
- Static strings are now initialized after the call to `sll_init` function
- Time version macro (`SLL_VERSION_BUILD_TIME`) now specifies nanoseconds instead of a string (can be converted by `sll_date_from_time_ns`)
- Updated CLI code

### Removed

- All `stdint.h` typedefs and macros (replaced by [`src/sll/include/sll/_size_types.h`][0.7.0/src/sll/include/sll/_size_types.h])
- Date version macro: `SLL_VERSION_BUILD_DATE`

## [0.6.37] - 2022-01-15

### Added

- Casting custom types to strings
- Environment variable API: `sll_environment_t`, `sll_environment_variable_t`, `sll_environment`, `sll_platform_remove_environment_variable`, `sll_platform_set_environment_variable`, `sll_get_environment_variable`, `sll_remove_environment_variable`, `sll_set_environment_variable`, `sys$ENVIRONMENT`, `sys$remove_env` and `sys$set_env`
- Error return code in `sll_file_from_data` ([#83])
- Euler's totient/phi function: `sll_math_euler_phi` and `math$euler_phi`
- Extension to the sandbox API: `sll_get_sandbox_flags`, `sys$get_sandbox_flags`, `sys$FLAG_DISABLE_FILE_IO`, `sys$FLAG_ENABLE_STDIN_IO`, `sys$FLAG_ENABLE_STDOUT_IO`, `sys$FLAG_DISABLE_PATH_API` and `sys$FLAG_DISABLE_PROCESS_API`
- Extra function in the file API: `file$TEMP_FILE_PATH`, `file$from_data` and `file$peek` ([#192])
- Floating-point number operation functions: `sll_math_mod`
- Implemented `sll:array_pop` ([#87] and [#215])
- Implemented `sll:array_push` ([#87] and [#118])
- Implemented `sll:array_remove` ([#87] and [#121])
- Implemented `sll:array_shift` ([#87] and [#213])
- Implemented `sll:array_unshift` ([#87] and [#214])
- Implemented `sll_array_extend` and `sll:array_extend` ([#87] and [#119])
- Implemented `sll_array_join_arrays` and `sll:array_join` ([#87] and [#115])
- Implemented `sll_array_split` and `sll:array_split` ([#87] and [#122])
- Implemented `sll_platform_get_temporary_file_path` ([#192])
- Implemented `sll_string_pad` and `sll:string_pad` ([#87] and [#110])
- Implemented `sll_string_secure_equal` and `string$secure_equal`
- Implemented `sll_string_trim_left` and `sll:string_trim_left` ([#87] and [#108])
- Implemented `sll_string_trim_right` and `sll:string_trim_right` ([#87] and [#109])
- Implemented `sll_string_trim` and `sll:string_trim` ([#87] and [#107])
- Implemented `uuid$from_bytes` ([#193] and [#218])
- Implemented `uuid$NIL` ([#193] and [#217])
- Implemented `uuid$to_bytes` ([#193] and [#219])
- Implemented `uuid$uuid3` ([#193] and [#220])
- Implemented `uuid$uuid4` ([#193] and [#221])
- Implemented `uuid$uuid5` ([#193] and [#222])
- Implemented `uuid$uuid_type` ([#193] and [#216])
- Implemented `uuid.sll` ([#193])
- Integer to decimal string conversion: `int$to_dec`
- Inversion flag to `sll_string_index_char`, `sll_string_index_multiple`, `sll_string_index_reverse_char` and `sll_string_index_reverse_multiple` ([#108] and [#109])
- New sandbox flag: `SLL_SANDBOX_FLAG_DISABLE_PROCESS_API`
- Option macro to enable experimental stack allocator (`USE_STACK_ALLOCATOR`)
- Prime factorization: `sll_factor_t`, `sll_math_factors`, `math$factor_type` and `math$factors`
- Sll map module: `map$extend` and `map$remove`
- Special structure field: `@@string@@`
- Type-to-string in custom types: `date$time_zone_type`, `date$date_type`, `file$file_type`, `json$json_null`, `json$json_true`, `json$json_false`, `math$factor_type`, `process$process_config_type`, `process$stream_data_type`, `sys$VERSION` and `uuid$uuid_type`

### Changed

- Fixed integer overflow when parsing floats
- Fixed integer-to-string conversion in `sll_string_format` and `sll_string_format_list`
- In-place operations are now supported by `sll_map_remove`
- In-place operations are now supported in some array functions: `sll_array_pop`, `sll_array_push`, `sll_array_remove`, `sll_array_shift` and `sll_array_unshift` ([#87], [#118], [#121], [#213], [#214] and [#215])
- Map functions now use `sll_operator_strict_equal` instead of `sll_operator_equal`
- Moved file argument from `sll_compilation_data_t` to `sll_parse_all_nodes`
- `sll_file_from_data` now internally duplicates the input buffer
- `time_zone_type$time_offset` is now an integer offset in minutes

### Removed

- Builtin header included files: `stddef.h` and `math.h`
- Copy declaration node and assembly types
- Deprecated deinitialization function: `sll_free_string_table`
- File argument from `sll_init_compilation_data`
- Object type initializer API
- Old operator number macros: `SLL_FLOAT_ABSOLUTE` and `SLL_FLOAT_ROUND`
- Unused string functions: `sll_string_subtract_array` and `sll_string_subtract_map`

## [0.6.36] - 2022-01-09

### Added

- Array generator operators: `([>)` and `([<)` ([#206])
- Assembly membership test instructions: `SLL_ASSEMBLY_INSTRUCTION_TYPE_JI` and `SLL_ASSEMBLY_INSTRUCTION_TYPE_JNI` ([#207])
- Call stack size member in the `sll_vm_config_t` structure
- CPU count constant in `sys.sll`: `sll_platform_get_cpu_count` and `sys$CPU_COUNT` ([#197])
- Extension to the logging API: `SLL_WARN`, `SLL_LOG_FLAG_SHOW` and `SLL_LOG_FLAG_NO_HEADER`
- File listing example
- Implemented `random.sll` module and `sll_platform_random` ([#208])
- Implemented `sll_api_random_get_char` and `random$get_char` ([#211])
- Implemented `sll_api_random_get_float` and `random$get_float` ([#210])
- Implemented `sll_api_random_get_int` and `random$get_int` ([#209])
- Implemented `sll_api_random_get_string` and `random$get_string` ([#212])
- Map generator operators: `({>)` and `({<)` ([#206])
- Membership test functions: `sll_string_includes`, `sll_string_includes_char`, `sll_array_includes` and `sll_map_includes` ([#207])
- Membership test operator: `(|:)` ([#207])
- New CLI code to dynamically find the shared library
- New operator: `sll_operator_includes` ([#207])
- Sll-from-C function calling to the VM: `sll_execute_function` ([#176])
- Support for special structure fields: `@@copy@@`, `@@delete@@` and `@@init@@` ([#205])

### Changed

- *\[POSIX only\]* After calling `sll_init` the library load flags are automatically fixed
- All of the `SLL_ASSEMBLY_INSTRUCTION_TYPE_ASSIGN_XXX` instruction now support the in-place flag instead of the `_VAR` suffix
- Call stack is no longer a pointer in the `sll_runtime_data_t` structure
- Moved CLI code from [`src/cli/main.c`][0.6.35/src/cli/main.c] to [`src/sll/cli.c`][0.6.36/src/sll/cli.c]
- Reworked custom type API
- *\[POSIX only\]* Shared libraries are now loaded with `RTLD_NOW` instead of `RTLD_LAZY`
- `sll_quicksort` (and `sort$sort`) now support a `fn` function argument
- `sort$sort` also includes an in-place boolean flag argument

### Removed

- Binary heap API
- CLI option to enable color (`-C`)
- End/Exit (`(@@@)`) node type (replaced by a return from module-level)
- Leftover URL-related functions and type definitions
- Macro constant: `SLL_CALL_STACK_SIZE`
- Return value of `sll_parse_all_nodes` and `sll_generate_assembly`
- VM error codes: `SLL_VM_INVALID_INSTRUCTION_INDEX` and `SLL_VM_INVALID_STACK_INDEX`

## [0.6.35] - 2022-01-03

### Added

- Character constants in `string.sll`: `string$BINARY_DIGITS`, `string$DIGITS`, `string$HEXADECIMAL_DIGITS`, `string$LETTERS`, `string$LOWERCASE_LETTERS`, `string$OCTAL_DIGITS`, `string$PRINTABLE_CHARACTERS`, `string$PUNCTUATION_CHARACTERS`, `string$UPPERCASE_LETTERS` and `string$WHITESPACE_CHARACTERS` ([#190])
- Implemented `base64$ALPHABET` ([#169] and [#170])
- Implemented `base64.sll` ([#169])
- Implemented `math$abs`, `math$ceil`, `math$floor` and `math$round` ([#182] and [#185])
- Implemented `math$cbrt`, `math$int_pow`, `math$int_sqrt`, `math$pow` and `math$sqrt` ([#182] and [#189])
- Implemented `math$combinations`, `math$factorial`, `math$gcd` and `math$permutations` ([#182] and [#186])
- Implemented `math$copy_sign` and `math$sign` ([#182] and [#187])
- Implemented `math$log`, `math$int_log2`, `math$log2` and `math$log10` ([#182] and [#188])
- Implemented `math.sll` ([#182])
- Implemented `sll_api_base64_decode` and `base64$decode` ([#169] and [#171])
- Implemented `sll_api_base64_encode` and `base64$encode` ([#169] and [#172])
- Implemented `sll_api_json_stringify` and `json$stringify` ([#58])
- Implemented `sll_clone_internal_function_table`
- Integer to string conversion module: `int.sll` ([#191])
- Internal function table field in the VM config structure
- Math constants ([#182] and [#183])
- Math trigonometric functions ([#182] and [#184])
- Module for looking-up MIME types: `mime.sll` ([#194])
- Version tag macro constant: `SLL_VERSION_TAG`

### Change

- Moved `sll_operator_cast` from [`src/sll/operator.c`][0.6.34/src/sll/operator.c] to [`src/sll/cast.c`][0.6.35/src/sll/cast.c]
- Renamed `sll_import_loader_t` to `sll_import_resolver_t`

### Removed

- Error API

## [0.6.34] - 2021-12-30

### Added

- Implemented `and` (`(&&)`) and `or` (`(||)`) operators ([#168])
- Implemented `hash.sll` and `hash$hexdigest` ([#23])
- Implemented `sll_api_hash_md5`, `hash$md5$BLOCK_SIZE`, `hash$md5$DIGEST_SIZE`, `hash$md5$init`, `hash$md5$update`, `hash$md5$digest` and `hash$md5$process` ([#23] and [#163])
- Implemented `sll_api_hash_sha1`, `hash$sha1$BLOCK_SIZE`, `hash$sha1$DIGEST_SIZE`, `hash$sha1$init`, `hash$sha1$update`, `hash$sha1$digest` and `hash$sha1$process` ([#23] and [#164])
- Implemented `sll_api_hash_sha224`, `hash$sha224$BLOCK_SIZE`, `hash$sha224$DIGEST_SIZE`, `hash$sha224$init`, `hash$sha224$update`, `hash$sha224$digest` and `hash$sha224$process` ([#23] and [#165])
- Implemented `sll_api_hash_sha256`, `hash$sha256$BLOCK_SIZE`, `hash$sha256$DIGEST_SIZE`, `hash$sha256$init`, `hash$sha256$update`, `hash$sha256$digest` and `hash$sha256$process` ([#23] and [#165])
- Implemented `sll_api_hash_sha384`, `hash$sha384$BLOCK_SIZE`, `hash$sha384$DIGEST_SIZE`, `hash$sha384$init`, `hash$sha384$update`, `hash$sha384$digest` and `hash$sha384$process` ([#23] and [#166])
- Implemented `sll_api_hash_sha512`, `hash$sha512$BLOCK_SIZE`, `hash$sha512$DIGEST_SIZE`, `hash$sha512$init`, `hash$sha512$update`, `hash$sha512$digest` and `hash$sha512$process` ([#23] and [#166])
- Implemented `sll_api_string_checksum` and `string$checksum`
- Implemented `sll_file_peek_char`
- Implemented `sll_process_join_args`, `process$split` and `process$join` ([#161])
- Replaced jumping to jump with the target of the second jump
- VM error return value constants: `SLL_VM_INVALID_INSTRUCTION_INDEX` and `SLL_VM_INVALID_STACK_INDEX`

### Changed

- Assembly optimizer code is now auto-generated from a file ([`src/sll/data/assembly_optimizer.txt`][0.6.34/src/sll/data/assembly_optimizer.txt]) ([#167])
- File & JSON APIs now uses custom types instead of handles ([#162])
- Fixed `sll_string_duplicate` and `sll_string_select`
- Fixed multiple optimizer and assembly generator bugs
- Moved CLI help file from [`rsrc/help.txt`][0.6.33/rsrc/help.txt] to [`src/cli/data/help.txt`][0.6.34/src/cli/data/help.txt]
- Renamed `sll_api_sort_sort` to `sll_api_sort_quicksort`
- Renamed `sll_api_sys_arg_get` to `sll_api_sys_get_args`
- Repository file layout
- `sys$argv` is now an array instead of a function

### Removed

- Error argument from `sll_execute_assembly` and `sll_import_loader_t`
- Error types: `SLL_ERROR_UNKNOWN`, `SLL_ERROR_UNEXPECTED_CHARACTER`, `SLL_ERROR_TOO_MANY_ARGUMENTS`, `SLL_ERROR_ARRAY_TOO_LONG`, `SLL_ERROR_MAP_TOO_LONG`, `SLL_ERROR_UNMATCHED_OPEN_QUOTE`, `SLL_ERROR_UNKNOWN_IDENTIFIER_CHARACTER`, `SLL_ERROR_UNKNOWN_IDENTIFIER`, `SLL_ERROR_UNMATCHED_QUOTES`, `SLL_ERROR_INVALID_FILE_FORMAT`, `SLL_ERROR_INVALID_INSTRUCTION`, `SLL_ERROR_INVALID_INSTRUCTION_INDEX` and `SLL_ERROR_INVALID_STACK_INDEX`
- Handle API ([#162])
- Old hash API
- Optional arguments from internal functions
- `SLL_FROM_FILE`, `SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_NULL`, `SLL_MAX_COMPILATION_ERROR` and `SLL_NO_CHAR`
- `sll_path_absolute` (replaced by `sll_platform_absolute_path`)
- `sys_arg_get_count` and `sys$argc`
- Test script
- Unneeded function: `sll_parse_node`
- Unused memory API and `SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_NULL`

## [0.6.33] - 2021-12-25

### Added

- All functions now store their names (used by `sll:log_log`)
- C Date API: `sll_date_t`, `sll_year_t`, `sll_month_t`, `sll_day_t`, `sll_hour_t`, `sll_minute_t`, `sll_second_t`, `sll_date_from_time` and `sll_date_to_time` ([#56] and [#157])
- CLI flag for removing debugging data: `-D`
- Custom types are now named when created in assignments ([#136])
- Error handling related to memory allocation array functions ([#83])
- File to object API: `sll_file_to_object` and `SLL_FROM_FILE`
- Implemented `date$current` ([#56] and [#156])
- Implemented `date$date_type` ([#56] and [#152])
- Implemented `date$DAYS` ([#56] and [#155])
- Implemented `date$MONTHS` ([#56] and [#154])
- Implemented `date$time_zone_type` ([#56] and [#153])
- Implemented `date.sll` ([#56])
- Implemented `process.sll` ([#22])
- Implemented `sll:date_merge` and `date$to_time` ([#56] and [#150])
- Implemented `sll:date_split` and `date$from_time` ([#56] and [#151])
- Implemented `sll:path_split` ([#146])
- Implemented `sll_api_file_flush`, `sll_api_string_count`, `sll:file_flush` and `sll:string_count`
- Implemented `sll_api_process_start`, `process$FLAG_xxx`, `process$process_config_type`, `process$default_process_config` and `process$start` ([#22] and [#159])
- Implemented `sll_api_string_count_left` and `sll:string_count_left` ([#113])
- Implemented `sll_api_string_count_right` and `sll:string_count_right` ([#114])
- Implemented `sll_current_instruction_index` and `sll_remove_debug_data`
- Implemented `sll_ext:debug_get_call_stack` ([#130] and [#134])
- Implemented `sll_ext:debug_get_instruction_count` and `sll_ext:debug_get_ref_count`
- Implemented `sll_ext:debug_get_instruction_index` ([#130] and [#144])
- Implemented `sll_ext:debug_get_name` ([#130] and [#137])
- Implemented `sll_ext:debug_get_vm_config` ([#130] and [#132])
- Implemented `sll_get_location` and `sll_ext:debug_get_location` ([#141])
- Implemented `sll_platform_execute_shell`, `sll:process_execute_shell`, `process$shell` and `process$shell_path` ([#22] and [#158])
- Implemented `sll_platform_get_library_file_path` and `sys$library`
- Implemented `sll_platform_get_pid`, `sll_pid_t`, `sll:process_get_pid` and `process$pid` ([#22] and [#160])
- Implemented `sll_string_index_reverse`, `sll_string_index_reverse_char`, `sll_string_index_reverse_multiple`, `sll_api_string_index_reverse`, `sll_api_string_index_reverse_list`, `sll:string_index_reverse` and `sll:string_index_reverse_list` ([#143])
- Implemented `sll_string_index`, `sll_string_index_char`, `sll_string_index_multiple`, `sll_api_string_index`, `sll_api_string_index_list`, `sll:string_index` and `sll:string_index_list` ([#142])
- Implemented `sll_string_join`, `sll_string_join_char` and `sll:string_join` ([#106])
- Implemented `sys$library_path` ([#145])
- Logging API now also print the line number
- New node type representing source code lines: `SLL_NODE_TYPE_DBG` ([#140])
- Platform process creation API: `sll_process_handle_t`, `sll_platform_close_process_handle`, `sll_platform_start_process` and `sll_platform_wait_process`
- Server request handler
- `type_type` which is an alias to `int_type`
- VM call stack is now part of the public API
- VM now receive configuration from `sll_vm_config_t` rather than `sll_runtime_data_t`

### Changed

- All optimization levels have been removed in favor of one CLI flag (`-O`)
- CLI now searches for builtin module in a directory relative to the Sll library (acquired by `sll_platform_get_library_file_path`)
- Fixed `SLL_STRING_FORMAT_PADDING`
- Fixed code generation for arrays and maps
- Fixed handling of floating-point numbers in `sll_operator_mod`
- Internal functions no longer return error values with invalid argument configuration ([#148])
- Libraries are now loaded only once ([#135])
- Renamed `sll_string_join`, `sll_string_join_char` and `sll_string_join_chars` to `sll_string_concat`, `sll_string_concat_char` and `sll_string_concat_chars`
- `SLL_ASSEMBLY_INSTRUCTION_TYPE_NOP` is no longer part of the public API
- `sll_internal_function_pointer_t` now uses `sll_object_t*` instead of `const sll_object_t*`
- `SLL_NODE_TYPE_UNKNOWN` is no longer part of the public API ([#139])
- `sll_remove_debug_data` has been renamed to `sll_remove_object_debug_data`
- Updated structure of the [`src/web`][0.6.33/src/web] folder

### Removed

- Duplicate type `sll_page_size_t` (replaced by `sll_size_t`)
- Node debugging data: `sll_debug_node_data_t`, `SLL_NODE_TYPE_DEBUG_DATA`, [`src/sll/debug_object.c`][0.6.32/src/sll/debug_object.c], `sll_insert_debug_node` and `sll_remove_node_debug_data`
- Static storage types ([#147])
- Support for standalone builds ([#149])
- Unneeded error types: `SLL_ERROR_INTERNAL_STACK_OVERFLOW`, `SLL_ERROR_EMPTY_CHAR_STRING`, `SLL_ERROR_UNTERMINATED_CHAR_STRING`, `SLL_ERROR_UNTERMINATED_ESCAPE_SEQUENCE`, `SLL_ERROR_UNKNOWN_ESCAPE_CHARACTER`, `SLL_ERROR_UNTERMINATED_HEX_ESCAPE_SEQUENCE`, `SLL_ERROR_UNKNOWN_SYMBOL`, `SLL_ERROR_SYMBOL_TOO_LONG`, `SLL_ERROR_NO_SYMBOL`, `SLL_ERROR_INTERNAL_FUNCTION_NAME_TOO_LONG`, `SLL_ERROR_INTERNAL_FUNCTION_NAME_NOT_ASCII`, `SLL_ERROR_NO_STACK`, `SLL_ERROR_DIVISION_BY_ZERO` and `SLL_ERROR_STACK_CORRUPTED` ([#138])
- Unused error argument in `sll_load_assembly`, `sll_load_compiled_node` and `sll_load_node`
- Unused function `sll_cleanup_handles`
- Utility API: `sll_api_util_instruction_count`, `sll_api_util_ref_count`, `sll:util_instruction_count` `sll:util_ref_count`, [`src/include/sll/api/util.h`][0.6.32/src/include/sll/api/util.h] and [`src/sll/api/util_api.c`][0.6.32/src/sll/api/util_api.c]
- VM I/O files have been removed from `sll_runtime_data_t` (they are accessible through `sll_current_vm_config`)

## [0.6.32] - 2021-12-18

### Added

- Constant macro for unsupplied `sll_char_t` argument: `SLL_NO_CHAR`
- Constant string containing the extension of a library on the current platform: `sys$library_extension` ([#123])
- External internal function declaration API: `SLL_NODE_TYPE_INTERNAL_FUNC_LOAD`, `SLL_ASSEMBLY_INSTRUCTION_TYPE_LOOKUP` and `SLL_ASSEMBLY_INSTRUCTION_TYPE_LOOKUP_STR`
- Implemented `sll:array_count` and `sll_api_array_count` ([#117])
- Implemented `sll_array_replace`, `sll_api_array_replace` and `sll:array_replace` ([#120])
- Implemented `sll_array_reverse`, `sll_api_array_reverse` and `sll:array_reverse` ([#116])
- Implemented `sll_path_split`
- Implemented `sll_string_ends`, `sll_api_string_ends` and `sll:string_ends` ([#125])
- Implemented `sll_string_pad_left`, `sll_string_pad_right`, `sll_api_string_pad_left`, `sll_api_string_pad_right`, `sll:string_pad_left`, `sll:string_pad_right`([#111] and [#112])
- Implemented `sll_string_reverse`, `sll_api_string_reverse` and `sll:string_reverse` ([#87])
- Implemented `sll_string_starts`, `sll_api_string_starts` and `sll:string_starts` ([#124])
- Implemented `sll_version` (function counterpart of `SLL_VERSION` macro)
- Library API: `sll_library_handle_t`, `sll_platform_load_library`, `sll_platform_lookup_function`, `sll_platform_unload_library` and `sll_api_sys_load_library` ([#123])
- Logging API: `SLL_LOG`, `sll_log`, `sll_set_log_default`, `sll_set_log_file` and `sll_set_log_function` ([#126], [#128] and [#129])
- Max internal function length: `SLL_INTERNAL_FUNCTION_MAX_LENGTH`
- Sll logging API: `sll_api_log_log`, `sll_api_log_set_default`, `sll_api_log_set_file`, `sll_api_log_set_function`, `sll:log_log`, `sll:log_set_default`, `sll:log_set_file` and `sll:log_set_function` ([#126] and [#127])
- Sll version API: `sll_api_sys_get_version` and `sll:sys_get_version`
- Website code in [`src/web`][0.6.32/src/web]

### Changed

- Fixed optimization of custom object types
- Improved large integer compression
- Moved platform-dependent code from [`src/sll/platform/posix.c`][0.6.31/src/sll/platform/posix.c] and [`src/sll/platform/windows.c`][0.6.31/src/sll/platform/windows.c] to [`src/sll/platform/posix`][0.6.32/src/sll/platform/posix] and [`src/sll/platform/windows`][0.6.32/src/sll/platform/windows]
- Object type functions now expect a `sll_object_type_table_t` instead of using the global `sll_current_runtime_data`
- `sll_array_count` now uses `sll_operator_strict_equal` instead of `sll_operator_equal`

### Removed

- Unneeded member `str` from `sll_error_data_t`
- Unused error type: `SLL_ERROR_UNKNOWN_INTERNAL_FUNCTION`

## [0.6.31] - 2021-12-12

### Added

- CLI option for generating Sll code (`-F` or `--generate-sll`) ([#104])
- Default file API: `sll_api_file_std_handle` and `file_std_handle`
- Handle clone API: `sll_handle_clone` ([#101])
- Handles are now cloned when assigned to custom type objects ([#101])
- Implemented `sll_add_initializer` ([#105])
- Implemented `sll_file_write_char_count`
- Implemented `sll_write_sll_code` ([#104])
- Implemented custom type creation optimization ([#105])
- Large page support: `SLL_ROUND_PAGE` and `SLL_ROUND_LARGE_PAGE` ([#102])
- New node and assembly instruction type: `SLL_NODE_TYPE_DECL_COPY` and `SLL_ASSEMBLY_INSTRUCTION_TYPE_DECL_COPY` ([#105])
- Optimizer now removes calls to undefined functions ([#103])

### Changed

- Converting handles to strings now uses a new template
- Fixed critical invalid identifier index bug in the optimizer
- Memory page size can now be accessed via `SLL_PAGE_SIZE` and `SLL_LARGE_PAGE_SIZE` macros ([#102])
- Renamed `sll_add_string_runtime` to `sll_add_string_object`
- `sll_platform_allocate_page` now accepts an argument to allocate large pages ([#102])

### Removed

- Page size API: `sll_platform_get_page_size`

## [0.6.30] - 2021-12-05

### Added

- Ability to access elements of custom types (or maps) from variables by using the `|# variable #|$|# field 1#|$|# field 2#|$|# field 3#|` syntax
- Ability to clone structures without references (by the `(: |# object #|)` syntax)
- Finished custom object type API ([#72])
- `func_type` which is an alias to `int_type`
- Implemented buffered file reading
- Implemented increment (`(++)`) and decrement (`(--)`) operators ([#96])
- Implemented `sll_api_string_split`, `sll_api_array_create`, `sll:string_split` and `sll:array_create` ([#87])
- Implemented `SLL_OBJECT_GET_TYPE_MASK` and `sll_array_zero`
- Memory structures can have constant fields (denoted by inverting (`(~)`) the type) ([#99])
- More examples ([#77])

### Changed

- Fixed storage type handling in structure objects
- Function calls now properly manage the stack ([#27])
- Internal Functions are now named according to [#73]
- Renamed the import operator from `(--)` to `(---)` ([#96])
- Structure fields are now declared as identifiers and are marked by an internal type (`SLL_NODE_TYPE_FIELD`)
- Syntax highlight now correctly marks type declarations

### Removed

- URL & socket APIs (Therefore invalidated [#66])

## [0.6.29] - 2021-11-28

### Added

- C and Sll variable argument list API: `sll_var_arg_list_t`, `sll_var_arg_get`, `sll_var_arg_get_int`, `sll_var_arg_get_float`, `sll_var_arg_get_char` and `sll_var_arg_get_string`
- Implemented `sll_api_string_format`, `sll_api_file_read`, `read`, `sll_string_decrease`
- Implemented assign-by-access
- Miscellaneous node function are now located in [`node.c`][0.6.29/src/sll/node.c]
- Object type table API: `sll_object_type_table_t`, `sll_object_type_data_t`, `sll_object_type_data_entry_t`, `sll_add_type`, `sll_create_new_object_type`, `sll_create_object_type`, `sll_object_get_field`, `sll_free_object_type_list`, `sll_object_set_field`
- Option for the builder script to generate only documentation: `--docs-only`
- String flip-case API: `flip_case`, `sll_api_string_flip_case`, `sll_api_string_flip_case_raw` and `sll_string_flip_case`
- Structure definition (`(&:)`) and creation (`(.)`) operators ([#99])

### Changed

- All `sll_deinit_xxx` functions have been renamed to `sll_free_xxx` and moved to their corresponding source files
- Changed declaration of `sll_handle_stringify_t`
- Documentation format
- Fixed critical bug in stack memory allocator
- Fixed multiple return types
- Improved performance of `sll_string_format` and `sll_string_format_list` with regards to integer to string conversion ([#80])
- Renamed `sll_register_standard_internal_functions` to `sll_register_builtin_internal_functions`
- Renamed objects to nodes, and runtime objects to objects
- Reworked syntax highlight
- `sll_object_type_t` is now 32-bit
- `sll_platform_file_size` now expects a file descriptor instead of a file path
- Storage of built-in internal functions and logic behind IFT-related functions

### Removed

- Deprecated initialization API: [`init.h`][0.6.28/src/include/sll/init.h], [`deinit.c`][0.6.28/src/sll/deinit.c] and [`init.c`][0.6.28/src/sll/init.c]
- `sll_object_to_string` has been replaced by `sll_api_string_convert`
- Unused `sll_deinit_xxx` functions
- Unused functions: `sll_api_string_convert_length` and `sll_api_string_length` (and matching `string.sll` function declarations)
- Useless GC debug data API: `SLL_GC_ZERO_DEBUG_DATA` and `SLL_GC_ZERO_DEBUG_DATA_STRUCT`

## [0.6.28] - 2021-11-22

### Added

- Boolean operator (`(!!)`) ([#100])
- Buffering for in `sll_file_read` and `sll_file_read_char`
- CLI option for enabling sandbox flags (`-S`) ([#97])
- Implemented `sll_platform_get_default_stream_descriptor`, `sll_file_write_format`, `sll_file_write_string`, `sll_string_format`, `sll_string_format_list` and `sll_string_select`
- Integer can now have digits split by underscores (`_`)
- Macro constants: `SLL_PLATFORM_STREAM_INPUT`, `SLL_PLATFORM_STREAM_OUTPUT` and `SLL_PLATFORM_STREAM_ERROR`
- Secure sandbox API: `sll_get_sandbox_flag`, `sll_set_sandbox_flags`, `SLL_SANDBOX_FLAG_DISABLE_FILE_IO`, `SLL_SANDBOX_FLAG_ENABLE_STDIN_IO`, `SLL_SANDBOX_FLAG_ENABLE_STDOUT_IO` and `SLL_SANDBOX_FLAG_DISABLE_PATH_API` ([#97])
- `sll_api_sys_get_executable` and the `executable` variable in `sys.sll`
- String copy API: `sll_copy_string`, `sll_copy_string_null` and `SLL_COPY_STRING_NULL` ([#79])
- Type-check (typeof) operator (`(:?)`) ([#74])

### Changed

- Fixed `sll_platform_enable_console_color`
- Fixed incorrect memory reallocation in `sll_string_increase`
- Return types of multiple internal functions
- `sll_debug_xxx` function now use the built-in file API
- The `sll_runtime_data_t` structure now contains a field for an error file (`sll_file_t* err`)

### Removed

- [`cast.h`][0.6.27/src/include/sll/cast.h] has been replaced by runtime object types
- Unused object: Input (`(<:)`)

## [0.6.27] - 2021-11-14

### Added

- CLI now properly releases all memory (by calling `sll_deinit`)
- Custom file IO API with output buffering: `sll_file_close`, `sll_file_flush`, `sll_file_from_data`, `sll_file_open`, `sll_file_open_descriptor`, `sll_file_read`, `sll_file_read_char`, `sll_file_reset`, `sll_file_reset_line`, `sll_file_write` and `sll_file_write_char`
- `extern` variables are now marked as `__SLL_EXTERNAL extern`
- Implemented `SLL_MEMORY_MOVE_DIRECTION_FROM_STACK`, `SLL_MEMORY_MOVE_DIRECTION_TO_STACK` and `sll_memory_move` ([#94])
- Implemented `sll_platform_enable_console_color`, `sll_platform_get_current_working_directory`, `sll_platform_get_executable_file_path`, `sll_platform_set_current_working_directory`, `sll_set_memory` and `sll_zero_memory`
- Implemented `sll_zero_allocate_stack` ([#98])
- More memory API functions: `sll_allocate_stack`
- Path separator macro constant: `SLL_API_FILE_PATH_SEPARATOR`
- `SLL_UNKNOWN_INTERNAL_FUNCTION_INDEX` macro constant ([#89])
- Three new functions in `path.sll`: `get_cwd`, `join` and `set_cwd` ([#87])

### Changed

- All `memcpy` calls have been replaced by `sll_copy_data`, and all `memset` calls have been replaced by `sll_set_memory` or `sll_zero_memory`
- Changed arguments of `sll_zero_allocate`
- CLI and test script now use built-in sll types
- Expanded two macros: `SLL_RETURN_ERROR` to `0` and `SLL_RETURN_NO_ERROR` to `1`
- Fixed incorrect union member access ([#84])
- *\[POSIX only\]* Fixed return type of `sll_platform_get_page_size` ([#88])
- Improved performance requested by [#92]
- Refactored code according to [#82] and [#95]
- Renamed `__SLL_FUNC` to `__SLL_EXTERNAL`
- Renamed `sll_loop_data_t` to `sll_loop_object_data_t` ([#85])
- Replaced `EXPAND_FILE_PATH` macro by `sll_path_absolute` function calls in the CLI
- Replaced `memcmp` calls with calls to `sll_compare_data` and `strcmp` with `sll_string_compare_pointer` ([#81])
- Replaced `sll_platform_path_absolute` by `sll_path_absolute` (which is platform-independent) ([#86])
- Replaced `sll_return_t` by `sll_bool_t`
- `SLL_API_MAX_FILE_PATH_LENGTH` is now platform-dependent
- `sll_copy_data` now expects `sll_size_t` instead of `sll_string_length_t`

### Removed

- Duplicated code ([#90] and [#93])
- Options related to updating sll from the CLI
- Stream API
- Unused CLI macros: `EXPAND_FILE_PATH` and `GET_EXECUATBLE_FILE_PATH`
- Unused constants: `SLL_MAX_ALLOCATED_BLOCK_LENGTH`, `SLL_MAX_READ_CHAR`, `SLL_MAX_STRING_CHECKSUM`, `SLL_MAX_RUNTIME_OBJECT_TYPE`, `SLL_MAX_OBJECT_TYPE`, `SLL_MAX_JSON_OBJECT_TYPE`, `SLL_MAX_ERROR_TYPE`, `SLL_MAX_FILE_FLAGS`, `SLL_MAX_COMPARE_RESULT`, `SLL_MAX_BOOL`, `SLL_MAX_CLEANUP_TYPE`, `SLL_MAX_INTERNAL_FUNCTION_TYPE`, `SLL_MAX_RETURN`, `SLL_MAX_FILE_PATH_INDEX`, `SLL_MAX_VERSION`, `SLL_MAX_COLUMN_NUMBER`, `SLL_MAX_LINE_NUMBER`, `SLL_RETURN_ERROR` and `SLL_RETURN_NO_ERROR`
- Unused headers ([#91])
- Unused types: `sll_allocated_block_length_t`, `sll_cleanup_type_t`, `sll_file_path_index_t`, `sll_return_t`, `sll_column_number_t` and `sll_input_buffer_t`

## [0.6.26] - 2021-11-07

### Added

- Custom memory API: `sll_allocate`, `sll_deallocate`, `sll_reallocate` and `sll_zero_allocate`
- Implemented `sll_char_to_object`, `sll_copy_data`, `sll_operator_strict_equal` and `sll_string_from_char`
- Option macro to disable the custom memory allocator (`USE_BUILTIN_MALLOC`)
- Strict equal (`(===)`) and not equal (`(!==)`) operators ([#78])

### Changed

- Runtime object pool used by GC now allocates memory pages in larger chunks
- `sll_execute_assembly` now expects the size of the stack instead of the stack itself
- `sll_get_object_size` is now correctly exported
- `sll_platform_list_directory` and `sll_platform_list_directory_recursive` now pre-allocates lists to reduce the number of memory reallocations
- Strings are now aligned 8 bytes above a 16-byte boundary (like all memory returned by `sll_allocate`, `sll_reallocate` or `sll_zero_allocate`)

### Removed

- `type` argument from `sll_register_cleanup`
- Unused error types: `SLL_ERROR_UNRELEASED_OBJECTS` and `SLL_ERROR_ASSERTION`
- Unused included headers
- Unused macros: `SLL_CLEANUP_ORDER_NORMAL`, `SLL_CLEANUP_ORDER_RESERVED0` and `SLL_STRING_ALIGN`
- Unused member `sll_stack_offset_t st` from `sll_assembly_instruction_data_t`
- Unused stack API: `sll_stack_offset_t`, `sll_stack_data_t`, `sll_deinit_stack_data`, `sll_init_stack_data`, `sll_setup_stack`, `SLL_MAX_STACK_OFFSET`, `SLL_INIT_STACK_DATA` and `SLL_INIT_STACK_DATA_STRUCT`
- Unused type `sll_cleanup_type_t`

## [0.6.25] - 2021-10-31

### Added

- Added [`examples`][0.6.25/examples]! ([#77])
- Implemented `sll_add_string_runtime`, `sll_array_compare`, `sll_array_init`, `sll_free_array`, `sll_operator_assign`, `sll_string_compare_array`, `sll_string_insert_pointer`, `sll_string_insert_pointer_length`, `sll_string_join_char`, `sll_string_join_chars`, `sll_string_prepend_char` and `sll_string_set_char`
- Implemented `SLL_STRING_INSERT_POINTER_STATIC` macro
- Index assignment optimization ([#69])
- Marked appropriate functions as `__SLL_CHECK_OUTPUT`
- Structure initialization functions in [`src/include/sll/init.h`][0.6.25/src/include/sll/init.h] and [`src/sll/init.c`][0.6.25/src/sll/init.c]

### Changed

- Changed `sll_bool_t` to use `_Bool` instead of `uint8_t`
- Fixed `sll_string_join`
- Fixed incorrect optimization of variables inside loops ([#76])
- Fixed invalid memory access in `sll_string_join`
- Fixed memory corruption in the CLI
- Moved `sll_deinit` and `sll_init` from [`src/include/sll/util.h`][0.6.25/src/include/sll/util.h] to [`src/include/sll/init.h`][0.6.25/src/include/sll/init.h]
- Renamed all `sll_free_` functions to `sll_deinit_` and moved them to [`src/include/sll/init.h`][0.6.25/src/include/sll/init.h] and [`src/sll/deinit.c`][0.6.25/src/sll/deinit.c]
- Renamed all `SLL_ZERO_` macros to `SLL_INIT_` and moved them to [`src/include/sll/init.h`][0.6.25/src/include/sll/init.h]
- Renamed `integer_heap_queue` to `binary_heap`
- Renamed `sll_cleanup_function` to `sll_cleanup_function_t`
- `sll_add_debug_data` now expects an `unsigned int` instead of `uint8_t`
- `sll_compilation_stack_data_t` and `sll_assembly_stack_data_t` now use an `unsigned int` instead of `uint32_t`

### Removed

- Most `memcpy` calls
- Unused `sll_instruction_index_t` member from `sll_runtime_object_data_t`
- Unused function `sll_integer_heap_queue_init` and `sll_string_set`
- Unused type `sll_allocation_size_t` and macro `SLL_MAX_ALLOCATION_SIZE`

## [0.6.24] - 2021-10-25

### Added

- API related to listing files in a directory (`sll_api_path_list_dir`, `sll_api_path_list_dir_raw`, `sll_api_path_recursive_list_dir`, `sll_api_path_recursive_list_dir_raw`, `sll_platform_list_directory`, `list_dir` and `recursive_list_dir`)
- Implemented `sll_array_get`, `sll_array_set`, `sll_map_get_key`, `sll_map_get_value`, `sll_map_set_key`, `sll_map_set_value`, `sll_stream_read`, `sll_stream_read_buffer`, `sll_stream_restart_line`, `sll_stream_write`, `sll_stream_write_buffer`, `sll_string_get` and `sll_string_set`
- Macro `SLL_ZERO_INTEGER_HEAP_QUEUE` for consistency
- Sorting API (`sll_api_sort_sort`, `sll_api_sort_sort_raw`, `sll_quicksort`, `sort`)

### Changed

- All constants excluding `true`, `false` and `nil` have been move to library files (`int_type`, `float_type`, `char_type`, `string_type`, `array_type`, `handle_type`, `map_type`, `map_key_type` and `map_value_type` have been moved to [`src/sll/lib/types.sll`][0.6.24/src/sll/lib/types.sll] and `stdin`, `stdout`, `stderr` have been moved to [`src/sll/lib/file.sll`][0.6.24/src/sll/lib/file.sll])
- *\[POSIX only\]* Fixed return value of `sll_platform_path_absolute`
- Fixed string checksum calculation in `sll_string_from_int`
- Identifiers are now correctly printed
- Renamed `sll_string_hash` to `sll_string_calculate_checksum`
- `sll_print_object` now accepts an optional internal function table, which it will use to decode internal function calls
- `sll_rotate_bits` has been renamed to `ROTATE_BITS` and is now used only internally
- Replaced the unused internal `OPTIMIZER_FLAG_IGNORE_LOOP_FLAG` flag with `OPTIMIZER_FLAG_ASSIGN`

### Removed

- Buffer-related types: `sll_buffer_t` (now `sll_char_t*`), `sll_const_buffer_t` (now `const sll_char_t*`) and `sll_buffer_size_t` (now `sll_string_length_t`)
- Functions for writing strings to buffers: `sll_output_data_stream_t.wsf` and `SLL_WRITE_STRING_TO_OUTPUT_DATA_STREAM`
- Maximum buffer size macro `SLL_MAX_BUFFER_SIZE` (now `SLL_MAX_STRING_LENGTH`)
- `memcpy` calls in [`src/sll/string.c`][0.6.24/src/sll/string.c] and `memset` calls in `SLL_STRING_FORMAT_PADDING`

## [0.6.23] - 2021-10-18

### Added

- Assembly instructions for jump tables (`SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_LABEL` and `SLL_ASSEMBLY_INSTRUCTION_TYPE_JT`) ([#67])
- Implemented `sll_acquire_object`,`sll_identifier_add_index`, `sll_identifier_create_identifier`, `sll_identifier_get_array_id`, `sll_identifier_get_array_index`, `sll_string_combine_checksums`, `sll_string_increase` and `sll_string_op`
- Macro `SLL_ZERO_HEADER`

### Changed

- Fixed assembly instruction counting
- Fixed bugs in optimization and merging of print objects
- Fixed string merging in optimization of `SLL_OBJECT_TYPE_PRINT` objects
- Fixed VM instruction jumps
- Instruction counter now increases before the instruction is executed
- Renamed `sll__add_debug_data` to `sll_add_debug_data`, `SLL_NO_DEBUG_DATA` to `SLL_GC_ZERO_DEBUG_DATA_STRUCT`, `SLL_LOOKUP_HANDLE_DESCRIPTOR_FAST` to `SLL_HANDLE_LOOKUP_DESCRIPTOR`, `SLL_UNKNOWN_HANLE_TYPE` to `SLL_HANDLE_UNKNOWN_TYPE`, `SLL_ZERO_integer_heap_queue_STRUCT` to `SLL_ZERO_INTEGER_HEAP_QUEUE_STRUCT`, `SLL_ABS_FLOAT` to `SLL_FLOAT_ABSOLUTE` and `SLL_ROUND_FLOAT` to `SLL_FLOAT_ROUND`
- Renamed `SLL_STRING_COMBINE_CHECKSUMS_FAST` to `SLL_STRING_COMBINE_CHECKSUMS` (and removed the latter one)
- Renamed integer lists to integer heap queues ([#71])
- Runtime objects now store debug data in the correct order

### Removed

- Internal operator declaration macros `__SLL_OPERATOR_UNARY`, `__SLL_OPERATOR_BINARY`, `__SLL_OPERATOR_TERNARY` and `__SLL_OPERATOR_QUATERNARY`
- Internal static object macros: `__SLL_STATIC_NAME` and `__SLL_STATIC_OBJECT_DECL`
- `SLL_STRING_INCREASE` macro (in favor of `sll_string_increase`), `SLL_STRING_FROM_STATIC` macro (in favor of `sll_string_from_pointer`, which removes bugs with writing to read-only memory)
- Unused error: `SLL_ERROR_TOO_MANY_STATEMENTS`

## [0.6.22] - 2021-10-10

### Added

- Adjacent `SLL_OBJECT_TYPE_PRINT` objects are not combined
- Inline-if statements (`(?:)`) and switch statements (`(??)`) ([#67])
- Integer lists
- `SLL_OBJECT_TYPE_ACCESS` is now optimized properly
- Strings longer than `STRING_COMPRESSION_MIN_LENGTH` characters are now compressed when generating string tables in files

### Changed

- All arguments in objects of type `SLL_OBJECT_TYPE_PRINT` are converted to strings
- Fixed bug in `SLL_IS_OBJECT_TYPE_NOT_TYPE`
- Fixed bugs in optimizing `SLL_OBJECT_TYPE_COMMA` objects
- Fixed optimization of if statements (`(?)`)
- Renamed `SLL_ERROR_SLL_ASSERTION` to `SLL_ERROR_ASSERTION`
- Renamed some array & string functions for consistency
- Rewritten header files
- Rewritten variable optimization

### Removed
- `sll_statement_count_t` and `SLL_MAX_STATEMENT_COUNT` (replaced by `sll_arg_count_t` and `SLL_MAX_ARG_COUNT`)
- [`src/include/sll/core.h`][0.6.21/src/include/sll/core.h] and [`src/include/sll/constants.h`][0.6.21/src/include/sll/constants.h]

## [0.6.21] - 2021-10-04

### Added

- All operators are now properly optimized
- Hash module ([#23])
- Implemented **ALL** operators ([#14])
- Implemented `sll_array_and_array`, `sll_array_combinations`, `sll_array_equal`, `sll_array_equal_map`, `sll_array_or_array`, `sll_array_xor_array`, `sll_array_range`, `sll_array_string_combinations`, `sll_map_add_array`, `sll_map_add_string`, `sll_map_and`, `sll_map_and_array`, `sll_map_and_string`, `sll_map_equal`, `sll_string_duplicate`, `sll_string_equal_array`, `sll_string_equal_map`, `sll_string_from_int`, `sll_string_replace_char` and `sll_string_split_char`
- Internal function can now be declared as `SLL_INTERNAL_FUNCTION_FLAG_COMPILATION_CALL`, which would make them be at compile-time
- Memory API & reference operator ([#42])

### Changed

- Bug fixes in the optimizer
- `sll_operator_add` now correctly combines objects with strings and arrays

## [0.6.20] - 2021-09-26

### Added

- Array, string and map functions ([#3])
- Implemented over 70% of all operators
- `instruction_count`, `sll_api_util_instruction_count` and `sll_current_instruction_count` ([#51])
- Operator function types: `sll_unary_operator_t`, `sll_binary_operator_t`, `sll_ternary_operator_t` and `sll_quaternary_operator_t`
- Optimizer has now (partial) array optimization support
- `SLL_ABS_FLOAT`
- `sll_platform_file_size` and `sll_string_join` functions

### Changed

- CLI now also tries to load plain-text modules from the `lib/` directory
- Fixed API Code Generation
- Fixed bugs related to incorrect variable inlining
- Fixed GC bugs in `SLL_FROM_CHAR` (and `SLL_ACQUIRE_STATIC_CHAR`)
- Fixed memory corruption in [`src/sll/api/file_api.c`][0.6.20/src/sll/api/file_api.c]
- Float comparison now uses an error constant to minimize unexpected behavior ([#53])
- `PRINT_INT_SIGN` now prints `+0` instead of `0` ([#52])
- Renamed `sll_operator_access_zero` back to `sll_operator_dup`
- Split the API header into multiple files in the [`src/include/sll/api`][0.6.20/src/include/sll/api] directory
- Strings checksums are now 32-bit (to combine two checksums use the `SLL_STRING_COMBINE_CHECKSUMS` or `SLL_STRING_COMBINE_CHECKSUMS_FAST` macro) ([#40])
- Useless `goto` statements replaced by do-while loops and `memcmp` calls ([#54])

### Removed

- Runtime object stack API: `sll_get_runtime_object_stack_data`, `sll_runtime_object_stack_data_t`, `sll_free_runtime_object_stack_data` etc.
- `SLL_SAME_ARRAY`, `SLL_SAME_MAP` and `SLL_SAME_STRING` macros

## [0.6.19] - 2021-09-21

### Added

- Added automatic function inlining ([#41]) and constant term evaluation ([#48])
- Added more operators
- Break (`(@)`) and continue (`(<<<)`) operators ([#44])
- [Code of Conduct][0.6.19/CODE_OF_CONDUCT.md]
- Comma operator (`(,)`): It behaves like an operator list (`{}`), but instead of returning `nil`, it returns the value of the last expression ([#43])
- Debugging functions, such as `sll_debug_print_object` or `sll_debug_print_runtime_object`
- Inline function (`(***)`) operator: It is equal to (`(<- (,,, |# code #|) |# args #|)`) ([#46])
- Internal function now store the type of the function, for example `SLL_INTERNAL_FUNCTION_TYPE_REQUIRED` (Used [#47] to remove useless function calls)
- `SLL_ACQUIRE_NO_DEBUG` macro (like `SLL_ACQUIRE` but does not add debug data)
- `SLL_ACQUIRE_STATIC_CHAR` macro (an alias to `SLL_FROM_CHAR`)
- `SLL_OBJECT_TYPE_FUNCTION_ID` type: Generated by the optimizer to correctly resolve function IDs

### Changed

- Fixed syntax highlight
- Compilation & assembly stacks are now stored & resized internally ([#50])
- **HUGE** memory (stack) corruption in [`src/cli/main.c`][0.6.19/src/cli/main.c]
- `sll_operator_bool` now returns `sll_bool_t` instead of `sll_compare_result_t`
- Renamed `sll_operator_dup` to `sll_operator_access_zero`
- Renamed the exit operator (`(@)`) to `(@@@)`
- Replaced `PRINT_STATIC_STRING` function calls to `SLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM` in [`src/sll/print.c`][0.6.19/src/sll/print.c]
- Updated the license name

### Removed

- Duplicate JSON API from [`tests`][0.6.19/tests] directory
- `SLL_RETURN_ZERO`, `SLL_RETURN_ONE`, `SLL_RETURN_ZERO_STRING` and `SLL_RETURN_ZERO_HANDLE` macros
- Stack types & stack API ([#50])
- Unused constant `SLL_COMPARE_RESULT_ERROR`

## [0.6.18] - 2021-09-13

### Added

- Added `sll_static_int[n]` for integers in range of `0 - 255` (inclusive), `sll_static_negative_int[-n-1]` for integers in range of `-16 to -1` (inclusive) (for example, `-2` can be accessed by `sll_static_negative_int[-(-2)-1] = sll_static_negative_int[1]`) and `sll_static_char[n]` for all characters (unsigned 8-bit integer)
- API function signatures (arguments and return types) are now stored in [`src/include/sll/_api_generated.h`][0.6.18/src/include/sll/_api_generated.h]
- Builder script option for generating code & signatures for API functions (`--generate-api`)
- Code generated for API functions is now located in [`src/sll/api/_generated.c`][0.6.18/src/sll/api/_generated.c]
- GC now also check for unreleased static objects
- Implemented more operations in  [`src/sll/operator.c`][0.6.18/src/sll/operator.c]
- Macro for creating an object from a `sll_char_t` value: `SLL_FROM_CHAR`
- `sll_init`, `sll_deinit` and `sll_char_to_object` functions
- Useless assembly code (`STORE $xx & POP,LOAD $xx & DEL`) is removed
- When updating, the CLI also downloads the Change Log for the given version

### Changed

- Function arguments will now shadow other variables instead of overriding them
- Marked internal CLI variables & function as `static`
- `sll_error_t` now uses `sll_char_t` instead of `char`
- `sll_map_t` now stores the length of the map (`n`) instead of the length of the element array (`2n`) (TL;DR Multiply the new value by a factor of 2 to get the previous value)
- VM variables are now properly released

### Removed

- `sll_static_minus_one`, `sll_static_zero`, `sll_static_one`, `sll_static_two`, `sll_static_three`, `sll_static_four` and `sll_static_float_third` (in favor of `sll_static_int` and `sll_static_negative_int`)

## [0.6.17] - 2021-09-09

### Added

- Automatic API function code generation
- Documentation parser
- Fixed bugs in the builder script
- JSON api
- Mappings ([#36])
- More logging related to updating from the CLI
- New Constants: `int_type`, `float_type`, `char_type`, `string_type`, `array_type`, `handle_type`, `map_type`, `map_key_type` and `map_value_type`
- New type casting operator: `(::)` ([#37])
- `sll_operator_cast`, `sll_api_json_type`, `sll_lookup_handle_descriptor` and `sll_string_hash` functions
- `sll_register_cleanup` function, which can be used for safely executing functions before key library features (like GC) are cleaned up
- Strings are now aligned on a 8-byte boundary to aid performance
- The `url.sll` module ([#29])
- The GC now stores data about each call to `SLL_CREATE`, `SLL_ACQUIRE`, `SLL_RELEASE` or other runtime object creation function

#### Mappings

Maps (Mapping Objects) are created by arrow brackets (`<key1 value1 key2 value2>`). If the number of elements is odd, a `nil` is automatically added to the end.

#### API Function Code Generation

Each API function is now split into two parts: `sll_api_XXX` and `sll_api_XXX_raw`. The 'raw' function takes an object array and error-checks each arguments. It then calls the base function with all the arguments converted into C types. The code is generated based on the documentation.

### Changed

- All `sll_operator_XXX` functions now use non-constant parameters (This has been changed to allow returning one of the operands)
- Function that return a value (except for `sll__add_debug_data` and `sll_register_internal_function`) are now marked with `__SLL_CHECK_OUTPUT`
- Internal functions are now declared as `static`
- `sll_handle_descriptor_t` can also specify a `sll_handle_stringify_t` function

### Removed

- `sll_init` function has been replaced by platform-dependent code to automatically set-up the console
- `__SLL_RETURN_xxx` macros from [`src/include/sll/common.h`][0.6.17/src/include/sll/common.h]

## [0.6.16] - 2021-08-30

### Added

- Ability to upload the executable to the website
- Bit shifting operators ([#30]): `(<<)` and `(>>)` as well as their `SLL_OBJECT_TYPE_` and `SLL_ASSEMBLY_INSTRUCTION_TYPE_` counterparts (This also includes updates to the syntax highlight)
- Fixed macro parsing in the builder script
- Fixed memory leak in `sll_add_string` ([#32]) and memory corruption in `sll__add_debug_data` ([#33])
- Handles
- JSON module ([#34])
- New object creation macro: `SLL_FROM_HANDLE` (and `sll_handle_to_object`)
- New option for updating sll from the CLI (`-U`) ([#35])
- New static object: `handle_zero`
- `sll_current_runtime_data` holds the current runtime data of the program (Can be used to ex. create handle types or write directly to output)
- `SLL_ERROR_UNRELEASED_OBJECTS` error: Raised when not all objects (allocated during execution of the program) have been `SLL_RELEASE`d
- `sll_platform_setup_console` and `sll_platform_reset_console`: On Windows, is sets the output mode to binary
- This Change Log!

#### Handles

Handles can be used to uniquely identify an object. They are a replacement for `SLL_RUNTIME_TYPE_INT`. They consists of the handle itself (`sll_handle_t`) and a type (`sll_handle_type_t`).

| Feature | SLL_RUNTIME_TYPE_INT | SLL_RUNTIME_TYPE_HANDLE |
|-|-|-|
| Range | -2\^63 to 2^63-1 | 0 to 2^64-1 |
| Type Range | N/A | 0 to 2^32-1 |
| Multiple Types | No | Yes |
| Type Name | No | Yes (up to 256 characters) |
| Automatic Cleanup | No | Yes |
| Arithmetic Operations | Yes | Yes |

Handle types can be create by calling `sll_create_handle()` and specifying the name of the new handle type and its destructor. The destructor is automatically called when the last instance of the object gets released. The destructor also gets called when the handle type is deregistered.
If a handles does not have a type (`SLL_UNKNOWN_HANDLE_TYPE`) it has a boolean value of zero. Otherwise, it has a boolean value of one.

### Changed

- Constant string objects in [`src/sll/vm.c`][0.6.16/src/sll/vm.c] now use part of the user-supplied stack instead of `malloc()`
- Renamed `SLL_MAX_ARGUMENT_COUNT` to `SLL_MAX_ARG_COUNT`
- Renamed top-of-stack variables in [`src/sll/vm.c`][0.6.16/src/sll/vm.c] to `tos`
- `sll_execute_assembly` now expects `sll_runtime_data_t` instead of multiple parameters

### Removed

- `SLL_UNIMPLEMENTED()` statements in [`src/sll/operator.c`][0.6.16/src/sll/operator.c]
- Unused constant `INVALID_FILE_HANDLE` and unused macro `SLL_API_INVALID_FILE_HANDLE`

## [0.6.15] - 2021-08-26

### Added

- *\[POSIX only\]* Compilation in debug mode (without `--release` flag) now generates debugging information (gcc `-g` flag)
- If a variable is used before it has been initialized, is is replaced by a zero: `(= x x x x x)` is compiled into `(= x 0 x x x)`
- Object allocation function: `sll_create_object`
- Object creation functions: `sll_int_to_object`, `sll_float_to_object`
- Static objects

#### Static Objects

It is very inefficient to constantly allocate objects with common values (such as zero). The `SLL_ACQUIRE_STATIC` macro allows to acquire on of the predefined static object (`int_minus_one`, `int_zero`, `int_one`, `int_two`, `int_three`, `int_four`, `float_zero`, `float_one`, `float_half`, `float_third`, `str_zero` and `array_zero`).
Helper functions (`sll_int_to_object` and `sll_float_to_object`) can be used to automatically convert a C type to either a new or a static object.

### Changed

- Changed how runtime objects are allocated, acquired, released and deallocated
- Combined cleanup functions into single `atexit` function (which calls other functions in the correct order)
- Fixed Function Macro Parsing
- `internal_function_t` now uses `sll_char_t[256]` instead of `char[256]`
- Printing functions now also prints their arguments
- Signatures of functions that return runtime objects

#### Runtime Objects

##### Allocation

Runtime Object used to call `malloc()`. This was inefficient, because each runtime object has the same size. Now, runtime objects use a paged memory pool, which gets dynamically increased when more objects are required. A convenience macro named `SLL_CREATE` can also be used to allocate a runtime object.

##### Acquiring

Instead of reference counting for strings and arrays, now every runtime object has built-in reference counting. The `SLL_ACQUIRE` macro increases the object's reference counter.

##### Releasing

The `SLL_RELEASE` macro can be used to decrease the object's reference counter.

##### Deallocation

The `SLL_RELEASE` macro automatically deallocates the object when its last reference is released. It recycles the object back into the memory pool.

### Removed

- Duplicated code in [`src/sll/parse.c`][0.6.15/src/sll/parse.c] and [`src/sll/print.c`][0.6.15/src/sll/print.c]
- `sll_acquire_object` function
- `sll_sys_arg_count_t` has been replaced by `sll_integer_t` (it was easy to confuse with `sll_arg_count_t`)
- The `restrict` keyword (Reverting [#18])

## [0.6.14] - 2021-08-24

### Added

- Ability to execute code directly from the command line with the `-s` option ([#19])
- The `restrict` keyword ([#18])
- The `SLL_VERSION_HAS_SHA` macro is defined when the `SLL_VERSION_SHA` and `SLL_VERSION_FULL_SHA` macros hold the commit sha-1 value

### Changed

- Moved the CLI code into its own folder ([`src/cli/`][0.6.14/src/cli])
- Reverted from the BSD 3-Clause License to the MIT License
- The order of arguments in an API function (Used to be `output, argument count, argument data` and now is `argument data, argument count, output`)

### Removed

- CLI no-logo (`-L`) option
- Useless clean-up code in [`src/sll/parse.c`][0.6.14/src/sll/parse.c] ([#20])

## [0.6.13] - 2021-08-20

### Added

- Added and/or removed included header files ([#11])
- Function for adding string to a string list: `sll_add_string`
- Implemented part of [`src/sll/operator.c`][0.6.13/src/sll/operator.c]
- New type: `sll_sys_arg_count_t`
- Object-to-string conversion functions: `sll_object_to_string` and `sll_object_to_string_length`
- Optimization in printing ([#15], [#16] and [#17])
- Platform-dependent function `sll_platform_sleep`
- System argument functions (`argv` in C): `sll_set_argument_count` and `sll_set_argument`
- Time sleeping functions: `sleep`, `sleep_nanos`, `sll_api_time_sleep` and `sll_api_time_sleep_nanos` ([#5])

### Changed

- Build script now does not combine files before compilation ([#12])
- Moved assembly functions into a separate header ([`src/include/sll/assembly.h`][0.6.13/src/include/sll/assembly.h])
- Moved internal function table functions into a separate header ([`src/include/sll/ift.h`][0.6.13/src/include/sll/ift.h])
- Moved object-related functions into a separate header ([`src/include/sll/object.h`][0.6.13/src/include/sll/object.h])
- Renamed `UNREACHABLE` to `SLL_UNREACHABLE`
- Renamed all API function files to end with a `_api` suffix to avoid name collisions with other files

### Removed

- ANSI escape sequences in error printing (Thus invalidating [#2])
- `sll_platform_setup_console` function

## [0.6.12] - 2021-08-19

### Added

- An option to enable ANSI escape codes (`-C`) to color the output
- An option to print the version of sll (`-V`)
- Looping operators (`SLL_OBJECT_TYPE_FOR`, `SLL_OBJECT_TYPE_WHILE` and `SLL_OBJECT_TYPE_LOOP`) now store their inner scope id
- Macros for ternary and quaternary operators
- New reference-counting based GC for strings and arrays (`sll_array_release` and `sll_string_release` are now `SLL_ACQUIRE`)
- Proper clean-up code generation after loops
- Six new assembly instructions: `SLL_ASSEMBLY_INSTRUCTION_TYPE_LENGTH`, `SLL_ASSEMBLY_INSTRUCTION_TYPE_COPY`, `SLL_ASSEMBLY_INSTRUCTION_TYPE_ACCESS`, `SLL_ASSEMBLY_INSTRUCTION_TYPE_ACCESS_TWO`, `SLL_ASSEMBLY_INSTRUCTION_TYPE_ACCESS_THREE`, `SLL_ASSEMBLY_INSTRUCTION_TYPE_LOAD_DEL`
- The `SLL_VERSION_SHA` and `SLL_VERSION_FULL_SHA` macros hold the current commits' hash
- Two new operators: length (`$`) and access (`:`) (as well as their `SLL_OBJECT_TYPE_` counterparts)

### Changed

- Changed `SLL_ASSEMBLY_INSTRUCTION_TYPE_DEL` to `SLL_ASSEMBLY_INSTRUCTION_TYPE_LOAD_DEL`
- Replaced `UINTxx_MAX` with `SLL_MAX_yy` where applicable ([#10])
- Rewritten assembly optimizer
- The new file extension for an assembly is `.sla` and for a compiled object `.slc`
- The test child process now uses a file to communicate its result instead of the piped standard output

### Removed

- Heap functions (`sll_allocate_raw`, `sll_deallocate_raw`, `sll_reset_allocator`, `sll_array_release` and `sll_string_release`)
- Heap-related types and structures
- The [`src/sll/allocator.c`][0.6.11/src/sll/allocator.c] file
- The `sll_api_string_ref_count` internal function
- Unused heap-related macros

## [0.6.11] - 2021-08-18

### Added

- Functions now store their inner scope id
- Jump instructions are no longer generated in unreachable locations
- Macro constants for limits of sll types (`SLL_MAX_xxx`)
- Support for variadic macros in the build script

### Changed

- `sll_arg_count_t` and `sll_statement_count_t` are now 32-bit values
- The sll library is now named `sll-{major}.{minor}.{patch}` instead of `sll`

### Removed

- Coverage data is now an experimental feature and has been moved to a [different branch][test-coverage]

## [0.6.10] - 2021-08-14

### Added

- Arrays
- Fix for Windows runner in Github Actions
- Four new API functions: `sll_api_string_convert`, `sll_api_string_length`, `sll_api_string_ref_count` and `sll_api_util_ref_count`
- Four new assembly instructions: `SLL_ASSEMBLY_INSTRUCTION_TYPE_PACK`, `SLL_ASSEMBLY_INSTRUCTION_TYPE_PACK_ZERO`, `SLL_ASSEMBLY_INSTRUCTION_TYPE_PACK_ONE` and `SLL_ASSEMBLY_INSTRUCTION_TYPE_DEL`
- Four new errors: `SLL_ERROR_UNMATCHED_ARRAY_OPEN_BRACKETS`, `SLL_ERROR_UNMATCHED_ARRAY_CLOSE_BRACKETS`, `SLL_ERROR_ARRAY_TOO_LONG` and `SLL_ERROR_INVALID_STACK_INDEX`
- Object to string conversion functions: `sll_object_to_string` and `sll_object_to_string_length`
- The build script now parses the [`src/include/sll/version.h`][0.6.10/src/include/sll/version.h] file
- Variables are now released after their last reference

#### Arrays

Literally, just arrays.

### Changed

- `ASSERT` has been renamed to `SLL_ASSERT`
- `memcpy()` is now used instead of loops
- Operators were moved to a separate file ([`src/sll/operator.c`][0.6.10/src/sll/operator.c])
- [`src/sll/string_heap.c`][0.6.9/src/sll/string_heap.c] was split into [`src/sll/allocator.c`][0.6.10/src/sll/allocator.c] and [`src/sll/string.c`][0.6.10/src/sll/string.c]
- String heap functions were renamed to generic allocator functions and are now used by both arrays and strings

## [0.6.9] - 2021-08-12

**Name Change: From *Lisp Like Language* to *Sl Lang***

### Added

- New operator: variable declaration (`(#)`)
- `SLL_MAX_ERROR` macro constant
- String heap allocator
- Test runner

### Changed

- Renamed all functions and macros
- Renamed [`src/include/lll`][0.6.8/src/include/lll] to [`src/include/sll`][0.6.9/src/include/sll]
- Renamed [`src/lll`][0.6.8/src/lll] to [`src/sll`][0.6.9/src/sll]
- Split [`src/_build/main.py`][0.6.8/src/_build/main.py] into [`src/_build/build.py`][0.6.9/src/_build/build.py], [`src/_build/header.py`][0.6.9/src/_build/header.py], [`src/_build/main.py`][0.6.9/src/_build/main.py] and [`src/_build/util.py`][0.6.9/src/_build/util.py]
- `sll_platform_free_page` now takes the page size as the second parameter

### Removed

- Unused file: [`build.py`][0.6.8/build.py]

## [0.6.8] - 2021-08-07

### Changed

- Moved [`build.py`][0.6.7/build.py] to [`src/_build/main.py`][0.6.8/src/_build/main.py]

## [0.6.7] - 2021-08-07

### Added

- New type: `lll_bool_t`
- Path-related functions: `lll_api_path_absolute`, `lll_api_path_exists`, `lll_api_path_is_dir`, `lll_api_path_relative` and `lll_api_path_size`
- Platform-dependent path functions: `lll_platform_path_absolute`, `lll_platform_path_exists` and `lll_platform_path_is_directory`

### Changed

- Minor fixes to the build script

## [0.6.6] - 2021-08-07

### Added

- A static zero-length string to reduce object allocation
- More verbose logs generated
- Platform name string (`lll_platform_string`)
- String release function (`lll_string_release`)
- The ability to pass arguments into the VM (`-A` option, `lll_set_argument_count` and `lll_set_argument` functions)
- Two new assembly instructions: `LLL_ASSEMBLY_INSTRUCTION_TYPE_CALL_ZERO` and `LLL_ASSEMBLY_INSTRUCTION_TYPE_CALL_ONE`

### Changed

- Renamed [`src/lll_lib`][0.6.5/src/lll_lib] to [`src/lll`][0.6.6/src/lll]
- The build script properly indents output from child processes
- Updated the assembly optimizer
- Updated the optimizer

### Removed

- String comparison and conversion functions
- Unused file constants: `LLL_API_FILE_APPEND`, `LLL_API_FILE_READ` and `LLL_API_FILE_WRITE`

## [0.6.5] - 2021-08-03

### Added

- Ability to create an input stream from a buffer (`lll_stream_create_input_from_buffer`)
- Standalone compilation mode

### Changed

- Rewritten [`build.py`][0.6.5/build.py]

## [0.6.4] - 2021-08-03

### Added

- File IO functions: `lll_api_open_file`, `lll_api_close_file` and `lll_api_write_file`
- New error: `LLL_ERROR_INTERNAL_FUNCTION_NAME_NOT_ASCII`
- String compare functions: `lll_string_compare`, `lll_string_compare_ascii`, `lll_string_compare_ignore_case` and `lll_string_compare_ignore_case_ascii`
- String convert functions: `lll_string_to_ascii`, `lll_string_length_as_bytes`, `lll_string_to_bytes`, `lll_string_to_stream` and `lll_string_encode_char_to_stream`

### Changed

- Minor tweaks & changes to the build script and cli program
- Moved console setup code to `lll_platform_setup_console`
- Renamed `time_current` and `time_current_nanos` into `time` and `time_nanos`
- Split [`src/include/lll/core.h`][0.6.3/src/include/lll/core.h] into [`src/include/lll/io.h`][0.6.4/src/include/lll/io.h], [`src/include/lll/stream.h`][0.6.4/src/include/lll/stream.h] and [`src/include/lll/string.h`][0.6.4/src/include/lll/string.h]

## [0.6.3] - 2021-08-02

### Added

- `sll_api_get_time_float` function
- Support for floating-point values
- The `INTERNAL_FUNCTION` macro can be used to declare default internal function
- The CLI tries to load each file as a compiled object (`<file>.lllc`)

### Changed

- Moved platform-dependent code to [`src/lll_lib/platform/linux.c`][0.6.3/src/lll_lib/platform/linux.c] and [`src/lll_lib/platform/windows.c`][0.6.3/src/lll_lib/platform/windows.c]
- Removed duplicated code in the builder script
- Renamed [`src/include/lll_lib_api.h`][0.6.2/src/include/lll_lib_api.h] and [`src/include/version.h`][0.6.2/src/include/version.h] to [`src/include/lll/api.h`][0.6.3/src/include/lll/api.h] and [`src/include/lll/version.h`][0.6.3/src/include/lll/version.h]
- `__SLL_IMPORT_EXPORT` has been renamed to `__SLL_FUNC`
- Split [`src/include/lll_lib.h`][0.6.2/src/include/lll_lib.h] to [`src/include/lll/common.h`][0.6.3/src/include/lll/common.h], [`src/include/lll/constants.h`][0.6.3/src/include/lll/constants.h], [`src/include/lll/core.h`][0.6.3/src/include/lll/core.h], [`src/include/lll/platform.h`][0.6.3/src/include/lll/platform.h] and [`src/include/lll/types.h`][0.6.3/src/include/lll/types.h]

## [0.6.2] - 2021-08-01

### Added

- Implemented identifier exporting (the `(##)` operator)
- `LLL_ASSEMBLY_INSTRUCTION)TYPE_CALL_POP` instruction
- New operator: `(--)` (import)

### Changed

- Bugs in the optimizer have been fixed
- Internal function `time` has been renamed to `time_current`
- `lll_parse_all_object` not requires a function used to load external files

### Removed

- Option to merge modules (`-m`). Modules are now merged automatically
- Types and functions related to import tables

## [0.6.1] - 2021-08-01

### Added

- Ability to export identifiers
- Export operator (`(##)`)

### Changed

- Changed some structure and type definitions

## [0.6.0] - 2021-07-31

### Added

- CLI tries to load files from the `<executable_directory>/lib/` directory
- Internal functions
- `lll_api_get_time` function in [`src/lll_lib/api/time.c`][0.6.0/src/lll_lib/api/time.c]
- [`src/include/lll_lib_api.h`][0.6.0/src/include/lll_lib_api.h]
- [`src/lll_lib/lib/time.lll`][0.6.0/src/lll_lib/lib/time.lll]

### Changed

- Fixed the builder script

## [0.5.2] - 2021-07-31

### Changed

- `(@@)` (return) operator now compiles into the exit operator (`(@)`) if used outside of a function
- `ASSERT` macro now only works in debug builds
- VM now uses a call stack instead of the `LLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_II` instruction
- VM now uses a stack index instead of a moving pointer

## [0.5.1] - 2021-07-30

### Changed

- Function `lll_run_assembly` has been renamed to `lll_execute_assembly`
- `lll_compare_runtime_object`, `lll_create_string`, `lll_runtime_object_size` and `lll_runtime_object_nonzero` functions are now public
- Runtime object functions (`lll_compare_runtime_object` and `lll_runtime_object_nonzero`) have been moved to [`src/lll_lib/object.c`][0.5.1/src/lll_lib/object.c]

## [0.5.0] - 2021-07-30

### Added

- Assembly instructions used to store small integer values (`-1`, `0`, `1`, `2`, `3` and `4`)
- `lll_optimize_object` function
- Two new operators: `(>-)` (do-while loop) and `(><)` (infinite loop)

### Changed

- Every single object uses the same structure
- `lll_optimize_metadata` function has been moved from [`src/lll_lib/optimize.c`][0.4.1/src/lll_lib/optimize.c] to [`src/lll_lib/metadata.c`][0.5.0/src/lll_lib/metadata.c]
- `lll_remove padding` function has been moved to [`src/lll_lib/padding.c`][0.5.0/src/lll_lib/padding.c]

### Removed

- `LLL_OBJECT_TYPE_NIL`: `nil` values get compiled directly into zeros

## [0.4.1] - 2021-07-29

### Added

- A level-3 optimization (`-O3`), which performs global optimization of the compiled object
- Assembly instructions for returning different types
- Errors related to invalid instructions, stack corruption and an invalid instruction index
- `LLL_ASSEMBLY_INSTRUCTION_TYPE_POP_ROT` instruction
- Two new operators: `(@)` (exit) and `(@@)` (return)

### Changed

- In a debug build, the `UNREACHABLE()` macro expands into `ASSERT(!"Unreachable")`
- Operator assembly instruction can be used directly on a variable

### Removed

- `LLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_NIL` instruction
- `lll_optimize_object` function
- `LLL_RUNTIME_OBJECT_TYPE_TRUE` and `LLL_RUNTIME_OBJECT_TYPE_FALSE` runtime object types

## [0.4.0] - 2021-07-28

### Added

- Assembly instructions used for pushing the current instruction index and for storing a variable and removing it of the stack
- Implemented `lll_run_assembly`

### Changed

- Assembly instruction data is now saved and read from files
- Assembly instruction that modify the instruction index can jump to relative offsets

### Removed

- Unimplemented language-generation functions and CLI arguments

## [0.3.5] - 2021-07-28

### Added

- Ability to save the generated assembly code (option `-a`)
- Assembly generator and related structures, types, functions and macros
- Option for printing assembly (`-P`)

## [0.3.4] - 2021-07-27

### Added

- `lll_optimize_metadata` function
- Option for language generation (`-g`)
- Option for saving the compiled object (`-c`)

## [0.3.3] - 2021-07-26

### Added

- Two new types: `lll_float_t` and `lll_time_t`

### Changed

- Moved `_create_string` and `_object_size` functions to [`src/lll_lib/util.c`][0.3.3/src/lll_lib/util.c]

## [0.3.2] - 2021-07-25

### Added

- `_create_string` function
- `lll_merge_import` function merges string

### Changed

- Renamed `uintXX_t` to custom types

## [0.3.1] - 2021-07-25

### Changed

- Fixed bugs related to bit shifting of narrow types in variable-length integer encoding
- Fixed other bugs

## [0.3.0] - 2021-07-25

### Changed

- Integers are now encoded differently

## [0.2.3] - 2021-07-25

### Changed

- Rewritten the help document

### Removed

- Object modifiers
- Unused macros related to assembly generation

## [0.2.2] - 2021-07-23

### Changed

- All object are now 8-byte aligned
- Identifier list compassion has been sped up
- Renamed some options

## [0.2.1] - 2021-07-03

### Added

- Debug object also store the line and column number

### Changed

- All integer are now stored as 64-bit value

## [0.2.0] - 2021-07-03

### Added

- Different structures for each object type

### Removed

- Object conversion functions

## [0.1.13] - 2021-07-02

*Nothing*

## [0.1.12] - 2021-07-02

### Changed

- Added builder script option for code generation (`--generate-code`)

## [0.1.11] - 2021-07-02

### Changed

- CLI now executes the code by default
- Rewritten help documentation

## [0.1.10] - 2021-07-01

### Changed

- Replaced `printf()` call in the CLI now print data using `print_str`, `print_str_const` and `print_int`

## [0.1.9] - 2021-07-01

### Changed

- Fixed bugs in the builder script

## [0.1.8] - 2021-07-01

### Added

- Function for executing code (`lll_run_compiled_object`)
- Options for running the program (`-r` / `-r0`) and generating output (`-g` / `-g0`)

### Changed

- Moved the example file to the [`example/`][0.1.8/example] directory
- The help string has been moved to a separate file ([`rsrc/help.txt`][0.1.8/rsrc/help.txt])

## [0.1.7] - 2021-07-01

### Changed

- Changed from the MIT License to the BSD 3-Clause License
- Implemented code generation for functions

## [0.1.6] - 2021-06-18

### Added

- List of volatile CPU registers

### Changed

- API functions are now marked with `__LLL_API_FUNCTION`
- Volatile CPU registers are now saved across function calls

### Removed

- Declarations of unused private functions

## [0.1.5] - 2021-06-17

### Added

- Functions tables are now used to store function arguments

### Changed

- Functions and loops have inner scopes

## [0.1.4] - 2021-06-16

### Added

- Support for functions

### Changed

- Rewritten the syntax highlight

## [0.1.3] - 2021-06-16

### Added

- Options for printing the help message
- Versions!

## [0.1.2] - 2021-06-15

Unfortunately, no versions were assigned before 2021-06-15 (:disappointed:), so the change log for version 0.1.2 (and below) consists of everything added up to that point.

[0.7.10]: https://github.com/sl-lang/sll/compare/sll-v0.7.9...main
[0.7.9]: https://github.com/sl-lang/sll/compare/sll-v0.7.8...sll-v0.7.9
[0.7.8]: https://github.com/sl-lang/sll/compare/sll-v0.7.7...sll-v0.7.8
[0.7.7]: https://github.com/sl-lang/sll/compare/sll-v0.7.6...sll-v0.7.7
[0.7.6]: https://github.com/sl-lang/sll/compare/sll-v0.7.5...sll-v0.7.6
[0.7.5]: https://github.com/sl-lang/sll/compare/sll-v0.7.4...sll-v0.7.5
[0.7.4]: https://github.com/sl-lang/sll/compare/sll-v0.7.3...sll-v0.7.4
[0.7.3]: https://github.com/sl-lang/sll/compare/sll-v0.7.2...sll-v0.7.3
[0.7.2]: https://github.com/sl-lang/sll/compare/sll-v0.7.1...sll-v0.7.2
[0.7.1]: https://github.com/sl-lang/sll/compare/sll-v0.7.0...sll-v0.7.1
[0.7.0]: https://github.com/sl-lang/sll/compare/sll-v0.6.37...sll-v0.7.0
[0.6.37]: https://github.com/sl-lang/sll/compare/sll-v0.6.36...sll-v0.6.37
[0.6.36]: https://github.com/sl-lang/sll/compare/sll-v0.6.35...sll-v0.6.36
[0.6.35]: https://github.com/sl-lang/sll/compare/sll-v0.6.34...sll-v0.6.35
[0.6.34]: https://github.com/sl-lang/sll/compare/sll-v0.6.33...sll-v0.6.34
[0.6.33]: https://github.com/sl-lang/sll/compare/sll-v0.6.32...sll-v0.6.33
[0.6.32]: https://github.com/sl-lang/sll/compare/sll-v0.6.31...sll-v0.6.32
[0.6.31]: https://github.com/sl-lang/sll/compare/sll-v0.6.30...sll-v0.6.31
[0.6.30]: https://github.com/sl-lang/sll/compare/sll-v0.6.29...sll-v0.6.30
[0.6.29]: https://github.com/sl-lang/sll/compare/sll-v0.6.28...sll-v0.6.29
[0.6.28]: https://github.com/sl-lang/sll/compare/sll-v0.6.27...sll-v0.6.28
[0.6.27]: https://github.com/sl-lang/sll/compare/sll-v0.6.26...sll-v0.6.27
[0.6.26]: https://github.com/sl-lang/sll/compare/sll-v0.6.25...sll-v0.6.26
[0.6.25]: https://github.com/sl-lang/sll/compare/sll-v0.6.24...sll-v0.6.25
[0.6.24]: https://github.com/sl-lang/sll/compare/sll-v0.6.23...sll-v0.6.24
[0.6.23]: https://github.com/sl-lang/sll/compare/sll-v0.6.22...sll-v0.6.23
[0.6.22]: https://github.com/sl-lang/sll/compare/sll-v0.6.21...sll-v0.6.22
[0.6.21]: https://github.com/sl-lang/sll/compare/sll-v0.6.20...sll-v0.6.21
[0.6.20]: https://github.com/sl-lang/sll/compare/sll-v0.6.19...sll-v0.6.20
[0.6.19]: https://github.com/sl-lang/sll/compare/sll-v0.6.18...sll-v0.6.19
[0.6.18]: https://github.com/sl-lang/sll/compare/sll-v0.6.17...sll-v0.6.18
[0.6.17]: https://github.com/sl-lang/sll/compare/sll-v0.6.16...sll-v0.6.17
[0.6.16]: https://github.com/sl-lang/sll/compare/sll-v0.6.15...sll-v0.6.16
[0.6.15]: https://github.com/sl-lang/sll/compare/sll-v0.6.14...sll-v0.6.15
[0.6.14]: https://github.com/sl-lang/sll/compare/sll-v0.6.13...sll-v0.6.14
[0.6.13]: https://github.com/sl-lang/sll/compare/sll-v0.6.12...sll-v0.6.13
[0.6.12]: https://github.com/sl-lang/sll/compare/sll-v0.6.11...sll-v0.6.12
[0.6.11]: https://github.com/sl-lang/sll/compare/sll-v0.6.10...sll-v0.6.11
[0.6.10]: https://github.com/sl-lang/sll/compare/sll-v0.6.9...sll-v0.6.10
[0.6.9]: https://github.com/sl-lang/sll/compare/lll-v0.6.8...sll-v0.6.9
[0.6.8]: https://github.com/sl-lang/sll/compare/lll-v0.6.7...lll-v0.6.8
[0.6.7]: https://github.com/sl-lang/sll/compare/lll-v0.6.6...lll-v0.6.7
[0.6.6]: https://github.com/sl-lang/sll/compare/lll-v0.6.5...lll-v0.6.6
[0.6.5]: https://github.com/sl-lang/sll/compare/lll-v0.6.4...lll-v0.6.5
[0.6.4]: https://github.com/sl-lang/sll/compare/lll-v0.6.3...lll-v0.6.4
[0.6.3]: https://github.com/sl-lang/sll/compare/lll-v0.6.2...lll-v0.6.3
[0.6.2]: https://github.com/sl-lang/sll/compare/lll-v0.6.1...lll-v0.6.2
[0.6.1]: https://github.com/sl-lang/sll/compare/lll-v0.6.0...lll-v0.6.1
[0.6.0]: https://github.com/sl-lang/sll/compare/lll-v0.5.2...lll-v0.6.0
[0.5.2]: https://github.com/sl-lang/sll/compare/lll-v0.5.1...lll-v0.5.2
[0.5.1]: https://github.com/sl-lang/sll/compare/lll-v0.5.0...lll-v0.5.1
[0.5.0]: https://github.com/sl-lang/sll/compare/lll-v0.4.1...lll-v0.5.0
[0.4.1]: https://github.com/sl-lang/sll/compare/lll-v0.4.0...lll-v0.4.1
[0.4.0]: https://github.com/sl-lang/sll/compare/lll-v0.3.5...lll-v0.4.0
[0.3.5]: https://github.com/sl-lang/sll/compare/lll-v0.3.4...lll-v0.3.5
[0.3.4]: https://github.com/sl-lang/sll/compare/lll-v0.3.3...lll-v0.3.4
[0.3.3]: https://github.com/sl-lang/sll/compare/lll-v0.3.2...lll-v0.3.3
[0.3.2]: https://github.com/sl-lang/sll/compare/lll-v0.3.1...lll-v0.3.2
[0.3.1]: https://github.com/sl-lang/sll/compare/lll-v0.3.0...lll-v0.3.1
[0.3.0]: https://github.com/sl-lang/sll/compare/lll-v0.2.3...lll-v0.3.0
[0.2.3]: https://github.com/sl-lang/sll/compare/lll-v0.2.2...lll-v0.2.3
[0.2.2]: https://github.com/sl-lang/sll/compare/lll-v0.2.1...lll-v0.2.2
[0.2.1]: https://github.com/sl-lang/sll/compare/lll-v0.2.0...lll-v0.2.1
[0.2.0]: https://github.com/sl-lang/sll/compare/lll-v0.1.13...lll-v0.2.0
[0.1.13]: https://github.com/sl-lang/sll/compare/lll-v0.1.12...lll-v0.1.13
[0.1.12]: https://github.com/sl-lang/sll/compare/lll-v0.1.11...lll-v0.1.12
[0.1.11]: https://github.com/sl-lang/sll/compare/lll-v0.1.10...lll-v0.1.11
[0.1.10]: https://github.com/sl-lang/sll/compare/lll-v0.1.9...lll-v0.1.10
[0.1.9]: https://github.com/sl-lang/sll/compare/lll-v0.1.8...lll-v0.1.9
[0.1.8]: https://github.com/sl-lang/sll/compare/lll-v0.1.7...lll-v0.1.8
[0.1.7]: https://github.com/sl-lang/sll/compare/lll-v0.1.6...lll-v0.1.7
[0.1.6]: https://github.com/sl-lang/sll/compare/lll-v0.1.5...lll-v0.1.6
[0.1.5]: https://github.com/sl-lang/sll/compare/lll-v0.1.4...lll-v0.1.5
[0.1.4]: https://github.com/sl-lang/sll/compare/lll-v0.1.3...lll-v0.1.4
[0.1.3]: https://github.com/sl-lang/sll/compare/lll-v0.1.2...lll-v0.1.3
[0.1.2]: https://github.com/sl-lang/sll/tree/v0.1.2
[#255]: https://github.com/sl-lang/sll/issues/255
[#254]: https://github.com/sl-lang/sll/issues/254
[#247]: https://github.com/sl-lang/sll/issues/247
[#246]: https://github.com/sl-lang/sll/issues/246
[#245]: https://github.com/sl-lang/sll/issues/245
[#244]: https://github.com/sl-lang/sll/issues/244
[#243]: https://github.com/sl-lang/sll/issues/243
[#238]: https://github.com/sl-lang/sll/issues/238
[#237]: https://github.com/sl-lang/sll/issues/237
[#235]: https://github.com/sl-lang/sll/issues/235
[#233]: https://github.com/sl-lang/sll/issues/233
[#232]: https://github.com/sl-lang/sll/issues/232
[#230]: https://github.com/sl-lang/sll/issues/230
[#229]: https://github.com/sl-lang/sll/issues/229
[#228]: https://github.com/sl-lang/sll/issues/228
[#227]: https://github.com/sl-lang/sll/issues/227
[#224]: https://github.com/sl-lang/sll/issues/224
[#222]: https://github.com/sl-lang/sll/issues/222
[#221]: https://github.com/sl-lang/sll/issues/221
[#220]: https://github.com/sl-lang/sll/issues/220
[#219]: https://github.com/sl-lang/sll/issues/219
[#218]: https://github.com/sl-lang/sll/issues/218
[#217]: https://github.com/sl-lang/sll/issues/217
[#216]: https://github.com/sl-lang/sll/issues/216
[#215]: https://github.com/sl-lang/sll/issues/215
[#214]: https://github.com/sl-lang/sll/issues/214
[#213]: https://github.com/sl-lang/sll/issues/213
[#212]: https://github.com/sl-lang/sll/issues/212
[#211]: https://github.com/sl-lang/sll/issues/211
[#210]: https://github.com/sl-lang/sll/issues/210
[#209]: https://github.com/sl-lang/sll/issues/209
[#208]: https://github.com/sl-lang/sll/issues/208
[#207]: https://github.com/sl-lang/sll/issues/207
[#206]: https://github.com/sl-lang/sll/issues/206
[#205]: https://github.com/sl-lang/sll/issues/205
[#203]: https://github.com/sl-lang/sll/issues/203
[#202]: https://github.com/sl-lang/sll/issues/202
[#201]: https://github.com/sl-lang/sll/issues/201
[#200]: https://github.com/sl-lang/sll/issues/200
[#199]: https://github.com/sl-lang/sll/issues/199
[#197]: https://github.com/sl-lang/sll/issues/197
[#196]: https://github.com/sl-lang/sll/issues/196
[#195]: https://github.com/sl-lang/sll/issues/195
[#194]: https://github.com/sl-lang/sll/issues/194
[#193]: https://github.com/sl-lang/sll/issues/193
[#192]: https://github.com/sl-lang/sll/issues/192
[#191]: https://github.com/sl-lang/sll/issues/191
[#190]: https://github.com/sl-lang/sll/issues/190
[#189]: https://github.com/sl-lang/sll/issues/189
[#188]: https://github.com/sl-lang/sll/issues/188
[#187]: https://github.com/sl-lang/sll/issues/187
[#186]: https://github.com/sl-lang/sll/issues/186
[#185]: https://github.com/sl-lang/sll/issues/185
[#184]: https://github.com/sl-lang/sll/issues/184
[#183]: https://github.com/sl-lang/sll/issues/183
[#182]: https://github.com/sl-lang/sll/issues/182
[#181]: https://github.com/sl-lang/sll/issues/181
[#180]: https://github.com/sl-lang/sll/issues/180
[#179]: https://github.com/sl-lang/sll/issues/179
[#177]: https://github.com/sl-lang/sll/issues/177
[#176]: https://github.com/sl-lang/sll/issues/176
[#175]: https://github.com/sl-lang/sll/issues/175
[#174]: https://github.com/sl-lang/sll/issues/174
[#173]: https://github.com/sl-lang/sll/issues/173
[#172]: https://github.com/sl-lang/sll/issues/172
[#171]: https://github.com/sl-lang/sll/issues/171
[#170]: https://github.com/sl-lang/sll/issues/170
[#169]: https://github.com/sl-lang/sll/issues/169
[#168]: https://github.com/sl-lang/sll/issues/168
[#167]: https://github.com/sl-lang/sll/issues/167
[#166]: https://github.com/sl-lang/sll/issues/166
[#165]: https://github.com/sl-lang/sll/issues/165
[#164]: https://github.com/sl-lang/sll/issues/164
[#163]: https://github.com/sl-lang/sll/issues/163
[#162]: https://github.com/sl-lang/sll/issues/162
[#161]: https://github.com/sl-lang/sll/issues/161
[#160]: https://github.com/sl-lang/sll/issues/160
[#159]: https://github.com/sl-lang/sll/issues/159
[#158]: https://github.com/sl-lang/sll/issues/158
[#157]: https://github.com/sl-lang/sll/issues/157
[#156]: https://github.com/sl-lang/sll/issues/156
[#155]: https://github.com/sl-lang/sll/issues/155
[#154]: https://github.com/sl-lang/sll/issues/154
[#153]: https://github.com/sl-lang/sll/issues/153
[#152]: https://github.com/sl-lang/sll/issues/152
[#151]: https://github.com/sl-lang/sll/issues/151
[#150]: https://github.com/sl-lang/sll/issues/150
[#149]: https://github.com/sl-lang/sll/issues/149
[#148]: https://github.com/sl-lang/sll/issues/148
[#147]: https://github.com/sl-lang/sll/issues/147
[#146]: https://github.com/sl-lang/sll/issues/146
[#145]: https://github.com/sl-lang/sll/issues/145
[#144]: https://github.com/sl-lang/sll/issues/144
[#143]: https://github.com/sl-lang/sll/issues/143
[#142]: https://github.com/sl-lang/sll/issues/142
[#141]: https://github.com/sl-lang/sll/issues/141
[#140]: https://github.com/sl-lang/sll/issues/140
[#139]: https://github.com/sl-lang/sll/issues/139
[#138]: https://github.com/sl-lang/sll/issues/138
[#137]: https://github.com/sl-lang/sll/issues/137
[#136]: https://github.com/sl-lang/sll/issues/136
[#135]: https://github.com/sl-lang/sll/issues/135
[#134]: https://github.com/sl-lang/sll/issues/134
[#132]: https://github.com/sl-lang/sll/issues/132
[#130]: https://github.com/sl-lang/sll/issues/130
[#129]: https://github.com/sl-lang/sll/issues/129
[#128]: https://github.com/sl-lang/sll/issues/128
[#127]: https://github.com/sl-lang/sll/issues/127
[#126]: https://github.com/sl-lang/sll/issues/126
[#125]: https://github.com/sl-lang/sll/issues/125
[#124]: https://github.com/sl-lang/sll/issues/124
[#123]: https://github.com/sl-lang/sll/issues/123
[#122]: https://github.com/sl-lang/sll/issues/122
[#121]: https://github.com/sl-lang/sll/issues/121
[#120]: https://github.com/sl-lang/sll/issues/120
[#119]: https://github.com/sl-lang/sll/issues/119
[#118]: https://github.com/sl-lang/sll/issues/118
[#117]: https://github.com/sl-lang/sll/issues/117
[#116]: https://github.com/sl-lang/sll/issues/116
[#115]: https://github.com/sl-lang/sll/issues/115
[#114]: https://github.com/sl-lang/sll/issues/114
[#113]: https://github.com/sl-lang/sll/issues/113
[#112]: https://github.com/sl-lang/sll/issues/112
[#111]: https://github.com/sl-lang/sll/issues/111
[#110]: https://github.com/sl-lang/sll/issues/110
[#109]: https://github.com/sl-lang/sll/issues/109
[#108]: https://github.com/sl-lang/sll/issues/108
[#107]: https://github.com/sl-lang/sll/issues/107
[#106]: https://github.com/sl-lang/sll/issues/106
[#105]: https://github.com/sl-lang/sll/issues/105
[#104]: https://github.com/sl-lang/sll/issues/104
[#103]: https://github.com/sl-lang/sll/issues/103
[#102]: https://github.com/sl-lang/sll/issues/102
[#101]: https://github.com/sl-lang/sll/issues/101
[#100]: https://github.com/sl-lang/sll/issues/100
[#99]: https://github.com/sl-lang/sll/issues/99
[#98]: https://github.com/sl-lang/sll/issues/98
[#97]: https://github.com/sl-lang/sll/issues/97
[#96]: https://github.com/sl-lang/sll/issues/96
[#95]: https://github.com/sl-lang/sll/issues/95
[#94]: https://github.com/sl-lang/sll/issues/94
[#93]: https://github.com/sl-lang/sll/issues/93
[#92]: https://github.com/sl-lang/sll/issues/92
[#91]: https://github.com/sl-lang/sll/issues/91
[#90]: https://github.com/sl-lang/sll/issues/90
[#89]: https://github.com/sl-lang/sll/issues/89
[#88]: https://github.com/sl-lang/sll/issues/88
[#87]: https://github.com/sl-lang/sll/issues/87
[#86]: https://github.com/sl-lang/sll/issues/86
[#85]: https://github.com/sl-lang/sll/issues/85
[#84]: https://github.com/sl-lang/sll/issues/84
[#83]: https://github.com/sl-lang/sll/issues/83
[#82]: https://github.com/sl-lang/sll/issues/82
[#81]: https://github.com/sl-lang/sll/issues/81
[#80]: https://github.com/sl-lang/sll/issues/80
[#79]: https://github.com/sl-lang/sll/issues/79
[#78]: https://github.com/sl-lang/sll/issues/78
[#77]: https://github.com/sl-lang/sll/issues/77
[#76]: https://github.com/sl-lang/sll/issues/76
[#74]: https://github.com/sl-lang/sll/issues/74
[#73]: https://github.com/sl-lang/sll/issues/73
[#72]: https://github.com/sl-lang/sll/issues/72
[#71]: https://github.com/sl-lang/sll/issues/71
[#69]: https://github.com/sl-lang/sll/issues/69
[#67]: https://github.com/sl-lang/sll/issues/67
[#66]: https://github.com/sl-lang/sll/issues/66
[#58]: https://github.com/sl-lang/sll/issues/58
[#56]: https://github.com/sl-lang/sll/issues/56
[#55]: https://github.com/sl-lang/sll/issues/55
[#54]: https://github.com/sl-lang/sll/issues/54
[#53]: https://github.com/sl-lang/sll/issues/53
[#52]: https://github.com/sl-lang/sll/issues/52
[#51]: https://github.com/sl-lang/sll/issues/51
[#50]: https://github.com/sl-lang/sll/issues/50
[#48]: https://github.com/sl-lang/sll/issues/48
[#47]: https://github.com/sl-lang/sll/issues/47
[#46]: https://github.com/sl-lang/sll/issues/46
[#44]: https://github.com/sl-lang/sll/issues/44
[#43]: https://github.com/sl-lang/sll/issues/43
[#42]: https://github.com/sl-lang/sll/issues/42
[#41]: https://github.com/sl-lang/sll/issues/41
[#40]: https://github.com/sl-lang/sll/issues/40
[#37]: https://github.com/sl-lang/sll/issues/37
[#36]: https://github.com/sl-lang/sll/issues/36
[#35]: https://github.com/sl-lang/sll/issues/35
[#34]: https://github.com/sl-lang/sll/issues/34
[#33]: https://github.com/sl-lang/sll/issues/33
[#32]: https://github.com/sl-lang/sll/issues/32
[#30]: https://github.com/sl-lang/sll/issues/30
[#29]: https://github.com/sl-lang/sll/issues/29
[#27]: https://github.com/sl-lang/sll/issues/27
[#26]: https://github.com/sl-lang/sll/issues/26
[#25]: https://github.com/sl-lang/sll/issues/25
[#23]: https://github.com/sl-lang/sll/issues/23
[#22]: https://github.com/sl-lang/sll/issues/22
[#20]: https://github.com/sl-lang/sll/issues/20
[#19]: https://github.com/sl-lang/sll/issues/19
[#18]: https://github.com/sl-lang/sll/issues/18
[#17]: https://github.com/sl-lang/sll/issues/17
[#16]: https://github.com/sl-lang/sll/issues/16
[#15]: https://github.com/sl-lang/sll/issues/15
[#14]: https://github.com/sl-lang/sll/issues/14
[#12]: https://github.com/sl-lang/sll/issues/12
[#11]: https://github.com/sl-lang/sll/issues/11
[#10]: https://github.com/sl-lang/sll/issues/10
[#8]: https://github.com/sl-lang/sll/issues/8
[#5]: https://github.com/sl-lang/sll/issues/5
[#3]: https://github.com/sl-lang/sll/issues/3
[#2]: https://github.com/sl-lang/sll/issues/2
[test-coverage]: https://github.com/sl-lang/sll/tree/test-coverage
[0.7.10/src/sll/data/operator_parser.txt]: https://github.com/sl-lang/sll/blob/main/src/sll/data/operator_parser.txt
[0.7.9/src/sll/include/sll/_internal]: https:/github.com/sl-lang/sll/tree/sll-v0.7.9/src/sll/include/sll/_internal
[0.7.8/src/sll/include/sll/_sll_internal.h]: https://github.com/sl-lang/sll/blob/sll-v0.7.8/src/sll/include/sll/_sll_internal.h
[0.7.8/src/sll/include/sll/platform]: https:/github.com/sl-lang/sll/tree/sll-v0.7.8/src/sll/include/sll/platform
[0.7.7/src/sll/include/sll/platform.h]: https://github.com/sl-lang/sll/blob/sll-v0.7.7/src/sll/include/sll/platform.h
[0.7.0/src/sll/api/_generated_raw.c]: https://github.com/sl-lang/sll/blob/sll-v0.7.0/src/sll/api/_generated_raw.c
[0.7.0/src/sll/compilation_data.c]: https://github.com/sl-lang/sll/blob/sll-v0.7.0/src/sll/compilation_data.c
[0.7.0/src/sll/data.c]: https://github.com/sl-lang/sll/blob/sll-v0.7.0/src/sll/data.c
[0.7.0/src/sll/debug_data.c]: https://github.com/sl-lang/sll/blob/sll-v0.7.0/src/sll/debug_data.c
[0.7.0/src/sll/env.c]: https://github.com/sl-lang/sll/blob/sll-v0.7.0/src/sll/env.c
[0.7.0/src/sll/include/sll/_size_types.h]: https://github.com/sl-lang/sll/blob/sll-v0.7.0/src/sll/include/sll/_size_types.h
[0.7.0/src/sll/include/sll/data.h]: https://github.com/sl-lang/sll/blob/sll-v0.7.0/src/sll/include/sll/data.h
[0.7.0/src/sll/include/sll/env.h]: https://github.com/sl-lang/sll/blob/sll-v0.7.0/src/sll/include/sll/env.h
[0.7.0/src/sll/include/sll/generated/api.h]: https://github.com/sl-lang/sll/blob/sll-v0.7.0/src/sll/include/sll/generated/api.h
[0.7.0/src/sll/include/sll/generated]: https://github.com/sl-lang/sll/tree/sll-v0.7.0/src/sll/include/sll/generated
[0.7.0/src/sll/include/sll/init.h]: https://github.com/sl-lang/sll/blob/sll-v0.7.0/src/sll/include/sll/init.h
[0.7.0/src/sll/include/sll/location.h]: https://github.com/sl-lang/sll/blob/sll-v0.7.0/src/sll/include/sll/location.h
[0.7.0/src/sll/include/sll/sandbox.h]: https://github.com/sl-lang/sll/blob/sll-v0.7.0/src/sll/include/sll/sandbox.h
[0.7.0/src/sll/include/sll/string_table.h]: https://github.com/sl-lang/sll/blob/sll-v0.7.0/src/sll/include/sll/string_table.h
[0.7.0/src/sll/include/sll/version.h]: https://github.com/sl-lang/sll/blob/sll-v0.7.0/src/sll/include/sll/version.h
[0.7.0/src/sll/init.c]: https://github.com/sl-lang/sll/blob/sll-v0.7.0/src/sll/init.c
[0.7.0/src/sll/location.c]: https://github.com/sl-lang/sll/blob/sll-v0.7.0/src/sll/location.c
[0.7.0/src/sll/sandbox.c]: https://github.com/sl-lang/sll/blob/sll-v0.7.0/src/sll/sandbox.c
[0.7.0/src/sll/size.c]: https://github.com/sl-lang/sll/blob/sll-v0.7.0/src/sll/size.c
[0.7.0/src/sll/skip.c]: https://github.com/sl-lang/sll/blob/sll-v0.7.0/src/sll/skip.c
[0.7.0/src/sll/string_table.c]: https://github.com/sl-lang/sll/blob/sll-v0.7.0/src/sll/string_table.c
[0.7.0/src/sll/version.c]: https://github.com/sl-lang/sll/blob/sll-v0.7.0/src/sll/version.c
[0.6.37/src/sll/api/_generated.c]: https://github.com/sl-lang/sll/blob/sll-v0.6.37/src/sll/api/_generated.c
[0.6.37/src/sll/include/sll/api/_generated.h]: https://github.com/sl-lang/sll/blob/sll-v0.6.37/src/sll/include/sll/api/_generated.h
[0.6.37/src/sll/include/sll/assembly.h]: https://github.com/sl-lang/sll/blob/sll-v0.6.37/src/sll/include/sll/assembly.h
[0.6.37/src/sll/include/sll/util.h]: https://github.com/sl-lang/sll/blob/sll-v0.6.37/src/sll/include/sll/util.h
[0.6.37/src/sll/node.c]: https://github.com/sl-lang/sll/blob/sll-v0.6.37/src/sll/node.c
[0.6.37/src/sll/util.c]: https://github.com/sl-lang/sll/blob/sll-v0.6.37/src/sll/util.c
[0.6.36/src/sll/cli.c]: https://github.com/sl-lang/sll/blob/sll-v0.6.36/src/sll/cli.c
[0.6.35/src/cli/main.c]: https://github.com/sl-lang/sll/blob/sll-v0.6.35/src/cli/main.c
[0.6.35/src/sll/cast.c]: https://github.com/sl-lang/sll/blob/sll-v0.6.35/src/sll/cast.c
[0.6.34/src/sll/operator.c]: https://github.com/sl-lang/sll/blob/sll-v0.6.34/src/sll/operator.c
[0.6.34/src/cli/data/help.txt]: https://github.com/sl-lang/sll/blob/sll-v0.6.34/src/cli/data/help.txt
[0.6.34/src/sll/data/assembly_optimizer.txt]: https://github.com/sl-lang/sll/blob/sll-v0.6.34/src/sll/data/assembly_optimizer.txt
[0.6.33/rsrc/help.txt]: https://github.com/sl-lang/sll/blob/sll-v0.6.33/rsrc/help.txt
[0.6.33/src/web]: https://github.com/sl-lang/sll/tree/sll-v0.6.33/src/web
[0.6.32/src/include/sll/api/util.h]: https://github.com/sl-lang/sll/blob/sll-v0.6.32/src/include/sll/api/util.h
[0.6.32/src/sll/api/util_api.c]: https://github.com/sl-lang/sll/blob/sll-v0.6.32/src/sll/api/util_api.c
[0.6.32/src/sll/debug_object.c]: https://github.com/sl-lang/sll/blob/sll-v0.6.32/src/sll/debug_object.c
[0.6.32/src/sll/platform/posix]: https://github.com/sl-lang/sll/tree/sll-v0.6.32/src/sll/platform/posix
[0.6.32/src/sll/platform/windows]: https://github.com/sl-lang/sll/tree/sll-v0.6.32/src/sll/platform/windows
[0.6.32/src/web]: https://github.com/sl-lang/sll/tree/sll-v0.6.32/src/web
[0.6.31/src/sll/platform/posix.c]: https://github.com/sl-lang/sll/blob/sll-v0.6.31/src/sll/platform/posix.c
[0.6.31/src/sll/platform/windows.c]: https://github.com/sl-lang/sll/blob/sll-v0.6.31/src/sll/platform/windows.c
[0.6.29/src/sll/node.c]: https://github.com/sl-lang/sll/blob/sll-v0.6.29/src/sll/node.c
[0.6.28/src/include/sll/init.h]: https://github.com/sl-lang/sll/blob/sll-v0.6.28/src/include/sll/init.h
[0.6.28/src/sll/deinit.c]: https://github.com/sl-lang/sll/blob/sll-v0.6.28/src/sll/deinit.c
[0.6.28/src/sll/init.c]: https://github.com/sl-lang/sll/blob/sll-v0.6.28/src/sll/init.c
[0.6.27/src/include/sll/cast.h]: https://github.com/sl-lang/sll/blob/sll-v0.6.27/src/include/sll/cast.h
[0.6.25/examples]: https://github.com/sl-lang/sll/tree/sll-v0.6.25/examples
[0.6.25/src/include/sll/init.h]: https://github.com/sl-lang/sll/blob/sll-v0.6.25/src/include/sll/init.h
[0.6.25/src/include/sll/util.h]: https://github.com/sl-lang/sll/blob/sll-v0.6.25/src/include/sll/util.h
[0.6.25/src/sll/deinit.c]: https://github.com/sl-lang/sll/blob/sll-v0.6.25/src/sll/deinit.c
[0.6.25/src/sll/init.c]: https://github.com/sl-lang/sll/blob/sll-v0.6.25/src/sll/init.c
[0.6.24/src/sll/lib/file.sll]: https://github.com/sl-lang/sll/blob/sll-v0.6.24/src/sll/lib/file.sll
[0.6.24/src/sll/lib/types.sll]: https://github.com/sl-lang/sll/blob/sll-v0.6.24/src/sll/lib/types.sll
[0.6.24/src/sll/string.c]: https://github.com/sl-lang/sll/blob/sll-v0.6.24/src/sll/string.c
[0.6.21/src/include/sll/constants.h]: https://github.com/sl-lang/sll/blob/sll-v0.6.21/src/include/sll/constants.h
[0.6.21/src/include/sll/core.h]: https://github.com/sl-lang/sll/blob/sll-v0.6.21/src/include/sll/core.h
[0.6.20/src/sll/api/file_api.c]: https://github.com/sl-lang/sll/blob/sll-v0.6.20/src/sll/api/file_api.c
[0.6.20/src/include/sll/api]: https://github.com/sl-lang/sll/tree/sll-v0.6.20/src/include/sll/api
[0.6.19/CODE_OF_CONDUCT.md]: https://github.com/sl-lang/sll/blob/sll-v0.6.19/CODE_OF_CONDUCT.md
[0.6.19/src/cli/main.c]: https://github.com/sl-lang/sll/blob/sll-v0.6.19/src/cli/main.c
[0.6.19/src/sll/print.c]: https://github.com/sl-lang/sll/blob/sll-v0.6.19/src/sll/print.c
[0.6.19/tests]: https://github.com/sl-lang/sll/tree/sll-v0.6.19/tests
[0.6.18/src/include/sll/_api_generated.h]: https://github.com/sl-lang/sll/blob/sll-v0.6.18/src/include/sll/_api_generated.h
[0.6.18/src/sll/api/_generated.c]: https://github.com/sl-lang/sll/blob/sll-v0.6.18/src/sll/api/_generated.c
[0.6.18/src/sll/operator.c]: https://github.com/sl-lang/sll/blob/sll-v0.6.18/src/sll/operator.c
[0.6.17/src/include/sll/common.h]: https://github.com/sl-lang/sll/blob/sll-v0.6.17/src/include/sll/common.h
[0.6.16/src/sll/operator.c]: https://github.com/sl-lang/sll/blob/sll-v0.6.16/src/sll/operator.c
[0.6.16/src/sll/vm.c]: https://github.com/sl-lang/sll/blob/sll-v0.6.16/src/sll/vm.c
[0.6.15/src/sll/parse.c]: https://github.com/sl-lang/sll/blob/sll-v0.6.15/src/sll/parse.c
[0.6.15/src/sll/print.c]: https://github.com/sl-lang/sll/blob/sll-v0.6.15/src/sll/print.c
[0.6.14/src/cli]: https://github.com/sl-lang/sll/tree/sll-v0.6.14/src/cli
[0.6.14/src/sll/parse.c]: https://github.com/sl-lang/sll/blob/sll-v0.6.14/src/sll/parse.c
[0.6.13/src/include/sll/assembly.h]: https://github.com/sl-lang/sll/blob/sll-v0.6.13/src/include/sll/assembly.h
[0.6.13/src/include/sll/ift.h]: https://github.com/sl-lang/sll/blob/sll-v0.6.13/src/include/sll/ift.h
[0.6.13/src/include/sll/object.h]: https://github.com/sl-lang/sll/blob/sll-v0.6.13/src/include/sll/object.h
[0.6.13/src/sll/operator.c]: https://github.com/sl-lang/sll/blob/sll-v0.6.13/src/sll/operator.c
[0.6.11/src/sll/allocator.c]: https://github.com/sl-lang/sll/blob/sll-v0.6.11/src/sll/allocator.c
[0.6.10/src/include/sll/version.h]: https://github.com/sl-lang/sll/blob/sll-v0.6.10/src/include/sll/version.h
[0.6.10/src/sll/allocator.c]: https://github.com/sl-lang/sll/blob/sll-v0.6.10/src/sll/allocator.c
[0.6.10/src/sll/operator.c]: https://github.com/sl-lang/sll/blob/sll-v0.6.10/src/sll/operator.c
[0.6.10/src/sll/string.c]: https://github.com/sl-lang/sll/blob/sll-v0.6.10/src/sll/string.c
[0.6.9/src/_build/build.py]: https://github.com/sl-lang/sll/blob/sll-v0.6.9/src/_build/build.py
[0.6.9/src/_build/header.py]: https://github.com/sl-lang/sll/blob/sll-v0.6.9/src/_build/header.py
[0.6.9/src/_build/main.py]: https://github.com/sl-lang/sll/blob/sll-v0.6.9/src/_build/main.py
[0.6.9/src/_build/util.py]: https://github.com/sl-lang/sll/blob/sll-v0.6.9/src/_build/util.py
[0.6.9/src/include/sll]: https://github.com/sl-lang/sll/tree/sll-v0.6.9/src/include/sll
[0.6.9/src/sll]: https://github.com/sl-lang/sll/tree/sll-v0.6.9/src/sll
[0.6.9/src/sll/string_heap.c]: https://github.com/sl-lang/sll/blob/sll-v0.6.9/src/sll/string_heap.c
[0.6.8/build.py]: https://github.com/sl-lang/sll/blob/v0.6.8/build.py
[0.6.8/src/_build/main.py]: https://github.com/sl-lang/sll/blob/lll-v0.6.8/src/_build/main.py
[0.6.8/src/include/lll]: https://github.com/sl-lang/sll/tree/lll-v0.6.8/src/include/lll
[0.6.8/src/lll]: https://github.com/sl-lang/sll/tree/lll-v0.6.8/src/lll
[0.6.7/build.py]: https://github.com/sl-lang/sll/blob/lll-v0.6.7/build.py
[0.6.6/src/lll]: https://github.com/sl-lang/sll/tree/lll-v0.6.6/src/lll
[0.6.5/build.py]: https://github.com/sl-lang/sll/blob/lll-v0.6.5/build.py
[0.6.5/src/lll_lib]: https://github.com/sl-lang/sll/tree/lll-v0.6.5/src/lll_lib
[0.6.4/src/include/lll/io.h]: https://github.com/sl-lang/sll/blob/lll-v0.6.4/src/include/lll/io.h
[0.6.4/src/include/lll/stream.h]: https://github.com/sl-lang/sll/blob/lll-v0.6.4/src/include/lll/stream.h
[0.6.4/src/include/lll/string.h]: https://github.com/sl-lang/sll/blob/lll-v0.6.4/src/include/lll/string.h
[0.6.3/src/include/lll/api.h]: https://github.com/sl-lang/sll/blob/lll-v0.6.3/src/include/lll/api.h
[0.6.3/src/include/lll/common.h]: https://github.com/sl-lang/sll/blob/lll-v0.6.3/src/include/lll/common.h
[0.6.3/src/include/lll/constants.h]: https://github.com/sl-lang/sll/blob/lll-v0.6.3/src/include/lll/constants.h
[0.6.3/src/include/lll/core.h]: https://github.com/sl-lang/sll/blob/lll-v0.6.3/src/include/lll/core.h
[0.6.3/src/include/lll/platform.h]: https://github.com/sl-lang/sll/blob/lll-v0.6.3/src/include/lll/platform.h
[0.6.3/src/include/lll/types.h]: https://github.com/sl-lang/sll/blob/lll-v0.6.3/src/include/lll/types.h
[0.6.3/src/include/lll/version.h]: https://github.com/sl-lang/sll/blob/lll-v0.6.3/src/include/lll/version.h
[0.6.3/src/lll_lib/platform/linux.c]: https://github.com/sl-lang/sll/blob/lll-v0.6.3/src/lll_lib/platform/linux.c
[0.6.3/src/lll_lib/platform/windows.c]: https://github.com/sl-lang/sll/blob/lll-v0.6.3/src/lll_lib/platform/windows.c
[0.6.2/src/include/lll_lib_api.h]: https://github.com/sl-lang/sll/blob/lll-v0.6.2/src/include/lll_lib_api.h
[0.6.2/src/include/version.h]: https://github.com/sl-lang/sll/blob/lll-v0.6.2/src/include/version.h
[0.6.2/src/include/lll_lib.h]: https://github.com/sl-lang/sll/blob/lll-v0.6.2/src/include/lll_lib.h
[0.6.0/src/include/lll_lib_api.h]: https://github.com/sl-lang/sll/blob/lll-v0.6.0/src/include/lll_lib_api.h
[0.6.0/src/lll_lib/api/time.c]: https://github.com/sl-lang/sll/blob/lll-v0.6.0/src/lll_lib/api/time.c
[0.6.0/src/lll_lib/lib/time.lll]: https://github.com/sl-lang/sll/blob/lll-v0.6.0/src/lll_lib/lib/time.lll
[0.5.1/src/lll_lib/object.c]: https://github.com/sl-lang/sll/blob/lll-v0.5.1/src/lll_lib/object.c
[0.5.0/src/lll_lib/metadata.c]: https://github.com/sl-lang/sll/blob/lll-v0.5.0/src/lll_lib/metadata.c
[0.5.0/src/lll_lib/padding.c]: https://github.com/sl-lang/sll/blob/lll-v0.5.0/src/lll_lib/padding.c
[0.4.1/src/lll_lib/optimize.c]: https://github.com/sl-lang/sll/blob/lll-v0.4.1/src/lll_lib/optimize.c
[0.3.3/src/lll_lib/util.c]: https://github.com/sl-lang/sll/blob/lll-v0.3.3/src/lll_lib/util.c
[0.1.8/example]: https://github.com/sl-lang/sll/tree/lll-v0.1.8/example
[0.1.8/rsrc/help.txt]: https://github.com/sl-lang/sll/blob/lll-v0.1.8/rsrc/help.txt
