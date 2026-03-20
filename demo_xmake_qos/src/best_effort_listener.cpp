#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

class BestEffortListener : public rclcpp::Node
{
public:
  BestEffortListener() : Node("best_effort_listener")
  {
    // NOTE: BEST_EFFORT subscriber cannot receive from RELIABLE publisher
    // This demonstrates QoS incompatibility detection
    auto qos = rclcpp::QoS(10).best_effort();

    // Set up incompatible QoS event handler
    rclcpp::SubscriptionOptions options;
    options.event_callbacks.incompatible_qos_callback =
      [this](rclcpp::QOSRequestedIncompatibleQoSInfo & event) {
        RCLCPP_WARN(this->get_logger(),
          "QoS incompatible! Policy: %d, count: %d",
          event.last_policy_kind, event.total_count);
      };

    subscription_ = this->create_subscription<std_msgs::msg::String>(
      "qos_topic", qos,
      std::bind(&BestEffortListener::on_message, this, std::placeholders::_1),
      options);
    RCLCPP_INFO(this->get_logger(), "BestEffortListener started (BEST_EFFORT)");
  }

private:
  void on_message(const std_msgs::msg::String & msg)
  {
    RCLCPP_INFO(this->get_logger(), "Received: '%s'", msg.data.c_str());
  }

  rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscription_;
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<BestEffortListener>());
  rclcpp::shutdown();
  return 0;
}
