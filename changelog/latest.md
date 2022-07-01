# Sll [0.7.24] - Ongoing

## Added

- Extended the container API to include macro-substitution-functions: `sll_container_clear`, `sll_container_filter`, `sll_container_init`, `sll_container_iter`, `sll_container_iter_clear` and `sll_container_push`
- Extended the handle container API to include macro-substitution-functions: `sll_handle_container_alloc`, `sll_handle_container_clear`, `sll_handle_container_check`, `sll_handle_container_dealloc`, `sll_handle_container_get`, `sll_handle_container_init`, `sll_handle_container_iter` and `sll_handle_container_iter_clear`
- Implemented `sll_container_callback_t` and `sll_container_check_callback_t`
- Implemented `SLL_HANDLE_CONTAINER_GET`
- Implemented `sll_string_parse_int`
- Imolemented `self_type` and `SLL_OBJECT_TYPE_SELF`
- Implemented custom type comparation between same types
- More MIME types in the HTTP extension module

## Changed

- Conditional arguments in `SLL_CONTAINER_FILTER` and `SLL_HANDLE_CONTAINER_FILTER` have been flipped to better reflect their meaning
- Fixed incorrect pointer calculations in container functions
- Reworked the optimizer
- System library loading audit events (`sll.sys.library.load.*`) no longer contain the `name` argument

[0.7.24]: https://github.com/sl-lang/sll/compare/sll-v0.7.23...main
