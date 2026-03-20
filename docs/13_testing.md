---
sidebar_position: 13
title: "13. 测试集成 — gtest + colcon test"
---
# 13. 测试集成 — gtest + colcon test

## 概述

本示例通过 [`demo_xmake_testing`](../demo_xmake_testing) 包，演示如何在 xmake 项目中集成 **gtest** 单元测试，并与 `colcon test` 兼容。

**ROS 2 知识点**：ament 测试框架、gtest 集成、节点单元测试

**xmake 知识点**：xmake test target、`colcon test` 兼容

## ROS 2 知识

### 测试最佳实践

- **纯逻辑测试**：不依赖 ROS 运行时的单元测试
- **节点测试**：验证节点创建、参数声明等 ROS 功能
- **集成测试**：多节点协作测试（通常用 launch_testing）

## 示例代码

### 项目结构

```
demo_xmake_testing/
├── xmake.lua
├── package.xml
├── src/
│   ├── calculator.hpp     # 可测试的类
│   ├── calculator.cpp
│   └── calculator_main.cpp
└── test/
    ├── test_calculator.cpp  # 纯逻辑测试
    └── test_node.cpp        # 节点测试
```

### xmake.lua 构建配置

```lua
target("demo_xmake_testing")
    set_kind("static")
    add_files("src/calculator.cpp")
    add_includedirs("src", {public = true})
    add_ros_deps("rclcpp")

target("calculator_node")
    set_kind("binary")
    set_installdir("lib/demo_xmake_testing")
    add_files("src/calculator_main.cpp")
    add_deps("demo_xmake_testing")
    add_ros_deps("rclcpp")
```

## 构建与运行

```bash
colcon build --packages-select demo_xmake_testing
colcon test --packages-select demo_xmake_testing
colcon test-result --verbose
```

## xmake 知识点小结

| 特性 | 说明 |
|------|------|
| `add_deps()` | 引用同包内的其他 target |
| 库 + 可执行文件 | 将逻辑封装为库，方便测试 |

## 内容扩展

> 以下资源可帮助你深入了解本章涉及的技术领域。

### 开源项目
- [launch_testing](https://github.com/ros2/launch/tree/rolling/launch_testing) — ROS 2 集成测试
- [ament_lint](https://github.com/ament/ament_lint) — 代码风格检查

### 工具
- [colcon-coveragepy-result](https://github.com/colcon/colcon-coveragepy-result) — 覆盖率统计

### 文档与教程
- [GoogleTest 文档](https://google.github.io/googletest/)
- [ROS 2 Testing 最佳实践](https://docs.ros.org/en/jazzy/Tutorials/Intermediate/Testing/Testing-Main.html)

## 参考

- [ROS 2 官方文档 - Testing](https://docs.ros.org/en/jazzy/Tutorials/Intermediate/Testing/Testing-Main.html)
