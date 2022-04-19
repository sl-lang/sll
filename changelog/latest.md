# Sll [0.7.14] - Ongoing

## Added

- Argument parsing code to parse a 16-bit unsigned integer: `W`
- Argument parsing code to parse a 64-bit unsigned integer: `Q`
- Argument parsing code to parse an 8-bit unsigned integer: `B`
- Common assembly code is now located in [`src/sll/include/sll/_internal/common.inc`][0.7.14/src/sll/include/sll/_internal/common.inc]
- Implemented `sll_version_sha` and `sll_version_tag`
- NASM is now used in the compilation process

## Changed

- API functions are now called directly, without the wrapper functions (named `XXX_raw`)
- Argument parsing code to parse a 32-bit unsigned integer has been renamed from `I` to `D`
- Array functions (`array$extend`, `array$push`, `array$remove` and `array$unshift`) now returns the new length of the array after the operation
- Audit event `sll.error.backtrace` no longer contains the amount of stack frames skipped
- Changing the floating-point compare error via `float$set_compare_error` will now return the old one
- Fixed crashes related to signals recived at unexpected times
- Functions `sll_set_log_default`, `sll_set_log_file` and `sll_set_log_function` have been merged into `sll_set_log_flags`
- logging functions now return a boolean success flag
- Negative time zone offsets are now correctly printed in `date$date_type`

## Removed

- API-related header files
- Unneeded `SLL_RETURN_TYPE_xxx` constants
- Unneeded type: `sll_return_type_t`

[0.7.14]: https://github.com/sl-lang/sll/compare/sll-v0.7.13...main
[0.7.14/src/sll/include/sll/_internal/common.inc]: https://github.com/sl-lang/sll/blob/main/src/sll/include/sll/_internal/common.inc
