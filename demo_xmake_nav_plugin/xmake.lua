set_project("demo_xmake_nav_plugin")
set_version("0.1.0")
set_languages("cxx17")

add_rules("mode.debug", "mode.release")
add_rules("ament_xmake.package")

install_ros_plugin("plugin_description.xml", "nav2_costmap_2d")

target("demo_xmake_nav_plugin")
    set_kind("shared")
    add_files("src/gradient_layer.cpp")
    add_includedirs("include", {public = true})
    add_headerfiles("include/(demo_xmake_nav_plugin/*.hpp)")
    add_ros_deps("rclcpp", "nav2_costmap_2d", "pluginlib")
