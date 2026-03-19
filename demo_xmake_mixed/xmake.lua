set_project("demo_xmake_mixed")
set_version("0.1.0")
set_languages("cxx17")

add_rules("mode.debug", "mode.release")
add_rules("ament_xmake.package")

target("cpp_publisher")
    set_kind("binary")
    set_installdir("lib/demo_xmake_mixed")
    add_files("src/cpp_node.cpp")
    add_ros_deps("rclcpp", "std_msgs")
