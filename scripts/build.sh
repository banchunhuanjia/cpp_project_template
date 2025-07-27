set -ex

PROJECT_ROOT="$(dirname "$(readlink -f "$0")")/.."
mkdir -p "$PROJECT_ROOT/build"
cd "$PROJECT_ROOT/build"

cmake ..
make -j$(nproc)