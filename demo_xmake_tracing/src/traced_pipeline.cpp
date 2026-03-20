#include <chrono>
#include <functional>
#include <memory>
#include <string>
#include <thread>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

using namespace std::chrono_literals;

class TracedPipeline : public rclcpp::Node
{
public:
  TracedPipeline()
  : Node("traced_pipeline"), count_(0)
  {
    // Stage 1: publish on a 10 Hz timer
    pub_stage1_ = this->create_publisher<std_msgs::msg::String>("/pipeline/stage1", 10);
    timer_ = this->create_wall_timer(100ms, std::bind(&TracedPipeline::timer_callback, this));

    // Stage 2: subscribe to stage1, process, publish to stage2
    pub_stage2_ = this->create_publisher<std_msgs::msg::String>("/pipeline/stage2", 10);
    sub_stage1_ = this->create_subscription<std_msgs::msg::String>(
      "/pipeline/stage1", 10,
      std::bind(&TracedPipeline::stage2_callback, this, std::placeholders::_1));

    // Stage 3: subscribe to stage2, final processing
    sub_stage2_ = this->create_subscription<std_msgs::msg::String>(
      "/pipeline/stage2", 10,
      std::bind(&TracedPipeline::stage3_callback, this, std::placeholders::_1));

    RCLCPP_INFO(this->get_logger(), "TracedPipeline started");
  }

private:
  void timer_callback()
  {
    auto msg = std_msgs::msg::String();
    msg.data = "frame_" + std::to_string(count_++);
    RCLCPP_INFO(this->get_logger(), "Stage1 publishing: '%s'", msg.data.c_str());
    pub_stage1_->publish(msg);
  }

  void stage2_callback(const std_msgs::msg::String::SharedPtr msg)
  {
    // Simulate processing latency
    std::this_thread::sleep_for(5ms);

    auto out = std_msgs::msg::String();
    out.data = msg->data + "_processed";
    RCLCPP_INFO(this->get_logger(), "Stage2 forwarding: '%s'", out.data.c_str());
    pub_stage2_->publish(out);
  }

  void stage3_callback(const std_msgs::msg::String::SharedPtr msg)
  {
    // Simulate final processing latency
    std::this_thread::sleep_for(3ms);

    RCLCPP_INFO(this->get_logger(), "Stage3 result: '%s'", msg->data.c_str());
  }

  rclcpp::TimerBase::SharedPtr timer_;
  rclcpp::Publisher<std_msgs::msg::String>::SharedPtr pub_stage1_;
  rclcpp::Publisher<std_msgs::msg::String>::SharedPtr pub_stage2_;
  rclcpp::Subscription<std_msgs::msg::String>::SharedPtr sub_stage1_;
  rclcpp::Subscription<std_msgs::msg::String>::SharedPtr sub_stage2_;
  size_t count_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<TracedPipeline>());
  rclcpp::shutdown();
  return 0;
}
