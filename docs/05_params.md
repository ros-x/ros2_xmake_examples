---
sidebar_position: 5
title: "05. 参数系统 — 声明、回调、YAML 配置"
---
# 05. 参数系统 — 声明、回调、YAML 配置

## 概述

本示例演示 ROS 2 的**参数系统**：声明参数、读取参数、监听动态修改，以及通过 YAML 文件加载参数。同时展示 ament_xmake 的**约定目录自动安装**机制。

**ROS 2 知识点**：declare_parameter、get_parameter、参数变更回调、YAML 参数文件

**xmake 知识点**：约定目录自动安装（launch/、config/）

## ROS 2 知识

### 参数系统

ROS 2 参数是节点级别的配置值，支持运行时动态修改：

```cpp
// 声明参数（带默认值）
this->declare_parameter("robot_name", "default_robot");

// 读取参数
auto name = this->get_parameter("robot_name").as_string();

// 监听参数变更
param_callback_ = this->add_on_set_parameters_callback(callback);
```

参数类型：`bool`、`int`、`double`、`string`、`byte_array`、`bool_array` 等。

### YAML 参数文件

```yaml
param_node:          # 节点名
  ros__parameters:   # 固定前缀
    robot_name: "xmake_bot"
    max_speed: 1.5
```

通过 launch 文件加载：

```python
Node(
    package='demo_xmake_params',
    executable='param_node',
    parameters=[config_file],  # YAML 文件路径
)
```

## 示例代码

### xmake.lua 构建配置

```lua
-- config/ 和 launch/ 目录会被自动安装到 share/<pkg>/
-- 无需显式调用 install_ros_data()（约定优于配置）
target("param_node")
    set_kind("binary")
    set_installdir("lib/demo_xmake_params")
    add_files("src/param_node.cpp")
    add_ros_deps("rclcpp")
```

**约定目录自动安装**：ament_xmake 会自动检测包根目录下的以下目录并安装到 `share/<pkg>/`：

| 目录 | 安装位置 | 说明 |
|------|---------|------|
| `launch/` | `share/<pkg>/launch/` | Launch 文件 |
| `config/` | `share/<pkg>/config/` | 配置文件 |
| `urdf/` | `share/<pkg>/urdf/` | URDF 模型 |
| `meshes/` | `share/<pkg>/meshes/` | 3D 模型 |

无需在 xmake.lua 中调用 `install_ros_data()`，这是**约定优于配置**的体现。

### 何时需要 install_ros_data()

当数据文件不在约定目录中，或需要自定义安装路径时：

```lua
-- 显式安装非约定目录的文件
install_ros_data("my_data", "data/*.csv")
```

## 构建与运行

```bash
colcon build --packages-select demo_xmake_params
source install/setup.bash

# 直接运行（使用代码中的默认值）
ros2 run demo_xmake_params param_node

# 通过 launch 加载 YAML 参数
ros2 launch demo_xmake_params param_demo.launch.py

# 运行时修改参数
ros2 param set /param_node max_speed 2.5
ros2 param set /param_node robot_name "new_bot"
```

## 验证

```bash
# 查看所有参数
ros2 param list /param_node

# 读取参数值
ros2 param get /param_node robot_name
ros2 param get /param_node max_speed

# 尝试设置非法值（会被回调拒绝）
ros2 param set /param_node max_speed -1.0
```

## xmake 知识点小结

| 特性 | 说明 |
|------|------|
| 约定目录自动安装 | `launch/`、`config/` 等目录自动安装到 `share/<pkg>/` |
| 无需 `install_ros_data()` | 约定目录无需显式声明 |

## 内容扩展

> 以下资源可帮助你深入了解本章涉及的技术领域。

### 开源项目
- [generate_parameter_library](https://github.com/PickNikRobotics/generate_parameter_library) — 声明式参数定义 + 代码生成

### 工具
- [rqt_reconfigure](https://docs.ros.org/en/jazzy/Tutorials/Beginner-Client-Libraries/Using-Parameters-In-A-Class-CPP.html) — GUI 动态参数调整

### 文档与教程
- [ROS 2 Parameter Design](https://design.ros2.org/articles/ros_parameters.html)
- [YAML 规范](https://yaml.org/spec/1.2.2/)

## 参考

- [ROS 2 官方文档 - Using parameters in a class (C++)](https://docs.ros.org/en/jazzy/Tutorials/Beginner-Client-Libraries/Using-Parameters-In-A-Class-CPP.html)
- [ROS 2 参数概念](https://docs.ros.org/en/jazzy/Concepts/Basic/About-Parameters.html)
