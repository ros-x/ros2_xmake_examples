set_project("demo_xmake_component")
set_version("0.1.0")
set_languages("cxx17")

add_rules("mode.debug", "mode.release")
add_rules("ament_xmake.package")

-- Register the plugin description for pluginlib discovery
install_ros_plugin("plugin_description.xml", "rclcpp_components")

target("demo_xmake_component")
    set_kind("shared")
    add_files("src/talker_component.cpp")
    add_ros_deps("rclcpp", "rclcpp_components", "std_msgs")
