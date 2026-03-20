#include <chrono>
#include <memory>

#include "rclcpp/rclcpp.hpp"

using namespace std::chrono_literals;

class TimerDemo : public rclcpp::Node
{
public:
  TimerDemo() : Node("timer_demo"), fast_count_(0), slow_count_(0)
  {
    // Fast timer - 500ms interval
    fast_timer_ = this->create_wall_timer(
      500ms, std::bind(&TimerDemo::fast_callback, this));

    // Slow timer - 2s interval
    slow_timer_ = this->create_wall_timer(
      2s, std::bind(&TimerDemo::slow_callback, this));

    RCLCPP_INFO(this->get_logger(), "TimerDemo started with two timers");
    RCLCPP_DEBUG(this->get_logger(), "Debug: fast=500ms, slow=2s");
  }

private:
  void fast_callback()
  {
    RCLCPP_INFO(this->get_logger(), "Fast timer #%zu", ++fast_count_);

    if (fast_count_ % 5 == 0) {
      RCLCPP_WARN(this->get_logger(), "Fast timer reached %zu ticks", fast_count_);
    }
  }

  void slow_callback()
  {
    RCLCPP_INFO(this->get_logger(), "Slow timer #%zu", ++slow_count_);

    if (slow_count_ >= 3) {
      RCLCPP_ERROR(this->get_logger(),
        "Slow timer reached %zu — demo stopping", slow_count_);
      rclcpp::shutdown();
    }
  }

  rclcpp::TimerBase::SharedPtr fast_timer_;
  rclcpp::TimerBase::SharedPtr slow_timer_;
  size_t fast_count_;
  size_t slow_count_;
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<TimerDemo>());
  rclcpp::shutdown();
  return 0;
}
