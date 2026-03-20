#include <memory>
#include "calculator.hpp"

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  auto calc = std::make_shared<Calculator>();

  calc->log_result("2 + 3", calc->add(2, 3));
  calc->log_result("10 - 4", calc->subtract(10, 4));
  calc->log_result("15 / 4", calc->divide(15, 4));
  calc->log_result("6 * 7", calc->multiply(6, 7));

  RCLCPP_INFO(calc->get_logger(), "Calculator demo complete");
  rclcpp::shutdown();
  return 0;
}
