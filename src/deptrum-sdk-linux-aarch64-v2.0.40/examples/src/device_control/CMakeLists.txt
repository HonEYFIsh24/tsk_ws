# Copyright (c) Deptrum Inc. All Rights Reserved.

cmake_minimum_required(VERSION 3.5)
project(deptrum_device_control)

add_executable(${PROJECT_NAME} dt_device_control.cpp)

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 11)
target_link_libraries(${PROJECT_NAME} dt::deptrum dt::examples::utils)
set_target_properties (${PROJECT_NAME} PROPERTIES FOLDER "examples")

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)