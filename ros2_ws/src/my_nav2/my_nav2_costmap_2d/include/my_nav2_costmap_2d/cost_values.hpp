#ifndef MY_NAV2_COSTMAP_2D__COST_VALUES_HPP_
#define MY_NAV2_COSTMAP_2D__COST_VALUES_HPP_

namespace my_nav2::costmap_2d {

/**
 * @enum my_nav2::costmap_2d::CombinationMethod
 * @brief Describes the method used to add data to master costmap, default to
 * maximum.
 */
enum class CombinationMethod : int {
  /**
   * Overwrite means every valid value from this layer
   * is written into the master grid (does not copy NO_INFORMATION)
   */
  Overwrite = 0,
  /**
   * Sets the new value to the maximum of the master_grid's value
   * and this layer's value. If the master value is NO_INFORMATION,
   * it is overwritten. If the layer's value is NO_INFORMATION,
   * the master value does not change
   */
  Max = 1,
  /**
   * Sets the new value to the maximum of the master_grid's value
   * and this layer's value. If the master value is NO_INFORMATION,
   * it is NOT overwritten. If the layer's value is NO_INFORMATION,
   * the master value does not change.
   */
  MaxWithoutUnknownOverwrite = 2
};

static constexpr unsigned char NO_INFORMATION = 255;
static constexpr unsigned char LETHAL_OBSTACLE = 254;
static constexpr unsigned char INSCRIBED_INFLATED_OBSTACLE = 253;
static constexpr unsigned char MAX_NON_OBSTACLE = 252;
static constexpr unsigned char FREE_SPACE = 0;

}  // namespace my_nav2::costmap_2d

#endif  // MY_NAV2_COSTMAP_2D__COST_VALUES_HPP_
