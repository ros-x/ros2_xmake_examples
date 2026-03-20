set_project("demo_xmake_rosbag")
set_version("0.1.0")
set_languages("cxx17")

add_rules("mode.debug", "mode.release")
add_rules("ament_xmake.package")

target("bag_recorder")
    set_kind("binary")
    set_installdir("lib/demo_xmake_rosbag")
    add_files("src/bag_recorder.cpp")
    add_ros_deps("rclcpp", "std_msgs", "rosbag2_cpp")

target("bag_player")
    set_kind("binary")
    set_installdir("lib/demo_xmake_rosbag")
    add_files("src/bag_player.cpp")
    add_ros_deps("rclcpp", "std_msgs", "rosbag2_cpp")
