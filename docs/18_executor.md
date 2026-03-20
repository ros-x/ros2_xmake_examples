# 18. 执行器 — 单线程/多线程与回调组

## 概述

本示例通过 [`demo_xmake_executor`](../demo_xmake_executor) 包，演示 ROS 2 的**执行器**（Executor）和**回调组**（CallbackGroup）。

**ROS 2 知识点**：SingleThreadedExecutor、MultiThreadedExecutor、CallbackGroup（MutuallyExclusive vs Reentrant）

**xmake 知识点**：同包多 binary 构建

## ROS 2 知识

### 执行器类型

| 执行器 | 线程数 | 特点 |
|--------|--------|------|
| SingleThreadedExecutor | 1 | 简单安全，回调串行执行 |
| MultiThreadedExecutor | N | 回调可并行，需注意线程安全 |

### 回调组

```cpp
// 互斥组 — 同一组的回调不会并行执行
auto exclusive = create_callback_group(MutuallyExclusive);

// 可重入组 — 同一组的回调可以并行执行
auto reentrant = create_callback_group(Reentrant);
```

### 使用场景

- 多个独立节点共享进程时使用 Executor
- 需要并行处理回调时使用 MultiThreadedExecutor + Reentrant group
- 需要保护共享状态时使用 MutuallyExclusive group

## 构建与运行

```bash
colcon build --packages-select demo_xmake_executor
source install/setup.bash

# 单线程演示
ros2 run demo_xmake_executor single_threaded_demo

# 多线程演示
ros2 run demo_xmake_executor multi_threaded_demo
```

## 内容扩展

> 以下资源可帮助你深入了解本章涉及的技术领域。

### 文档与教程
- [ROS 2 Executor 概念](https://docs.ros.org/en/jazzy/Concepts/Intermediate/About-Executors.html)
- [events_executor 设计](https://discourse.ros.org/t/ros2-middleware-change-proposal-to-support-events-executor/15596)

### 论文与技术报告
- J. Staschulat et al., "The rclcpp Executor", ECRTS 2021 — Executor 调度延迟分析
- T. Blass et al., "A ROS 2 Response Time Analysis", RTAS 2021

## 参考

- [ROS 2 Executor 概念](https://docs.ros.org/en/jazzy/Concepts/Intermediate/About-Executors.html)
