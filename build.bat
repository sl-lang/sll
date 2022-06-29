@echo off
if not exist build mkdir build
if not exist build\_sll_runtime_windows (
	mkdir build\_sll_runtime_windows
	curl -o build\_sll_runtime_windows\windows.zip "https://sl-lang.github.io/bin/nightly/windows.zip"
	powershell -command "Expand-Archive -Force 'build\_sll_runtime_windows\windows.zip' 'build\_sll_runtime_windows\'"
	del /f /q build\_sll_runtime_windows\windows.zip
)
if not exist build\_build_script.sla (
	build\_sll_runtime_windows\sll.exe src\_build\main.sll -I "@build-script|src\_build" -R -a -o build/_build_script
)
build\_sll_runtime_windows\sll.exe build\_build_script.sla -A %*
