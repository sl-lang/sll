#!/bin/bash
wget -qO - "https://sll.krzem.workers.dev/apt/key.gpg" | sudo apt-key add -
echo "deb https://sll.krzem.workers.dev/ apt/unstable/" | sudo tee /etc/apt/sources.list.d/sll.list
sudo apt-get update
sudo apt-get install sll
