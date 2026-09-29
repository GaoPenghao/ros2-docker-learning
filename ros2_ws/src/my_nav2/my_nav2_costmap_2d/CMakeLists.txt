cmake_minimum_required(VERSION 3.8)
project(my_nav2_costmap_2d)

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(my_nav2_common REQUIRED)

my_nav2_package()

add_library(${PROJECT_NAME}_core SHARED
  src/costmap_math.cpp
)
add_library(${PROJECT_NAME}::${PROJECT_NAME}_core ALIAS ${PROJECT_NAME}_core)
target_include_directories(${PROJECT_NAME}_core PUBLIC
  "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
  "$<INSTALL_INTERFACE:include/${PROJECT_NAME}>"
)

install(TARGETS ${PROJECT_NAME}_core
  EXPORT export_${PROJECT_NAME}
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
)

install(DIRECTORY include/
  DESTINATION include/${PROJECT_NAME}
)

ament_export_targets(export_${PROJECT_NAME} HAS_LIBRARY_TARGET)
ament_export_include_directories(include/${PROJECT_NAME})

ament_package()
