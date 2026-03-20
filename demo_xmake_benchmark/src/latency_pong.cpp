#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

class LatencyPong : public rclcpp::Node
{
public:
  LatencyPong()
  : Node("latency_pong")
  {
    publisher_ = this->create_publisher<std_msgs::msg::String>("/benchmark/pong", 10);
    subscription_ = this->create_subscription<std_msgs::msg::String>(
      "/benchmark/ping", 10,
      std::bind(&LatencyPong::ping_callback, this, std::placeholders::_1));
    RCLCPP_INFO(this->get_logger(), "LatencyPong started");
  }

private:
  void ping_callback(const std_msgs::msg::String::SharedPtr msg)
  {
    publisher_->publish(*msg);
  }

  rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;
  rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscription_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<LatencyPong>());
  rclcpp::shutdown();
  return 0;
}
