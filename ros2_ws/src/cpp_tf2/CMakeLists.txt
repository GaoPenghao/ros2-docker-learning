cmake_minimum_required(VERSION 3.8)
project(cpp_tf2)

if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(tf2 REQUIRED)
find_package(tf2_ros REQUIRED)
find_package(geometry_msgs REQUIRED)

# =========================================================
# library
# =========================================================
add_library(cpp_tf2_static_broadcaster
  src/static_broadcaster.cpp
)
target_include_directories(cpp_tf2_static_broadcaster PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)
ament_target_dependencies(cpp_tf2_static_broadcaster
  rclcpp
  tf2
  tf2_ros
  geometry_msgs
)

add_library(cpp_tf2_dynamic_broadcaster
  src/dynamic_broadcaster.cpp
)
target_include_directories(cpp_tf2_dynamic_broadcaster PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)
ament_target_dependencies(cpp_tf2_dynamic_broadcaster
  rclcpp
  tf2
  tf2_ros
  geometry_msgs
)

add_library(cpp_tf2_listener
  src/tf2_listener.cpp
)
target_include_directories(cpp_tf2_listener PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)
ament_target_dependencies(cpp_tf2_listener
  rclcpp
  tf2
  tf2_ros
  geometry_msgs
)

# =========================================================
# Executables
# =========================================================
add_executable(static_broadcaster src/static_broadcaster_main.cpp)
target_link_libraries(static_broadcaster cpp_tf2_static_broadcaster)

add_executable(dynamic_broadcaster src/dynamic_broadcaster_main.cpp)
target_link_libraries(dynamic_broadcaster cpp_tf2_dynamic_broadcaster)

add_executable(tf2_listener src/tf2_listener_main.cpp)
target_link_libraries(tf2_listener cpp_tf2_listener)

# =========================================================
# Install
# =========================================================
install(TARGETS
  cpp_tf2_static_broadcaster
  cpp_tf2_dynamic_broadcaster
  cpp_tf2_listener
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION lib/${PROJECT_NAME}
)

install(TARGETS
  static_broadcaster
  dynamic_broadcaster
  tf2_listener
  DESTINATION lib/${PROJECT_NAME}
)

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
