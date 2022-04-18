# Sll [0.7.14] - Ongoing

## Added

- Common assembly code is now located in [`src/sll/include/sll/_internal/common.inc`][0.7.14/src/sll/include/sll/_internal/common.inc]
- Implemented `sll_version_sha` and `sll_version_tag`
- NASM is now used in the compilation process

## Changed

- API functions are now called directly, without the wrapper functions (named `XXX_raw`)
- Audit event `sll.error.backtrace` no longer contains the amount of stack frames skipped
- Fixed crashes related to signals recived at unexpected times
- Functions `sll_set_log_default`, `sll_set_log_file` and `sll_set_log_function` have been merged into `sll_set_log_flags`
- Negative time zone offsets are now correctly printed in `date$date_type`

## Removed

- API-related header files
- Unneeded `SLL_RETURN_TYPE_xxx` constants
- Unneeded type: `sll_return_type_t`

[0.7.14]: https://github.com/sl-lang/sll/compare/sll-v0.7.13...main
[0.7.14/src/sll/include/sll/_internal/common.inc]: https://github.com/sl-lang/sll/blob/main/src/sll/include/sll/_internal/common.inc