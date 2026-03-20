#include <chrono>
#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "rosbag2_cpp/writer.hpp"

using namespace std::chrono_literals;

class BagRecorder : public rclcpp::Node
{
public:
  BagRecorder() : Node("bag_recorder"), count_(0)
  {
    writer_ = std::make_unique<rosbag2_cpp::Writer>();
    writer_->open("/tmp/demo_xmake_bag");

    subscription_ = this->create_subscription<std_msgs::msg::String>(
      "chatter", 10,
      std::bind(&BagRecorder::on_message, this, std::placeholders::_1));

    // Also publish some messages to record
    publisher_ = this->create_publisher<std_msgs::msg::String>("chatter", 10);
    timer_ = this->create_wall_timer(
      500ms, std::bind(&BagRecorder::on_timer, this));

    RCLCPP_INFO(this->get_logger(), "BagRecorder started — writing to /tmp/demo_xmake_bag");
  }

private:
  void on_timer()
  {
    auto msg = std_msgs::msg::String();
    msg.data = "bag_msg_" + std::to_string(count_++);
    publisher_->publish(msg);

    if (count_ >= 10) {
      RCLCPP_INFO(this->get_logger(), "Recorded %zu messages, stopping", count_);
      rclcpp::shutdown();
    }
  }

  void on_message(const std_msgs::msg::String & msg)
  {
    auto serialized_msg = std::make_shared<rclcpp::SerializedMessage>();
    rclcpp::Serialization<std_msgs::msg::String> serializer;
    serializer.serialize_message(&msg, serialized_msg.get());

    writer_->write(
      serialized_msg, "chatter", "std_msgs/msg/String",
      this->get_clock()->now());

    RCLCPP_INFO(this->get_logger(), "Recorded: '%s'", msg.data.c_str());
  }

  std::unique_ptr<rosbag2_cpp::Writer> writer_;
  rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscription_;
  rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;
  rclcpp::TimerBase::SharedPtr timer_;
  size_t count_;
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<BagRecorder>());
  rclcpp::shutdown();
  return 0;
}
