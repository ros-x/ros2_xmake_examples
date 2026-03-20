#include <chrono>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

class ThroughputTest : public rclcpp::Node
{
public:
  ThroughputTest()
  : Node("throughput_test"),
    msg_count_(0),
    msg_size_(1024)
  {
    publisher_ = this->create_publisher<std_msgs::msg::String>("/benchmark/throughput", 10);
    subscription_ = this->create_subscription<std_msgs::msg::String>(
      "/benchmark/throughput", 10,
      std::bind(&ThroughputTest::topic_callback, this, std::placeholders::_1));

    payload_ = std::string(msg_size_, 'A');

    publish_timer_ = this->create_wall_timer(
      std::chrono::milliseconds(1),
      std::bind(&ThroughputTest::publish_callback, this));

    report_timer_ = this->create_wall_timer(
      std::chrono::seconds(1),
      std::bind(&ThroughputTest::report_callback, this));

    last_report_time_ = std::chrono::steady_clock::now();
    RCLCPP_INFO(this->get_logger(), "ThroughputTest started");
  }

private:
  void publish_callback()
  {
    auto msg = std_msgs::msg::String();
    msg.data = payload_;
    publisher_->publish(msg);
  }

  void topic_callback(const std_msgs::msg::String::SharedPtr /*msg*/)
  {
    msg_count_++;
  }

  void report_callback()
  {
    auto now = std::chrono::steady_clock::now();
    double elapsed = std::chrono::duration<double>(now - last_report_time_).count();
    if (elapsed > 0.0) {
      double msgs_per_sec = static_cast<double>(msg_count_) / elapsed;
      double kb_per_sec = msgs_per_sec * static_cast<double>(msg_size_) / 1024.0;
      RCLCPP_INFO(
        this->get_logger(),
        "Throughput: %.0f msgs/sec (%.0f KB/sec)", msgs_per_sec, kb_per_sec);
    }
    msg_count_ = 0;
    last_report_time_ = now;
  }

  rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;
  rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscription_;
  rclcpp::TimerBase::SharedPtr publish_timer_;
  rclcpp::TimerBase::SharedPtr report_timer_;
  std::chrono::steady_clock::time_point last_report_time_;
  size_t msg_count_;
  size_t msg_size_;
  std::string payload_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<ThroughputTest>());
  rclcpp::shutdown();
  return 0;
}
