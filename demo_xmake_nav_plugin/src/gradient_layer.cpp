#include "demo_xmake_nav_plugin/gradient_layer.hpp"
#include "pluginlib/class_list_macros.hpp"
#include "nav2_costmap_2d/costmap_math.hpp"

namespace demo_xmake_nav_plugin
{

GradientLayer::GradientLayer() {}

void GradientLayer::onInitialize()
{
  RCLCPP_INFO(logger_, "GradientLayer plugin initialized");
  current_ = true;
}

void GradientLayer::updateBounds(
  double, double, double,
  double * min_x, double * min_y,
  double * max_x, double * max_y)
{
  *min_x = -10.0;
  *min_y = -10.0;
  *max_x = 10.0;
  *max_y = 10.0;
}

void GradientLayer::updateCosts(
  nav2_costmap_2d::Costmap2D & master_grid,
  int min_i, int min_j, int max_i, int max_j)
{
  if (!enabled_) {
    return;
  }

  unsigned int size_x = master_grid.getSizeInCellsX();
  for (int j = min_j; j < max_j; j++) {
    for (int i = min_i; i < max_i; i++) {
      int index = master_grid.getIndex(i, j);
      // Simple gradient: cost increases with distance from center
      double dx = static_cast<double>(i) - static_cast<double>(size_x) / 2.0;
      unsigned char cost = static_cast<unsigned char>(
        std::min(254.0, std::abs(dx) * 2.0));
      unsigned char old_cost = master_grid.getCost(i, j);
      master_grid.setCost(i, j, std::max(old_cost, cost));
    }
  }
}

void GradientLayer::reset()
{
  current_ = false;
}

}  // namespace demo_xmake_nav_plugin

PLUGINLIB_EXPORT_CLASS(demo_xmake_nav_plugin::GradientLayer, nav2_costmap_2d::Layer)
