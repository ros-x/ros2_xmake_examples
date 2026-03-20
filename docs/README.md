# ros2_xmake_examples 文档

本文档集提供两套索引，同一套示例可从不同角度检索。

## ROS 2 学习路线

### 通信基础

| 知识点 | 示例 | 文档 |
|--------|------|------|
| Topic 发布/订阅 | [demo_xmake_pubsub](../demo_xmake_pubsub) | [01_pubsub](01_pubsub.md) |
| Service 请求/响应 | [demo_xmake_service](../demo_xmake_service) | [02_service](02_service.md) |
| Action 目标/反馈/结果 | [demo_xmake_action](../demo_xmake_action) | [08_action](08_action.md) |
| QoS 服务质量配置 | [demo_xmake_qos](../demo_xmake_qos) | [12_qos](12_qos.md) |

### 节点与执行

| 知识点 | 示例 | 文档 |
|--------|------|------|
| 定时器与日志 | [demo_xmake_timer](../demo_xmake_timer) | [03_timer_logging](03_timer_logging.md) |
| 参数系统 | [demo_xmake_params](../demo_xmake_params) | [05_params](05_params.md) |
| 生命周期节点 | [demo_xmake_lifecycle](../demo_xmake_lifecycle) | [09_lifecycle](09_lifecycle.md) |
| 执行器与多线程 | [demo_xmake_executor](../demo_xmake_executor) | [18_executor](18_executor.md) |
| 组件与插件 | [demo_xmake_component](../demo_xmake_component) | [07_component](07_component.md) |

### 接口与数据

| 知识点 | 示例 | 文档 |
|--------|------|------|
| 自定义 msg/srv/action | [demo_xmake_interfaces](../demo_xmake_interfaces) | [06_interfaces](06_interfaces.md) |
| 图像处理管线 | [demo_xmake_image](../demo_xmake_image) | [19_image](19_image.md) |
| 数据录制回放 | [demo_xmake_rosbag](../demo_xmake_rosbag) | [20_rosbag](20_rosbag.md) |

### 机器人系统

| 知识点 | 示例 | 文档 |
|--------|------|------|
| 坐标变换 TF2 | [demo_xmake_tf2](../demo_xmake_tf2) | [15_tf2](15_tf2.md) |
| URDF 机器人模型 | [demo_xmake_robot_description](../demo_xmake_robot_description) | [16_robot_description](16_robot_description.md) |
| RViz 可视化 | [demo_xmake_rviz_markers](../demo_xmake_rviz_markers) | [17_rviz_markers](17_rviz_markers.md) |
| 导航插件 | [demo_xmake_nav_plugin](../demo_xmake_nav_plugin) | [21_nav_plugin](21_nav_plugin.md) |
| 硬件驱动接口 | [demo_xmake_hardware](../demo_xmake_hardware) | [22_hardware](22_hardware.md) |

### 系统集成

| 知识点 | 示例 | 文档 |
|--------|------|------|
| Launch 多节点编排 | [demo_xmake_launch](../demo_xmake_launch) | [10_launch](10_launch.md) |
| C++/Python 混合 | [demo_xmake_mixed](../demo_xmake_mixed) | [11_mixed](11_mixed.md) |
| 库构建与下游消费 | [demo_xmake_cpp](../demo_xmake_cpp) | [04_library](04_library.md) |
| 测试框架 | [demo_xmake_testing](../demo_xmake_testing) | [13_testing](13_testing.md) |
| CMake 互操作 | [downstream_cmake_consumer](../downstream_cmake_consumer) | [14_cmake_interop](14_cmake_interop.md) |
| DDS/RMW 配置 | [demo_xmake_dds_config](../demo_xmake_dds_config) | [23_dds_config](23_dds_config.md) |

### 调试与性能优化

| 知识点 | 示例 | 文档 |
|--------|------|------|
| 高级日志与诊断 | [demo_xmake_logging](../demo_xmake_logging) | [24_logging_diagnostics](24_logging_diagnostics.md) |
| 性能追踪 | [demo_xmake_tracing](../demo_xmake_tracing) | [25_tracing](25_tracing.md) |
| 内存与 CPU 分析 | （纯文档） | [26_profiling](26_profiling.md) |
| 通信基准测试 | [demo_xmake_benchmark](../demo_xmake_benchmark) | [27_benchmark](27_benchmark.md) |

---

## xmake 构建特性索引

### 基础构建

| xmake 特性 | 首次出现 | 文档 |
|------------|---------|------|
| `set_kind("binary")` 构建可执行文件 | [demo_xmake_pubsub](../demo_xmake_pubsub) | [01](01_pubsub.md) |
| `add_ros_deps()` ROS 依赖解析 | [demo_xmake_pubsub](../demo_xmake_pubsub) | [01](01_pubsub.md) |
| 同包多 target 定义 | [demo_xmake_pubsub](../demo_xmake_pubsub) | [01](01_pubsub.md) |
| `set_kind("static")` 静态库 | [demo_xmake_cpp](../demo_xmake_cpp) | [04](04_library.md) |
| `set_kind("shared")` 共享库 | [demo_xmake_cpp](../demo_xmake_cpp) | [04](04_library.md) |
| `add_headerfiles()` 头文件导出 | [demo_xmake_cpp](../demo_xmake_cpp) | [04](04_library.md) |
| `{visibility = "public"}` 公开依赖 | [demo_xmake_cpp](../demo_xmake_cpp) | [04](04_library.md) |

### 数据文件安装

| xmake 特性 | 首次出现 | 文档 |
|------------|---------|------|
| 约定目录自动安装 (launch/, config/) | [demo_xmake_params](../demo_xmake_params) | [05](05_params.md) |
| `install_ros_data()` 显式安装 | [demo_xmake_launch](../demo_xmake_launch) | [10](10_launch.md) |
| urdf/meshes 自动安装 | [demo_xmake_robot_description](../demo_xmake_robot_description) | [16](16_robot_description.md) |

### 高级特性

| xmake 特性 | 首次出现 | 文档 |
|------------|---------|------|
| rosidl 集成 (msg/srv/action) | [demo_xmake_interfaces](../demo_xmake_interfaces) | [06](06_interfaces.md) |
| `install_ros_plugin()` 插件注册 | [demo_xmake_component](../demo_xmake_component) | [07](07_component.md) |
| CMake Config 自动生成 | [downstream_cmake_consumer](../downstream_cmake_consumer) | [14](14_cmake_interop.md) |
| xmake + setup.py 混合构建 | [demo_xmake_mixed](../demo_xmake_mixed) | [11](11_mixed.md) |
| xmake test target | [demo_xmake_testing](../demo_xmake_testing) | [13](13_testing.md) |
| phony target (纯数据包) | [demo_xmake_dds_config](../demo_xmake_dds_config) | [23](23_dds_config.md) |
| 系统库 + ROS 包混合依赖 | [demo_xmake_image](../demo_xmake_image) | [19](19_image.md) |
| `add_cxflags()` 编译器标志 | （纯文档） | [26](26_profiling.md) |
| `diagnostic_updater` 依赖 | [demo_xmake_logging](../demo_xmake_logging) | [24](24_logging_diagnostics.md) |
| launch/ 自动安装 + 可选依赖 | [demo_xmake_tracing](../demo_xmake_tracing) | [25](25_tracing.md) |
| 同包多 target + 条件编译 | [demo_xmake_benchmark](../demo_xmake_benchmark) | [27](27_benchmark.md) |
