CMAKE_BUILD_DIR="./cmake-build/"

if test -d "$CMAKE_BUILD_DIR"; then
  # CMake project
  rm -rf "$CMAKE_BUILD_DIR" || echo "Error cleaning!"; exit 1
else
  # Makefile project
  find . -name "*.o" -type f -delete
fi

rm raspi64
