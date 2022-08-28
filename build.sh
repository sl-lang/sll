#!/bin/bash
[ ! -d "build" ]&&mkdir build
if [ "$(uname -s)" == "Darwin" ]; then
	platform="darwin"
else
	platform="linux"
fi
if [ ! -d "build/_sll_runtime_$platform" ]; then
	mkdir "build/_sll_runtime_$platform"
	curl -o "build/_sll_runtime_$platform/$platform.zip" "https://sl-lang.github.io/bin/$platform.zip"
	unzip -qq -o "build/_sll_runtime_$platform/$platform.zip" -d "build/_sll_runtime_$platform/"
	rm -f "build/_sll_runtime_$platform/$platform.zip"
fi
if [[ ! -f "build/_build_script.sla" || "$SLL_FORCE_REBUILD" != "" ]]; then
	"build/_sll_runtime_$platform/sll" src/_build/main.sll -I "@build-script|src/_build" -O -x 5 -r -R -a -o build/_build_script
fi
"build/_sll_runtime_$platform/sll" build/_build_script.sla -A "$@"
[[ "$?" == "0" || "$?" == "6" ]]&&exit 0
