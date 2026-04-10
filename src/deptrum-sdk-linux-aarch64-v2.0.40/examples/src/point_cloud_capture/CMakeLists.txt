# Copyright (c) Deptrum Inc. All Rights Reserved.

cmake_minimum_required(VERSION 3.5)
project(deptrum_point_cloud_capture)

add_executable(${PROJECT_NAME} dt_point_cloud_capture.cpp)

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 11)
target_link_libraries(${PROJECT_NAME} dt::deptrum dt::examples::utils glfw ${OPENGL_LIBRARIES})
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "examples")

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)