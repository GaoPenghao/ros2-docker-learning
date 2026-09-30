#ifndef MY_NAV2_UTIL__OCC_GRID_VALUES_HPP_
#define MY_NAV2_UTIL__OCC_GRID_VALUES_HPP_

#include <cstdint>

namespace my_nav2::util {

/**
 * @brief OccupancyGrid data constants
 */
static constexpr std::int8_t OCC_GRID_UNKNOWN = -1;
static constexpr int8_t OCC_GRID_FREE = 0;
static constexpr int8_t OCC_GRID_OCCUPIED = 100;

}  // namespace my_nav2::util

#endif  // MY_NAV2_UTIL__OCC_GRID_VALUES_HPP_
