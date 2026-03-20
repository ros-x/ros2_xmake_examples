#include <chrono>
#include <memory>
#include <thread>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

using namespace std::chrono_literals;

class MultiCallbackNode : public rclcpp::Node
{
public:
  MultiCallbackNode() : Node("multi_callback_node"), count_(0)
  {
    // MutuallyExclusive group — callbacks won't run in parallel
    exclusive_group_ = this->create_callback_group(
      rclcpp::CallbackGroupType::MutuallyExclusive);

    // Reentrant group — callbacks may run in parallel
    reentrant_group_ = this->create_callback_group(
      rclcpp::CallbackGroupType::Reentrant);

    // Timer in exclusive group
    timer1_ = this->create_wall_timer(
      500ms,
      [this]() {
        RCLCPP_INFO(this->get_logger(), "[exclusive] timer tick #%zu (thread %s)",
          ++count_, thread_id().c_str());
      },
      exclusive_group_);

    // Subscription in reentrant group
    rclcpp::SubscriptionOptions sub_options;
    sub_options.callback_group = reentrant_group_;
    subscription_ = this->create_subscription<std_msgs::msg::String>(
      "multi_topic", 10,
      [this](const std_msgs::msg::String & msg) {
        RCLCPP_INFO(this->get_logger(), "[reentrant] received: '%s' (thread %s)",
          msg.data.c_str(), thread_id().c_str());
      },
      sub_options);

    // Publisher + timer to generate messages
    publisher_ = this->create_publisher<std_msgs::msg::String>("multi_topic", 10);
    timer2_ = this->create_wall_timer(
      300ms,
      [this]() {
        auto msg = std_msgs::msg::String();
        msg.data = "msg_" + std::to_string(count_);
        publisher_->publish(msg);
      },
      reentrant_group_);

    RCLCPP_INFO(this->get_logger(), "MultiCallbackNode started with 2 callback groups");
  }

private:
  static std::string thread_id()
  {
    std::ostringstream oss;
    oss << std::this_thread::get_id();
    return oss.str();
  }

  rclcpp::CallbackGroup::SharedPtr exclusive_group_;
  rclcpp::CallbackGroup::SharedPtr reentrant_group_;
  rclcpp::TimerBase::SharedPtr timer1_;
  rclcpp::TimerBase::SharedPtr timer2_;
  rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;
  rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscription_;
  size_t count_;
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);

  auto node = std::make_shared<MultiCallbackNode>();

  rclcpp::executors::MultiThreadedExecutor executor(rclcpp::ExecutorOptions(), 4);
  executor.add_node(node);

  RCLCPP_INFO(rclcpp::get_logger("main"), "MultiThreadedExecutor running with 4 threads");
  executor.spin();

  rclcpp::shutdown();
  return 0;
}
