set_project("demo_xmake_robot_description")
set_version("0.1.0")
set_languages("cxx17")

add_rules("mode.debug", "mode.release")
add_rules("ament_xmake.package")

-- urdf/ and launch/ are auto-installed by convention

target("demo_xmake_robot_description_marker")
    set_kind("phony")
