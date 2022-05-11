# Sll [0.7.17] - Ongoing

## Added

- Implemented `sys$ROOT_PATH`

## Changed

- Audit event `sll.sys.library.load` now also contains the path to the library
- Buffer passed to `sll_file_from_data` is no longer internaly allocated
- Libraries loaded via `load_library` are now longer constrained to the `sys_lib/` directory
- Library extensions returned by `sys$LIBRARY_EXTENSION` now contain the separating dot
- Newline file flushes are now enabled in append mode ([#276])
- Replaced char pointer casting with `ADDR` and `PTR` macros ([#282])
- XMM registers are now cleared before internal function call ([#278])

## Removed

- Extension directory (`sys_lib/`), `sys$LIBRARY_PATH` and `-i extension-library-path`

[0.7.17]: https://github.com/sl-lang/sll/compare/sll-v0.7.16...main
[#282]: https://github.com/sl-lang/sll/issues/282
[#278]: https://github.com/sl-lang/sll/issues/278
[#276]: https://github.com/sl-lang/sll/issues/276
