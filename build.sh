#!/bin/bash
[ ! -d "build" ]&&mkdir build
python src/_build/main.py "$@"
