# generated from genmsg/cmake/pkg-genmsg.cmake.em

message(STATUS "hipnuc_imu: 4 messages, 0 services")

set(MSG_I_FLAGS "-Ihipnuc_imu:/home/dut/lidar6/src/hipnuc_imu/msg;-Istd_msgs:/opt/ros/noetic/share/std_msgs/cmake/../msg;-Igeometry_msgs:/opt/ros/noetic/share/geometry_msgs/cmake/../msg")

# Find all generators
find_package(gencpp REQUIRED)
find_package(geneus REQUIRED)
find_package(genlisp REQUIRED)
find_package(gennodejs REQUIRED)
find_package(genpy REQUIRED)

add_custom_target(hipnuc_imu_generate_messages ALL)

# verify that message/service dependencies have not changed since configure



get_filename_component(_filename "/home/dut/lidar6/src/hipnuc_imu/msg/Ins_0x81_package.msg" NAME_WE)
add_custom_target(_hipnuc_imu_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "hipnuc_imu" "/home/dut/lidar6/src/hipnuc_imu/msg/Ins_0x81_package.msg" ""
)

get_filename_component(_filename "/home/dut/lidar6/src/hipnuc_imu/msg/Imu_0x92_package.msg" NAME_WE)
add_custom_target(_hipnuc_imu_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "hipnuc_imu" "/home/dut/lidar6/src/hipnuc_imu/msg/Imu_0x92_package.msg" ""
)

get_filename_component(_filename "/home/dut/lidar6/src/hipnuc_imu/msg/Imu_0x91_package.msg" NAME_WE)
add_custom_target(_hipnuc_imu_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "hipnuc_imu" "/home/dut/lidar6/src/hipnuc_imu/msg/Imu_0x91_package.msg" ""
)

get_filename_component(_filename "/home/dut/lidar6/src/hipnuc_imu/msg/hipnuc_imu_msg.msg" NAME_WE)
add_custom_target(_hipnuc_imu_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "hipnuc_imu" "/home/dut/lidar6/src/hipnuc_imu/msg/hipnuc_imu_msg.msg" "hipnuc_imu/Ins_0x81_package:std_msgs/Header:hipnuc_imu/Imu_0x91_package:hipnuc_imu/Imu_0x92_package"
)

#
#  langs = gencpp;geneus;genlisp;gennodejs;genpy
#

### Section generating for lang: gencpp
### Generating Messages
_generate_msg_cpp(hipnuc_imu
  "/home/dut/lidar6/src/hipnuc_imu/msg/Ins_0x81_package.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/hipnuc_imu
)
_generate_msg_cpp(hipnuc_imu
  "/home/dut/lidar6/src/hipnuc_imu/msg/Imu_0x92_package.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/hipnuc_imu
)
_generate_msg_cpp(hipnuc_imu
  "/home/dut/lidar6/src/hipnuc_imu/msg/Imu_0x91_package.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/hipnuc_imu
)
_generate_msg_cpp(hipnuc_imu
  "/home/dut/lidar6/src/hipnuc_imu/msg/hipnuc_imu_msg.msg"
  "${MSG_I_FLAGS}"
  "/home/dut/lidar6/src/hipnuc_imu/msg/Ins_0x81_package.msg;/opt/ros/noetic/share/std_msgs/cmake/../msg/Header.msg;/home/dut/lidar6/src/hipnuc_imu/msg/Imu_0x91_package.msg;/home/dut/lidar6/src/hipnuc_imu/msg/Imu_0x92_package.msg"
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/hipnuc_imu
)

### Generating Services

### Generating Module File
_generate_module_cpp(hipnuc_imu
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/hipnuc_imu
  "${ALL_GEN_OUTPUT_FILES_cpp}"
)

add_custom_target(hipnuc_imu_generate_messages_cpp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_cpp}
)
add_dependencies(hipnuc_imu_generate_messages hipnuc_imu_generate_messages_cpp)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/dut/lidar6/src/hipnuc_imu/msg/Ins_0x81_package.msg" NAME_WE)
add_dependencies(hipnuc_imu_generate_messages_cpp _hipnuc_imu_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/dut/lidar6/src/hipnuc_imu/msg/Imu_0x92_package.msg" NAME_WE)
add_dependencies(hipnuc_imu_generate_messages_cpp _hipnuc_imu_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/dut/lidar6/src/hipnuc_imu/msg/Imu_0x91_package.msg" NAME_WE)
add_dependencies(hipnuc_imu_generate_messages_cpp _hipnuc_imu_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/dut/lidar6/src/hipnuc_imu/msg/hipnuc_imu_msg.msg" NAME_WE)
add_dependencies(hipnuc_imu_generate_messages_cpp _hipnuc_imu_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(hipnuc_imu_gencpp)
add_dependencies(hipnuc_imu_gencpp hipnuc_imu_generate_messages_cpp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS hipnuc_imu_generate_messages_cpp)

### Section generating for lang: geneus
### Generating Messages
_generate_msg_eus(hipnuc_imu
  "/home/dut/lidar6/src/hipnuc_imu/msg/Ins_0x81_package.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/hipnuc_imu
)
_generate_msg_eus(hipnuc_imu
  "/home/dut/lidar6/src/hipnuc_imu/msg/Imu_0x92_package.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/hipnuc_imu
)
_generate_msg_eus(hipnuc_imu
  "/home/dut/lidar6/src/hipnuc_imu/msg/Imu_0x91_package.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/hipnuc_imu
)
_generate_msg_eus(hipnuc_imu
  "/home/dut/lidar6/src/hipnuc_imu/msg/hipnuc_imu_msg.msg"
  "${MSG_I_FLAGS}"
  "/home/dut/lidar6/src/hipnuc_imu/msg/Ins_0x81_package.msg;/opt/ros/noetic/share/std_msgs/cmake/../msg/Header.msg;/home/dut/lidar6/src/hipnuc_imu/msg/Imu_0x91_package.msg;/home/dut/lidar6/src/hipnuc_imu/msg/Imu_0x92_package.msg"
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/hipnuc_imu
)

### Generating Services

### Generating Module File
_generate_module_eus(hipnuc_imu
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/hipnuc_imu
  "${ALL_GEN_OUTPUT_FILES_eus}"
)

add_custom_target(hipnuc_imu_generate_messages_eus
  DEPENDS ${ALL_GEN_OUTPUT_FILES_eus}
)
add_dependencies(hipnuc_imu_generate_messages hipnuc_imu_generate_messages_eus)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/dut/lidar6/src/hipnuc_imu/msg/Ins_0x81_package.msg" NAME_WE)
add_dependencies(hipnuc_imu_generate_messages_eus _hipnuc_imu_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/dut/lidar6/src/hipnuc_imu/msg/Imu_0x92_package.msg" NAME_WE)
add_dependencies(hipnuc_imu_generate_messages_eus _hipnuc_imu_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/dut/lidar6/src/hipnuc_imu/msg/Imu_0x91_package.msg" NAME_WE)
add_dependencies(hipnuc_imu_generate_messages_eus _hipnuc_imu_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/dut/lidar6/src/hipnuc_imu/msg/hipnuc_imu_msg.msg" NAME_WE)
add_dependencies(hipnuc_imu_generate_messages_eus _hipnuc_imu_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(hipnuc_imu_geneus)
add_dependencies(hipnuc_imu_geneus hipnuc_imu_generate_messages_eus)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS hipnuc_imu_generate_messages_eus)

### Section generating for lang: genlisp
### Generating Messages
_generate_msg_lisp(hipnuc_imu
  "/home/dut/lidar6/src/hipnuc_imu/msg/Ins_0x81_package.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/hipnuc_imu
)
_generate_msg_lisp(hipnuc_imu
  "/home/dut/lidar6/src/hipnuc_imu/msg/Imu_0x92_package.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/hipnuc_imu
)
_generate_msg_lisp(hipnuc_imu
  "/home/dut/lidar6/src/hipnuc_imu/msg/Imu_0x91_package.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/hipnuc_imu
)
_generate_msg_lisp(hipnuc_imu
  "/home/dut/lidar6/src/hipnuc_imu/msg/hipnuc_imu_msg.msg"
  "${MSG_I_FLAGS}"
  "/home/dut/lidar6/src/hipnuc_imu/msg/Ins_0x81_package.msg;/opt/ros/noetic/share/std_msgs/cmake/../msg/Header.msg;/home/dut/lidar6/src/hipnuc_imu/msg/Imu_0x91_package.msg;/home/dut/lidar6/src/hipnuc_imu/msg/Imu_0x92_package.msg"
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/hipnuc_imu
)

### Generating Services

### Generating Module File
_generate_module_lisp(hipnuc_imu
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/hipnuc_imu
  "${ALL_GEN_OUTPUT_FILES_lisp}"
)

add_custom_target(hipnuc_imu_generate_messages_lisp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_lisp}
)
add_dependencies(hipnuc_imu_generate_messages hipnuc_imu_generate_messages_lisp)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/dut/lidar6/src/hipnuc_imu/msg/Ins_0x81_package.msg" NAME_WE)
add_dependencies(hipnuc_imu_generate_messages_lisp _hipnuc_imu_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/dut/lidar6/src/hipnuc_imu/msg/Imu_0x92_package.msg" NAME_WE)
add_dependencies(hipnuc_imu_generate_messages_lisp _hipnuc_imu_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/dut/lidar6/src/hipnuc_imu/msg/Imu_0x91_package.msg" NAME_WE)
add_dependencies(hipnuc_imu_generate_messages_lisp _hipnuc_imu_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/dut/lidar6/src/hipnuc_imu/msg/hipnuc_imu_msg.msg" NAME_WE)
add_dependencies(hipnuc_imu_generate_messages_lisp _hipnuc_imu_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(hipnuc_imu_genlisp)
add_dependencies(hipnuc_imu_genlisp hipnuc_imu_generate_messages_lisp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS hipnuc_imu_generate_messages_lisp)

### Section generating for lang: gennodejs
### Generating Messages
_generate_msg_nodejs(hipnuc_imu
  "/home/dut/lidar6/src/hipnuc_imu/msg/Ins_0x81_package.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/hipnuc_imu
)
_generate_msg_nodejs(hipnuc_imu
  "/home/dut/lidar6/src/hipnuc_imu/msg/Imu_0x92_package.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/hipnuc_imu
)
_generate_msg_nodejs(hipnuc_imu
  "/home/dut/lidar6/src/hipnuc_imu/msg/Imu_0x91_package.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/hipnuc_imu
)
_generate_msg_nodejs(hipnuc_imu
  "/home/dut/lidar6/src/hipnuc_imu/msg/hipnuc_imu_msg.msg"
  "${MSG_I_FLAGS}"
  "/home/dut/lidar6/src/hipnuc_imu/msg/Ins_0x81_package.msg;/opt/ros/noetic/share/std_msgs/cmake/../msg/Header.msg;/home/dut/lidar6/src/hipnuc_imu/msg/Imu_0x91_package.msg;/home/dut/lidar6/src/hipnuc_imu/msg/Imu_0x92_package.msg"
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/hipnuc_imu
)

### Generating Services

### Generating Module File
_generate_module_nodejs(hipnuc_imu
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/hipnuc_imu
  "${ALL_GEN_OUTPUT_FILES_nodejs}"
)

add_custom_target(hipnuc_imu_generate_messages_nodejs
  DEPENDS ${ALL_GEN_OUTPUT_FILES_nodejs}
)
add_dependencies(hipnuc_imu_generate_messages hipnuc_imu_generate_messages_nodejs)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/dut/lidar6/src/hipnuc_imu/msg/Ins_0x81_package.msg" NAME_WE)
add_dependencies(hipnuc_imu_generate_messages_nodejs _hipnuc_imu_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/dut/lidar6/src/hipnuc_imu/msg/Imu_0x92_package.msg" NAME_WE)
add_dependencies(hipnuc_imu_generate_messages_nodejs _hipnuc_imu_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/dut/lidar6/src/hipnuc_imu/msg/Imu_0x91_package.msg" NAME_WE)
add_dependencies(hipnuc_imu_generate_messages_nodejs _hipnuc_imu_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/dut/lidar6/src/hipnuc_imu/msg/hipnuc_imu_msg.msg" NAME_WE)
add_dependencies(hipnuc_imu_generate_messages_nodejs _hipnuc_imu_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(hipnuc_imu_gennodejs)
add_dependencies(hipnuc_imu_gennodejs hipnuc_imu_generate_messages_nodejs)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS hipnuc_imu_generate_messages_nodejs)

### Section generating for lang: genpy
### Generating Messages
_generate_msg_py(hipnuc_imu
  "/home/dut/lidar6/src/hipnuc_imu/msg/Ins_0x81_package.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/hipnuc_imu
)
_generate_msg_py(hipnuc_imu
  "/home/dut/lidar6/src/hipnuc_imu/msg/Imu_0x92_package.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/hipnuc_imu
)
_generate_msg_py(hipnuc_imu
  "/home/dut/lidar6/src/hipnuc_imu/msg/Imu_0x91_package.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/hipnuc_imu
)
_generate_msg_py(hipnuc_imu
  "/home/dut/lidar6/src/hipnuc_imu/msg/hipnuc_imu_msg.msg"
  "${MSG_I_FLAGS}"
  "/home/dut/lidar6/src/hipnuc_imu/msg/Ins_0x81_package.msg;/opt/ros/noetic/share/std_msgs/cmake/../msg/Header.msg;/home/dut/lidar6/src/hipnuc_imu/msg/Imu_0x91_package.msg;/home/dut/lidar6/src/hipnuc_imu/msg/Imu_0x92_package.msg"
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/hipnuc_imu
)

### Generating Services

### Generating Module File
_generate_module_py(hipnuc_imu
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/hipnuc_imu
  "${ALL_GEN_OUTPUT_FILES_py}"
)

add_custom_target(hipnuc_imu_generate_messages_py
  DEPENDS ${ALL_GEN_OUTPUT_FILES_py}
)
add_dependencies(hipnuc_imu_generate_messages hipnuc_imu_generate_messages_py)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/dut/lidar6/src/hipnuc_imu/msg/Ins_0x81_package.msg" NAME_WE)
add_dependencies(hipnuc_imu_generate_messages_py _hipnuc_imu_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/dut/lidar6/src/hipnuc_imu/msg/Imu_0x92_package.msg" NAME_WE)
add_dependencies(hipnuc_imu_generate_messages_py _hipnuc_imu_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/dut/lidar6/src/hipnuc_imu/msg/Imu_0x91_package.msg" NAME_WE)
add_dependencies(hipnuc_imu_generate_messages_py _hipnuc_imu_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/dut/lidar6/src/hipnuc_imu/msg/hipnuc_imu_msg.msg" NAME_WE)
add_dependencies(hipnuc_imu_generate_messages_py _hipnuc_imu_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(hipnuc_imu_genpy)
add_dependencies(hipnuc_imu_genpy hipnuc_imu_generate_messages_py)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS hipnuc_imu_generate_messages_py)



if(gencpp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/hipnuc_imu)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/hipnuc_imu
    DESTINATION ${gencpp_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_cpp)
  add_dependencies(hipnuc_imu_generate_messages_cpp std_msgs_generate_messages_cpp)
endif()
if(TARGET geometry_msgs_generate_messages_cpp)
  add_dependencies(hipnuc_imu_generate_messages_cpp geometry_msgs_generate_messages_cpp)
endif()

if(geneus_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/hipnuc_imu)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/hipnuc_imu
    DESTINATION ${geneus_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_eus)
  add_dependencies(hipnuc_imu_generate_messages_eus std_msgs_generate_messages_eus)
endif()
if(TARGET geometry_msgs_generate_messages_eus)
  add_dependencies(hipnuc_imu_generate_messages_eus geometry_msgs_generate_messages_eus)
endif()

if(genlisp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/hipnuc_imu)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/hipnuc_imu
    DESTINATION ${genlisp_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_lisp)
  add_dependencies(hipnuc_imu_generate_messages_lisp std_msgs_generate_messages_lisp)
endif()
if(TARGET geometry_msgs_generate_messages_lisp)
  add_dependencies(hipnuc_imu_generate_messages_lisp geometry_msgs_generate_messages_lisp)
endif()

if(gennodejs_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/hipnuc_imu)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/hipnuc_imu
    DESTINATION ${gennodejs_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_nodejs)
  add_dependencies(hipnuc_imu_generate_messages_nodejs std_msgs_generate_messages_nodejs)
endif()
if(TARGET geometry_msgs_generate_messages_nodejs)
  add_dependencies(hipnuc_imu_generate_messages_nodejs geometry_msgs_generate_messages_nodejs)
endif()

if(genpy_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/hipnuc_imu)
  install(CODE "execute_process(COMMAND \"/usr/bin/python3\" -m compileall \"${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/hipnuc_imu\")")
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/hipnuc_imu
    DESTINATION ${genpy_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_py)
  add_dependencies(hipnuc_imu_generate_messages_py std_msgs_generate_messages_py)
endif()
if(TARGET geometry_msgs_generate_messages_py)
  add_dependencies(hipnuc_imu_generate_messages_py geometry_msgs_generate_messages_py)
endif()
