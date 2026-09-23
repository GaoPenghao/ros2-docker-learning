#include "cpp_tf2/static_broadcaster.hpp"

namespace learning_ros2::cpp_tf2 {

StaticFrameBroadcaster::StaticFrameBroadcaster(
    const rclcpp::NodeOptions& options)
    : rclcpp::Node("static_frame_broadcaster", options) {
  tf_broadcaster_ = std::make_shared<tf2_ros::StaticTransformBroadcaster>(this);
  publish_static_transform();
}

void StaticFrameBroadcaster::publish_static_transform() {
  geometry_msgs::msg::TransformStamped t;

  t.header.stamp = this->get_clock()->now();
  t.header.frame_id = "base_link";
  t.child_frame_id = "laser_frame";

  t.transform.translation.x = 0.2;
  t.transform.translation.y = 0.0;
  t.transform.translation.z = 0.15;

  t.transform.rotation.x = 0.0;
  t.transform.rotation.y = 0.0;
  t.transform.rotation.z = 0.0;
  t.transform.rotation.w = 1.0;

  tf_broadcaster_->sendTransform(t);

  RCLCPP_INFO(this->get_logger(),
              "Published static transform: base_link -> laser_frame");
}

}  // namespace learning_ros2::cpp_tf2
