#include <chrono>
#include <memory>
#include <random>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

using namespace std::chrono_literals;

class SensorDemo : public rclcpp::Node
{
public:
  SensorDemo() : Node("sensor_demo"), count_(0)
  {
    // SensorDataQoS: BEST_EFFORT reliability, VOLATILE durability, small depth
    auto qos = rclcpp::SensorDataQoS();

    publisher_ = this->create_publisher<std_msgs::msg::String>("sensor_data", qos);

    subscription_ = this->create_subscription<std_msgs::msg::String>(
      "sensor_data", qos,
      std::bind(&SensorDemo::on_message, this, std::placeholders::_1));

    timer_ = this->create_wall_timer(
      100ms, std::bind(&SensorDemo::on_timer, this));

    RCLCPP_INFO(this->get_logger(), "SensorDemo started (SensorDataQoS)");
  }

private:
  void on_timer()
  {
    auto msg = std_msgs::msg::String();
    msg.data = "sensor_reading_" + std::to_string(count_++);
    publisher_->publish(msg);
  }

  void on_message(const std_msgs::msg::String & msg)
  {
    if (count_ % 50 == 0) {
      RCLCPP_INFO(this->get_logger(), "Sensor: '%s'", msg.data.c_str());
    }
  }

  rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;
  rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscription_;
  rclcpp::TimerBase::SharedPtr timer_;
  size_t count_;
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<SensorDemo>());
  rclcpp::shutdown();
  return 0;
}
