cmake_minimum_required(VERSION 3.16)
project(downstream_cmake_consumer)

find_package(ament_cmake REQUIRED)
find_package(demo_xmake_cpp REQUIRED CONFIG)

add_executable(consumer src/consumer.cpp)
target_link_libraries(consumer PRIVATE demo_xmake_cpp::demo_xmake_cpp)

install(TARGETS consumer DESTINATION lib/${PROJECT_NAME})

ament_package()
