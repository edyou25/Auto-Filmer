# 代码笔记

## 1. 目录结构

src
|-- auto-filmer
|   |-- mapping
|   |   |-- config 相机内外参，rviz参数
|   |   |-- include 点云缓存数据结构
|   |   |-- launch 启动nodelet manager，mapping和rviz
|   |   `-- src 
|   |       |-- mapping.cc mapping算法，读取depth/global_map
|   |       |-- mapping_nodelet.cpp mapping节点管理
|   |       |-- mapping_vis_node.cpp 独立节点，occ转pc
|   |       `-- visualize_history_path.cpp 
|   `-- planning
|       |-- planning
|       |   |-- include
|       |   |   |-- env
|       |   |   |   `-- env.hpp
|       |   |   |-- path
|       |   |   |   `-- path.hpp
|       |   |   |-- prediction
|       |   |   |   `-- prediction.hpp
|       |   |   |-- shot
|       |   |   |   `-- shot.hpp
|       |   |   |-- visualization
|       |   |   |   `-- visualization.hpp
|       |   |   `-- wr_msg
|       |   |       `-- wr_msg.hpp
|       |   |-- launch
|       |   |   |-- af_tracking.launch 
|       |   |   `-- run_in_sim.launch
|       |   `-- src
|       |       |-- planning_nodelet.cpp
|       |       `-- traj_server.cpp
|       `-- traj_opt
|           |-- include
|           |   `-- traj_opt
|           |       |-- flatness.hpp
|           |       |-- geoutils.hpp
|           |       |-- lbfgs.hpp
|           |       |-- minco.hpp
|           |       |-- quickhull.hpp
|           |       |-- root_finder.hpp
|           |       |-- sdlp.hpp
|           |       |-- traj_opt.h
|           |       `-- trajectory.hpp
|           `-- src
|               `-- traj_opt.cc
|-- gui 
|-- target
|   |-- path_searching
|   |-- plan_env 生成occ
|   |-- plan_manage
|   |   |-- include 状态机
|   |   |-- launch 
|   |   |-- package.xml
|   |   `-- src
|   |       |-- ego_planner_node.cpp target ego-planner v2 节点
|   |       |-- ego_replan_fsm.cpp ego-planner v2 状态机器
|   |       |-- planner_manager.cpp
|   |       `-- traj_server.cpp
|   |-- traj_opt_target
|   |   |-- CMakeLists.txt
|   |   |-- include
|   |   |   `-- optimizer
|   |   |       |-- gcopter.hpp
|   |   |       |-- lbfgs.hpp
|   |   |       |-- poly_traj_optimizer.h
|   |   |       |-- poly_traj_utils.hpp
|   |   |       `-- root_finder.hpp
|   |   `-- src
|   |       `-- poly_traj_optimizer.cpp
|   `-- traj_utils
|       |-- CMakeLists.txt
|       |-- include
|       |   `-- traj_utils
|       |       |-- plan_container.hpp
|       |       `-- planning_visualization.h
|       |-- msg
|       |   |-- DataDisp.msg
|       |   |-- MINCOTraj.msg
|       |   `-- PolyTraj.msg
|       |-- package.xml
|       `-- src
|           `-- planning_visualization.cpp
`-- utils
    |-- DecompROS 安全走廊
    |-- cmake_utils cmake脚本
    |-- quadrotor_msgs
    |   |-- msg 云台控制，地图，轨迹，姿态控制，拍摄参数
    |-- uav_simulator
    |   |-- fake_drone
    |   |   |-- CMakeLists.txt
    |   |   |-- package.xml
    |   |   `-- src
    |   |       `-- fake_gimbal.cpp
    |   |-- local_sensing
    |   |   |-- CMakeLists.txt
    |   |   |-- CMakeModules
    |   |   |   |-- FindCUDA
    |   |   |   |   |-- make2cmake.cmake
    |   |   |   |   |-- parse_cubin.cmake
    |   |   |   |   `-- run_nvcc.cmake
    |   |   |   |-- FindCUDA.cmake
    |   |   |   |-- FindEigen.cmake
    |   |   |   `-- FindEigen.cmake~
    |   |   |-- cfg
    |   |   |   `-- local_sensing_node.cfg
    |   |   |-- package.xml
    |   |   |-- params
    |   |   |   `-- camera.yaml
    |   |   `-- src
    |   |       |-- AlignError.h
    |   |       |-- ceres_extensions.h
    |   |       |-- csv_convert.py
    |   |       |-- cuda_exception.cuh
    |   |       |-- depth_render.cu
    |   |       |-- depth_render.cuh
    |   |       |-- device_image.cuh
    |   |       |-- empty.cpp
    |   |       |-- empty.h
    |   |       |-- euroc.cpp
    |   |       |-- helper_math.h
    |   |       |-- pcl_render_node.cpp
    |   |       |-- pointcloud_render_node.cpp
    |   |       `-- shot.hpp
    |   |-- map_generator
    |   |   |-- CMakeLists.txt
    |   |   |-- package.xml
    |   |   `-- src
    |   |       |-- map_generator_easy.py
    |   |       `-- random_forest_sensing.cpp
    |   |-- odom_vis
    |   |   |-- odom_visualization
    |   |   |   |-- CMakeLists.txt
    |   |   |   |-- Makefile
    |   |   |   |-- bin
    |   |   |   |   |-- odom_visualization
    |   |   |   |   `-- odom_visualization_vicon45
    |   |   |   |-- launch
    |   |   |   |   |-- real_vis.launch
    |   |   |   |   |-- sim_vis.launch
    |   |   |   |   `-- vins_vis.launch
    |   |   |   |-- mainpage.dox
    |   |   |   |-- meshes
    |   |   |   |   |-- car.dae
    |   |   |   |   |-- f250.dae
    |   |   |   |   `-- hummingbird.mesh
    |   |   |   |-- package.xml
    |   |   |   `-- src
    |   |   |       |-- odom_visualization.cpp
    |   |   |       `-- odom_visualization_car.cpp
    |   |   `-- pose_utils
    |   |       |-- CMakeLists.txt
    |   |       |-- Makefile
    |   |       |-- include
    |   |       |   `-- pose_utils.h
    |   |       |-- package.xml
    |   |       `-- src
    |   |           `-- pose_utils.cpp
    |   |-- so3_control
    |   |   |-- CMakeLists.txt
    |   |   |-- config
    |   |   |   |-- corrections_hummingbird.yaml
    |   |   |   |-- corrections_pelican.yaml
    |   |   |   |-- gains.yaml
    |   |   |   |-- gains_hummingbird.yaml
    |   |   |   `-- gains_pelican.yaml
    |   |   |-- include
    |   |   |   `-- so3_control
    |   |   |       `-- SO3Control.h
    |   |   |-- mainpage.dox
    |   |   |-- nodelet_plugin.xml
    |   |   |-- package.xml
    |   |   `-- src
    |   |       |-- SO3Control.cpp
    |   |       |-- control_example.cpp
    |   |       `-- so3_control_nodelet.cpp
    |   |-- so3_quadrotor_simulator
    |   |   |-- CMakeLists.txt
    |   |   |-- config
    |   |   |   `-- rviz.rviz
    |   |   |-- include
    |   |   |   |-- ode
    |   |   |   |   |-- CHANGELOG
    |   |   |   |   |-- Jamroot
    |   |   |   |   |-- README
    |   |   |   |   |-- boost
    |   |   |   |   |   `-- numeric
    |   |   |   |   |       |-- odeint
    |   |   |   |   |       |   |-- algebra
    |   |   |   |   |       |   |   |-- array_algebra.hpp
    |   |   |   |   |       |   |   |-- default_operations.hpp
    |   |   |   |   |       |   |   |-- detail
    |   |   |   |   |       |   |   |   |-- for_each.hpp
    |   |   |   |   |       |   |   |   |-- macros.hpp
    |   |   |   |   |       |   |   |   `-- reduce.hpp
    |   |   |   |   |       |   |   |-- fusion_algebra.hpp
    |   |   |   |   |       |   |   |-- range_algebra.hpp
    |   |   |   |   |       |   |   `-- vector_space_algebra.hpp
    |   |   |   |   |       |   |-- config.hpp
    |   |   |   |   |       |   |-- external
    |   |   |   |   |       |   |   |-- gsl
    |   |   |   |   |       |   |   |   `-- gsl_wrapper.hpp
    |   |   |   |   |       |   |   |-- mkl
    |   |   |   |   |       |   |   |   `-- mkl_operations.hpp
    |   |   |   |   |       |   |   |-- mtl4
    |   |   |   |   |       |   |   |   |-- implicit_euler_mtl4.hpp
    |   |   |   |   |       |   |   |   `-- mtl4_resize.hpp
    |   |   |   |   |       |   |   |-- thrust
    |   |   |   |   |       |   |   |   |-- thrust_algebra.hpp
    |   |   |   |   |       |   |   |   |-- thrust_operations.hpp
    |   |   |   |   |       |   |   |   `-- thrust_resize.hpp
    |   |   |   |   |       |   |   |-- vexcl
    |   |   |   |   |       |   |   |   `-- vexcl_resize.hpp
    |   |   |   |   |       |   |   `-- viennacl
    |   |   |   |   |       |   |       |-- viennacl_operations.hpp
    |   |   |   |   |       |   |       `-- viennacl_resize.hpp
    |   |   |   |   |       |   |-- integrate
    |   |   |   |   |       |   |   |-- detail
    |   |   |   |   |       |   |   |   |-- integrate_adaptive.hpp
    |   |   |   |   |       |   |   |   |-- integrate_const.hpp
    |   |   |   |   |       |   |   |   |-- integrate_n_steps.hpp
    |   |   |   |   |       |   |   |   `-- integrate_times.hpp
    |   |   |   |   |       |   |   |-- integrate.hpp
    |   |   |   |   |       |   |   |-- integrate_adaptive.hpp
    |   |   |   |   |       |   |   |-- integrate_const.hpp
    |   |   |   |   |       |   |   |-- integrate_n_steps.hpp
    |   |   |   |   |       |   |   |-- integrate_times.hpp
    |   |   |   |   |       |   |   |-- null_observer.hpp
    |   |   |   |   |       |   |   `-- observer_collection.hpp
    |   |   |   |   |       |   |-- stepper
    |   |   |   |   |       |   |   |-- adams_bashforth.hpp
    |   |   |   |   |       |   |   |-- adams_bashforth_moulton.hpp
    |   |   |   |   |       |   |   |-- adams_moulton.hpp
    |   |   |   |   |       |   |   |-- base
    |   |   |   |   |       |   |   |   |-- algebra_stepper_base.hpp
    |   |   |   |   |       |   |   |   |-- explicit_error_stepper_base.hpp
    |   |   |   |   |       |   |   |   |-- explicit_error_stepper_fsal_base.hpp
    |   |   |   |   |       |   |   |   |-- explicit_stepper_base.hpp
    |   |   |   |   |       |   |   |   `-- symplectic_rkn_stepper_base.hpp
    |   |   |   |   |       |   |   |-- bulirsch_stoer.hpp
    |   |   |   |   |       |   |   |-- bulirsch_stoer_dense_out.hpp
    |   |   |   |   |       |   |   |-- controlled_runge_kutta.hpp
    |   |   |   |   |       |   |   |-- controlled_step_result.hpp
    |   |   |   |   |       |   |   |-- dense_output_runge_kutta.hpp
    |   |   |   |   |       |   |   |-- detail
    |   |   |   |   |       |   |   |   |-- adams_bashforth_call_algebra.hpp
    |   |   |   |   |       |   |   |   |-- adams_bashforth_coefficients.hpp
    |   |   |   |   |       |   |   |   |-- adams_moulton_call_algebra.hpp
    |   |   |   |   |       |   |   |   |-- adams_moulton_coefficients.hpp
    |   |   |   |   |       |   |   |   |-- generic_rk_algorithm.hpp
    |   |   |   |   |       |   |   |   |-- generic_rk_call_algebra.hpp
    |   |   |   |   |       |   |   |   |-- generic_rk_operations.hpp
    |   |   |   |   |       |   |   |   `-- rotating_buffer.hpp
    |   |   |   |   |       |   |   |-- euler.hpp
    |   |   |   |   |       |   |   |-- explicit_error_generic_rk.hpp
    |   |   |   |   |       |   |   |-- explicit_generic_rk.hpp
    |   |   |   |   |       |   |   |-- generation
    |   |   |   |   |       |   |   |   |-- generation_controlled_runge_kutta.hpp
    |   |   |   |   |       |   |   |   |-- generation_dense_output_runge_kutta.hpp
    |   |   |   |   |       |   |   |   |-- generation_rosenbrock4.hpp
    |   |   |   |   |       |   |   |   |-- generation_runge_kutta_cash_karp54.hpp
    |   |   |   |   |       |   |   |   |-- generation_runge_kutta_cash_karp54_classic.hpp
    |   |   |   |   |       |   |   |   |-- generation_runge_kutta_dopri5.hpp
    |   |   |   |   |       |   |   |   |-- generation_runge_kutta_fehlberg78.hpp
    |   |   |   |   |       |   |   |   |-- make_controlled.hpp
    |   |   |   |   |       |   |   |   `-- make_dense_output.hpp
    |   |   |   |   |       |   |   |-- generation.hpp
    |   |   |   |   |       |   |   |-- implicit_euler.hpp
    |   |   |   |   |       |   |   |-- modified_midpoint.hpp
    |   |   |   |   |       |   |   |-- rosenbrock4.hpp
    |   |   |   |   |       |   |   |-- rosenbrock4_controller.hpp
    |   |   |   |   |       |   |   |-- rosenbrock4_dense_output.hpp
    |   |   |   |   |       |   |   |-- runge_kutta4.hpp
    |   |   |   |   |       |   |   |-- runge_kutta4_classic.hpp
    |   |   |   |   |       |   |   |-- runge_kutta_cash_karp54.hpp
    |   |   |   |   |       |   |   |-- runge_kutta_cash_karp54_classic.hpp
    |   |   |   |   |       |   |   |-- runge_kutta_dopri5.hpp
    |   |   |   |   |       |   |   |-- runge_kutta_fehlberg78.hpp
    |   |   |   |   |       |   |   |-- stepper_categories.hpp
    |   |   |   |   |       |   |   |-- symplectic_euler.hpp
    |   |   |   |   |       |   |   |-- symplectic_rkn_sb3a_m4_mclachlan.hpp
    |   |   |   |   |       |   |   `-- symplectic_rkn_sb3a_mclachlan.hpp
    |   |   |   |   |       |   |-- util
    |   |   |   |   |       |   |   |-- bind.hpp
    |   |   |   |   |       |   |   |-- copy.hpp
    |   |   |   |   |       |   |   |-- detail
    |   |   |   |   |       |   |   |   |-- is_range.hpp
    |   |   |   |   |       |   |   |   `-- less_with_sign.hpp
    |   |   |   |   |       |   |   |-- is_pair.hpp
    |   |   |   |   |       |   |   |-- is_resizeable.hpp
    |   |   |   |   |       |   |   |-- resize.hpp
    |   |   |   |   |       |   |   |-- resizer.hpp
    |   |   |   |   |       |   |   |-- same_instance.hpp
    |   |   |   |   |       |   |   |-- same_size.hpp
    |   |   |   |   |       |   |   |-- state_wrapper.hpp
    |   |   |   |   |       |   |   |-- ublas_wrapper.hpp
    |   |   |   |   |       |   |   |-- unit_helper.hpp
    |   |   |   |   |       |   |   `-- unwrap_reference.hpp
    |   |   |   |   |       |   `-- version.hpp
    |   |   |   |   |       `-- odeint.hpp
    |   |   |   |   `-- libs
    |   |   |   |       `-- numeric
    |   |   |   |           `-- odeint
    |   |   |   |               |-- doc
    |   |   |   |               |   |-- Jamfile.v2
    |   |   |   |               |   |-- acknowledgements.qbk
    |   |   |   |               |   |-- concepts
    |   |   |   |               |   |   |-- controlled_stepper.qbk
    |   |   |   |               |   |   |-- dense_output_stepper.qbk
    |   |   |   |               |   |   |-- error_stepper.qbk
    |   |   |   |               |   |   |-- implicit_system.qbk
    |   |   |   |               |   |   |-- state_algebra_operations.qbk
    |   |   |   |               |   |   |-- state_wrapper.qbk
    |   |   |   |               |   |   |-- stepper.qbk
    |   |   |   |               |   |   |-- symplectic_system.qbk
    |   |   |   |               |   |   `-- system.qbk
    |   |   |   |               |   |-- concepts.qbk
    |   |   |   |               |   |-- controlled_stepper_table.qbk
    |   |   |   |               |   |-- details.qbk
    |   |   |   |               |   |-- details_bind_member_functions.qbk
    |   |   |   |               |   |-- details_boost_range.qbk
    |   |   |   |               |   |-- details_boost_ref.qbk
    |   |   |   |               |   |-- details_generation_functions.qbk
    |   |   |   |               |   |-- details_integrate_functions.qbk
    |   |   |   |               |   |-- details_state_types_algebras_operations.qbk
    |   |   |   |               |   |-- details_steppers.qbk
    |   |   |   |               |   |-- examples_table.qbk
    |   |   |   |               |   |-- getting_started.qbk
    |   |   |   |               |   |-- html
    |   |   |   |               |   |   |-- boostbook.css
    |   |   |   |               |   |   |-- images
    |   |   |   |               |   |   |   |-- alert.png
    |   |   |   |               |   |   |   |-- blank.png
    |   |   |   |               |   |   |   |-- callouts
    |   |   |   |               |   |   |   |   |-- 1.png
    |   |   |   |               |   |   |   |   |-- 1.svg
    |   |   |   |               |   |   |   |   |-- 10.png
    |   |   |   |               |   |   |   |   |-- 10.svg
    |   |   |   |               |   |   |   |   |-- 11.png
    |   |   |   |               |   |   |   |   |-- 11.svg
    |   |   |   |               |   |   |   |   |-- 12.png
    |   |   |   |               |   |   |   |   |-- 12.svg
    |   |   |   |               |   |   |   |   |-- 13.png
    |   |   |   |               |   |   |   |   |-- 13.svg
    |   |   |   |               |   |   |   |   |-- 14.png
    |   |   |   |               |   |   |   |   |-- 14.svg
    |   |   |   |               |   |   |   |   |-- 15.png
    |   |   |   |               |   |   |   |   |-- 15.svg
    |   |   |   |               |   |   |   |   |-- 16.svg
    |   |   |   |               |   |   |   |   |-- 17.svg
    |   |   |   |               |   |   |   |   |-- 18.svg
    |   |   |   |               |   |   |   |   |-- 19.svg
    |   |   |   |               |   |   |   |   |-- 2.png
    |   |   |   |               |   |   |   |   |-- 2.svg
    |   |   |   |               |   |   |   |   |-- 20.svg
    |   |   |   |               |   |   |   |   |-- 21.svg
    |   |   |   |               |   |   |   |   |-- 22.svg
    |   |   |   |               |   |   |   |   |-- 23.svg
    |   |   |   |               |   |   |   |   |-- 24.svg
    |   |   |   |               |   |   |   |   |-- 25.svg
    |   |   |   |               |   |   |   |   |-- 26.svg
    |   |   |   |               |   |   |   |   |-- 27.svg
    |   |   |   |               |   |   |   |   |-- 28.svg
    |   |   |   |               |   |   |   |   |-- 29.svg
    |   |   |   |               |   |   |   |   |-- 3.png
    |   |   |   |               |   |   |   |   |-- 3.svg
    |   |   |   |               |   |   |   |   |-- 30.svg
    |   |   |   |               |   |   |   |   |-- 4.png
    |   |   |   |               |   |   |   |   |-- 4.svg
    |   |   |   |               |   |   |   |   |-- 5.png
    |   |   |   |               |   |   |   |   |-- 5.svg
    |   |   |   |               |   |   |   |   |-- 6.png
    |   |   |   |               |   |   |   |   |-- 6.svg
    |   |   |   |               |   |   |   |   |-- 7.png
    |   |   |   |               |   |   |   |   |-- 7.svg
    |   |   |   |               |   |   |   |   |-- 8.png
    |   |   |   |               |   |   |   |   |-- 8.svg
    |   |   |   |               |   |   |   |   |-- 9.png
    |   |   |   |               |   |   |   |   `-- 9.svg
    |   |   |   |               |   |   |   |-- caution.png
    |   |   |   |               |   |   |   |-- caution.svg
    |   |   |   |               |   |   |   |-- draft.png
    |   |   |   |               |   |   |   |-- home.png
    |   |   |   |               |   |   |   |-- home.svg
    |   |   |   |               |   |   |   |-- important.png
    |   |   |   |               |   |   |   |-- important.svg
    |   |   |   |               |   |   |   |-- next.png
    |   |   |   |               |   |   |   |-- next.svg
    |   |   |   |               |   |   |   |-- next_disabled.png
    |   |   |   |               |   |   |   |-- note.png
    |   |   |   |               |   |   |   |-- note.svg
    |   |   |   |               |   |   |   |-- prev.png
    |   |   |   |               |   |   |   |-- prev.svg
    |   |   |   |               |   |   |   |-- prev_disabled.png
    |   |   |   |               |   |   |   |-- smiley.png
    |   |   |   |               |   |   |   |-- tip.png
    |   |   |   |               |   |   |   |-- tip.svg
    |   |   |   |               |   |   |   |-- toc-blank.png
    |   |   |   |               |   |   |   |-- toc-minus.png
    |   |   |   |               |   |   |   |-- toc-plus.png
    |   |   |   |               |   |   |   |-- up.png
    |   |   |   |               |   |   |   |-- up.svg
    |   |   |   |               |   |   |   |-- up_disabled.png
    |   |   |   |               |   |   |   |-- warning.png
    |   |   |   |               |   |   |   `-- warning.svg
    |   |   |   |               |   |   |-- logo.jpg
    |   |   |   |               |   |   |-- phase_lattice_2d_0000.jpg
    |   |   |   |               |   |   |-- phase_lattice_2d_0100.jpg
    |   |   |   |               |   |   |-- phase_lattice_2d_1000.jpg
    |   |   |   |               |   |   `-- solar_system.jpg
    |   |   |   |               |   |-- literature.qbk
    |   |   |   |               |   |-- make_controlled_table.qbk
    |   |   |   |               |   |-- make_dense_output_table.qbk
    |   |   |   |               |   |-- odeint.idx
    |   |   |   |               |   |-- odeint.qbk
    |   |   |   |               |   |-- range_table.qbk
    |   |   |   |               |   |-- stepper_table.qbk
    |   |   |   |               |   |-- tutorial.qbk
    |   |   |   |               |   |-- tutorial_chaotic_system.qbk
    |   |   |   |               |   |-- tutorial_harmonic_oscillator.qbk
    |   |   |   |               |   |-- tutorial_solar_system.qbk
    |   |   |   |               |   |-- tutorial_special_topics.qbk
    |   |   |   |               |   |-- tutorial_stiff_systems.qbk
    |   |   |   |               |   |-- tutorial_thrust_cuda.qbk
    |   |   |   |               |   `-- tutorial_vexcl_opencl.qbk
    |   |   |   |               |-- examples
    |   |   |   |               |   |-- 2d_lattice
    |   |   |   |               |   |   |-- Jamfile.v2
    |   |   |   |               |   |   |-- lattice2d.hpp
    |   |   |   |               |   |   |-- nested_range_algebra.hpp
    |   |   |   |               |   |   |-- spreading.cpp
    |   |   |   |               |   |   `-- vector_vector_resize.hpp
    |   |   |   |               |   |-- Jamfile.v2
    |   |   |   |               |   |-- bind_member_functions.cpp
    |   |   |   |               |   |-- bind_member_functions_cpp11.cpp
    |   |   |   |               |   |-- bulirsch_stoer.cpp
    |   |   |   |               |   |-- chaotic_system.cpp
    |   |   |   |               |   |-- elliptic.py
    |   |   |   |               |   |-- elliptic_functions.cpp
    |   |   |   |               |   |-- fpu.cpp
    |   |   |   |               |   |-- generation_functions.cpp
    |   |   |   |               |   |-- gmpxx
    |   |   |   |               |   |   `-- lorenz_gmpxx.cpp
    |   |   |   |               |   |-- gram_schmidt.hpp
    |   |   |   |               |   |-- harmonic_oscillator.cpp
    |   |   |   |               |   |-- harmonic_oscillator_units.cpp
    |   |   |   |               |   |-- heun.cpp
    |   |   |   |               |   |-- list_lattice.cpp
    |   |   |   |               |   |-- lorenz_point.cpp
    |   |   |   |               |   |-- mtl
    |   |   |   |               |   |   |-- Jamfile.v2
    |   |   |   |               |   |   |-- gauss_packet.cpp
    |   |   |   |               |   |   `-- implicit_euler_mtl.cpp
    |   |   |   |               |   |-- my_vector.cpp
    |   |   |   |               |   |-- phase_oscillator_ensemble.cpp
    |   |   |   |               |   |-- point_type.hpp
    |   |   |   |               |   |-- quadmath
    |   |   |   |               |   |   |-- Jamfile.v2
    |   |   |   |               |   |   `-- black_hole.cpp
    |   |   |   |               |   |-- resizing_lattice.cpp
    |   |   |   |               |   |-- simple1d.cpp
    |   |   |   |               |   |-- solar_system.agr
    |   |   |   |               |   |-- solar_system.cpp
    |   |   |   |               |   |-- stepper_details.cpp
    |   |   |   |               |   |-- stiff_system.cpp
    |   |   |   |               |   |-- stochastic_euler.cpp
    |   |   |   |               |   |-- stuart_landau.cpp
    |   |   |   |               |   |-- thrust
    |   |   |   |               |   |   |-- Makefile
    |   |   |   |               |   |   |-- lorenz_parameters.cu
    |   |   |   |               |   |   |-- phase_oscillator_chain.cu
    |   |   |   |               |   |   |-- phase_oscillator_ensemble.cu
    |   |   |   |               |   |   `-- relaxation.cu
    |   |   |   |               |   |-- two_dimensional_phase_lattice.cpp
    |   |   |   |               |   |-- ublas
    |   |   |   |               |   |   |-- Jamfile.v2
    |   |   |   |               |   |   `-- lorenz_ublas.cpp
    |   |   |   |               |   |-- van_der_pol_stiff.cpp
    |   |   |   |               |   `-- vexcl
    |   |   |   |               |       |-- Jamfile.v2
    |   |   |   |               |       `-- lorenz_ensemble.cpp
    |   |   |   |               |-- index.html
    |   |   |   |               |-- performance
    |   |   |   |               |   |-- Jamfile.v2
    |   |   |   |               |   |-- fusion_algebra.hpp
    |   |   |   |               |   |-- fusion_explicit_error_rk.hpp
    |   |   |   |               |   |-- fusion_explicit_rk_new.hpp
    |   |   |   |               |   |-- generic_odeint_rk4_lorenz.cpp
    |   |   |   |               |   |-- gsl_rk4_lorenz.cpp
    |   |   |   |               |   |-- lorenz.hpp
    |   |   |   |               |   |-- lorenz_gsl.hpp
    |   |   |   |               |   |-- nr_rk4_lorenz.cpp
    |   |   |   |               |   |-- nr_rk4_phase_lattice.cpp
    |   |   |   |               |   |-- odeint_rk4_lorenz_array.cpp
    |   |   |   |               |   |-- odeint_rk4_lorenz_range.cpp
    |   |   |   |               |   |-- odeint_rk4_phase_lattice.cpp
    |   |   |   |               |   |-- odeint_rk4_phase_lattice_mkl.cpp
    |   |   |   |               |   |-- performance.py
    |   |   |   |               |   |-- phase_lattice.hpp
    |   |   |   |               |   |-- phase_lattice_mkl.hpp
    |   |   |   |               |   |-- plot_result.py
    |   |   |   |               |   |-- rk4_lorenz.f
    |   |   |   |               |   |-- rk_performance_test_case.hpp
    |   |   |   |               |   |-- rt_algebra.hpp
    |   |   |   |               |   |-- rt_explicit_rk.hpp
    |   |   |   |               |   |-- rt_generic_rk4_lorenz.cpp
    |   |   |   |               |   `-- rt_generic_rk4_phase_lattice.cpp
    |   |   |   |               |-- test
    |   |   |   |               |   |-- Jamfile.v2
    |   |   |   |               |   |-- adams_bashforth.cpp
    |   |   |   |               |   |-- adams_bashforth_moulton.cpp
    |   |   |   |               |   |-- adams_moulton.cpp
    |   |   |   |               |   |-- boost_units_helpers.hpp
    |   |   |   |               |   |-- bulirsch_stoer.cpp
    |   |   |   |               |   |-- const_range.hpp
    |   |   |   |               |   |-- default_operations.cpp
    |   |   |   |               |   |-- diagnostic_state_type.hpp
    |   |   |   |               |   |-- dummy_odes.hpp
    |   |   |   |               |   |-- dummy_steppers.hpp
    |   |   |   |               |   |-- euler_stepper.cpp
    |   |   |   |               |   |-- fusion_algebra.cpp
    |   |   |   |               |   |-- generation.cpp
    |   |   |   |               |   |-- generic_error_stepper.cpp
    |   |   |   |               |   |-- generic_stepper.cpp
    |   |   |   |               |   |-- implicit_euler.cpp
    |   |   |   |               |   |-- integrate.cpp
    |   |   |   |               |   |-- integrate_implicit.cpp
    |   |   |   |               |   |-- integrate_times.cpp
    |   |   |   |               |   |-- is_pair.cpp
    |   |   |   |               |   |-- is_resizeable.cpp
    |   |   |   |               |   |-- numeric
    |   |   |   |               |   |   |-- Jamfile.v2
    |   |   |   |               |   |   |-- rosenbrock.cpp
    |   |   |   |               |   |   |-- runge_kutta.cpp
    |   |   |   |               |   |   `-- symplectic.cpp
    |   |   |   |               |   |-- prepare_stepper_testing.hpp
    |   |   |   |               |   |-- range_algebra.cpp
    |   |   |   |               |   |-- resize.cpp
    |   |   |   |               |   |-- resizing.cpp
    |   |   |   |               |   |-- rosenbrock4.cpp
    |   |   |   |               |   |-- runge_kutta_concepts.cpp
    |   |   |   |               |   |-- runge_kutta_controlled_concepts.cpp
    |   |   |   |               |   |-- runge_kutta_error_concepts.cpp
    |   |   |   |               |   |-- same_size.cpp
    |   |   |   |               |   |-- stepper_copying.cpp
    |   |   |   |               |   |-- stepper_with_ranges.cpp
    |   |   |   |               |   |-- stepper_with_units.cpp
    |   |   |   |               |   |-- symplectic_steppers.cpp
    |   |   |   |               |   |-- trivial_state.cpp
    |   |   |   |               |   `-- vector_space_1d.hpp
    |   |   |   |               `-- test_external
    |   |   |   |                   |-- gmp
    |   |   |   |                   |   |-- Jamfile.v2
    |   |   |   |                   |   |-- check_gmp.cpp
    |   |   |   |                   |   `-- gmp_integrate.cpp
    |   |   |   |                   |-- gsl
    |   |   |   |                   |   |-- Jamfile.v2
    |   |   |   |                   |   `-- check_gsl.cpp
    |   |   |   |                   |-- mkl
    |   |   |   |                   |   |-- Jamfile.v2
    |   |   |   |                   |   `-- check_mkl.cpp
    |   |   |   |                   |-- mtl4
    |   |   |   |                   |   |-- Jamfile.v2
    |   |   |   |                   |   `-- mtl4_resize.cpp
    |   |   |   |                   |-- thrust
    |   |   |   |                   |   |-- Makefile
    |   |   |   |                   |   `-- check_thrust.cu
    |   |   |   |                   `-- vexcl
    |   |   |   |                       |-- Jamfile.v2
    |   |   |   |                       `-- lorenz.cpp
    |   |   |   `-- quadrotor_simulator
    |   |   |       `-- Quadrotor.h
    |   |   |-- launch
    |   |   |   `-- simulator_example.launch
    |   |   |-- package.xml
    |   |   `-- src
    |   |       |-- dynamics
    |   |       |   `-- Quadrotor.cpp
    |   |       |-- quadrotor_simulator_so3.cpp
    |   |       `-- test_dynamics
    |   |           `-- test_dynamics.cpp
    |   `-- uav_simulator
    |       |-- CMakeLists.txt
    |       |-- config
    |       |   |-- mockamap.yaml
    |       |   |-- so3_controller.yaml
    |       |   `-- so3_quadrotor.yaml
    |       |-- launch
    |       |   `-- uav_simulator.launch
    |       `-- package.xml
    `-- uav_utils
        |-- CMakeLists.txt
        |-- README.md
        |-- include
        |   `-- uav_utils
        |       |-- converters.h
        |       |-- geometry_utils.h
        |       `-- utils.h
        |-- package.xml
        |-- scripts
        |   |-- odom_to_euler.py
        |   |-- send_odom.py
        |   |-- tf_assist.py
        |   `-- topic_statistics.py
        `-- src
            `-- uav_utils_test.cpp

