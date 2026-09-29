add_library(${library_name} SHARED 
  string_utils.cpp 
  robot_utils.cpp 
  node_thread.cpp 
  array_parser.cpp 
  node_utils.cpp 
  lifecycle_node.cpp 
)
target_include_directories(${library_name} PUBLIC 
  "$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>"
  "$<INSTALL_INTERFACE:include/${PROJECT_NAME}>"
)
target_link_libraries(${library_name} PUBLIC 
  ${geometry_msgs_TARGETS}
  ${nav_msgs_TARGETS}
  ${tf2_geometry_msgs_TARGETS}
  rclcpp::rclcpp
  tf2::tf2
  tf2_ros::tf2_ros
  ${rcl_interfaces_TARGETS}
  bondcpp::bondcpp
  rclcpp_lifecycle::rclcpp_lifecycle
  ${lifecycle_msgs_TARGETS}
  rclcpp_action::rclcpp_action
)
target_link_libraries(${library_name} PRIVATE
  ${bond_TARGETS}
)
install(TARGETS ${library_name}
  EXPORT ${library_name}
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
)
