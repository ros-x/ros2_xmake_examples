set_project("demo_xmake_cpp")
set_version("0.1.0")
set_languages("cxx17")

add_rules("mode.debug", "mode.release")
add_rules("ament_xmake.package")

add_library("demo_xmake_cpp")
    set_kind("static")
    add_files("src/demo_lib.cpp")
    add_headerfiles("src/demo_lib.hpp")

add_binary("demo_xmake_cpp_node")
    add_files("src/main.cpp")
    add_deps("demo_xmake_cpp")

-- Placeholder for future export metadata needed by downstream CMake consumers.
