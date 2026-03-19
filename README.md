# ros2_xmake_examples

[![E2E](https://github.com/ros-x/ros2_xmake_examples/actions/workflows/e2e.yml/badge.svg)](https://github.com/ros-x/ros2_xmake_examples/actions/workflows/e2e.yml)

Example ROS 2 packages demonstrating [xmake](https://xmake.io/) as a build system via [ament_xmake](https://github.com/ros-x/ament_xmake) and [colcon-xmake](https://github.com/ros-x/colcon-xmake). Part of the [ros-x](https://github.com/ros-x) organization.

## Packages

| Package | Description |
|---------|-------------|
| `demo_xmake_cpp` | Core example using `build_type=ament_xmake` with `add_ros_deps()` for ROS package-oriented linking |
| `demo_xmake_interfaces` | Custom message/service definitions built with xmake (rosidl) |
| `demo_xmake_custom_msg_node` | Node that publishes custom messages from `demo_xmake_interfaces` |
| `demo_xmake_component` | Composable node (rclcpp_components plugin) built with xmake |
| `demo_xmake_launch` | Launch file integration example |
| `demo_xmake_mixed` | Mixed C++/Python package built with xmake |
| `downstream_cmake_consumer` | Standard `ament_cmake` package that consumes exports from `demo_xmake_cpp` |

## Setup

### Prerequisites

- ROS 2 Jazzy (or later) installed
- [xmake](https://xmake.io/) installed

### Clone and install dependencies

```bash
# Clone this repo
git clone https://github.com/ros-x/ros2_xmake_examples.git
cd ros2_xmake_examples

# Install colcon-xmake
pip install git+https://github.com/ros-x/colcon-xmake.git

# Import workspace dependencies (ament_xmake)
vcs import < ros2_xmake.repos
```

### Build

```bash
source /opt/ros/jazzy/setup.bash
colcon build --event-handlers console_direct+
source install/setup.bash
```

### Run tests

```bash
colcon test --packages-select demo_xmake_cpp downstream_cmake_consumer --event-handlers console_direct+
colcon test-result --verbose
```

### Turtlesim circle demo

In two terminals:

```bash
ros2 run turtlesim turtlesim_node
```

```bash
ros2 run demo_xmake_cpp turtle_circle_node
```

## See also

- [ament_xmake](https://github.com/ros-x/ament_xmake) -- CMake macros that teach ament about xmake packages
- [colcon-xmake](https://github.com/ros-x/colcon-xmake) -- colcon plugin for xmake-based packages
- [ros-x organization](https://github.com/ros-x)
