# Sll [0.7.20] - Ongoing

## Added

- Ability to use assembly in extension modules
- C interface library: `clib` ([#286])
- Implemented `audit$register_event`
- Implemented `sll_gc_add_root`, `sll_gc_add_roots`, `sll_gc_collect`, `sll_gc_remove_root` and `sll_gc_remove_roots` ([#287])
- Implemented `SLL_ERROR_STRING`, `sll_error_from_string_pointer`, `sll_error_get_string_pointer` and `error$ERROR_STRING`
- Tests ([#285])

## Changed

- Current dynamic library handle can be obtained by calling `sll_platform_load_library` with a `NULL` file path argument
- Fixed CLI output when supplied a directory
- Fixed issues with constant rebuilding of assembly files
- Fixed memory leaks caused by circular references ([#287])
- Fixed stringification of circular references
- Renamed `audit$BUILTIN_EVENTS` to `audit$EVENT_ARGUMENTS`
- Thread, lock, barrier and semaphore creation functions now return error objects

## Removed

- Deprecated `apt-key` is no longer used for repository keys

[0.7.20]: https://github.com/sl-lang/sll/compare/sll-v0.7.19...main
[#287]: https://github.com/sl-lang/sll/issues/287
[#286]: https://github.com/sl-lang/sll/issues/286
[#285]: https://github.com/sl-lang/sll/issues/285
