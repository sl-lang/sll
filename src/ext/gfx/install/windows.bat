@echo off
if not exist build\ext\gfx_windows\vulkan_headers (
	mkdir build\ext\gfx_windows\vulkan_headers
	git clone "https://github.com/KhronosGroup/Vulkan-Headers.git" build\ext\gfx_windows\vulkan_headers
)
