if [ ! -d "build" ]; then
  echo "build directory does not exist, creating it"
  mkdir build
fi

echo "build directory exists, continuing"
cd build

if command -v ninja >/dev/null 2>&1 ; then
    echo "using ninja to build"
    cmake .. -G "Ninja"
    ninja -j4
elif command -v make >/dev/null 2>&1 ; then
    echo "using make to build"
    cmake ..
    make -j4
else
    echo "neither make nor ninja found, exiting"
fi