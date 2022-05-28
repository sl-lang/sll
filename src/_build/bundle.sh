#!/bin/bash
if [ "$(uname -s)" == "Darwin" ]; then
	platform="darwin"
else
	platform="linux"
fi
(
	cd build||exit
	zip -9 "../$platform.zip" sll "sll-$(cat version).so" sll.h $(find . -name "*.slb" -type f -and -not -path "./_sll_runtime*/*" -or -name "*.*" -path "./sys_lib/*")
)
