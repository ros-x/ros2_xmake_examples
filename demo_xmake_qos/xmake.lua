set_project("demo_xmake_qos")
set_version("0.1.0")
set_languages("cxx17")

add_rules("mode.debug", "mode.release")
add_rules("ament_xmake.package")

target("reliable_talker")
    set_kind("binary")
    set_installdir("lib/demo_xmake_qos")
    add_files("src/reliable_talker.cpp")
    add_ros_deps("rclcpp", "std_msgs")

target("best_effort_listener")
    set_kind("binary")
    set_installdir("lib/demo_xmake_qos")
    add_files("src/best_effort_listener.cpp")
    add_ros_deps("rclcpp", "std_msgs")

target("sensor_demo")
    set_kind("binary")
    set_installdir("lib/demo_xmake_qos")
    add_files("src/sensor_demo.cpp")
    add_ros_deps("rclcpp", "std_msgs")
