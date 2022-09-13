#!/bin/bash
packages_to_install=""
for pkg in "libxcb-randr0 libxcb-randr0-dev libxcb-xkb-dev libxcb-icccm4 libxcb-icccm4-dev libxcb-cursor0 libxcb-cursor-dev"; do
	if [ "" = "$(dpkg-query -W --showformat='${Status}\n' $pkg|grep "install ok installed")" ]; then
		packages_to_install+=" $pkg"
	fi
done
if [ "" != "$packages_to_install" ]; then
	echo "Installing packages:$packages_to_install"
	eval "sudo apt install$packages_to_install"
fi
