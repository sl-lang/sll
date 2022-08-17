#!/bin/bash
sudo apt install vulkan-tools
if [ ! -d "build/ext/vulkan/headers" ]; then
	mkdir build/ext/vulkan/headers
	git clone https://github.com/KhronosGroup/Vulkan-Headers.git build/ext/vulkan/headers
fi
