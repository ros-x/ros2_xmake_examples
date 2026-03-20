#include <chrono>
#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "visualization_msgs/msg/marker.hpp"
#include "visualization_msgs/msg/marker_array.hpp"

using namespace std::chrono_literals;

class MarkerPublisher : public rclcpp::Node
{
public:
  MarkerPublisher() : Node("marker_publisher"), id_(0)
  {
    marker_pub_ = this->create_publisher<visualization_msgs::msg::Marker>(
      "visualization_marker", 10);
    array_pub_ = this->create_publisher<visualization_msgs::msg::MarkerArray>(
      "visualization_marker_array", 10);
    timer_ = this->create_wall_timer(
      1s, std::bind(&MarkerPublisher::on_timer, this));
    RCLCPP_INFO(this->get_logger(), "MarkerPublisher started");
  }

private:
  void on_timer()
  {
    // Publish a single arrow marker
    visualization_msgs::msg::Marker arrow;
    arrow.header.frame_id = "base_link";
    arrow.header.stamp = this->get_clock()->now();
    arrow.ns = "demo";
    arrow.id = id_++;
    arrow.type = visualization_msgs::msg::Marker::ARROW;
    arrow.action = visualization_msgs::msg::Marker::ADD;
    arrow.pose.position.x = 0.0;
    arrow.pose.position.y = 0.0;
    arrow.pose.position.z = 1.0;
    arrow.pose.orientation.w = 1.0;
    arrow.scale.x = 0.5;
    arrow.scale.y = 0.05;
    arrow.scale.z = 0.05;
    arrow.color.r = 1.0;
    arrow.color.a = 1.0;
    arrow.lifetime = rclcpp::Duration(0, 0);
    marker_pub_->publish(arrow);

    // Publish marker array with sphere and text
    visualization_msgs::msg::MarkerArray array;

    visualization_msgs::msg::Marker sphere;
    sphere.header.frame_id = "base_link";
    sphere.header.stamp = this->get_clock()->now();
    sphere.ns = "demo";
    sphere.id = id_++;
    sphere.type = visualization_msgs::msg::Marker::SPHERE;
    sphere.action = visualization_msgs::msg::Marker::ADD;
    sphere.pose.position.x = 1.0;
    sphere.pose.position.y = 0.0;
    sphere.pose.position.z = 1.0;
    sphere.pose.orientation.w = 1.0;
    sphere.scale.x = 0.2;
    sphere.scale.y = 0.2;
    sphere.scale.z = 0.2;
    sphere.color.g = 1.0;
    sphere.color.a = 1.0;
    array.markers.push_back(sphere);

    visualization_msgs::msg::Marker text;
    text.header.frame_id = "base_link";
    text.header.stamp = this->get_clock()->now();
    text.ns = "demo";
    text.id = id_++;
    text.type = visualization_msgs::msg::Marker::TEXT_VIEW_FACING;
    text.action = visualization_msgs::msg::Marker::ADD;
    text.pose.position.x = 0.0;
    text.pose.position.y = 0.0;
    text.pose.position.z = 2.0;
    text.pose.orientation.w = 1.0;
    text.scale.z = 0.3;
    text.color.r = 1.0;
    text.color.g = 1.0;
    text.color.b = 1.0;
    text.color.a = 1.0;
    text.text = "xmake markers!";
    array.markers.push_back(text);

    array_pub_->publish(array);
    RCLCPP_INFO(this->get_logger(), "Published markers (id up to %d)", id_);
  }

  rclcpp::Publisher<visualization_msgs::msg::Marker>::SharedPtr marker_pub_;
  rclcpp::Publisher<visualization_msgs::msg::MarkerArray>::SharedPtr array_pub_;
  rclcpp::TimerBase::SharedPtr timer_;
  int id_;
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<MarkerPublisher>());
  rclcpp::shutdown();
  return 0;
}
