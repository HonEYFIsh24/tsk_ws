# Copyright (c) Deptrum Inc. All Rights Reserved.

cmake_minimum_required(VERSION 3.5)
project(deptrum_examples_utils)

add_library(${PROJECT_NAME} STATIC
  ${CMAKE_CURRENT_LIST_DIR}/utils_c.c
  ${CMAKE_CURRENT_LIST_DIR}/utils_c.h
  ${CMAKE_CURRENT_LIST_DIR}/utils.cpp
  ${CMAKE_CURRENT_LIST_DIR}/utils.hpp
)

# find_package(Threads REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC ${dt_threads} dt::deptrum)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(${PROJECT_NAME} PUBLIC ${DT_PUBLIC_HEADERS_DIR})

add_library(dt::examples::utils ALIAS ${PROJECT_NAME})
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "examples")