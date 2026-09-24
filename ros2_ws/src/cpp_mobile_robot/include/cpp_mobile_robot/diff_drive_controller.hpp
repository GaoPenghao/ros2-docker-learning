#ifndef CPP_MOBILE_ROBOT__DIFF_DRIVE_CONTROLLER_HPP_
#define CPP_MOBILE_ROBOT__DIFF_DRIVE_CONTROLLER_HPP_

#include <memory>
#include <string>

#include "geometry_msgs/msg/transform_stamped.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include "nav_msgs/msg/odometry.hpp"
#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/joint_state.hpp"
#include "tf2_ros/transform_broadcaster.hpp"

namespace learning_ros2::cpp_mobile_robot {

class DiffDriveController : public rclcpp::Node {
 public:
  explicit DiffDriveController(
      const rclcpp::NodeOptions& options = rclcpp::NodeOptions());

 private:
  void cmd_vel_callback(const geometry_msgs::msg::Twist::SharedPtr msg);
  void update_callback();
  void publish_joint_states(const rclcpp::Time& current_time);
  void publish_odom(const rclcpp::Time& current_time);

 private:
  double wheel_radius_;
  double wheel_separation_real_;
  double wheel_separation_est_;
  double x_real_{0.0};
  double y_real_{0.0};
  double theta_real_{0.0};
  double x_est_{0.0};
  double y_est_{0.0};
  double theta_est_{0.0};
  double left_wheel_angle_{0.0};
  double right_wheel_angle_{0.0};
  double v_{0.0};
  double omega_{0.0};
  double dt_{0.05};
  rclcpp::Time last_time_;

  rclcpp::Subscription<geometry_msgs::msg::Twist>::SharedPtr cmd_vel_sub_;
  rclcpp::Publisher<sensor_msgs::msg::JointState>::SharedPtr joint_pub_;
  rclcpp::Publisher<nav_msgs::msg::Odometry>::SharedPtr odom_pub_;
  std::shared_ptr<tf2_ros::TransformBroadcaster> tf_broadcaster_;
  rclcpp::TimerBase::SharedPtr timer_;
};

}  // namespace learning_ros2::cpp_mobile_robot

#endif