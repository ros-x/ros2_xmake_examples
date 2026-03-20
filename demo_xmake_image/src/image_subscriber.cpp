#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/image.hpp"
#include "cv_bridge/cv_bridge.h"
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

class ImageSubscriber : public rclcpp::Node
{
public:
  ImageSubscriber() : Node("image_subscriber"), count_(0)
  {
    subscription_ = this->create_subscription<sensor_msgs::msg::Image>(
      "camera/image_raw", 10,
      std::bind(&ImageSubscriber::on_image, this, std::placeholders::_1));
    RCLCPP_INFO(this->get_logger(), "Image subscriber started");
  }

private:
  void on_image(const sensor_msgs::msg::Image & msg)
  {
    auto cv_ptr = cv_bridge::toCvCopy(msg, "bgr8");

    // Convert to grayscale as simple processing
    cv::Mat gray;
    cv::cvtColor(cv_ptr->image, gray, cv::COLOR_BGR2GRAY);

    if (++count_ % 30 == 0) {
      RCLCPP_INFO(this->get_logger(),
        "Processed frame %zu: %dx%d -> grayscale",
        count_, gray.cols, gray.rows);
    }
  }

  rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr subscription_;
  size_t count_;
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<ImageSubscriber>());
  rclcpp::shutdown();
  return 0;
}
