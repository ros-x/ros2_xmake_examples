# 20. 数据录制回放 — rosbag2 程序化 API

## 概述

本示例通过 `demo_xmake_rosbag` 包，演示如何使用 **rosbag2** 的 C++ API 程序化录制和回放话题数据。

**ROS 2 知识点**：rosbag2 API、程序化录制/回放

**xmake 知识点**：`rosbag2_cpp` 依赖

## ROS 2 知识

### rosbag2

rosbag2 支持命令行和程序化两种使用方式：

```bash
# 命令行
ros2 bag record /chatter
ros2 bag play my_bag

# 程序化 API
rosbag2_cpp::Writer writer;
writer.open("path");
writer.write(msg, "topic", "type", timestamp);
```

## 构建与运行

```bash
colcon build --packages-select demo_xmake_rosbag
source install/setup.bash

# 录制
ros2 run demo_xmake_rosbag bag_recorder

# 回放
ros2 run demo_xmake_rosbag bag_player
```

## 内容扩展

> 以下资源可帮助你深入了解本章涉及的技术领域。

### 开源项目
- [rosbag2](https://github.com/ros2/rosbag2) — rosbag2 核心
- [mcap](https://github.com/foxglove/mcap) — 高性能录制格式（rosbag2 后端）

### 工具
- [Foxglove Studio](https://foxglove.dev/) — 浏览器端 bag 回放分析
- [PlotJuggler](https://github.com/facontidavide/PlotJuggler) — bag 数据可视化

## 参考

- [ROS 2 官方文档 - rosbag2](https://docs.ros.org/en/jazzy/Tutorials/Beginner-CLI-Tools/Recording-And-Playing-Back-Data/Recording-And-Playing-Back-Data.html)
