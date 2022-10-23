ARG VARIANT="jammy"
ARG USERNAME="vscode"
FROM mcr.microsoft.com/vscode/devcontainers/base:0-${VARIANT}
RUN apt-get update&&export DEBIAN_FRONTEND=noninteractive&&apt-get -y install --no-install-recommends nasm gcc
RUN SNIPPET="export PROMPT_COMMAND='history -a'&&export HISTFILE=/commandhistory/.bash_history"&&mkdir /commandhistory&&touch /commandhistory/.bash_history&&chown -R $USERNAME /commandhistory&&echo "$SNIPPET">>"/home/$USERNAME/.bashrc"
