# Sll [0.7.18] - Ongoing

## Added

- Fast object pool used in the GC
- Hash descriptor type `hash$hash_type` now contains a field `type` containing the type returned by functions from the given descriptor
- Implemented `path$join`
- Implemented `sll_file_read_all` and `file$read` with no arguments
- Implemented `sll_object_to_map`

## Changed

- Fixed `sll_string_compare`, `sll_string_includes`, `sll_string_split` and `sll_string_replace`
- Fixed assembly stack frames
- Fixed incorrect assembly jump generation
- *\[Linux only\]* Fixed permission bits for directory creation in `sll_platform_create_directory`
- Translated part of the build script to Sll ([#75])

## Removed

- Function `gauss2` is no longer exported from `math.sll`

[0.7.18]: https://github.com/sl-lang/sll/compare/sll-v0.7.17...main
[#75]: https://github.com/sl-lang/sll/issues/75
