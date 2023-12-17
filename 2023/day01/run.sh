#!/bin/bash

input_file="$1"

mkdir bin -p

g++ -std=c++17 -Werror -o bin/main main.cpp
if [[ $? == 0 ]]; then
    bin/main $input_file
fi
