#include <chrono>
#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"

using namespace std::chrono_literals;

class ParamNode : public rclcpp::Node
{
public:
  ParamNode() : Node("param_node")
  {
    // Declare parameters with default values
    this->declare_parameter("robot_name", "default_robot");
    this->declare_parameter("max_speed", 1.0);
    this->declare_parameter("use_sim", false);
    this->declare_parameter("wheel_radius", 0.1);

    // Register a parameter change callback
    param_callback_handle_ = this->add_on_set_parameters_callback(
      std::bind(&ParamNode::on_parameter_change, this, std::placeholders::_1));

    // Timer to periodically print parameters
    timer_ = this->create_wall_timer(
      2s, std::bind(&ParamNode::print_params, this));

    RCLCPP_INFO(this->get_logger(), "ParamNode started");
    print_params();
  }

private:
  void print_params()
  {
    auto name = this->get_parameter("robot_name").as_string();
    auto speed = this->get_parameter("max_speed").as_double();
    auto sim = this->get_parameter("use_sim").as_bool();
    auto radius = this->get_parameter("wheel_radius").as_double();

    RCLCPP_INFO(this->get_logger(),
      "Params: robot_name='%s', max_speed=%.2f, use_sim=%s, wheel_radius=%.3f",
      name.c_str(), speed, sim ? "true" : "false", radius);
  }

  rcl_interfaces::msg::SetParametersResult on_parameter_change(
    const std::vector<rclcpp::Parameter> & params)
  {
    auto result = rcl_interfaces::msg::SetParametersResult();
    result.successful = true;

    for (const auto & param : params) {
      RCLCPP_INFO(this->get_logger(), "Parameter changed: %s = %s",
        param.get_name().c_str(), param.value_to_string().c_str());

      if (param.get_name() == "max_speed" && param.as_double() < 0.0) {
        result.successful = false;
        result.reason = "max_speed must be non-negative";
        RCLCPP_WARN(this->get_logger(), "Rejected: %s", result.reason.c_str());
      }
    }
    return result;
  }

  rclcpp::TimerBase::SharedPtr timer_;
  rclcpp::node_interfaces::OnSetParametersCallbackHandle::SharedPtr param_callback_handle_;
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<ParamNode>());
  rclcpp::shutdown();
  return 0;
}
