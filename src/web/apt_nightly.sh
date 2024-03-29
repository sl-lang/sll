#!/bin/bash
[ ! -d "/usr/local/share/keyrings" ]&&mkdir /usr/local/share/keyrings
curl -s "https://sl-lang.github.io/pkg/apt/key.asc" > /usr/local/share/keyrings/sll.asc
rm -f /tmp/sll-keyring.gpg
echo "deb [signed-by=/usr/local/share/keyrings/sll.asc] https://sl-lang.github.io/ pkg/apt/nightly/" > /etc/apt/sources.list.d/sll.list
apt update
export DEBIAN_FRONTEND=noninteractive
sudo apt -y install --no-install-recommends sll
