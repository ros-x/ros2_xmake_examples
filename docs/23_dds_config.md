---
sidebar_position: 23
title: "23. DDS 配置 — RMW 切换与 QoS Profile"
---
# 23. DDS 配置 — RMW 切换与 QoS Profile

## 概述

本示例通过 [`demo_xmake_dds_config`](../demo_xmake_dds_config) 包，演示如何管理 DDS/RMW 配置文件，并通过 launch 文件设置环境变量。

**ROS 2 知识点**：RMW 切换（FastDDS/CycloneDDS）、QoS XML Profile、DDS Discovery 配置

**xmake 知识点**：config 文件安装 + phony target

## ROS 2 知识

### RMW（ROS Middleware）

ROS 2 通过 RMW 抽象层支持多种 DDS 实现：

| RMW | DDS 实现 | 特点 |
|-----|---------|------|
| `rmw_fastrtps_cpp` | FastDDS | ROS 2 默认 |
| `rmw_cyclonedds_cpp` | CycloneDDS | 轻量、适合嵌入式 |

### 切换 RMW

```bash
export RMW_IMPLEMENTATION=rmw_cyclonedds_cpp
```

### DDS 配置文件

- **FastDDS**：XML Profile 文件，通过 `FASTRTPS_DEFAULT_PROFILES_FILE` 环境变量加载
- **CycloneDDS**：XML 配置文件，通过 `CYCLONEDDS_URI` 环境变量加载

## 示例代码

### xmake.lua 构建配置

```lua
-- config/ 和 launch/ 目录自动安装
target("demo_xmake_dds_config_marker")
    set_kind("phony")
```

纯数据包：只包含配置文件和 launch 文件。

## 构建与运行

```bash
colcon build --packages-select demo_xmake_dds_config
source install/setup.bash

ros2 launch demo_xmake_dds_config with_dds_config.launch.py
ros2 launch demo_xmake_dds_config with_dds_config.launch.py \
  rmw_implementation:=rmw_cyclonedds_cpp
```

## 验证

```bash
# 查看当前 RMW
ros2 doctor --report | grep middleware
```

## xmake 知识点小结

| 特性 | 说明 |
|------|------|
| phony target | 纯数据包的最小配置 |
| 约定目录自动安装 | config/ 自动安装到 share/<pkg>/config/ |

## 内容扩展

> 以下资源可帮助你深入了解本章涉及的技术领域。

### 开源项目
- [Fast-DDS](https://github.com/eProsima/Fast-DDS) — eProsima FastDDS
- [CycloneDDS](https://github.com/eclipse-cyclonedds/cyclonedds) — Eclipse CycloneDDS
- [Zenoh](https://github.com/eclipse-zenoh/zenoh) — RMW Zenoh（新一代中间件）

### 文档与教程
- [DDS 规范 (OMG)](https://www.omg.org/spec/DDS/)
- [FastDDS XML 配置](https://fast-dds.docs.eprosima.com/en/latest/fastdds/xml_configuration/xml_configuration.html)

### 论文与技术报告
- G. Pardo-Castellote, "OMG Data-Distribution Service", IEEE Distributed Systems, 2003

## 参考

- [ROS 2 DDS 配置](https://docs.ros.org/en/jazzy/How-To-Guides/DDS-tuning.html)
- [FastDDS XML Profile](https://fast-dds.docs.eprosima.com/en/latest/fastdds/xml_configuration/xml_configuration.html)
