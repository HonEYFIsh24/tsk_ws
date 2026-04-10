# Copyright (c) Deptrum Inc. All Rights Reserved.

cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_STANDARD 11)
project(deptrum_capture_c)
add_executable(${PROJECT_NAME} dt_capture.c)

target_link_libraries(${PROJECT_NAME} PRIVATE dt::deptrum dt::examples::utils)
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "examples_c")

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)