---
sidebar_position: 12
title: "12. QoS 配置 — 可靠性、持久性、传感器模式"
---
# 12. QoS 配置 — 可靠性、持久性、传感器模式

## 概述

本示例通过 [`demo_xmake_qos`](../demo_xmake_qos) 包，演示 ROS 2 的 **QoS**（Quality of Service）配置，包括可靠性策略、持久性策略和 SensorDataQoS 预设。

**ROS 2 知识点**：QoS Profile、Reliability、Durability、History、Depth、QoS 不兼容检测

**xmake 知识点**：多 target 不同 QoS 配置对比

## ROS 2 知识

### QoS 参数

| 参数 | 选项 | 说明 |
|------|------|------|
| Reliability | RELIABLE / BEST_EFFORT | 消息可靠性保证 |
| Durability | VOLATILE / TRANSIENT_LOCAL | 是否为新订阅者保留历史消息 |
| History | KEEP_LAST / KEEP_ALL | 消息缓冲策略 |
| Depth | 整数 | 缓冲区大小 |

### QoS 兼容性

发布者和订阅者的 QoS 必须兼容才能通信：

| 发布者 | 订阅者 | 兼容? |
|--------|--------|-------|
| RELIABLE | RELIABLE | 是 |
| RELIABLE | BEST_EFFORT | 否 |
| BEST_EFFORT | BEST_EFFORT | 是 |
| BEST_EFFORT | RELIABLE | 是 |

### 预设 QoS Profile

| Profile | Reliability | Durability | 适用场景 |
|---------|-------------|------------|---------|
| 默认 | RELIABLE | VOLATILE | 常规通信 |
| SensorDataQoS | BEST_EFFORT | VOLATILE | 高频传感器 |
| SystemDefaultsQoS | 系统默认 | 系统默认 | 跟随系统 |

## 示例代码

### xmake.lua 构建配置

```lua
target("reliable_talker")
    set_kind("binary")
    set_installdir("lib/demo_xmake_qos")
    add_files("src/reliable_talker.cpp")
    add_ros_deps("rclcpp", "std_msgs")

target("best_effort_listener")
    set_kind("binary")
    set_installdir("lib/demo_xmake_qos")
    add_files("src/best_effort_listener.cpp")
    add_ros_deps("rclcpp", "std_msgs")

target("sensor_demo")
    set_kind("binary")
    set_installdir("lib/demo_xmake_qos")
    add_files("src/sensor_demo.cpp")
    add_ros_deps("rclcpp", "std_msgs")
```

三个 target 展示不同 QoS 配置，构建配置本身与 QoS 无关（QoS 在代码中设置）。

## 构建与运行

```bash
colcon build --packages-select demo_xmake_qos
source install/setup.bash

# 演示 QoS 不兼容
ros2 run demo_xmake_qos reliable_talker     # 终端 1
ros2 run demo_xmake_qos best_effort_listener # 终端 2（无法接收）

# 传感器模式演示
ros2 run demo_xmake_qos sensor_demo
```

## 验证

```bash
# 查看 QoS 信息
ros2 topic info /qos_topic --verbose
```

## xmake 知识点小结

本示例没有引入新的 xmake 特性，重点在于展示同包多个 binary target。

## 内容扩展

> 以下资源可帮助你深入了解本章涉及的技术领域。

### 工具
- [ros2 topic info --verbose](https://docs.ros.org/en/jazzy/Concepts/Intermediate/About-Quality-of-Service-Settings.html) — 查看 QoS 详情

### 文档与教程
- [DDS QoS 策略完整列表](https://community.rti.com/static/documentation/connext-dds/current/doc/manuals/connext_dds_professional/users_manual/index.htm#users_manual/QosPolicies.htm)
- [ROS 2 QoS 兼容性矩阵](https://docs.ros.org/en/jazzy/Concepts/Intermediate/About-Quality-of-Service-Settings.html)

### 论文与技术报告
- G. Pardo-Castellote, "OMG Data-Distribution Service", IEEE Distributed Systems, 2003

## 参考

- [ROS 2 官方文档 - Quality of Service](https://docs.ros.org/en/jazzy/Concepts/Intermediate/About-Quality-of-Service-Settings.html)
