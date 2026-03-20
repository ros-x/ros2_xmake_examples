# 14. CMake 互操作 — xmake 包被 CMake 消费

## 概述

本示例通过 [`downstream_cmake_consumer`](../downstream_cmake_consumer) 包，演示标准 `ament_cmake` 包如何消费 xmake 构建的包导出。这验证了 xmake 包的 CMake 互操作性。

**ROS 2 知识点**：find_package、ament_cmake 生态

**xmake 知识点**：xmake 自动生成的 CMake Config 如何被下游消费

## ROS 2 知识

### find_package 与 Config 模式

ROS 2 的包发现机制基于 CMake 的 `find_package(... CONFIG)`：

```cmake
find_package(demo_xmake_cpp REQUIRED CONFIG)
target_link_libraries(consumer PRIVATE demo_xmake_cpp::demo_xmake_cpp)
```

`CONFIG` 模式直接使用包提供的 `<pkg>Config.cmake` 文件，而非 CMake 的 `Find<pkg>.cmake`。

## 示例代码

### CMakeLists.txt

```cmake
cmake_minimum_required(VERSION 3.16)
project(downstream_cmake_consumer)

find_package(ament_cmake REQUIRED)
find_package(demo_xmake_cpp REQUIRED CONFIG)

add_executable(consumer src/consumer.cpp)
target_link_libraries(consumer PRIVATE demo_xmake_cpp::demo_xmake_cpp)

install(TARGETS consumer DESTINATION lib/${PROJECT_NAME})
ament_package()
```

### xmake 自动生成的 Config 文件

ament_xmake 在安装时自动生成 `<pkg>Config.cmake`，包含：

- imported target `<pkg>::<pkg>`
- include 目录
- 链接库路径
- 依赖传递

## 构建与运行

```bash
# 先构建被依赖的 xmake 包
colcon build --packages-select demo_xmake_cpp

# 再构建 CMake 消费包
colcon build --packages-select downstream_cmake_consumer
source install/setup.bash

# 运行
ros2 run downstream_cmake_consumer consumer
```

## 验证

```bash
# 验证 Config 文件存在
cat install/demo_xmake_cpp/share/demo_xmake_cpp/cmake/demo_xmake_cppConfig.cmake

# 验证 imported target 可用
ros2 run downstream_cmake_consumer consumer
```

## xmake 知识点小结

| 特性 | 说明 |
|------|------|
| CMake Config 自动生成 | ament_xmake 规则自动生成 Config.cmake |
| imported target | 下游包通过 `<pkg>::<pkg>` 使用 xmake 包 |

## 内容扩展

> 以下资源可帮助你深入了解本章涉及的技术领域。

### 开源项目
- [colcon-xmake](https://github.com/ros-x/colcon-xmake) — colcon 的 xmake 插件

### 文档与教程
- [ament_xmake RULE_SPEC](https://github.com/ros-x/ament_xmake/blob/main/docs/RULE_SPEC.md)
- [CMake find_package Config Mode](https://cmake.org/cmake/help/latest/command/find_package.html)
- [xmake 与 CMake 互操作指南](https://xmake.io/#/guide/faq)

## 参考

- [ament_xmake RULE_SPEC](https://github.com/ros-x/ament_xmake/blob/main/docs/RULE_SPEC.md)
- [CMake Config Mode](https://cmake.org/cmake/help/latest/command/find_package.html#config-mode-search-procedure)
