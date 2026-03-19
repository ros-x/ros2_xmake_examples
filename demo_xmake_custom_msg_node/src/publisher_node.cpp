#include <chrono>
#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "demo_xmake_interfaces/msg/my_msg.hpp"

class CustomMsgPublisher : public rclcpp::Node {
public:
  CustomMsgPublisher() : rclcpp::Node("custom_msg_publisher"), count_(0) {
    publisher_ = this->create_publisher<demo_xmake_interfaces::msg::MyMsg>(
        "custom_topic", 10);
    timer_ = this->create_wall_timer(
        std::chrono::seconds(1),
        std::bind(&CustomMsgPublisher::on_timer, this));
    RCLCPP_INFO(this->get_logger(), "CustomMsgPublisher started");
  }

private:
  void on_timer() {
    auto msg = demo_xmake_interfaces::msg::MyMsg();
    msg.id = count_++;
    msg.name = "hello_xmake";
    msg.value = 3.14 * count_;
    RCLCPP_INFO(this->get_logger(),
                "Publishing: id=%u, name='%s', value=%.2f",
                msg.id, msg.name.c_str(), msg.value);
    publisher_->publish(msg);
  }

  rclcpp::Publisher<demo_xmake_interfaces::msg::MyMsg>::SharedPtr publisher_;
  rclcpp::TimerBase::SharedPtr timer_;
  uint32_t count_;
};

int main(int argc, char **argv) {
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<CustomMsgPublisher>());
  rclcpp::shutdown();
  return 0;
}
