#!/bin/bash
curl -s "https://sll.krzem.workers.dev/apt/key.gpg" | apt-key add -
echo "deb https://sll.krzem.workers.dev/ apt/nightly/" | tee /etc/apt/sources.list.d/sll.list
apt update
apt install sll
