#!/bin/bash
required_packages=( "libxcb-randr0" "libxcb-randr0-dev" "libxcb-xkb-dev" "libxcb-icccm4" "libxcb-icccm4-dev" "libxcb-cursor0" "libxcb-cursor-dev" )
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
