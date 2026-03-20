set_project("demo_xmake_image")
set_version("0.1.0")
set_languages("cxx17")

add_rules("mode.debug", "mode.release")
add_rules("ament_xmake.package")

add_requires("opencv", {system = true})

target("image_publisher")
    set_kind("binary")
    set_installdir("lib/demo_xmake_image")
    add_files("src/image_publisher.cpp")
    add_ros_deps("rclcpp", "sensor_msgs", "cv_bridge", "image_transport")
    add_packages("opencv")
    add_links("cv_bridge")

target("image_subscriber")
    set_kind("binary")
    set_installdir("lib/demo_xmake_image")
    add_files("src/image_subscriber.cpp")
    add_ros_deps("rclcpp", "sensor_msgs", "cv_bridge", "image_transport")
    add_packages("opencv")
    add_links("cv_bridge")
