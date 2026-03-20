---
sidebar_position: 22
title: "22. 硬件接口 — ros2_control 驱动插件"
---
# 22. 硬件接口 — ros2_control 驱动插件

## 概述

本示例通过 [`demo_xmake_hardware`](../demo_xmake_hardware) 包，演示如何用 xmake 构建 **ros2_control** 硬件接口插件。

**ROS 2 知识点**：ros2_control 架构、SystemInterface、hardware_interface 插件

**xmake 知识点**：`install_ros_plugin()` + 硬件抽象层

## ROS 2 知识

### ros2_control 架构

```
Controller Manager
      │
      ├── Controllers (PID, Trajectory, etc.)
      │
      └── Hardware Interfaces
            ├── SystemInterface   (完整系统)
            ├── ActuatorInterface (单个执行器)
            └── SensorInterface   (单个传感器)
```

### 硬件接口生命周期

硬件接口是生命周期组件：

| 回调 | 用途 |
|------|------|
| `on_init` | 解析配置、分配资源 |
| `on_activate` | 连接硬件、启动通信 |
| `on_deactivate` | 停止通信 |
| `read` | 读取传感器数据 |
| `write` | 发送控制命令 |

## 示例代码

### xmake.lua 构建配置

```lua
install_ros_plugin("plugin_description.xml", "hardware_interface")

target("demo_xmake_hardware")
    set_kind("shared")
    add_files("src/mock_robot_hardware.cpp")
    add_ros_deps("rclcpp", "hardware_interface", "pluginlib", "rclcpp_lifecycle")
```

## 构建与运行

```bash
colcon build --packages-select demo_xmake_hardware
```

硬件接口通过 ros2_control 配置文件加载，非独立运行。

## 内容扩展

> 以下资源可帮助你深入了解本章涉及的技术领域。

### 开源项目
- [ros2_control](https://github.com/ros-controls/ros2_control) — 控制框架核心
- [ros2_controllers](https://github.com/ros-controls/ros2_controllers) — 标准控制器集
- [gz_ros2_control](https://github.com/ros-controls/gz_ros2_control) — Gazebo 仿真硬件接口

### 文档与教程
- [ros2_control 文档](https://control.ros.org/jazzy/)

### 论文与技术报告
- D. Štogl et al., "ros2_control: A platform-agnostic framework", pedestrian workshop, 2022

## 参考

- [ros2_control 文档](https://control.ros.org/jazzy/index.html)
- [编写硬件接口](https://control.ros.org/jazzy/doc/ros2_control/hardware_interface/doc/writing_new_hardware_component.html)
