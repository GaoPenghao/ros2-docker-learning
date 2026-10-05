#include "my_nav2_costmap_2d/costmap_layer.hpp"

#include <algorithm>
#include <mutex>
#include <stdexcept>

#include "my_nav2_costmap_2d/cost_values.hpp"
#include "my_nav2_costmap_2d/layered_costmap.hpp"
#include "rclcpp/rclcpp.hpp"

namespace my_nav2::costmap_2d {

void CostmapLayer::matchSize() {
  std::lock_guard<Costmap2D::mutex_t> guard(*getMutex());
  Costmap2D* master = layered_costmap_->getCostmap();
  if (!master) {
    RCLCPP_WARN(
        rclcpp::get_logger("my_nav2_costmap_2d"),
        "Cannot match size for layer, master costmap is not initialized yet.");
    return;
  }
  resizeMap(master->getSizeInCellsX(), master->getSizeInCellsY(),
            master->getResolution(), master->getOriginX(),
            master->getOriginY());
}

void CostmapLayer::clearArea(int start_x, int start_y, int end_x, int end_y,
                             bool invert) {
  current_ = false;
  unsigned char* grid = getCharMap();
  for (int x = 0; x < static_cast<int>(getSizeInCellsX()); x++) {
    bool xrange = x > start_x && x < end_x;

    for (int y = 0; y < static_cast<int>(getSizeInCellsY()); y++) {
      if ((xrange && y > start_y && y < end_y) == invert) {
        continue;
      }
      int index = getIndex(x, y);
      if (grid[index] != NO_INFORMATION) {
        grid[index] = NO_INFORMATION;
      }
    }
  }
}

void CostmapLayer::addExtraBounds(double mx0, double my0, double mx1,
                                  double my1) {
  extra_min_x_ = std::min(mx0, extra_min_x_);
  extra_max_x_ = std::max(mx1, extra_max_x_);
  extra_min_y_ = std::min(my0, extra_min_y_);
  extra_max_y_ = std::max(my1, extra_max_y_);
  has_extra_bounds_ = true;
}

void CostmapLayer::updateWithTrueOverwrite(Costmap2D& master_grid, int min_i,
                                           int min_j, int max_i, int max_j) {
  if (!enabled_) {
    return;
  }

  if (costmap_ == nullptr) {
    throw std::runtime_error(
        "Can't update costmap layer: It has't been initialized yet!");
  }

  unsigned char* master = master_grid.getCharMap();
  unsigned int span = master_grid.getSizeInCellsX();

  for (int j = min_j; j < max_j; j++) {
    unsigned int it = span * j + min_i;
    for (int i = min_i; i < max_i; i++, it++) {
      master[it] = costmap_[it];
    }
  }
}

void CostmapLayer::updateWithOverwrite(Costmap2D& master_grid, int min_i,
                                       int min_j, int max_i, int max_j) {
  if (!enabled_) {
    return;
  }
  unsigned char* master = master_grid.getCharMap();
  unsigned int span = master_grid.getSizeInCellsX();

  for (int j = min_j; j < max_j; j++) {
    unsigned int it = span * j + min_i;
    for (int i = min_i; i < max_i; i++, it++) {
      if (costmap_[it] != NO_INFORMATION) {
        master[it] = costmap_[it];
      }
    }
  }
}

void CostmapLayer::updateWithMax(Costmap2D& master_grid, int min_i, int min_j,
                                 int max_i, int max_j) {
  if (!enabled_) {
    return;
  }

  unsigned char* master_array = master_grid.getCharMap();
  unsigned int span = master_grid.getSizeInCellsX();

  for (int j = min_j; j < max_j; j++) {
    unsigned int it = j * span + min_i;
    for (int i = min_i; i < max_i; i++, it++) {
      if (costmap_[it] == NO_INFORMATION) {
        continue;
      }

      unsigned char old_cost = master_array[it];
      if (old_cost == NO_INFORMATION || old_cost < costmap_[it]) {
        master_array[it] = costmap_[it];
      }
    }
  }
}

void CostmapLayer::updateWithMaxWithoutUnknownOverwrite(Costmap2D& master_grid,
                                                        int min_i, int min_j,
                                                        int max_i, int max_j) {
  if (!enabled_) {
    return;
  }

  unsigned char* master_array = master_grid.getCharMap();
  unsigned int span = master_grid.getSizeInCellsX();

  for (int j = min_j; j < max_j; j++) {
    unsigned int it = j * span + min_i;
    for (int i = min_i; i < max_i; i++, it++) {
      if (costmap_[it] == NO_INFORMATION) {
        continue;
      }

      unsigned char old_cost = master_array[it];
      if (old_cost != NO_INFORMATION && old_cost < costmap_[it]) {
        master_array[it] = costmap_[it];
      }
    }
  }
}

void CostmapLayer::updateWithAddition(Costmap2D& master_grid, int min_i,
                                      int min_j, int max_i, int max_j) {
  if (!enabled_) {
    return;
  }
  unsigned char* master_array = master_grid.getCharMap();
  unsigned int span = master_grid.getSizeInCellsX();

  for (int j = min_j; j < max_j; j++) {
    unsigned int it = j * span + min_i;
    for (int i = min_i; i < max_i; i++, it++) {
      if (costmap_[it] == NO_INFORMATION) {
        continue;
      }

      unsigned char old_cost = master_array[it];
      if (old_cost == NO_INFORMATION) {
        master_array[it] = costmap_[it];
      } else {
        int sum = old_cost + costmap_[it];
        if (sum >= INSCRIBED_INFLATED_OBSTACLE) {
          master_array[it] = INSCRIBED_INFLATED_OBSTACLE - 1;
        } else {
          master_array[it] = sum;
        }
      }
    }
  }
}

void CostmapLayer::touch(double x, double y, double* min_x, double* min_y,
                         double* max_x, double* max_y) {
  *min_x = std::min(x, *min_x);
  *min_y = std::min(y, *min_y);
  *max_x = std::max(x, *max_x);
  *max_y = std::max(y, *max_y);
}

void CostmapLayer::useExtraBounds(double* min_x, double* min_y, double* max_x,
                                  double* max_y) {
  if (!has_extra_bounds_) {
    return;
  }

  *min_x = std::min(extra_min_x_, *min_x);
  *min_y = std::min(extra_min_y_, *min_y);
  *max_x = std::max(extra_max_x_, *max_x);
  *max_y = std::max(extra_max_y_, *max_y);
  extra_min_x_ = 1e6;
  extra_min_y_ = 1e6;
  extra_max_x_ = -1e6;
  extra_max_y_ = -1e6;
  has_extra_bounds_ = false;
}

CombinationMethod CostmapLayer::combination_method_from_int(const int value) {
  switch (value) {
    case 0:
      return CombinationMethod::Overwrite;
    case 1:
      return CombinationMethod::Max;
    case 2:
      return CombinationMethod::MaxWithoutUnknownOverwrite;
    default:
      RCLCPP_WARN(
          logger_,
          "Param combination_method: %i. Possible values are 0 (Overwrite) or "
          "1 (Maximum) or "
          "2 (Maximum without overwriting the master's NO_INFORMATION values)."
          "The default value 1 will be used",
          value);
      return CombinationMethod::Max;
  }
}

}  // namespace my_nav2::costmap_2d
