# Sll [0.7.28] - 2022-07-30

## Added

- Error raised during library initialization failure: `SLL_ERROR_INITIALIZATION`
- Implemented `error$ERROR_INITIALIZATION` and `error$ERROR_NOT_A_SOCKET`
- Implemented `sll_platform_socket_read` and `sll_platform_socket_write`
- Implemented `sll_std_file_type_t`
- Macro for checking if a node has children: `SLL_NODE_HAS_CHILDREN`

## Changed

- Fixed API calls with multiples of 256 arguments
- Fixed crashes related to modified registers in CLib function calls
- Fixed file deadlocks
- Fixed issue with environment variable creation via `sll_set_environment_variable`
- Fixed socket address info lookup errors
- *\[Windows only\]* Fixed socket initialization
- Fixed unwanted frames in `error$create` and `error$from_internal`
- Object type (`sll_object_t`) is now a pointer to a structure

## Removed

- Unused macro constant: `SLL_MAX_SIZE`
- Unused macros: `SLL_IS_OBJECT_TYPE_IF` and `SLL_IS_OBJECT_TYPE_NOT_TYPE`

[0.7.28]: https://github.com/sl-lang/sll/compare/sll-v0.7.27...main
