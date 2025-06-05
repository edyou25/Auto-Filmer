cmake_minimum_required(VERSION 3.0.2)
project(traj_opt)

add_compile_options(-std=c++14)
option(DMODE "Build in Debug mode" ON)
message(STATUS "Build in Debug mode: ${DMODE}")

if(DMODE)
  set(CMAKE_BUILD_TYPE Debug)
  set(CMAKE_CXX_FLAGS "-g -O0 -Wall")
else()
  set(CMAKE_BUILD_TYPE Release)
  set(CMAKE_CXX_FLAGS "-O3 -Wall")
endif()

find_package(Eigen3 REQUIRED)
find_package(catkin REQUIRED COMPONENTS
  roscpp
  quadrotor_msgs
)

catkin_package(
  INCLUDE_DIRS include ${EIGEN3_INCLUDE_DIR}
  LIBRARIES ${PROJECT_NAME}
  CATKIN_DEPENDS roscpp quadrotor_msgs
  DEPENDS EIGEN3
)

include_directories(
  include
  ${EIGEN3_INCLUDE_DIR}
  ${catkin_INCLUDE_DIRS}
)

add_library(${PROJECT_NAME}
  src/${PROJECT_NAME}.cc
#   src/${PROJECT_NAME}_fake.cc
)

target_link_libraries(${PROJECT_NAME}
  ${catkin_LIBRARIES}
)
