CMAKE_BUILD_DIR="./cmake-build/"

if test -f /proc/cpuinfo; then
  NUM_CPUS="$(grep siblings /proc/cpuinfo | head -n1 | cut -d':' -f2 | cut -d' ' -f2)"
else
  NUM_CPUS="$(sysctl -n hw.ncpu)"
fi

mkdir -p "$CMAKE_BUILD_DIR"

cd "$CMAKE_BUILD_DIR" || exit 1
cmake .. || exit 1
make -j "$NUM_CPUS" || exit 1
