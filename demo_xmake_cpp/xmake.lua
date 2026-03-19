set_project("demo_xmake_cpp")
set_version("0.1.0")
set_languages("cxx17")

add_rules("mode.debug", "mode.release")
add_rules("ament_xmake.package")

target("demo_xmake_cpp")
    set_kind("static")
    set_installdir("lib")
    add_files("src/demo_lib.cpp")
    add_headerfiles("src/demo_lib.hpp", {prefixdir = "include/demo_xmake_cpp"})

target("demo_xmake_cpp_node")
    set_kind("binary")
    set_installdir("lib/demo_xmake_cpp")
    add_files("src/main.cpp")
    add_deps("demo_xmake_cpp")

target("turtle_circle_node")
    set_kind("binary")
    set_installdir("lib/demo_xmake_cpp")
    add_files("src/turtle_circle_node.cpp")
    add_ros_deps("rclcpp", "geometry_msgs")
