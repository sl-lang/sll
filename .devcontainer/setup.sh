#!/bin/bash
apt-get update
export DEBIAN_FRONTEND=noninteractive
apt-get -y install --no-install-recommends nasm gcc
apt-get clean
rm -rf /var/lib/apt/lists/*
mkdir /commandhistory
touch /commandhistory/.bash_history
chown -R vscode /commandhistory
echo "export PROMPT_COMMAND='history -a'&&export HISTFILE=/commandhistory/.bash_history" >> "/home/vscode/.bashrc"
