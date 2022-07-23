# Sll [0.7.27] - Ongoing

## Added

- Identifier type: `sll_identifier_table_length_t`

## Changed

- Fixed critical uninitialized memory access bugs in API calls
- Unified small and long identifier tables

## Removed

- Identifier functiond: `sll_identifier_add_index`, `sll_identifier_create`, `sll_identifier_get_array_id`, `sll_identifier_get_array_index`
- Identifier macros: `SLL_MAX_IDENTIFIER_LIST_LENGTH`, `SLL_MAX_SHORT_IDENTIFIER_LENGTH`, `SLL_IDENTIFIER_GET_ARRAY_ID`, `SLL_IDENTIFIER_GET_ARRAY_INDEX`, `SLL_IDENTIFIER_ADD_INDEX` and `SLL_CREATE_IDENTIFIER`
- Identifier types: `sll_identifier_list_t` and `sll_identifier_list_length_t`

[0.7.27]: https://github.com/sl-lang/sll/compare/sll-v0.7.26...main
