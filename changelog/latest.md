# Sll [0.7.22] - Ongoing

## Added

- *\[Linux only\]* Build script option to build using Clang: `--clang`
- *\[Linux only\]* Build script option to enable fuzzing: `--fuzzer` (Requires `--clang`)
- *\[Linux only\]* Build script option to replay a fuzzer result: `--fuzzer-debug`
- Implemented `http/mime$DEFAULT_EXTENSION`
- Implemented `SLL_FILE_FLAG_SOCKET`
- Implemented `sll_internal_function_table_descriptor_t`
- Implemented `sll_platform_get_library_file_path`
- Implemented `SLL_RANDOM_BITS`
- Internal function descriptors located in `SLL_ABI_INTERNAL_FUNCTION_TABLE_DESCRIPTOR` are now automatically loaded upon initialization
- More tests ([#285])
- New audit event: `clib.library.load.error`
- Object refernece counter now has to be masked with `SLL_OBJECT_REFERENCE_COUNTER_MASK` upon access
- Objects can now be part of multiple GC roots
- Temporary GC roots can now be added by flagging the `fast` argument in `sll_gc_add_root`

## Changed

- Audit event `clib.library.load` now contains a `module_path` argument
- CLib library names are now absolute file paths
- Fixed array and string underflow indexing
- Fixed calling variable argument function via array in the VM
- Fixed checksum calculation in `sll_string_duplicate`
- Fixed global state initialization
- Fixed incorrect return value from function `sll_string_compare_pointer`
- Fixed internal thread creation in `sll_platform_start_thread`
- Fixed invalid identifier parsing
- Fixed multiple edge case parsing scenarios
- Fixed null pointer dereference during parsing
- Fixed shallow object copy in `sll_object_clone`
- Fixed VM initialization bug
- Function `clib/library$unload` now return an error or `nil`
- Merged `sll_builtin_internal_function_data` and `sll_builtin_internal_function_count` into `sll_builtin_internal_functions`
- Moved the `mime.sll` file to the `http` extension module
- Non-containter object access now returns `nil`

[0.7.22]: https://github.com/sl-lang/sll/compare/sll-v0.7.21...main
[#285]: https://github.com/sl-lang/sll/issues/285
