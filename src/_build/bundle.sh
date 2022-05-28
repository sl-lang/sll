#!/bin/bash
if [ "$(uname -s)" == "Darwin" ]; then
	platform="darwin"
else
	platform="linux"
fi
zip -9 "$platform.zip" build/sll "build/sll-$(cat build/version).so" build/sll.h $(find build -name "*.slb" -type f -and -not -path "build/_sll_runtime*/*" -or -name "*.*" -path "build/sys_lib/*")
