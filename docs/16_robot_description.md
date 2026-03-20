---
sidebar_position: 16
title: "16. 机器人模型 — URDF 与 robot_state_publisher"
---
# 16. 机器人模型 — URDF 与 robot_state_publisher

## 概述

本示例通过 [`demo_xmake_robot_description`](../demo_xmake_robot_description) 包，演示如何管理 URDF 机器人模型并通过 launch 文件加载。

**ROS 2 知识点**：URDF 格式、robot_state_publisher、joint_state_publisher

**xmake 知识点**：`urdf/` 约定目录自动安装、phony target 纯数据包

## ROS 2 知识

### URDF

URDF（Unified Robot Description Format）描述机器人的物理结构：

```xml
<robot name="simple_robot">
  <link name="base_link">
    <visual>
      <geometry><box size="0.5 0.3 0.1"/></geometry>
    </visual>
  </link>
  <joint name="base_to_sensor" type="fixed">
    <parent link="base_link"/>
    <child link="sensor_link"/>
    <origin xyz="0.2 0.0 0.1"/>
  </joint>
</robot>
```

### robot_state_publisher

将 URDF 和关节状态发布为 TF 变换，使 RViz 等工具可以可视化机器人模型。

## 示例代码

### xmake.lua 构建配置

```lua
-- urdf/ 和 launch/ 目录自动安装
target("demo_xmake_robot_description_marker")
    set_kind("phony")
```

这是一个**纯数据包** — 没有 C++ 代码，只包含 URDF 和 launch 文件。phony target 确保 ament_xmake 规则被触发。

## 构建与运行

```bash
colcon build --packages-select demo_xmake_robot_description
source install/setup.bash

ros2 launch demo_xmake_robot_description display.launch.py
```

## 验证

```bash
# 验证 URDF 已安装
ls install/demo_xmake_robot_description/share/demo_xmake_robot_description/urdf/

# 查看 robot_description 参数
ros2 param get /robot_state_publisher robot_description
```

## xmake 知识点小结

| 特性 | 说明 |
|------|------|
| `urdf/` 自动安装 | 约定目录，无需显式声明 |
| phony target | 纯数据包的最小 target |

## 内容扩展

> 以下资源可帮助你深入了解本章涉及的技术领域。

### 开源项目
- [xacro](https://github.com/ros/xacro) — URDF 宏语言（减少重复）
- [urdf](https://github.com/ros/urdf) — URDF 解析库
- [SDF (Gazebo)](http://sdformat.org/) — 仿真描述格式（URDF 超集）

### 工具
- [check_urdf](https://docs.ros.org/en/jazzy/Tutorials/Intermediate/URDF/URDF-Main.html) — URDF 语法校验
- [urdf_to_graphviz](https://docs.ros.org/en/jazzy/Tutorials/Intermediate/URDF/URDF-Main.html) — URDF 树形图生成

### 文档与教程
- [URDF XML 规范](https://wiki.ros.org/urdf/XML)

## 参考

- [ROS 2 官方文档 - URDF tutorials](https://docs.ros.org/en/jazzy/Tutorials/Intermediate/URDF/URDF-Main.html)
