#include <chrono>
#include <cmath>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "diagnostic_updater/diagnostic_updater.hpp"

using namespace std::chrono_literals;

class DiagnosticsNode : public rclcpp::Node
{
public:
  DiagnosticsNode()
  : Node("diagnostics_node"),
    updater_(this),
    start_time_(this->now()),
    tick_count_(0)
  {
    updater_.setHardwareID("simulated_hw_01");
    updater_.add("Hardware Status", this, &DiagnosticsNode::produce_diagnostics);

    timer_ = this->create_wall_timer(1s, std::bind(&DiagnosticsNode::timer_callback, this));

    RCLCPP_INFO(this->get_logger(), "DiagnosticsNode started");
  }

private:
  void timer_callback()
  {
    ++tick_count_;
    double uptime_sec = (this->now() - start_time_).seconds();

    // Simulate a CPU temperature that fluctuates over time
    double cpu_temp = 45.0 + 15.0 * std::sin(uptime_sec * 0.1);

    // Demonstrate different log levels based on simulated temperature
    if (cpu_temp > 55.0) {
      RCLCPP_ERROR(this->get_logger(), "CPU temperature CRITICAL: %.1f C", cpu_temp);
    } else if (cpu_temp > 50.0) {
      RCLCPP_WARN(this->get_logger(), "CPU temperature elevated: %.1f C", cpu_temp);
    } else {
      RCLCPP_INFO(this->get_logger(), "CPU temperature nominal: %.1f C", cpu_temp);
    }

    // Demonstrate runtime log-level switching every 10 ticks
    if (tick_count_ % 10 == 0) {
      auto current_level = rcutils_logging_get_logger_effective_level(
        this->get_logger().get_name());

      auto new_level = (current_level == RCUTILS_LOG_SEVERITY_INFO)
                         ? RCUTILS_LOG_SEVERITY_DEBUG
                         : RCUTILS_LOG_SEVERITY_INFO;

      auto ret = rcutils_logging_set_logger_level(this->get_logger().get_name(), new_level);
      if (ret == RCUTILS_RET_OK) {
        RCLCPP_INFO(this->get_logger(),
          "Switched log level to %s",
          (new_level == RCUTILS_LOG_SEVERITY_DEBUG) ? "DEBUG" : "INFO");
      }
    }

    RCLCPP_DEBUG(this->get_logger(), "Tick %zu, uptime %.1f s", tick_count_, uptime_sec);

    // Force an updater cycle so diagnostics are published
    updater_.force_update();
  }

  void produce_diagnostics(diagnostic_updater::DiagnosticStatusWrapper & stat)
  {
    double uptime_sec = (this->now() - start_time_).seconds();
    double cpu_temp = 45.0 + 15.0 * std::sin(uptime_sec * 0.1);

    stat.add("CPU Temperature (C)", cpu_temp);
    stat.add("Uptime (s)", uptime_sec);
    stat.add("Tick Count", tick_count_);

    if (cpu_temp > 55.0) {
      stat.summary(diagnostic_msgs::msg::DiagnosticStatus::ERROR,
        "CPU temperature critical");
    } else if (cpu_temp > 50.0) {
      stat.summary(diagnostic_msgs::msg::DiagnosticStatus::WARN,
        "CPU temperature elevated");
    } else {
      stat.summary(diagnostic_msgs::msg::DiagnosticStatus::OK,
        "All systems nominal");
    }
  }

  diagnostic_updater::Updater updater_;
  rclcpp::TimerBase::SharedPtr timer_;
  rclcpp::Time start_time_;
  size_t tick_count_;
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<DiagnosticsNode>());
  rclcpp::shutdown();
  return 0;
}
