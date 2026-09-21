cmake_minimum_required(VERSION 3.8)
project(cpp_srvcli)

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

# =======================================================
# Service library
# =======================================================
add_library(cpp_srvcli_service
  src/service_node.cpp
)
target_include_directories(cpp_srvcli_service PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)
ament_target_dependencies(cpp_srvcli_service
  rclcpp
  tutorial_interfaces
)

# =======================================================
# Client library
# =======================================================
add_library(cpp_srvcli_client
  src/client_node.cpp
)
target_include_directories(cpp_srvcli_client PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)
ament_target_dependencies(cpp_srvcli_client
  rclcpp
  tutorial_interfaces
)

# =======================================================
# Executables
# =======================================================
add_executable(service src/service_main.cpp)
target_link_libraries(service cpp_srvcli_service)

add_executable(client src/client_main.cpp)
target_link_libraries(client cpp_srvcli_client)

# =======================================================
# Install
# =======================================================
install(TARGETS 
  cpp_srvcli_service
  cpp_srvcli_client
  ARCHIVE DESTINATION lib 
  LIBRARY DESTINATION lib 
  RUNTIME DESTINATION lib/${PROJECT_NAME}
)

install(TARGETS 
  service
  client 
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
