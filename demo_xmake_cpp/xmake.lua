set_project("demo_xmake_cpp")
set_version("0.1.0")
set_languages("cxx17")
if is_plat("linux") then
    set_toolset("cc", "/usr/bin/gcc")
    set_toolset("cxx", "/usr/bin/g++")
end

add_rules("mode.debug", "mode.release")
add_rules("ament_xmake.package")

local function add_ros2_rclcpp_deps()
    local ament_prefix_path = os.getenv("AMENT_PREFIX_PATH") or ""
    local prefixes = path.splitenv(ament_prefix_path)
    for _, prefix in ipairs(prefixes) do
        local include_root = path.join(prefix, "include")
        add_includedirs(include_root)
        for _, include_dir in ipairs(os.dirs(path.join(include_root, "*"))) do
            add_includedirs(include_dir)
        end
        add_linkdirs(path.join(prefix, "lib"))
        add_rpathdirs(path.join(prefix, "lib"))
    end
    add_linkdirs("/usr/lib/x86_64-linux-gnu")
    add_rpathdirs("/usr/lib/x86_64-linux-gnu")
    add_ldflags("-Wl,-rpath-link,/usr/lib/x86_64-linux-gnu")
    add_links(
        "rclcpp",
        "rcl",
        "geometry_msgs__rosidl_typesupport_cpp",
        "rcl_interfaces__rosidl_typesupport_c",
        "rcl_interfaces__rosidl_generator_c",
        "rcl_interfaces__rosidl_typesupport_cpp",
        "rcutils",
        "rcpputils",
        "rmw",
        "rmw_implementation",
        "tracetools",
        "rosidl_runtime_c",
        "rosidl_typesupport_c",
        "rosidl_typesupport_cpp")
    add_links(
        "yaml",
        "lttng-ust",
        "lttng-ust-common",
        "lttng-ust-tracepoint",
        "numa",
        "spdlog",
        "fmt")
    add_syslinks("pthread", "dl", "m")
end

target("demo_xmake_cpp")
    set_kind("static")
    set_installdir("lib")
    add_files("src/demo_lib.cpp")
    add_headerfiles("src/demo_lib.hpp", {prefixdir = "include/demo_xmake_cpp"})

target("demo_xmake_cpp_node")
    set_kind("binary")
    set_installdir("lib/demo_xmake_cpp")
    add_files("src/main.cpp")
    add_deps("demo_xmake_cpp")

target("turtle_circle_node")
    set_kind("binary")
    set_installdir("lib/demo_xmake_cpp")
    add_files("src/turtle_circle_node.cpp")
    add_ros2_rclcpp_deps()
