# Sll [0.8.2] - Ongoing

## Added

- Ability to specify element-specific callbacks in the UI: `element$base$callbacks`
- Moved all UI color-related types and functions to `ui/rgb.sll`
- New UI element: `ui:element:gradient`

## Changed

- Fixed UI slider movement and resizing
- Floored division resulting in a floating-point number is now be correctly rounded down
- UI `ui:element:color` element now accepts per-vertex colors
- UI color and position types are now serializeable

[0.8.2]: https://github.com/sl-lang/sll/compare/sll-v0.8.1...main
