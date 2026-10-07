cmake_minimum_required(VERSION 3.8)
project(my_nav2_costmap_2d)

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(my_nav2_common REQUIRED)
find_package(my_nav2_util REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(nav_msgs REQUIRED)
find_package(map_msgs REQUIRED)
find_package(my_nav2_msgs REQUIRED)
find_package(tf2_geometry_msgs REQUIRED)
find_package(tf2 REQUIRED)
find_package(tf2_ros REQUIRED)
find_package(pluginlib REQUIRED)

my_nav2_package()

add_library(${PROJECT_NAME}_core SHARED
  src/costmap_math.cpp
  src/costmap_2d.cpp
  src/layer.cpp 
  src/footprint.cpp 
  src/layered_costmap.cpp
  src/footprint_collision_checker.cpp
  src/costmap_layer.cpp
  src/costmap_2d_publisher.cpp
  src/clear_costmap_service.cpp
  src/costmap_2d_ros.cpp
)
add_library(${PROJECT_NAME}::${PROJECT_NAME}_core ALIAS ${PROJECT_NAME}_core)
target_include_directories(${PROJECT_NAME}_core PUBLIC
  "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
  "$<INSTALL_INTERFACE:include/${PROJECT_NAME}>"
)

set(dependencies 
  geometry_msgs
  nav_msgs
  my_nav2_util
  map_msgs
  my_nav2_msgs
  tf2_geometry_msgs
  tf2
  tf2_ros
  pluginlib
)

ament_target_dependencies(${PROJECT_NAME}_core
  ${dependencies}
)

add_library(layers SHARED
  plugins/static_layer.cpp
  plugins/inflation_layer.cpp
)
add_library(${PROJECT_NAME}::layers ALIAS layers)
ament_target_dependencies(layers
  ${dependencies}
)
target_link_libraries(layers
  ${PROJECT_NAME}::${PROJECT_NAME}_core
)

install(TARGETS 
  ${PROJECT_NAME}_core 
  layers
  EXPORT export_${PROJECT_NAME}
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
)

install(DIRECTORY include/
  DESTINATION include/${PROJECT_NAME}
)

ament_export_targets(export_${PROJECT_NAME} HAS_LIBRARY_TARGET)
ament_export_dependencies(${dependencies})
pluginlib_export_plugin_description_file(my_nav2_costmap_2d costmap_plugins.xml)

ament_package()
