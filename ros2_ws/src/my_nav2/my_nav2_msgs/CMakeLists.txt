cmake_minimum_required(VERSION 3.8)
project(my_nav2_msgs)

if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
# uncomment the following section in order to fill in
# further dependencies manually.
# find_package(<dependency> REQUIRED)
find_package(builtin_interfaces REQUIRED)
find_package(nav_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(rosidl_default_generators REQUIRED)
find_package(std_msgs REQUIRED)

rosidl_generate_interfaces(${PROJECT_NAME}
  # msg
  "msg/Particle.msg"
  "msg/ParticleCloud.msg"
  "msg/CostmapMetaData.msg"
  "msg/Costmap.msg"
  "msg/CostmapUpdate.msg"
  "msg/VoxelGrid.msg"
  "msg/CostmapFilterInfo.msg"
  "msg/MissedWaypoint.msg"
  # srv
  "srv/GetCost.srv"
  "srv/ClearCostmapAroundPose.srv"
  "srv/ClearCostmapAroundRobot.srv"
  "srv/ClearCostmapExceptRegion.srv"
  "srv/ClearEntireCostmap.srv"
  "srv/GetCostmap.srv"
  # action
  "action/NavigateToPose.action"
  "action/ComputePathToPose.action"
  "action/FollowPath.action"
  "action/Spin.action"
  "action/BackUp.action"
  "action/Wait.action"
  "action/DriveOnHeading.action"
  "action/DockRobot.action"
  "action/UndockRobot.action"
  "action/ComputePathThroughPoses.action"
  "action/NavigateThroughPoses.action"
  "action/SmoothPath.action"
  "action/FollowWaypoints.action"
  "action/AssistedTeleop.action"
  "action/DummyBehavior.action"

  DEPENDENCIES 
    std_msgs 
    geometry_msgs 
    nav_msgs 
    builtin_interfaces
)

ament_export_dependencies(rosidl_default_runtime)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # comment the line when a copyright and license is added to all source files
  set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # comment the line when this package is in a git repo and when
  # a copyright and license is added to all source files
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
