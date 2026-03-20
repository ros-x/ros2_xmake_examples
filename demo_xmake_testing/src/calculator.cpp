#include "calculator.hpp"
#include <stdexcept>

Calculator::Calculator() : Node("calculator")
{
  RCLCPP_INFO(this->get_logger(), "Calculator node created");
}

int Calculator::add(int a, int b)
{
  return a + b;
}

int Calculator::subtract(int a, int b)
{
  return a - b;
}

double Calculator::divide(int a, int b)
{
  if (b == 0) {
    throw std::invalid_argument("Division by zero");
  }
  return static_cast<double>(a) / b;
}

int Calculator::multiply(int a, int b)
{
  return a * b;
}

void Calculator::log_result(const std::string & operation, double result)
{
  RCLCPP_INFO(this->get_logger(), "%s = %.2f", operation.c_str(), result);
}
