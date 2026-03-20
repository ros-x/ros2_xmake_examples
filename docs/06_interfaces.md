# 06. 自定义接口 — msg/srv/action 定义与 rosidl

## 概述

本示例通过 `demo_xmake_interfaces` 包，演示如何定义和构建自定义的 ROS 2 消息（msg）、服务（srv）和动作（action）接口。

**ROS 2 知识点**：rosidl 代码生成管线、.msg/.srv/.action 定义语法

**xmake 知识点**：rosidl 与 colcon-xmake 的集成、phony target

## ROS 2 知识

### 接口定义语法

**消息 (.msg)**：
```
# 字段类型 字段名
uint32 id
string name
float64 value
```

**服务 (.srv)**：
```
# 请求
string input
---
# 响应
bool success
string output
```

**动作 (.action)**：
```
# Goal（目标）
int32 target_number
---
# Result（结果）
bool success
---
# Feedback（反馈）
int32 current_number
```

### rosidl 管线

rosidl 是 ROS 2 的接口代码生成工具链：

```
.msg/.srv/.action 文件
       ↓ rosidl_generate_interfaces()
C/C++ 源码（类型支持）
       ↓ 编译
7 个共享库（C、C++、typesupport 等）
```

## 示例代码

### 项目结构

```
demo_xmake_interfaces/
├── msg/
│   └── MyMsg.msg
├── srv/
│   └── MySrv.srv
├── action/
│   └── Countdown.action
├── xmake.lua
└── package.xml
```

### xmake.lua 构建配置

```lua
set_project("demo_xmake_interfaces")
set_version("0.1.0")
set_languages("cxx17")

add_rules("mode.debug", "mode.release")
add_rules("ament_xmake.package")

-- rosidl targets 由 colcon-xmake rosidl 管线自动生成注入
-- 无需显式 target 定义
target("demo_xmake_interfaces_marker")
    set_kind("phony")
```

**rosidl 与 xmake 的协作**：colcon-xmake 会检测 `package.xml` 中的 `rosidl_default_generators` 依赖和 `rosidl_interface_packages` 组成员关系，自动调用 rosidl 管线生成代码并注入到构建系统中。

### package.xml 关键配置

```xml
<buildtool_depend>rosidl_default_generators</buildtool_depend>
<depend>builtin_interfaces</depend>
<depend>rosidl_default_runtime</depend>
<member_of_group>rosidl_interface_packages</member_of_group>
```

| 配置 | 说明 |
|------|------|
| `rosidl_default_generators` | 构建时依赖 rosidl 代码生成器 |
| `rosidl_default_runtime` | 运行时依赖 rosidl 类型支持 |
| `rosidl_interface_packages` | 声明为接口包，使其被自动发现 |

## 构建与运行

```bash
colcon build --packages-select demo_xmake_interfaces
source install/setup.bash

# 查看生成的接口
ros2 interface show demo_xmake_interfaces/msg/MyMsg
ros2 interface show demo_xmake_interfaces/srv/MySrv
ros2 interface show demo_xmake_interfaces/action/Countdown
```

## 验证

```bash
# 验证 rosidl 生成的 7 个库
ls install/demo_xmake_interfaces/lib/libdemo_xmake_interfaces__*.so | wc -l
# 输出: 7

# 验证接口可被发现
ros2 interface list | grep demo_xmake
```

## xmake 知识点小结

| 特性 | 说明 |
|------|------|
| rosidl 集成 | colcon-xmake 自动处理 rosidl 代码生成 |
| phony target | 纯接口包用 phony target 触发安装规则 |

## 内容扩展

> 以下资源可帮助你深入了解本章涉及的技术领域。

### 开源项目
- [rosidl](https://github.com/ros2/rosidl) — ROS 接口生成器核心
- [common_interfaces](https://github.com/ros2/common_interfaces) — 标准消息定义参考

### 工具
- [ros2 interface CLI](https://docs.ros.org/en/jazzy/Tutorials/Beginner-CLI-Tools/Understanding-ROS2-Interfaces/Understanding-ROS2-Interfaces.html) — 接口查看工具

### 文档与教程
- [rosidl 类型支持设计](https://design.ros2.org/articles/rosidl_typesupport.html)
- [IDL 4.2 规范 (OMG)](https://www.omg.org/spec/IDL/4.2/)

## 参考

- [ROS 2 官方文档 - Creating custom msg and srv files](https://docs.ros.org/en/jazzy/Tutorials/Beginner-Client-Libraries/Custom-ROS2-Interfaces.html)
- [rosidl 设计文档](https://design.ros2.org/articles/rosidl_typesupport.html)
