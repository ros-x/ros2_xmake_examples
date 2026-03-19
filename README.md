# ros2_xmake_examples

Example packages for validating ROS 2 + xmake integration.

- `demo_xmake_cpp`: package using `build_type=ament_xmake`.
  - demonstrates `add_ros_deps("rclcpp", "geometry_msgs")` for ROS package-oriented linking
- `downstream_cmake_consumer`: package using `ament_cmake` that consumes exported config from `demo_xmake_cpp`.

See scenario details in `docs/SCENARIOS.md`.
See release changes in `CHANGELOG.md`.

## Run turtlesim circle demo

Build:

```bash
source /opt/ros/jazzy/setup.bash
colcon build --packages-select demo_xmake_cpp
source install/setup.bash
```

Run in two terminals:

```bash
ros2 run turtlesim turtlesim_node
```

```bash
ros2 run demo_xmake_cpp turtle_circle_node
```
