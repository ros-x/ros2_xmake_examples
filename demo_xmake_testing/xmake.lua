set_project("demo_xmake_testing")
set_version("0.1.0")
set_languages("cxx17")

add_rules("mode.debug", "mode.release")
add_rules("ament_xmake.package")

target("demo_xmake_testing")
    set_kind("static")
    add_files("src/calculator.cpp")
    add_includedirs("src", {public = true})
    add_ros_deps("rclcpp")

target("calculator_node")
    set_kind("binary")
    set_installdir("lib/demo_xmake_testing")
    add_files("src/calculator_main.cpp")
    add_deps("demo_xmake_testing")
    add_ros_deps("rclcpp")
