#!/usr/bin/bash
if [ "$(uname -s)" == "Darwin" ]; then
	platform="darwin"
else
	platform="linux"
fi
version=`cat build/version`
cd build
zip -9 "../$platform.zip" sll "sll-$version.so" sll.h lib/stdlib.slb lib_debug/stdlib.slb
cd ..
