# 21. 导航插件 — Nav2 Costmap Layer

## 概述

本示例通过 `demo_xmake_nav_plugin` 包，演示如何用 xmake 构建 Nav2 的 **Costmap Layer** 插件。

**ROS 2 知识点**：Nav2 插件架构、costmap_2d layer 插件、pluginlib

**xmake 知识点**：`install_ros_plugin()` 在导航场景的实战

## ROS 2 知识

### Nav2 Costmap 插件

Nav2 的代价地图由多个 Layer 叠加而成，每个 Layer 是一个 pluginlib 插件：

```
StaticLayer → ObstacleLayer → InflationLayer → [自定义Layer]
```

自定义 Layer 需要：
1. 继承 `nav2_costmap_2d::Layer`
2. 实现 `onInitialize`、`updateBounds`、`updateCosts` 等虚函数
3. 使用 `PLUGINLIB_EXPORT_CLASS` 注册

## 示例代码

### xmake.lua 构建配置

```lua
install_ros_plugin("plugin_description.xml", "nav2_costmap_2d")

target("demo_xmake_nav_plugin")
    set_kind("shared")
    add_files("src/gradient_layer.cpp")
    add_includedirs("include", {public = true})
    add_headerfiles("include/(demo_xmake_nav_plugin/*.hpp)")
    add_ros_deps("rclcpp", "nav2_costmap_2d", "pluginlib")
```

与 `demo_xmake_component` 相同的 `install_ros_plugin()` 模式，但基础包改为 `nav2_costmap_2d`。

## 构建与运行

```bash
colcon build --packages-select demo_xmake_nav_plugin
```

插件在 Nav2 配置中激活，非独立运行。

## xmake 知识点小结

| 特性 | 说明 |
|------|------|
| `install_ros_plugin()` | 同一 API 适用于不同类型的插件（组件、导航、硬件） |

## 内容扩展

> 以下资源可帮助你深入了解本章涉及的技术领域。

### 开源项目
- [navigation2](https://github.com/ros-navigation/navigation2) — Nav2 导航框架
- [slam_toolbox](https://github.com/SteveMacenski/slam_toolbox) — 2D SLAM

### 工具
- [Nav2 配置指南](https://docs.nav2.org/configuration/index.html)

### 文档与教程
- [编写 Costmap Plugin](https://docs.nav2.org/plugin_tutorials/docs/writing_new_costmap2d_plugin.html)

### 论文与技术报告
- S. Macenski et al., "The Marathon 2: A Navigation System", IROS 2020

## 参考

- [Nav2 Costmap Plugin 教程](https://docs.nav2.org/plugin_tutorials/docs/writing_new_costmap2d_plugin.html)
