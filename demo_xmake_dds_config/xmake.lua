set_project("demo_xmake_dds_config")
set_version("0.1.0")
set_languages("cxx17")

add_rules("mode.debug", "mode.release")
add_rules("ament_xmake.package")

-- config/ and launch/ are auto-installed by convention
-- This is a pure-data package (phony target)

target("demo_xmake_dds_config_marker")
    set_kind("phony")
