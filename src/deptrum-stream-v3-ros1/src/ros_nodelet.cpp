// Copyright (c) Deptrum Inc. All Rights Reserved.
#include "ros/ros.h"
#include "deptrum_camera/dt_camera_node_driver.h"
#include "nodelet/nodelet.h"
#include <pluginlib/class_list_macros.h>

namespace deptrum_camera {
class DTCameraNodelet : public nodelet::Nodelet {
 public:
  DTCameraNodelet() {}

  ~DTCameraNodelet() {}

 private:
  void onInit() override {
    ros::NodeHandle nh = getNodeHandle();
    ros::NodeHandle nh_private = getPrivateNodeHandle();
    dt_camera_node_driver_.reset(new DtCameraNodeDriver(nh, nh_private));
  }

  boost::shared_ptr<DtCameraNodeDriver> dt_camera_node_driver_;
};
}  // namespace deptrum_camera

PLUGINLIB_EXPORT_CLASS(deptrum_camera::DTCameraNodelet, nodelet::Nodelet)