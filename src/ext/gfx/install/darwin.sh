#!/bin/bash
if [ ! -d "build/ext/gfx_darwin/vulkan_headers" ]; then
	mkdir build/ext/gfx_darwin/vulkan_headers
	git clone https://github.com/KhronosGroup/Vulkan-Headers.git build/ext/gfx_darwin/vulkan_headers
fi
