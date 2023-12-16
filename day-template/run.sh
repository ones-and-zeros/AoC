#!/bin/bash

input_file="$1"

mkdir bin -p
g++ -std=c++17 -o bin/main main.cpp
bin/main $input_file
