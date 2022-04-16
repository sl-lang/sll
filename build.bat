@echo off
if not exist build mkdir build
if not exist build\_sll_runtime_windows (
	mkdir build\_sll_runtime_windows
	curl -o build\_sll_runtime_windows\windows.zip https://sll.krzem.workers.dev/bin/windows.zip
	powershell -command "Expand-Archive -Force 'build\_sll_runtime_windows\windows.zip' 'build\_sll_runtime_windows\'"
	del /f /q build\_sll_runtime_windows\windows.zip
)
python src\_build\main.py %*
