#!/usr/bin/env bash
set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
ROS_DISTRO="${ROS_DISTRO:-jazzy}"
ROS_SETUP="/opt/ros/${ROS_DISTRO}/setup.bash"

export AMENT_TRACE_SETUP_FILES="${AMENT_TRACE_SETUP_FILES-}"
set +u
if [[ ! -f "$ROS_SETUP" ]]; then
  echo "ROS setup file not found: $ROS_SETUP"
  exit 1
fi
source "$ROS_SETUP"
set -u

pip install --break-system-packages git+https://github.com/ros-x/colcon-xmake.git

TMPDIR="$(mktemp -d)"
trap 'rm -rf "$TMPDIR"' EXIT

cat > "$TMPDIR/xmake.lua" <<'LUA'
set_project("tmp_xmake_pkg")
target("tmp_xmake_pkg")
    set_kind("binary")
LUA

# Negative check 1: xmake not found
set +e
PATH="/tmp/ros2_xmake_empty_path" colcon build --base-paths "$TMPDIR" --event-handlers console_direct+
RC1=$?
set -e
if [[ "$RC1" -eq 0 ]]; then
  echo "Expected failure when xmake is missing"
  exit 1
fi

# Negative check 2: invalid timeout
set +e
colcon build --base-paths "$TMPDIR" --xmake-timeout 0 --event-handlers console_direct+
RC2=$?
set -e
if [[ "$RC2" -eq 0 ]]; then
  echo "Expected failure for invalid timeout"
  exit 1
fi

echo "Negative checks passed"
