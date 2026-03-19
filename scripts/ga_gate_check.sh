#!/usr/bin/env bash
set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
ROS_DISTRO="${ROS_DISTRO:-jazzy}"

export PATH="/usr/bin:/bin:/usr/local/bin:$PATH"
export CC="${CC:-/usr/bin/gcc}"
export CXX="${CXX:-/usr/bin/g++}"
export AR="${AR:-/usr/bin/ar}"
export RANLIB="${RANLIB:-/usr/bin/ranlib}"

echo "== ga gate: unit tests (colcon-xmake) =="
python3 -m pytest -q --pyargs colcon_xmake

echo "== ga gate: negative checks =="
ROS_DISTRO="$ROS_DISTRO" "$ROOT/scripts/e2e_negative_checks.sh"

echo "== ga gate: clean e2e build =="
ROS_DISTRO="$ROS_DISTRO" "$ROOT/scripts/e2e_clean_build.sh"

echo "== ga gate: runtime smoke =="
ROS_DISTRO="$ROS_DISTRO" "$ROOT/scripts/runtime_smoke.sh"

echo "GA gate checks passed"
