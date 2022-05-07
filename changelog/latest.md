# Sll [0.7.17] - Ongoing

## Added

- Implemented `sys$ROOT_PATH`

## Changed

- Audit event `sll.sys.library.load` now also contains the path to the library
- Libraries loaded via `load_library` are now longer constrained to the `sys_lib/` directory
- Library extensions returned by `sys$LIBRARY_EXTENSION` now contain the separating dot

## Removed

- Extension directory (`sys_lib/`), `sys$LIBRARY_PATH` and `-i extension-library-path`

[0.7.17]: https://github.com/sl-lang/sll/compare/sll-v0.7.16...main
