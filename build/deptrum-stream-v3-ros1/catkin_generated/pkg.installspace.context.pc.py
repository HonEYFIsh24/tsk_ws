# generated from catkin/cmake/template/pkg.context.pc.in
CATKIN_PACKAGE_PREFIX = ""
PROJECT_PKG_CONFIG_INCLUDE_DIRS = "${prefix}/include".split(';') if "${prefix}/include" != "" else []
PROJECT_CATKIN_DEPENDS = "camera_info_manager;cv_bridge;dynamic_reconfigure;image_geometry;image_transport;message_filters;message_runtime;roscpp;sensor_msgs;std_srvs;tf2;tf2_ros;backward_ros".replace(';', ' ')
PKG_CONFIG_LIBRARIES_WITH_PREFIX = "-ldeptrum_camera".split(';') if "-ldeptrum_camera" != "" else []
PROJECT_NAME = "deptrum_camera"
PROJECT_SPACE_DIR = "/home/dut/lidar6/install"
PROJECT_VERSION = "0.0.8"
