#include "cpp_mobile_robot/diff_drive_controller.hpp"

int main(int argc, char* argv[]) {
  rclcpp::init(argc, argv);
  auto node =
      std::make_shared<learning_ros2::cpp_mobile_robot::DiffDriveController>();
  rclcpp::spin(node);
  rclcpp::shutdown();

  return 0;
}
