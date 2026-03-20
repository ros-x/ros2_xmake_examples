# 24. 高级日志与诊断 — 系统健康监控

## 概述

本示例演示 ROS 2 的日志系统高级用法和诊断框架。通过 `demo_xmake_logging` 包，你将学习如何在运行时切换日志级别、使用 `diagnostic_updater` 发布系统健康状态，以及利用 `ros2 doctor` 进行系统检查。

**ROS 2 知识点**：ros2 doctor、日志级别运行时切换、diagnostic_updater、自定义诊断

**xmake 知识点**：`diagnostic_updater` 依赖解析

## ROS 2 知识

### 日志系统架构

ROS 2 的日志系统基于 spdlog，支持 5 个级别：

```
DEBUG < INFO < WARN < ERROR < FATAL
```

日志级别可以在多个层面控制：

- **启动时**：`--ros-args --log-level debug`
- **运行时**：通过 Service `/<node>/set_logger_level` 动态切换
- **配置文件**：`--ros-args --log-config-file <path>`

### diagnostic_updater 框架

`diagnostic_updater` 提供了一种标准化的方式来报告硬件和软件状态：

```
┌──────────────┐    /diagnostics    ┌──────────────────┐
│  Your Node   │ ─────────────────> │ diagnostic_agg   │
│  (Updater)   │                    │ (聚合/监控)       │
└──────────────┘                    └──────────────────┘
       │                                     │
  DiagnosticTask                    rqt_robot_monitor
  - 硬件温度                         (GUI 查看)
  - 通信状态
  - 资源使用率
```

每个 `DiagnosticTask` 报告一个诊断状态，包括：
- **OK**：正常
- **WARN**：需要注意
- **ERROR**：需要处理
- **STALE**：数据过期

### ros2 doctor

`ros2 doctor` 是系统级健康检查工具，可以检测：

- ROS 2 环境配置
- 网络连通性
- DDS 配置
- 话题/服务连接状态

## 示例代码

### 项目结构

```
demo_xmake_logging/
├── xmake.lua          # 构建配置
├── package.xml        # deps: rclcpp, diagnostic_updater
└── src/
    └── diagnostics_node.cpp  # 诊断发布 + 日志级别演示
```

### xmake.lua 构建配置

```lua
set_project("demo_xmake_logging")
set_version("0.1.0")
set_languages("cxx17")

add_rules("mode.debug", "mode.release")
add_rules("ament_xmake.package")

target("diagnostics_node")
    set_kind("binary")
    set_installdir("lib/demo_xmake_logging")
    add_files("src/diagnostics_node.cpp")
    add_ros_deps("rclcpp", "diagnostic_updater")
```

`diagnostic_updater` 是一个标准 ROS 2 包，通过 `add_ros_deps()` 即可引入，xmake 会自动解析其头文件路径和链接库。

### 源码要点

**诊断任务注册**：

```cpp
// 创建 Updater（自动发布到 /diagnostics）
diagnostic_updater::Updater updater(this);
updater.setHardwareID("demo_robot");

// 添加诊断任务
updater.add("Hardware Status", [this](diagnostic_updater::DiagnosticStatusWrapper & stat) {
    if (cpu_temp_ < 60.0) {
        stat.summary(diagnostic_msgs::msg::DiagnosticStatus::OK, "Temperature normal");
    } else if (cpu_temp_ < 75.0) {
        stat.summary(diagnostic_msgs::msg::DiagnosticStatus::WARN, "Temperature elevated");
    } else {
        stat.summary(diagnostic_msgs::msg::DiagnosticStatus::ERROR, "Overheating!");
    }
    stat.add("CPU Temperature", cpu_temp_);
    stat.add("Uptime (s)", uptime_seconds_);
});
```

**运行时日志级别切换**：

```cpp
// 程序化切换日志级别
auto logger = this->get_logger();
auto ret = rcutils_logging_set_logger_level(
    logger.get_name(), RCUTILS_LOG_SEVERITY_DEBUG);
```

## 构建与运行

```bash
# 构建
colcon build --packages-select demo_xmake_logging
source install/setup.bash

# 运行诊断节点
ros2 run demo_xmake_logging diagnostics_node

# 在另一个终端，指定日志级别启动
ros2 run demo_xmake_logging diagnostics_node --ros-args --log-level debug
```

## 验证

```bash
# 查看诊断话题
ros2 topic echo /diagnostics

# 系统健康检查
ros2 doctor

# 运行时切换日志级别（通过 service）
ros2 service call /diagnostics_node/set_logger_level \
  rcl_interfaces/srv/SetLoggerLevel \
  "{logger_name: 'diagnostics_node', level: 10}"
```

## xmake 知识点小结

| 特性 | 说明 |
|------|------|
| `add_ros_deps("diagnostic_updater")` | 引入诊断框架依赖，自动解析头文件和链接库 |
| 单 target 简单包 | 一个 binary target 的最小包结构 |

## 内容扩展

> 以下资源可帮助你深入了解本章涉及的技术领域。

### 开源项目
- [diagnostic_updater](https://github.com/ros/diagnostics) — ROS 2 诊断框架核心实现

### 工具
- [rqt_runtime_monitor](https://docs.ros.org/en/jazzy/p/rqt_runtime_monitor/) — 诊断信息实时监控 GUI
- [rqt_robot_monitor](https://docs.ros.org/en/jazzy/p/rqt_robot_monitor/) — 机器人整体状态概览 GUI

### 文档与教程
- [diagnostic_msgs](https://docs.ros.org/en/jazzy/p/diagnostic_msgs/) — 诊断消息类型定义
- [ROS 2 日志系统概念](https://docs.ros.org/en/jazzy/Concepts/Intermediate/About-Logging.html) — 官方日志架构说明

## 参考

- [ROS 2 Logging](https://docs.ros.org/en/jazzy/Concepts/Intermediate/About-Logging.html)
- [diagnostic_updater API](https://docs.ros.org/en/jazzy/p/diagnostic_updater/)
- [ros2 doctor](https://docs.ros.org/en/jazzy/Tutorials/Beginner-Client-Libraries/Getting-Started-With-Ros2doctor.html)
- [ament_xmake](https://github.com/ros-x/ament_xmake)
