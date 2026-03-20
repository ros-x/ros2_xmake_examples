#include <chrono>
#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "rclcpp_action/rclcpp_action.hpp"
#include "demo_xmake_interfaces/action/countdown.hpp"

using Countdown = demo_xmake_interfaces::action::Countdown;
using GoalHandleCountdown = rclcpp_action::ClientGoalHandle<Countdown>;

class CountdownClient : public rclcpp::Node
{
public:
  CountdownClient() : Node("countdown_client")
  {
    client_ = rclcpp_action::create_client<Countdown>(this, "countdown");
  }

  void send_goal(int32_t target)
  {
    using namespace std::chrono_literals;

    if (!client_->wait_for_action_server(5s)) {
      RCLCPP_ERROR(this->get_logger(), "Action server not available");
      return;
    }

    auto goal = Countdown::Goal();
    goal.target_number = target;

    auto send_goal_options = rclcpp_action::Client<Countdown>::SendGoalOptions();
    send_goal_options.goal_response_callback =
      [this](const GoalHandleCountdown::SharedPtr & goal_handle) {
        if (!goal_handle) {
          RCLCPP_ERROR(this->get_logger(), "Goal rejected");
        } else {
          RCLCPP_INFO(this->get_logger(), "Goal accepted");
        }
      };
    send_goal_options.feedback_callback =
      [this](GoalHandleCountdown::SharedPtr,
             const std::shared_ptr<const Countdown::Feedback> feedback) {
        RCLCPP_INFO(this->get_logger(), "Feedback: %d", feedback->current_number);
      };
    send_goal_options.result_callback =
      [this](const GoalHandleCountdown::WrappedResult & result) {
        if (result.result->success) {
          RCLCPP_INFO(this->get_logger(), "Countdown succeeded!");
        } else {
          RCLCPP_WARN(this->get_logger(), "Countdown did not complete");
        }
        rclcpp::shutdown();
      };

    RCLCPP_INFO(this->get_logger(), "Sending goal: count from %d", target);
    client_->async_send_goal(goal, send_goal_options);
  }

private:
  rclcpp_action::Client<Countdown>::SharedPtr client_;
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  auto node = std::make_shared<CountdownClient>();
  node->send_goal(5);
  rclcpp::spin(node);
  rclcpp::shutdown();
  return 0;
}
