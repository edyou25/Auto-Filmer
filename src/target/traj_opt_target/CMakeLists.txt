cmake_minimum_required(VERSION 2.8.3)
project(traj_opt_target)

option(DMODE "Build in Debug mode" ON)
message(STATUS "Build in Debug mode: ${DMODE}")

if(DMODE)
  set(CMAKE_BUILD_TYPE Debug)
  set(CMAKE_CXX_FLAGS "-g -O0 -Wall")
else()
  set(CMAKE_BUILD_TYPE Release)
  set(CMAKE_CXX_FLAGS "-O3 -Wall")
endif()
ADD_COMPILE_OPTIONS(-std=c++11 )
ADD_COMPILE_OPTIONS(-std=c++14 )

find_package(Eigen3 REQUIRED)
find_package(PCL 1.7 REQUIRED)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  std_msgs
  visualization_msgs
  plan_env
  cv_bridge
  traj_utils
  path_searching
)

catkin_package(
 INCLUDE_DIRS include
 LIBRARIES traj_opt_target
 CATKIN_DEPENDS plan_env 
#  DEPENDS system_lib
)

include_directories( 
    SYSTEM 
    include 
    ${catkin_INCLUDE_DIRS}
    ${Eigen3_INCLUDE_DIRS} 
    ${PCL_INCLUDE_DIRS}
)

add_library( traj_opt_target 
    src/poly_traj_optimizer.cpp 
    )
target_link_libraries( traj_opt_target
    ${catkin_LIBRARIES} 
    )  
