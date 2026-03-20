set_project("demo_xmake_service")
set_version("0.1.0")
set_languages("cxx17")

add_rules("mode.debug", "mode.release")
add_rules("ament_xmake.package")

target("add_two_ints_server")
    set_kind("binary")
    set_installdir("lib/demo_xmake_service")
    add_files("src/add_two_ints_server.cpp")
    add_ros_deps("rclcpp", "example_interfaces")

target("add_two_ints_client")
    set_kind("binary")
    set_installdir("lib/demo_xmake_service")
    add_files("src/add_two_ints_client.cpp")
    add_ros_deps("rclcpp", "example_interfaces")
