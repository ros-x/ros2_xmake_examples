#pragma once

#include "rclcpp/rclcpp.hpp"

namespace demo_xmake_component
{

class TalkerComponent : public rclcpp::Node
{
public:
  explicit TalkerComponent(const rclcpp::NodeOptions & options);
};

}  // namespace demo_xmake_component
