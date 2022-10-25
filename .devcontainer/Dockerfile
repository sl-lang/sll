ARG VARIANT="jammy"
FROM mcr.microsoft.com/vscode/devcontainers/base:0-${VARIANT}
COPY .devcontainer/setup.sh /tmp/setup.sh
RUN bash /tmp/setup.sh
