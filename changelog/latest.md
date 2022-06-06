# Sll [0.7.21] - Ongoing

## Added

- CLI option to treat warnings as errors: `-E` or `--enable-errors`

## Changed

- Build script compilation now uses optimization
- Fixed issue with backslash escaping in `sll_api_process_start`
- Funcion `sll_get_location` now accepts `NULL` arguments
- Renamed `call_stack_type$return` to `call_stack_type$location`

## Removed

- Call stack frames no longer contain the name of the function
- Verbose output from module compilation

[0.7.21]: https://github.com/sl-lang/sll/compare/sll-v0.7.20...main
