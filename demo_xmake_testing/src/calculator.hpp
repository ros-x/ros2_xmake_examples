#pragma once

#include <string>
#include "rclcpp/rclcpp.hpp"

class Calculator : public rclcpp::Node
{
public:
  Calculator();

  int add(int a, int b);
  int subtract(int a, int b);
  double divide(int a, int b);
  int multiply(int a, int b);

  void log_result(const std::string & operation, double result);
};
