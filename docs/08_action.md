# 08. Action 通信 — Goal/Feedback/Result

## 概述

本示例通过 `demo_xmake_action` 包，演示 ROS 2 的 **Action** 通信模式。Action 是对 Service 的扩展，支持长时间运行的任务，并提供中间反馈和取消机制。

**ROS 2 知识点**：Action、Goal/Feedback/Result 生命周期、ActionServer、ActionClient、cancel

**xmake 知识点**：跨包依赖自定义 action 类型

## ROS 2 知识

### Topic vs Service vs Action

| 特性 | Topic | Service | Action |
|------|-------|---------|--------|
| 模式 | 发布/订阅 | 请求/响应 | 目标/反馈/结果 |
| 同步性 | 异步 | 同步/异步 | 异步 |
| 反馈 | 无 | 无 | 有 |
| 取消 | 无 | 无 | 有 |
| 适用场景 | 连续数据流 | 快速操作 | 长时间任务 |

### Action 生命周期

```
Client                          Server
  │                                │
  │──── Send Goal ────────────────>│
  │<─── Goal Response ────────────│
  │                                │ (执行中)
  │<─── Feedback ─────────────────│
  │<─── Feedback ─────────────────│
  │<─── Feedback ─────────────────│
  │<─── Result ───────────────────│
```

Action 内部基于 Topic 和 Service 实现：
- Goal Service：发送目标
- Result Service：获取结果
- Feedback Topic：接收反馈
- Status Topic：目标状态

## 示例代码

### xmake.lua 构建配置

```lua
target("countdown_server")
    set_kind("binary")
    set_installdir("lib/demo_xmake_action")
    add_files("src/countdown_server.cpp")
    add_ros_deps("rclcpp", "rclcpp_action", "demo_xmake_interfaces")
```

注意 `demo_xmake_interfaces` 是跨包依赖 — 使用的 `Countdown.action` 定义在另一个包中。`add_ros_deps()` 会自动解析跨包依赖关系。

### 源码解读

**ActionServer** 需要处理三个回调：

```cpp
action_server_ = rclcpp_action::create_server<Countdown>(
  this, "countdown",
  handle_goal,     // 决定是否接受目标
  handle_cancel,   // 处理取消请求
  handle_accepted  // 目标被接受后执行
);
```

**ActionClient** 通过 `SendGoalOptions` 设置回调：

```cpp
auto options = rclcpp_action::Client<Countdown>::SendGoalOptions();
options.goal_response_callback = ...;  // 目标是否被接受
options.feedback_callback = ...;       // 接收反馈
options.result_callback = ...;         // 接收结果
```

## 构建与运行

```bash
# 先构建接口包
colcon build --packages-select demo_xmake_interfaces
# 再构建 action 包
colcon build --packages-select demo_xmake_action
source install/setup.bash

# 终端 1：启动 Action 服务端
ros2 run demo_xmake_action countdown_server

# 终端 2：启动 Action 客户端
ros2 run demo_xmake_action countdown_client
```

## 验证

```bash
# 查看 action 列表
ros2 action list
# 输出: /countdown

# 查看 action 信息
ros2 action info /countdown

# 通过 CLI 发送 goal
ros2 action send_goal /countdown demo_xmake_interfaces/action/Countdown \
  "{target_number: 3}" --feedback
```

## xmake 知识点小结

| 特性 | 说明 |
|------|------|
| 跨包依赖 | `add_ros_deps("demo_xmake_interfaces")` 引用其他包的接口 |
| `rclcpp_action` 依赖 | Action 需要额外的 `rclcpp_action` 包 |

## 内容扩展

> 以下资源可帮助你深入了解本章涉及的技术领域。

### 开源项目
- [BehaviorTree.CPP](https://github.com/BehaviorTree/BehaviorTree.CPP) — 行为树框架（常与 Action 配合）
- [nav2_behavior_tree](https://github.com/ros-navigation/navigation2/tree/main/nav2_behavior_tree) — Nav2 行为树节点

### 工具
- [Groot2](https://www.behaviortree.dev/groot/) — 行为树可视化编辑器

### 文档与教程
- [ROS 2 Action 设计](https://design.ros2.org/articles/actions.html)

## 参考

- [ROS 2 官方文档 - Writing an action server and client (C++)](https://docs.ros.org/en/jazzy/Tutorials/Intermediate/Writing-an-Action-Server-Client/Cpp.html)
- [ROS 2 Action 设计](https://design.ros2.org/articles/actions.html)
