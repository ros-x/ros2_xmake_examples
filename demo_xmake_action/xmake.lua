set_project("demo_xmake_action")
set_version("0.1.0")
set_languages("cxx17")

add_rules("mode.debug", "mode.release")
add_rules("ament_xmake.package")

target("countdown_server")
    set_kind("binary")
    set_installdir("lib/demo_xmake_action")
    add_files("src/countdown_server.cpp")
    add_ros_deps("rclcpp", "rclcpp_action", "demo_xmake_interfaces")

target("countdown_client")
    set_kind("binary")
    set_installdir("lib/demo_xmake_action")
    add_files("src/countdown_client.cpp")
    add_ros_deps("rclcpp", "rclcpp_action", "demo_xmake_interfaces")
