# Sll [0.7.15] - Ongoing

## Added

- Checks for unknown file descriptors in the serial API
- Implemented `sll_contains_character`
- Implemented `SLL_FILE_FLUSH_ON_NEWLINE`

## Changed

- Assembly jump instructions now store a pointer to the target instruction ([#270])
- Complex number arguments and return values are now passed by pointers
- File flags are now 16-bit
- Fixed execution of an arbitrary memory location ([#272])
- Increased per-core thread count limit from `8192` to `16384`
- Registers are now stored in the register area in debug mode
- Renamed audit event `sll.process.shell` to `sll.process.start.shell` ([#271])
- Renamed website page names

[0.7.15]: https://github.com/sl-lang/sll/compare/sll-v0.7.14...main
[#272]: https://github.com/sl-lang/sll/issues/272
[#271]: https://github.com/sl-lang/sll/issues/271
[#270]: https://github.com/sl-lang/sll/issues/270
