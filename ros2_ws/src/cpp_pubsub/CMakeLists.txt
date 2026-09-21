cmake_minimum_required(VERSION 3.8)
project(cpp_pubsub)

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
find_package(std_msgs REQUIRED)

# ==========================================
# Publisher library
# ==========================================
add_library(cpp_pubsub_publisher
  src/publisher_node.cpp
)
target_include_directories(cpp_pubsub_publisher PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)
ament_target_dependencies(cpp_pubsub_publisher
  rclcpp
  std_msgs
)

# ==========================================
# Subscriber liberary
# ==========================================
add_library(cpp_pubsub_subscriber
  src/subscriber_node.cpp
)
target_include_directories(cpp_pubsub_subscriber PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)
ament_target_dependencies(cpp_pubsub_subscriber
  rclcpp
  std_msgs
)

# ==========================================
# Executabls
# ==========================================
add_executable(talker src/publisher_main.cpp)
target_link_libraries(talker cpp_pubsub_publisher)

add_executable(listener src/subscriber_main.cpp)
target_link_libraries(listener cpp_pubsub_subscriber)

# ==========================================
# Install
# ==========================================
install(TARGETS
  cpp_pubsub_publisher
  cpp_pubsub_subscriber
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION lib/${PROJECT_NAME}
)

install(TARGETS
  talker
  listener
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
