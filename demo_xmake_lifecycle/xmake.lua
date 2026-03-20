set_project("demo_xmake_lifecycle")
set_version("0.1.0")
set_languages("cxx17")

add_rules("mode.debug", "mode.release")
add_rules("ament_xmake.package")

target("lifecycle_talker")
    set_kind("binary")
    set_installdir("lib/demo_xmake_lifecycle")
    add_files("src/lifecycle_talker.cpp")
    add_ros_deps("rclcpp", "rclcpp_lifecycle", "std_msgs")

target("lifecycle_manager")
    set_kind("binary")
    set_installdir("lib/demo_xmake_lifecycle")
    add_files("src/lifecycle_manager.cpp")
    add_ros_deps("rclcpp", "lifecycle_msgs")
