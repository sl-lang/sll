# Sll [0.7.22] - Ongoing

## Added

- Implemented `sll_internal_function_table_descriptor_t`
- Implemented `sll_platform_get_library_file_path`
- Implemented `SLL_RANDOM_BITS`
- Internal function descriptors located in `SLL_ABI_INTERNAL_FUNCTION_TABLE_DESCRIPTOR` are now automatically loaded upon initialization
- More tests ([#285])

## Changed

- CLib library names are now absolute file paths to modules
- Fixed array and string underflow indexing
- Fixed shallow object copy in `sll_object_clone`
- Merged `sll_builtin_internal_function_data` and `sll_builtin_internal_function_count` into `sll_builtin_internal_functions`
- Moved the `mime.sll` file to the `http` extension module
- Non-containter object access now returns `nil`

[0.7.22]: https://github.com/sl-lang/sll/compare/sll-v0.7.21...main
[#285]: https://github.com/sl-lang/sll/issues/285
