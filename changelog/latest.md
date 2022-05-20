# Sll [0.7.18] - Ongoing

## Added

- CLI flag to load path resolver library: `-F` or `--file-path-resolver`
- CLI include directories can now be aliases pointing to directories (separated by `|`)
- Fast object pool used in the GC
- Hash descriptor type `hash$hash_type` now contains a field `type` containing the type returned by functions from the given descriptor
- Implemented `path$join`
- Implemented `SLL_ABI_PATH_RESOLVER_DEINIT`, `SLL_ABI_PATH_RESOLVER_INIT` and `SLL_ABI_PATH_RESOLVER_RESOLVE`
- Implemented `sll_cli_lookup_result_t`, `sll_cli_lookup_data_t`, `sll_cli_path_resolver_t`, `sll_cli_expand_path`, `sll_cli_lookup_file`, `sll_cli_register_path_resolver` and `sll_cli_unregister_path_resolver`
- Implemented `sll_compilation_data_from_source_file`
- Implemented `sll_file_read_all` and `file$read` with no arguments
- Implemented `sll_object_to_map`
- Implemented `thread$exit`
- Operator for ending execution of the current thread and returning a value: `!@@`

## Changed

- Fixed `sll_map_remove`
- Fixed `sll_string_compare`, `sll_string_includes`, `sll_string_replace`, `sll_string_select` and `sll_string_split`
- Fixed assembly stack frames
- Fixed incorrect assembly jump generation
- *\[Linux only\]* Fixed permission bits for directory creation in `sll_platform_create_directory`
- Fixed weak references created via the API
- Improved the performance of the GC
- Translated part of the build script to Sll ([#75])

## Removed

- Function `gauss2` is no longer directly exported from `math.sll`

[0.7.18]: https://github.com/sl-lang/sll/compare/sll-v0.7.17...main
[#75]: https://github.com/sl-lang/sll/issues/75
