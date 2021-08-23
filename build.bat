@echo off
cls
setlocal enableextensions
if defined ALWAYS_REBUILD_SLL_BUILDER goto build
if not exist build\builder\main.exe (
:build
	if not exist build mkdir build
	if not exist build\builder (mkdir build\builder) else del /s /q /f build\builder\*>nul
	cd build\builder
	cl /I ../../src/builder/include ../../src/builder/main.c ../../src/builder/util.c ../../src/builder/windows.c /nologo
	cd ../../
)
endlocal
build\builder\main.exe %*
