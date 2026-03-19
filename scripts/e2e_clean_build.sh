#!/usr/bin/env bash
set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
ROS_DISTRO="${ROS_DISTRO:-jazzy}"
ROS_SETUP="/opt/ros/${ROS_DISTRO}/setup.bash"

# Keep regression deterministic on hosts with toolchain shims.
export PATH="/usr/bin:/bin:/usr/local/bin:$PATH"
export CC="${CC:-/usr/bin/gcc}"
export CXX="${CXX:-/usr/bin/g++}"
export AR="${AR:-/usr/bin/ar}"
export RANLIB="${RANLIB:-/usr/bin/ranlib}"

export AMENT_TRACE_SETUP_FILES="${AMENT_TRACE_SETUP_FILES-}"
set +u
if [[ ! -f "$ROS_SETUP" ]]; then
  echo "ROS setup file not found: $ROS_SETUP"
  exit 1
fi
source "$ROS_SETUP"
set -u

pip install --break-system-packages git+https://github.com/ros-x/colcon-xmake.git

# Clone ament_xmake if not present
if [[ ! -d "$ROOT/ament_xmake" ]]; then
  git clone https://github.com/ros-x/ament_xmake.git "$ROOT/ament_xmake"
fi

rm -rf "$ROOT/build" "$ROOT/install" "$ROOT/log"

cd "$ROOT"
colcon build --event-handlers console_direct+
colcon test --packages-select demo_xmake_cpp downstream_cmake_consumer --event-handlers console_direct+
colcon test-result --verbose
