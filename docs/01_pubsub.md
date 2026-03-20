# 01. 发布者/订阅者 — Topic 通信基础

## 概述

本示例演示 ROS 2 最基本的通信模式：**Topic 发布/订阅**。通过 `demo_xmake_pubsub` 包，你将学习如何创建 Publisher 和 Subscriber 节点，并了解 xmake 构建 ROS 2 包的最小配置。

**ROS 2 知识点**：Node、Topic、Publisher、Subscriber、spin、回调函数

**xmake 知识点**：多 target 定义、`add_ros_deps()`、`set_kind("binary")`

## ROS 2 知识

### Topic 通信模型

Topic 是 ROS 2 中最常用的通信机制，采用**发布/订阅**模式：

```
┌──────────┐    /chatter    ┌──────────┐
│  Talker  │ ──────────────>│ Listener │
│(Publisher)│   std_msgs/   │(Subscriber)
└──────────┘    String      └──────────┘
```

- **Publisher**（发布者）：向一个 Topic 发送消息，不关心是否有订阅者
- **Subscriber**（订阅者）：从一个 Topic 接收消息，通过回调函数处理
- **Topic**（话题）：消息的命名通道，如 `/chatter`
- **消息类型**：Topic 上传输的数据结构，如 `std_msgs/msg/String`

### 关键概念

- **Node**（节点）：ROS 2 的基本执行单元，每个节点通常负责一个功能
- **spin**：事件循环，驱动回调函数执行
- **WallTimer**：基于系统时钟的定时器，用于周期性发布消息

## 示例代码

### 项目结构

```
demo_xmake_pubsub/
├── xmake.lua          # 构建配置：两个 binary target
├── package.xml        # ROS 包声明
└── src/
    ├── talker.cpp     # 发布者节点
    └── listener.cpp   # 订阅者节点
```

### xmake.lua 构建配置

```lua
set_project("demo_xmake_pubsub")
set_version("0.1.0")
set_languages("cxx17")

add_rules("mode.debug", "mode.release")
add_rules("ament_xmake.package")

target("talker")
    set_kind("binary")
    set_installdir("lib/demo_xmake_pubsub")
    add_files("src/talker.cpp")
    add_ros_deps("rclcpp", "std_msgs")

target("listener")
    set_kind("binary")
    set_installdir("lib/demo_xmake_pubsub")
    add_files("src/listener.cpp")
    add_ros_deps("rclcpp", "std_msgs")
```

**逐行解读**：

| 语句 | 说明 |
|------|------|
| `set_project("demo_xmake_pubsub")` | 项目名，需与 package.xml 中的 `<name>` 一致 |
| `set_languages("cxx17")` | ROS 2 Jazzy 要求 C++17 |
| `add_rules("ament_xmake.package")` | 启用 ament_xmake 规则，自动处理安装和导出 |
| `set_kind("binary")` | 构建可执行文件 |
| `set_installdir("lib/demo_xmake_pubsub")` | ROS 2 约定：可执行文件安装到 `lib/<pkg>/` |
| `add_ros_deps("rclcpp", "std_msgs")` | 添加 ROS 包依赖，自动解析头文件路径、链接库和 rpath |

**与 CMakeLists.txt 对比**：

```cmake
# CMake 版本（约 15 行）
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
add_executable(talker src/talker.cpp)
ament_target_dependencies(talker rclcpp std_msgs)
add_executable(listener src/listener.cpp)
ament_target_dependencies(listener rclcpp std_msgs)
install(TARGETS talker listener DESTINATION lib/${PROJECT_NAME})
ament_package()
```

xmake 版本更简洁，`add_ros_deps()` 一行替代了 `find_package` + `ament_target_dependencies` 的组合。

### 源码解读

**talker.cpp** — 发布者节点：

```cpp
class Talker : public rclcpp::Node
{
public:
  Talker() : Node("talker"), count_(0)
  {
    // 创建发布者：话题名 "chatter"，队列深度 10
    publisher_ = this->create_publisher<std_msgs::msg::String>("chatter", 10);
    // 创建定时器：每 500ms 触发一次回调
    timer_ = this->create_wall_timer(
      std::chrono::milliseconds(500),
      std::bind(&Talker::on_timer, this));
  }

private:
  void on_timer()
  {
    auto msg = std_msgs::msg::String();
    msg.data = "Hello xmake #" + std::to_string(count_++);
    publisher_->publish(msg);  // 发布消息
  }
};
```

**listener.cpp** — 订阅者节点：

```cpp
class Listener : public rclcpp::Node
{
public:
  Listener() : Node("listener")
  {
    // 创建订阅者：话题名 "chatter"，队列深度 10，绑定回调
    subscription_ = this->create_subscription<std_msgs::msg::String>(
      "chatter", 10,
      std::bind(&Listener::on_message, this, std::placeholders::_1));
  }

private:
  void on_message(const std_msgs::msg::String & msg)
  {
    RCLCPP_INFO(this->get_logger(), "Received: '%s'", msg.data.c_str());
  }
};
```

## 构建与运行

```bash
# 构建
colcon build --packages-select demo_xmake_pubsub
source install/setup.bash

# 终端 1：启动发布者
ros2 run demo_xmake_pubsub talker

# 终端 2：启动订阅者
ros2 run demo_xmake_pubsub listener
```

## 验证

```bash
# 查看活跃话题
ros2 topic list
# 输出应包含: /chatter

# 查看话题消息
ros2 topic echo /chatter
# 输出: data: "Hello xmake #0" ...

# 查看话题信息
ros2 topic info /chatter
# 输出: Type: std_msgs/msg/String, Publisher count: 1, Subscription count: 1
```

## xmake 知识点小结

| 特性 | 说明 |
|------|------|
| `set_kind("binary")` | 构建可执行文件（对应 CMake 的 `add_executable`） |
| `add_ros_deps()` | 一站式 ROS 依赖解析：头文件、链接库、rpath |
| 多 target 定义 | 一个 xmake.lua 中定义多个构建目标 |
| `set_installdir()` | 指定安装路径（ROS 约定 `lib/<pkg>/`） |

## 内容扩展

> 以下资源可帮助你深入了解本章涉及的技术领域。

### 开源项目
- [micro-ROS](https://micro.ros.org/) — 嵌入式设备上的 ROS 2 pub/sub
- [zenoh-plugin-ros2dds](https://github.com/eclipse-zenoh/zenoh-plugin-ros2dds) — Zenoh 桥接 ROS 2 Topic

### 工具
- [PlotJuggler](https://github.com/facontidavide/PlotJuggler) — 实时话题数据可视化
- [rqt_graph](https://docs.ros.org/en/jazzy/Concepts/Intermediate/About-RQt.html) — 节点/话题关系图
- [Foxglove Studio](https://foxglove.dev/) — 浏览器端 ROS 可视化

### 文档与教程
- [ROS 2 Design - Topics](https://design.ros2.org/articles/topic_and_service_names.html) — Topic 命名规范设计文档

## 参考

- [ROS 2 官方文档 - Writing a simple publisher and subscriber (C++)](https://docs.ros.org/en/jazzy/Tutorials/Beginner-Client-Libraries/Writing-A-Simple-Cpp-Publisher-And-Subscriber.html)
- [ament_xmake](https://github.com/ros-x/ament_xmake)
- [xmake 官方文档](https://xmake.io/)
