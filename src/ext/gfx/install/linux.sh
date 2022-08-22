#!/bin/bash
curl -L -o "build/ext/gfx_linux/glslang.zip" "https://github.com/KhronosGroup/glslang/releases/download/master-tot/glslang-master-linux-Release.zip"
unzip build/ext/gfx_linux/glslang.zip -d build/ext/gfx_linux/glslang
if [ ! -d "build/ext/gfx_linux/vulkan_headers" ]; then
	mkdir "build/ext/gfx_linux/vulkan_headers"
	git clone https://github.com/KhronosGroup/Vulkan-Headers.git "build/ext/gfx_linux/vulkan_headers"
fi
