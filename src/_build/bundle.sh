#!/bin/bash
if [ "$(uname -s)" == "Darwin" ]; then
	platform="darwin"
else
	platform="linux"
fi
(
	cd build||exit
	zip -9 "../$platform.zip" sll "sll-$(cat version).so" sll.h lib/stdlib.slb lib_debug/stdlib.slb
)
