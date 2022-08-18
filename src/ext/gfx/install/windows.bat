@echo off
curl -L -o build/ext/gfx_windows/installer.exe "https://sdk.lunarg.com/sdk/download/latest/windows/vulkan-sdk.exe"
: build/ext/gfx_windows/installer.exe /S
if not exist build\ext\gfx_windows\vulkan_headers (
	mkdir build\ext\gfx_windows\vulkan_headers
	git clone "https://github.com/KhronosGroup/Vulkan-Headers.git" build\ext\gfx_windows\vulkan_headers
)
