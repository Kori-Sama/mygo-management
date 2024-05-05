#!/bin/bash

export MY_INSTALL_DIR=$HOME/.local
mkdir -p $MY_INSTALL_DIR
export PATH="$MY_INSTALL_DIR/bin:$PATH"

echo "-- installing build tools"
sudo apt install -y build-essential autoconf libtool pkg-config

echo "-- cloning grpc source code from github"
git clone --recurse-submodules -b v1.62.0 --depth 1 --shallow-submodules https://github.com/grpc/grpc

echo "-- download finished, begin to build"
cd grpc
mkdir -p cmake/build
pushd cmake/build
cmake -DgRPC_INSTALL=ON \
    -DgRPC_BUILD_TESTS=OFF \
    -DCMAKE_INSTALL_PREFIX=$MY_INSTALL_DIR \
    ../..
make -j 4
make install
popd

echo "-- build complete, begin to delete source directory"
cd ..
rm -rf grpc