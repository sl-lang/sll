#!/bin/bash
sudo apt install vulkan-tools
if [ ! -d "build/ext/gfx_linux/vulkan_headers" ]; then
	mkdir build/ext/gfx_linux/vulkan_headers
	git clone https://github.com/KhronosGroup/Vulkan-Headers.git build/ext/gfx_linux/vulkan_headers
fi
