# Sll [0.8.1] - Ongoing

## Added

- Ability to create UI elements without the parent UI, for layout calculation purposes
- Ability to serialize and deserialize UI elements and layouts
- Implemented filter layouts in the UI extension
- UI button element

## Updated

- All UI layout functions and types are now located in `ui/layout.sll`
- Fixed incorrect window callback calls
- Fixed Window manager mouse-button related events
- *\[Linux Only\]* Fixed X11 lag when using a GFX context
- Merged `ui/position.sll` and `ui/size.sll`

## Removed

- Unneeded file: `ui/size.sll`
- Window manager geometry-change callabck: `window_manager_window_type$callbacks$geometry`

[0.8.1]: https://github.com/sl-lang/sll/compare/sll-v0.8.0...main
