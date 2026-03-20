# ros2_xmake_examples

[![E2E](https://github.com/ros-x/ros2_xmake_examples/actions/workflows/e2e.yml/badge.svg)](https://github.com/ros-x/ros2_xmake_examples/actions/workflows/e2e.yml)

Example ROS 2 packages demonstrating [xmake](https://xmake.io/) as a build system via [ament_xmake](https://github.com/ros-x/ament_xmake) and [colcon-xmake](https://github.com/ros-x/colcon-xmake). Part of the [ros-x](https://github.com/ros-x) organization.

## Packages

### Level 1: Beginner — ROS 2 Basics

| Package | Description |
|---------|-------------|
| `demo_xmake_pubsub` | Publisher/Subscriber with Topic communication |
| `demo_xmake_service` | Service Client/Server with `example_interfaces` |
| `demo_xmake_timer` | Timers and ROS 2 logging system |

### Level 2: Intermediate — Core Features

| Package | Description |
|---------|-------------|
| `demo_xmake_cpp` | Library build (static/shared) with `add_ros_deps()` and turtlesim |
| `demo_xmake_params` | Parameter system with YAML config and launch files |
| `demo_xmake_interfaces` | Custom msg/srv/action definitions (rosidl) |
| `demo_xmake_component` | Composable node plugin with `install_ros_plugin()` |
| `demo_xmake_action` | Action server/client using custom Countdown action |
| `demo_xmake_lifecycle` | Lifecycle (managed) node with state machine |
| `demo_xmake_launch` | Launch file and config installation |
| `demo_xmake_mixed` | Mixed C++/Python package |
| `demo_xmake_qos` | QoS profiles: reliable, best-effort, sensor data |
| `demo_xmake_testing` | gtest integration with `colcon test` |
| `downstream_cmake_consumer` | CMake interop — consuming xmake package exports |
| `demo_xmake_custom_msg_node` | Node publishing custom messages from `demo_xmake_interfaces` |

### Level 3: Advanced — Robotics Ecosystem

| Package | Description |
|---------|-------------|
| `demo_xmake_tf2` | TF2 coordinate transforms (static + dynamic) |
| `demo_xmake_robot_description` | URDF robot model with robot_state_publisher |
| `demo_xmake_rviz_markers` | RViz Marker/MarkerArray visualization |
| `demo_xmake_executor` | Executors and CallbackGroups (single/multi-threaded) |
| `demo_xmake_image` | Image processing with cv_bridge and OpenCV |
| `demo_xmake_rosbag` | Programmatic rosbag2 recording/playback |
| `demo_xmake_nav_plugin` | Nav2 costmap layer plugin |
| `demo_xmake_hardware` | ros2_control hardware interface plugin |
| `demo_xmake_dds_config` | DDS/RMW configuration (FastDDS/CycloneDDS) |

### Level 4: Debugging & Performance

| Package | Description |
|---------|-------------|
| `demo_xmake_logging` | Advanced logging and diagnostics with `diagnostic_updater` |
| `demo_xmake_tracing` | Performance tracing with multi-stage pipeline |
| `demo_xmake_benchmark` | Communication latency and throughput benchmarks |

> **Documentation**: See [docs/README.md](docs/README.md) for the full tutorial index (in Chinese) with both ROS 2 learning path and xmake feature views.

## Setup

### Prerequisites

- ROS 2 Jazzy (or later) installed
- [xmake](https://xmake.io/) installed

> **Tip:** You can use [xlings](https://github.com/d2learn/xlings) to quickly set up xmake and other dev tools:
> ```bash
> curl -fsSL https://raw.githubusercontent.com/d2learn/xlings/refs/heads/main/tools/other/quick_install.sh | bash
> xlings install xmake
> ```

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
