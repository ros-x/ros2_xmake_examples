#include <gtest/gtest.h>
#include <stdexcept>
#include "rclcpp/rclcpp.hpp"
#include "calculator.hpp"

class CalculatorTest : public ::testing::Test
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

TEST_F(CalculatorTest, Add)
{
  EXPECT_EQ(calc_->add(2, 3), 5);
  EXPECT_EQ(calc_->add(-1, 1), 0);
  EXPECT_EQ(calc_->add(0, 0), 0);
}

TEST_F(CalculatorTest, Subtract)
{
  EXPECT_EQ(calc_->subtract(10, 4), 6);
  EXPECT_EQ(calc_->subtract(0, 5), -5);
}

TEST_F(CalculatorTest, Multiply)
{
  EXPECT_EQ(calc_->multiply(6, 7), 42);
  EXPECT_EQ(calc_->multiply(0, 100), 0);
}

TEST_F(CalculatorTest, Divide)
{
  EXPECT_DOUBLE_EQ(calc_->divide(10, 2), 5.0);
  EXPECT_DOUBLE_EQ(calc_->divide(15, 4), 3.75);
}

TEST_F(CalculatorTest, DivideByZero)
{
  EXPECT_THROW(calc_->divide(1, 0), std::invalid_argument);
}

int main(int argc, char ** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
