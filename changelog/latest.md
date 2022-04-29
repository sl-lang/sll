# Sll [0.7.15] - Ongoing

## Added

- Checks for unknown file descriptors in the serial API
- CPU flags are now checked during the initialization process
- Implemented `sll_contains_character`
- Implemented `SLL_FILE_FLUSH_ON_NEWLINE`
- Implemented `SLL_RELEASE`
- Implemented `SLL_VAR_ARG_INIT_C` and `SLL_VAR_ARG_INIT_SLL`
- Raw string quote escape sequence ([#274])
- Sandbox flag to disable object reference counter queries: `no-ref`

## Changed

- Assembly jump instructions now store a pointer to the target instruction ([#270])
- Changed the website layout
- Complex number arguments and return values are now passed by pointers
- File flags are now 16-bit
- Fixed error codes returned by `sll_api_base64_decode`
- Fixed execution of an arbitrary memory location ([#272])
- Fixed issues related to incorrect cleanup order
- Fixed issues related to the atexit API
- Fixed string checksums returned by `sll_string_from_pointer_length`
- Increased per-core thread count limit from `8192` to `16384`
- Moved website files from [`src/web/client`][0.7.14/src/web/client] to [`src/web`][0.7.15/src/web]
- Registers are now stored in the register area in debug mode
- Renamed audit event `sll.process.shell` to `sll.process.start.shell` ([#271])
- Renamed website page names
- Reworked the `struct.sll` module

## Removed

- Website server code

[0.7.15]: https://github.com/sl-lang/sll/compare/sll-v0.7.14...main
[#274]: https://github.com/sl-lang/sll/issues/274
[#272]: https://github.com/sl-lang/sll/issues/272
[#271]: https://github.com/sl-lang/sll/issues/271
[#270]: https://github.com/sl-lang/sll/issues/270
[0.7.15/src/web]: https://github.com/sl-lang/sll/tree/main/src/web
[0.7.14/src/web/client]: https://github.com/sl-lang/sll/tree/sll-v0.7.14/src/web/client
