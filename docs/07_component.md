# 07. 组件插件 — Composable Node 与 pluginlib

## 概述

本示例通过 [`demo_xmake_component`](../demo_xmake_component) 包，演示如何构建 ROS 2 **Composable Node**（组件节点），并使用 `install_ros_plugin()` 注册插件。

**ROS 2 知识点**：Composable Node、Component Container、RCLCPP_COMPONENTS_REGISTER_NODE、零拷贝 intra-process

**xmake 知识点**：`install_ros_plugin()`、plugin_description.xml、共享库作为插件

## ROS 2 知识

### 组件 vs 独立节点

| 特性 | 独立节点 | 组件节点 |
|------|---------|---------|
| 进程 | 独立进程 | 共享进程（Container） |
| 通信 | 序列化/反序列化 | 可零拷贝（intra-process） |
| 性能 | 进程间通信开销 | 低延迟、低内存 |
| 部署 | ros2 run | ros2 component load |

### 组件注册

通过 `RCLCPP_COMPONENTS_REGISTER_NODE` 宏将节点注册为可动态加载的插件：

```cpp
RCLCPP_COMPONENTS_REGISTER_NODE(demo_xmake_component::TalkerComponent)
```

组件节点必须：
1. 构造函数接受 `const rclcpp::NodeOptions &` 参数
2. 编译为**共享库**
3. 提供 `plugin_description.xml` 描述插件信息

## 示例代码

### xmake.lua 构建配置

```lua
-- 注册插件描述文件
install_ros_plugin("plugin_description.xml", "rclcpp_components")

target("demo_xmake_component")
    set_kind("shared")  -- 组件必须是共享库
    add_files("src/talker_component.cpp")
    add_ros_deps("rclcpp", "rclcpp_components", "std_msgs")
```

**`install_ros_plugin()` 详解**：

```lua
install_ros_plugin("plugin_description.xml", "rclcpp_components")
--                  ↑ 插件描述文件              ↑ 基础包名（pluginlib 索引键）
```

该函数做两件事：
1. 安装 `plugin_description.xml` 到 `share/<pkg>/`
2. 在 ament index 中注册 `rclcpp_components__pluginlib__plugin` 索引

### plugin_description.xml

```xml
<library path="demo_xmake_component">
  <class type="demo_xmake_component::TalkerComponent"
         base_class_type="rclcpp_components::NodeFactory">
    <description>A demo talker component</description>
  </class>
</library>
```

## 构建与运行

```bash
colcon build --packages-select demo_xmake_component
source install/setup.bash

# 启动组件容器
ros2 run rclcpp_components component_container

# 在另一个终端加载组件
ros2 component load /ComponentManager demo_xmake_component \
  demo_xmake_component::TalkerComponent

# 查看已加载的组件
ros2 component list
```

## 验证

```bash
# 验证插件描述文件已安装
ls install/demo_xmake_component/share/demo_xmake_component/plugin_description.xml

# 验证 pluginlib 索引已注册
ls install/demo_xmake_component/share/ament_index/resource_index/rclcpp_components__pluginlib__plugin/

# 验证共享库已安装
ls install/demo_xmake_component/lib/libdemo_xmake_component.so
```

## xmake 知识点小结

| 特性 | 说明 |
|------|------|
| `install_ros_plugin()` | 注册 pluginlib 插件并创建 ament index 索引 |
| `set_kind("shared")` | 组件必须编译为共享库 |

## 内容扩展

> 以下资源可帮助你深入了解本章涉及的技术领域。

### 开源项目
- [rclcpp_components](https://github.com/ros2/rclcpp/tree/rolling/rclcpp_components) — 组件容器实现
- [type_adaptation](https://github.com/ros2/rclcpp/tree/rolling/rclcpp/include/rclcpp) — 类型适配（零拷贝进阶）

### 文档与教程
- [Composition 设计文档](https://design.ros2.org/articles/composition.html)

### 论文与技术报告
- D. Thomas et al., "ROS 2 Composition", ROSCon 2019 — 组件化架构演讲

## 参考

- [ROS 2 官方文档 - Composing multiple nodes](https://docs.ros.org/en/jazzy/Tutorials/Intermediate/Composition.html)
- [rclcpp_components 设计](https://design.ros2.org/articles/composition.html)
