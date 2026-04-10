// Copyright (c) Deptrum Inc. All Rights Reserved.
#include "deptrum_camera/dt_camera_node.h"
#include <algorithm>

namespace deptrum_camera {

void DtCameraNode::SetupCameraCtrlServices() {
  using std_srvs::SetBool;
  for (const auto& stream_index : IMAGE_STREAMS) {
    if (!enable_stream_[stream_index]) {
      ROS_INFO_STREAM("Stream " << stream_name_[stream_index] << " is disabled.");
      continue;
    }
    auto stream_name = stream_name_[stream_index];
    std::string service_name = "/" + camera_name_ + "/" + "get_" + stream_name + "_exposure";
    get_exposure_srv_[stream_index] = nh_.advertiseService<GetInt32Request, GetInt32Response>(
        service_name, [this, stream_index](GetInt32Request& request, GetInt32Response& response) {
          response.success = this->GetExposureCallback(request, response, stream_index);
          return response.success;
        });
    service_name = "/" + camera_name_ + "/" + "set_" + stream_name + "_exposure";
    set_exposure_srv_[stream_index] =
        nh_.advertiseService<SetInt32TwoParamRequest, SetInt32TwoParamResponse>(
            service_name, [this, stream_index](SetInt32TwoParamRequest& request,
                                               SetInt32TwoParamResponse& response) {
              response.success = this->SetExposureCallback(request, response, stream_index);
              return response.success;
            });

    service_name = "/" + camera_name_ + "/" + "set_" + stream_name + "_crop_roi_mtof_enable";
    set_crop_roi_mtof_enable_srv_[stream_index] =
        nh_.advertiseService<std_srvs::SetBoolRequest, std_srvs::SetBoolResponse>(
            service_name, [this, stream_index](std_srvs::SetBoolRequest& request,
                                               std_srvs::SetBoolResponse& response) {
              response.success = this->setCropRoiEnableCallback(request, response, stream_index,
                                                                kDtOptionMtofDepthCropRoiStruct);
              return response.success;
            });
    service_name = "/" + camera_name_ + "/" + "set_" + stream_name + "_crop_roi_stof_enable";
    set_crop_roi_stof_enable_srv_[stream_index] =
        nh_.advertiseService<std_srvs::SetBoolRequest, std_srvs::SetBoolResponse>(
            service_name, [this, stream_index](std_srvs::SetBoolRequest& request,
                                               std_srvs::SetBoolResponse& response) {
              response.success = this->setCropRoiEnableCallback(request, response, stream_index,
                                                                kDtOptionStofDepthCropRoiStruct);
              return response.success;
            });
    // service_name = "/" + camera_name_ + "/" + "get_" + stream_name + "_crop_roi_enable";
    // get_crop_roi_enable_srv_[stream_index] = nh_.advertiseService<GetBoolRequest,
    // GetBoolResponse>(
    //     service_name, [this, stream_index](GetBoolRequest& request, GetBoolResponse& response) {
    //       response.success = this->GetCropRoiEnableCallback(request, response, stream_index);
    //       return response.success;
    //     });
    service_name = "/" + camera_name_ + "/" + "set_" + stream_name + "_crop_roi";
    set_crop_roi_srv_[stream_index] = nh_.advertiseService<SetArraysRequest, SetArraysResponse>(
        service_name, [this, stream_index](SetArraysRequest& request, SetArraysResponse& response) {
          response.success = this->SetCropRoiCallback(request, response, stream_index);
          return response.success;
        });

    service_name = "/" + camera_name_ + "/" + "set_" + stream_name + "_ae_roi";
    set_ae_roi_srv_[stream_index] = nh_.advertiseService<SetArraysRequest, SetArraysResponse>(
        service_name, [this, stream_index](SetArraysRequest& request, SetArraysResponse& response) {
          response.success = this->SetAeRoiCallback(request, response, stream_index);
          return response.success;
        });
    service_name = "/" + camera_name_ + "/" + "reset_" + stream_name + "_exposure";
    reset_exposure_srv_[stream_index] =
        nh_.advertiseService<std_srvs::EmptyRequest, std_srvs::EmptyResponse>(
            service_name, [this, stream_index](std_srvs::EmptyRequest& request,
                                               std_srvs::EmptyResponse& response) {
              return this->ResetCameraExposureCallback(request, response, stream_index);
            });
    service_name = "/" + camera_name_ + "/" + "get_" + stream_name + "_gain";
    get_gain_srv_[stream_index] = nh_.advertiseService<GetInt32Request, GetInt32Response>(
        service_name, [this, stream_index](GetInt32Request& request, GetInt32Response& response) {
          response.success = this->GetGainCallback(request, response, stream_index);
          return response.success;
        });
    service_name = "/" + camera_name_ + "/" + "set_" + stream_name + "_gain";
    set_gain_srv_[stream_index] = nh_.advertiseService<SetInt32Request, SetInt32Response>(
        service_name, [this, stream_index](SetInt32Request& request, SetInt32Response& response) {
          response.success = this->SetGainCallback(request, response, stream_index);
          return response.success;
        });
    service_name = "/" + camera_name_ + "/" + "reset_" + stream_name + "_gain";
    reset_gain_srv_[stream_index] =
        nh_.advertiseService<std_srvs::EmptyRequest, std_srvs::EmptyResponse>(
            service_name, [this, stream_index](std_srvs::EmptyRequest& request,
                                               std_srvs::EmptyResponse& response) {
              return this->ResetCameraGainCallback(request, response, stream_index);
            });
    service_name = "/" + camera_name_ + "/" + "set_" + stream_name + "_mirror";
    set_mirror_srv_[stream_index] =
        nh_.advertiseService<std_srvs::SetBoolRequest, std_srvs::SetBoolResponse>(
            service_name, [this, stream_index](std_srvs::SetBoolRequest& request,
                                               std_srvs::SetBoolResponse& response) {
              response.success = this->SetMirrorCallback(request, response, stream_index);
              return response.success;
            });
    service_name = "/" + camera_name_ + "/" + "set_" + stream_name + "_flip";
    set_flip_srv_[stream_index] =
        nh_.advertiseService<std_srvs::SetBoolRequest, std_srvs::SetBoolResponse>(
            service_name, [this, stream_index](std_srvs::SetBoolRequest& request,
                                               std_srvs::SetBoolResponse& response) {
              response.success = this->SetFlipCallback(request, response, stream_index);
              return response.success;
            });
    service_name = "/" + camera_name_ + "/" + "set_" + stream_name + "_rotation";
    set_rotation_srv_[stream_index] = nh_.advertiseService<SetInt32Request, SetInt32Response>(
        service_name, [this, stream_index](SetInt32Request& request, SetInt32Response& response) {
          response.success = this->SetRotationCallback(request, response, stream_index);
          return response.success;
        });
    service_name = "/" + camera_name_ + "/" + "set_" + stream_name + "_auto_exposure";
    set_auto_exposure_srv_[stream_index] =
        nh_.advertiseService<std_srvs::SetBoolRequest, std_srvs::SetBoolResponse>(
            service_name, [this, stream_index](std_srvs::SetBoolRequest& request,
                                               std_srvs::SetBoolResponse& response) {
              response.success = this->SetAutoExposureCallback(request, response, stream_index);
              return response.success;
            });
    service_name = "/" + camera_name_ + "/" + "get_" + stream_name + "_auto_exposure";
    get_auto_exposure_srv_[stream_index] = nh_.advertiseService<GetBoolRequest, GetBoolResponse>(
        service_name, [this, stream_index](GetBoolRequest& request, GetBoolResponse& response) {
          response.success = this->GetAutoExposureCallback(request, response, stream_index);
          return response.success;
        });
    service_name = "/" + camera_name_ + "/" + "toggle_" + stream_name;
    toggle_sensor_srv_[stream_index] =
        nh_.advertiseService<std_srvs::SetBoolRequest, std_srvs::SetBoolResponse>(
            service_name, [this, stream_index](std_srvs::SetBoolRequest& request,
                                               std_srvs::SetBoolResponse& response) {
              response.success = this->ToggleSensorCallback(request, response, stream_index);
              return response.success;
            });
    service_name = "/" + camera_name_ + "/" + "get_" + stream_name + "_camera_info";
    get_camera_info_srv_[stream_index] =
        nh_.advertiseService<GetCameraInfoRequest, GetCameraInfoResponse>(
            service_name,
            [this, stream_index](GetCameraInfoRequest& request, GetCameraInfoResponse& response) {
              response.success = this->GetCameraInfoCallback(request, response, stream_index);
              return response.success;
            });
  }
  get_auto_white_balance_srv_ = nh_.advertiseService<GetInt32Request, GetInt32Response>(
      "/" + camera_name_ + "/" + "get_auto_white_balance",
      [this](GetInt32Request& request, GetInt32Response& response) {
        response.success = this->GetAutoWhiteBalanceCallback(request, response);
        return response.success;
      });
  set_auto_white_balance_srv_ = nh_.advertiseService<SetInt32Request, SetInt32Response>(
      "/" + camera_name_ + "/" + "set_auto_white_balance",
      [this](SetInt32Request& request, SetInt32Response& response) {
        response.success = this->SetAutoWhiteBalanceCallback(request, response);
        return response.success;
      });
  get_white_balance_srv_ = nh_.advertiseService<GetInt32Request, GetInt32Response>(
      "/" + camera_name_ + "/" + "get_white_balance",
      [this](GetInt32Request& request, GetInt32Response& response) {
        response.success = this->GetWhiteBalanceCallback(request, response);
        return response.success;
      });
  set_white_balance_srv_ = nh_.advertiseService<SetInt32Request, SetInt32Response>(
      "/" + camera_name_ + "/" + "set_white_balance",
      [this](SetInt32Request& request, SetInt32Response& response) {
        response.success = this->SetWhiteBalanceCallback(request, response);
        return response.success;
      });
  reset_white_balance_srv_ = nh_.advertiseService<std_srvs::EmptyRequest, std_srvs::EmptyResponse>(
      "/" + camera_name_ + "/" + "reset_white_balance",
      [this](std_srvs::EmptyRequest& request, std_srvs::EmptyResponse& response) {
        return this->ResetCameraWhiteBalanceCallback(request, response);
      });

  set_laser_srv_ = nh_.advertiseService<std_srvs::SetBoolRequest, std_srvs::SetBoolResponse>(
      "/" + camera_name_ + "/" + "set_laser",
      [this](std_srvs::SetBoolRequest& request, std_srvs::SetBoolResponse& response) {
        response.success = this->SetLaserCallback(request, response);
        return response.success;
      });

  get_device_info_srv_ = nh_.advertiseService<GetDeviceInfoRequest, GetDeviceInfoResponse>(
      "/" + camera_name_ + "/" + "get_device_info",
      [this](GetDeviceInfoRequest& request, GetDeviceInfoResponse& response) {
        response.success = this->GetDeviceInfoCallback(request, response);
        return response.success;
      });
  get_serial_number_srv_ = nh_.advertiseService<GetStringRequest, GetStringResponse>(
      "/" + camera_name_ + "/" + "get_serial",
      [this](GetStringRequest& request, GetStringResponse& response) {
        response.success = this->GetSerialNumberCallback(request, response);
        return response.success;
      });
  get_camera_params_srv_ = nh_.advertiseService<GetCameraParamsRequest, GetCameraParamsResponse>(
      "/" + camera_name_ + "/" + "get_camera_params",
      [this](GetCameraParamsRequest& request, GetCameraParamsResponse& response) {
        response.success = this->GetCameraParamsCallback(request, response);
        return response.success;
      });

  get_sdk_version_srv_ = nh_.advertiseService<GetStringRequest, GetStringResponse>(
      "/" + camera_name_ + "/" + "get_sdk_version",
      [this](GetStringRequest& request, GetStringResponse& response) {
        response.success = this->GetSdkVersionCallback(request, response);
        return response.success;
      });

  save_point_cloud_srv_ = nh_.advertiseService<std_srvs::EmptyRequest, std_srvs::EmptyResponse>(
      "/" + camera_name_ + "/" + "save_point_cloud",
      [this](std_srvs::EmptyRequest& request, std_srvs::EmptyResponse& response) {
        return this->SavePointCloudCallback(request, response);
      });
  save_images_srv_ = nh_.advertiseService<std_srvs::EmptyRequest, std_srvs::EmptyResponse>(
      "/" + camera_name_ + "/" + "save_images",
      [this](std_srvs::EmptyRequest& request, std_srvs::EmptyResponse& response) {
        return this->SaveImagesCallback(request, response);
      });
}

bool DtCameraNode::SetMirrorCallback(std_srvs::SetBoolRequest& request,
                                     std_srvs::SetBoolResponse& response,
                                     const stream_index_pair& stream_index) {
  if (!enable_stream_[stream_index]) {
    ROS_ERROR_STREAM("Camera " << stream_name_[stream_index] << " is not enabled.");
    response.success = false;
    return response.success;
  }
  auto stream = stream_index.first;
  try {
    switch (stream) {
      case kDtStreamIrRight:
        device_->SetBoolOption(kDtOptionIrRightMirrorBool, request.data);
        break;
      case kDtStreamIrLeft:
      case kDtStreamIr:
        device_->SetBoolOption(kDtOptionIrMirrorBool, request.data);
        break;
      case kDtStreamDepth:
        device_->SetBoolOption(kDtOptionDepthMirrorBool, request.data);
        break;
      case kDtStreamColor:
        device_->SetBoolOption(kDtOptionColorMirrorBool, request.data);
        break;
      default:
        ROS_ERROR_STREAM(" NOT a video stream" << __FUNCTION__);
        return false;
        break;
    }
  } catch (const dt::Error& e) {
    ROS_ERROR_STREAM("Failed to set mirror mode: " << e.GetMessage());
    response.success = false;
    return response.success;
  }
  return true;
}

bool DtCameraNode::SetFlipCallback(std_srvs::SetBoolRequest& request,
                                   std_srvs::SetBoolResponse& response,
                                   const stream_index_pair& stream_index) {
  if (!enable_stream_[stream_index]) {
    ROS_ERROR_STREAM("Camera " << stream_name_[stream_index] << " is not enabled.");
    response.success = false;
    return response.success;
  }
  auto stream = stream_index.first;
  try {
    switch (stream) {
      case kDtStreamIrRight:
        device_->SetBoolOption(kDtOptionIrRightFlipBool, request.data);
        break;
      case kDtStreamIrLeft:
        device_->SetBoolOption(kDtOptionIrFlipBool, request.data);
        break;
      case kDtStreamIr:
        device_->SetBoolOption(kDtOptionIrFlipBool, request.data);
        break;
      case kDtStreamDepth:
        device_->SetBoolOption(kDtOptionDepthFlipBool, request.data);
        break;
      case kDtStreamColor:
        device_->SetBoolOption(kDtOptionColorFlipBool, request.data);
        break;
      default:
        ROS_ERROR_STREAM(" NOT a video stream" << __FUNCTION__);
        return false;
        break;
    }
  } catch (const dt::Error& e) {
    ROS_ERROR_STREAM("Failed to set flip mode: " << e.GetMessage());
    response.success = false;
    return response.success;
  }
  return true;
}

bool DtCameraNode::SetRotationCallback(SetInt32Request& request, SetInt32Response& response,
                                       const stream_index_pair& stream_index) {
  if (!enable_stream_[stream_index]) {
    ROS_ERROR_STREAM("Camera " << stream_name_[stream_index] << " is not enabled.");
    response.success = false;
    return false;
  }
  auto stream = stream_index.first;
  try {
    switch (stream) {
      case kDtStreamIrRight:
        device_->SetIntOption(kDtOptionIrRightRotateInt, request.data);
        return true;
        break;
      case kDtStreamIrLeft:
        device_->SetIntOption(kDtOptionIrRotateInt, request.data);
        return true;
        break;
      case kDtStreamIr:
        device_->SetIntOption(kDtOptionIrRotateInt, request.data);
        return true;
        break;
      case kDtStreamDepth:
        device_->SetIntOption(kDtOptionDepthRotateInt, request.data);
        return true;
        break;
      case kDtStreamColor:
        device_->SetIntOption(kDtOptionColorRotateInt, request.data);
        return true;
        break;
      default:
        ROS_ERROR_STREAM(" NOT a video stream" << __FUNCTION__);
        return false;
        break;
    }
  } catch (const dt::Error& e) {
    ROS_ERROR_STREAM("Failed to set rotation mode: " << e.GetMessage());
    response.success = false;
    return false;
  }
  return true;
}

bool DtCameraNode::GetExposureCallback(GetInt32Request& request, GetInt32Response& response,
                                       const stream_index_pair& stream_index) {
  (void)request;
  if (!enable_stream_[stream_index]) {
    ROS_ERROR_STREAM("Camera " << stream_name_[stream_index] << " is not enabled.");
    response.success = false;
    return false;
  }
  auto sensor = sensors_[stream_index];
  try {
    response.data = sensor->GetExposure();
  } catch (const dt::Error& e) {
    ROS_ERROR_STREAM("Failed to get exposure: " << e.GetMessage());
    response.success = false;
    return false;
  }
  return true;
}

bool DtCameraNode::SetExposureCallback(SetInt32TwoParamRequest& request,
                                       SetInt32TwoParamResponse& response,
                                       const stream_index_pair& stream_index) {
  if (!enable_stream_[stream_index]) {
    ROS_ERROR_STREAM("Camera " << stream_name_[stream_index] << " is not enabled.");
    response.success = false;
    return false;
  }
  auto sensor = sensors_[stream_index];
  try {
    // todo::暂不支持读 后续再加
    //  auto range = sensor->GetExposureRange();
    //  if (request.data < range.min || request.data > range.max) {
    //    ROS_ERROR_STREAM("Exposure value " << request.data << " out of range" << range.min << " -
    //    "
    //                                       << range.max);
    //    response.success = false;
    //    return false;
    //  }
    sensor->SetExposure(request.data, request.data_two);
  } catch (const dt::Error& e) {
    ROS_ERROR_STREAM("Failed to set exposure: " << e.GetMessage());
    response.success = false;
    return false;
  } catch (const std::exception& e) {
    ROS_INFO_STREAM("Failed to set exposure: " << e.what());
    response.success = false;
    return false;
  }
  return true;
}

bool DtCameraNode::setCropRoiEnableCallback(std_srvs::SetBoolRequest& request,
                                            std_srvs::SetBoolResponse& response,
                                            const stream_index_pair& stream_index,
                                            DtOptionId optionId) {
  try {
    auto stream = stream_index.first;
    switch (stream) {
      case kDtStreamDepth: {
        auto device_info = device_->GetDeviceInfo();
        CHECK_NOTNULL(device_info);
        auto pid = device_info->GetPid();
        if (IsNebulaSeriesPid(pid)) {
          if (optionId == kDtOptionMtofDepthCropRoiStruct) {
            depth_crop_roi_mtof_enable_ = request.data;
            ROS_INFO_STREAM("setCropRoiEnableCallback to depth_crop_roi_mtof_enable_: "
                            << depth_crop_roi_mtof_enable_);
          } else if (optionId == kDtOptionStofDepthCropRoiStruct) {
            depth_crop_roi_stof_enable_ = request.data;
            ROS_INFO_STREAM("setCropRoiEnableCallback to depth_crop_roi_stof_enable: "
                            << depth_crop_roi_stof_enable_);
          } else {
            throw std::runtime_error(
                "setCropRoiEnableCallback optionId is not kDtOptionMtofDepthCropRoiStruct or "
                "kDtOptionStofDepthCropRoiStruct");
          }

          response.success = true;
          return true;
        }
        throw std::runtime_error(
            "setCropRoiEnableCallback current not support except nebula series!");
      } break;
      default:
        throw std::runtime_error(
            "setCropRoiEnableCallback NOT a video stream or not  nebula series!");
    }

  } catch (const dt::Error& e) {
    ROS_ERROR_STREAM("setCropRoiEnableCallback failed : " << e.GetMessage());
    response.success = false;
    response.message = e.GetMessage();
    return false;
  } catch (const std::exception& e) {
    ROS_ERROR_STREAM("setCropRoiEnableCallback failed : " << e.what());
    response.success = false;
    response.message = e.what();
    return false;
  }
}

// bool DtCameraNode::GetCropRoiEnableCallback(GetBoolRequest& request, GetBoolResponse& response,
//                                             const stream_index_pair& stream_index) {
//   (void)request;
//   try {
//     auto stream = stream_index.first;
//     switch (stream) {
//       case kDtStreamDepth: {
//         auto device_info = device_->GetDeviceInfo();
//         CHECK_NOTNULL(device_info);
//         auto pid = device_info->GetPid();
//         ROS_INFO_STREAM("GetCropRoiEnableCallback to pid: " << pid);
//         if (IsNebula410Pid(pid)) {
//           response.data = depth_crop_roi_enable_;
//           ROS_INFO_STREAM(
//               "GetCropRoiEnableCallback to depth_crop_roi_enable: " << depth_crop_roi_enable_);
//           response.success = true;
//           return true;
//         }
//         throw std::runtime_error("current not support except 410!");
//       }
//       default:
//         throw std::runtime_error("GetCropRoiEnableCallback NOT a video stream or not 410!");
//     }

//   } catch (const dt::Error& e) {
//     ROS_ERROR_STREAM("GetCropRoiEnableCallback failed : " << e.GetMessage());
//     response.success = false;
//     response.message = e.GetMessage();
//     return false;
//   } catch (const std::exception& e) {
//     ROS_ERROR_STREAM("GetCropRoiEnableCallback failed : " << e.what());
//     response.success = false;
//     response.message = e.what();
//     return false;
//   }
// }

bool DtCameraNode::SetCropRoiCallback(SetArraysRequest& request, SetArraysResponse& response,
                                      const stream_index_pair& stream_index) {
  auto stream = stream_index.first;
  auto config = CropRoi();
  uint32_t data_size = sizeof(config);
  try {
    switch (stream) {
      case kDtStreamDepth: {
        auto device_info = device_->GetDeviceInfo();
        CHECK_NOTNULL(device_info);
        auto pid = device_info->GetPid();

        if (!IsNebulaSeriesPid(pid)) {
          throw std::runtime_error("not implement SetCropRoiCallback except nebula series now");
        }

        int crop_type = static_cast<int>(request.data_param[4]);
        if (crop_type == kDtOptionMtofDepthCropRoiStruct && !depth_crop_roi_mtof_enable_) {
          throw std::runtime_error(
              "SetCropRoiCallback launch file depth_crop_roi_mtof_enable is false");
        }
        if (crop_type == kDtOptionStofDepthCropRoiStruct && !depth_crop_roi_stof_enable_) {
          throw std::runtime_error(
              "SetCropRoiCallback launch file depth_crop_roi_stof_enable is false");
        }

        const int raw_x0 = request.data_param[0];
        const int raw_y0 = request.data_param[1];
        const int width = request.data_param[2];
        const int height = request.data_param[3];
        
        const int max_width = static_cast<int>(width_[DEPTH]) - 1;
        const int max_height = static_cast<int>(height_[DEPTH]) - 1;

        auto clamp = [](int val, int min, int max) -> int {
          if (val < min) return min;
          if (val > max) return max;
          return val;
        };

        config.x0_left = static_cast<uint8_t>(clamp(raw_x0, 0, max_width));
        config.y0_top = static_cast<uint8_t>(clamp(raw_y0, 0, max_height));
        config.width = static_cast<uint8_t>(clamp(width, 0, max_width));
        config.height = static_cast<uint8_t>(clamp(height, 0, max_height));

        device_->SetStructuredData((dt_option_id)crop_type,
                                   reinterpret_cast<const uint8_t*>(&config), data_size);
        ROS_INFO_STREAM("SetDepthCropExposureRoi x0_left:"
                        << (int)config.x0_left << " ,y0_top: " << (int)config.y0_top
                        << " ,width: " << (int)config.width << " ,height :" << (int)config.height
                        << " ,crop_type:" << (int)crop_type);
        response.success = true;
        return true;
      }
      default:
        ROS_ERROR_STREAM("SetCropRoiCallback NOT a video stream or not 410" << __FUNCTION__);
        response.success = false;
        response.message = "setCropRoiCallbackNOT a video stream or not 410";
        return response.success = false;
    }
    return response.success = true;
  } catch (const dt::Error& e) {
    response.message = e.GetMessage();
    return response.success = false;
  } catch (const std::exception& e) {
    response.message = e.what();
    ROS_ERROR_STREAM("SetCropRoiCallback failed : " << e.what());
    return response.success = false;
  } catch (...) {
    response.message = "unknown error";
    return response.success = false;
  }
}

bool DtCameraNode::SetAeRoiCallback(SetArraysRequest& request, SetArraysResponse& response,
                                    const stream_index_pair& stream_index) {
  auto stream = stream_index.first;
  auto config = DtRegionOfInterest();
  uint32_t data_size = sizeof(config);
  try {
    switch (stream) {
      case kDtStreamIrLeft:
      case kDtStreamIrRight:
      case kDtStreamIr:
      case kDtStreamDepth:
        config.x = (static_cast<short int>(request.data_param[0]) < 0)
                       ? 0
                       : static_cast<short int>(request.data_param[0]);
        config.x = (static_cast<short int>(request.data_param[0]) > width_[DEPTH] - 1)
                       ? width_[DEPTH] - 1
                       : config.x;
        config.y = (static_cast<short int>(request.data_param[2]) < 0)
                       ? 0
                       : static_cast<short int>(request.data_param[2]);
        config.y = (static_cast<short int>(request.data_param[2]) > height_[DEPTH] - 1)
                       ? height_[DEPTH] - 1
                       : config.y;
        config.width = (static_cast<short int>(request.data_param[1]) < 0)
                           ? 0
                           : static_cast<short int>(request.data_param[1]);
        config.width = (static_cast<short int>(request.data_param[1]) > width_[DEPTH] - 1)
                           ? width_[DEPTH] - 1
                           : config.width;
        config.height = (static_cast<short int>(request.data_param[3]) < 0)
                            ? 0
                            : static_cast<short int>(request.data_param[3]);
        config.height = (static_cast<short int>(request.data_param[3]) > height_[DEPTH] - 1)
                            ? height_[DEPTH] - 1
                            : config.height;
        device_->SetStructuredData(kDtOptionDepthAeRoiStruct,
                                   reinterpret_cast<const uint8_t*>(&config), sizeof(config));
        device_->GetStructuredData(kDtOptionDepthAeRoiStruct, reinterpret_cast<uint8_t*>(&config),
                                   &data_size);
        ROS_INFO_STREAM("set depth AE ROI : " << "[x: " << config.x << ", y: " << config.y
                                              << ", width: " << config.width
                                              << ", height: " << config.height << " ]");
        break;
      case kDtStreamColor:
        config.x = (static_cast<short int>(request.data_param[0]) < 0)
                       ? 0
                       : static_cast<short int>(request.data_param[0]);
        config.x = (static_cast<short int>(request.data_param[0]) > width_[COLOR] - 1)
                       ? width_[COLOR] - 1
                       : config.x;
        config.y = (static_cast<short int>(request.data_param[2]) < 0)
                       ? 0
                       : static_cast<short int>(request.data_param[2]);
        config.y = (static_cast<short int>(request.data_param[2]) > height_[COLOR] - 1)
                       ? height_[COLOR] - 1
                       : config.y;
        config.width = (static_cast<short int>(request.data_param[1]) < 0)
                           ? 0
                           : static_cast<short int>(request.data_param[1]);
        config.width = (static_cast<short int>(request.data_param[1]) > width_[COLOR] - 1)
                           ? width_[COLOR] - 1
                           : config.width;
        config.height = (static_cast<short int>(request.data_param[3]) < 0)
                            ? 0
                            : static_cast<short int>(request.data_param[3]);
        config.height = (static_cast<short int>(request.data_param[3]) > height_[COLOR] - 1)
                            ? height_[COLOR] - 1
                            : config.height;
        device_->SetStructuredData(kDtOptionColorAeRoiStruct,
                                   reinterpret_cast<const uint8_t*>(&config), sizeof(config));
        device_->GetStructuredData(kDtOptionColorAeRoiStruct, reinterpret_cast<uint8_t*>(&config),
                                   &data_size);
        ROS_INFO_STREAM("set color AE ROI : " << "[x: " << config.x << ", y: " << config.y
                                              << ", width: " << config.width
                                              << ", height: " << config.height << " ]");
        break;
      default:
        ROS_ERROR_STREAM(" NOT a video stream" << __FUNCTION__);
        response.success = false;
        response.message = "NOT a video stream";
        return response.success = false;
    }
    return response.success = true;
  } catch (const dt::Error& e) {
    response.message = e.GetMessage();
    return response.success = false;
  } catch (const std::exception& e) {
    response.message = e.what();
    return response.success = false;
  } catch (...) {
    response.message = "unknown error";
    return response.success = false;
  }
}

bool DtCameraNode::GetGainCallback(GetInt32Request& request, GetInt32Response& response,
                                   const stream_index_pair& stream_index) {
  (void)request;
  if (!enable_stream_[stream_index]) {
    ROS_ERROR_STREAM("Camera " << stream_name_[stream_index] << " is not enabled.");
    response.success = false;
    return false;
  }
  auto sensor = sensors_[stream_index];
  try {
    response.data = sensor->GetGain();
  } catch (const dt::Error& e) {
    ROS_ERROR_STREAM("Failed to get gain: " << e.GetMessage());
    response.success = false;
    return false;
  }
  return true;
}

bool DtCameraNode::SetGainCallback(SetInt32Request& request, SetInt32Response& response,
                                   const stream_index_pair& stream_index) {
  if (!enable_stream_[stream_index]) {
    ROS_ERROR_STREAM("Camera " << stream_name_[stream_index] << " is not enabled.");
    response.success = false;
    return false;
  }
  auto sensor = sensors_[stream_index];
  try {
    auto range = sensor->GetGainRange();
    if (request.data < range.min || request.data > range.max) {
      ROS_ERROR_STREAM("Gain value " << request.data << " out of range" << range.min << " - "
                                     << range.max);
      response.success = false;
      return false;
    }
    sensor->SetGain(request.data);
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    auto gain = sensor->GetGain();
    ROS_INFO_STREAM("After set gain: " << gain);
  } catch (const dt::Error& e) {
    ROS_ERROR_STREAM("Failed to set gain: " << e.GetMessage());
    response.success = false;
    return false;
  }
  return true;
}

bool DtCameraNode::GetAutoWhiteBalanceCallback(GetInt32Request& request,
                                               GetInt32Response& response) {
  (void)request;
  if (!enable_stream_[COLOR]) {
    ROS_ERROR_STREAM("Camera " << stream_name_[COLOR] << " is not enabled.");
    response.success = false;
    return false;
  }
  auto sensor = sensors_[COLOR];
  try {
    response.data = sensor->GetAutoWhiteBalance();
  } catch (const dt::Error& e) {
    ROS_ERROR_STREAM("Failed to get auto white balance: " << e.GetMessage());
    response.success = false;
    return false;
  }
  return true;
}

bool DtCameraNode::SetAutoWhiteBalanceCallback(SetInt32Request& request,
                                               SetInt32Response& response) {
  if (!enable_stream_[COLOR]) {
    ROS_ERROR_STREAM("Camera " << stream_name_[COLOR] << " is not enabled.");
    response.success = false;
    return false;
  }
  auto sensor = sensors_[COLOR];
  try {
    auto result = sensor->GetAutoWhiteBalance();
    ROS_INFO_STREAM("Current auto white balance: " << result);
    sensor->SetAutoWhiteBalance(request.data);
    ROS_INFO_STREAM("Set auto white balance to: " << request.data);
    result = sensor->GetAutoWhiteBalance();
    ROS_INFO_STREAM("After set auto white balance: " << result);
  } catch (const dt::Error& e) {
    ROS_ERROR_STREAM("Failed to set auto white balance: " << e.GetMessage());
    response.success = false;
    return false;
  }
  return true;
}

bool DtCameraNode::GetWhiteBalanceCallback(GetInt32Request& request, GetInt32Response& response) {
  (void)request;
  if (!enable_stream_[COLOR]) {
    ROS_ERROR_STREAM("Camera " << stream_name_[COLOR] << " is not enabled.");
    response.success = false;
    return false;
  }
  auto sensor = sensors_[COLOR];
  try {
    response.data = sensor->GetWhiteBalance();
  } catch (const dt::Error& e) {
    ROS_ERROR_STREAM("Failed to get white balance: " << e.GetMessage());
    response.success = false;
    return false;
  }
  return true;
}

bool DtCameraNode::SetWhiteBalanceCallback(SetInt32Request& request, SetInt32Response& response) {
  if (!enable_stream_[COLOR]) {
    ROS_ERROR_STREAM("Camera " << stream_name_[COLOR] << " is not enabled.");
    response.success = false;
    return false;
  }
  auto sensor = sensors_[COLOR];
  try {
    auto range = sensor->GetWhiteBalanceRange();
    if (request.data < range.min || request.data > range.max) {
      ROS_ERROR_STREAM("White balance value " << request.data << " out of range" << range.min
                                              << " - " << range.max);
      response.success = false;
      return false;
    }
    bool is_auto_white_balance = sensor->GetAutoWhiteBalance();
    if (is_auto_white_balance) {
      ROS_ERROR_STREAM("Auto white balance is enabled, please disable it first.");
      response.success = false;
      return false;
    }
    sensor->SetWhiteBalance(request.data);
  } catch (const dt::Error& e) {
    ROS_ERROR_STREAM("Failed to set white balance: " << e.GetMessage());
    response.success = false;
    return false;
  }
  return true;
}

bool DtCameraNode::SetAutoExposureCallback(std_srvs::SetBoolRequest& request,
                                           std_srvs::SetBoolResponse& response,
                                           const stream_index_pair& stream_index) {
  if (!enable_stream_[stream_index]) {
    ROS_ERROR_STREAM("Camera " << stream_name_[stream_index] << " is not enabled.");
    response.success = false;
    return false;
  }
  auto sensor = sensors_[stream_index];
  try {
    sensor->SetAutoExposure(request.data);
  } catch (const dt::Error& e) {
    ROS_ERROR_STREAM("Failed to set auto exposure: " << e.GetMessage());
    response.success = false;
    return false;
  }
  return true;
}

bool DtCameraNode::GetAutoExposureCallback(GetBoolRequest& request, GetBoolResponse& response,
                                           const stream_index_pair& stream_index) {
  (void)request;
  if (!enable_stream_[stream_index]) {
    ROS_ERROR_STREAM("Camera " << stream_name_[stream_index] << " is not enabled.");
    response.success = false;
    return false;
  }
  auto sensor = sensors_[stream_index];
  try {
    response.data = sensor->GetAutoExposure();
  } catch (const dt::Error& e) {
    ROS_ERROR_STREAM("Failed to get auto exposure: " << e.GetMessage());
    response.success = false;
    return false;
  }
  return true;
}

bool DtCameraNode::SetLaserCallback(std_srvs::SetBoolRequest& request,
                                    std_srvs::SetBoolResponse& response) {
  (void)response;
  std::lock_guard<decltype(device_lock_)> lock(device_lock_);
  try {
    int data = request.data ? 1 : 0;
    if (device_->IsOptionSupported(kDtOptionLaserControlInt, kDtPermissionReadWrite)) {
      device_->SetIntOption(kDtOptionLaserControlInt, data);
    } else if (device_->IsOptionSupported(kDtOptionLaserBool, kDtPermissionReadWrite)) {
      device_->SetIntOption(kDtOptionLaserBool, data);
    }
  } catch (const dt::Error& e) {
    ROS_ERROR_STREAM("Failed to set laser: " << e.GetMessage());
    response.message = e.GetMessage();
    return false;
  }
  return true;
}

bool DtCameraNode::GetDeviceInfoCallback(GetDeviceInfoRequest& request,
                                         GetDeviceInfoResponse& response) {
  (void)request;
  std::lock_guard<decltype(device_lock_)> lock(device_lock_);
  auto device_info = device_->GetDeviceInfo();
  response.info.name = device_info->GetName();
  response.info.serial_number = device_info->GetSerialNumber();
  response.info.firmware_version = device_info->GetFirmwareVersion();
  response.info.supported_min_sdk_version = device_info->GetSupportedMinSdkVersion();
  response.success = true;
  return true;
}

bool DtCameraNode::GetSdkVersionCallback(GetStringRequest& request, GetStringResponse& response) {
  (void)request;
  std::lock_guard<decltype(device_lock_)> lock(device_lock_);
  auto device_info = device_->GetDeviceInfo();
  nlohmann::json data;
  data["firmware_version"] = device_info->GetFirmwareVersion();
  data["supported_min_sdk_version"] = device_info->GetSupportedMinSdkVersion();
  data["ros_sdk_version"] = DT_ROS_VERSION_STR;
  std::string major = std::to_string(dt::Version::GetMajor());
  std::string minor = std::to_string(dt::Version::GetMinor());
  std::string patch = std::to_string(dt::Version::GetPatch());
  std::string version = major + "." + minor + "." + patch;
  data["dt_sdk_version"] = version;
  response.data = data.dump(2);
  response.success = true;
  return true;
}

bool DtCameraNode::ToggleSensorCallback(std_srvs::SetBoolRequest& request,
                                        std_srvs::SetBoolResponse& response,
                                        const stream_index_pair& stream_index) {
  std::string msg;
  if (request.data) {
    if (enable_stream_[stream_index]) {
      msg = stream_name_[stream_index] + " Already ON";
    }
    ROS_INFO_STREAM("toggling sensor " << stream_name_[stream_index] << " ON");
  } else {
    if (!enable_stream_[stream_index]) {
      msg = stream_name_[stream_index] + " Already OFF";
    }
    ROS_INFO_STREAM("toggling sensor " << stream_name_[stream_index] << " OFF");
  }
  if (!msg.empty()) {
    ROS_INFO_STREAM(msg);
    response.success = false;
    response.message = msg;
    return false;
  }
  response.success = ToggleSensor(stream_index, request.data, response.message);
  return response.success;
}

bool DtCameraNode::ToggleSensor(const stream_index_pair& stream_index, bool enabled,
                                std::string& msg) {
  (void)msg;
  StopStreams();
  enable_stream_[stream_index] = enabled;
  StartStreams();
  return true;
}

bool DtCameraNode::SaveImagesCallback(std_srvs::EmptyRequest& request,
                                      std_srvs::EmptyResponse& response) {
  (void)request;
  (void)response;
  for (const auto& stream_index : IMAGE_STREAMS) {
    if (enable_stream_[stream_index]) {
      save_images_[stream_index] = true;
      save_images_count_[stream_index] = 0;
    } else {
      ROS_WARN_STREAM("Camera " << stream_name_[stream_index] << " is not enabled.");
    }
  }
  return true;
}

bool DtCameraNode::SavePointCloudCallback(std_srvs::EmptyRequest& request,
                                          std_srvs::EmptyResponse& response) {
  (void)request;
  (void)response;
  save_point_cloud_ = true;
  save_colored_point_cloud_ = true;
  return true;
}

bool DtCameraNode::GetCameraParamsCallback(deptrum_camera::GetCameraParamsRequest& request,
                                           deptrum_camera::GetCameraParamsResponse& response) {
  (void)request;
  try {
    DtCameraParam camera_param{};
    auto default_param = GetCameraParam();
    if (depth_registration_ && pipeline_started_ && pipeline_ != nullptr) {
      camera_param = pipeline_->GetCameraParam();
    } else if (default_param) {
      camera_param = *default_param;
    } else {
      ROS_ERROR_STREAM("get camera param failed");
      response.message = "get camera param failed";
      return false;
    }
    response.l_intr_p[0] = camera_param.depth_intrinsic.fx;
    response.l_intr_p[1] = camera_param.depth_intrinsic.fy;
    response.l_intr_p[2] = camera_param.depth_intrinsic.cx;
    response.l_intr_p[3] = camera_param.depth_intrinsic.cy;
    response.r_intr_p[0] = camera_param.rgb_intrinsic.fx;
    response.r_intr_p[1] = camera_param.rgb_intrinsic.fy;
    response.r_intr_p[2] = camera_param.rgb_intrinsic.cx;
    response.r_intr_p[3] = camera_param.rgb_intrinsic.cy;
    for (int i = 0; i < 9; i++) {
      if (i < 3) {
        response.r2l_t[i] = camera_param.transform.trans[i];
      }
      response.r2l_r[i] = camera_param.transform.rot[i];
    }

  } catch (const dt::Error& e) {
    ROS_ERROR_STREAM("Failed to get camera params: " << e.GetMessage());
    return false;
  }
  return true;
}

bool DtCameraNode::GetSerialNumberCallback(GetStringRequest& request, GetStringResponse& response) {
  (void)request;
  std::lock_guard<decltype(device_lock_)> lock(device_lock_);
  try {
    auto device_info = device_->GetDeviceInfo();
    response.data = device_info->GetSerialNumber();
  } catch (const dt::Error& e) {
    ROS_ERROR_STREAM("Failed to get serial number: " << e.GetMessage());
    return false;
  }
  response.success = true;
  return true;
}

bool DtCameraNode::GetCameraInfoCallback(GetCameraInfoRequest& request,
                                         GetCameraInfoResponse& response,
                                         const stream_index_pair& stream_index) {
  (void)request;
  try {
    auto camera_param = pipeline_->GetCameraParam();
    auto& intrinsic =
        stream_index == COLOR ? camera_param.rgb_intrinsic : camera_param.depth_intrinsic;
    auto& distortion =
        stream_index == COLOR ? camera_param.rgb_distortion : camera_param.depth_distortion;
    auto width = width_[stream_index];
    auto camera_info = ConvertToCameraInfo(intrinsic, distortion, width);
    response.info = camera_info;
  } catch (const dt::Error& e) {
    ROS_ERROR_STREAM("Failed to get camera info: " << e.GetMessage());
    return false;
  }
  return true;
}

bool DtCameraNode::ResetCameraGainCallback(std_srvs::EmptyRequest& request,
                                           std_srvs::EmptyResponse& response,
                                           const stream_index_pair& stream_index) {
  (void)request;
  (void)response;
  auto data = default_gain_[stream_index];
  auto sensor = sensors_[stream_index];
  if (sensor) {
    try {
      auto range = sensor->GetGainRange();
      if (data < range.min || data > range.max) {
        ROS_ERROR_STREAM("Failed to set gain: invalid value");
        return false;
      }
      sensor->SetGain(data);
      return true;
    } catch (const dt::Error& e) {
      ROS_ERROR_STREAM("Failed to set gain: " << e.GetMessage());
      return false;
    }
  } else {
    ROS_ERROR_STREAM("Failed to set gain: sensor is not initialized");
    return false;
  }
}

bool DtCameraNode::ResetCameraExposureCallback(std_srvs::EmptyRequest& request,
                                               std_srvs::EmptyResponse& response,
                                               const stream_index_pair& stream_index) {
  (void)request;
  (void)response;
  auto data = default_exposure_[stream_index];
  auto sensor = sensors_[stream_index];
  if (sensor) {
    try {
      sensor->SetExposure(data, kDtOptionMtofDepthExposureInt);
      return true;
    } catch (const dt::Error& e) {
      ROS_ERROR_STREAM("Failed to reset exposure: " << e.GetMessage());
      return false;
    } catch (const std::exception& e) {
      ROS_INFO_STREAM("Failed to reset exposure: " << e.what());
      return false;
    }
  } else {
    ROS_ERROR_STREAM("Failed to reset exposure: sensor is not initialized");
    return false;
  }
}

bool DtCameraNode::ResetCameraWhiteBalanceCallback(std_srvs::EmptyRequest& request,
                                                   std_srvs::EmptyResponse& response) {
  (void)request;
  (void)response;
  auto data = default_white_balance_;
  auto sensor = sensors_[COLOR];
  if (sensor) {
    try {
      auto range = sensor->GetWhiteBalanceRange();
      if (data < range.min || data > range.max) {
        ROS_ERROR_STREAM("Failed to set white balance: invalid value");
        return false;
      }
      sensor->SetWhiteBalance(data);
      return true;
    } catch (const dt::Error& e) {
      ROS_ERROR_STREAM("Failed to set white balance: " << e.GetMessage());
      return false;
    }
  } else {
    ROS_ERROR_STREAM("Failed to set white balance: sensor is not initialized");
    return false;
  }
}
}  // namespace deptrum_camera
