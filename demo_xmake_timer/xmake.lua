set_project("demo_xmake_timer")
set_version("0.1.0")
set_languages("cxx17")

add_rules("mode.debug", "mode.release")
add_rules("ament_xmake.package")

target("timer_demo")
    set_kind("binary")
    set_installdir("lib/demo_xmake_timer")
    add_files("src/timer_demo.cpp")
    add_ros_deps("rclcpp")
