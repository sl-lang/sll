# Sll [0.7.21] - 2022-06-11

## Added

- CLI option to treat warnings as errors: `-E` or `--enable-errors`

## Changed

- Build script compilation now uses optimization
- Fixed `sll_object_get_field`, `sll_object_set_field` and `sll_object_to_array` to use the provided type table
- Fixed issue with backslash escaping in `sll_api_process_start`
- Funcion `sll_get_location` now accepts `NULL` arguments
- Renamed `call_stack_type$return` to `call_stack_type$location`

## Removed

- Call stack frames no longer contain the name of the function
- Verbose output from module compilation

[0.7.21]: https://github.com/sl-lang/sll/compare/sll-v0.7.20...sll-v0.7.21
