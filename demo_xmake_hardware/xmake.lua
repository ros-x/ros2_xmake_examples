set_project("demo_xmake_hardware")
set_version("0.1.0")
set_languages("cxx17")

add_rules("mode.debug", "mode.release")
add_rules("ament_xmake.package")

install_ros_plugin("plugin_description.xml", "hardware_interface")

target("demo_xmake_hardware")
    set_kind("shared")
    add_files("src/mock_robot_hardware.cpp")
    add_ros_deps("rclcpp", "hardware_interface", "pluginlib", "rclcpp_lifecycle")
