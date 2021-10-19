# The Sl Programing Language Change Log

## [0.6.24] - Ongoing

### Added

- API related to listing files in a directory (`sll_api_path_list_dir`, `sll_api_path_list_dir_raw`, `sll_api_path_recursive_list_dir`, `sll_api_path_recursive_list_dir_raw`, `sll_platform_list_directory`, `list_dir` and `recursive_list_dir`)
- Sorting API (`sll_api_sort_sort`, `sll_api_sort_sort_raw`, `sll_quicksort`, `sort`)

### Changed

- All constants excluding `true`, `false` and `nil` have been move to library files (`int_type`, `float_type`, `char_type`, `string_type`, `array_type`, `handle_type`, `map_type`, `map_key_type` and `map_value_type` have been moved to [`src/sll/lib/types.sll`][main/src/sll/lib/types.sll] and `stdin`, `stdout`, `stderr` have been moved to [`src/sll/lib/file.sll`][main/src/sll/lib/file.sll])
- *\[POSIX only\]* Fixed return value of `sll_platform_path_absolute`

### Removed

- Buffer-related types: `sll_buffer_t` (now `sll_char_t*`), `sll_const_buffer_t` (now `const sll_char_t*`) and `sll_buffer_size_t` (now `sll_string_length_t`)
- Maximum buffer size macro `SLL_MAX_BUFFER_SIZE` (now `SLL_MAX_STRING_LENGTH`)

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

- `SLL_STRING_INCREASE` macro (in favor of `sll_string_increase`), `SLL_STRING_FROM_STATIC` macro (in favor of `sll_string_from_pointer`, which removes bugs with writing to read-only memory)
- Internal operator declaration macros `__SLL_OPERATOR_UNARY`, `__SLL_OPERATOR_BINARY`, `__SLL_OPERATOR_TERNARY` and `__SLL_OPERATOR_QUATERNARY`
- Internal static object macros: `__SLL_STATIC_NAME` and `__SLL_STATIC_OBJECT_DECL`
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

## [0.6.21] - 2021-10-4

### Added

- All operators are now properly optimized
- Hash module ([#23])
- Implemented **ALL** operators ([#14])
- Implemented `sll_array_and_array`, `sll_array_combinations`, `sll_array_equal`, `sll_array_equal_map`, `sll_array_or_array`, `sll_array_xor_array`, `sll_array_range`, `sll_array_string_combinations`, `sll_map_add_array`, `sll_map_add_string`, `sll_map_and`, `sll_map_and_array`, `sll_map_and_string`, `sll_map_equal`, `sll_string_duplicate`, `sll_string_equal_array`, `sll_string_equal_map`, `sll_string_from_int`, `sll_string_replace_char` and `sll_string_split_char`
- Internal function can now be declared as `SLL_INTERNAL_FUNCTION_FLAG_COMPILATION_CALL`, which would make them be optimizable at compile-time
- Memory API & reference operator ([#42])

### Changed

- Bug fixes in the optimizer
- `sll_operator_add` now correctly combines objects with strings and arrays

## [0.6.20] - 2021-9-26

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

## [0.6.19] - 2021-9-21

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

## [0.6.18] - 2021-9-13

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

## [0.6.17] - 2021-9-9

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

## [0.6.16] - 2021-8-30

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

## [0.6.15] - 2021-8-26

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

## [0.6.14] - 2021-8-24

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
- Useless clean-up code in [`src/sll/parse.c`][0.6.14/src/sll/parse.c] \([#20]\)

## [0.6.13] - 2021-8-20

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

## [0.6.12] - 2021-8-19

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

## [0.6.11] - 2021-8-18

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

## [0.6.10] - 2021-8-14

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

## [0.6.9] - 2021-8-12

**Name Change: From *Lisp Like Language* to *Sl Lang* !!!**

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

## [0.6.8] - 2021-8-7

### Changed

- Moved [`build.py`][0.6.7/build.py] to [`src/_build/main.py`][0.6.8/src/_build/main.py]

## [0.6.7] - 2021-8-7

### Added

- New type: `lll_bool_t`
- Path-related functions: `lll_api_path_absolute`, `lll_api_path_exists`, `lll_api_path_is_dir`, `lll_api_path_relative` and `lll_api_path_size`
- Platform-dependent path functions: `lll_platform_path_absolute`, `lll_platform_path_exists` and `lll_platform_path_is_directory`

### Changed

- Minor fixes to the build script

## [0.6.6] - 2021-8-7

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

## [0.6.5] - 2021-8-3

### Added

- Ability to create an input stream from a buffer (`lll_stream_create_input_from_buffer`)
- Standalone compilation mode

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
- Moved console setup code to `lll_platform_setup_console`
- Renamed `time_current` and `time_current_nanos` into `time` and `time_nanos`
- Split [`src/include/lll/core.h`][0.6.3/src/include/lll/core.h] into [`src/include/lll/io.h`][0.6.4/src/include/lll/io.h], [`src/include/lll/stream.h`][0.6.4/src/include/lll/stream.h] and [`src/include/lll/string.h`][0.6.4/src/include/lll/string.h]

## [0.6.3] - 2021-8-2

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

## [0.6.2] - 2021-8-1

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

## [0.6.1] - 2021-8-1

### Added

- Ability to export identifiers
- Export operator (`(##)`)

### Changed

- Changed some structure and type definitions

## [0.6.0] - 2021-7-31

### Added

- CLI tries to load files from the `<executable_directory>/lib/` directory
- Internal functions
- `lll_api_get_time` function in [`src/lll_lib/api/time.c`][0.6.0/src/lll_lib/api/time.c]
- [`src/include/lll_lib_api.h`][0.6.0/src/include/lll_lib_api.h]
- [`src/lll_lib/lib/time.lll`][0.6.0/src/lll_lib/lib/time.lll]

### Changed

- Fixed the builder script

## [0.5.2] - 2021-7-31

### Changed

- `(@@)` (return) operator now compiles into the exit operator (`(@)`) if used outside of a function
- `ASSERT` macro now only works in debug builds
- VM now uses a call stack instead of the `LLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_II` instruction
- VM now uses a stack index instead of a moving pointer

## [0.5.1] - 2021-7-30

### Changed

- Function `lll_run_assembly` has been renamed to `lll_execute_assembly`
- `lll_compare_runtime_object`, `lll_create_string`, `lll_runtime_object_size` and `lll_runtime_object_nonzero` functions are now public
- Runtime object functions (`lll_compare_runtime_object` and `lll_runtime_object_nonzero`) have been moved to [`src/lll_lib/object.c`][0.5.1/src/lll_lib/object.c]

## [0.5.0] - 2021-7-30

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

## [0.4.1] - 2021-7-29

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

## [0.4.0] - 2021-7-28

### Added

- Assembly instructions used for pushing the current instruction index and for storing a variable and removing it of the stack
- Implemented `lll_run_assembly`

### Changed

- Assembly instruction data is now saved and read from files
- Assembly instruction that modify the instruction index can jump to relative offsets

### Removed

- Unimplemented language-generation functions and CLI arguments

## [0.3.5] - 2021-7-28

### Added

- Ability to save the generated assembly code (option `-a`)
- Assembly generator and related structures, types, functions and macros
- Option for printing assembly (`-P`)

## [0.3.4] - 2021-7-27

### Added

- `lll_optimize_metadata` function
- Option for language generation (`-g`)
- Option for saving the compiled object (`-c`)

## [0.3.3] - 2021-7-26

### Added

- Two new types: `lll_float_t` and `lll_time_t`

### Changed

- Moved `_create_string` and `_object_size` functions to [`src/lll_lib/util.c`][0.3.3/src/lll_lib/util.c]

## [0.3.2] - 2021-7-25

### Added

- `_create_string` function
- `lll_merge_import` function merges string

### Changed

- Renamed `uintXX_t` to custom types

## [0.3.1] - 2021-7-25

### Changed

- Fixed bugs related to bit shifting of narrow types in variable-length integer encoding
- Fixed other bugs

## [0.3.0] - 2021-7-25

### Changed

- Integers are now encoded differently

## [0.2.3] - 2021-7-25

### Changed

- Rewritten the help document

### Removed

- Object modifiers
- Unused macros related to assembly generation

## [0.2.2] - 2021-7-23

### Changed

- All object are now 8-byte aligned
- Identifier list compassion has been sped up
- Renamed some options

## [0.2.1] - 2021-7-3

### Added

- Debug object also store the line and column number

### Changed

- All integer are now stored as 64-bit value

## [0.2.0] - 2021-7-3

### Added

- Different structures for each object type

### Removed

- Object conversion functions

## [0.1.13] - 2021-7-2

*Nothing*

## [0.1.12] - 2021-7-2

### Changed

- Added builder script option for code generation (`--generate-code`)

## [0.1.11] - 2021-7-2

### Changed

- CLI now executes the code by default
- Rewritten help documentation

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

### Changed

- Changed from the MIT License to the BSD 3-Clause License
- Implemented code generation for functions

## [0.1.6] - 2021-6-18

### Added

- List of volatile CPU registers

### Changed

- API functions are now marked with `__LLL_API_FUNCTION`
- Volatile CPU registers are now saved across function calls

### Removed

- Declarations of unused private functions

## [0.1.5] - 2021-6-17

### Added

- Functions tables are now used to store function arguments

### Changed

- Functions and loops have inner scopes

## [0.1.4] - 2021-6-16

### Added

- Support for functions

### Changed

- Rewritten the syntax highlight

## [0.1.3] - 2021-6-16

### Added

- Options for printing the help message
- Versions!

## [0.1.2] - 2021-6-15

Unfortunately, no versions were assigned before 2021-6-15 (:disappointed:), so the change log for version 0.1.2 (and below) consists of everything added up to that point.

[0.6.24]: https://github.com/sl-lang/sll/compare/v0.6.23...main
[0.6.23]: https://github.com/sl-lang/sll/compare/v0.6.22...v0.6.23
[0.6.22]: https://github.com/sl-lang/sll/compare/v0.6.21...v0.6.22
[0.6.21]: https://github.com/sl-lang/sll/compare/v0.6.20...v0.6.21
[0.6.20]: https://github.com/sl-lang/sll/compare/v0.6.19...v0.6.20
[0.6.19]: https://github.com/sl-lang/sll/compare/v0.6.18...v0.6.19
[0.6.18]: https://github.com/sl-lang/sll/compare/v0.6.17...v0.6.18
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
[#71]: https://github.com/sl-lang/sll/issues/71
[#67]: https://github.com/sl-lang/sll/issues/67
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
[#23]: https://github.com/sl-lang/sll/issues/23
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
[#5]: https://github.com/sl-lang/sll/issues/5
[#3]: https://github.com/sl-lang/sll/issues/3
[#2]: https://github.com/sl-lang/sll/issues/2
[test-coverage]: https://github.com/sl-lang/sll/tree/test-coverage
[main/src/sll/lib/file.sll]: https://github.com/sl-lang/sll/blob/main/src/sll/lib/file.sll
[main/src/sll/lib/types.sll]: https://github.com/sl-lang/sll/blob/main/src/sll/lib/types.sll
[0.6.21/src/include/sll/constants.h]: https://github.com/sl-lang/sll/blob/v0.6.21/src/include/sll/constants.h
[0.6.21/src/include/sll/core.h]: https://github.com/sl-lang/sll/blob/v0.6.21/src/include/sll/core.h
[0.6.20/src/sll/api/file_api.c]: https://github.com/sl-lang/sll/blob/v0.6.20/src/sll/api/file_api.c
[0.6.20/src/include/sll/api]: https://github.com/sl-lang/sll/tree/v0.6.20/src/include/sll/api
[0.6.19/CODE_OF_CONDUCT.md]: https://github.com/sl-lang/sll/blob/v0.6.19/CODE_OF_CONDUCT.md
[0.6.19/src/cli/main.c]: https://github.com/sl-lang/sll/blob/v0.6.19/src/cli/main.c
[0.6.19/src/sll/print.c]: https://github.com/sl-lang/sll/blob/v0.6.19/src/sll/print.c
[0.6.19/tests]: https://github.com/sl-lang/sll/tree/v0.6.19/tests
[0.6.18/src/include/sll/_api_generated.h]: https://github.com/sl-lang/sll/blob/v0.6.18/src/include/sll/_api_generated.h
[0.6.18/src/sll/api/_generated.c]: https://github.com/sl-lang/sll/blob/v0.6.18/src/sll/api/_generated.c
[0.6.18/src/sll/operator.c]: https://github.com/sl-lang/sll/blob/v0.6.18/src/sll/operator.c
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
