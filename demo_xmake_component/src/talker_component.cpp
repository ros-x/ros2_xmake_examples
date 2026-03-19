#include <chrono>
#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "rclcpp_components/register_node_macro.hpp"
#include "std_msgs/msg/string.hpp"

namespace demo_xmake_component
{

class TalkerComponent : public rclcpp::Node
{
public:
  explicit TalkerComponent(const rclcpp::NodeOptions & options)
  : Node("talker_component", options), count_(0)
  {
    publisher_ = this->create_publisher<std_msgs::msg::String>("chatter", 10);
    timer_ = this->create_wall_timer(
      std::chrono::seconds(1),
      std::bind(&TalkerComponent::on_timer, this));
    RCLCPP_INFO(this->get_logger(), "TalkerComponent started");
  }

private:
  void on_timer()
  {
    auto msg = std_msgs::msg::String();
    msg.data = "Hello from xmake component #" + std::to_string(count_++);
    RCLCPP_INFO(this->get_logger(), "Publishing: '%s'", msg.data.c_str());
    publisher_->publish(msg);
  }

  rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;
  rclcpp::TimerBase::SharedPtr timer_;
  size_t count_;
};

}  // namespace demo_xmake_component

RCLCPP_COMPONENTS_REGISTER_NODE(demo_xmake_component::TalkerComponent)
