#!/bin/bash
sudo apt install vulkan-tools
if [ ! -d "build/ext/vulkan_linux/headers" ]; then
	mkdir build/ext/vulkan_linux/headers
	git clone https://github.com/KhronosGroup/Vulkan-Headers.git build/ext/vulkan_linux/headers
fi
