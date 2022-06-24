# Sll [0.7.23] - Ongoing

## Added

- Assembly instruction to push an empty string to the stack: `SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_EMPTY_STRING`
- Fuzzer now removes timeout crashes caused by generated infinite loops
- Implemented `path$relative`
- Implemented `sll_copy_objects`
- Implemented `SLL_ERROR_UNKNOWN_FUNCTION` and `error$ERROR_UNKNOWN_FUNCTION`
- Implemented the container API: `sll_container_t`, `SLL_CONTAINER_CLEAR`, `SLL_CONTAINER_FILTER`, `SLL_CONTAINER_INIT`, `SLL_CONTAINER_INIT_STRUCT`, `SLL_CONTAINER_ITER` and `SLL_CONTAINER_PUSH`
- Implemented the handle container API: `sll_handle_container_t`, `SLL_HANDLE_CONTAINER_ALLOC`, `SLL_HANDLE_CONTAINER_CHECK`, `SLL_HANDLE_CONTAINER_CLEAR`, `SLL_HANDLE_CONTAINER_DEALLOC`, `SLL_HANDLE_CONTAINER_INIT` and `SLL_HANDLE_CONTAINER_INIT_STRUCT`

## Changed

- Build script now uses relative instead of absolute file paths
- File handles now start from `1` to avoid linking `nil` with `stdin`
- Fixed `json$parse` with no arguments
- Fixed `nil` pointer dereference in `sll_api_process_join`
- Fixed `sll_string_increase` with empty string argument
- Fixed GC root assignment
- Fixed multiple assembly generation bugs
- Fixed nested function definitions
- Fixed weakref API global state access
- Fuzzers now restore the current working directory to prevent global state changing crashes
- Object creation symbol now works with complex numbers
- Root thread can no longer be deleted
- Static objects are no loner added as GC roots
- String length of a `nil` pointer returned by `sll_string_length` is now equal to `0`

## Removed

- Unimplemented functions: `sll_array_parse_char`, `sll_array_parse_float`, `sll_array_parse_int`, `sll_api_path_relative` and `sll_path_relative`

[0.7.23]: https://github.com/sl-lang/sll/compare/sll-v0.7.22...main
