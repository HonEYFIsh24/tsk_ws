# Copyright (c) Deptrum Inc. All Rights Reserved.

cmake_minimum_required(VERSION 3.5)

project(deptrum_enumerate_c)
add_executable(${PROJECT_NAME} dt_enumerate.c)

target_link_libraries(${PROJECT_NAME} PRIVATE dt::deptrum dt::examples::utils)
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "examples_c")

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)