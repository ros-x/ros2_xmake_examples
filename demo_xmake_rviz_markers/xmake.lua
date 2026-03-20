set_project("demo_xmake_rviz_markers")
set_version("0.1.0")
set_languages("cxx17")

add_rules("mode.debug", "mode.release")
add_rules("ament_xmake.package")

target("marker_publisher")
    set_kind("binary")
    set_installdir("lib/demo_xmake_rviz_markers")
    add_files("src/marker_publisher.cpp")
    add_ros_deps("rclcpp", "visualization_msgs")
