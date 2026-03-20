# 25. 性能追踪与分析 — ros2_tracing

## 概述

本示例演示如何使用 ros2_tracing 框架对 ROS 2 系统进行性能追踪。通过 [`demo_xmake_tracing`](../demo_xmake_tracing) 包中的多阶段处理管线，你将学习如何录制追踪数据、分析回调延迟和调度抖动。

**ROS 2 知识点**：ros2_tracing（LTTng）、回调延迟分析、tracetools、Trace Compass

**xmake 知识点**：launch 目录自动安装、可选依赖处理

## ROS 2 知识

### ros2_tracing 架构

ros2_tracing 基于 LTTng（Linux Trace Toolkit: next generation），在 ROS 2 的关键路径中插入追踪点（tracepoints）：

```
┌─────────────┐     ┌──────────────┐     ┌──────────────┐
│  rclcpp     │     │  rmw         │     │  DDS         │
│  tracepoints│────>│  tracepoints │────>│  transport   │
└─────────────┘     └──────────────┘     └──────────────┘
       │                    │
       v                    v
  ┌─────────────────────────────┐
  │     LTTng 追踪会话          │
  │  (内核态 + 用户态追踪点)     │
  └─────────────────────────────┘
       │
       v
  ┌─────────────────────────────┐
  │  CTF 追踪文件               │
  │  → Trace Compass 可视化     │
  │  → tracetools_analysis     │
  └─────────────────────────────┘
```

### 追踪点类型

ros2_tracing 提供的关键追踪点：

| 追踪点 | 说明 |
|--------|------|
| `ros2:callback_start` | 回调函数开始执行 |
| `ros2:callback_end` | 回调函数执行结束 |
| `ros2:rclcpp_publish` | 消息发布 |
| `ros2:rclcpp_subscription_callback_added` | 订阅回调注册 |
| `ros2:rcl_timer_init` | 定时器初始化 |

### 回调延迟分析

通过追踪数据可以分析：

- **回调执行时间**：`callback_end - callback_start`
- **调度延迟**：从消息到达到回调开始的时间
- **端到端延迟**：从发布到最终处理完成的总时间
- **调度抖动**：定时器回调的实际触发间隔与期望间隔的差异

## 示例代码

### 项目结构

```
demo_xmake_tracing/
├── xmake.lua
├── package.xml         # deps: rclcpp, std_msgs
├── launch/
│   └── tracing_demo.launch.py  # 带追踪的 launch 配置
└── src/
    └── traced_pipeline.cpp  # 多阶段处理管线
```

### xmake.lua 构建配置

```lua
set_project("demo_xmake_tracing")
set_version("0.1.0")
set_languages("cxx17")

add_rules("mode.debug", "mode.release")
add_rules("ament_xmake.package")

target("traced_pipeline")
    set_kind("binary")
    set_installdir("lib/demo_xmake_tracing")
    add_files("src/traced_pipeline.cpp")
    add_ros_deps("rclcpp", "std_msgs")
```

`launch/` 目录会被 ament_xmake 自动安装到 `share/<pkg>/launch/`，无需额外配置。

### 源码要点

**多阶段管线**：

```cpp
// Stage 1: 数据生成（10 Hz）
timer_ = this->create_wall_timer(100ms, [this]() {
    auto msg = std_msgs::msg::String();
    msg.data = "frame_" + std::to_string(frame_id_++);
    pub_stage1_->publish(msg);
});

// Stage 2: 模拟处理（引入 5ms 延迟）
sub_stage1_ = this->create_subscription<std_msgs::msg::String>(
    "/pipeline/stage1", 10, [this](const std_msgs::msg::String::SharedPtr msg) {
        std::this_thread::sleep_for(5ms);  // 模拟计算
        pub_stage2_->publish(*msg);
    });

// Stage 3: 最终处理
sub_stage2_ = this->create_subscription<std_msgs::msg::String>(
    "/pipeline/stage2", 10, [this](const std_msgs::msg::String::SharedPtr msg) {
        std::this_thread::sleep_for(3ms);  // 模拟计算
        RCLCPP_INFO(this->get_logger(), "Pipeline complete: %s", msg->data.c_str());
    });
```

这种多阶段模式在追踪分析中特别有用，可以清楚看到每个阶段的延迟贡献。

## 构建与运行

```bash
# 构建
colcon build --packages-select demo_xmake_tracing
source install/setup.bash

# 直接运行
ros2 run demo_xmake_tracing traced_pipeline

# 或通过 launch 文件
ros2 launch demo_xmake_tracing tracing_demo.launch.py
```

### 使用 ros2_tracing 录制追踪

```bash
# 安装 ros2_tracing（如未安装）
sudo apt install ros-jazzy-tracetools ros-jazzy-ros2trace

# 开始追踪录制
ros2 trace --session-name demo_trace --path /tmp/demo_trace

# 在另一个终端运行节点
ros2 run demo_xmake_tracing traced_pipeline

# 停止追踪（Ctrl+C）
# 追踪数据保存在 /tmp/demo_trace/
```

## 验证

```bash
# 确认节点运行
ros2 topic hz /pipeline/stage1
# 期望: average rate: ~10 Hz

ros2 topic hz /pipeline/stage2
# 期望: average rate: ~10 Hz（受 stage2 处理时间影响）

# 查看追踪数据（需安装 babeltrace2）
babeltrace2 /tmp/demo_trace/
```

### 使用 Trace Compass 可视化

1. 下载 [Trace Compass](https://www.eclipse.org/tracecompass/)
2. 导入 CTF 追踪文件
3. 使用 ros2_tracing 分析视图查看回调时序

## xmake 知识点小结

| 特性 | 说明 |
|------|------|
| launch/ 目录自动安装 | ament_xmake 约定：launch/ 目录自动安装到 share/ |
| `add_ros_deps()` 按需依赖 | 只依赖运行时必需的包，tracetools 由 rclcpp 间接引入 |

## 内容扩展

> 以下资源可帮助你深入了解本章涉及的技术领域。

### 开源项目
- [ros2_tracing](https://github.com/ros2/ros2_tracing) — ROS 2 官方追踪框架
- [ros2_tracing_analysis](https://gitlab.com/ros-tracing/tracetools_analysis) — 追踪数据分析工具

### 工具
- [Trace Compass](https://www.eclipse.org/tracecompass/) — Eclipse 追踪可视化工具
- [LTTng](https://lttng.org/) — Linux 追踪工具包（ros2_tracing 底层依赖）
- [perf](https://perf.wiki.kernel.org/) — Linux 性能分析工具（可与追踪配合）

### 文档与教程
- [ros2_tracing 使用教程](https://github.com/ros2/ros2_tracing#tracing) — 官方使用说明

### 论文与技术报告
- C. Bédard et al., "ros2_tracing: Multipurpose Low-Overhead Framework for Real-Time Tracing of ROS 2", IEEE RA-L, 2022 — ros2_tracing 架构与性能评估

## 参考

- [ros2_tracing GitHub](https://github.com/ros2/ros2_tracing)
- [LTTng Documentation](https://lttng.org/docs/)
- [Trace Compass User Guide](https://archive.eclipse.org/tracecompass/doc/stable/org.eclipse.tracecompass.doc.user/User-Guide.html)
- [ament_xmake](https://github.com/ros-x/ament_xmake)
