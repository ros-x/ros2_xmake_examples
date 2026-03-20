# 17. 可视化标记 — RViz Marker 调试

## 概述

本示例通过 [`demo_xmake_rviz_markers`](../demo_xmake_rviz_markers) 包，演示如何发布 RViz **Marker** 和 **MarkerArray** 用于可视化调试。

**ROS 2 知识点**：visualization_msgs::Marker/MarkerArray、RViz 调试技巧

**xmake 知识点**：`visualization_msgs` 依赖

## ROS 2 知识

### Marker 类型

| 类型 | 枚举值 | 用途 |
|------|--------|------|
| ARROW | 0 | 方向指示 |
| CUBE | 1 | 障碍物、区域 |
| SPHERE | 2 | 点位标记 |
| CYLINDER | 3 | 柱状物体 |
| LINE_STRIP | 4 | 路径、轨迹 |
| TEXT_VIEW_FACING | 9 | 文字标注 |
| MESH_RESOURCE | 10 | 3D 模型 |

### Marker 属性

每个 Marker 必须设置：`header.frame_id`、`ns`（命名空间）、`id`（唯一标识）、`type`、`action`、`pose`、`scale`、`color`。

## 构建与运行

```bash
colcon build --packages-select demo_xmake_rviz_markers
source install/setup.bash

ros2 run demo_xmake_rviz_markers marker_publisher
# 在 RViz 中添加 Marker 和 MarkerArray 显示
```

## 验证

```bash
ros2 topic list | grep marker
ros2 topic echo /visualization_marker
```

## 内容扩展

> 以下资源可帮助你深入了解本章涉及的技术领域。

### 开源项目
- [rviz_visual_tools](https://github.com/PickNikRobotics/rviz_visual_tools) — 高级可视化辅助库
- [jsk_visualization](https://github.com/jsk-ros-pkg/jsk_visualization) — 扩展 RViz 插件集

### 工具
- [Foxglove Studio](https://foxglove.dev/) — 浏览器端替代 RViz

### 文档与教程
- [RViz 用户指南](https://docs.ros.org/en/jazzy/Tutorials/Intermediate/RViz/RViz-User-Guide/RViz-User-Guide.html)

## 参考

- [RViz Marker 文档](https://wiki.ros.org/rviz/DisplayTypes/Marker)
