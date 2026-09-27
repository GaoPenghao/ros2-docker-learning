cmake_minimum_required(VERSION 3.8)
project(my_nav2_common NONE)

find_package(ament_cmake_core REQUIRED)
find_package(ament_cmake_python REQUIRED)

ament_python_install_package(my_nav2_common)

ament_package(
  CONFIG_EXTRAS "my_nav2_common-extras.cmake"
)

install(
  DIRECTORY cmake
  DESTINATION share/${PROJECT_NAME}
)
