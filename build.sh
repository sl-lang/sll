#!/bin/bash
[ ! -d "build" ]&&mkdir build
python3 src/_build/main.py "$@"
