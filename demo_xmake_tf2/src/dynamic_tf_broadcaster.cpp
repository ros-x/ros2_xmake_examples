#include <chrono>
#include <cmath>
#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "tf2_ros/transform_broadcaster.h"
#include "geometry_msgs/msg/transform_stamped.hpp"

using namespace std::chrono_literals;

class DynamicTFBroadcaster : public rclcpp::Node
{
public:
  DynamicTFBroadcaster() : Node("dynamic_tf_broadcaster")
  {
    broadcaster_ = std::make_unique<tf2_ros::TransformBroadcaster>(this);
    timer_ = this->create_wall_timer(
      100ms, std::bind(&DynamicTFBroadcaster::broadcast_timer, this));
    RCLCPP_INFO(this->get_logger(), "Dynamic TF broadcaster started");
  }

private:
  void broadcast_timer()
  {
    double t = this->get_clock()->now().seconds();

    geometry_msgs::msg::TransformStamped transform;
    transform.header.stamp = this->get_clock()->now();
    transform.header.frame_id = "base_link";
    transform.child_frame_id = "sensor_link";
    transform.transform.translation.x = 0.3;
    transform.transform.translation.y = std::sin(t) * 0.1;
    transform.transform.translation.z = 0.2;
    transform.transform.rotation.x = 0.0;
    transform.transform.rotation.y = 0.0;
    transform.transform.rotation.z = std::sin(t / 2.0) * 0.1;
    transform.transform.rotation.w = std::cos(t / 2.0);

    broadcaster_->sendTransform(transform);
  }

  std::unique_ptr<tf2_ros::TransformBroadcaster> broadcaster_;
  rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<DynamicTFBroadcaster>());
  rclcpp::shutdown();
  return 0;
}
