#!/bin/bash

mkdir build
cd build || exit
cmake .. -DCMAKE_BUILD_TYPE=Debug
make -j4
cd ..

dir_path="bin/tests"

if [ ! -d "$dir_path" ]; then
    echo "Error: Specified directory does not exist"
    exit 1
fi

for file in "$dir_path"/*; do
    if [ -x "$file" ] && [ -f "$file" ]; then
        echo "--------------- Executing: $file ---------------"
        "$file"
        echo "--------------- Executed: $file  ---------------"
        echo ""
    fi
done

echo "All executable files have been executed"