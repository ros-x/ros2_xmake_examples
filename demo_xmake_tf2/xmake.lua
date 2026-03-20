set_project("demo_xmake_tf2")
set_version("0.1.0")
set_languages("cxx17")

add_rules("mode.debug", "mode.release")
add_rules("ament_xmake.package")

target("static_tf_broadcaster")
    set_kind("binary")
    set_installdir("lib/demo_xmake_tf2")
    add_files("src/static_tf_broadcaster.cpp")
    add_ros_deps("rclcpp", "tf2_ros", "geometry_msgs")

target("dynamic_tf_broadcaster")
    set_kind("binary")
    set_installdir("lib/demo_xmake_tf2")
    add_files("src/dynamic_tf_broadcaster.cpp")
    add_ros_deps("rclcpp", "tf2_ros", "geometry_msgs")

target("tf_listener")
    set_kind("binary")
    set_installdir("lib/demo_xmake_tf2")
    add_files("src/tf_listener.cpp")
    add_ros_deps("rclcpp", "tf2_ros", "tf2_geometry_msgs", "geometry_msgs")
