# RMW 切换指南

## 查看当前 RMW

```bash
ros2 doctor --report | grep middleware
```

## 切换 RMW

```bash
# 使用 FastDDS（默认）
export RMW_IMPLEMENTATION=rmw_fastrtps_cpp

# 使用 CycloneDDS
export RMW_IMPLEMENTATION=rmw_cyclonedds_cpp
```

## 使用自定义配置

```bash
# FastDDS
export FASTRTPS_DEFAULT_PROFILES_FILE=/path/to/fastdds_profile.xml

# CycloneDDS
export CYCLONEDDS_URI=file:///path/to/cyclonedds_config.xml
```

## 通过 Launch 文件配置

```bash
ros2 launch demo_xmake_dds_config with_dds_config.launch.py rmw_implementation:=rmw_fastrtps_cpp
```
