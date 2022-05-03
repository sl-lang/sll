# Sll [0.7.16] - Ongoing

## Added

- Argument parsing code to parse a float or a complex number: `X`
- Argument parsing code to parse an integer, a float or a complex number: `z`
- Argument parsing modifier to allow `nil` arguments: `@`
- CLI flag to diable all audit events: `-X` or `--disable-audit`
- Implemented `file$FLAG_FLUSH_ON_NEWLINE`
- Implemented `sll_audit_enable`
- Implemented `sll_complex_exp` and `sll_complex_log`
- Implemented `sll_execution_flags_t`, `sll_process_creation_flags_t` and `sll_search_flags_t`
- Implemented `sll_file_handle_t`
- Implemented `sll_math_abs` and `sll_math_copy_sign`
- Implemented `sll_number_type_t` and `SLL_PARSE_ARGS_TYPE_COMPLEX`
- Implemented `sll_platform_allocate_page_aligned`
- Nightly builds (available via the `/apt/nightly/` channel)

## Changed

- Improved compilation speed
- More system functions return error codes
- Moved code between header files
- Renamed `sll_flags_t` to `sll_logger_flags_t`
- Renamed `sll_int_float_t` and `sll_int_float_data_t` to `sll_number_t` and `sll_number_data_t`

## Removed

- Duplicated function: `sll_math_euler_phi`
- Float compare error API: `sll_float_compare_error`, `sll_api_float_get_compare_error`, `sll_api_float_set_compare_error`, `float$get_compare_error` and `float$set_compare_error`
- Unneeded macro: `SLL_UNKNOWN_LIBRARY_HANDLE`
- Unused limit macros: `SLL_MAX_EXPORT_TABLE_LENGTH` and `SLL_MAX_STACK_OFFSET`
- Unused types: `sll_assembly_file_table_length_t`, `sll_checksum_t`, `sll_name_length_t`, `sll_object_type_field_t`, `sll_unary_operator_t`, `sll_ternary_operator_t` and `sll_quaternary_operator_t`

[0.7.16]: https://github.com/sl-lang/sll/compare/sll-v0.7.15...main
