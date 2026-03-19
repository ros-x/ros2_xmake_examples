set_project("demo_xmake_custom_msg_node")
set_version("0.1.0")
set_languages("cxx17")

add_rules("mode.debug", "mode.release")
add_rules("ament_xmake.package")

target("custom_msg_publisher")
    set_kind("binary")
    set_installdir("lib/demo_xmake_custom_msg_node")
    add_files("src/publisher_node.cpp")
    add_ros_deps("rclcpp", "demo_xmake_interfaces")
