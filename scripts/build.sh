#!/bin/bash

if [ ! -d "build" ]; then
  mkdir build
fi

cd build

if [[ $1 = "debug" ]]; then
    echo "building in debug mode"
    complie_flag="-DCMAKE_BUILD_TYPE=Debug"
elif [[ $1 = "release" ]]; then
    echo "building in release mode"
    complie_flag="-DCMAKE_BUILD_TYPE=Release"
else
    echo "no build type specified, defaulting to release"
    complie_flag="-DCMAKE_BUILD_TYPE=Release"
fi

use_make() {
    if command -v make >/dev/null 2>&1 ; then
        echo "using make to build"
        cmake .. $complie_flag
        make -j4
    else
        echo "neither make nor ninja found, exiting"
    fi
}

use_ninja() {
    if command -v ninja >/dev/null 2>&1 ; then
        echo "using ninja to build"
        cmake .. -G "Ninja" $complie_flag
        ninja -j4
    else
        echo "ninja not found, falling back to make"
        use_make
    fi
}

if [[ $2 = "make" ]]; then
    use_make
elif [[ $2 = "ninja" ]]; then
    use_ninja
else
    echo "no build system specified, defaulting to ninja"
    use_ninja
fi
