set_project("demo_xmake_logging")
set_version("0.1.0")
set_languages("cxx17")

add_rules("mode.debug", "mode.release")
add_rules("ament_xmake.package")

target("diagnostics_node")
    set_kind("binary")
    set_installdir("lib/demo_xmake_logging")
    add_files("src/diagnostics_node.cpp")
    add_ros_deps("rclcpp", "diagnostic_updater")
