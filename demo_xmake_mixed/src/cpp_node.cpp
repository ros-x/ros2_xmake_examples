#include <chrono>
#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

class CppPublisher : public rclcpp::Node {
public:
  CppPublisher() : rclcpp::Node("cpp_publisher"), count_(0) {
    publisher_ = this->create_publisher<std_msgs::msg::String>("mixed_topic", 10);
    timer_ = this->create_wall_timer(
      std::chrono::seconds(1),
      std::bind(&CppPublisher::on_timer, this));
    RCLCPP_INFO(this->get_logger(), "C++ publisher started");
  }

private:
  void on_timer() {
    auto msg = std_msgs::msg::String();
    msg.data = "cpp_msg_" + std::to_string(count_++);
    publisher_->publish(msg);
  }

  rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;
  rclcpp::TimerBase::SharedPtr timer_;
  size_t count_;
};

int main(int argc, char ** argv) {
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<CppPublisher>());
  rclcpp::shutdown();
  return 0;
}
