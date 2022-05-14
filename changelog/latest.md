# Sll [0.7.17] - 2022-05-14

## Added

- Implemented `sys$ROOT_PATH`
- Implemented the stack memory allocator ([#281])

## Changed

- Audit event `sll.sys.library.load` now also contains the path to the library
- Buffer passed to `sll_file_from_data` is no longer internaly allocated
- EOF error (`error$ERROR_EOF`) now contains the file that raised the error ([#277])
- *\[Windows only\]* Fixed `sll_platform_set_cpu`
- Libraries loaded via `load_library` are now longer constrained to the `sys_lib/` directory
- Library extensions returned by `sys$LIBRARY_EXTENSION` now contain the separating dot
- Newline file flushes are now enabled in append mode ([#276])
- Replaced arbitrary memory pointer dereferences with shell types ([#283])
- Replaced char pointer casting with `ADDR` and `PTR` macros ([#282])
- XMM registers are now cleared before internal function call ([#278])

## Removed

- Extension directory (`sys_lib/`), `sys$LIBRARY_PATH` and `-i extension-library-path`
- Unused constant macro: `SLL_MAX_PAGE_SIZE`

[0.7.17]: https://github.com/sl-lang/sll/compare/sll-v0.7.16...main
[#283]: https://github.com/sl-lang/sll/issues/283
[#282]: https://github.com/sl-lang/sll/issues/282
[#281]: https://github.com/sl-lang/sll/issues/281
[#278]: https://github.com/sl-lang/sll/issues/278
[#277]: https://github.com/sl-lang/sll/issues/277
[#276]: https://github.com/sl-lang/sll/issues/276
