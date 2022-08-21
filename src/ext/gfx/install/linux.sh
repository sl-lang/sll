#!/bin/bash
if [ "$(uname -s)" == "Darwin" ]; then
	platform="darwin"
else
	platform="linux"
fi
if [ ! -d "build/ext/gfx_$platform/vulkan_headers" ]; then
	mkdir "build/ext/gfx_$platform/vulkan_headers"
	git clone https://github.com/KhronosGroup/Vulkan-Headers.git "build/ext/gfx_$platform/vulkan_headers"
fi
