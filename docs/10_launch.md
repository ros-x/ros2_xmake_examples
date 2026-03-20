---
sidebar_position: 10
title: "10. Launch 系统 — 多节点编排与参数加载"
---
# 10. Launch 系统 — 多节点编排与参数加载

## 概述

本示例通过 [`demo_xmake_launch`](../demo_xmake_launch) 包，演示 ROS 2 的 **Launch** 系统：多节点编排、参数文件加载和条件启动。

**ROS 2 知识点**：LaunchDescription、Node action、参数加载、namespace

**xmake 知识点**：`install_ros_data()` 的显式用法 vs 约定目录

## ROS 2 知识

### Launch 系统

Launch 文件用 Python 编写，核心概念：

```python
from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    return LaunchDescription([
        Node(
            package='pkg_name',
            executable='node_name',
            parameters=[config_file],
            output='screen',
        ),
    ])
```

### Launch 文件 vs 命令行

| 方式 | 适用场景 |
|------|---------|
| `ros2 run` | 单节点调试 |
| `ros2 launch` | 多节点编排、参数加载 |

## 示例代码

### xmake.lua 构建配置

```lua
-- 显式安装 launch 和 config 文件
install_ros_data("launch", "launch/*.py")
install_ros_data("config", "config/*.yaml")

target("demo_xmake_launch_marker")
    set_kind("phony")
```

**`install_ros_data()` vs 约定目录**：

| 方式 | 用法 | 场景 |
|------|------|------|
| 约定目录 | 无需配置 | `launch/`、`config/` 等标准目录 |
| `install_ros_data()` | 显式调用 | 自定义目录或文件选择 |

两种方式效果相同，约定目录更简洁。[`demo_xmake_launch`](../demo_xmake_launch) 使用显式方式作为 API 演示。

## 构建与运行

```bash
colcon build --packages-select demo_xmake_launch
source install/setup.bash

ros2 launch demo_xmake_launch demo.launch.py
```

## 验证

```bash
# 验证 launch 文件已安装
ls install/demo_xmake_launch/share/demo_xmake_launch/launch/

# 验证 config 文件已安装
ls install/demo_xmake_launch/share/demo_xmake_launch/config/
```

## xmake 知识点小结

| 特性 | 说明 |
|------|------|
| `install_ros_data()` | 显式安装数据文件到 `share/<pkg>/` |
| phony target | 纯数据包需要至少一个 target |

## 内容扩展

> 以下资源可帮助你深入了解本章涉及的技术领域。

### 开源项目
- [launch](https://github.com/ros2/launch) — ROS 2 Launch 核心
- [launch_testing](https://github.com/ros2/launch/tree/rolling/launch_testing) — 集成测试框架

### 工具
- [ros2 launch --show-all-subprocesses-output](https://docs.ros.org/en/jazzy/Tutorials/Intermediate/Launch/Launch-Main.html) — 显示所有子进程输出

### 文档与教程
- [Launch 架构设计](https://design.ros2.org/articles/roslaunch.html)

## 参考

- [ROS 2 官方文档 - Launch tutorials](https://docs.ros.org/en/jazzy/Tutorials/Intermediate/Launch/Launch-Main.html)
