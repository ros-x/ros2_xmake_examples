set_project("demo_xmake_tracing")
set_version("0.1.0")
set_languages("cxx17")

add_rules("mode.debug", "mode.release")
add_rules("ament_xmake.package")

target("traced_pipeline")
    set_kind("binary")
    set_installdir("lib/demo_xmake_tracing")
    add_files("src/traced_pipeline.cpp")
    add_ros_deps("rclcpp", "std_msgs")
