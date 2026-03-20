#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "rosbag2_cpp/reader.hpp"

class BagPlayer : public rclcpp::Node
{
public:
  BagPlayer() : Node("bag_player")
  {
    publisher_ = this->create_publisher<std_msgs::msg::String>("playback", 10);
    RCLCPP_INFO(this->get_logger(), "BagPlayer started");
  }

  void play(const std::string & bag_path)
  {
    rosbag2_cpp::Reader reader;
    reader.open(bag_path);

    rclcpp::Serialization<std_msgs::msg::String> serializer;
    size_t count = 0;

    while (reader.has_next()) {
      auto bag_msg = reader.read_next();

      if (bag_msg->topic_name == "chatter") {
        auto msg = std_msgs::msg::String();
        rclcpp::SerializedMessage serialized(*bag_msg->serialized_data);
        serializer.deserialize_message(&serialized, &msg);

        publisher_->publish(msg);
        RCLCPP_INFO(this->get_logger(), "Playback [%zu]: '%s'",
                     ++count, msg.data.c_str());
      }
    }

    RCLCPP_INFO(this->get_logger(), "Playback complete: %zu messages", count);
  }

private:
  rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  auto node = std::make_shared<BagPlayer>();

  std::string bag_path = "/tmp/demo_xmake_bag";
  if (argc > 1) {
    bag_path = argv[1];
  }

  node->play(bag_path);
  rclcpp::shutdown();
  return 0;
}
