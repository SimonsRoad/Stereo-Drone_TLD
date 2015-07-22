cmake_minimum_required(VERSION 2.8.3)
project(husky_gazebo)
#include(ExternalProject)
#set(custom_objects_gazebo_PREFIX "${CMAKE_CURRENT_BINARY_DIR}/custom_objects_gazebo")
#set(custom_objects_gazebo_INSTALL_DIR "${CMAKE_CURRENT_BINARY_DIR}/custom_objects_gazebo")
#set(custom_objects_gazebo_CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${custom_objects_gazebo_INSTALL_DIR})

#ExternalProject_Add(custom_objects_gazebo
#  PREFIX ${custom_objects_gazebo_PREFIX}
 # GIT_REPOSITORY https://github.com/HALLAB-Halifax/custom_objects_gazebo.git
 # INSTALL_DIR ${custom_objects_gazebo_INSTALL_DIR}
 # CMAKE_ARGS ${custom_objects_gazebo_CMAKE_ARGS}
#)
## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED)

catkin_package(
)

install(
  DIRECTORY launch config
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
)

