@echo off
if not exist build mkdir build
python src\_build\main.py %*
