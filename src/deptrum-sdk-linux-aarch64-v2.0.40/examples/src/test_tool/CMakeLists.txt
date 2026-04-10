# deptrum sdk
file(GLOB TEST_SOURCES ./*.cpp)

project(deptrum_test_tool)

add_executable(${PROJECT_NAME} ${TEST_SOURCES})
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 11)
target_link_libraries(${PROJECT_NAME} dt::deptrum dt::examples::utils)
if(BUILD_GRAPHICAL_EXAMPLES)
  target_link_libraries(${PROJECT_NAME} glfw ${OPENGL_LIBRARIES})
endif()

set_target_properties (${PROJECT_NAME} PROPERTIES FOLDER "examples")

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)