# Sll [0.7.23] - Ongoing

## Added

- Assembly instruction to push an empty string to the stack: `SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_EMPTY_STRING`
- Implemented `path$relative`
- Implemented `SLL_ERROR_UNKNOWN_FUNCTION` and `error$ERROR_UNKNOWN_FUNCTION`

## Changed

- File handels now start from `1` to avoid linking `nil` with `stdin`
- Fixed `json$parse` with no arguments
- Fixed `nil` pointer dereference in `sll_api_process_join`
- Fixed GC root assignment
- Fixed multiple assembly generation bugs
- Fixed nested function definitions
- Fixed weakref API global state access
- Fuzzers now restore the current working directory to prevent global state changing crashes
- Object cretion symol now works with complex numbers
- Root thread can no longer be deleted
- Static objects are no loner added as GC roots
- String length of a `nil` pointer returned by `sll_string_length` is now equal to `0`

## Removed

- Unimplemented functions `sll_api_path_relative` and `sll_path_relative`

[0.7.23]: https://github.com/sl-lang/sll/compare/sll-v0.7.22...main
