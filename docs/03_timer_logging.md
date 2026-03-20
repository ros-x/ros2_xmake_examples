# 03. 定时器与日志系统

## 概述

本示例演示 ROS 2 的**定时器**和**日志系统**。定时器用于周期性任务，日志系统提供分级输出，是调试和监控的核心工具。

**ROS 2 知识点**：Timer、WallTimer、日志级别 (DEBUG/INFO/WARN/ERROR)

**xmake 知识点**：单文件最小包、单 target 极简配置

## ROS 2 知识

### 定时器

ROS 2 提供两种定时器：
- **WallTimer**：基于系统时钟，不受仿真时间影响
- **Timer**：基于 ROS 时钟，可被仿真时间控制

```cpp
// 创建 500ms 间隔的 WallTimer
timer_ = this->create_wall_timer(500ms, callback);
```

### 日志系统

ROS 2 日志有四个级别（从低到高）：

| 级别 | 宏 | 用途 |
|------|-----|------|
| DEBUG | `RCLCPP_DEBUG` | 详细调试信息，默认不显示 |
| INFO | `RCLCPP_INFO` | 常规运行信息 |
| WARN | `RCLCPP_WARN` | 警告，不影响功能但需注意 |
| ERROR | `RCLCPP_ERROR` | 错误，功能受影响 |

运行时可通过 `--ros-args --log-level DEBUG` 调整日志级别。

## 示例代码

### xmake.lua 构建配置

```lua
set_project("demo_xmake_timer")
set_version("0.1.0")
set_languages("cxx17")

add_rules("mode.debug", "mode.release")
add_rules("ament_xmake.package")

target("timer_demo")
    set_kind("binary")
    set_installdir("lib/demo_xmake_timer")
    add_files("src/timer_demo.cpp")
    add_ros_deps("rclcpp")
```

这是**最简单的 xmake 配置**：单个 target、单个源文件、单个 ROS 依赖。

### 源码解读

```cpp
class TimerDemo : public rclcpp::Node
{
public:
  TimerDemo() : Node("timer_demo")
  {
    // 快速定时器 — 500ms
    fast_timer_ = this->create_wall_timer(
      500ms, std::bind(&TimerDemo::fast_callback, this));

    // 慢速定时器 — 2s
    slow_timer_ = this->create_wall_timer(
      2s, std::bind(&TimerDemo::slow_callback, this));

    RCLCPP_INFO(this->get_logger(), "TimerDemo started");
    RCLCPP_DEBUG(this->get_logger(), "Debug: fast=500ms, slow=2s");
  }
};
```

两个定时器独立运行，展示了同一节点中多定时器的并发行为。

## 构建与运行

```bash
colcon build --packages-select demo_xmake_timer
source install/setup.bash

# 正常运行
ros2 run demo_xmake_timer timer_demo

# 启用 DEBUG 日志
ros2 run demo_xmake_timer timer_demo --ros-args --log-level DEBUG
```

## 验证

运行后应看到交替输出的快速和慢速定时器日志：

```
[INFO] [timer_demo]: TimerDemo started with two timers
[INFO] [timer_demo]: Fast timer #1
[INFO] [timer_demo]: Fast timer #2
[INFO] [timer_demo]: Fast timer #3
[INFO] [timer_demo]: Fast timer #4
[INFO] [timer_demo]: Slow timer #1
[WARN] [timer_demo]: Fast timer reached 5 ticks
```

程序会在慢速定时器第 3 次触发后自动退出。

## xmake 知识点小结

| 特性 | 说明 |
|------|------|
| 单 target 配置 | 最小可用的 xmake.lua 结构 |
| 单依赖 | 只需 `add_ros_deps("rclcpp")`，无消息类型依赖 |

## 内容扩展

> 以下资源可帮助你深入了解本章涉及的技术领域。

### 开源项目
- [ros2_logging](https://github.com/ros2/rcl_logging) — ROS 2 日志后端实现

### 工具
- [ros2 bag + grep](https://docs.ros.org/en/jazzy/Tutorials/Beginner-CLI-Tools/Recording-And-Playing-Back-Data/Recording-And-Playing-Back-Data.html) — 用 rosbag 录制日志话题
- [rqt_console](https://docs.ros.org/en/jazzy/Tutorials/Beginner-CLI-Tools/Using-Rqt-Console/Using-Rqt-Console.html) — GUI 日志查看过滤器

### 文档与教程
- [spdlog](https://github.com/gabime/spdlog) — ROS 2 底层使用的 C++ 日志库
- [ROS 2 日志最佳实践](https://docs.ros.org/en/jazzy/Concepts/Intermediate/About-Logging.html)

## 参考

- [ROS 2 官方文档 - Using parameters in a class (C++)](https://docs.ros.org/en/jazzy/Tutorials/Beginner-Client-Libraries/Using-Parameters-In-A-Class-CPP.html)
- [ROS 2 Logging](https://docs.ros.org/en/jazzy/Concepts/Intermediate/About-Logging.html)
