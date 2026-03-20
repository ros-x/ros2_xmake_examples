---
sidebar_position: 2
title: "02. 服务通信 — Client/Server 模型"
---
# 02. 服务通信 — Client/Server 模型

## 概述

本示例演示 ROS 2 的**服务通信**模式。与 Topic 的发布/订阅不同，Service 是**请求/响应**模式，适用于需要返回结果的操作。

**ROS 2 知识点**：Service、Client/Server 模型、同步 vs 异步调用、`example_interfaces`

**xmake 知识点**：使用系统 ROS 包中的 srv 类型

## ROS 2 知识

### Service 通信模型

```
┌──────────┐   Request    ┌──────────┐
│  Client  │ ────────────>│  Server  │
│          │              │          │
│          │<────────────│          │
└──────────┘   Response   └──────────┘
```

| 特性 | Topic | Service |
|------|-------|---------|
| 模式 | 发布/订阅（1:N） | 请求/响应（1:1） |
| 同步性 | 异步 | 同步或异步 |
| 适用场景 | 连续数据流（传感器、状态） | 一次性操作（计算、配置） |
| 超时 | 无 | 可设置等待超时 |

### AddTwoInts 服务

本示例使用 `example_interfaces/srv/AddTwoInts`：

```
# Request
int64 a
int64 b
---
# Response
int64 sum
```

## 示例代码

### xmake.lua 构建配置

```lua
target("add_two_ints_server")
    set_kind("binary")
    set_installdir("lib/demo_xmake_service")
    add_files("src/add_two_ints_server.cpp")
    add_ros_deps("rclcpp", "example_interfaces")
```

`example_interfaces` 是 ROS 2 提供的标准接口包，包含常用的 srv/msg 定义。`add_ros_deps()` 会自动解析该包的头文件路径和链接库。

### 源码解读

**Server** — 处理请求：

```cpp
service_ = this->create_service<example_interfaces::srv::AddTwoInts>(
  "add_two_ints",
  std::bind(&AddTwoIntsServer::handle_request, this, _1, _2));

void handle_request(
  const std::shared_ptr<AddTwoInts::Request> request,
  std::shared_ptr<AddTwoInts::Response> response)
{
  response->sum = request->a + request->b;
}
```

**Client** — 发送请求（异步）：

```cpp
auto future = client_->async_send_request(request);
if (rclcpp::spin_until_future_complete(node, future) == SUCCESS) {
  auto result = future.get();
}
```

## 构建与运行

```bash
colcon build --packages-select demo_xmake_service
source install/setup.bash

# 终端 1：启动服务端
ros2 run demo_xmake_service add_two_ints_server

# 终端 2：启动客户端
ros2 run demo_xmake_service add_two_ints_client

# 或使用 CLI 直接调用
ros2 service call /add_two_ints example_interfaces/srv/AddTwoInts "{a: 5, b: 3}"
```

## 验证

```bash
# 查看可用服务
ros2 service list
# 输出应包含: /add_two_ints

# 查看服务类型
ros2 service type /add_two_ints
# 输出: example_interfaces/srv/AddTwoInts
```

## xmake 知识点小结

本示例没有引入新的 xmake 特性，重点在于展示 `add_ros_deps()` 对标准 ROS 接口包的支持。

## 内容扩展

> 以下资源可帮助你深入了解本章涉及的技术领域。

### 开源项目
- [rosbridge_suite](https://github.com/RobotWebTools/rosbridge_suite) — WebSocket 桥接 Service 调用

### 工具
- [rqt_service_caller](https://docs.ros.org/en/jazzy/Concepts/Intermediate/About-RQt.html) — GUI 方式调用 Service

### 文档与教程
- [ROS 2 Design - Services](https://design.ros2.org/articles/ros_on_dds.html) — Service 在 DDS 上的实现

## 参考

- [ROS 2 官方文档 - Writing a simple service and client (C++)](https://docs.ros.org/en/jazzy/Tutorials/Beginner-Client-Libraries/Writing-A-Simple-Cpp-Service-And-Client.html)
- [example_interfaces 包](https://index.ros.org/p/example_interfaces/)
