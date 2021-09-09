# The Sl Programing Language Change Log

## [0.6.18] - Ongoing
### Added
- `sll_init` and `sll_deinit` functions

## [0.6.17] - 2021-9-9
### Added
- Mappings ([#36])
- More logging related to updating from the CLI
- `sll_register_cleanup` function, which can be used for safely executing functions before key library features (like GC) are cleaned up
- JSON api
- Documentation parser
- Automatic API function code generation
- The `url.sll` module ([#29])
- Fixed bugs in the builder script
- New Constants: `int_type`, `float_type`, `char_type`, `string_type`, `array_type`, `handle_type`, `map_type`, `map_key_type` and `map_value_type`
- New type casting operator: `(::)` ([#37])
- `sll_operator_cast`, `sll_api_json_type`, `sll_lookup_handle_descriptor` and `sll_string_hash` functions
- Strings are now aligned on a 8-byte boundary to aid performance
- The GC now stores data about each call to `SLL_CREATE`, `SLL_ACQUIRE`, `SLL_RELEASE` or other runtime object creation function  

#### Mappings
Maps (Mapping Objects) can be created by arrow brackets (`<key1 value1 key2 value2>`). If the number of elements is odd, a `nil` is automatically added to the end.

#### API Function Code Generation
Each API function is now split into two parts: `sll_api_XXX` and `sll_api_XXX_raw`. The 'raw' function takes an object array and error-checks each arguments. It then calls the base function with all the arguments converted into C types. The code is generated based on the documentation.

### Changed
- Internal functions are now declared as `static`
- All `sll_operator_XXX` functions now use non-constant parameters (This has been changed to allow returning one of the operands)
- `sll_handle_descriptor_t` can also specify a `sll_handle_stringify_t` function
- Function that return a value (except for `sll__add_debug_data` and `sll_register_internal_function`) are now marked with `__SLL_CHECK_OUTPUT`

### Removed
- `sll_init` function has been replaced by platform-dependent code to automatically set-up the console
- `__SLL_RETURN_xxx` macros from [`src/include/sll/common.h`][0.6.17/src/include/sll/common.h]

## [0.6.16] - 2021-8-30
### Added
- This Change Log!
- Bit shifting operators ([#30]): `(<<)` and `(>>)` as well as their `SLL_OBJECT_TYPE_` and `SLL_ASSEMBLY_INSTRUCTION_TYPE_` counterparts (This also includes updates to the syntax highlight)
- `SLL_ERROR_UNRELEASED_OBJECTS` error: It is raised when not all objects (allocated during execution of the program) have been `SLL_RELEASE`d
- `sll_current_runtime_data` holds the current runtime data of the program (Can be used to ex. create handle types or write directly to output)
- Handles
- New static object: `handle_zero`
- New object creation macro: `SLL_FROM_HANDLE` (and `sll_handle_to_object`)
- `sll_platform_setup_console` and `sll_platform_reset_console`: On Windows, is sets the output mode to binary
- Fixed memory leak in `sll_add_string` ([#32]) and memory corruption in `sll__add_debug_data` ([#33])
- Ability to upload the executable to the website
- JSON module ([#34])
- New option for updating sll from the CLI (`-U`) ([#35])
- Fixed macro parsing in the builder script

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
- Renamed top-of-stack variables in [`src/sll/vm.c`][0.6.16/src/sll/vm.c] to `tos`
- Constant string objects in [`src/sll/vm.c`][0.6.16/src/sll/vm.c] now use part of the user-supplied stack instead of `malloc()`
- Renamed `SLL_MAX_ARGUMENT_COUNT` to `SLL_MAX_ARG_COUNT`
- `sll_execute_assembly` now expects `sll_runtime_data_t` instead of multiple parameters

### Removed
- `SLL_UNIMPLEMENTED()` statements in [`src/sll/operator.c`][0.6.16/src/sll/operator.c]
- Unused constant `INVALID_FILE_HANDLE` and unused macro `SLL_API_INVALID_FILE_HANDLE`

## [0.6.15] - 2021-8-26
### Added
- *\[POSIX only\]* Compilation in debug mode (without `--release` flag) now generates debugging information (gcc `-g` flag)
- Static objects
- Object creation functions: `sll_int_to_object`, `sll_float_to_object`
- Object allocation function: `sll_create_object`
- If a variable is used before it has been initialized, is is replaced by a zero: `(= x x x x x)` is compiled into `(= x 0 x x x)`

#### Static Objects
It is very inefficient to constantly allocate objects with common values (such as zero). The `SLL_ACQUIRE_STATIC` macro allows to acquire on of the predefined static object (`int_minus_one`, `int_zero`, `int_one`, `int_two`, `int_three`, `int_four`, `float_zero`, `float_one`, `float_half`, `float_third`, `str_zero` and `array_zero`).
Helper functions (`sll_int_to_object` and `sll_float_to_object`) can be used to automatically convert a C type to either a new or a static object.

### Changed
- Fixed Function Macro Parsing
- `internal_function_t` now uses `sll_char_t[256]` instead of `char[256]`
- Combined cleanup functions into single `atexit` function (which calls other functions in the correct order)
- Changed how runtime objects are allocated, acquired, released and deallocated
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
- The `restrict` keyword (Reverting [#18])
- `sll_sys_arg_count_t` has been replaced by `sll_integer_t` (it was easy to confuse with `sll_arg_count_t`)
- Duplicated code in [`src/sll/parse.c`][0.6.15/src/sll/parse.c] and [`src/sll/print.c`][0.6.15/src/sll/print.c]
- `sll_acquire_object` function

## [0.6.14] - 2021-8-24
### Added
- Ability to execute code directly from the command line with the `-s` option ([#19])
- The `restrict` keyword ([#18])
- The `SLL_VERSION_HAS_SHA` macro is defined when the `SLL_VERSION_SHA` and `SLL_VERSION_FULL_SHA` macros hold the commit sha-1 value

### Changed
- Reverted from the BSD 3-Clause License to the MIT License
- Moved the CLI code into its own folder ([`src/cli/`][0.6.14/src/cli])
- The order of arguments in an API function (Used to be `output, argument count, argument data` and now is `argument data, argument count, output`)

### Removed
- CLI no-logo (`-L`) option
- Useless clean-up code in [`src/sll/parse.c`][0.6.14/src/sll/parse.c] \([#20]\)

## [0.6.13] - 2021-8-20
### Added
- Time sleeping functions: `sleep`, `sleep_nanos`, `sll_api_time_sleep` and `sll_api_time_sleep_nanos` ([#5])
- Platform-dependent function `sll_platform_sleep`
- Object-to-string conversion functions: `sll_object_to_string` and `sll_object_to_string_length`
- System argument functions (similar to `argv` in C): `sll_set_argument_count` and `sll_set_argument`
- New type: `sll_sys_arg_count_t`
- Implemented part of [`src/sll/operator.c`][0.6.13/src/sll/operator.c]
- Optimization in printing ([#15], [#16] and [#17])
- Added and/or removed included header files ([#11])
- Function for adding string to a string list: `sll_add_string`

### Changed
- Build script now does not combine files before compilation ([#12])
- Renamed `UNREACHABLE` to `SLL_UNREACHABLE`
- Moved assembly functions into a separate header ([`src/include/sll/assembly.h`][0.6.13/src/include/sll/assembly.h])
- Moved internal function table functions into a separate header ([`src/include/sll/ift.h`][0.6.13/src/include/sll/ift.h])
- Moved object-related functions into a separate header ([`src/include/sll/object.h`][0.6.13/src/include/sll/object.h])
- Renamed all API function files to end with a `_api` suffix to avoid name collisions with other files

### Removed
- `sll_platform_setup_console` function
- ANSI escape sequences in error printing (Thus invalidating [#2])

## [0.6.12] - 2021-8-19
### Added
- An option to enable ANSI escape codes (`-C`) to color the output
- An option to print the version of sll (`-V`)
- The `SLL_VERSION_SHA` and `SLL_VERSION_FULL_SHA` macros hold the current commits' hash
- Two new operators: length (`$`) and access (`:`) (as well as their `SLL_OBJECT_TYPE_` counterparts)
- Six new assembly instructions: `SLL_ASSEMBLY_INSTRUCTION_TYPE_LENGTH`, `SLL_ASSEMBLY_INSTRUCTION_TYPE_COPY`, `SLL_ASSEMBLY_INSTRUCTION_TYPE_ACCESS`, `SLL_ASSEMBLY_INSTRUCTION_TYPE_ACCESS_TWO`, `SLL_ASSEMBLY_INSTRUCTION_TYPE_ACCESS_THREE`, `SLL_ASSEMBLY_INSTRUCTION_TYPE_LOAD_DEL`
- New reference-counting based GC for strings and arrays (`sll_array_release` and `sll_string_release` are now `SLL_ACQUIRE`)
- Macros for ternary and quaternary operators
- Proper clean-up code generation after loops
- Looping operators (`SLL_OBJECT_TYPE_FOR`, `SLL_OBJECT_TYPE_WHILE` and `SLL_OBJECT_TYPE_LOOP`) now store their inner scope id

### Changed
- The new file extension for an assembly is `.sla` and for a compiled object `.slc`
- Changed `SLL_ASSEMBLY_INSTRUCTION_TYPE_DEL` to `SLL_ASSEMBLY_INSTRUCTION_TYPE_LOAD_DEL`
- Rewritten assembly optimizer
- Replaced `UINTxx_MAX` with `SLL_MAX_yy` where applicable ([#10])
- The test child process now uses a file to communicate its result instead of the piped standard output

### Removed
- Heap functions (`sll_allocate_raw`, `sll_deallocate_raw`, `sll_reset_allocator`, `sll_array_release` and `sll_string_release`)
- The [`src/sll/allocator.c`][0.6.11/src/sll/allocator.c] file
- The `sll_api_string_ref_count` internal function
- Heap-related types and structures
- Unused heap-related macros

## [0.6.11] - 2021-8-18
### Added
- Support for variadic macros in the build script
- Macro constants for limits of sll types (`SLL_MAX_xxx`)
- Functions now store their inner scope id
- Jump instructions are no longer generated in unreachable locations

### Changed
- The sll library is now named `sll-{major}.{minor}.{patch}` instead of `sll`
- `sll_arg_count_t` and `sll_statement_count_t` are now 32-bit values

### Removed
- Coverage data is now an experimental feature and has been moved to a [different branch][test-coverage]

## [0.6.10] - 2021-8-14
### Added
- The build script now parses the [`src/include/sll/version.h`][0.6.10/src/include/sll/version.h] file
- Fix for Windows runner in Github Actions
- Four new API functions: `sll_api_string_convert`, `sll_api_string_length`, `sll_api_string_ref_count` and `sll_api_util_ref_count`
- Object to string conversion functions: `sll_object_to_string` and `sll_object_to_string_length`
- Arrays
- Four new errors: `SLL_ERROR_UNMATCHED_ARRAY_OPEN_BRACKETS`, `SLL_ERROR_UNMATCHED_ARRAY_CLOSE_BRACKETS`, `SLL_ERROR_ARRAY_TOO_LONG` and `SLL_ERROR_INVALID_STACK_INDEX`
- Four new assembly instructions: `SLL_ASSEMBLY_INSTRUCTION_TYPE_PACK`, `SLL_ASSEMBLY_INSTRUCTION_TYPE_PACK_ZERO`, `SLL_ASSEMBLY_INSTRUCTION_TYPE_PACK_ONE` and `SLL_ASSEMBLY_INSTRUCTION_TYPE_DEL`
- Variables are now released after their last reference

#### Arrays
Literally, just arrays.

### Changed
- `ASSERT` has been renamed to `SLL_ASSERT`
- Operators were moved to a separate file ([`src/sll/operator.c`][0.6.10/src/sll/operator.c])
- String heap functions were renamed to generic allocator functions and are now used by both arrays and strings
- [`src/sll/string_heap.c`][0.6.9/src/sll/string_heap.c] was split into [`src/sll/allocator.c`][0.6.10/src/sll/allocator.c] and [`src/sll/string.c`][0.6.10/src/sll/string.c]
- `memcpy()` is now used instead of loops

## [0.6.9] - 2021-8-12
**Name Change: From _Lisp Like Language_ to _Sl Lang_ !!!**

### Added
- Test runner
- `SLL_MAX_ERROR` macro constant
- String heap allocator
- New operator: variable declaration (`(#)`)

### Changed
- Renamed [`src/include/lll`][0.6.8/src/include/lll] to [`src/include/sll`][0.6.9/src/include/sll]
- Renamed [`src/lll`][0.6.8/src/lll] to [`src/sll`][0.6.9/src/sll]
- Renamed all functions and macros
- Split [`src/_build/main.py`][0.6.8/src/_build/main.py] into [`src/_build/build.py`][0.6.9/src/_build/build.py], [`src/_build/header.py`][0.6.9/src/_build/header.py], [`src/_build/main.py`][0.6.9/src/_build/main.py] and [`src/_build/util.py`][0.6.9/src/_build/util.py]
- `sll_platform_free_page` now takes the page size as the second parameter

### Removed
- Unused file: [`build.py`][0.6.8/build.py]

## [0.6.8] - 2021-8-7
### Changed
- Moved [`build.py`][0.6.7/build.py] to [`src/_build/main.py`][0.6.8/src/_build/main.py]

## [0.6.7] - 2021-8-7
### Added
- Path-related functions: `lll_api_path_absolute`, `lll_api_path_exists`, `lll_api_path_is_dir`, `lll_api_path_relative` and `lll_api_path_size`
- Platform-dependent path functions: `lll_platform_path_absolute`, `lll_platform_path_exists` and `lll_platform_path_is_directory`
- New type: `lll_bool_t`

### Changed
- Minor fixes to the build script

## [0.6.6] - 2021-8-7
### Added
- More verbose logs generated
- The ability to pass arguments into the VM (`-A` option, `lll_set_argument_count` and `lll_set_argument` functions)
- A static zero-length string to reduce object allocation
- Two new assembly instructions: `LLL_ASSEMBLY_INSTRUCTION_TYPE_CALL_ZERO` and `LLL_ASSEMBLY_INSTRUCTION_TYPE_CALL_ONE`
- String release function (`lll_string_release`)
- Platform name string (`lll_platform_string`)

### Changed
- The build script properly indents output from child processes
- Renamed [`src/lll_lib`][0.6.5/src/lll_lib] to [`src/lll`][0.6.6/src/lll]
- Updated the optimizer
- Updated the assembly optimizer

### Removed
- Unused file constants: `LLL_API_FILE_APPEND`, `LLL_API_FILE_READ` and `LLL_API_FILE_WRITE`
- String comparison and conversion functions

## [0.6.5] - 2021-8-3
### Added
- Standalone compilation mode
- Ability to create an input stream from a buffer (`lll_stream_create_input_from_buffer`)

### Changed
- Rewritten [`build.py`][0.6.5/build.py]

## [0.6.4] - 2021-8-3
### Added
- File IO functions: `lll_api_open_file`, `lll_api_close_file` and `lll_api_write_file`
- New error: `LLL_ERROR_INTERNAL_FUNCTION_NAME_NOT_ASCII`
- String compare functions: `lll_string_compare`, `lll_string_compare_ascii`, `lll_string_compare_ignore_case` and `lll_string_compare_ignore_case_ascii`
- String convert functions: `lll_string_to_ascii`, `lll_string_length_as_bytes`, `lll_string_to_bytes`, `lll_string_to_stream` and `lll_string_encode_char_to_stream`

### Changed
- Minor tweaks & changes to the build script and cli program
- Split [`src/include/lll/core.h`][0.6.3/src/include/lll/core.h] into [`src/include/lll/io.h`][0.6.4/src/include/lll/io.h], [`src/include/lll/stream.h`][0.6.4/src/include/lll/stream.h] and [`src/include/lll/string.h`][0.6.4/src/include/lll/string.h]
- Moved console setup code to `lll_platform_setup_console`
- Renamed `time_current` and `time_current_nanos` into `time` and `time_nanos`

## [0.6.3] - 2021-8-2
### Added
- The `INTERNAL_FUNCTION` macro can be used to declare default internal function
- `sll_api_get_time_float` function
- Support for floating-point values
- The CLI tries to load each file as a compiled object (`<file>.lllc`)

### Changed
- Removed duplications in the builder script
- `__SLL_IMPORT_EXPORT` has been renamed to `__SLL_FUNC`
- Split [`src/include/lll_lib.h`][0.6.2/src/include/lll_lib.h] to [`src/include/lll/common.h`][0.6.3/src/include/lll/common.h], [`src/include/lll/constants.h`][0.6.3/src/include/lll/constants.h], [`src/include/lll/core.h`][0.6.3/src/include/lll/core.h], [`src/include/lll/platform.h`][0.6.3/src/include/lll/platform.h] and [`src/include/lll/types.h`][0.6.3/src/include/lll/types.h]
- Renamed [`src/include/lll_lib_api.h`][0.6.2/src/include/lll_lib_api.h] and [`src/include/version.h`][0.6.2/src/include/version.h] to [`src/include/lll/api.h`][0.6.3/src/include/lll/api.h] and [`src/include/lll/version.h`][0.6.3/src/include/lll/version.h]
- Moved platform-dependent code to [`src/lll_lib/platform/linux.c`][0.6.3/src/lll_lib/platform/linux.c] and [`src/lll_lib/platform/windows.c`][0.6.3/src/lll_lib/platform/windows.c]

## [0.6.2] - 2021-8-1
### Added
- New operator: `(--)` (import)
- Implemented identifier exporting (the `(##)` operator)
- `LLL_ASSEMBLY_INSTRUCTION)TYPE_CALL_POP` instruction

### Changed
- Internal function `time` has been renamed to `time_current`
- Bugs in the optimizer have been fixed
- `lll_parse_all_object` not requires a function used to load external files

### Removed
- Types and functions related to import tables
- Option to merge modules (`-m`). Modules are now merged automatically

## [0.6.1] - 2021-8-1
### Added
- Ability to export identifiers
- Export operator (`(##)`)

### Changed
- Changed some structure and type definitions

## [0.6.0] - 2021-7-31
### Added
- Internal functions
- [`src/include/lll_lib_api.h`][0.6.0/src/include/lll_lib_api.h]
- `lll_api_get_time` function in [`src/lll_lib/api/time.c`][0.6.0/src/lll_lib/api/time.c]
- [`src/lll_lib/lib/time.lll`][0.6.0/src/lll_lib/lib/time.lll]
- CLI tries to load files from the `<executable_directory>/lib/` directory

### Changed
- Fixed the builder script

## [0.5.2] - 2021-7-31
### Changed
- `ASSERT` macro now only works in debug builds
- VM now uses a call stack instead of the `LLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_II` instruction
- `(@@)` (return) operator now compiles into the exit operator (`(@)`) if it is used outside of a function
- VM now uses a stack index instead of a moving pointer

## [0.5.1] - 2021-7-30
### Changed
- `lll_compare_runtime_object`, `lll_create_string`, `lll_runtime_object_size` and `lll_runtime_object_nonzero` functions are now public
- Function `lll_run_assembly` has been renamed to `lll_execute_assembly`
- Runtime object functions (`lll_compare_runtime_object` and `lll_runtime_object_nonzero`) have been moved to [`src/lll_lib/object.c`][0.5.1/src/lll_lib/object.c]

## [0.5.0] - 2021-7-30
### Added
- Two new operators: `(>-)` (do-while loop) and `(><)` (infinite loop)
- Assembly instructions used to store small integer values (`-1`, `0`, `1`, `2`, `3` and `4`)
- `lll_optimize_object` function

### Changed
- Every single object uses the same structure
- `lll_optimize_metadata` function has been moved from [`src/lll_lib/optimize.c`][0.4.1/src/lll_lib/optimize.c] to [`src/lll_lib/metadata.c`][0.5.0/src/lll_lib/metadata.c]
- `lll_remove padding` function has been moved to [`src/lll_lib/padding.c`][0.5.0/src/lll_lib/padding.c]

### Removed
- `LLL_OBJECT_TYPE_NIL`: `nil` values get compiled directly into zeros

## [0.4.1] - 2021-7-29
### Added
- A level-3 optimization (`-O3`), which performs global optimization of the compiled object
- `LLL_ASSEMBLY_INSTRUCTION_TYPE_POP_ROT` instruction
- Assembly instructions for returning diffrent types
- Errors related to invalid instructions, stack corruption and an invalid instruction index
- Two new operators: `(@)` (exit) and `(@@)` return

### Chagned
- In a debug build, the `UNREACHABLE()` macro expands into `ASSERT(!"Unreachable")`
- Operator assembly instruction can be used directly on a variable

### Removed
- `LLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_NIL` instruction
- `LLL_RUNTIME_OBJECT_TYPE_TRUE` and `LLL_RUNTIME_OBJECT_TYPE_FALSE` runtime object types
- `lll_optimize_object` function

## [0.4.0] - 2021-7-28
### Added
- Assembly instructions used for pushing the current instruicton index and for storing a variable and poping it of the stack
- Implemented `lll_run_assembly`

### Changed
- Assembly instruction that modify the instruction index can jump to relative offsets
- Assembly instruction data is now saved and read from files

### Removed
- Unimplemented language-generation functions and CLI arguments

## [0.3.5] - 2021-7-28
### Added
- Ability to save the generated assembly code (option `-a`)
- Option for printing assembly (`-P`)
- Assembly generator and related structures, types, functions and macros

## [0.3.4] - 2021-7-27
### Added
- Option for saving the compiled object (`-c`)
- Option for language generation (`-g`)
- `lll_optimize_metadata` function

## [0.3.3] - 2021-7-26
### Added
- Two new types: `lll_float_t` and `lll_time_t`

### Changed
- Moved `_create_string` and `_object_size` functions to [`src/lll_lib/util.c`][0.3.3/src/lll_lib/util.c]

## [0.3.2] - 2021-7-25
### Added
- `lll_merge_import` function merges string
- `_create_string` function

### Changed
- Renamed `uintXX_t` to custom types

## [0.3.1] - 2021-7-25
### Changed
- Fixed bugs related to bit shifting of narrow types in variable-length integer encoding
- Fixed other bugs

## [0.3.0] - 2021-7-25
### Changed
-  Integers are now encoded differently

## [0.2.3] - 2021-7-25
### Changed
- Rewritten the help document

### Removed
- Object modifiers
- Unused macros related to assembly generation

## [0.2.2] - 2021-7-23
### Changed
- Renamed some options
- All object are now 8-byte alignemd
- Identifier listist comparasion has been sped up

## [0.2.1] - 2021-7-3
### Added
- Debug object also store the line and column number

### Changed
- All integer are now stored as 64-bit value

## [0.2.0] - 2021-7-3
### Added
- Diffrent structures for each object type

### Removed
- Object conversion functions

## [0.1.13] - 2021-7-2
*Nothing*

## [0.1.12] - 2021-7-2
### Changed
- Added builder script option for code generation (`--generate-code`)

## [0.1.11] - 2021-7-2
### Changed
- Rewritten help documentation
- CLI now executes the code by default

## [0.1.10] - 2021-7-1
### Changed
- Replaced `printf()` call in the CLI now print data using `print_str`, `print_str_const` and `print_int`

## [0.1.9] - 2021-7-1
### Changed
- Fixed bugs in the builder script

## [0.1.8] - 2021-7-1
### Added
- Function for executing code (`lll_run_compiled_object`)
- Options for running the program (`-r` / `-r0`) and generating output (`-g` / `-g0`)

### Changed
- Moved the example file to the [`example/`][0.1.8/example] directory
- The help string has been moved to a separate file ([`rsrc/help.txt`][0.1.8/rsrc/help.txt])

## [0.1.7] - 2021-7-1
*Undocumented*

## [0.1.6] - 2021-6-18
*Undocumented*

## [0.1.5] - 2021-6-17
*Undocumented*

## [0.1.4] - 2021-6-16
*Undocumented*

## [0.1.3] - 2021-6-16
*Undocumented*

## [0.1.2] - 2021-6-15
Unfortunately, no versions were assigned before 2021-6-15 (:disappointed:), so the change log for version 0.1.2 (and below) consists of everything added up to that point.

[0.6.18]: https://github.com/sl-lang/sll/compare/v0.6.17...main
[0.6.17]: https://github.com/sl-lang/sll/compare/v0.6.16...v0.6.17
[0.6.16]: https://github.com/sl-lang/sll/compare/v0.6.15...v0.6.16
[0.6.15]: https://github.com/sl-lang/sll/compare/v0.6.14...v0.6.15
[0.6.14]: https://github.com/sl-lang/sll/compare/v0.6.13...v0.6.14
[0.6.13]: https://github.com/sl-lang/sll/compare/v0.6.12...v0.6.13
[0.6.12]: https://github.com/sl-lang/sll/compare/v0.6.11...v0.6.12
[0.6.11]: https://github.com/sl-lang/sll/compare/v0.6.10...v0.6.11
[0.6.10]: https://github.com/sl-lang/sll/compare/v0.6.9...v0.6.10
[0.6.9]: https://github.com/sl-lang/sll/compare/v0.6.8...v0.6.9
[0.6.8]: https://github.com/sl-lang/sll/compare/v0.6.7...v0.6.8
[0.6.7]: https://github.com/sl-lang/sll/compare/v0.6.6...v0.6.7
[0.6.6]: https://github.com/sl-lang/sll/compare/v0.6.5...v0.6.6
[0.6.5]: https://github.com/sl-lang/sll/compare/v0.6.4...v0.6.5
[0.6.4]: https://github.com/sl-lang/sll/compare/v0.6.3...v0.6.4
[0.6.3]: https://github.com/sl-lang/sll/compare/v0.6.2...v0.6.3
[0.6.2]: https://github.com/sl-lang/sll/compare/v0.6.1...v0.6.2
[0.6.1]: https://github.com/sl-lang/sll/compare/v0.6.0...v0.6.1
[0.6.0]: https://github.com/sl-lang/sll/compare/v0.5.2...v0.6.0
[0.5.2]: https://github.com/sl-lang/sll/compare/v0.5.1...v0.5.2
[0.5.1]: https://github.com/sl-lang/sll/compare/v0.5.0...v0.5.1
[0.5.0]: https://github.com/sl-lang/sll/compare/v0.4.1...v0.5.0
[0.4.1]: https://github.com/sl-lang/sll/compare/v0.4.0...v0.4.1
[0.4.0]: https://github.com/sl-lang/sll/compare/v0.3.5...v0.4.0
[0.3.5]: https://github.com/sl-lang/sll/compare/v0.3.4...v0.3.5
[0.3.4]: https://github.com/sl-lang/sll/compare/v0.3.3...v0.3.4
[0.3.3]: https://github.com/sl-lang/sll/compare/v0.3.2...v0.3.3
[0.3.2]: https://github.com/sl-lang/sll/compare/v0.3.1...v0.3.2
[0.3.1]: https://github.com/sl-lang/sll/compare/v0.3.0...v0.3.1
[0.3.0]: https://github.com/sl-lang/sll/compare/v0.2.3...v0.3.0
[0.2.3]: https://github.com/sl-lang/sll/compare/v0.2.2...v0.2.3
[0.2.2]: https://github.com/sl-lang/sll/compare/v0.2.1...v0.2.2
[0.2.1]: https://github.com/sl-lang/sll/compare/v0.2.0...v0.2.1
[0.2.0]: https://github.com/sl-lang/sll/compare/v0.1.13...v0.2.0
[0.1.13]: https://github.com/sl-lang/sll/compare/v0.1.12...v0.1.13
[0.1.12]: https://github.com/sl-lang/sll/compare/v0.1.11...v0.1.12
[0.1.11]: https://github.com/sl-lang/sll/compare/v0.1.10...v0.1.11
[0.1.10]: https://github.com/sl-lang/sll/compare/v0.1.9...v0.1.10
[0.1.9]: https://github.com/sl-lang/sll/compare/v0.1.8...v0.1.9
[0.1.8]: https://github.com/sl-lang/sll/compare/v0.1.7...v0.1.8
[0.1.7]: https://github.com/sl-lang/sll/compare/v0.1.6...v0.1.7
[0.1.6]: https://github.com/sl-lang/sll/compare/v0.1.5...v0.1.6
[0.1.5]: https://github.com/sl-lang/sll/compare/v0.1.4...v0.1.5
[0.1.4]: https://github.com/sl-lang/sll/compare/v0.1.3...v0.1.4
[0.1.3]: https://github.com/sl-lang/sll/compare/v0.1.2...v0.1.3
[0.1.2]: https://github.com/sl-lang/sll/tree/v0.1.2
[#37]: https://github.com/sl-lang/sll/issues/37
[#36]: https://github.com/sl-lang/sll/issues/36
[#35]: https://github.com/sl-lang/sll/issues/35
[#34]: https://github.com/sl-lang/sll/issues/34
[#33]: https://github.com/sl-lang/sll/issues/33
[#32]: https://github.com/sl-lang/sll/issues/32
[#30]: https://github.com/sl-lang/sll/issues/30
[#29]: https://github.com/sl-lang/sll/issues/29
[#20]: https://github.com/sl-lang/sll/issues/20
[#19]: https://github.com/sl-lang/sll/issues/19
[#18]: https://github.com/sl-lang/sll/issues/18
[#17]: https://github.com/sl-lang/sll/issues/17
[#16]: https://github.com/sl-lang/sll/issues/16
[#15]: https://github.com/sl-lang/sll/issues/15
[#12]: https://github.com/sl-lang/sll/issues/12
[#11]: https://github.com/sl-lang/sll/issues/11
[#10]: https://github.com/sl-lang/sll/issues/10
[#5]: https://github.com/sl-lang/sll/issues/5
[#2]: https://github.com/sl-lang/sll/issues/2
[test-coverage]: https://github.com/sl-lang/sll/tree/test-coverage
[0.6.17/src/include/sll/common.h]: https://github.com/sl-lang/sll/blob/v0.6.17/src/include/sll/common.h
[0.6.16/src/sll/operator.c]: https://github.com/sl-lang/sll/blob/v0.6.16/src/sll/operator.c
[0.6.16/src/sll/vm.c]: https://github.com/sl-lang/sll/blob/v0.6.16/src/sll/vm.c
[0.6.15/src/sll/parse.c]: https://github.com/sl-lang/sll/blob/v0.6.15/src/sll/parse.c
[0.6.15/src/sll/print.c]: https://github.com/sl-lang/sll/blob/v0.6.15/src/sll/print.c
[0.6.14/src/cli]: https://github.com/sl-lang/sll/tree/v0.6.14/src/cli
[0.6.14/src/sll/parse.c]: https://github.com/sl-lang/sll/blob/v0.6.14/src/sll/parse.c
[0.6.13/src/include/sll/assembly.h]: https://github.com/sl-lang/sll/blob/v0.6.13/src/include/sll/assembly.h
[0.6.13/src/include/sll/ift.h]: https://github.com/sl-lang/sll/blob/v0.6.13/src/include/sll/ift.h
[0.6.13/src/include/sll/object.h]: https://github.com/sl-lang/sll/blob/v0.6.13/src/include/sll/object.h
[0.6.13/src/sll/operator.c]: https://github.com/sl-lang/sll/blob/v0.6.13/src/sll/operator.c
[0.6.11/src/sll/allocator.c]: https://github.com/sl-lang/sll/blob/v0.6.11/src/sll/allocator.c
[0.6.10/src/include/sll/version.h]: https://github.com/sl-lang/sll/blob/v0.6.10/src/include/sll/version.h
[0.6.10/src/sll/allocator.c]: https://github.com/sl-lang/sll/blob/v0.6.10/src/sll/allocator.c
[0.6.10/src/sll/operator.c]: https://github.com/sl-lang/sll/blob/v0.6.10/src/sll/operator.c
[0.6.10/src/sll/string.c]: https://github.com/sl-lang/sll/blob/v0.6.10/src/sll/string.c
[0.6.9/src/_build/build.py]: https://github.com/sl-lang/sll/blob/v0.6.9/src/_build/build.py
[0.6.9/src/_build/header.py]: https://github.com/sl-lang/sll/blob/v0.6.9/src/_build/header.py
[0.6.9/src/_build/main.py]: https://github.com/sl-lang/sll/blob/v0.6.9/src/_build/main.py
[0.6.9/src/_build/util.py]: https://github.com/sl-lang/sll/blob/v0.6.9/src/_build/util.py
[0.6.9/src/include/sll]: https://github.com/sl-lang/sll/tree/v0.6.9/src/include/sll
[0.6.9/src/sll]: https://github.com/sl-lang/sll/tree/v0.6.9/src/sll
[0.6.9/src/sll/string_heap.c]: https://github.com/sl-lang/sll/blob/v0.6.9/src/sll/string_heap.c
[0.6.8/build.py]: https://github.com/sl-lang/sll/blob/v0.6.8/build.py
[0.6.8/src/_build/main.py]: https://github.com/sl-lang/sll/blob/v0.6.8/src/_build/main.py
[0.6.8/src/include/lll]: https://github.com/sl-lang/sll/tree/v0.6.8/src/include/lll
[0.6.8/src/lll]: https://github.com/sl-lang/sll/tree/v0.6.8/src/lll
[0.6.7/build.py]: https://github.com/sl-lang/sll/blob/v0.6.7/build.py
[0.6.6/src/lll]: https://github.com/sl-lang/sll/tree/v0.6.6/src/lll
[0.6.5/build.py]: https://github.com/sl-lang/sll/blob/v0.6.5/build.py
[0.6.5/src/lll_lib]: https://github.com/sl-lang/sll/tree/v0.6.5/src/lll_lib
[0.6.4/src/include/lll/io.h]: https://github.com/sl-lang/sll/blob/v0.6.4/src/include/lll/io.h
[0.6.4/src/include/lll/stream.h]: https://github.com/sl-lang/sll/blob/v0.6.4/src/include/lll/stream.h
[0.6.4/src/include/lll/string.h]: https://github.com/sl-lang/sll/blob/v0.6.4/src/include/lll/string.h
[0.6.3/src/include/lll/api.h]: https://github.com/sl-lang/sll/blob/v0.6.3/src/include/lll/api.h
[0.6.3/src/include/lll/common.h]: https://github.com/sl-lang/sll/blob/v0.6.3/src/include/lll/common.h
[0.6.3/src/include/lll/constants.h]: https://github.com/sl-lang/sll/blob/v0.6.3/src/include/lll/constants.h
[0.6.3/src/include/lll/core.h]: https://github.com/sl-lang/sll/blob/v0.6.3/src/include/lll/core.h
[0.6.3/src/include/lll/core.h]: https://github.com/sl-lang/sll/blob/v0.6.3/src/include/lll/core.h
[0.6.3/src/include/lll/platform.h]: https://github.com/sl-lang/sll/blob/v0.6.3/src/include/lll/platform.h
[0.6.3/src/include/lll/types.h]: https://github.com/sl-lang/sll/blob/v0.6.3/src/include/lll/types.h
[0.6.3/src/include/lll/version.h]: https://github.com/sl-lang/sll/blob/v0.6.3/src/include/lll/version.h
[0.6.3/src/lll_lib/platform/linux.c]: https://github.com/sl-lang/sll/blob/v0.6.3/src/lll_lib/platform/linux.c
[0.6.3/src/lll_lib/platform/windows.c]: https://github.com/sl-lang/sll/blob/v0.6.3/src/lll_lib/platform/windows.c
[0.6.2/src/include/lll_lib_api.h]: https://github.com/sl-lang/sll/blob/v0.6.2/src/include/lll_lib_api.h
[0.6.2/src/include/version.h]: https://github.com/sl-lang/sll/blob/v0.6.2/src/include/version.h
[0.6.2/src/include/lll_lib.h]: https://github.com/sl-lang/sll/blob/v0.6.2/src/include/lll_lib.h
[0.6.0/src/include/lll_lib_api.h]: https://github.com/sl-lang/sll/blob/v0.6.0/src/include/lll_lib_api.h
[0.6.0/src/lll_lib/api/time.c]: https://github.com/sl-lang/sll/blob/v0.6.0/src/lll_lib/api/time.c
[0.6.0/src/lll_lib/lib/time.lll]: https://github.com/sl-lang/sll/blob/v0.6.0/src/lll_lib/lib/time.lll
[0.5.1/src/lll_lib/object.c]: https://github.com/sl-lang/sll/blob/v0.5.1/src/lll_lib/object.c
[0.5.0/src/lll_lib/metadata.c]: https://github.com/sl-lang/sll/blob/v0.5.0/src/lll_lib/metadata.c
[0.5.0/src/lll_lib/padding.c]: https://github.com/sl-lang/sll/blob/v0.5.0/src/lll_lib/padding.c
[0.4.1/src/lll_lib/optimize.c]: https://github.com/sl-lang/sll/blob/v0.4.1/src/lll_lib/optimize.c
[0.3.3/src/lll_lib/util.c]: https://github.com/sl-lang/sll/blob/v0.3.3/src/lll_lib/util.c
[0.1.8/example]: https://github.com/sl-lang/sll/tree/v0.1.8/example
[0.1.8/rsrc/help.txt]: https://github.com/sl-lang/sll/blob/v0.1.8/rsrc/help.txt
