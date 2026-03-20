---
sidebar_position: 27
title: "27. 通信性能基准测试 — 延迟与吞吐量"
---
# 27. 通信性能基准测试 — 延迟与吞吐量

## 概述

本示例演示如何测量 ROS 2 通信的延迟和吞吐量。通过 [`demo_xmake_benchmark`](../demo_xmake_benchmark) 包，你将学习 ping-pong 延迟测量方法、吞吐量测试，以及 intra-process 零拷贝与 inter-process 通信的性能差异。

**ROS 2 知识点**：通信延迟测量、吞吐量测试、intra-process vs inter-process、DDS 调优

**xmake 知识点**：同包多 target 定义、条件编译

## ROS 2 知识

### 通信延迟测量

Ping-Pong 模式是测量通信延迟的标准方法：

```
┌──────────┐  /benchmark/ping   ┌──────────┐
│  Ping    │ ──────────────────> │  Pong    │
│  (测量端) │ <────────────────── │  (回声端) │
└──────────┘  /benchmark/pong   └──────────┘

Round-trip latency = t_receive_pong - t_send_ping
One-way latency ≈ Round-trip / 2
```

### 影响延迟的因素

| 因素 | 影响程度 | 说明 |
|------|----------|------|
| 进程模型 | 高 | 同进程（零拷贝）vs 跨进程（序列化） |
| 消息大小 | 高 | 大消息序列化/拷贝耗时更长 |
| DDS 实现 | 中 | FastDDS vs CycloneDDS vs Zenoh |
| QoS 配置 | 中 | reliable 比 best_effort 延迟更高 |
| 系统负载 | 中 | CPU 竞争影响调度延迟 |
| 网络（跨机） | 高 | 网络传输延迟、带宽限制 |

### Intra-process 零拷贝

当 Publisher 和 Subscriber 在同一进程中时，ROS 2 可以使用 intra-process 通信，避免序列化和数据拷贝：

```cpp
// 启用 intra-process 通信
rclcpp::NodeOptions options;
options.use_intra_process_comms(true);
auto node = std::make_shared<MyNode>("node", options);
```

性能对比（典型值，仅供参考）：

| 通信方式 | 1KB 消息延迟 | 1MB 消息延迟 |
|----------|-------------|-------------|
| Intra-process | ~10-50 μs | ~50-200 μs |
| Inter-process (同机) | ~100-500 μs | ~1-5 ms |
| Inter-process (跨机) | ~1-10 ms | ~10-100 ms |

### 多节点性能经验

- **低频小消息**（状态、命令）：拆成多 node 开销很小
- **高频大数据**（图像、点云）：应使用 Composable Nodes + intra-process
- **几十个节点、几十 Hz 以下**：通常不是瓶颈
- **真正的瓶颈信号**：CPU 高但业务不复杂、延迟抖动大、`ros2 topic bw` 显示带宽很大

## 示例代码

### 项目结构

```
demo_xmake_benchmark/
├── xmake.lua
├── package.xml         # deps: rclcpp, std_msgs
└── src/
    ├── latency_ping.cpp    # 延迟测量 — ping 端
    ├── latency_pong.cpp    # 延迟测量 — pong 端
    └── throughput_test.cpp # 吞吐量测试
```

### xmake.lua 构建配置

```lua
set_project("demo_xmake_benchmark")
set_version("0.1.0")
set_languages("cxx17")

add_rules("mode.debug", "mode.release")
add_rules("ament_xmake.package")

target("latency_ping")
    set_kind("binary")
    set_installdir("lib/demo_xmake_benchmark")
    add_files("src/latency_ping.cpp")
    add_ros_deps("rclcpp", "std_msgs")

target("latency_pong")
    set_kind("binary")
    set_installdir("lib/demo_xmake_benchmark")
    add_files("src/latency_pong.cpp")
    add_ros_deps("rclcpp", "std_msgs")

target("throughput_test")
    set_kind("binary")
    set_installdir("lib/demo_xmake_benchmark")
    add_files("src/throughput_test.cpp")
    add_ros_deps("rclcpp", "std_msgs")
```

同一个 xmake.lua 中定义三个独立的 binary target，每个都安装到 `lib/<pkg>/` 下。

### 源码要点

**Ping-Pong 延迟测量**：

```cpp
// ping 端：发送带时间戳的消息
void send_ping() {
    auto msg = std_msgs::msg::String();
    auto now = std::chrono::steady_clock::now();
    msg.data = std::to_string(now.time_since_epoch().count());
    pub_ping_->publish(msg);
}

// ping 端：接收 pong，计算延迟
void on_pong(const std_msgs::msg::String::SharedPtr msg) {
    auto now = std::chrono::steady_clock::now();
    auto send_time = std::chrono::steady_clock::time_point(
        std::chrono::nanoseconds(std::stoll(msg->data)));
    auto latency = std::chrono::duration_cast<std::chrono::microseconds>(
        now - send_time);
    RCLCPP_INFO(get_logger(), "Round-trip latency: %.2f ms",
                latency.count() / 1000.0);
}
```

**吞吐量测试**：

```cpp
// 每秒统计接收到的消息数
void report_throughput() {
    auto elapsed = /* 计算时间间隔 */;
    double msgs_per_sec = msg_count_ / elapsed;
    double kb_per_sec = (msg_count_ * msg_size_) / (elapsed * 1024.0);
    RCLCPP_INFO(get_logger(), "Throughput: %.0f msgs/sec (%.0f KB/sec)",
                msgs_per_sec, kb_per_sec);
    msg_count_ = 0;
}
```

## 构建与运行

```bash
# 构建
colcon build --packages-select demo_xmake_benchmark
source install/setup.bash

# 延迟测试 — 两个终端
ros2 run demo_xmake_benchmark latency_pong    # 终端 1
ros2 run demo_xmake_benchmark latency_ping    # 终端 2

# 吞吐量测试
ros2 run demo_xmake_benchmark throughput_test
```

## 验证

```bash
# 查看 benchmark 话题
ros2 topic list | grep benchmark
# /benchmark/ping
# /benchmark/pong
# /benchmark/throughput

# 查看消息频率
ros2 topic hz /benchmark/ping

# 查看带宽
ros2 topic bw /benchmark/throughput
```

## xmake 知识点小结

| 特性 | 说明 |
|------|------|
| 同包多 target | 一个 xmake.lua 定义 3 个 binary target |
| `set_installdir()` | 所有可执行文件安装到同一个 `lib/<pkg>/` 目录 |
| `add_ros_deps()` 共享依赖 | 多个 target 使用相同的 ROS 依赖 |

## 内容扩展

> 以下资源可帮助你深入了解本章涉及的技术领域。

### 开源项目
- [performance_test](https://gitlab.com/ApexAI/performance_test) — Apex.AI 官方 ROS 2 性能测试工具
- [builtin_interfaces](https://github.com/ros2/builtin_interfaces) — ROS 2 基础类型定义

### 文档与教程
- [ROS 2 QoS 与性能](https://docs.ros.org/en/jazzy/Concepts/Intermediate/About-Quality-of-Service-Settings.html) — QoS 对性能的影响
- [DDS 调优指南](https://docs.ros.org/en/jazzy/How-To-Guides/DDS-tuning.html) — DDS 参数优化

### 论文与技术报告
- Y. Maruyama et al., "Exploring the Performance of ROS 2", EMSOFT 2016 — ROS 2 早期性能基准测试与分析

## 参考

- [Apex.AI performance_test](https://gitlab.com/ApexAI/performance_test)
- [ROS 2 Intra-Process Communication](https://docs.ros.org/en/jazzy/Tutorials/Demos/Intra-Process-Communication.html)
- [ROS 2 DDS Tuning](https://docs.ros.org/en/jazzy/How-To-Guides/DDS-tuning.html)
- [ament_xmake](https://github.com/ros-x/ament_xmake)
