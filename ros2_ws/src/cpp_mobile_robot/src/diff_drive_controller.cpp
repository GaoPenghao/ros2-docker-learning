#include "cpp_mobile_robot/diff_drive_controller.hpp"

#include <chrono>

namespace learning_ros2::cpp_mobile_robot {

DiffDriveController::DiffDriveController(const rclcpp::NodeOptions& options)
    : rclcpp::Node("diff_drive_controller", options) {
  this->declare_parameter("wheel_radius", 0.05);
  this->declare_parameter("wheel_separation_real", 0.34);
  this->declare_parameter("wheel_separation_est", 0.34);

  wheel_radius_ = this->get_parameter("wheel_radius").as_double();
  wheel_separation_real_ =
      this->get_parameter("wheel_separation_real").as_double();
  wheel_separation_est_ =
      this->get_parameter("wheel_separation_est").as_double();

  last_time_ = this->get_clock()->now();

  cmd_vel_sub_ = this->create_subscription<geometry_msgs::msg::Twist>(
      "cmd_vel", 10,
      std::bind(&DiffDriveController::cmd_vel_callback, this,
                std::placeholders::_1));

  joint_pub_ =
      this->create_publisher<sensor_msgs::msg::JointState>("joint_states", 10);
  odom_pub_ = this->create_publisher<nav_msgs::msg::Odometry>("odom", 10);

  tf_broadcaster_ = std::make_shared<tf2_ros::TransformBroadcaster>(this);

  timer_ = this->create_wall_timer(
      std::chrono::duration<double>(dt_),
      std::bind(&DiffDriveController::update_callback, this));

  RCLCPP_INFO(
      this->get_logger(),
      "Diff drive controller started: wheel_radius=%.3f, real=%.3f, est=%.3f",
      wheel_radius_, wheel_separation_real_, wheel_separation_est_);
}

void DiffDriveController::cmd_vel_callback(
    const geometry_msgs::msg::Twist::SharedPtr msg) {
  v_ = msg->linear.x;
  omega_ = msg->angular.z;
}

void DiffDriveController::update_callback() {
  rclcpp::Time current_time = this->get_clock()->now();
  double dt = (current_time - last_time_).seconds();
  last_time_ = current_time;
  if (dt <= 0.0) {
    return;
  }

  double v_left = v_ - omega_ * wheel_separation_est_ / 2.0;
  double v_right = v_ + omega_ * wheel_separation_est_ / 2.0;

  double v_real = (v_left + v_right) / 2.0;
  double omega_real = (v_right - v_left) / wheel_separation_real_;
  x_real_ += v_real * std::cos(theta_real_) * dt;
  y_real_ += v_real * std::sin(theta_real_) * dt;
  theta_real_ += omega_real * dt;

  double v_est = (v_left + v_right) / 2.0;
  double omega_est = (v_right - v_left) / wheel_separation_est_;
  x_est_ += v_est * std::cos(theta_est_) * dt;
  y_est_ += v_est * std::sin(theta_est_) * dt;
  theta_est_ += omega_est * dt;

  left_wheel_angle_ += (v_left / wheel_radius_) * dt;
  right_wheel_angle_ += (v_right / wheel_radius_) * dt;

  publish_joint_states(current_time);
  publish_odom(current_time);
}

void DiffDriveController::publish_joint_states(
    const rclcpp::Time& current_time) {
  sensor_msgs::msg::JointState msg;
  msg.header.stamp = current_time;
  msg.name = {"base_to_left_wheel", "base_to_right_wheel"};
  msg.position = {left_wheel_angle_, right_wheel_angle_};
  joint_pub_->publish(msg);
}

void DiffDriveController::publish_odom(const rclcpp::Time& current_time) {
  geometry_msgs::msg::Quaternion q_real;
  q_real.x = 0.0;
  q_real.y = 0.0;
  q_real.z = std::sin(theta_real_ / 2.0);
  q_real.w = std::cos(theta_real_ / 2.0);

  geometry_msgs::msg::TransformStamped t;
  t.header.stamp = current_time;
  t.header.frame_id = "odom";
  t.child_frame_id = "base_link";
  t.transform.translation.x = x_real_;
  t.transform.translation.y = y_real_;
  t.transform.translation.z = 0.0;
  t.transform.rotation = q_real;
  tf_broadcaster_->sendTransform(t);

  geometry_msgs::msg::Quaternion q_est;
  q_est.x = 0.0;
  q_est.y = 0.0;
  q_est.z = std::sin(theta_est_ / 2.0);
  q_est.w = std::cos(theta_est_ / 2.0);

  nav_msgs::msg::Odometry odom;
  odom.header.stamp = current_time;
  odom.header.frame_id = "odom";
  odom.child_frame_id = "base_link";
  odom.pose.pose.position.x = x_est_;
  odom.pose.pose.position.y = y_est_;
  odom.pose.pose.position.z = 0.0;
  odom.pose.pose.orientation = q_est;
  odom.twist.twist.linear.x = v_;
  odom.twist.twist.angular.z = omega_;
  odom_pub_->publish(odom);
}

}  // namespace learning_ros2::cpp_mobile_robot
