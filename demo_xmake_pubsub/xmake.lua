set_project("demo_xmake_pubsub")
set_version("0.1.0")
set_languages("cxx17")

add_rules("mode.debug", "mode.release")
add_rules("ament_xmake.package")

target("talker")
    set_kind("binary")
    set_installdir("lib/demo_xmake_pubsub")
    add_files("src/talker.cpp")
    add_ros_deps("rclcpp", "std_msgs")

target("listener")
    set_kind("binary")
    set_installdir("lib/demo_xmake_pubsub")
    add_files("src/listener.cpp")
    add_ros_deps("rclcpp", "std_msgs")
