# Sll [0.8.2] - Ongoing

## Added

- Ability to enable GFX debug mode in `ui:core$create` and `ui:serialize$deserialize`
- Ability to specify element-specific callbacks in the UI: `element$base$callbacks`
- DS4 controller interface library: `ds4`
- Moved all UI color-related types and functions to `ui/rgb.sll`
- New UI elements: `ui:element:gradient` and `ui:element:text`

## Changed

- Fixed signed `dats_file` read functions
- Fixed UI serialization of named elements
- Fixed UI slider movement and resizing
- Floored division resulting in a floating-point number is now be correctly rounded down
- Improved GFX texture synchronization performance
- Improved performance of UI renderer
- Improved UI serializer algorithm
- Renamed `ui:core$set_background` to `ui:core$set_background_color`
- UI `ui:element:color` element now accepts per-vertex colors
- UI color and position types are now serializeable

[0.8.2]: https://github.com/sl-lang/sll/compare/sll-v0.8.1...main
