cmake_minimum_required(VERSION 3.8)
project(cpp_msg)

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
find_package(tutorial_interfaces REQUIRED)

# =========================================================
# Publisher library
# =========================================================
add_library(cpp_msg_publisher
  src/robot_status_publisher.cpp
)
target_include_directories(cpp_msg_publisher PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)
ament_target_dependencies(cpp_msg_publisher
  rclcpp
  tutorial_interfaces
)

# =========================================================
# Subscriber library
# =========================================================
add_library(cpp_msg_subscriber
  src/robot_status_subscriber.cpp
)
target_include_directories(cpp_msg_subscriber PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)
ament_target_dependencies(cpp_msg_subscriber
  rclcpp
  tutorial_interfaces
)

# =========================================================
# Executables
# =========================================================
add_executable(robot_status_publisher src/robot_status_publisher_main.cpp)
target_link_libraries(robot_status_publisher cpp_msg_publisher)

add_executable(robot_status_subscriber src/robot_status_subscriber_main.cpp)
target_link_libraries(robot_status_subscriber cpp_msg_subscriber)

# =========================================================
# Install
# =========================================================
install(TARGETS
  cpp_msg_publisher
  cpp_msg_subscriber
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION lib/${PROJECT_NAME}
)

install(TARGETS
  robot_status_publisher
  robot_status_subscriber
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
