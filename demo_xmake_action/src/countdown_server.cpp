#include <chrono>
#include <memory>
#include <thread>

#include "rclcpp/rclcpp.hpp"
#include "rclcpp_action/rclcpp_action.hpp"
#include "demo_xmake_interfaces/action/countdown.hpp"

using Countdown = demo_xmake_interfaces::action::Countdown;
using GoalHandleCountdown = rclcpp_action::ServerGoalHandle<Countdown>;

class CountdownServer : public rclcpp::Node
{
public:
  CountdownServer() : Node("countdown_server")
  {
    action_server_ = rclcpp_action::create_server<Countdown>(
      this, "countdown",
      std::bind(&CountdownServer::handle_goal, this,
                std::placeholders::_1, std::placeholders::_2),
      std::bind(&CountdownServer::handle_cancel, this, std::placeholders::_1),
      std::bind(&CountdownServer::handle_accepted, this, std::placeholders::_1));
    RCLCPP_INFO(this->get_logger(), "Countdown action server ready");
  }

private:
  rclcpp_action::GoalResponse handle_goal(
    const rclcpp_action::GoalUUID &,
    std::shared_ptr<const Countdown::Goal> goal)
  {
    RCLCPP_INFO(this->get_logger(), "Received goal: count down from %d",
                goal->target_number);
    if (goal->target_number < 0) {
      return rclcpp_action::GoalResponse::REJECT;
    }
    return rclcpp_action::GoalResponse::ACCEPT_AND_EXECUTE;
  }

  rclcpp_action::CancelResponse handle_cancel(
    const std::shared_ptr<GoalHandleCountdown>)
  {
    RCLCPP_INFO(this->get_logger(), "Cancellation requested");
    return rclcpp_action::CancelResponse::ACCEPT;
  }

  void handle_accepted(const std::shared_ptr<GoalHandleCountdown> goal_handle)
  {
    std::thread([this, goal_handle]() { execute(goal_handle); }).detach();
  }

  void execute(const std::shared_ptr<GoalHandleCountdown> goal_handle)
  {
    RCLCPP_INFO(this->get_logger(), "Executing countdown...");
    auto feedback = std::make_shared<Countdown::Feedback>();
    auto result = std::make_shared<Countdown::Result>();
    int32_t current = goal_handle->get_goal()->target_number;

    rclcpp::Rate rate(1.0);
    while (current > 0 && rclcpp::ok()) {
      if (goal_handle->is_canceling()) {
        result->success = false;
        goal_handle->canceled(result);
        RCLCPP_INFO(this->get_logger(), "Goal canceled at %d", current);
        return;
      }

      feedback->current_number = current;
      goal_handle->publish_feedback(feedback);
      RCLCPP_INFO(this->get_logger(), "Countdown: %d", current);
      --current;
      rate.sleep();
    }

    result->success = true;
    goal_handle->succeed(result);
    RCLCPP_INFO(this->get_logger(), "Countdown complete!");
  }

  rclcpp_action::Server<Countdown>::SharedPtr action_server_;
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<CountdownServer>());
  rclcpp::shutdown();
  return 0;
}
