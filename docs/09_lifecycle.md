# 09. 生命周期节点 — 状态机与受管节点

## 概述

本示例通过 [`demo_xmake_lifecycle`](../demo_xmake_lifecycle) 包，演示 ROS 2 的**生命周期节点**（Managed Node）。生命周期节点通过状态机控制节点的初始化、激活和清理过程。

**ROS 2 知识点**：LifecycleNode 状态机、Transition 回调、lifecycle_msgs

**xmake 知识点**：`rclcpp_lifecycle` 依赖解析

## ROS 2 知识

### 生命周期状态机

```
                  ┌─────────────┐
          ┌──────>│ Unconfigured │<──────┐
          │       └──────┬──────┘       │
          │          configure          │
          │       ┌──────▼──────┐       │
          │       │   Inactive   │   cleanup
          │       └──────┬──────┘       │
          │          activate           │
          │       ┌──────▼──────┐       │
      shutdown    │    Active    │───────┘
          │       └──────┬──────┘  deactivate
          │          shutdown
          │       ┌──────▼──────┐
          └──────>│  Finalized   │
                  └─────────────┘
```

每个转换都有对应的回调：`on_configure`、`on_activate`、`on_deactivate`、`on_cleanup`、`on_shutdown`。

### 使用场景

- 需要确定性的启动/关闭顺序
- 资源管理（如硬件连接）
- 系统编排（多节点协调启动）

## 示例代码

### xmake.lua 构建配置

```lua
target("lifecycle_talker")
    set_kind("binary")
    set_installdir("lib/demo_xmake_lifecycle")
    add_files("src/lifecycle_talker.cpp")
    add_ros_deps("rclcpp", "rclcpp_lifecycle", "std_msgs")
```

`rclcpp_lifecycle` 提供 `LifecycleNode` 基类和相关转换服务。

## 构建与运行

```bash
colcon build --packages-select demo_xmake_lifecycle
source install/setup.bash

# 终端 1：启动生命周期节点
ros2 run demo_xmake_lifecycle lifecycle_talker

# 终端 2：驱动状态转换
ros2 lifecycle set /lifecycle_talker configure
ros2 lifecycle set /lifecycle_talker activate

# 或启动管理器自动驱动
ros2 run demo_xmake_lifecycle lifecycle_manager
```

## 验证

```bash
# 查看当前状态
ros2 lifecycle get /lifecycle_talker

# 查看可用转换
ros2 lifecycle list /lifecycle_talker

# 查看生命周期话题
ros2 topic echo /lifecycle_talker/transition_event
```

## xmake 知识点小结

本示例没有引入新的 xmake 特性，重点在于 `rclcpp_lifecycle` 作为 ROS 依赖的使用。

## 内容扩展

> 以下资源可帮助你深入了解本章涉及的技术领域。

### 开源项目
- [launch_ros lifecycle events](https://github.com/ros2/launch_ros) — Launch 中的生命周期事件
- [bondcpp](https://github.com/ros/bond_core) — 节点生命监控（心跳）

### 文档与教程
- [Managed Nodes 设计](https://design.ros2.org/articles/node_lifecycle.html)

### 论文与技术报告
- I. Lütkebohle, "Deterministic Startup with Lifecycle Nodes", ROSCon 2019

## 参考

- [ROS 2 官方文档 - Managed Nodes](https://docs.ros.org/en/jazzy/Tutorials/Intermediate/Managing-Nodes-Lifecycle.html)
- [ROS 2 Lifecycle 设计](https://design.ros2.org/articles/node_lifecycle.html)
