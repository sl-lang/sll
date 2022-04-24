# Sll [0.7.15] - Ongoing

## Added

- Checks for unknown file descriptors in the serial API

## Changed

- Fixed execution of an arbitrary memory location ([#272])
- Assembly jump instructions now store a pointer to the target instruction ([#270])
- Renamed audit event `sll.process.shell` to `sll.process.start.shell` ([#271])

[0.7.15]: https://github.com/sl-lang/sll/compare/sll-v0.7.14...main
[#272]: https://github.com/sl-lang/sll/issues/272
[#271]: https://github.com/sl-lang/sll/issues/271
[#270]: https://github.com/sl-lang/sll/issues/270
