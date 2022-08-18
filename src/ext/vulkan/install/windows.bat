@echo off
echo $$$ Line 1
curl -L -o build/ext/vulkan_windows/installer.exe "https://sdk.lunarg.com/sdk/download/latest/windows/vulkan-sdk.exe"
echo $$$ Line 2
build/ext/vulkan_windows/installer.exe /S
echo $$$ Line 3
if not exist build\ext\vulkan_windows\headers (
	echo $$$ Line 4
	mkdir build\ext\vulkan_windows\headers
	echo $$$ Line 5
	git clone "https://github.com/KhronosGroup/Vulkan-Headers.git" build\ext\vulkan_windows\headers
)
