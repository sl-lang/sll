@echo off
cls
goto build
if not exist build\builder\main.exe (
:build
	if not exist build mkdir build
	if not exist build\builder (
		mkdir build\builder
	) else del /s /q /f build\builder\*>nul
	cd build\builder
	cl.exe ../../src/builder/*.c /nologo
	cls
	cd ../../
)
build\builder\main.exe %*
