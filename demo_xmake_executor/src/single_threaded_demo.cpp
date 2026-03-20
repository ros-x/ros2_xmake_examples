#include <chrono>
#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

using namespace std::chrono_literals;

class NodeA : public rclcpp::Node
{
public:
  NodeA() : Node("node_a"), count_(0)
  {
    publisher_ = this->create_publisher<std_msgs::msg::String>("topic_a", 10);
    timer_ = this->create_wall_timer(
      500ms, std::bind(&NodeA::on_timer, this));
    RCLCPP_INFO(this->get_logger(), "NodeA started");
  }

private:
  void on_timer()
  {
    auto msg = std_msgs::msg::String();
    msg.data = "from_A_" + std::to_string(count_++);
    publisher_->publish(msg);
  }

  rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;
  rclcpp::TimerBase::SharedPtr timer_;
  size_t count_;
};

class NodeB : public rclcpp::Node
{
public:
  NodeB() : Node("node_b")
  {
    subscription_ = this->create_subscription<std_msgs::msg::String>(
      "topic_a", 10,
      [this](const std_msgs::msg::String & msg) {
        RCLCPP_INFO(this->get_logger(), "NodeB received: '%s'", msg.data.c_str());
      });
    RCLCPP_INFO(this->get_logger(), "NodeB started");
  }

private:
  rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscription_;
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);

  auto node_a = std::make_shared<NodeA>();
  auto node_b = std::make_shared<NodeB>();

  rclcpp::executors::SingleThreadedExecutor executor;
  executor.add_node(node_a);
  executor.add_node(node_b);

  RCLCPP_INFO(rclcpp::get_logger("main"), "SingleThreadedExecutor running two nodes");
  executor.spin();

  rclcpp::shutdown();
  return 0;
}
