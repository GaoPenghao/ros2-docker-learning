#ifndef MY_NAV2_UTIL__EXECUTION_TIMER_HPP_
#define MY_NAV2_UTIL__EXECUTION_TIMER_HPP_

#include <chrono>

namespace my_nav2::util {

/**
 * @brief Measures execution time of code between calls to start and end
 */
class ExecutionTimer {
 public:
  using Clock = std::chrono::high_resolution_clock;
  using nanoseconds = std::chrono::nanoseconds;

  /**
   * @brief Call just prior to code you want to measure
   */
  void start() { start_ = Clock::now(); }

  /**
   * @brief Call just after the code you want to measure
   */
  void end() { end_ = Clock::now(); }

  /**
   * @brief Extract the measured time as an integral std::chrono::duration
   * object
   */
  nanoseconds elapsed_time() { return end_ - start_; }

  /**
   * @brief Extract the measured time as a floating point number of seconds.
   */
  double elapsed_time_in_seconds() {
    return std::chrono::duration<double>(end_ - start_).count();
  }

 protected:
  Clock::time_point start_;
  Clock::time_point end_;
};

}  // namespace my_nav2::util

#endif  // MY_NAV2_UTIL__EXECUTION_TIMER_HPP_
