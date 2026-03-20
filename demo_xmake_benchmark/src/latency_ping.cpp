#include <chrono>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

class LatencyPing : public rclcpp::Node
{
public:
  LatencyPing()
  : Node("latency_ping")
  {
    publisher_ = this->create_publisher<std_msgs::msg::String>("/benchmark/ping", 10);
    subscription_ = this->create_subscription<std_msgs::msg::String>(
      "/benchmark/pong", 10,
      std::bind(&LatencyPing::pong_callback, this, std::placeholders::_1));
    timer_ = this->create_wall_timer(
      std::chrono::milliseconds(100),
      std::bind(&LatencyPing::timer_callback, this));
    RCLCPP_INFO(this->get_logger(), "LatencyPing started");
  }

private:
  void timer_callback()
  {
    auto now = std::chrono::steady_clock::now();
    auto ns = std::chrono::duration_cast<std::chrono::nanoseconds>(
      now.time_since_epoch()).count();
    auto msg = std_msgs::msg::String();
    msg.data = std::to_string(ns);
    publisher_->publish(msg);
  }

  void pong_callback(const std_msgs::msg::String::SharedPtr msg)
  {
    auto now = std::chrono::steady_clock::now();
    auto send_ns = std::stoll(msg->data);
    auto now_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(
      now.time_since_epoch()).count();
    double latency_ms = static_cast<double>(now_ns - send_ns) / 1e6;
    RCLCPP_INFO(this->get_logger(), "Round-trip latency: %.2f ms", latency_ms);
  }

  rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;
  rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscription_;
  rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<LatencyPing>());
  rclcpp::shutdown();
  return 0;
}
