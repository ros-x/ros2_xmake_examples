# Example Scenarios

## Scenario 1: Native ament_xmake package

Package: `demo_xmake_cpp`
- builds static library + binary with xmake
- exports package config for downstream consumers

## Scenario 2: Downstream ament_cmake consumer

Package: `downstream_cmake_consumer`
- resolves `demo_xmake_cpp` via `find_package(... CONFIG)`
- hard-links `demo_xmake_cpp::demo_xmake_cpp`

This validates cross-buildsystem compatibility.
