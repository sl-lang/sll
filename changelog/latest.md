# Sll [0.7.13] - Ongoing

## Added

- Argument parsing code to parse a complex number: `d`
- Argument parsing modifier to parse an array of a given type: `+`
- CLI option to query the installation path of specified objects: `-i` or `--install-path`
- Imaginary numbers can be created by adding the `i` suffix to a number ([#262])
- Implemented `sll_arg_state_t`, `sll_free_args` and `sll_parse_args_list`
- Implemented `sll_complex_abs`, `sll_complex_add`, `sll_complex_conjugate`, `sll_complex_div`, `sll_complex_div_float`, `sll_complex_mult`, `sll_complex_mult_float`, `sll_complex_neg`, `sll_complex_pow`, `sll_complex_pow_float`, `sll_complex_pow_int`, `sll_complex_reciprocal` and `sll_complex_sub` ([#262])
- Implemented `sll_complex_t` ([#262])
- Implemented `sll_compressed_integer_t`, `sll_compress_integer`, `sll_decompress_integer` and `SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_INT_COMPRESSED`
- Implemented `SLL_DECODE_SIGNED_INTEGER` and `SLL_ENCODE_SIGNED_INTEGER`
- Implemented `SLL_ERROR_FLAG_SLL`, `SLL_UNMAPPED_WINDOWS_ERROR`, `SLL_ERROR_EOF`, `SLL_ERROR_GET_EXTRA` and `sll_platform_get_error`
- Implemented `SLL_NODE_TYPE_COMPLEX` and `SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_COMPLEX` ([#262])
- Implemented `SLL_OBJECT_TYPE_COMPLEX`, `SLL_INIT_COMPLEX`, `SLL_INIT_COMPLEX_STRUCT`, `sll_static_complex_zero`, `sll_complex_to_object` and `complex_type` ([#262])
- Implemented `sll_parse_arg_count`
- Implemented `sll_return_type_t` and `SLL_RETURN_TYPE_xxx` constants
- Implemented `sll_var_arg_get_complex`
- Implemented `statistics$quantiles` ([#260])
- Improved the performance of `sll_object_get_field` and `sll_object_set_field` ([#268])
- Improved the string matching performance ([#59] and [#268])
- Integer and floats can now be prefixed with the `+` sign for consistency
- Memory allocations are now served from a pool from small allocations ([#267])
- Object creation code to generate complex numbers: `d`
- Object creation modifier to dereference array elements: `*`

## Changed

- All Sll API functions now use the Microsoft x64 calling convention
- AVX, MMX and SSE instructions are now used in multiple functions
- Fixed file lock acquisition
- Fixed lack of memory initialization in array creation
- Fixed multiple memory leaks
- More platform-dependent function return error codes
- Pointers returned by `sll_var_arg_get` are no longer marked as `const`
- Reduced the number of calls to `sll_platform_lock_acquire` and `sll_platform_lock_release`
- Reduced the number of thread-local variables
- Renamed `SLL_ERROR_FLAG_LIBC` and `error_codes$LIBC_ERROR_CODES` to `SLL_ERROR_FLAG_SYSTEM` and `error_codes$SYSTEM_ERROR_CODES`
- Renamed `sll_string_length_unaligned` to `sll_string_length`
- Renamed `XXX_fail` function to `XXX_raw`
- Renamed argument parsing modifier symbolizing variadic functions from `+` to `!`
- Rewritten the CLI executable loader code
- Split `sll_string_pointer_to_object` to `sll_string_pointer_to_object` and `sll_string_pointer_length_to_object`
- Split the operators from [`src/sll/operator.c`][0.7.12/src/sll/operator.c] to [`src/sll/operator`][0.7.13/src/sll/operator]
- *\[Windows Only\]* Windows error codes are now remapped to POSIX `errno` values

## Removed

- Thread number limit macro: `SLL_SCHEDULER_MAX_THREADS`
- Unused define: `SLL_ERROR_FLAG_WINAPI`
- Unused function: `sll_string_length`
- Unused map: `error$WINDOWS_ERROR_CODES`
- Unused type: `sll_decl_copy_node_data_t`

[0.7.13]: https://github.com/sl-lang/sll/compare/sll-v0.7.12...sll-v0.7.13
[#268]: https://github.com/sl-lang/sll/issues/268
[#267]: https://github.com/sl-lang/sll/issues/267
[#262]: https://github.com/sl-lang/sll/issues/262
[#260]: https://github.com/sl-lang/sll/issues/260
[#59]: https://github.com/sl-lang/sll/issues/59
[0.7.13/src/sll/operator]: https://github.com/sl-lang/sll/blob/sll-v0.7.13/src/sll/operator
[0.7.12/src/sll/operator.c]: https://github.com/sl-lang/sll/blob/sll-v0.7.12/src/sll/operator.c
