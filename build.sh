#!/bin/bash
[ ! -d "build" ]&&mkdir build
if [ "$(uname -s)" == "Darwin" ]; then
	platform="darwin"
else
	platform="linux"
fi
if [ ! -d "build/_sll_runtime_$platform" ]; then
	mkdir "build/_sll_runtime_$platform"
	curl -o "build/_sll_runtime_$platform/$platform.zip" "https://sll.krzem.workers.dev/bin/$platform.zip"
	unzip -qq -o "build/_sll_runtime_$platform/$platform.zip" -d "build/_sll_runtime_$platform/"
	rm -f "build/_sll_runtime_$platform/$platform.zip"
fi
if [ ! -f "build/_build_script.sla" ]; then
	"build/_sll_runtime_$platform/sll" src/_build/main.sll -I "@build-script|src/_build" -d -r -R -a -o build/_build_script
fi
"build/_sll_runtime_$platform/sll" build/_build_script.sla -A "$@"
