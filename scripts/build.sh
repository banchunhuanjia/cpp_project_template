set -ex

SCRIPT_DIR=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" &> /dev/null && pwd)
PROJECT_ROOT="$SCRIPT_DIR/.."
mkdir -p "$PROJECT_ROOT/build"
cd "$PROJECT_ROOT/build"

cmake ..
make -j$(nproc)