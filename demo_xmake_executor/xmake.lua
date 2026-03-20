set_project("demo_xmake_executor")
set_version("0.1.0")
set_languages("cxx17")

add_rules("mode.debug", "mode.release")
add_rules("ament_xmake.package")

target("single_threaded_demo")
    set_kind("binary")
    set_installdir("lib/demo_xmake_executor")
    add_files("src/single_threaded_demo.cpp")
    add_ros_deps("rclcpp", "std_msgs")

target("multi_threaded_demo")
    set_kind("binary")
    set_installdir("lib/demo_xmake_executor")
    add_files("src/multi_threaded_demo.cpp")
    add_ros_deps("rclcpp", "std_msgs")
