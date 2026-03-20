# 04. 库构建 — 静态库/共享库/下游消费

## 概述

本示例通过 [`demo_xmake_cpp`](../demo_xmake_cpp) 包，演示如何用 xmake 构建 ROS 2 中的**静态库和共享库**，并让下游包通过 `find_package()` 消费。

**ROS 2 知识点**：geometry_msgs::Twist、turtlesim 仿真

**xmake 知识点**：`set_kind("static")`/`set_kind("shared")`、`add_headerfiles`、public include 路径、`{visibility = "public"}`

## ROS 2 知识

### geometry_msgs::Twist

`geometry_msgs::Twist` 是机器人运动控制的核心消息类型：

```
Vector3 linear   # 线速度 (x, y, z)
Vector3 angular  # 角速度 (x, y, z)
```

在 turtlesim 中，发布 Twist 到 `/turtle1/cmd_vel` 可控制海龟运动。

## 示例代码

### xmake.lua 构建配置

```lua
-- 静态库
target("demo_xmake_cpp")
    set_kind("static")
    add_files("src/demo_lib.cpp")
    add_includedirs("include", {public = true})
    add_headerfiles("include/(demo_xmake_cpp/*.hpp)")

-- 共享库（同一源码）
target("demo_xmake_cpp_shared")
    set_kind("shared")
    add_files("src/demo_lib.cpp")
    add_includedirs("include", {public = true})
    add_headerfiles("include/(demo_xmake_cpp/*.hpp)")
    set_basename("demo_xmake_cpp")

-- 使用 ROS 依赖的可执行文件
target("turtle_circle_node")
    set_kind("binary")
    set_installdir("lib/demo_xmake_cpp")
    add_files("src/turtle_circle_node.cpp")
    add_ros_deps("rclcpp", "geometry_msgs")
```

**关键 API 解读**：

| API | 说明 |
|-----|------|
| `set_kind("static")` | 构建静态库（.a） |
| `set_kind("shared")` | 构建共享库（.so） |
| `add_includedirs("include", {public = true})` | 公开头文件路径，下游 target 自动继承 |
| `add_headerfiles("include/(xxx/*.hpp)")` | 声明要安装的头文件，括号内为相对安装路径 |
| `set_basename("demo_xmake_cpp")` | 设置输出文件名（不含 lib 前缀和扩展名） |

### CMake Config 自动生成

ament_xmake 会自动为主 target（与包同名的 target）生成 CMake Config 文件，使下游包可以：

```cmake
find_package(demo_xmake_cpp REQUIRED CONFIG)
target_link_libraries(consumer PRIVATE demo_xmake_cpp::demo_xmake_cpp)
```

### add_ros_deps 的 visibility 选项

当构建库时，如果下游包也需要使用你的 ROS 依赖，应使用 `{visibility = "public"}`：

```lua
add_ros_deps("rclcpp", "geometry_msgs", {visibility = "public"})
```

这会将依赖的头文件路径和链接库传递给下游 target。

## 构建与运行

```bash
colcon build --packages-select demo_xmake_cpp
source install/setup.bash

# 运行基础节点
ros2 run demo_xmake_cpp demo_xmake_cpp_node

# Turtlesim 演示（需要两个终端）
ros2 run turtlesim turtlesim_node          # 终端 1
ros2 run demo_xmake_cpp turtle_circle_node # 终端 2
```

## 验证

```bash
# 验证静态库和共享库都已安装
ls install/demo_xmake_cpp/lib/
# 应包含: libdemo_xmake_cpp.a  libdemo_xmake_cpp.so

# 验证头文件已安装
ls install/demo_xmake_cpp/include/demo_xmake_cpp/
# 应包含: demo_lib.hpp

# 验证 CMake Config
ls install/demo_xmake_cpp/share/demo_xmake_cpp/cmake/
# 应包含: demo_xmake_cppConfig.cmake
```

## xmake 知识点小结

| 特性 | 说明 |
|------|------|
| `set_kind("static")` | 构建静态库 |
| `set_kind("shared")` | 构建共享库 |
| `add_headerfiles()` | 声明安装的头文件 |
| `add_includedirs({public = true})` | 公开 include 路径 |
| `set_basename()` | 自定义输出文件名 |
| CMake Config 自动生成 | 主 target 自动导出为 CMake imported target |

## 内容扩展

> 以下资源可帮助你深入了解本章涉及的技术领域。

### 开源项目
- [ament_cmake](https://github.com/ament/ament_cmake) — 标准 ament 构建工具（对比参考）

### 文档与教程
- [xmake target API](https://xmake.io/#/manual/project_target) — xmake target 配置完整参考
- [CMake Modern Best Practices](https://cliutils.gitlab.io/modern-cmake/) — 理解 xmake 自动生成的 CMake Config
- [Creating and Using Libraries (ROS 2)](https://docs.ros.org/en/jazzy/Tutorials/Beginner-Client-Libraries/Creating-A-Workspace/Creating-A-Workspace.html)

## 参考

- [ROS 2 官方文档 - Creating a workspace](https://docs.ros.org/en/jazzy/Tutorials/Beginner-Client-Libraries/Creating-A-Workspace/Creating-A-Workspace.html)
- [ament_xmake RULE_SPEC](https://github.com/ros-x/ament_xmake/blob/main/docs/RULE_SPEC.md)
