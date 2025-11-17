cmake_minimum_required(VERSION 3.8)
project(ft_panel)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(pluginlib REQUIRED)
find_package(rviz_common REQUIRED)
find_package(Qt5 REQUIRED COMPONENTS Widgets)

# Für Q_OBJECT in Headern
set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
qt5_wrap_cpp(MOC_FILES
  include/ft_panel/ft_panel.hpp
)

include_directories(
  include
)

add_library(${PROJECT_NAME} SHARED
  src/ft_panel.cpp
  ${MOC_FILES}
)

target_include_directories(${PROJECT_NAME} PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

ament_target_dependencies(${PROJECT_NAME}
  rclcpp
  geometry_msgs
  pluginlib
  rviz_common
)

target_link_libraries(ft_panel Qt5::Widgets)
# Plugin-Beschreibung für RViz/Pluginlib
pluginlib_export_plugin_description_file(rviz_common rviz_plugin.xml)



install(TARGETS ${PROJECT_NAME}
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
)

install(DIRECTORY include/
  DESTINATION include
)

install(FILES rviz_plugin.xml
  DESTINATION share/${PROJECT_NAME}
)

ament_package()

