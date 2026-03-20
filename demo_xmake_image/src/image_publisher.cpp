#include <chrono>
#include <memory>
#include <vector>

#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/image.hpp"
#include "cv_bridge/cv_bridge.h"
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

using namespace std::chrono_literals;

class ImagePublisher : public rclcpp::Node
{
public:
  ImagePublisher() : Node("image_publisher"), count_(0)
  {
    publisher_ = this->create_publisher<sensor_msgs::msg::Image>("camera/image_raw", 10);
    timer_ = this->create_wall_timer(
      100ms, std::bind(&ImagePublisher::on_timer, this));
    RCLCPP_INFO(this->get_logger(), "Image publisher started");
  }

private:
  void on_timer()
  {
    // Create a test image with a moving circle
    cv::Mat image(480, 640, CV_8UC3, cv::Scalar(0, 0, 0));
    int x = static_cast<int>(320 + 200 * std::sin(count_ * 0.1));
    int y = static_cast<int>(240 + 100 * std::cos(count_ * 0.1));
    cv::circle(image, cv::Point(x, y), 30, cv::Scalar(0, 255, 0), -1);
    cv::putText(image, "xmake #" + std::to_string(count_),
                cv::Point(10, 30), cv::FONT_HERSHEY_SIMPLEX, 1.0,
                cv::Scalar(255, 255, 255), 2);

    auto msg = cv_bridge::CvImage(std_msgs::msg::Header(), "bgr8", image).toImageMsg();
    msg->header.stamp = this->get_clock()->now();
    publisher_->publish(*msg);
    ++count_;
  }

  rclcpp::Publisher<sensor_msgs::msg::Image>::SharedPtr publisher_;
  rclcpp::TimerBase::SharedPtr timer_;
  size_t count_;
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<ImagePublisher>());
  rclcpp::shutdown();
  return 0;
}
