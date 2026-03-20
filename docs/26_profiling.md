---
sidebar_position: 26
title: "26. 内存与 CPU 分析 — 性能调优指南"
---
# 26. 内存与 CPU 分析 — 性能调优指南

## 概述

本章是纯文档章节（无示例包），指导如何使用外部工具分析 ROS 2 节点的内存和 CPU 性能。你将学习 Valgrind、AddressSanitizer、perf、Flame Graph 和 heaptrack 的用法，以及在 xmake 中启用 sanitizer 的方法。

**ROS 2 知识点**：节点资源监控、内存泄漏检测、CPU 热点分析、性能陷阱

**xmake 知识点**：`add_cxflags` 启用 sanitizer、debug 模式构建

## ROS 2 知识

### 性能分析的必要性

ROS 2 系统中常见的性能问题：

| 问题类型 | 症状 | 常见原因 |
|----------|------|----------|
| CPU 过高 | 节点占用 >100% CPU | 序列化/反序列化、不必要拷贝、高频回调 |
| 内存泄漏 | 内存持续增长 | 未释放的消息缓冲、累积的 TF 数据 |
| 延迟抖动 | 回调时间不稳定 | 线程竞争、GC（Python）、内存分配 |
| 吞吐量不足 | 消息丢失或延迟 | QoS 误配、序列化瓶颈、DDS 配置 |

### 多节点场景的性能考量

把系统拆成很多 ROS 2 node 的典型性能影响主要在两方面：

**通信开销**：
- 跨进程通信需要序列化/反序列化 + 数据拷贝
- 高频大消息（图像、点云）会占用大量 CPU 和内存带宽
- DDS 发现协议在节点多时有额外网络/CPU 负担

**调度/进程开销**：
- 独立进程的 node 增加 OS 调度开销和上下文切换
- 每个进程各有一份 DDS 参与者/内存/日志
- CPU cache 命中率可能下降

**优化策略**：
- 用 Composable Nodes 将强耦合节点放入同一进程
- 对大数据使用共享内存/零拷贝（loaned message）
- 优化 QoS：传感器用 best_effort + keep_last 小深度
- 降低消息频率/大小：下采样、压缩、只发 ROI
- 合理配置 Executor/线程模型

### ROS 2 节点的典型性能陷阱

1. **不必要的消息拷贝**：在回调中拷贝大消息而非使用引用
2. **序列化开销**：同进程内的组件间通信如果走 DDS 会有不必要的序列化
3. **QoS 误配**：传感器数据使用 reliable + 大队列导致延迟
4. **定时器竞争**：多个高频定时器在单线程 Executor 中互相阻塞
5. **TF2 缓存膨胀**：长时间运行的 TF listener 累积过多历史数据

## 工具使用指南

### 1. Valgrind — 内存错误检测

```bash
# 使用 Valgrind 检查内存错误
valgrind --tool=memcheck --leak-check=full \
  ros2 run demo_xmake_pubsub talker

# 检查未初始化内存访问
valgrind --tool=memcheck --track-origins=yes \
  ros2 run demo_xmake_pubsub talker
```

注意：Valgrind 会使程序运行速度降低 10-50 倍，适合离线分析。

### 2. AddressSanitizer (ASan) — 编译时内存检测

在 xmake 中启用 ASan：

```lua
-- 方法 1：在 xmake.lua 中添加
target("my_node")
    set_kind("binary")
    add_cxflags("-fsanitize=address")
    add_ldflags("-fsanitize=address")

-- 方法 2：命令行传入
-- xmake f --cxflags="-fsanitize=address" --ldflags="-fsanitize=address"
```

ASan 相比 Valgrind 性能损失更小（约 2 倍），适合开发阶段常开。

### 3. perf + Flame Graph — CPU 热点分析

```bash
# 录制性能数据（30 秒）
perf record -g -p $(pgrep -f "talker") -- sleep 30

# 生成报告
perf report

# 生成 Flame Graph
perf script | stackcollapse-perf.pl | flamegraph.pl > flamegraph.svg
```

Flame Graph 能直观展示 CPU 时间花在哪些函数调用栈上，是定位序列化/反序列化开销的利器。

### 4. heaptrack — 内存分配追踪

```bash
# 追踪内存分配
heaptrack ros2 run demo_xmake_pubsub talker

# 分析结果
heaptrack_print heaptrack.talker.*.gz

# GUI 分析（推荐）
heaptrack_gui heaptrack.talker.*.gz
```

heaptrack 可以展示：
- 每个函数分配了多少内存
- 内存分配的调用栈
- 内存泄漏的来源

### 5. LeakSanitizer (LSan) — 快速泄漏检测

```lua
-- 在 xmake.lua 中启用
target("my_node")
    add_cxflags("-fsanitize=leak")
    add_ldflags("-fsanitize=leak")
```

LSan 比完整的 ASan 更轻量，仅在程序退出时报告泄漏。

## 实战示例：分析 pubsub 节点

```bash
# 1. 用 debug 模式构建
colcon build --packages-select demo_xmake_pubsub --cmake-args -DCMAKE_BUILD_TYPE=Debug

# 2. 运行 perf 录制
ros2 run demo_xmake_pubsub talker &
TALKER_PID=$!
perf record -g -p $TALKER_PID -- sleep 10
kill $TALKER_PID

# 3. 查看热点
perf report --sort=dso,symbol
```

## xmake 知识点小结

| 特性 | 说明 |
|------|------|
| `add_cxflags("-fsanitize=address")` | 为 target 添加编译器标志（启用 ASan） |
| `add_ldflags("-fsanitize=address")` | 为 target 添加链接器标志 |
| `mode.debug` 规则 | 启用调试信息，便于性能分析工具解析符号 |
| `xmake f --cxflags=...` | 命令行传入全局编译标志 |

## 内容扩展

> 以下资源可帮助你深入了解本章涉及的技术领域。

### 工具
- [Valgrind](https://valgrind.org/) — 内存错误检测和分析框架
- [heaptrack](https://github.com/KDE/heaptrack) — 轻量级堆内存分配分析器
- [perf](https://perf.wiki.kernel.org/) — Linux 内核性能分析工具
- [Flame Graphs](https://www.brendangregg.com/flamegraphs.html) — CPU 热点火焰图可视化方法

### 文档与教程
- [AddressSanitizer](https://clang.llvm.org/docs/AddressSanitizer.html) — Clang ASan 文档
- [LeakSanitizer](https://clang.llvm.org/docs/LeakSanitizer.html) — Clang LSan 文档
- [xmake 编译选项](https://xmake.io/#/manual/project_target?id=targetadd_cxflags) — add_cxflags 参考

## 参考

- [Valgrind Quick Start](https://valgrind.org/docs/manual/quick-start.html)
- [Brendan Gregg's perf Examples](https://www.brendangregg.com/perf.html)
- [ROS 2 Performance Best Practices](https://docs.ros.org/en/jazzy/How-To-Guides/DDS-tuning.html)
- [ament_xmake](https://github.com/ros-x/ament_xmake)
