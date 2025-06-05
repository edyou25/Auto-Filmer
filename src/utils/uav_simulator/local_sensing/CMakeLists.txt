PROJECT(local_sensing_node)
CMAKE_MINIMUM_REQUIRED(VERSION 2.8.3)
option(DMODE "Build in Debug mode" ON)
message(STATUS "Build in Debug mode: ${DMODE}")

if(DMODE)
  set(CMAKE_BUILD_TYPE Debug)
  set(CMAKE_CXX_FLAGS "-g -O0 -Wall")
else()
  set(CMAKE_BUILD_TYPE Release)
  set(CMAKE_CXX_FLAGS "-O3 -Wall")
endif()#SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
ADD_COMPILE_OPTIONS(-std=c++11 )
ADD_COMPILE_OPTIONS(-std=c++14 )

#set(ENABLE_CUDA false)
set(ENABLE_CUDA true)

if(ENABLE_CUDA)
  find_package(CUDA REQUIRED)
  SET(CUDA_NVCC_FLAGS ${CUDA_NVCC_FLAGS};-O3 -use_fast_math)
  set(CUDA_NVCC_FLAGS 
      # set this according to your cuda version
      -gencode=arch=compute_90,code=compute_90;
  ) 

  SET(CUDA_PROPAGATE_HOST_FLAGS OFF)

  find_package(OpenCV REQUIRED)
  find_package(Eigen3 REQUIRED)
  find_package(Boost REQUIRED COMPONENTS system filesystem)

  find_package(catkin REQUIRED COMPONENTS
      roscpp roslib cmake_modules cv_bridge image_transport pcl_ros sensor_msgs geometry_msgs nav_msgs quadrotor_msgs dynamic_reconfigure rostest)
  
  if(CATKIN_ENABLE_TESTING)
    find_package(GTest REQUIRED)
  endif()
  generate_dynamic_reconfigure_options(
    cfg/local_sensing_node.cfg
  )
  catkin_package(
      DEPENDS OpenCV Eigen Boost
      CATKIN_DEPENDS roscpp roslib image_transport pcl_ros
  #INCLUDE_DIRS include
      LIBRARIES depth_render_cuda
  )

  include_directories(
    SYSTEM 
    #include 
    ${catkin_INCLUDE_DIRS}
    ${OpenCV_INCLUDE_DIRS}
    ${Eigen_INCLUDE_DIRS}
    ${Boost_INCLUDE_DIRS}
  )

  CUDA_ADD_LIBRARY( depth_render_cuda
      src/depth_render.cu
  )

  add_executable(
    pcl_render_node
    src/pcl_render_node.cpp
  )
  target_link_libraries( pcl_render_node
    depth_render_cuda
    ${OpenCV_LIBS}
    ${Boost_LIBRARIES}
    ${catkin_LIBRARIES}
  )
else(ENABLE_CUDA)
  find_package(Eigen3 REQUIRED)
  find_package(catkin REQUIRED COMPONENTS
      roscpp roslib cmake_modules pcl_ros sensor_msgs geometry_msgs nav_msgs quadrotor_msgs)
  
  catkin_package(
      DEPENDS Eigen
      CATKIN_DEPENDS roscpp roslib pcl_ros
  )

  include_directories(
    SYSTEM 
    ${catkin_INCLUDE_DIRS}
    ${Eigen_INCLUDE_DIRS}.
  )

  add_executable(
    pcl_render_node
    src/pointcloud_render_node.cpp
  )

  target_link_libraries( pcl_render_node
    ${catkin_LIBRARIES}
    ${PCL_LIBRARIES}
  )
endif(ENABLE_CUDA)

# Testing
find_package(rostest REQUIRED)
find_package(GTest REQUIRED)

# CUDA depth rendering tests (only if CUDA is enabled)
if(ENABLE_CUDA)
  # Create a simple gtest executable for depth rendering
  add_executable(test_depth_render test/test_depth_render.cpp)
  target_link_libraries(test_depth_render 
    depth_render_cuda 
    ${catkin_LIBRARIES} 
    ${GTEST_LIBRARIES}
    ${CUDA_LIBRARIES}
    pthread
  )
  # Add CUDA include directories for the test
  target_include_directories(test_depth_render PRIVATE ${CUDA_INCLUDE_DIRS})
  
  # Add the test to CTest
  add_test(NAME depth_render_unit_tests COMMAND test_depth_render)
endif()
  

