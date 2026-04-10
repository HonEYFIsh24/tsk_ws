// Copyright (c) Deptrum Inc. All Rights Reserved.
#include "ros/ros.h"
#include "deptrum_camera/dt_camera_node_driver.h"

int main(int argc, char** argv) {
  ros::init(argc, argv, "deptrum_camera");
  ros::NodeHandle nh;
  ros::NodeHandle nh_private("~");
  deptrum_camera::DtCameraNodeDriver dt_camera_node_factory(nh, nh_private);
  ros::spin();
  ros::shutdown();
  return 0;
}
