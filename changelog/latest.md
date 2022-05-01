# Sll [0.7.16] - Ongoing

## Added

- CLI flag to diable all audit events: `-X` or `--disable-audit`
- Implemented `file$FLAG_FLUSH_ON_NEWLINE`
- Implemented `sll_audit_enable`
- Implemented `sll_execution_flags_t`, `sll_process_creation_flags_t` and `sll_search_flags_t`
- Implemented `sll_platform_allocate_page_aligned`
- Nightly builds (available via the `/apt/nightly/` channel)

## Changed

- Improved compilation speed
- Moved code between header files
- Renamed `sll_flags_t` to `sll_logger_flags_t`

## Removed

- Unused limit macros: `SLL_MAX_EXPORT_TABLE_LENGTH` and `SLL_MAX_STACK_OFFSET`
- Unused types: `sll_assembly_file_table_length_t`, `sll_checksum_t`, `sll_name_length_t`, `sll_object_type_field_t`, `sll_unary_operator_t`, `sll_ternary_operator_t` and `sll_quaternary_operator_t`

[0.7.16]: https://github.com/sl-lang/sll/compare/sll-v0.7.15...main
