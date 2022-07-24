# Sll [0.7.28] - Ongoing

## Added

- Implemented `sll_platform_socket_read` and `sll_platform_socket_write`
- Implemented `sll_std_file_type_t`

## Changed

- Fixed API calls with multiples of 256 arguments
- Fixed crashes related to overriden registers in CLib function calls
- Fixed file deadlocks
- Fixed socket address info lookup errors
- Fixed unwanted frames in `error$create` and `error$from_internal`
- Object type (`sll_object_t`) is now a pointer to a structure

## Removed

- Unused macro constant: `SLL_MAX_SIZE`

[0.7.28]: https://github.com/sl-lang/sll/compare/sll-v0.7.27...main
