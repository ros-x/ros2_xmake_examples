#include <gtest/gtest.h>
#include <memory>
#include "rclcpp/rclcpp.hpp"
#include "calculator.hpp"

class NodeTest : public ::testing::Test
{
protected:
  void SetUp() override
  {
    if (!rclcpp::ok()) {
      rclcpp::init(0, nullptr);
    }
    calc_ = std::make_shared<Calculator>();
  }

  std::shared_ptr<Calculator> calc_;
};

TEST_F(NodeTest, NodeName)
{
  EXPECT_EQ(std::string(calc_->get_name()), "calculator");
}

TEST_F(NodeTest, NodeNamespace)
{
  EXPECT_EQ(std::string(calc_->get_namespace()), "/");
}

TEST_F(NodeTest, LoggerWorks)
{
  EXPECT_NO_THROW(calc_->log_result("test", 42.0));
}

int main(int argc, char ** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
