#include <chrono>
#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "rclcpp_lifecycle/lifecycle_node.hpp"
#include "std_msgs/msg/string.hpp"

using namespace std::chrono_literals;
using CallbackReturn = rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn;

class LifecycleTalker : public rclcpp_lifecycle::LifecycleNode
{
public:
  LifecycleTalker() : LifecycleNode("lifecycle_talker"), count_(0)
  {
    RCLCPP_INFO(this->get_logger(), "LifecycleTalker created (unconfigured)");
  }

  CallbackReturn on_configure(const rclcpp_lifecycle::State &) override
  {
    publisher_ = this->create_publisher<std_msgs::msg::String>("lifecycle_chatter", 10);
    RCLCPP_INFO(this->get_logger(), "on_configure: publisher created");
    return CallbackReturn::SUCCESS;
  }

  CallbackReturn on_activate(const rclcpp_lifecycle::State &) override
  {
    timer_ = this->create_wall_timer(
      1s, std::bind(&LifecycleTalker::on_timer, this));
    RCLCPP_INFO(this->get_logger(), "on_activate: timer started");
    return CallbackReturn::SUCCESS;
  }

  CallbackReturn on_deactivate(const rclcpp_lifecycle::State &) override
  {
    timer_.reset();
    RCLCPP_INFO(this->get_logger(), "on_deactivate: timer stopped");
    return CallbackReturn::SUCCESS;
  }

  CallbackReturn on_cleanup(const rclcpp_lifecycle::State &) override
  {
    publisher_.reset();
    count_ = 0;
    RCLCPP_INFO(this->get_logger(), "on_cleanup: resources released");
    return CallbackReturn::SUCCESS;
  }

  CallbackReturn on_shutdown(const rclcpp_lifecycle::State &) override
  {
    timer_.reset();
    publisher_.reset();
    RCLCPP_INFO(this->get_logger(), "on_shutdown: node shutting down");
    return CallbackReturn::SUCCESS;
  }

private:
  void on_timer()
  {
    auto msg = std_msgs::msg::String();
    msg.data = "Lifecycle hello #" + std::to_string(count_++);
    RCLCPP_INFO(this->get_logger(), "Publishing: '%s'", msg.data.c_str());
    publisher_->publish(msg);
  }

  rclcpp_lifecycle::LifecyclePublisher<std_msgs::msg::String>::SharedPtr publisher_;
  rclcpp::TimerBase::SharedPtr timer_;
  size_t count_;
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<LifecycleTalker>()->get_node_base_interface());
  rclcpp::shutdown();
  return 0;
}
