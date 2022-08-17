curl -L -o build/ext/vulkan_windows/installer.exe "https://sdk.lunarg.com/sdk/download/latest/windows/vulkan-sdk.exe"
build/ext/vulkan_windows/installer.exe /S
if not exist build\ext\vulkan_windows\headers (
	mkdir build\ext\vulkan_windows\headers
	git clone "https://github.com/KhronosGroup/Vulkan-Headers.git" build\ext\vulkan_windows\headers
)
