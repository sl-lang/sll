#!/bin/bash
[ ! -d "build" ]&&mkdir build
if [ "$(uname -s)" == "Darwin" ]; then
	platform="darwin"
else
	platform="linux"
fi
if [ ! -d "build/_sll_runtime_$platform" ]; then
	mkdir "build/_sll_runtime_$platform"
	curl -o "build/_sll_runtime_$platform/$platform.zip" "https://sll.krzem.workers.dev/bin/nightly/$platform.zip"
	unzip -qq -o "build/_sll_runtime_$platform/$platform.zip" -d "build/_sll_runtime_$platform/"
	rm -f "build/_sll_runtime_$platform/$platform.zip"
fi
python3 src/_build/main.py "$@"
