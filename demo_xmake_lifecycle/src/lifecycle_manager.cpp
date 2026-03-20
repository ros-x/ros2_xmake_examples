#include <chrono>
#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "lifecycle_msgs/srv/change_state.hpp"
#include "lifecycle_msgs/srv/get_state.hpp"

using namespace std::chrono_literals;

class LifecycleManager : public rclcpp::Node
{
public:
  LifecycleManager() : Node("lifecycle_manager")
  {
    change_state_client_ = this->create_client<lifecycle_msgs::srv::ChangeState>(
      "lifecycle_talker/change_state");
    get_state_client_ = this->create_client<lifecycle_msgs::srv::GetState>(
      "lifecycle_talker/get_state");

    // Drive the lifecycle transitions after a short delay
    timer_ = this->create_wall_timer(2s, std::bind(&LifecycleManager::run_lifecycle, this));
    RCLCPP_INFO(this->get_logger(), "LifecycleManager ready");
  }

private:
  void run_lifecycle()
  {
    timer_.reset();  // Run once

    if (!change_state_client_->wait_for_service(5s)) {
      RCLCPP_ERROR(this->get_logger(), "change_state service not available");
      return;
    }

    // Configure
    change_state(lifecycle_msgs::msg::Transition::TRANSITION_CONFIGURE);
    // Activate
    change_state(lifecycle_msgs::msg::Transition::TRANSITION_ACTIVATE);

    RCLCPP_INFO(this->get_logger(), "Lifecycle talker is now active");
  }

  void change_state(uint8_t transition_id)
  {
    auto request = std::make_shared<lifecycle_msgs::srv::ChangeState::Request>();
    request->transition.id = transition_id;

    auto future = change_state_client_->async_send_request(request);
    if (rclcpp::spin_until_future_complete(
          this->get_node_base_interface(), future, 5s) ==
        rclcpp::FutureReturnCode::SUCCESS)
    {
      if (future.get()->success) {
        RCLCPP_INFO(this->get_logger(), "Transition %u succeeded", transition_id);
      } else {
        RCLCPP_ERROR(this->get_logger(), "Transition %u failed", transition_id);
      }
    } else {
      RCLCPP_ERROR(this->get_logger(), "Transition %u timed out", transition_id);
    }
  }

  rclcpp::Client<lifecycle_msgs::srv::ChangeState>::SharedPtr change_state_client_;
  rclcpp::Client<lifecycle_msgs::srv::GetState>::SharedPtr get_state_client_;
  rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<LifecycleManager>());
  rclcpp::shutdown();
  return 0;
}
