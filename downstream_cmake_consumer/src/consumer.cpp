#include <iostream>
#include "demo_xmake_cpp/demo_lib.hpp"

int main() {
  std::cout << "downstream_cmake_consumer: add_two(5)=" << add_two(5) << std::endl;
  return 0;
}
