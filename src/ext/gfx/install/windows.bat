@echo off
curl -L -o "build/ext/gfx_windows/glslang.zip" "https://github.com/KhronosGroup/glslang/releases/download/master-tot/glslang-master-windows-x64-Release.zip"
powershell Expand-Archive build/ext/gfx_windows/glslang.zip -DestinationPath build/ext/gfx_windows/glslang
mkdir build\ext\gfx_windows\vulkan_headers
git clone "https://github.com/KhronosGroup/Vulkan-Headers.git" build\ext\gfx_windows\vulkan_headers
