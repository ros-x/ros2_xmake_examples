#include <chrono>
#include <memory>

#include "geometry_msgs/msg/twist.hpp"
#include "rclcpp/rclcpp.hpp"

class TurtleCircleNode : public rclcpp::Node {
 public:
  TurtleCircleNode() : rclcpp::Node("turtle_circle_node") {
    publisher_ =
        this->create_publisher<geometry_msgs::msg::Twist>("/turtle1/cmd_vel", 10);
    timer_ = this->create_wall_timer(
        std::chrono::milliseconds(100),
        std::bind(&TurtleCircleNode::on_timer, this));
    RCLCPP_INFO(this->get_logger(),
                "Publishing circle Twist on /turtle1/cmd_vel");
  }

 private:
  void on_timer() {
    geometry_msgs::msg::Twist cmd;
    cmd.linear.x = 2.0;
    cmd.angular.z = 1.0;
    publisher_->publish(cmd);
  }

  rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr publisher_;
  rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char **argv) {
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<TurtleCircleNode>());
  rclcpp::shutdown();
  return 0;
}
