set_project("demo_xmake_benchmark")
set_version("0.1.0")
set_languages("cxx17")

add_rules("mode.debug", "mode.release")
add_rules("ament_xmake.package")

target("latency_ping")
    set_kind("binary")
    set_installdir("lib/demo_xmake_benchmark")
    add_files("src/latency_ping.cpp")
    add_ros_deps("rclcpp", "std_msgs")

target("latency_pong")
    set_kind("binary")
    set_installdir("lib/demo_xmake_benchmark")
    add_files("src/latency_pong.cpp")
    add_ros_deps("rclcpp", "std_msgs")

target("throughput_test")
    set_kind("binary")
    set_installdir("lib/demo_xmake_benchmark")
    add_files("src/throughput_test.cpp")
    add_ros_deps("rclcpp", "std_msgs")
