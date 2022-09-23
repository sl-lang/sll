#!/bin/bash
required_packages=( "libfontconfig1" "libfontconfig-dev" )
packages_to_install=""
for pkg in "${required_packages[@]}"; do
	if [ "" = "$(dpkg-query -W --showformat='${Status}\n' $pkg|grep "install ok installed")" ]; then
		packages_to_install+=" $pkg"
	fi
done
if [ "" != "$packages_to_install" ]; then
	sudo apt update
	eval "sudo apt install$packages_to_install"
fi
mkdir "build/ext/font_linux/stb_libraries"
git clone https://github.com/nothings/stb.git "build/ext/font_linux/stb_libraries"
