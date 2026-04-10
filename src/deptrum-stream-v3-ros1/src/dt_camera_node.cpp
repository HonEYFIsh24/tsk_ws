// Copyright (c) Deptrum Inc. All Rights Reserved.

#include "deptrum_camera/dt_camera_node.h"
#include "deptrum/hpp/dt_utils.hpp"
#if defined(USE_RK_HW_DECODER)
#include "deptrum_camera/rk_mpp_decoder.h"
#elif defined(USE_NV_HW_DECODER)
#include "deptrum_camera/jetson_nv_decoder.h"
#endif

namespace deptrum_camera {
DtCameraNode::DtCameraNode(ros::NodeHandle& nh, ros::NodeHandle& nh_private,
                           std::shared_ptr<dt::Device> device)
    : nh_(nh),
      nh_private_(nh_private),
      device_(std::move(device)),
      device_info_(device_->GetDeviceInfo()) {
  stream_name_[COLOR] = "color";
  stream_name_[DEPTH] = "depth";
  stream_name_[INFRA0] = "ir";
  stream_name_[INFRA1] = "ir2";
  stream_name_[ACCEL] = "accel";
  stream_name_[GYRO] = "gyro";
  nh_ir_ = ros::NodeHandle(stream_name_[INFRA0]);
  nh_rgb_ = ros::NodeHandle(stream_name_[COLOR]);
  Init();
}

void DtCameraNode::Init() {
  std::lock_guard<decltype(device_lock_)> lock(device_lock_);
  CHECK_NOTNULL(device_.get());
  is_running_ = true;
  SetupConfig();
  GetParameters();
  ROS_INFO("test---- SetupDevices");
  SetupDevices();
  ROS_INFO("test---- SetupDepthPostProcessFilter");
  SetupDepthPostProcessFilter();
  ROS_INFO("test---- SetupColorPostProcessFilter");
  SetupColorPostProcessFilter();

  ROS_INFO("test---- SelectBaseStream");
  SelectBaseStream();
  ROS_INFO("test---- SetupProfiles");
  SetupProfiles();
  ROS_INFO("test---- SetupCameraInfo");
  SetupCameraInfo();
  ROS_INFO("test---- SetupTopics");
  SetupTopics();
  ROS_INFO("test---- SetupCameraCtrlServices");
  SetupCameraCtrlServices();
  ROS_INFO("test---- SetupFrameCallback");
  SetupFrameCallback();
  ROS_INFO("test---- ReadDefaultExposure");
  ReadDefaultExposure();
  ROS_INFO("test---- ReadDefaultGain");
  ReadDefaultGain();
  ROS_INFO("test---- ReadDefaultWhiteBalance");
  ReadDefaultWhiteBalance();
#if defined(USE_RK_HW_DECODER)
  mjpeg_decoder_ = std::make_shared<RkMjpegDecoder>(width_[COLOR], height_[COLOR]);
#elif defined(USE_NV_HW_DECODER)
  mjpeg_decoder_ = std::make_shared<JetsonNvJpegDecoder>(width_[COLOR], height_[COLOR]);
#endif
  ROS_INFO_STREAM("test---- enable_stream_color " << enable_stream_[COLOR]);
  if (enable_stream_[COLOR]) {
    CHECK(width_[COLOR] > 0 && height_[COLOR] > 0);
    rgb_buffer_ = new uint8_t[width_[COLOR] * height_[COLOR] * 4];
  }
  if (enable_colored_point_cloud_ && enable_stream_[COLOR] && enable_stream_[DEPTH]) {
    CHECK(width_[COLOR] > 0 && height_[COLOR] > 0);
    rgb_point_cloud_buffer_size_ = width_[COLOR] * height_[COLOR] * sizeof(DtColorPoint);
    rgb_point_cloud_buffer_ = new uint8_t[rgb_point_cloud_buffer_size_];
    xy_table_data_size_ = width_[COLOR] * height_[COLOR] * 2;
    xy_table_data_ = new float[xy_table_data_size_];
  }
  rgb_is_decoded_ = false;
  is_initialized_ = true;
}

bool DtCameraNode::IsInitialized() const { return is_initialized_; }

void DtCameraNode::RebootDevice() {
  ROS_INFO("Reboot device");
  if (device_) {
    device_->Reboot();
  }
  ROS_INFO("Reboot device DONE");
}

void DtCameraNode::Clean() {
  ROS_INFO_STREAM("DtCameraNode::~DtCameraNode() start");
  std::lock_guard<decltype(device_lock_)> lock(device_lock_);
  is_running_ = false;
  ROS_INFO_STREAM("DtCameraNode::~DtCameraNode() stop tf thread");
  if (tf_thread_ && tf_thread_->joinable()) {
    tf_thread_->join();
  }

  if (colorFrameThread_ && colorFrameThread_->joinable()) {
    colorFrameCV_.notify_all();
    colorFrameThread_->join();
  }
  if (diagnostics_thread_ && diagnostics_thread_->joinable()) {
    diagnostics_thread_->join();
  }

  ROS_INFO_STREAM("DtCameraNode::~DtCameraNode() stop stream");
  StopStreams();
  ROS_INFO_STREAM("DtCameraNode::~DtCameraNode() delete rgb_buffer");
  delete[] rgb_buffer_;
  delete[] rgb_point_cloud_buffer_;
  delete[] xy_table_data_;
  ROS_INFO_STREAM("DtCameraNode::~DtCameraNode() end");
}

DtCameraNode::~DtCameraNode() noexcept { Clean(); }
void DtCameraNode::GetParameters() {
  camera_name_ = nh_private_.param<std::string>("camera_name", "camera");
  camera_link_frame_id_ = camera_name_ + "_link";
  camera_mode_ = nh_private_.param<int>("camera_mode", 0);
  device_mode_ = nh_private_.param<int>("device_mode", 0);

  for (const auto& stream_index : IMAGE_STREAMS) {
    frame_id_[stream_index] = camera_name_ + "_" + stream_name_[stream_index] + "_frame";
    optical_frame_id_[stream_index] =
        camera_name_ + "_" + stream_name_[stream_index] + "_optical_frame";
  }
  for (const auto& stream_index : IMAGE_STREAMS) {
    std::string param_name = stream_name_[stream_index] + "_width";
    width_[stream_index] = nh_private_.param<int>(param_name, IMAGE_WIDTH);
    param_name = stream_name_[stream_index] + "_height";
    height_[stream_index] = nh_private_.param<int>(param_name, IMAGE_HEIGHT);
    param_name = stream_name_[stream_index] + "_fps";
    fps_[stream_index] = nh_private_.param<int>(param_name, IMAGE_FPS);
    param_name = "enable_" + stream_name_[stream_index];
    enable_stream_[stream_index] = nh_private_.param<bool>(param_name, false);
    param_name = stream_name_[stream_index] + "_flip";
    image_flip_[stream_index] = nh_private_.param<bool>(param_name, false);
    param_name = stream_name_[stream_index] + "_mirror";
    image_mirror_[stream_index] = nh_private_.param<bool>(param_name, false);
    param_name = stream_name_[stream_index] + "_format";
    format_str_[stream_index] =
        nh_private_.param<std::string>(param_name, format_str_[stream_index]);
    format_[stream_index] = DtFormatFromString(format_str_[stream_index]);
    param_name = stream_name_[stream_index] + "_rotation";
    image_rotation_[stream_index] = nh_private_.param<int>(param_name, -1);
  }
  depth_aligned_frame_id_[DEPTH] = optical_frame_id_[COLOR];

  accel_gyro_frame_id_ = camera_name_ + "_accel_gyro_optical_frame";

  publish_tf_ = nh_private_.param<bool>("publish_tf", false);
  depth_registration_ = nh_private_.param<bool>("depth_registration", false);
  enable_frame_sync_ = nh_private_.param<bool>("enable_frame_sync", false);
  ir_info_uri_ = nh_private_.param<std::string>("ir_info_uri", "");
  color_info_uri_ = nh_private_.param<std::string>("color_info_uri", "");
  enable_d2c_viewer_ = nh_private_.param<bool>("enable_d2c_viewer", false);
  enable_pipeline_ = nh_private_.param<bool>("enable_pipeline", true);
  enable_point_cloud_ = nh_private_.param<bool>("enable_point_cloud", true);
  enable_colored_point_cloud_ = nh_private_.param<bool>("enable_colored_point_cloud", false);
  depth_work_mode_ = nh_private_.param<std::string>("depth_work_mode", "");
  enable_soft_filter_ = nh_private_.param<bool>("enable_soft_filter", true);
  enable_color_auto_exposure_ = nh_private_.param<bool>("enable_color_auto_exposure", true);
  enable_color_auto_exposure_priority_ =
      nh_private_.param<bool>("enable_color_auto_exposure_priority", false);
  enable_color_auto_white_balance_ =
      nh_private_.param<bool>("enable_color_auto_white_balance", true);
  enable_color_backlight_compenstation_ =
      nh_private_.param<bool>("enable_color_backlight_compenstation", false);
  color_powerline_freq_ = nh_private_.param<std::string>("color_powerline_freq", "");
  enable_color_decimation_filter_ =
      nh_private_.param<bool>("enable_color_decimation_filter", false);
  color_ae_roi_left_ = nh_private_.param<int>("color_ae_roi_left", -1);
  color_ae_roi_top_ = nh_private_.param<int>("color_ae_roi_top", -1);
  color_ae_roi_right_ = nh_private_.param<int>("color_ae_roi_right", -1);
  color_ae_roi_bottom_ = nh_private_.param<int>("color_ae_roi_bottom", -1);
  color_exposure_ = nh_private_.param<int>("color_exposure", -1);
  color_gain_ = nh_private_.param<int>("color_gain", -1);
  color_brightness_ = nh_private_.param<int>("color_brightness", -1);
  color_sharpness_ = nh_private_.param<int>("color_sharpness", -1);
  color_gamma_ = nh_private_.param<int>("color_gamma", -1);
  color_white_balance_ = nh_private_.param<int>("color_white_balance", -1);
  color_saturation_ = nh_private_.param<int>("color_saturation", -1);
  color_constrast_ = nh_private_.param<int>("color_constrast", -1);
  color_hue_ = nh_private_.param<int>("color_hue", -1);
  color_ae_max_exposure_ = nh_private_.param<int>("color_ae_max_exposure", -1);
  color_decimation_filter_scale_ = nh_private_.param<int>("color_decimation_filter_scale", -1);
  depth_ae_roi_left_ = nh_private_.param<int>("depth_ae_roi_left", -1);
  depth_ae_roi_top_ = nh_private_.param<int>("depth_ae_roi_top", -1);
  depth_ae_roi_right_ = nh_private_.param<int>("depth_ae_roi_right", -1);
  depth_ae_roi_bottom_ = nh_private_.param<int>("depth_ae_roi_bottom", -1);
  enable_ir_auto_exposure_ = nh_private_.param<bool>("enable_ir_auto_exposure", true);

  depth_crop_roi_mtof_enable_ = nh_private_.param<bool>("depth_crop_roi_mtof_enable", false);
  depth_crop_roi_mtof_left_ = nh_private_.param<int>("depth_crop_roi_mtof_left", -1);
  depth_crop_roi_mtof_top_ = nh_private_.param<int>("depth_crop_roi_mtof_top", -1);
  depth_crop_roi_mtof_width_ = nh_private_.param<int>("depth_crop_roi_mtof_width", -1);
  depth_crop_roi_mtof_height_ = nh_private_.param<int>("depth_crop_roi_mtof_height", -1);

  ROS_INFO_STREAM("test---- depth_crop_roi_mtof_enable_:"
                  << depth_crop_roi_mtof_enable_
                  << " ,depth_crop_roi_mtof_left_: " << depth_crop_roi_mtof_left_
                  << " ,depth_crop_roi_mtof_width_: " << depth_crop_roi_mtof_width_
                  << " ,depth_crop_roi_mtof_height_ :" << depth_crop_roi_mtof_height_);

  depth_crop_roi_stof_enable_ = nh_private_.param<bool>("depth_crop_roi_stof_enable", false);
  depth_crop_roi_stof_left_ = nh_private_.param<int>("depth_crop_roi_stof_left", -1);
  depth_crop_roi_stof_top_ = nh_private_.param<int>("depth_crop_roi_stof_top", -1);
  depth_crop_roi_stof_width_ = nh_private_.param<int>("depth_crop_roi_stof_width", -1);
  depth_crop_roi_stof_height_ = nh_private_.param<int>("depth_crop_roi_stof_height", -1);

  enable_depth_auto_exposure_ = nh_private_.param<bool>("enable_depth_auto_exposure", true);

  depth_exposure_mode_ =
      nh_private_.param<int>("depth_exposure_mode", kDtOptionMtofDepthExposureInt);
  depth_exposure_mtof_value_ = nh_private_.param<int>("depth_exposure_mtof_value", -1);
  depth_exposure_stof_value_ = nh_private_.param<int>("depth_exposure_stof_value", -1);
  depth_exposure_ = nh_private_.param<int>("depth_exposure", -1);

  ir_exposure_ = nh_private_.param<int>("ir_exposure", -1);
  ir_ae_max_exposure_ = nh_private_.param<int>("ir_ae_max_exposure", -1);
  enable_right_ir_sequence_id_filter_ =
      nh_private_.param<bool>("enable_right_ir_sequence_id_filter", false);
  right_ir_sequence_id_filter_id_ = nh_private_.param<int>("right_ir_sequence_id_filter_id", -1);
  enable_left_ir_sequence_id_filter_ =
      nh_private_.param<bool>("enable_left_ir_sequence_id_filter", false);
  left_ir_sequence_id_filter_id_ = nh_private_.param<int>("left_ir_sequence_id_filter_id", -1);
  enable_ptp_config_ = nh_private_.param<bool>("enable_ptp_config", false);
  if (enable_colored_point_cloud_ || enable_d2c_viewer_) {
    depth_registration_ = true;
  }
  depth_filter_config_ = nh_private_.param<std::string>("depth_filter_config", "");
  ordered_pc_ = nh_private_.param<bool>("ordered_pc", false);
  max_save_images_count_ = nh_private_.param<int>("max_save_images_count", 10);
  if (!depth_filter_config_.empty()) {
    enable_depth_filter_ = true;
  }

  enable_sync_output_accel_gyro_ = nh_private_.param<bool>("enable_sync_output_accel_gyro", false);
  for (const auto& stream_index : HID_STREAMS) {
    std::string param_name = "enable_" + stream_name_[stream_index];
    enable_stream_[stream_index] = nh_private_.param<bool>(param_name, false);
    if (enable_sync_output_accel_gyro_) {
      enable_stream_[stream_index] = true;
    }
    param_name = stream_name_[stream_index] + "_rate";
    imu_rate_[stream_index] = nh_private_.param<std::string>(param_name, "");
    param_name = stream_name_[stream_index] + "_range";
    imu_range_[stream_index] = nh_private_.param<std::string>(param_name, "");
    param_name = camera_name_ + "_" + stream_name_[stream_index] + "_frame_id";
    std::string default_frame_id = camera_name_ + "_" + stream_name_[stream_index] + "_frame";
    frame_id_[stream_index] = nh_private_.param<std::string>(param_name, default_frame_id);
    std::string default_optical_frame_id =
        camera_name_ + "_" + stream_name_[stream_index] + "_optical_frame";
    param_name = stream_name_[stream_index] + "_optical_frame_id";
    optical_frame_id_[stream_index] =
        nh_private_.param<std::string>(param_name, default_optical_frame_id);
  }
  // filter switch
  enable_decimation_filter_ = nh_private_.param<bool>("enable_decimation_filter", false);
  enable_hdr_merge_ = nh_private_.param<bool>("enable_hdr_merge", false);
  enable_sequenced_filter_ = nh_private_.param<bool>("enable_sequenced_filter", false);
  enable_disaparity_to_depth_ = nh_private_.param<bool>("enable_disaparity_to_depth", true);
  enable_threshold_filter_ = nh_private_.param<bool>("enable_threshold_filter", false);
  enable_spatial_filter_ = nh_private_.param<bool>("enable_spatial_filter", false);
  enable_temporal_filter_ = nh_private_.param<bool>("enable_temporal_filter", false);
  enable_hole_filling_filter_ = nh_private_.param<bool>("enable_hole_filling_filter", false);
  decimation_filter_scale_range_ = nh_private_.param<int>("decimation_filter_scale_range", -1);
  sequence_id_filter_id_ = nh_private_.param<int>("sequence_id_filter_id", -1);
  threshold_filter_max_ = nh_private_.param<int>("threshold_filter_max", -1);
  threshold_filter_min_ = nh_private_.param<int>("threshold_filter_min", -1);
  hardware_noise_removal_filter_threshold_ =
      nh_private_.param<float>("hardware_noise_removal_filter_threshold", -1.0);
  enable_laser_ = nh_private_.param<bool>("enable_laser", true);
  std::string align_target_stream_str_;
  align_target_stream_str_ = nh_private_.param<std::string>("align_target_stream", "COLOR");
  align_target_stream_ = DtStreamTypeFromString(align_target_stream_str_);
  ROS_INFO_STREAM("align_target_stream_: " << align_target_stream_);
  enable_depth_scale_ = nh_private_.param<bool>("enable_depth_scale", true);
  tf_publish_rate_ = nh_private_.param<double>("tf_publish_rate", 0.0);
  enable_heartbeat_ = nh_private_.param<bool>("enable_heartbeat", false);
  time_domain_ = nh_private_.param<std::string>("time_domain", "global");

  ROS_INFO_STREAM("test---- to GetDeviceInfo");
  auto device_info = device_->GetDeviceInfo();
  ROS_INFO_STREAM("test---- GetDeviceInfo over");
  CHECK_NOTNULL(device_info.get());
  ROS_INFO_STREAM("test---- to getPid");
  auto pid = device_info->GetPid();
  ROS_INFO_STREAM("test---- pid:" << pid);

  ROS_INFO_STREAM("current time domain:" << time_domain_);

}

void DtCameraNode::StartStreams() {
  std::lock_guard<decltype(device_lock_)> lock(device_lock_);
  if (enable_pipeline_) {
    CHECK_NOTNULL(pipeline_.get());
    if (enable_frame_sync_) {
      ROS_INFO_STREAM("StartStreams() ====Enable frame sync====");
      pipeline_->EnableFrameSync();
    } else {
      pipeline_->DisableFrameSync();
    }
    try {
      SetupPipelineConfig();
      ROS_INFO_STREAM("StartStreams() pipeline_->start pipeline_config_");
      pipeline_->Start(pipeline_config_, [this](const std::shared_ptr<dt::FrameSet>& frame_set) {
        CHECK_NOTNULL(frame_set.get());
        this->OnNewFrameSetCallback(frame_set);
      });
    } catch (const dt::Error& e) {
      ROS_ERROR_STREAM("StartStreams() failed to start pipeline: "
                       << e.GetMessage() << " try to disable ir stream try again");
      enable_stream_[INFRA0] = false;
      SetupPipelineConfig();
      pipeline_->Start(pipeline_config_, [this](const std::shared_ptr<dt::FrameSet>& frame_set) {
        CHECK_NOTNULL(frame_set.get());
        this->OnNewFrameSetCallback(frame_set);
      });
    } catch (...) {
      ROS_ERROR_STREAM("StartStreams() failed to start pipeline");
      throw;
    }
    if (!colorFrameThread_ && enable_stream_[COLOR]) {
      ROS_INFO_STREAM("StartStreams() Create color frame read thread.");
      colorFrameThread_ = std::make_shared<std::thread>([this]() { OnNewColorFrameCallback(); });
    }
    pipeline_started_ = true;
  } else {
    for (const auto& stream_index : IMAGE_STREAMS) {
      if (enable_stream_[stream_index] && !stream_started_[stream_index]) {
        StartStream(stream_index);
      }
    }
  }
}

void DtCameraNode::StartImuSyncStream() {
  if (!imuPipeline_) {
    ROS_INFO_STREAM("start IMU sync stream failed, IMU pileline is not initialized!");
    return;
  }

  if (imu_sync_output_start_) {
    ROS_INFO_STREAM("IMU sync stream already started.");
    return;
  }

  // ACCEL
  auto accelProfiles = imuPipeline_->GetStreamProfileList(kDtSensorAccel);
  auto accel_range = FullAccelScaleRangeFromString(imu_range_[ACCEL]);
  auto accel_rate = SampleRateFromString(imu_rate_[ACCEL]);
  auto accelProfile = accelProfiles->GetAccelStreamProfile(accel_range, accel_rate);
  // GYRO
  auto gyroProfiles = imuPipeline_->GetStreamProfileList(kDtSensorGyro);
  auto gyro_range = FullGyroScaleRangeFromString(imu_range_[GYRO]);
  auto gyro_rate = SampleRateFromString(imu_rate_[GYRO]);
  auto gyroProfile = gyroProfiles->GetGyroStreamProfile(gyro_range, gyro_rate);
  std::shared_ptr<dt::Config> imuConfig = std::make_shared<dt::Config>();
  imuConfig->EnableStream(accelProfile);
  imuConfig->EnableStream(gyroProfile);
  imuPipeline_->EnableFrameSync();
  imuPipeline_->Start(imuConfig, [&](std::shared_ptr<dt::Frame> frame) {
    auto frameSet = frame->As<dt::FrameSet>();
    if (!frameSet) {
      return;
    }
    auto aFrame = frameSet->GetFrame(kDtFrameAccel);
    auto gFrame = frameSet->GetFrame(kDtFrameGyro);
    if (aFrame && gFrame) {
      OnNewImuFrameSyncOutputCallback(aFrame, gFrame);
    }
  });

  imu_sync_output_start_ = true;
  if (!imu_sync_output_start_) {
    ROS_ERROR_STREAM(
        "Failed to start IMU stream, please check the imu_rate and imu_range parameters.");
  } else {
    ROS_INFO_STREAM("start accel stream with range: " << FullAccelScaleRangeToString(accel_range)
                                                      << ",rate:" << SampleRateToString(accel_rate)
                                                      << ", and start gyro stream with range:"
                                                      << FullGyroScaleRangeToString(gyro_range)
                                                      << ",rate:" << SampleRateToString(gyro_rate));
  }
}

void DtCameraNode::StartAccel() {
  const stream_index_pair stream_index = ACCEL;
  if (!enable_stream_[stream_index]) {
    return;
  }
  if (!sensors_[stream_index] || !imu_sensor_[stream_index]) {
    ROS_ERROR_STREAM("accel sensor is null");
    return;
  }
  auto profile_list = sensors_[ACCEL]->GetStreamProfileList();
  for (size_t i = 0; i < profile_list->GetCount(); i++) {
    auto item = profile_list->GetProfile(i);
    auto profile = item->As<dt::AccelStreamProfile>();
    auto accel_rate = SampleRateFromString(imu_rate_[stream_index]);
    auto accel_range = FullAccelScaleRangeFromString(imu_range_[stream_index]);
    if (profile->GetFullScaleRange() == accel_range && profile->GetSampleRate() == accel_rate) {
      imu_sensor_[stream_index]->Start(
          profile, [this, stream_index](const std::shared_ptr<dt::Frame>& frame) {
            OnNewImuFrameCallback(frame, stream_index);
          });
      imu_started_[stream_index] = true;
      ROS_INFO_STREAM("start accel stream with " << FullAccelScaleRangeToString(accel_range)
                                                 << " range and " << SampleRateToString(accel_rate)
                                                 << " rate");
    }
  }
  if (enable_stream_[stream_index] && !imu_started_[stream_index]) {
    ROS_INFO_STREAM("Failed to start IMU stream: "
                    << stream_name_[stream_index]
                    << ", please check the imu_rate and imu_range parameters");
  }
}

void DtCameraNode::StartGyro() {
  const stream_index_pair stream_index = GYRO;
  if (!enable_stream_[stream_index]) {
    return;
  }
  if (!sensors_[stream_index] || !imu_sensor_[stream_index]) {
    ROS_ERROR_STREAM("gyro sensor is null");
  }
  auto profile_list = sensors_[GYRO]->GetStreamProfileList();
  for (size_t i = 0; i < profile_list->GetCount(); i++) {
    auto item = profile_list->GetProfile(i);
    auto profile = item->As<dt::GyroStreamProfile>();
    auto gyro_rate = SampleRateFromString(imu_rate_[stream_index]);
    auto gyro_range = FullGyroScaleRangeFromString(imu_range_[stream_index]);
    if (profile->GetFullScaleRange() == gyro_range && profile->GetSampleRate() == gyro_rate) {
      imu_sensor_[stream_index]->Start(
          profile, [this, stream_index](const std::shared_ptr<dt::Frame>& frame) {
            OnNewImuFrameCallback(frame, stream_index);
          });
      imu_started_[stream_index] = true;
      ROS_INFO_STREAM("start gyro stream with " << FullGyroScaleRangeToString(gyro_range)
                                                << " range and " << SampleRateToString(gyro_rate)
                                                << " rate");
    }
  }
  if (enable_stream_[stream_index] && !imu_started_[stream_index]) {
    ROS_INFO_STREAM("Failed to start IMU stream: "
                    << stream_name_[stream_index]
                    << ", please check the imu_rate and imu_range parameters");
  }
}

void DtCameraNode::StartImu(const stream_index_pair& stream_index) {
  if (enable_sync_output_accel_gyro_) {
    StartImuSyncStream();
  } else {
    if (!stream_profile_[stream_index]) {
      ROS_ERROR_STREAM("stream " << stream_name_[stream_index] << " profile is null!");
      return;
    }
    auto profile = stream_profile_[stream_index];
    imu_sensor_[stream_index]->Start(profile,
                                     [this, stream_index](const std::shared_ptr<dt::Frame>& frame) {
                                       OnNewImuFrameCallback(frame, stream_index);
                                     });
    imu_started_[stream_index] = true;
    ROS_INFO_STREAM("start IMU stream with " << imu_range_[stream_index] << " range and "
                                             << imu_rate_[stream_index] << " rate");
  }
}

void DtCameraNode::StopStreams() {
  std::lock_guard<decltype(device_lock_)> lock(device_lock_);
  if (enable_pipeline_ && pipeline_ && pipeline_started_) {
    CHECK_NOTNULL(pipeline_.get());
    try {
      pipeline_->Stop();
      ROS_INFO_STREAM("StopStreams pipeline_->Stop ");
    } catch (const dt::Error& e) {
      ROS_ERROR_STREAM("Failed to stop pipeline: " << e.GetMessage());
    }
    pipeline_started_ = false;
  } else {
    for (const auto& stream_index : IMAGE_STREAMS) {
      if (stream_started_[stream_index]) {
        StopStream(stream_index);
      }
    }
  }
}

void DtCameraNode::StopImu(const deptrum_camera::stream_index_pair& stream_index) {
  if (imu_started_[stream_index]) {
    CHECK(sensors_.count(stream_index));
    ROS_INFO_STREAM("stop " << stream_name_[stream_index] << " stream");
    imu_sensor_[stream_index]->Stop();
    imu_started_[stream_index] = false;
  }
}

void DtCameraNode::StopImu() {
  if (enable_sync_output_accel_gyro_) {
    if (!imu_sync_output_start_ || !imuPipeline_) {
      return;
    }
    try {
      imuPipeline_->Stop();
    } catch (const dt::Error& e) {
      ROS_ERROR_STREAM("Failed to stop imu pipeline: " << e.GetMessage());
    }
  } else {
    for (const auto& stream_index : HID_STREAMS) {
      if (imu_started_[stream_index]) {
        StopImu(stream_index);
      }
    }
  }
}

void DtCameraNode::StartStream(const stream_index_pair& stream_index) {
  std::lock_guard<decltype(device_lock_)> lock(device_lock_);
  if (enable_pipeline_) {
    ROS_WARN_STREAM("StartStream Cannot start stream when pipeline is enabled");
    return;
  }
  if (!enable_stream_[stream_index]) {
    ROS_WARN_STREAM("StartStream Stream " << stream_name_[stream_index]
                                          << " is not enabled, cannot start it.");
    return;
  }
  if (stream_started_[stream_index]) {
    ROS_WARN_STREAM("StartStream Stream " << stream_name_[stream_index] << " is already started.");
    return;
  }
  ROS_INFO_STREAM("StartStream Starting stream " << stream_name_[stream_index] << "...");
  bool has_subscriber = image_publishers_[stream_index].getNumSubscribers() > 0;
  if (!has_subscriber) {
    ROS_INFO_STREAM("StartStream No subscriber for stream " << stream_name_[stream_index]
                                                            << ", skip it.");
    return;
  }
  CHECK_GE(stream_profile_.count(stream_index), 0u);
  CHECK_GE(frame_callback_.count(stream_index), 0u);
  CHECK_GE(sensors_.count(stream_index), 0u);
  auto callback = frame_callback_[stream_index];
  auto profile = stream_profile_[stream_index];
  try {
    sensors_[stream_index]->StartStream(profile, callback);
    stream_started_[stream_index] = true;

    if (!colorFrameThread_ && stream_index == COLOR) {
      ROS_INFO_STREAM("StartStream Create color frame read thread.");
      colorFrameThread_ = std::make_shared<std::thread>([this]() { OnNewColorFrameCallback(); });
    }
    ROS_INFO_STREAM("StartStream Stream " << stream_name_[stream_index] << " started.");
  } catch (...) {
    ROS_ERROR_STREAM("StartStream Failed to start stream " << stream_name_[stream_index] << ".");
  }
}

void DtCameraNode::StopStream(const stream_index_pair& stream_index) {
  std::lock_guard<decltype(device_lock_)> lock(device_lock_);
  if (enable_pipeline_) {
    ROS_WARN_STREAM("Cannot stop stream when pipeline is enabled");
    return;
  }
  if (!stream_started_[stream_index]) {
    ROS_WARN_STREAM("Stream " << stream_name_[stream_index] << " is not started.");
    return;
  }
  ROS_INFO_STREAM("Stopping stream " << stream_name_[stream_index] << "...");
  sensors_[stream_index]->StopStream();
  stream_started_[stream_index] = false;
  ROS_INFO_STREAM("Stream " << stream_name_[stream_index] << " stopped.");
}

void DtCameraNode::PublishPointCloud(const std::shared_ptr<dt::FrameSet>& frame_set) {
  try {
    if (depth_registration_ || enable_colored_point_cloud_) {
      if (frame_set->GetDepthFrame() != nullptr && frame_set->GetColorFrame() != nullptr) {
        ROS_INFO_STREAM("test---- PublishPointCloud to PublishColoredPointCloud");

        PublishColoredPointCloud(frame_set);
      }
    }

    if (enable_point_cloud_ && frame_set->GetDepthFrame() != nullptr) {
      // ROS_INFO_STREAM("test---- PublishPointCloud to PublishDepthPointCloud");
      PublishDepthPointCloud(frame_set);
    }
  } catch (const dt::Error& e) {
    ROS_ERROR_STREAM("PublishPointCloud error: " << e.GetMessage());
  } catch (const std::exception& e) {
    ROS_ERROR_STREAM("PublishPointCloud excepiion: " << e.what());
  } catch (...) {
    ROS_ERROR_STREAM("PublishPointCloud with unknown error");
  }
}

void DtCameraNode::PublishDepthPointCloud(const std::shared_ptr<dt::FrameSet>& frame_set) {
  if (((!depth_cloud_pub_ || depth_cloud_pub_.getNumSubscribers() == 0) &&
       (!depth_cloud_pub_second_ || depth_cloud_pub_second_.getNumSubscribers() == 0)) ||
      !enable_point_cloud_) {
    // ROS_ERROR_STREAM("test---- PublishDepthPointCloud depth_cloud_pub_"
    //                  << depth_cloud_pub_ << " depth_cloud_pub_.getNumSubscribers"
    //                  << depth_cloud_pub_.getNumSubscribers() << " enable_point_cloud_ "
    //                  << enable_point_cloud_);
    return;
  }

  std::lock_guard<decltype(cloud_mutex_)> cloud_lock(cloud_mutex_);
  auto depth_frame = frame_set->GetDepthFrame();
  if (!depth_frame) {
    ROS_ERROR_STREAM("depth frame is null");
    return;
  }
  CHECK_NOTNULL(pipeline_);
  auto camera_params = pipeline_->GetCameraParam();
  float depth_scale = depth_frame->GetValueScale();
  depth_point_cloud_filter_.SetCoordinateDataScaled(depth_scale);
  depth_point_cloud_filter_.SetCreatePointFormat(kDtFormatPoint);

  auto result_frame = depth_point_cloud_filter_.Process(frame_set);
  if (!result_frame) {
    ROS_DEBUG("Failed to create point cloud");
    return;
  }

  uint32_t point_size = 0;

  DtPoint* points = nullptr;
  DtColorPoint* points_stof = nullptr;

  auto width = depth_frame->GetWidth();
  auto height = depth_frame->GetHeight();
  sensor_msgs::PointCloud2Modifier modifier(cloud_msg_);
  modifier.setPointCloud2FieldsByString(1, "xyz");

  if (strcmp(depth_frame->GetName(), "md") == 0) {
    point_size = result_frame->GetDataSize() / sizeof(DtPoint);
    points = reinterpret_cast<DtPoint*>(result_frame->GetData());
  } else {
    point_size = result_frame->GetDataSize() / sizeof(DtColorPoint);
    points_stof = reinterpret_cast<DtColorPoint*>(result_frame->GetData());
  }

  modifier.resize(width * height);
  cloud_msg_.width = depth_frame->GetWidth();
  cloud_msg_.height = depth_frame->GetHeight();

  const static double MIN_DISTANCE = 20.0;
  const static double MAX_DISTANCE = 10000.0;
  const static double min_depth = MIN_DISTANCE / depth_scale;
  const static double max_depth = MAX_DISTANCE / depth_scale;
  size_t valid_count = 0;

  if (strcmp(depth_frame->GetName(), "md") == 0) {
    cloud_msg_.row_step = cloud_msg_.width * cloud_msg_.point_step;
    cloud_msg_.data.resize(cloud_msg_.height * cloud_msg_.row_step);
    sensor_msgs::PointCloud2Iterator<float> iter_x(cloud_msg_, "x");
    sensor_msgs::PointCloud2Iterator<float> iter_y(cloud_msg_, "y");
    sensor_msgs::PointCloud2Iterator<float> iter_z(cloud_msg_, "z");

    for (size_t i = 0; i < point_size; i++) {
      bool valid_point = points[i].z >= min_depth && points[i].z <= max_depth;
      if (valid_point || ordered_pc_) {
        *iter_x = static_cast<float>(points[i].x / 1000.0);
        *iter_y = static_cast<float>(points[i].y / 1000.0);
        *iter_z = static_cast<float>(points[i].z / 1000.0);
        ++iter_x, ++iter_y, ++iter_z;
        valid_count++;
      }
    }
  } else {
    std::string format_str = "rgb";
    cloud_msg_.point_step =
        addPointField(cloud_msg_, format_str, 1, sensor_msgs::PointField::FLOAT32,
                      static_cast<int>(cloud_msg_.point_step));
    cloud_msg_.row_step = cloud_msg_.width * cloud_msg_.point_step;
    cloud_msg_.data.resize(cloud_msg_.height * cloud_msg_.row_step);
    sensor_msgs::PointCloud2Iterator<float> iter_x(cloud_msg_, "x");
    sensor_msgs::PointCloud2Iterator<float> iter_y(cloud_msg_, "y");
    sensor_msgs::PointCloud2Iterator<float> iter_z(cloud_msg_, "z");
    sensor_msgs::PointCloud2Iterator<uint8_t> iter_r(cloud_msg_, "b");
    sensor_msgs::PointCloud2Iterator<uint8_t> iter_g(cloud_msg_, "g");
    sensor_msgs::PointCloud2Iterator<uint8_t> iter_b(cloud_msg_, "r");

    for (size_t i = 0; i < point_size; i++) {
      bool valid_point = points_stof[i].z >= min_depth && points_stof[i].z <= max_depth;
      if (valid_point || ordered_pc_) {
        *iter_x = static_cast<float>(points_stof[i].x / 1000.0);
        *iter_y = static_cast<float>(points_stof[i].y / 1000.0);
        *iter_z = static_cast<float>(points_stof[i].z / 1000.0);
        *iter_r = static_cast<uint8_t>(points_stof[i].r);
        *iter_g = static_cast<uint8_t>(points_stof[i].g);
        *iter_b = static_cast<uint8_t>(points_stof[i].b);
        ++iter_x, ++iter_y, ++iter_z, ++iter_r, ++iter_g, ++iter_b;
        ++valid_count;
      }
    }
  }
  if (!ordered_pc_) {
    cloud_msg_.is_dense = true;
    cloud_msg_.width = valid_count;
    cloud_msg_.height = 1;
    modifier.resize(valid_count);
  }
  auto frame_timestamp = GetFrameTimestampUs(depth_frame);
  // ROS_INFO_STREAM("test---- frame_timestamp: " << frame_timestamp);
  auto timestamp = FromUsToRosTime(frame_timestamp);
  // std::string frame_id = depth_registration_ ? optical_frame_id_[COLOR] :
  // optical_frame_id_[DEPTH];
  std::string frame_id = optical_frame_id_[DEPTH];
  cloud_msg_.header.stamp = timestamp;
  cloud_msg_.header.frame_id = frame_id;

  // if (save_point_cloud_) {
  //   save_point_cloud_ = false;
  //   auto now = std::time(nullptr);
  //   std::stringstream ss;
  //   ss << std::put_time(std::localtime(&now), "%Y%m%d_%H%M%S");
  //   auto current_path = boost::filesystem::current_path().string();
  //   std::string filename = current_path + "/point_cloud/points_" + ss.str() + ".ply";
  //   if (!boost::filesystem::exists(current_path + "/point_cloud")) {
  //     boost::filesystem::create_directory(current_path + "/point_cloud");
  //   }
  //   ROS_INFO_STREAM("Saving point cloud to " << filename);
  //   try {
  //     SaveDepthPointCloudMsgToPly(cloud_msg_, filename);
  //   } catch (const std::exception& e) {
  //     ROS_ERROR_STREAM("Failed to save point cloud: " << e.what());
  //   } catch (...) {
  //     ROS_ERROR_STREAM("Failed to save point cloud");
  //   }
  // }
  auto device_info = device_->GetDeviceInfo();
  CHECK_NOTNULL(device_info.get());
  auto pid = device_info->GetPid();
  if (camera_mode_ == 1 && IsNebulaSeriesPid(pid)) {
    if (strcmp(depth_frame->GetName(), "md") == 0 && depth_cloud_pub_.getNumSubscribers() != 0) {
      depth_cloud_pub_.publish(cloud_msg_);
      ToHandleSavePointCloud(depth_frame, cloud_msg_);
    } else if (strcmp(depth_frame->GetName(), "sd") == 0 &&
               depth_cloud_pub_second_.getNumSubscribers() != 0) {
      ToHandleSavePointCloud(depth_frame, cloud_msg_);
      depth_cloud_pub_second_.publish(cloud_msg_);
    }
  } else {
    ToHandleSavePointCloud(depth_frame, cloud_msg_);
    depth_cloud_pub_.publish(cloud_msg_);
  }
}

void DtCameraNode::ToHandleSavePointCloud(const std::shared_ptr<dt::Frame>& depth_frame,
                                          const sensor_msgs::PointCloud2& point_cloud_msg) {
  if (save_point_cloud_) {
    save_point_cloud_ = false;
    auto now = std::time(nullptr);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&now), "%Y%m%d_%H%M%S");
    auto current_path = boost::filesystem::current_path().string();
    std::string filename =
        current_path + "/point_cloud/points_" + depth_frame->GetName() + "_" + ss.str() + ".ply";
    if (!boost::filesystem::exists(current_path + "/point_cloud")) {
      boost::filesystem::create_directory(current_path + "/point_cloud");
    }
    ROS_INFO_STREAM("Saving point cloud to " << filename);
    try {
      strcmp(depth_frame->GetName(), "md") == 0
          ? SaveDepthPointCloudMsgToPly(point_cloud_msg, filename)
          : SaveRgbPointCloudMsgToPly(point_cloud_msg, filename);
    } catch (const std::exception& e) {
      ROS_ERROR_STREAM("Failed to save point cloud: " << e.what());
    } catch (...) {
      ROS_ERROR_STREAM("Failed to save point cloud");
    }
  }
}

void DtCameraNode::PublishColoredPointCloud(const std::shared_ptr<dt::FrameSet>& frame_set) {
  if (!depth_registered_cloud_pub_ || depth_registered_cloud_pub_.getNumSubscribers() == 0 ||
      !enable_colored_point_cloud_) {
    return;
  }
  std::lock_guard<decltype(cloud_mutex_)> cloud_lock(cloud_mutex_);
  auto depth_frame = frame_set->GetDepthFrame();
  auto color_frame = frame_set->GetColorFrame();
  if (!depth_frame || !color_frame) {
    return;
  }
  auto depth_width = depth_frame->GetWidth();
  auto depth_height = depth_frame->GetHeight();
  auto color_width = color_frame->GetWidth();
  auto color_height = color_frame->GetHeight();
  if (depth_width != color_width || depth_height != color_height) {
    ROS_DEBUG("Depth (%d x %d) and color (%d x %d) frame size mismatch", depth_width, depth_height,
              color_width, color_height);
    return;
  }

  CHECK_NOTNULL(pipeline_);
  auto camera_params = pipeline_->GetCameraParam();
  auto device_info = device_->GetDeviceInfo();
  CHECK_NOTNULL(device_info.get());
  auto pid = device_info->GetPid();
  // if (depth_registration_ || pid == XXX_PID) {
  //   camera_params.depth_intrinsic = camera_params.rgb_intrinsic;
  // }

  auto depth_scale = depth_frame->GetValueScale();
  color_point_cloud_filter_.SetCoordinateDataScaled(depth_scale);
  color_point_cloud_filter_.SetCreatePointFormat(kDtFormatRgbPoint);
  auto result_frame = color_point_cloud_filter_.Process(frame_set);
  if (!result_frame) {
    ROS_ERROR_STREAM("Failed to process depth frame");
    return;
  }
  auto point_size = result_frame->GetDataSize() / sizeof(DtColorPoint);
  auto* point_cloud = reinterpret_cast<DtColorPoint*>(result_frame->GetData());

  sensor_msgs::PointCloud2Modifier modifier(cloud_msg_);
  modifier.setPointCloud2FieldsByString(1, "xyz");
  cloud_msg_.width = color_frame->GetWidth();
  cloud_msg_.height = color_frame->GetHeight();
  std::string format_str = "rgb";
  cloud_msg_.point_step = addPointField(cloud_msg_, format_str, 1, sensor_msgs::PointField::FLOAT32,
                                        static_cast<int>(cloud_msg_.point_step));
  cloud_msg_.row_step = cloud_msg_.width * cloud_msg_.point_step;
  cloud_msg_.data.resize(cloud_msg_.height * cloud_msg_.row_step);
  sensor_msgs::PointCloud2Iterator<float> iter_x(cloud_msg_, "x");
  sensor_msgs::PointCloud2Iterator<float> iter_y(cloud_msg_, "y");
  sensor_msgs::PointCloud2Iterator<float> iter_z(cloud_msg_, "z");
  sensor_msgs::PointCloud2Iterator<uint8_t> iter_r(cloud_msg_, "b");
  sensor_msgs::PointCloud2Iterator<uint8_t> iter_g(cloud_msg_, "g");
  sensor_msgs::PointCloud2Iterator<uint8_t> iter_b(cloud_msg_, "r");
  size_t valid_count = 0;
  static const float MIN_DISTANCE = 20.0;
  static const float MAX_DISTANCE = 10000.0;
  static float min_depth = MIN_DISTANCE / depth_scale;
  static float max_depth = MAX_DISTANCE / depth_scale;
  for (size_t i = 0; i < point_size; i++) {
    bool valid_point = point_cloud[i].z >= min_depth && point_cloud[i].z <= max_depth;
    if (valid_point || ordered_pc_) {
      *iter_x = static_cast<float>(point_cloud[i].x / 1000.0);
      *iter_y = static_cast<float>(point_cloud[i].y / 1000.0);
      *iter_z = static_cast<float>(point_cloud[i].z / 1000.0);
      *iter_r = static_cast<uint8_t>(point_cloud[i].r);
      *iter_g = static_cast<uint8_t>(point_cloud[i].g);
      *iter_b = static_cast<uint8_t>(point_cloud[i].b);
      ++iter_x, ++iter_y, ++iter_z, ++iter_r, ++iter_g, ++iter_b;
      ++valid_count;
    }
  }
  if (!ordered_pc_) {
    cloud_msg_.is_dense = true;
    cloud_msg_.width = valid_count;
    cloud_msg_.height = 1;
    modifier.resize(valid_count);
  }
  auto frame_timestamp = GetFrameTimestampUs(depth_frame);
  auto timestamp = FromUsToRosTime(frame_timestamp);
  cloud_msg_.header.stamp = timestamp;
  cloud_msg_.header.frame_id = optical_frame_id_[COLOR];
  depth_registered_cloud_pub_.publish(cloud_msg_);
  if (save_colored_point_cloud_) {
    save_colored_point_cloud_ = false;
    auto now = std::time(nullptr);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&now), "%Y%m%d_%H%M%S");
    auto current_path = boost::filesystem::current_path().string();
    std::string filename = current_path + "/point_cloud/colored_points_" + ss.str() + ".ply";
    if (!boost::filesystem::exists(current_path + "/point_cloud")) {
      boost::filesystem::create_directory(current_path + "/point_cloud");
    }
    ROS_INFO_STREAM("Saving point cloud to " << filename);
    try {
      SaveRgbPointCloudMsgToPly(cloud_msg_, filename);
    } catch (const std::exception& e) {
      ROS_ERROR_STREAM("Failed to save colored point cloud: " << e.what());
    } catch (...) {
      ROS_ERROR_STREAM("Failed to save colored point cloud with unknown error");
    }
  }
}

ImuInfo DtCameraNode::CreateImuInfo(const stream_index_pair& stream_index) {
  ImuInfo imu_info;
  imu_info.header.frame_id = optical_frame_id_[stream_index];
  imu_info.header.stamp = ros::Time::now();
  auto imu_profile = stream_profile_[stream_index];
  if (stream_index == GYRO) {
    auto gyro_profile = stream_profile_[stream_index]->As<dt::GyroStreamProfile>();
    auto gyro_intrinsics = gyro_profile->GetIntrinsic();
    imu_info.noise_density = gyro_intrinsics.noise_density;
    imu_info.random_walk = gyro_intrinsics.random_walk;
    imu_info.reference_temperature = gyro_intrinsics.reference_temp;
    imu_info.bias = {gyro_intrinsics.bias[0], gyro_intrinsics.bias[1], gyro_intrinsics.bias[2]};
    imu_info.scale_misalignment = {
        gyro_intrinsics.scale_misalignment[0], gyro_intrinsics.scale_misalignment[1],
        gyro_intrinsics.scale_misalignment[2], gyro_intrinsics.scale_misalignment[3],
        gyro_intrinsics.scale_misalignment[4], gyro_intrinsics.scale_misalignment[5],
        gyro_intrinsics.scale_misalignment[6], gyro_intrinsics.scale_misalignment[7],
        gyro_intrinsics.scale_misalignment[8]};
    imu_info.temperature_slope = {gyro_intrinsics.temp_slope[0], gyro_intrinsics.temp_slope[1],
                                  gyro_intrinsics.temp_slope[2], gyro_intrinsics.temp_slope[3],
                                  gyro_intrinsics.temp_slope[4], gyro_intrinsics.temp_slope[5],
                                  gyro_intrinsics.temp_slope[6], gyro_intrinsics.temp_slope[7],
                                  gyro_intrinsics.temp_slope[8]};
  } else if (stream_index == ACCEL) {
    auto accel_profile = stream_profile_[stream_index]->As<dt::AccelStreamProfile>();
    auto accel_intrinsics = accel_profile->GetIntrinsic();
    imu_info.noise_density = accel_intrinsics.noise_density;
    imu_info.random_walk = accel_intrinsics.random_walk;
    imu_info.reference_temperature = accel_intrinsics.reference_temp;
    imu_info.bias = {accel_intrinsics.bias[0], accel_intrinsics.bias[1], accel_intrinsics.bias[2]};
    imu_info.gravity = {accel_intrinsics.gravity[0], accel_intrinsics.gravity[1],
                        accel_intrinsics.gravity[2]};
    imu_info.scale_misalignment = {
        accel_intrinsics.scale_misalignment[0], accel_intrinsics.scale_misalignment[1],
        accel_intrinsics.scale_misalignment[2], accel_intrinsics.scale_misalignment[3],
        accel_intrinsics.scale_misalignment[4], accel_intrinsics.scale_misalignment[5],
        accel_intrinsics.scale_misalignment[6], accel_intrinsics.scale_misalignment[7],
        accel_intrinsics.scale_misalignment[8]};
    imu_info.temperature_slope = {accel_intrinsics.temp_slope[0], accel_intrinsics.temp_slope[1],
                                  accel_intrinsics.temp_slope[2], accel_intrinsics.temp_slope[3],
                                  accel_intrinsics.temp_slope[4], accel_intrinsics.temp_slope[5],
                                  accel_intrinsics.temp_slope[6], accel_intrinsics.temp_slope[7],
                                  accel_intrinsics.temp_slope[8]};
  }

  return imu_info;
}

void DtCameraNode::SetDefaultImuMessage(sensor_msgs::Imu& imu_msg) {
  imu_msg.header.frame_id = "imu_link";
  imu_msg.orientation.x = 0.0;
  imu_msg.orientation.y = 0.0;
  imu_msg.orientation.z = 0.0;
  imu_msg.orientation.w = 1.0;

  imu_msg.orientation_covariance = {-1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
  imu_msg.linear_acceleration_covariance = {
      liner_accel_cov_, 0.0, 0.0, 0.0, liner_accel_cov_, 0.0, 0.0, 0.0, liner_accel_cov_};
  imu_msg.angular_velocity_covariance = {
      angular_vel_cov_, 0.0, 0.0, 0.0, angular_vel_cov_, 0.0, 0.0, 0.0, angular_vel_cov_};
}

sensor_msgs::Imu DtCameraNode::CreateUnitImuMessage(const ImuData& accel_data,
                                                    const ImuData& gyro_data) {
  sensor_msgs::Imu imu_msg;
  ros::Time timestamp = ros::Time::now();
  imu_msg.header.stamp = timestamp;
  imu_msg.angular_velocity.x = gyro_data.data_.x();
  imu_msg.angular_velocity.y = gyro_data.data_.y();
  imu_msg.angular_velocity.z = gyro_data.data_.z();

  imu_msg.linear_acceleration.x = accel_data.data_.x();
  imu_msg.linear_acceleration.y = accel_data.data_.y();
  imu_msg.linear_acceleration.z = accel_data.data_.z();
  return imu_msg;
}

void DtCameraNode::OnNewImuFrameSyncOutputCallback(const std::shared_ptr<dt::Frame>& accel_frame,
                                                   const std::shared_ptr<dt::Frame>& gyro_frame) {
  if (!IsInitialized()) {
    ROS_WARN_ONCE("IMU sync output callback called before initialization");
    return;
  }
  if (!imu_gyro_accel_publisher_) {
    ROS_ERROR_STREAM("stream Accel Gyro publisher not initialized");
    return;
  }
  ROS_INFO_STREAM_ONCE("IMU sync output callback called");
  auto has_subscriber = imu_gyro_accel_publisher_.getNumSubscribers() > 0;
  has_subscriber = has_subscriber || imu_info_publishers_[ACCEL].getNumSubscribers() > 0;
  has_subscriber = has_subscriber || imu_info_publishers_[GYRO].getNumSubscribers() > 0;
  if (!has_subscriber) {
    return;
  }

  std::this_thread::sleep_for(std::chrono::nanoseconds(1));
  auto imu_msg = sensor_msgs::Imu();
  SetDefaultImuMessage(imu_msg);

  imu_msg.header.frame_id = optical_frame_id_[GYRO];
  auto frame_timestamp = GetFrameTimestampUs(accel_frame);
  auto timestamp = FromUsToRosTime(frame_timestamp);
  imu_msg.header.stamp = timestamp;

  auto gyro_info = CreateImuInfo(GYRO);
  gyro_info.header = imu_msg.header;
  imu_info_publishers_[GYRO].publish(gyro_info);

  auto accel_info = CreateImuInfo(ACCEL);
  imu_msg.header.frame_id = optical_frame_id_[ACCEL];
  accel_info.header = imu_msg.header;
  imu_info_publishers_[ACCEL].publish(accel_info);

  imu_msg.header.frame_id = accel_gyro_frame_id_;

  auto gyro_cast_frame = gyro_frame->As<dt::GyroFrame>();
  auto gyroData = gyro_cast_frame->GetValue();
  imu_msg.angular_velocity.x = gyroData.x - gyro_info.bias[0];
  imu_msg.angular_velocity.y = gyroData.y - gyro_info.bias[1];
  imu_msg.angular_velocity.z = gyroData.z - gyro_info.bias[2];

  auto accel_cast_frame = accel_frame->As<dt::AccelFrame>();
  auto accelData = accel_cast_frame->GetValue();
  imu_msg.linear_acceleration.x = accelData.x - accel_info.bias[0];
  imu_msg.linear_acceleration.y = accelData.y - accel_info.bias[1];
  imu_msg.linear_acceleration.z = accelData.z - accel_info.bias[2];

  imu_gyro_accel_publisher_.publish(imu_msg);
}

void DtCameraNode::OnNewImuFrameCallback(const std::shared_ptr<dt::Frame>& frame,
                                         const stream_index_pair& stream_index) {
  if (!IsInitialized()) {
    ROS_WARN_ONCE("IMU callback called before initialization");
    return;
  }
  if (!imu_publishers_.count(stream_index)) {
    ROS_ERROR_STREAM("stream " << stream_name_[stream_index] << " publisher not initialized");
    return;
  }
  auto has_subscriber = imu_publishers_[stream_index].getNumSubscribers() > 0;
  has_subscriber = has_subscriber || imu_info_publishers_[stream_index].getNumSubscribers() > 0;
  if (!has_subscriber) {
    return;
  }
  std::this_thread::sleep_for(std::chrono::nanoseconds(1));

  auto imu_msg = sensor_msgs::Imu();
  SetDefaultImuMessage(imu_msg);
  imu_msg.header.frame_id = optical_frame_id_[stream_index];
  auto frame_timestamp = GetFrameTimestampUs(frame);
  auto timestamp = FromUsToRosTime(frame_timestamp);
  imu_msg.header.stamp = timestamp;

  auto imu_info = CreateImuInfo(stream_index);
  imu_info.header = imu_msg.header;
  imu_info_publishers_[stream_index].publish(imu_info);

  if (frame->GetType() == kDtFrameGyro) {
    auto gyro_frame = frame->As<dt::GyroFrame>();
    auto data = gyro_frame->GetValue();
    imu_msg.angular_velocity.x = data.x - imu_info.bias[0];
    imu_msg.angular_velocity.y = data.y - imu_info.bias[1];
    imu_msg.angular_velocity.z = data.z - imu_info.bias[2];
  } else if (frame->GetType() == kDtFrameAccel) {
    auto accel_frame = frame->As<dt::AccelFrame>();
    auto data = accel_frame->GetValue();
    imu_msg.linear_acceleration.x = data.x - imu_info.bias[0];
    imu_msg.linear_acceleration.y = data.y - imu_info.bias[1];
    imu_msg.linear_acceleration.z = data.z - imu_info.bias[2];
  } else {
    ROS_ERROR("Unsupported IMU frame type");
    return;
  }
  imu_publishers_[stream_index].publish(imu_msg);
}

bool DtCameraNode::DecodeColorFrameToBuffer(const std::shared_ptr<dt::Frame>& frame,
                                            uint8_t* dest) {
  if (!rgb_buffer_) {
    return false;
  }
  bool has_subscriber = image_publishers_[COLOR].getNumSubscribers() > 0;
  if (enable_colored_point_cloud_ && depth_registered_cloud_pub_.getNumSubscribers() > 0) {
    has_subscriber = true;
  }
  if (metadata_publishers_.count(COLOR) && metadata_publishers_[COLOR].getNumSubscribers() > 0) {
    has_subscriber = true;
  }
  if (camera_info_publishers_.count(COLOR) &&
      camera_info_publishers_[COLOR].getNumSubscribers() > 0) {
    has_subscriber = true;
  }
  if (!has_subscriber) {
    return false;
  }
  bool is_decoded = false;
  if (!frame) {
    return false;
  }
#if defined(USE_RK_HW_DECODER) || defined(USE_NV_HW_DECODER)
  if (frame && frame->GetFormat() != kDtFormatRgb) {
    if (frame->GetFormat() == kDtFormatMjpg && mjpeg_decoder_) {
      CHECK_NOTNULL(mjpeg_decoder_.get());
      CHECK_NOTNULL(rgb_buffer_);
      auto video_frame = frame->As<dt::ColorFrame>();
      bool ret = mjpeg_decoder_->Decode(video_frame, rgb_buffer_);
      if (!ret) {
        ROS_ERROR_STREAM("Decode frame failed");
        is_decoded = false;

      } else {
        is_decoded = true;
      }
    }
  }
#endif
  if (!is_decoded) {
    auto video_frame = SoftwareDecodeColorFrame(frame);
    if (!video_frame) {
      ROS_ERROR_STREAM("Decode frame failed");
      return false;
    }
    CHECK_NOTNULL(rgb_buffer_);
    CHECK_NOTNULL(dest);
    memcpy(rgb_buffer_, video_frame->GetData(), video_frame->GetDataSize());

    return true;
  }
  return true;
}

std::shared_ptr<dt::Frame> DtCameraNode::DecodeIrMjpgFrame(
    const std::shared_ptr<dt::Frame>& frame) {
  if (frame->GetFormat() == kDtFormatMjpeg &&
      (frame->GetType() == kDtFrameIr || frame->GetType() == kDtFrameIrLeft ||
       frame->GetType() == kDtFrameIrRight)) {
    auto video_frame = frame->As<dt::IrFrame>();

    cv::Mat mjpgMat(1, video_frame->GetDataSize(), CV_8UC1, video_frame->GetData());
    cv::Mat irRawMat = cv::imdecode(mjpgMat, cv::IMREAD_GRAYSCALE);

    std::shared_ptr<dt::Frame> irFrame = dt::FrameHelper::CreateFrame(
        video_frame->GetType(), video_frame->GetFormat(), video_frame->GetDataSize());

    uint32_t buffer_size = irRawMat.rows * irRawMat.cols * irRawMat.channels();

    if (buffer_size > irFrame->GetDataSize()) {
      ROS_ERROR_STREAM("Insufficient buffer size allocation,failed to decode ir mjpg frame!");
      return nullptr;
    }

    memcpy(irFrame->GetData(), irRawMat.data, buffer_size);
    // dt::FrameHelper::setFrameDeviceTimestamp(irFrame, video_frame->timeStamp());
    dt::FrameHelper::SetFrameDeviceTimestampUs(irFrame, video_frame->GetTimeStampUs());
    // dt::FrameHelper::setFrameSystemTimestamp(irFrame, video_frame->systemTimeStamp());
    return irFrame;
  }

  return nullptr;
}

std::shared_ptr<dt::Frame> DtCameraNode::ProcessRightIrFrameFilter(
    std::shared_ptr<dt::Frame>& frame) {
  if (frame == nullptr || frame->GetType() != kDtFrameIrRight) {
    return nullptr;
  }
  for (size_t i = 0; i < right_ir_filter_list_.size(); i++) {
    auto filter = right_ir_filter_list_[i];
    CHECK_NOTNULL(filter.get());
    if (filter->IsEnabled() && frame != nullptr) {
      frame = filter->Process(frame);
      if (frame == nullptr) {
        ROS_ERROR_STREAM("Right Ir filter process failed");
        break;
      }
    }
  }
  return frame;
}
std::shared_ptr<dt::Frame> DtCameraNode::ProcessLeftIrFrameFilter(
    std::shared_ptr<dt::Frame>& frame) {
  if (frame == nullptr || frame->GetType() != kDtFrameIrLeft) {
    return nullptr;
  }
  for (size_t i = 0; i < left_ir_filter_list_.size(); i++) {
    auto filter = left_ir_filter_list_[i];
    CHECK_NOTNULL(filter.get());
    if (filter->IsEnabled() && frame != nullptr) {
      frame = filter->Process(frame);
      if (frame == nullptr) {
        ROS_ERROR_STREAM("Left Ir filter process failed");
        break;
      }
    }
  }
  return frame;
}

std::shared_ptr<dt::Frame> DtCameraNode::ProcessColorFrameFilter(
    std::shared_ptr<dt::Frame>& frame) {
  if (frame == nullptr || frame->GetType() != kDtFrameColor) {
    return nullptr;
  }
  for (size_t i = 0; i < color_filter_list_.size(); i++) {
    auto filter = color_filter_list_[i];
    CHECK_NOTNULL(filter.get());
    if (filter->IsEnabled() && frame != nullptr) {
      frame = filter->Process(frame);
      if (frame == nullptr) {
        ROS_ERROR_STREAM("Color filter process failed");
        break;
      }
    }
  }
  return frame;
}
std::shared_ptr<dt::Frame> DtCameraNode::ProcessDepthFrameFilter(
    std::shared_ptr<dt::Frame>& frame) {
  if (frame == nullptr || frame->GetType() != kDtFrameDepth) {
    return nullptr;
  }
  for (size_t i = 0; i < depth_filter_list_.size(); i++) {
    auto filter = depth_filter_list_[i];
    CHECK_NOTNULL(filter.get());
    if (filter->IsEnabled() && frame != nullptr) {
      frame = filter->Process(frame);
      if (frame == nullptr) {
        ROS_ERROR_STREAM("Depth filter process failed");
        break;
      }
    }
  }
  return frame;
}

uint64_t DtCameraNode::GetFrameTimestampUs(const std::shared_ptr<dt::Frame>& frame) {
  if (frame == nullptr) {
    ROS_WARN_STREAM("GetFrameTimestampUs: frame is nullptr, return 0");
    return 0;
  }
  // ROS_WARN_STREAM("test ---- time_domain_: " << time_domain_);
  if (time_domain_ == "device") {
    return frame->GetTimeStampUs();
  } else if (time_domain_ == "global") {
    return frame->GetGlobalTimeStampUs();
  } else {
    return frame->GetSystemTimeStampUs();
  }
}

void DtCameraNode::OnNewFrameSetCallback(std::shared_ptr<dt::FrameSet> frame_set) {
  if (!is_running_) {
    ROS_WARN_ONCE("OnNewFrameSetCallback called before initialization");
    ROS_INFO_STREAM("test---- OnNewFrameSetCallback called before initialization");
    return;
  }
  if (!IsInitialized()) {
    ROS_WARN_ONCE("OnNewFrameSetCallback called before initialization");
    ROS_INFO_STREAM("test---- OnNewFrameSetCallback called before initialization");
    return;
  }
  if (frame_set == nullptr) {
    ROS_INFO_STREAM("test---- OnNewFrameSetCallback frame_set is null");
    return;
  }
  ROS_INFO_STREAM_ONCE("Received first frame set");
  try {
    // std::shared_ptr<dt::ColorFrame> color_frame = frame_set->GetColorFrame();
    auto depth_frame = frame_set->GetFrame(kDtFrameDepth);
    auto color_frame = frame_set->GetFrame(kDtFrameColor);
    auto left_ir_frame = frame_set->GetFrame(kDtFrameIrLeft);
    auto right_ir_frame = frame_set->GetFrame(kDtFrameIrRight);
    if (depth_frame) {
      SetDepthAutoExposureRoi();
      depth_frame = ProcessDepthFrameFilter(depth_frame);
      frame_set->PushFrame(depth_frame);
    }
    if (color_frame) {
      SetColorAutoExposureRoi();
      color_frame = ProcessColorFrameFilter(color_frame);
      frame_set->PushFrame(color_frame);
    }
    if (left_ir_frame) {
      left_ir_frame = ProcessLeftIrFrameFilter(left_ir_frame);
      frame_set->PushFrame(left_ir_frame);
    }
    if (right_ir_frame) {
      right_ir_frame = ProcessRightIrFrameFilter(right_ir_frame);
      frame_set->PushFrame(right_ir_frame);
    }

    if (depth_registration_ && align_filter_ && depth_frame) {
      if (auto new_frame = align_filter_->Process(frame_set)) {
        auto new_frame_set = new_frame->As<dt::FrameSet>();
        CHECK_NOTNULL(new_frame_set.get());
        frame_set = new_frame_set;
      } else {
        auto device_info = device_->GetDeviceInfo();
        CHECK_NOTNULL(device_info);
        auto pid = device_info->GetPid();
        if (!IsNebulaSeriesPid(pid)) {
          ROS_ERROR_STREAM(
              "OnNewFrameSetCallback Depth frame align depth frame to colot failed and pid :"
              << pid << " is not IsNebulaSeriesPid so return");
          return;
        }
      }
    }
    if (enable_stream_[COLOR] && color_frame) {
      std::unique_lock<std::mutex> colorLock(colorFrameMtx_);
      colorFrameQueue_.push(frame_set);
      colorFrameCV_.notify_all();
    } else {
      PublishPointCloud(frame_set);
    }

    for (const auto& stream_index : IMAGE_STREAMS) {
      if (enable_stream_[stream_index]) {
        auto frame_type = STREAM_TYPE_TO_FRAME_TYPE.at(stream_index.first);
        if (frame_type == kDtFrameColor) {
          continue;
        }

        auto frame = frame_set->GetFrame(frame_type);
        if (frame == nullptr) {
          ROS_DEBUG_STREAM("frame type " << frame_type << " is null");
          continue;
        }
        if (frame_type == kDtFrameDepth) {
          frame = frame;
        }

        std::shared_ptr<dt::Frame> irFrame = DecodeIrMjpgFrame(frame);
        if (irFrame) {
          OnNewFrameCallback(irFrame, stream_index);
        } else {
          OnNewFrameCallback(frame, stream_index);
        }
      }
    }
  } catch (const dt::Error& e) {
    ROS_ERROR_STREAM("OnNewFrameSetCallback error: " << e.GetMessage());
  } catch (const std::exception& e) {
    ROS_ERROR_STREAM("OnNewFrameSetCallback error: " << e.what());
  } catch (...) {
    ROS_ERROR_STREAM("OnNewFrameSetCallback error: unknown error");
  }
}

void DtCameraNode::OnNewColorFrameCallback() {
  while (enable_stream_[COLOR] && ros::ok() && is_running_.load()) {
    std::unique_lock<std::mutex> lock(colorFrameMtx_);
    colorFrameCV_.wait(lock,
                       [this]() { return !colorFrameQueue_.empty() || !(is_running_.load()); });

    if (!ros::ok() || !is_running_.load()) {
      break;
    }
    if (colorFrameQueue_.empty()) {
      continue;
    }
    std::shared_ptr<dt::FrameSet> frameSet = colorFrameQueue_.front();
    colorFrameQueue_.pop();
    rgb_is_decoded_ = DecodeColorFrameToBuffer(frameSet->GetColorFrame(), rgb_buffer_);
    PublishPointCloud(frameSet);
    OnNewFrameCallback(frameSet->GetColorFrame(), IMAGE_STREAMS.at(2));
  }

  ROS_INFO_STREAM("Color frame thread exit!");
}

std::shared_ptr<dt::Frame> DtCameraNode::SoftwareDecodeColorFrame(
    const std::shared_ptr<dt::Frame>& frame) {
  if (frame->GetFormat() == kDtFormatRgb || frame->GetFormat() == kDtFormatBgr) {
    return frame;
  }
  if (frame->GetFormat() == kDtFormatY16 || frame->GetFormat() == kDtFormatY8) {
    return frame;
  }
  if (frame->GetFormat() == kDtFormatRgba || frame->GetFormat() == kDtFormatBgra) {
    return frame;
  }
  if (!SetupFormatConvertType(frame->GetFormat())) {
    ROS_ERROR_STREAM("Unsupported color format: " << (int)frame->GetFormat());
    return nullptr;
  }
  auto covert_frame = format_convert_filter_.Process(frame);
  if (covert_frame == nullptr) {
    ROS_ERROR_STREAM("Format " << (int)frame->GetFormat() << " convert to RGB888 failed");
    return nullptr;
  }
  return covert_frame;
}

void DtCameraNode::OnNewFrameCallback(std::shared_ptr<dt::Frame> frame,
                                      const stream_index_pair& stream_index) {
  if (frame == nullptr) {
    ROS_ERROR_STREAM("test---- OnNewFrameCallback frame is null");
    return;
  }
  bool has_subscriber = image_publishers_[stream_index].getNumSubscribers() > 0;
  if (camera_info_publishers_[stream_index].getNumSubscribers() > 0) {
    has_subscriber = true;
  }
  if (metadata_publishers_.count(stream_index) &&
      metadata_publishers_[stream_index].getNumSubscribers() > 0) {
    has_subscriber = true;
  }
  if (!has_subscriber) {
    // ROS_ERROR_STREAM("test---- OnNewFrameCallback has_subscriber false");
    return;
  }
  std::shared_ptr<dt::VideoFrame> video_frame;
  if (frame->GetType() == kDtFrameColor) {
    video_frame = frame->As<dt::ColorFrame>();
  } else if (frame->GetType() == kDtFrameDepth) {
    video_frame = frame->As<dt::DepthFrame>();
  } else if (frame->GetType() == kDtFrameIr || frame->GetType() == kDtFrameIrLeft ||
             frame->GetType() == kDtFrameIrRight) {
    video_frame = frame->As<dt::IrFrame>();
  } else {
    ROS_ERROR_STREAM("OnNewFrameCallback Unsupported frame type: " << frame->GetType());
    return;
  }
  if (!video_frame) {
    ROS_ERROR_STREAM("OnNewFrameCallback Failed to convert frame to video frame");
    return;
  }
  int width = static_cast<int>(video_frame->GetWidth());
  int height = static_cast<int>(video_frame->GetHeight());
  auto frame_timestamp = GetFrameTimestampUs(frame);
  auto timestamp = FromUsToRosTime(frame_timestamp);
  std::string frame_id = (depth_registration_ && stream_index == DEPTH)
                             ? depth_aligned_frame_id_[stream_index]
                             : optical_frame_id_[stream_index];
  if (color_camera_info_manager_ && color_camera_info_manager_->isCalibrated() &&
      stream_index == COLOR) {
    auto camera_info_publisher = camera_info_publishers_[stream_index];
    auto camera_info = color_camera_info_manager_->getCameraInfo();
    camera_info.header.stamp = timestamp;
    camera_info.header.frame_id = frame_id;
    camera_info_publisher.publish(camera_info);
    PublishMetadata(frame, stream_index, camera_info.header);
  } else if (ir_camera_info_manager_ && ir_camera_info_manager_->isCalibrated() &&
             (stream_index == INFRA0 || stream_index == DEPTH)) {
    auto camera_info_publisher = camera_info_publishers_[stream_index];
    auto camera_info = ir_camera_info_manager_->getCameraInfo();
    camera_info.header.stamp = timestamp;
    camera_info.header.frame_id = frame_id;
    camera_info_publisher.publish(camera_info);
    PublishMetadata(frame, stream_index, camera_info.header);
  } else {
    DtCameraIntrinsic intrinsic;
    DtCameraDistortion distortion;
    CHECK_NOTNULL(device_info_.get());
    // if (isxxxPID(device_info_->GetPid())) {
    //   auto stream_profile = frame->GetStreamProfile();
    //   CHECK_NOTNULL(stream_profile.get());
    //   auto video_stream_profile = stream_profile->As<dt::VideoStreamProfile>();
    //   CHECK_NOTNULL(video_stream_profile);
    //   intrinsic = video_stream_profile->GetIntrinsic();
    //   distortion = video_stream_profile->getDistortion();
    // } else {
    //   auto camera_params = pipeline_->GetCameraParam();
    //   intrinsic = stream_index == COLOR ? camera_params.rgb_intrinsic :
    //   camera_params.depth_intrinsic; distortion =
    //       stream_index == COLOR ? camera_params.rgb_distortion : camera_params.depth_distortion;
    //   if (device_info_->GetPid() == xxx_PID) {
    //     // use color extrinsic
    //     intrinsic = camera_params.rgb_intrinsic;
    //     distortion = camera_params.rgb_distortion;
    //   }
    // }

    auto camera_params = pipeline_->GetCameraParam();
    intrinsic = stream_index == COLOR ? camera_params.rgb_intrinsic : camera_params.depth_intrinsic;
    distortion =
        stream_index == COLOR ? camera_params.rgb_distortion : camera_params.depth_distortion;

    auto camera_info = ConvertToCameraInfo(intrinsic, distortion, width);
    CHECK(camera_info_publishers_.count(stream_index) > 0);
    auto camera_info_publisher = camera_info_publishers_[stream_index];
    camera_info.width = width;
    camera_info.height = height;
    camera_info.header.stamp = timestamp;
    camera_info.header.frame_id = frame_id;
    if (frame->GetType() == kDtFrameIrRight && enable_stream_[INFRA1]) {
      auto left_video_profile = stream_profile_[INFRA1]->As<dt::VideoStreamProfile>();
      CHECK_NOTNULL(left_video_profile.get());
      auto stream_profile = frame->GetStreamProfile();
      CHECK_NOTNULL(stream_profile.get());
      auto video_stream_profile = stream_profile->As<dt::VideoStreamProfile>();
      CHECK_NOTNULL(video_stream_profile.get());
      auto ex = video_stream_profile->GetExtrinsicTo(left_video_profile);
      double fx = camera_info.K.at(0);
      double fy = camera_info.K.at(4);
      camera_info.P.at(3) = fx * ex.trans[0] / 1000.0 + 0.0;
      camera_info.P.at(7) = fy * ex.trans[1] / 1000.0 + 0.0;
    }
    camera_info_publisher.publish(camera_info);
    PublishMetadata(frame, stream_index, camera_info.header);
  }

  CHECK(image_publishers_.count(stream_index));
  if (!image_publishers_[stream_index].getNumSubscribers()) {
    return;
  }
  auto& image = images_[stream_index];
  if (image.empty() || image.cols != width || image.rows != height) {
    image.create(height, width, image_format_[stream_index]);
  }
  if (frame->GetType() == kDtFrameColor && frame->GetFormat() != kDtFormatY8 &&
      frame->GetFormat() != kDtFormatY16 && !rgb_is_decoded_ &&
      image_publishers_[COLOR].getNumSubscribers() > 0) {
    ROS_ERROR_STREAM("frame is not decoded");
    return;
  }
  if (frame->GetType() == kDtFrameColor && frame->GetFormat() != kDtFormatY8 &&
      frame->GetFormat() != kDtFormatY16 && frame->GetFormat() != kDtFormatBgra &&
      frame->GetFormat() != kDtFormatRgba && image_publishers_[COLOR].getNumSubscribers() > 0) {
    memcpy(image.data, rgb_buffer_, width * height * 3);
  } else {
    memcpy(image.data, video_frame->GetData(), video_frame->GetDataSize());
  }

  if (stream_index == DEPTH) {
    auto depth_scale = video_frame->As<dt::DepthFrame>()->GetValueScale();
    image = image * depth_scale;
  }
  auto image_publisher = image_publishers_[stream_index];
  auto image_msg =
      cv_bridge::CvImage(std_msgs::Header(), encoding_[stream_index], image).toImageMsg();
  CHECK_NOTNULL(image_msg.get());
  auto& seq = image_seq_[stream_index];
  image_msg->header.stamp = timestamp;
  image_msg->is_bigendian = false;
  image_msg->step = width * unit_step_size_[stream_index];
  image_msg->header.frame_id = frame_id;
  image_msg->header.seq = seq++;

  // ROS_INFO_STREAM("test---- OnNewFrameCallback image_publisher publish image_flip_[stream_index]:"
  //                 << image_flip_[stream_index]);
  if (!image_flip_[stream_index]) {
    image_publisher.publish(image_msg);
  } else {
    cv::Mat flipped_image;
    cv::flip(image, flipped_image, 1);
    auto flipped_image_msg =
        cv_bridge::CvImage(std_msgs::Header(), encoding_[stream_index], flipped_image).toImageMsg();
    CHECK_NOTNULL(flipped_image_msg.get());
    flipped_image_msg->header.stamp = timestamp;
    flipped_image_msg->is_bigendian = false;
    flipped_image_msg->step = width * unit_step_size_[stream_index];
    flipped_image_msg->header.frame_id = frame_id;
    image_publisher.publish(flipped_image_msg);
  }
  SaveImageToFile(stream_index, image, image_msg);
}

void DtCameraNode::PublishMetadata(const std::shared_ptr<dt::Frame>& frame,
                                   const stream_index_pair& stream_index,
                                   const std_msgs::Header& header) {
  if (metadata_publishers_.count(stream_index) == 0) {
    return;
  }
  auto metadata_publisher = metadata_publishers_[stream_index];
  if (metadata_publisher.getNumSubscribers() == 0) {
    return;
  }
  deptrum_camera::Metadata metadata_msg;
  metadata_msg.header = header;
  nlohmann::json json_data;

  for (int i = 0; i < kDtFrameMetadataTypeCount; i++) {
    auto meta_data_type = static_cast<DtFrameMetadataType>(i);
    std::string field_name = MetaDataTypeToString(meta_data_type);
    if (!frame->HasMetadata(meta_data_type)) {
      continue;
    }
    int64_t value = frame->GetMetadataValue(meta_data_type);
    json_data[field_name] = value;
  }
  metadata_msg.json_data = json_data.dump(2);
  metadata_publisher.publish(metadata_msg);
}

void DtCameraNode::SaveImageToFile(const stream_index_pair& stream_index, const cv::Mat& image,
                                   const sensor_msgs::ImagePtr& image_msg) {
  if (save_images_[stream_index]) {
    auto now = time(nullptr);
    std::stringstream ss;
    ss << std::put_time(localtime(&now), "%Y%m%d_%H%M%S");
    auto current_path = boost::filesystem::current_path().string();
    auto fps = fps_[stream_index];
    int index = save_images_count_[stream_index];
    std::string file_suffix = stream_index == COLOR ? ".png" : ".raw";
    std::string filename = current_path + "/image/" + stream_name_[stream_index] + "_" +
                           std::to_string(image_msg->width) + "x" +
                           std::to_string(image_msg->height) + "_" + std::to_string(fps) + "hz_" +
                           ss.str() + "_" + std::to_string(index) + file_suffix;
    if (!boost::filesystem::exists(current_path + "/image")) {
      boost::filesystem::create_directory(current_path + "/image");
    }
    ROS_INFO_STREAM("Saving image to " << filename);
    if (stream_index.first == kDtStreamColor) {
      auto image_to_save =
          cv_bridge::toCvCopy(image_msg, sensor_msgs::image_encodings::BGR8)->image;
      cv::imwrite(filename, image_to_save);
    } else if (stream_index.first == kDtStreamIr || stream_index.first == kDtStreamIrLeft ||
               stream_index.first == kDtStreamIrRight || stream_index.first == kDtStreamDepth) {
      std::ofstream ofs(filename, std::ios::out | std::ios::binary);
      if (!ofs.is_open()) {
        ROS_ERROR_STREAM("Failed to open file: " << filename);
        return;
      }
      if (image.isContinuous()) {
        ofs.write(reinterpret_cast<const char*>(image.data), image.total() * image.elemSize());
      } else {
        int rows = image.rows;
        int cols = image.cols * image.channels();
        for (int r = 0; r < rows; ++r) {
          ofs.write(reinterpret_cast<const char*>(image.ptr<uchar>(r)), cols);
        }
      }
      ofs.close();
    } else {
      ROS_ERROR_STREAM("Unsupported stream type: " << stream_index.first);
    }
    if (++save_images_count_[stream_index] >= max_save_images_count_) {
      save_images_[stream_index] = false;
    }
  }
}

void DtCameraNode::ImageSubscribedCallback(const stream_index_pair& stream_index) {
  ROS_INFO_STREAM("Image stream " << stream_name_[stream_index] << " subscribed");
  std::lock_guard<decltype(device_lock_)> lock(device_lock_);
  if (enable_pipeline_) {
    if (pipeline_started_) {
      ROS_INFO_STREAM("pipe line already started");
      return;
    }
    try {
      ROS_INFO_STREAM("test---- ImageSubscribedCallback StartStreams");
      StartStreams();
    } catch (const dt::Error& e) {
      ROS_ERROR_STREAM("ImageSubscribedCallback Failed to start streams: " << e.GetMessage());
      return;
    } catch (const std::exception& e) {
      ROS_ERROR_STREAM("ImageSubscribedCallback Failed to start streams: " << e.what());
      return;
    }
  } else {
    if (stream_started_[stream_index]) {
      ROS_INFO_STREAM("ImageSubscribedCallback Stream " << stream_name_[stream_index]
                                                        << " is already started.");
      return;
    }
    ROS_INFO_STREAM("test---- ImageSubscribedCallback StartStreams(stream_index)");
    StartStream(stream_index);
  }
}

void DtCameraNode::ImuSubscribedCallback(const deptrum_camera::stream_index_pair& stream_index) {
  ROS_INFO_STREAM("IMU stream " << stream_name_[stream_index] << " subscribed");
  std::lock_guard<decltype(device_lock_)> lock(device_lock_);
  try {
    if (enable_sync_output_accel_gyro_) {
      if (imu_sync_output_start_) {
        ROS_INFO_STREAM("IMU stream accel and gyro are already started.");
        return;
      }
    } else {
      if (imu_started_[stream_index]) {
        ROS_INFO_STREAM("IMU stream " << stream_name_[stream_index] << " is already started.");
        return;
      }
    }

    StartImu(stream_index);
  } catch (const dt::Error& e) {
    ROS_ERROR_STREAM("Failed to start streams: " << e.GetMessage());
    return;
  } catch (const std::exception& e) {
    ROS_ERROR_STREAM("Failed to start streams: " << e.what());
    return;
  }
}

void DtCameraNode::ImageUnsubscribedCallback(const stream_index_pair& stream_index) {
  ROS_INFO_STREAM("Image stream " << stream_name_[stream_index] << " unsubscribed");
  std::lock_guard<decltype(device_lock_)> lock(device_lock_);
  if (enable_pipeline_) {
    if (!pipeline_started_) {
      ROS_INFO_STREAM("ImageUnsubscribedCallback pipe line not start");
      return;
    }
    bool all_stream_no_subscriber = true;
    for (auto& item : image_publishers_) {
      if (item.second.getNumSubscribers() > 0) {
        all_stream_no_subscriber = false;
        break;
      }
    }
    for (auto& item : camera_info_publishers_) {
      if (item.second.getNumSubscribers() > 0) {
        all_stream_no_subscriber = false;
        break;
      }
    }
    if (enable_point_cloud_) {
      if (depth_cloud_pub_.getNumSubscribers() > 0 ||
          depth_cloud_pub_second_.getNumSubscribers() > 0) {
        all_stream_no_subscriber = false;
      }
    }
    if (enable_colored_point_cloud_) {
      if (depth_registered_cloud_pub_.getNumSubscribers() > 0) {
        all_stream_no_subscriber = false;
      }
    }
    //todo::不在无订阅时停流时，可解决下次订阅cpu异常升高的问题。 待sdk解决
    // if (all_stream_no_subscriber) {
    //   StopStreams();
    // }
  } else {
    if (!stream_started_[stream_index]) {
      ROS_INFO_STREAM("Stream " << stream_name_[stream_index] << " is not started.");
      return;
    }
    auto subscriber_count = image_publishers_[stream_index].getNumSubscribers();
    if (subscriber_count == 0) {
      StopStream(stream_index);
    }
  }
}

void DtCameraNode::ImuUnsubscribedCallback(const stream_index_pair& stream_index) {
  if (enable_sync_output_accel_gyro_) {
    ROS_INFO_STREAM("IMU stream accel and gyro unsubscribed");
  } else {
    ROS_INFO_STREAM("IMU stream " << stream_name_[stream_index] << " unsubscribed");
  }
  std::lock_guard<decltype(device_lock_)> lock(device_lock_);
  if (imu_publishers_.count(stream_index) > 0) {
    auto subscriber_count = imu_publishers_[stream_index].getNumSubscribers();
    if (subscriber_count > 0) {
      return;
    }
  }
  if (imu_gyro_accel_publisher_.getNumSubscribers() > 0) {
    return;
  }
  if (imu_info_publishers_.count(stream_index) > 0) {
    auto subscriber_count = imu_info_publishers_[stream_index].getNumSubscribers();
    if (subscriber_count > 0) {
      return;
    }
  }
  StopImu(stream_index);
}

void DtCameraNode::PointCloudSubscribedCallback() {
  ROS_INFO_STREAM("point cloud subscribed");
  ImageSubscribedCallback(DEPTH);
}

void DtCameraNode::PointCloudUnsubscribedCallback() {
  ROS_INFO_STREAM("point cloud unsubscribed");
  if (depth_cloud_pub_.getNumSubscribers() > 0) {
    return;
  }
  ImageUnsubscribedCallback(DEPTH);
}

void DtCameraNode::ColoredPointCloudSubscribedCallback() {
  ROS_INFO_STREAM("rgb point cloud subscribed");
  ImageSubscribedCallback(DEPTH);
  ImageSubscribedCallback(COLOR);
}

void DtCameraNode::ColoredPointCloudUnsubscribedCallback() {
  ROS_INFO_STREAM("point cloud unsubscribed");
  if (depth_registered_cloud_pub_.getNumSubscribers() > 0) {
    return;
  }
  ImageUnsubscribedCallback(DEPTH);
  ImageUnsubscribedCallback(COLOR);
}

boost::optional<DtCameraParam> DtCameraNode::GetCameraParam() {
  auto camera_params = device_->GetCalibrationCameraParamList();
  for (size_t i = 0; i < camera_params->GetCount(); i++) {
    auto param = camera_params->GetCameraParam(i);
    int depth_w = param.depth_intrinsic.width;
    int depth_h = param.depth_intrinsic.height;
    int color_w = param.rgb_intrinsic.width;
    int color_h = param.rgb_intrinsic.height;
    if ((depth_w * height_[DEPTH] == depth_h * width_[DEPTH]) &&
        (color_w * height_[COLOR] == color_h * width_[COLOR])) {
      return param;
    }
  }
  return {};
}

boost::optional<DtCameraParam> DtCameraNode::GetCameraDepthParam() {
  auto camera_params = device_->GetCalibrationCameraParamList();
  for (size_t i = 0; i < camera_params->GetCount(); i++) {
    auto param = camera_params->GetCameraParam(i);
    int depth_w = param.depth_intrinsic.width;
    int depth_h = param.depth_intrinsic.height;
    if (depth_w == width_[DEPTH] && depth_h == height_[DEPTH]) {
      ROS_INFO_STREAM("GetCameraDepthParam w=" << depth_w << ", h=" << depth_h);
      return param;
    }
  }

  for (size_t i = 0; i < camera_params->GetCount(); i++) {
    auto param = camera_params->GetCameraParam(i);
    int depth_w = param.depth_intrinsic.width;
    int depth_h = param.depth_intrinsic.height;
    if (depth_w * height_[DEPTH] == depth_h * width_[DEPTH]) {
      ROS_INFO_STREAM("GetCameraDepthParam w=" << depth_w << ", h=" << depth_h);
      return param;
    }
  }
  return {};
}

boost::optional<DtCameraParam> DtCameraNode::GetCameraColorParam() {
  auto camera_params = device_->GetCalibrationCameraParamList();
  for (size_t i = 0; i < camera_params->GetCount(); i++) {
    auto param = camera_params->GetCameraParam(i);
    int color_w = param.rgb_intrinsic.width;
    int color_h = param.rgb_intrinsic.height;
    if (color_w == width_[COLOR] && color_h == height_[COLOR]) {
      ROS_INFO_STREAM("GetCameraColorParam w=" << color_w << ", h=" << color_h);
      return param;
    }
  }

  for (size_t i = 0; i < camera_params->GetCount(); i++) {
    auto param = camera_params->GetCameraParam(i);
    int color_w = param.rgb_intrinsic.width;
    int color_h = param.rgb_intrinsic.height;
    if (color_w * height_[COLOR] == color_h * width_[COLOR]) {
      ROS_INFO_STREAM("GetCameraColorParam w=" << color_w << ", h=" << color_h);
      return param;
    }
  }
  return {};
}

int DtCameraNode::GetCameraParamIndex() {
  auto camera_params = device_->GetCalibrationCameraParamList();
  for (size_t i = 0; i < camera_params->GetCount(); i++) {
    auto param = camera_params->GetCameraParam(i);
    int depth_w = param.depth_intrinsic.width;
    int depth_h = param.depth_intrinsic.height;
    int color_w = param.rgb_intrinsic.width;
    int color_h = param.rgb_intrinsic.height;
    if ((depth_w * height_[DEPTH] == depth_h * width_[DEPTH]) &&
        (color_w * height_[COLOR] == color_h * width_[COLOR])) {
      return static_cast<int>(i);
    }
  }
  return -1;
}

void DtCameraNode::PublishStaticTransforms(const ros::Time& t, const tf2::Vector3& trans,
                                   const tf2::Quaternion& q, const std::string& from,
                                   const std::string& to) {
  geometry_msgs::TransformStamped msg;
  msg.header.stamp = t;
  msg.header.frame_id = from;
  msg.child_frame_id = to;
  msg.transform.translation.x = trans[2] / 1000.0;
  msg.transform.translation.y = -trans[0] / 1000.0;
  msg.transform.translation.z = -trans[1] / 1000.0;
  msg.transform.rotation.x = q.getX();
  msg.transform.rotation.y = q.getY();
  msg.transform.rotation.z = q.getZ();
  msg.transform.rotation.w = q.getW();
  static_tf_msgs_.push_back(msg);
}

void DtCameraNode::CalcAndPublishStaticTransform() {
  tf2::Quaternion quaternion_optical, zero_rot;
  zero_rot.setRPY(0.0, 0.0, 0.0);
  quaternion_optical.setRPY(-M_PI / 2, 0.0, -M_PI / 2);
  tf2::Vector3 zero_trans(0, 0, 0);
  if (!stream_profile_.count(base_stream_)) {
    ROS_ERROR_STREAM("Base stream is not available");
    return;
  }
  auto base_stream_profile = stream_profile_[base_stream_];
  CHECK_NOTNULL(base_stream_profile.get());
  for (const auto& item : stream_profile_) {
    auto stream_index = item.first;
    auto stream_profile = item.second;
    if (!stream_profile) {
      continue;
    }
    DtExtrinsic ex;
    try {
      ex = stream_profile->GetExtrinsicTo(base_stream_profile);
    } catch (const dt::Error& e) {
      ROS_ERROR_STREAM("Failed to get " << stream_name_[stream_index]
                                        << " extrinsic: " << e.GetMessage());
      ex = DtExtrinsic({{1, 0, 0, 0, 1, 0, 0, 0, 1}, {0, 0, 0}});
    }

    auto Q = RotationMatrixToQuaternion(ex.rot);
    Q = quaternion_optical * Q * quaternion_optical.inverse();
    Q = Q.normalize();
    tf2::Vector3 trans(ex.trans[0], ex.trans[1], ex.trans[2]);

    auto timestamp = ros::Time::now();
    if (stream_index.first != base_stream_.first) {
      if (stream_index.first == kDtStreamIrRight && base_stream_.first == kDtStreamDepth) {
        trans[0] = std::abs(trans[0]);  // because left and right ir calibration is error
      }
      PublishStaticTransforms(timestamp, trans, Q, frame_id_[base_stream_], frame_id_[stream_index]);
    }
    PublishStaticTransforms(timestamp, zero_trans, quaternion_optical, frame_id_[stream_index],
                    optical_frame_id_[stream_index]);
    ROS_INFO_STREAM("Publishing static transform from " << stream_name_[stream_index] << " to "
                                                        << stream_name_[base_stream_]);
    ROS_INFO_STREAM("Translation " << trans[0] << ", " << trans[1] << ", " << trans[2]);
    ROS_INFO_STREAM("Rotation " << Q.getX() << ", " << Q.getY() << ", " << Q.getZ() << ", "
                                << Q.getW());
  }
  auto device_info = device_->GetDeviceInfo();
  CHECK_NOTNULL(device_info);
  // auto pid = device_info->GetPid();
  // if ((pid == xxx || pid == xxxx) && enable_stream_[DEPTH] &&
  //     enable_stream_[COLOR]) {
  //   // calc depth to color
  //   CHECK_NOTNULL(stream_profile_[COLOR]);
  //   auto depth_to_color_extrinsics = base_stream_profile->GetExtrinsicTo(stream_profile_[COLOR]);
  //   auto Q = RotationMatrixToQuaternion(depth_to_color_extrinsics.rot);
  //   Q = quaternion_optical * Q * quaternion_optical.inverse();
  //   Q = Q.normalize();
  //   PublishStaticTransforms(ros::Time::now(), zero_trans, Q, camera_link_frame_id_,
  //                   frame_id_[base_stream_]);
  // } else {
  //   PublishStaticTransforms(ros::Time::now(), zero_trans, zero_rot, camera_link_frame_id_,
  //                   frame_id_[base_stream_]);
  // }
  PublishStaticTransforms(ros::Time::now(), zero_trans, zero_rot, camera_link_frame_id_,
                  frame_id_[base_stream_]);
}

void DtCameraNode::PublishDynamicTransforms() {
  ROS_WARN("Publishing dynamic camera transforms (/tf) at %g Hz", tf_publish_rate_);
  static std::mutex mu;
  std::unique_lock<std::mutex> lock(mu);
  while (ros::ok() && is_running_) {
    tf_cv_.wait_for(lock, std::chrono::milliseconds((int)(1000.0 / tf_publish_rate_)),
                    [this] { return (!(is_running_)); });
    {
      ros::Time t = ros::Time::now();
      for (auto& msg : static_tf_msgs_) {
        msg.header.stamp = t;
      }
      CHECK_NOTNULL(dynamic_tf_broadcaster_.get());
      dynamic_tf_broadcaster_->sendTransform(static_tf_msgs_);
    }
  }
}

void DtCameraNode::PublishStaticTransforms() {
  static_tf_broadcaster_ = std::make_shared<tf2_ros::StaticTransformBroadcaster>();
  dynamic_tf_broadcaster_ = std::make_shared<tf2_ros::TransformBroadcaster>();
  auto base_stream_profile = stream_profile_[base_stream_];
  if (enable_stream_[DEPTH] && enable_stream_[COLOR]) {
    static const char* frame_id = "depth_to_color_extrinsics";
    DtExtrinsic ex;
    try {
      ex = base_stream_profile->GetExtrinsicTo(stream_profile_[COLOR]);
    } catch (const dt::Error& e) {
      ROS_ERROR_STREAM("Failed to get " << frame_id << " extrinsic: " << e.GetMessage());
      ex = DtExtrinsic({{1, 0, 0, 0, 1, 0, 0, 0, 1}, {0, 0, 0}});
    }
    depth_to_other_extrinsics_[COLOR] = ex;
    auto ex_msg = DtExtrinsicsToMsg(ex, frame_id);
    depth_to_other_extrinsics_publishers_[COLOR].publish(ex_msg);
  }
  if (enable_stream_[DEPTH] && enable_stream_[INFRA0]) {
    static const char* frame_id = "depth_to_ir_extrinsics";
    DtExtrinsic ex;
    try {
      ex = base_stream_profile->GetExtrinsicTo(stream_profile_[INFRA0]);
    } catch (const dt::Error& e) {
      ROS_ERROR_STREAM("Failed to get " << frame_id << " extrinsic: " << e.GetMessage());
      ex = DtExtrinsic({{1, 0, 0, 0, 1, 0, 0, 0, 1}, {0, 0, 0}});
    }
    depth_to_other_extrinsics_[INFRA0] = ex;
    auto ex_msg = DtExtrinsicsToMsg(ex, frame_id);
    depth_to_other_extrinsics_publishers_[INFRA0].publish(ex_msg);
  }
  if (enable_stream_[DEPTH] && enable_stream_[INFRA1]) {
    static const char* frame_id = "depth_to_left_ir_extrinsics";
    DtExtrinsic ex;
    try {
      ex = base_stream_profile->GetExtrinsicTo(stream_profile_[INFRA1]);
    } catch (const dt::Error& e) {
      ROS_ERROR_STREAM("Failed to get " << frame_id << " extrinsic: " << e.GetMessage());
      ex = DtExtrinsic({{1, 0, 0, 0, 1, 0, 0, 0, 1}, {0, 0, 0}});
    }
    depth_to_other_extrinsics_[INFRA1] = ex;
    auto ex_msg = DtExtrinsicsToMsg(ex, frame_id);
    depth_to_other_extrinsics_publishers_[INFRA1].publish(ex_msg);
  }
  if (enable_stream_[DEPTH] && enable_stream_[INFRA2]) {
    static const char* frame_id = "depth_to_right_ir_extrinsics";
    DtExtrinsic ex;
    try {
      ex = base_stream_profile->GetExtrinsicTo(stream_profile_[INFRA2]);
    } catch (const dt::Error& e) {
      ROS_ERROR_STREAM("Failed to get " << frame_id << " extrinsic: " << e.GetMessage());
      ex = DtExtrinsic({{1, 0, 0, 0, 1, 0, 0, 0, 1}, {0, 0, 0}});
    }
    depth_to_other_extrinsics_[INFRA2] = ex;
    auto ex_msg = DtExtrinsicsToMsg(ex, frame_id);
    depth_to_other_extrinsics_publishers_[INFRA2].publish(ex_msg);
  }
  if (enable_stream_[DEPTH] && enable_stream_[ACCEL]) {
    static const char* frame_id = "depth_to_accel_extrinsics";
    DtExtrinsic ex;
    try {
      ex = base_stream_profile->GetExtrinsicTo(stream_profile_[ACCEL]);
    } catch (const dt::Error& e) {
      ROS_ERROR_STREAM("Failed to get " << frame_id << " extrinsic: " << e.GetMessage());
      ex = DtExtrinsic({{1, 0, 0, 0, 1, 0, 0, 0, 1}, {0, 0, 0}});
    }
    depth_to_other_extrinsics_[ACCEL] = ex;
    auto ex_msg = DtExtrinsicsToMsg(ex, frame_id);
    depth_to_other_extrinsics_publishers_[ACCEL].publish(ex_msg);
  }
  if (enable_stream_[DEPTH] && enable_stream_[GYRO]) {
    static const char* frame_id = "depth_to_gyro_extrinsics";
    DtExtrinsic ex;
    try {
      ex = base_stream_profile->GetExtrinsicTo(stream_profile_[GYRO]);
    } catch (const dt::Error& e) {
      ROS_ERROR_STREAM("Failed to get " << frame_id << " extrinsic: " << e.GetMessage());
      ex = DtExtrinsic({{1, 0, 0, 0, 1, 0, 0, 0, 1}, {0, 0, 0}});
    }
    depth_to_other_extrinsics_[GYRO] = ex;
    auto ex_msg = DtExtrinsicsToMsg(ex, frame_id);
    depth_to_other_extrinsics_publishers_[GYRO].publish(ex_msg);
  }
  CalcAndPublishStaticTransform();
  if (tf_publish_rate_ > 0) {
    tf_thread_ = std::make_shared<std::thread>([this]() { PublishDynamicTransforms(); });
  } else {
    CHECK_NOTNULL(static_tf_broadcaster_.get());
    static_tf_broadcaster_->sendTransform(static_tf_msgs_);
  }
}

}  // namespace deptrum_camera
