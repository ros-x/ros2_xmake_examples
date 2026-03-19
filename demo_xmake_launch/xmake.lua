set_project("demo_xmake_launch")
set_version("0.1.0")
set_languages("cxx17")

add_rules("mode.debug", "mode.release")
add_rules("ament_xmake.package")

-- Install launch and config files to standard ROS locations
install_ros_data("launch", "launch/*.py")
install_ros_data("config", "config/*.yaml")

-- Dummy target so xmake has something to process
-- (pure data packages still need at least one target for the rule to fire)
target("demo_xmake_launch_marker")
    set_kind("phony")
