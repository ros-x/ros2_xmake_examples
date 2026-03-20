#include <chrono>
#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

using namespace std::chrono_literals;

class ReliableTalker : public rclcpp::Node
{
public:
  ReliableTalker() : Node("reliable_talker"), count_(0)
  {
    auto qos = rclcpp::QoS(10)
      .reliable()
      .transient_local();

    publisher_ = this->create_publisher<std_msgs::msg::String>("qos_topic", qos);
    timer_ = this->create_wall_timer(
      500ms, std::bind(&ReliableTalker::on_timer, this));
    RCLCPP_INFO(this->get_logger(), "ReliableTalker started (RELIABLE + TRANSIENT_LOCAL)");
  }

private:
  void on_timer()
  {
    auto msg = std_msgs::msg::String();
    msg.data = "reliable_msg_" + std::to_string(count_++);
    RCLCPP_INFO(this->get_logger(), "Publishing: '%s'", msg.data.c_str());
    publisher_->publish(msg);
  }

  rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;
  rclcpp::TimerBase::SharedPtr timer_;
  size_t count_;
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<ReliableTalker>());
  rclcpp::shutdown();
  return 0;
}
