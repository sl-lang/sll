#!/bin/bash
curl -s "https://sl-lang.github.io/apt/key.gpg" | apt-key add -
echo "deb https://sl-lang.github.io/ apt/unstable/" | tee /etc/apt/sources.list.d/sll.list
apt update
apt install sll
