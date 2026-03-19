set_project("demo_xmake_cpp")
set_version("0.1.0")
set_languages("cxx17")

add_rules("mode.debug", "mode.release")

target("demo_xmake_cpp")
    set_kind("static")
    add_rules("ament_xmake.package")
    set_installdir("lib")
    add_files("src/demo_lib.cpp")
    add_headerfiles("src/demo_lib.hpp", {prefixdir = "include/demo_xmake_cpp"})

target("demo_xmake_cpp_node")
    set_kind("binary")
    add_rules("ament_xmake.package")
    set_installdir("lib/demo_xmake_cpp")
    add_files("src/main.cpp")
    add_deps("demo_xmake_cpp")

-- Placeholder for future export metadata needed by downstream CMake consumers.
