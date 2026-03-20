---
sidebar_position: 11
title: "11. 混合语言包 — C++ 发布 / Python 订阅"
---
# 11. 混合语言包 — C++ 发布 / Python 订阅

## 概述

本示例通过 [`demo_xmake_mixed`](../demo_xmake_mixed) 包，演示如何在一个包中混合 C++ 和 Python 代码。C++ 节点作为发布者，Python 节点作为订阅者，通过 Topic 通信。

**ROS 2 知识点**：rclpy 基础、跨语言 Topic 通信

**xmake 知识点**：xmake + setup.py 混合构建

## ROS 2 知识

### 跨语言通信

ROS 2 的消息序列化机制使 C++ 和 Python 节点可以无缝通信。只要使用相同的消息类型和话题名，不同语言的节点可以互操作。

## 示例代码

### 项目结构

```
demo_xmake_mixed/
├── xmake.lua           # C++ 部分构建
├── setup.py            # Python 部分构建
├── package.xml
├── src/
│   └── cpp_node.cpp    # C++ 发布者
└── demo_xmake_mixed/
    ├── __init__.py
    └── py_node.py      # Python 订阅者
```

### xmake.lua 构建配置

```lua
target("cpp_publisher")
    set_kind("binary")
    set_installdir("lib/demo_xmake_mixed")
    add_files("src/cpp_node.cpp")
    add_ros_deps("rclcpp", "std_msgs")
```

xmake 只负责 C++ 部分。Python 部分由 `setup.py` 处理，colcon 会自动识别并安装。

### setup.py

```python
from setuptools import setup

setup(
    name='demo_xmake_mixed',
    packages=['demo_xmake_mixed'],
    entry_points={
        'console_scripts': [
            'py_subscriber = demo_xmake_mixed.py_node:main',
        ],
    },
)
```

## 构建与运行

```bash
colcon build --packages-select demo_xmake_mixed
source install/setup.bash

# 终端 1：C++ 发布者
ros2 run demo_xmake_mixed cpp_publisher

# 终端 2：Python 订阅者
ros2 run demo_xmake_mixed py_subscriber
```

## xmake 知识点小结

| 特性 | 说明 |
|------|------|
| xmake + setup.py | C++ 用 xmake 构建，Python 用 setuptools |

## 内容扩展

> 以下资源可帮助你深入了解本章涉及的技术领域。

### 开源项目
- [rclpy](https://github.com/ros2/rclpy) — Python 客户端库
- [pybind11_catkin](https://github.com/ipab-slmc/pybind11_catkin) — C++↔Python 绑定参考

### 文档与教程
- [rclpy API 文档](https://docs.ros2.org/latest/api/rclpy/)

## 参考

- [ROS 2 官方文档 - Writing a simple publisher and subscriber (Python)](https://docs.ros.org/en/jazzy/Tutorials/Beginner-Client-Libraries/Writing-A-Simple-Py-Publisher-And-Subscriber.html)
