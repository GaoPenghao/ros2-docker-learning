cmake_minimum_required(VERSION 3.8)
project(my_nav2_util)

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(my_nav2_common REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(nav_msgs REQUIRED)
find_package(tf2 REQUIRED)
find_package(tf2_geometry_msgs REQUIRED)
find_package(rclcpp REQUIRED)
find_package(tf2_ros REQUIRED)
find_package(rcl_interfaces REQUIRED)
find_package(bond REQUIRED)
find_package(bondcpp REQUIRED)
find_package(lifecycle_msgs REQUIRED)
find_package(rclcpp_lifecycle REQUIRED)
find_package(rclcpp_action REQUIRED)

my_nav2_package()

set(library_name ${PROJECT_NAME}_core)

add_subdirectory(src)

install(DIRECTORY include/
  DESTINATION include/${PROJECT_NAME}
)

ament_export_include_directories(include/${PROJECT_NAME})
ament_export_libraries(${library_name})
ament_export_dependencies(
  geometry_msgs
  nav_msgs
  tf2
  tf2_geometry_msgs
  rclcpp
  tf2_ros
  rcl_interfaces
  bondcpp
  lifecycle_msgs
  rclcpp_lifecycle
  rclcpp_action
)
ament_export_targets(${library_name})

ament_package()
