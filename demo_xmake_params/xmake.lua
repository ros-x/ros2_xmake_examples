set_project("demo_xmake_params")
set_version("0.1.0")
set_languages("cxx17")

add_rules("mode.debug", "mode.release")
add_rules("ament_xmake.package")

-- config/ and launch/ directories are auto-installed by convention
-- No need for explicit install_ros_data() calls

target("param_node")
    set_kind("binary")
    set_installdir("lib/demo_xmake_params")
    add_files("src/param_node.cpp")
    add_ros_deps("rclcpp")
