#!/usr/bin/env bash
set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
ROS_DISTRO="${ROS_DISTRO:-jazzy}"
ROS_SETUP="/opt/ros/${ROS_DISTRO}/setup.bash"

export PATH="/usr/bin:/bin:/usr/local/bin:$PATH"
export CC="${CC:-/usr/bin/gcc}"
export CXX="${CXX:-/usr/bin/g++}"
export AR="${AR:-/usr/bin/ar}"
export RANLIB="${RANLIB:-/usr/bin/ranlib}"

if [[ ! -f "$ROS_SETUP" ]]; then
  echo "ROS setup file not found: $ROS_SETUP"
  exit 1
fi

set +u
source "$ROS_SETUP"
source "$ROOT/install/setup.bash"
set -u

echo "== runtime smoke: demo_xmake_cpp_node output =="
NODE_OUT="$(ros2 run demo_xmake_cpp demo_xmake_cpp_node)"
echo "$NODE_OUT"
if [[ "$NODE_OUT" != *"demo_xmake_cpp result=42"* ]]; then
  echo "Unexpected output from demo_xmake_cpp_node"
  exit 1
fi

echo "== runtime smoke: turtle_circle_node startup =="
set +e
timeout 3s ros2 run demo_xmake_cpp turtle_circle_node >/tmp/ros2_xmake_turtle_circle.log 2>&1
RC=$?
set -e
if [[ "$RC" -ne 124 ]]; then
  echo "turtle_circle_node did not stay alive for smoke interval (rc=$RC)"
  cat /tmp/ros2_xmake_turtle_circle.log
  exit 1
fi
if ! grep -q "Publishing circle Twist on /turtle1/cmd_vel" /tmp/ros2_xmake_turtle_circle.log; then
  echo "turtle_circle_node did not emit expected startup log"
  cat /tmp/ros2_xmake_turtle_circle.log
  exit 1
fi

echo "== runtime smoke: downstream_cmake_consumer =="
CONSUMER_OUT="$($ROOT/install/downstream_cmake_consumer/lib/downstream_cmake_consumer/consumer)"
echo "$CONSUMER_OUT"
if [[ "$CONSUMER_OUT" != *"add_two"* ]]; then
  echo "Unexpected output from downstream_cmake_consumer"
  exit 1
fi

echo "== runtime smoke: ros2 launch (demo_xmake_launch) =="
set +e
LAUNCH_OUT="$(timeout 5s ros2 launch demo_xmake_launch demo.launch.py 2>&1)"
set -e
echo "$LAUNCH_OUT"
if [[ "$LAUNCH_OUT" != *"launch file working"* ]]; then
  echo "Launch file did not produce expected output"
  exit 1
fi

echo "== runtime smoke: custom msg publisher (rosidl) =="
set +e
CUSTOM_OUT="$(timeout 4s ros2 run demo_xmake_custom_msg_node custom_msg_publisher 2>&1)"
set -e
echo "$CUSTOM_OUT"
if [[ "$CUSTOM_OUT" != *"hello_xmake"* ]]; then
  echo "Custom msg publisher did not produce expected output"
  exit 1
fi

echo "== runtime smoke: shared library check =="
if [[ ! -f "$ROOT/install/demo_xmake_cpp/lib/libdemo_xmake_cpp.so" ]]; then
  echo "Shared library not found"
  exit 1
fi
if [[ ! -f "$ROOT/install/demo_xmake_cpp/lib/libdemo_xmake_cpp.a" ]]; then
  echo "Static library not found"
  exit 1
fi

echo "== runtime smoke: rosidl libraries =="
ROSIDL_COUNT=$(ls "$ROOT/install/demo_xmake_interfaces/lib"/libdemo_xmake_interfaces__*.so 2>/dev/null | wc -l)
if [[ "$ROSIDL_COUNT" -ne 7 ]]; then
  echo "Expected 7 rosidl libraries, found $ROSIDL_COUNT"
  exit 1
fi

echo "== runtime smoke: component plugin registration =="
if [[ ! -f "$ROOT/install/demo_xmake_component/share/demo_xmake_component/plugin_description.xml" ]]; then
  echo "Plugin description XML not installed"
  exit 1
fi
if [[ ! -f "$ROOT/install/demo_xmake_component/share/ament_index/resource_index/rclcpp_components__pluginlib__plugin/demo_xmake_component" ]]; then
  echo "Pluginlib index not registered"
  exit 1
fi

echo "== runtime smoke: mixed package C++ node =="
set +e
MIXED_OUT="$(timeout 3s ros2 run demo_xmake_mixed cpp_publisher 2>&1)"
set -e
echo "$MIXED_OUT"
if [[ "$MIXED_OUT" != *"C++ publisher started"* ]]; then
  echo "Mixed package C++ publisher did not start"
  exit 1
fi

echo "Runtime smoke checks passed (all phases)"
