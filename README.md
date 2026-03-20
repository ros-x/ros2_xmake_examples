# ros2_xmake_examples

[![E2E](https://github.com/ros-x/ros2_xmake_examples/actions/workflows/e2e.yml/badge.svg)](https://github.com/ros-x/ros2_xmake_examples/actions/workflows/e2e.yml)

使用 [xmake](https://xmake.io/) 作为构建系统的 ROS 2 示例包集合，基于 [ament_xmake](https://github.com/ros-x/ament_xmake) 和 [colcon-xmake](https://github.com/ros-x/colcon-xmake)。隶属于 [ros-x](https://github.com/ros-x) 组织。

## 快速开始

### 环境要求

- ROS 2 Jazzy（或更高版本）
- [xmake](https://xmake.io/)

> **提示：** 可以使用 [xlings](https://github.com/d2learn/xlings) 快速安装 xmake 和其他开发工具：
> ```bash
> curl -fsSL https://raw.githubusercontent.com/d2learn/xlings/refs/heads/main/tools/other/quick_install.sh | bash
> xlings install xmake
> ```

### 克隆与安装依赖

```bash
# 克隆本仓库
git clone https://github.com/ros-x/ros2_xmake_examples.git
cd ros2_xmake_examples

# 安装 colcon-xmake
pip install git+https://github.com/ros-x/colcon-xmake.git

# 导入工作空间依赖（ament_xmake）
vcs import < ros2_xmake.repos
```

### 构建

```bash
source /opt/ros/jazzy/setup.bash
colcon build --event-handlers console_direct+
source install/setup.bash
```

### 运行测试

```bash
colcon test --packages-select demo_xmake_cpp downstream_cmake_consumer --event-handlers console_direct+
colcon test-result --verbose
```

### Turtlesim 画圆演示

分别在两个终端中运行：

```bash
ros2 run turtlesim turtlesim_node
```

```bash
ros2 run demo_xmake_cpp turtle_circle_node
```

## 示例包

### 第一级：入门 — ROS 2 基础

| 包名 | 说明 |
|------|------|
| [`demo_xmake_pubsub`](demo_xmake_pubsub) | 发布者/订阅者与 Topic 通信 |
| [`demo_xmake_service`](demo_xmake_service) | 服务客户端/服务端，使用 `example_interfaces` |
| [`demo_xmake_timer`](demo_xmake_timer) | 定时器与 ROS 2 日志系统 |

### 第二级：进阶 — 核心功能

| 包名 | 说明 |
|------|------|
| [`demo_xmake_cpp`](demo_xmake_cpp) | 库构建（静态/动态），使用 `add_ros_deps()` 和 turtlesim |
| [`demo_xmake_params`](demo_xmake_params) | 参数系统，支持 YAML 配置和 launch 文件 |
| [`demo_xmake_interfaces`](demo_xmake_interfaces) | 自定义 msg/srv/action 定义（rosidl） |
| [`demo_xmake_component`](demo_xmake_component) | 可组合节点插件，使用 `install_ros_plugin()` |
| [`demo_xmake_action`](demo_xmake_action) | Action 服务端/客户端，使用自定义 Countdown action |
| [`demo_xmake_lifecycle`](demo_xmake_lifecycle) | 生命周期（托管）节点与状态机 |
| [`demo_xmake_launch`](demo_xmake_launch) | Launch 文件与配置安装 |
| [`demo_xmake_mixed`](demo_xmake_mixed) | C++/Python 混合包 |
| [`demo_xmake_qos`](demo_xmake_qos) | QoS 策略：可靠传输、尽力传输、传感器数据 |
| [`demo_xmake_testing`](demo_xmake_testing) | gtest 集成与 `colcon test` |
| [`downstream_cmake_consumer`](downstream_cmake_consumer) | CMake 互操作 — 消费 xmake 包的导出 |
| [`demo_xmake_custom_msg_node`](demo_xmake_custom_msg_node) | 发布来自 [`demo_xmake_interfaces`](demo_xmake_interfaces) 的自定义消息的节点 |

### 第三级：高级 — 机器人生态

| 包名 | 说明 |
|------|------|
| [`demo_xmake_tf2`](demo_xmake_tf2) | TF2 坐标变换（静态 + 动态） |
| [`demo_xmake_robot_description`](demo_xmake_robot_description) | URDF 机器人模型与 robot_state_publisher |
| [`demo_xmake_rviz_markers`](demo_xmake_rviz_markers) | RViz Marker/MarkerArray 可视化 |
| [`demo_xmake_executor`](demo_xmake_executor) | 执行器与回调组（单线程/多线程） |
| [`demo_xmake_image`](demo_xmake_image) | 图像处理，使用 cv_bridge 和 OpenCV |
| [`demo_xmake_rosbag`](demo_xmake_rosbag) | 编程式 rosbag2 录制/回放 |
| [`demo_xmake_nav_plugin`](demo_xmake_nav_plugin) | Nav2 代价地图层插件 |
| [`demo_xmake_hardware`](demo_xmake_hardware) | ros2_control 硬件接口插件 |
| [`demo_xmake_dds_config`](demo_xmake_dds_config) | DDS/RMW 配置（FastDDS/CycloneDDS） |

### 第四级：调试与性能

| 包名 | 说明 |
|------|------|
| [`demo_xmake_logging`](demo_xmake_logging) | 高级日志与诊断，使用 `diagnostic_updater` |
| [`demo_xmake_tracing`](demo_xmake_tracing) | 多级流水线性能追踪 |
| [`demo_xmake_benchmark`](demo_xmake_benchmark) | 通信延迟与吞吐量基准测试 |

> **教程文档**：完整的教程索引（中文）见 [docs/README.md](docs/README.md)，包含 ROS 2 学习路径和 xmake 功能两种视角。

## 相关项目

- [ament_xmake](https://github.com/ros-x/ament_xmake) — 让 ament 支持 xmake 包的 CMake 宏
- [colcon-xmake](https://github.com/ros-x/colcon-xmake) — xmake 构建系统的 colcon 插件
- [ros-x 组织](https://github.com/ros-x)
