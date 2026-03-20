# 19. 图像处理 — cv_bridge 与 image_transport

## 概述

本示例通过 `demo_xmake_image` 包，演示 ROS 2 的图像处理管线：使用 `cv_bridge` 在 ROS 消息和 OpenCV 之间转换。

**ROS 2 知识点**：sensor_msgs::Image、cv_bridge、image_transport

**xmake 知识点**：OpenCV 系统库 + ROS 包混合依赖

## ROS 2 知识

### cv_bridge

cv_bridge 在 `sensor_msgs::Image` 和 `cv::Mat` 之间转换：

```cpp
// ROS → OpenCV
auto cv_ptr = cv_bridge::toCvCopy(msg, "bgr8");
cv::Mat image = cv_ptr->image;

// OpenCV → ROS
auto ros_msg = cv_bridge::CvImage(header, "bgr8", image).toImageMsg();
```

## 示例代码

### xmake.lua 构建配置

```lua
target("image_publisher")
    set_kind("binary")
    set_installdir("lib/demo_xmake_image")
    add_files("src/image_publisher.cpp")
    add_ros_deps("rclcpp", "sensor_msgs", "cv_bridge", "image_transport")
```

`cv_bridge` 包会自动引入 OpenCV 依赖，无需在 xmake 中单独处理 OpenCV。这是 `add_ros_deps()` 传递依赖的体现。

## 构建与运行

```bash
colcon build --packages-select demo_xmake_image
source install/setup.bash

ros2 run demo_xmake_image image_publisher   # 终端 1
ros2 run demo_xmake_image image_subscriber  # 终端 2
```

## 验证

```bash
ros2 topic echo /camera/image_raw --no-arr
ros2 topic hz /camera/image_raw
```

## xmake 知识点小结

| 特性 | 说明 |
|------|------|
| 系统库 + ROS 包混合依赖 | OpenCV 通过 `cv_bridge` 的传递依赖自动引入 |

## 内容扩展

> 以下资源可帮助你深入了解本章涉及的技术领域。

### 开源项目
- [image_pipeline](https://github.com/ros-perception/image_pipeline) — 图像矫正/深度处理
- [vision_opencv](https://github.com/ros-perception/vision_opencv) — cv_bridge + image_geometry

### 工具
- [rqt_image_view](https://docs.ros.org/en/jazzy/p/rqt_image_view/) — GUI 图像查看器

### 文档与教程
- [image_transport 插件机制](https://docs.ros.org/en/jazzy/p/image_transport/)
- [OpenCV 文档](https://docs.opencv.org/4.x/)

## 参考

- [cv_bridge 文档](https://docs.ros.org/en/jazzy/p/cv_bridge/)
