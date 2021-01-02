CMAKE_BUILD_DIR="./cmake-build-debug/"

mkdir -p "$CMAKE_BUILD_DIR"

cd "$CMAKE_BUILD_DIR" || exit 1
cmake .. || exit 1
make || exit 1
