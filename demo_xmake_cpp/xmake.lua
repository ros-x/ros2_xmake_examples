set_project("demo_xmake_cpp")
set_version("0.1.0")
set_languages("cxx17")

local function import_ament_xmake_rule()
    for _, prefix in ipairs(path.splitenv(os.getenv("AMENT_PREFIX_PATH") or "")) do
        local rulefile = path.join(
            prefix, "share", "ament_xmake", "xmake", "rules", "ament_xmake", "package.lua")
        if os.isfile(rulefile) then
            includes(rulefile)
            return true
        end
    end
    return false
end

if not import_ament_xmake_rule() then
    raise("cannot find ament_xmake rule via AMENT_PREFIX_PATH")
end

add_rules("mode.debug", "mode.release")

target("demo_xmake_cpp")
    set_kind("static")
    add_rules("ament_xmake.package")
    add_files("src/demo_lib.cpp")
    add_headerfiles("src/demo_lib.hpp")

target("demo_xmake_cpp_node")
    set_kind("binary")
    add_rules("ament_xmake.package")
    add_files("src/main.cpp")
    add_deps("demo_xmake_cpp")

-- Placeholder for future export metadata needed by downstream CMake consumers.
