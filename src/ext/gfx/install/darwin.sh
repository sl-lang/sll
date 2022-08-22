#!/bin/bash
curl -L -o "build/ext/gfx_darwin/glslang.zip" "https://github.com/KhronosGroup/glslang/releases/download/master-tot/glslang-master-osx-x86_64-Release.zip"
unzip build/ext/gfx_darwin/glslang.zip -d build/ext/gfx_darwin/glslang
if [ ! -d "build/ext/gfx_darwin/vulkan_headers" ]; then
	mkdir "build/ext/gfx_darwin/vulkan_headers"
	git clone https://github.com/KhronosGroup/Vulkan-Headers.git "build/ext/gfx_darwin/vulkan_headers"
fi
