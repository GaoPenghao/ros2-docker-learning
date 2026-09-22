cmake_minimum_required(VERSION 3.8)
project(cpp_action)

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
find_package(rclcpp_action REQUIRED)
find_package(tutorial_interfaces REQUIRED)

# ==============================================================
# Action library
# ==============================================================
add_library(cpp_action_server 
  src/fibonacci_action_server.cpp 
)
target_include_directories(cpp_action_server PUBLIC 
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)
ament_target_dependencies(cpp_action_server
  rclcpp
  rclcpp_action
  tutorial_interfaces
)

add_library(cpp_action_client
  src/fibonacci_action_client.cpp
)
target_include_directories(cpp_action_client PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)
ament_target_dependencies(cpp_action_client
  rclcpp
  rclcpp_action
  tutorial_interfaces
)

# =========================================================
# Executable
# =========================================================
add_executable(fibonacci_action_server src/fibonacci_action_server_main.cpp)
target_link_libraries(fibonacci_action_server cpp_action_server)

add_executable(fibonacci_action_client src/fibonacci_action_client_main.cpp)
target_link_libraries(fibonacci_action_client cpp_action_client)

# =========================================================
# Install
# =========================================================
install(TARGETS
  cpp_action_server
  cpp_action_client
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION lib/${PROJECT_NAME}
)

install(TARGETS
  fibonacci_action_server
  fibonacci_action_client
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
