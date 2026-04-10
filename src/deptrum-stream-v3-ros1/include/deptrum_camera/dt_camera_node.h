// Copyright (c) Deptrum Inc. All Rights Reserved.

#pragma once

#include "types.h"
#include "utils.h"
#include "ros_sensor.h"
#include "ros/ros.h"
#include <opencv2/opencv.hpp>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/CameraInfo.h>
#include <sensor_msgs/PointCloud2.h>
#include <sensor_msgs/distortion_models.h>
#include <sensor_msgs/Imu.h>
#include <sensor_msgs/point_cloud2_iterator.h>
#include <tf2/LinearMath/Quaternion.h>
#include <tf2/LinearMath/Transform.h>
#include <tf2/LinearMath/Vector3.h>
#include <tf2_ros/static_transform_broadcaster.h>
#include <tf2_ros/transform_broadcaster.h>
#include <condition_variable>
#include <thread>
#include <atomic>
#include <camera_info_manager/camera_info_manager.h>
#include <std_srvs/SetBool.h>
#include <std_srvs/Empty.h>
#include "deptrum_camera/d2c_viewer.h"
#include "deptrum_camera/GetCameraParams.h"
#include <boost/optional.hpp>
#include <image_transport/image_transport.h>
#include <deptrum_camera/Metadata.h>
#include <deptrum_camera/ImuInfo.h>

#include "jpeg_decoder.h"

#include <diagnostic_updater/diagnostic_updater.h>

namespace deptrum_camera {
class DtCameraNode {
 public:
  DtCameraNode(ros::NodeHandle &nh, ros::NodeHandle &nh_private,
               std::shared_ptr<dt::Device> device);

  DtCameraNode(const DtCameraNode &) = delete;

  DtCameraNode &operator=(const DtCameraNode &) = delete;

  DtCameraNode(DtCameraNode &&) = delete;

  DtCameraNode &operator=(DtCameraNode &&) = delete;

  ~DtCameraNode() noexcept;

  bool IsInitialized() const;

  void RebootDevice();

  void Clean();

 private:
  struct ImuData {
    ImuData() = default;

    ImuData(stream_index_pair stream, Eigen::Vector3d data, double timestamp)
        : stream_(std::move(stream)), data_(std::move(data)), timestamp_(timestamp) {}

    bool IsSet() const { return timestamp_ >= 0; }

    stream_index_pair stream_{};
    Eigen::Vector3d data_{};
    double timestamp_ = -1;  // in nanoseconds
  };

  void Init();

  void SetupCameraCtrlServices();

  void SetupConfig();

  void GetParameters();

  void SetupDevices();

  void SelectBaseStream();

  void SetupDepthPostProcessFilter();

  void SetupColorPostProcessFilter();

  void SetupFrameCallback();

  void ReadDefaultGain();

  void ReadDefaultExposure();

  void ReadDefaultWhiteBalance();

  std::shared_ptr<dt::Frame> SoftwareDecodeColorFrame(const std::shared_ptr<dt::Frame> &frame);

  void OnNewFrameCallback(std::shared_ptr<dt::Frame> frame, const stream_index_pair &stream_index);

  void PublishMetadata(const std::shared_ptr<dt::Frame> &frame,
                       const stream_index_pair &stream_index, const std_msgs::Header &header);

  void OnNewImuFrameSyncOutputCallback(const std::shared_ptr<dt::Frame> &accel_frame,
                                       const std::shared_ptr<dt::Frame> &gyro_frame);

  void OnNewImuFrameCallback(const std::shared_ptr<dt::Frame> &frame,
                             const stream_index_pair &stream_index);

  bool DecodeColorFrameToBuffer(const std::shared_ptr<dt::Frame> &frame, uint8_t *dest);

  std::shared_ptr<dt::Frame> DecodeIrMjpgFrame(const std::shared_ptr<dt::Frame> &frame);

  void OnNewFrameSetCallback(std::shared_ptr<dt::FrameSet> frame_set);

  std::shared_ptr<dt::Frame> ProcessDepthFrameFilter(std::shared_ptr<dt::Frame> &frame);

  std::shared_ptr<dt::Frame> ProcessColorFrameFilter(std::shared_ptr<dt::Frame> &frame);

  std::shared_ptr<dt::Frame> ProcessRightIrFrameFilter(std::shared_ptr<dt::Frame> &frame);

  std::shared_ptr<dt::Frame> ProcessLeftIrFrameFilter(std::shared_ptr<dt::Frame> &frame);

  uint64_t GetFrameTimestampUs(const std::shared_ptr<dt::Frame> &frame);

  void OnNewColorFrameCallback();

  void PublishPointCloud(const std::shared_ptr<dt::FrameSet> &frame_set);

  void ToHandleSavePointCloud(const std::shared_ptr<dt::Frame> &depth_frame,
                              const sensor_msgs::PointCloud2 &point_cloud_msg);

  void PublishDepthPointCloud(const std::shared_ptr<dt::FrameSet> &frame_set);

  void PublishColoredPointCloud(const std::shared_ptr<dt::FrameSet> &frame_set);

  bool SetupFormatConvertType(DtFormat type);

  void SetupProfiles();

  void UpdateImageConfig(const stream_index_pair &stream_index,
                         const std::shared_ptr<dt::VideoStreamProfile> &selected_profile);
  static void PrintProfiles(const std::shared_ptr<dt::Sensor> &sensor);

  void SetupTopics();

  void CheckAndSetDepthMode();

  void CheckAndSetDeviceMode();

  void SetupPipelineConfig();

  void SetupPublishers();

  void PublishStaticTransforms(const ros::Time &t, const tf2::Vector3 &trans, const tf2::Quaternion &q,
                       const std::string &from, const std::string &to);

  void StartStreams();

  void StartImuSyncStream();

  void StartAccel();

  void StartGyro();

  void StartImu(const stream_index_pair &stream_index);

  void StopStreams();

  void StopImu(const stream_index_pair &stream_index);

  void StopImu();

  void SetDefaultImuMessage(sensor_msgs::Imu &imu_msg);

  ImuInfo CreateImuInfo(const stream_index_pair &stream_index);

  sensor_msgs::Imu CreateUnitImuMessage(const ImuData &accel_data, const ImuData &gyro_data);

  void StartStream(const stream_index_pair &stream_index);

  void StopStream(const stream_index_pair &stream_index);

  void ImageSubscribedCallback(const stream_index_pair &stream_index);

  void ImuSubscribedCallback(const stream_index_pair &stream_index);

  void ImageUnsubscribedCallback(const stream_index_pair &stream_index);

  void ImuUnsubscribedCallback(const stream_index_pair &stream_index);

  void PointCloudSubscribedCallback();

  void PointCloudUnsubscribedCallback();

  void ColoredPointCloudSubscribedCallback();

  void ColoredPointCloudUnsubscribedCallback();

  void CalcAndPublishStaticTransform();

  void PublishDynamicTransforms();

  void PublishStaticTransforms();

  boost::optional<DtCameraParam> GetCameraParam();

  boost::optional<DtCameraParam> GetCameraDepthParam();

  boost::optional<DtCameraParam> GetCameraColorParam();

  int GetCameraParamIndex();

  void SetupCameraInfo();

  // camera control services
  bool SetMirrorCallback(std_srvs::SetBoolRequest &request, std_srvs::SetBoolResponse &response,
                         const stream_index_pair &stream_index);

  bool SetFlipCallback(std_srvs::SetBoolRequest &request, std_srvs::SetBoolResponse &response,
                       const stream_index_pair &stream_index);

  bool SetRotationCallback(SetInt32Request &request, SetInt32Response &response,
                           const stream_index_pair &stream_index);

  bool GetExposureCallback(GetInt32Request &request, GetInt32Response &response,
                           const stream_index_pair &stream_index);

  bool SetExposureCallback(SetInt32TwoParamRequest &request, SetInt32TwoParamResponse &response,
                           const stream_index_pair &stream_index);

  bool GetGainCallback(GetInt32Request &request, GetInt32Response &response,
                       const stream_index_pair &stream_index);

  bool setCropRoiEnableCallback(std_srvs::SetBoolRequest &request,
                                std_srvs::SetBoolResponse &response,
                                const stream_index_pair &stream_index, DtOptionId optionId);

  // bool GetCropRoiEnableCallback(GetBoolRequest &request, GetBoolResponse &response,
  //                              const stream_index_pair &stream_index);
  bool SetCropRoiCallback(SetArraysRequest &request, SetArraysResponse &response,
                        const stream_index_pair &stream_index);

  bool SetAeRoiCallback(SetArraysRequest &request, SetArraysResponse &response,
                        const stream_index_pair &stream_index);

  bool SetGainCallback(SetInt32Request &request, SetInt32Response &response,
                       const stream_index_pair &stream_index);

  bool GetAutoWhiteBalanceCallback(GetInt32Request &request, GetInt32Response &response);

  bool SetAutoWhiteBalanceCallback(SetInt32Request &request, SetInt32Response &response);

  bool GetWhiteBalanceCallback(GetInt32Request &request, GetInt32Response &response);

  bool SetWhiteBalanceCallback(SetInt32Request &request, SetInt32Response &response);

  bool SetAutoExposureCallback(std_srvs::SetBoolRequest &request,
                               std_srvs::SetBoolResponse &response,
                               const stream_index_pair &stream_index);

  bool GetAutoExposureCallback(GetBoolRequest &request, GetBoolResponse &response,
                               const stream_index_pair &stream_index);

  bool SetLaserCallback(std_srvs::SetBoolRequest &request, std_srvs::SetBoolResponse &response);

  bool GetDeviceInfoCallback(GetDeviceInfoRequest &request, GetDeviceInfoResponse &response);

  bool GetSdkVersionCallback(GetStringRequest &request, GetStringResponse &response);

  bool ToggleSensorCallback(std_srvs::SetBoolRequest &request, std_srvs::SetBoolResponse &response,
                            const stream_index_pair &stream_index);

  bool SaveImagesCallback(std_srvs::EmptyRequest &request, std_srvs::EmptyResponse &response);

  void SaveImageToFile(const stream_index_pair &stream_index, const cv::Mat &image,
                       const sensor_msgs::ImagePtr &image_msg);

  bool SavePointCloudCallback(std_srvs::EmptyRequest &request, std_srvs::EmptyResponse &response);

  bool ToggleSensor(const stream_index_pair &stream_index, bool enabled, std::string &msg);

  bool GetCameraParamsCallback(deptrum_camera::GetCameraParamsRequest &request,
                               deptrum_camera::GetCameraParamsResponse &response);

  bool GetSerialNumberCallback(GetStringRequest &request, GetStringResponse &response);

  bool GetCameraInfoCallback(GetCameraInfoRequest &request, GetCameraInfoResponse &response,
                             const stream_index_pair &stream_index);

  bool ResetCameraGainCallback(std_srvs::EmptyRequest &request, std_srvs::EmptyResponse &response,
                               const stream_index_pair &stream_index);

  bool ResetCameraExposureCallback(std_srvs::EmptyRequest &request,
                                   std_srvs::EmptyResponse &response,
                                   const stream_index_pair &stream_index);

  bool ResetCameraWhiteBalanceCallback(std_srvs::EmptyRequest &request,
                                       std_srvs::EmptyResponse &response);

  bool SetFilterCallback(SetFilterRequest &request, SetFilterResponse &response);

  // Set ROI
  void SetColorAutoExposureRoi();
  void SetDepthAutoExposureRoi();

  void SetDepthCropExposureRoi();

 private:
  ros::NodeHandle nh_;
  ros::NodeHandle nh_private_;
  ros::NodeHandle nh_rgb_;
  ros::NodeHandle nh_ir_;
  std::shared_ptr<dt::Device> device_ = nullptr;
  std::shared_ptr<dt::DeviceInfo> device_info_ = nullptr;
  std::atomic_bool is_running_{false};
  std::map<stream_index_pair, std::shared_ptr<RosDtSensor>> sensors_;
  std::map<stream_index_pair, int> width_;
  std::map<stream_index_pair, int> height_;
  std::map<stream_index_pair, int> fps_;
  std::map<stream_index_pair, dt_format> format_;  // for open stream
  std::map<stream_index_pair, std::string> encoding_;
  std::map<stream_index_pair, cv::Mat> images_;
  std::map<stream_index_pair, int> image_format_;  // for cv_bridge
  std::map<stream_index_pair, int> unit_step_size_;
  std::map<stream_index_pair, bool> enable_stream_;
  std::map<stream_index_pair, std::shared_ptr<dt::StreamProfile>> stream_profile_;
  std::map<stream_index_pair, std::shared_ptr<dt::StreamProfileList>> supported_profiles_;
  std::map<stream_index_pair, std::string> stream_name_;
  std::map<stream_index_pair, std::atomic_bool> save_images_;
  std::map<stream_index_pair, int> save_images_count_;
  int max_save_images_count_ = 10;
  std::map<stream_index_pair, image_transport::Publisher> image_publishers_;
  std::map<stream_index_pair, uint32_t> image_seq_;
  std::map<stream_index_pair, ros::Publisher> camera_info_publishers_;
  std::map<stream_index_pair, dt::Sensor::FrameCallback> frame_callback_;
  std::map<stream_index_pair, sensor_msgs::CameraInfo> camera_infos_;
  std::map<stream_index_pair, ros::Publisher> metadata_publishers_;
  std::map<stream_index_pair, ros::Publisher> imu_info_publishers_;
  std::map<stream_index_pair, ros::Publisher> depth_to_other_extrinsics_publishers_;
  std::map<stream_index_pair, DtExtrinsic> depth_to_other_extrinsics_;
  std::map<stream_index_pair, bool> image_flip_;
  std::map<stream_index_pair, bool> image_mirror_;
  std::map<stream_index_pair, bool> stream_started_;
  dt::FormatConvertFilter format_convert_filter_;

  std::map<stream_index_pair, std::string> format_str_;
  std::map<stream_index_pair, std::string> frame_id_;
  std::map<stream_index_pair, std::string> optical_frame_id_;
  std::map<stream_index_pair, std::string> depth_aligned_frame_id_;
  std::map<stream_index_pair, int> default_gain_;
  std::map<stream_index_pair, int> default_exposure_;
  stream_index_pair base_stream_ = DEPTH;
  int default_white_balance_ = 0;
  std::string camera_link_frame_id_ = "camera_link";
  std::string camera_name_ = "camera";
  int camera_mode_ = 0;
  int device_mode_ = 0;
  std::string accel_gyro_frame_id_ = "camera_accel_gyro_optical_frame";
  const std::string imu_frame_id_ = "camera_gyro_frame";
  std::map<stream_index_pair, ros::ServiceServer> get_exposure_srv_;
  std::map<stream_index_pair, ros::ServiceServer> set_exposure_srv_;
  std::map<stream_index_pair, ros::ServiceServer> set_crop_roi_mtof_enable_srv_;
  std::map<stream_index_pair, ros::ServiceServer> set_crop_roi_stof_enable_srv_;
  // std::map<stream_index_pair, ros::ServiceServer> get_crop_roi_enable_srv_;
  std::map<stream_index_pair, ros::ServiceServer> set_crop_roi_srv_;
  std::map<stream_index_pair, ros::ServiceServer> set_ae_roi_srv_;
  std::map<stream_index_pair, ros::ServiceServer> reset_exposure_srv_;
  std::map<stream_index_pair, ros::ServiceServer> get_gain_srv_;
  std::map<stream_index_pair, ros::ServiceServer> set_gain_srv_;
  std::map<stream_index_pair, ros::ServiceServer> reset_gain_srv_;
  std::map<stream_index_pair, ros::ServiceServer> set_mirror_srv_;
  std::map<stream_index_pair, ros::ServiceServer> set_flip_srv_;
  std::map<stream_index_pair, ros::ServiceServer> set_rotation_srv_;
  std::map<stream_index_pair, ros::ServiceServer> toggle_sensor_srv_;
  std::map<stream_index_pair, ros::ServiceServer> set_auto_exposure_srv_;
  std::map<stream_index_pair, ros::ServiceServer> get_auto_exposure_srv_;
  std::map<stream_index_pair, ros::ServiceServer> get_camera_info_srv_;
  ros::ServiceServer get_sdk_version_srv_;
  ros::ServiceServer get_device_info_srv_;
  ros::ServiceServer set_laser_srv_;

  ros::ServiceServer get_auto_white_balance_srv_;
  ros::ServiceServer set_auto_white_balance_srv_;
  ros::ServiceServer get_white_balance_srv_;
  ros::ServiceServer set_white_balance_srv_;
  ros::ServiceServer reset_white_balance_srv_;
  ros::ServiceServer get_serial_number_srv_;
  ros::ServiceServer get_camera_params_srv_;
  ros::ServiceServer save_point_cloud_srv_;
  ros::ServiceServer save_images_srv_;
  ros::ServiceServer set_filter_srv_;
  ros::ServiceServer set_write_customerdata_srv_;
  ros::ServiceServer set_read_customerdata_srv_;

  bool publish_tf_ = true;
  std::shared_ptr<tf2_ros::StaticTransformBroadcaster> static_tf_broadcaster_ = nullptr;
  std::shared_ptr<tf2_ros::TransformBroadcaster> dynamic_tf_broadcaster_ = nullptr;
  std::vector<geometry_msgs::TransformStamped> static_tf_msgs_;
  std::shared_ptr<std::thread> tf_thread_ = nullptr;
  std::condition_variable tf_cv_;
  double tf_publish_rate_ = 10.0;
  bool depth_registration_ = false;
  bool enable_frame_sync_ = false;
  std::recursive_mutex device_lock_;
  std::shared_ptr<camera_info_manager::CameraInfoManager> color_camera_info_manager_ = nullptr;
  std::shared_ptr<camera_info_manager::CameraInfoManager> ir_camera_info_manager_ = nullptr;
  std::vector<std::shared_ptr<dt::Filter>> depth_filter_list_;
  std::vector<std::shared_ptr<dt::Filter>> color_filter_list_;
  std::vector<std::shared_ptr<dt::Filter>> left_ir_filter_list_;
  std::vector<std::shared_ptr<dt::Filter>> right_ir_filter_list_;
  std::string ir_info_uri_;
  std::string color_info_uri_;
  bool enable_d2c_viewer_ = false;
  std::shared_ptr<D2CViewer> d2c_viewer_ = nullptr;
  bool enable_pipeline_ = false;
  std::shared_ptr<dt::Pipeline> pipeline_ = nullptr;
  std::shared_ptr<dt::Config> pipeline_config_ = nullptr;
  ros::Publisher depth_cloud_pub_;
  ros::Publisher depth_cloud_pub_second_;

  ros::Publisher depth_registered_cloud_pub_;
  sensor_msgs::PointCloud2 cloud_msg_;
  std::recursive_mutex cloud_mutex_;
  std::atomic_bool pipeline_started_{false};
  bool enable_point_cloud_ = false;
  bool enable_colored_point_cloud_ = false;
  std::atomic_bool save_point_cloud_{false};
  std::atomic_bool save_colored_point_cloud_{false};
  boost::optional<DtCameraParam> camera_params_;
  bool is_initialized_ = false;
  bool enable_soft_filter_ = true;
  bool enable_color_auto_exposure_ = true;
  bool enable_color_auto_exposure_priority_ = false;
  bool enable_color_auto_white_balance_ = true;
  bool enable_color_backlight_compenstation_ = false;
  std::string color_powerline_freq_;
  bool enable_color_decimation_filter_ = false;
  // color ae roi
  int color_ae_roi_left_ = -1;
  int color_ae_roi_top_ = -1;
  int color_ae_roi_right_ = -1;
  int color_ae_roi_bottom_ = -1;
  int color_exposure_ = -1;
  int color_gain_ = -1;
  int color_brightness_ = -1;
  int color_sharpness_ = -1;
  int color_gamma_ = -1;
  int color_white_balance_ = -1;
  int color_saturation_ = -1;
  int color_constrast_ = -1;
  int color_hue_ = -1;
  int color_ae_max_exposure_ = -1;
  int color_decimation_filter_scale_ = -1;
  bool enable_ir_auto_exposure_ = true;

  bool enable_depth_auto_exposure_ = true;
  int depth_exposure_mode_ = kDtOptionMtofDepthExposureInt;
  int depth_exposure_mtof_value_ = -1;
  int depth_exposure_stof_value_ = -1;
  int depth_exposure_ = -1;

  bool enable_depth_scale_ = true;
  // crop roi
  bool depth_crop_roi_mtof_enable_ = false;
  int depth_crop_roi_mtof_left_ = -1;
  int depth_crop_roi_mtof_top_ = -1;
  int depth_crop_roi_mtof_width_ = -1;
  int depth_crop_roi_mtof_height_ = -1;

  bool depth_crop_roi_stof_enable_ = false;
  int depth_crop_roi_stof_left_ = -1;
  int depth_crop_roi_stof_top_ = -1;
  int depth_crop_roi_stof_width_ = -1;
  int depth_crop_roi_stof_height_ = -1;

  // depth ae roi
  int depth_ae_roi_left_ = -1;
  int depth_ae_roi_top_ = -1;
  int depth_ae_roi_right_ = -1;
  int depth_ae_roi_bottom_ = -1;
  int ir_exposure_ = -1;
  bool enable_right_ir_sequence_id_filter_ = false;
  int right_ir_sequence_id_filter_id_ = -1;
  bool enable_left_ir_sequence_id_filter_ = false;
  int left_ir_sequence_id_filter_id_ = -1;
  int ir_ae_max_exposure_ = -1;
  std::string depth_filter_config_;
  bool enable_depth_filter_ = false;

  std::string depth_work_mode_;
  bool enable_ptp_config_ = false;
  // IMU
  std::map<stream_index_pair, ros::Publisher> imu_publishers_;
  std::map<stream_index_pair, std::string> imu_rate_;
  std::map<stream_index_pair, std::string> imu_range_;
  std::map<stream_index_pair, std::string> imu_qos_;
  std::map<stream_index_pair, bool> imu_started_;
  std::map<stream_index_pair, std::shared_ptr<dt::Sensor>> imu_sensor_;
  double liner_accel_cov_ = 0.0001;
  double angular_vel_cov_ = 0.0001;
  std::deque<ImuData> imu_history_;
  ImuData accel_data_{ACCEL, {0, 0, 0}, -1.0};
  bool enable_accel_data_correction_ = true;
  bool enable_gyro_data_correction_ = true;

  bool enable_sync_output_accel_gyro_ = false;
  std::shared_ptr<dt::Pipeline> imuPipeline_ = nullptr;
  ros::Publisher imu_gyro_accel_publisher_;
  bool imu_sync_output_start_ = false;

  // mjpeg decoder
  std::shared_ptr<JpegDecoder> mjpeg_decoder_ = nullptr;
  uint8_t *rgb_buffer_ = nullptr;
  std::atomic_bool rgb_is_decoded_{false};

  // For color
  std::queue<std::shared_ptr<dt::FrameSet>> colorFrameQueue_;
  std::shared_ptr<std::thread> colorFrameThread_ = nullptr;
  std::mutex colorFrameMtx_;
  std::condition_variable colorFrameCV_;
  // ordered point cloud
  bool ordered_pc_ = false;
  // filter switch
  bool enable_decimation_filter_ = false;
  bool enable_hdr_merge_ = false;
  bool enable_sequenced_filter_ = false;
  bool enable_disaparity_to_depth_ = true;
  bool enable_threshold_filter_ = false;
  bool enable_spatial_filter_ = true;
  bool enable_temporal_filter_ = false;
  bool enable_hole_filling_filter_ = false;
  // filter params
  int decimation_filter_scale_range_ = -1;
  int sequence_id_filter_id_ = -1;
  int threshold_filter_max_ = -1;
  int threshold_filter_min_ = -1;
  float hardware_noise_removal_filter_threshold_ = -1.0;
  ros::Publisher filter_status_pub_;
  nlohmann::json filter_status_;
  std::shared_ptr<std::thread> diagnostics_thread_ = nullptr;
  bool enable_laser_ = true;
  std::shared_ptr<dt::Align> align_filter_ = nullptr;
  DtStreamType align_target_stream_ = kDtStreamColor;
  dt::PointCloudFilter depth_point_cloud_filter_;
  dt::PointCloudFilter color_point_cloud_filter_;
  boost::optional<DtXyTables> xy_tables_;
  float *xy_table_data_ = nullptr;
  uint32_t xy_table_data_size_ = 0;
  uint8_t *rgb_point_cloud_buffer_ = nullptr;
  uint32_t rgb_point_cloud_buffer_size_ = 0;
  ros::Publisher sdk_version_pub_;
  bool enable_heartbeat_ = false;
  bool has_first_color_frame_ = false;
  // rotation degree
  std::map<stream_index_pair, int> image_rotation_;
  std::string time_domain_ = "global";
};

}  // namespace deptrum_camera
