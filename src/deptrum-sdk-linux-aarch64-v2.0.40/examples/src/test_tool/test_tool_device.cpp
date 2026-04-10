#include "test_tool_device.hpp"
#include <math.h>
#include <future>
#include <mutex>
#include <thread>

// #include "sample_types.h"
#include "frame_rate_helper.h"
#include "test_tool_device_manage.hpp"
#include "test_utils.hpp"
#include "types.hpp"
#include "utils.hpp"

#ifdef BUILD_GRAPHICAL_EXAMPLES
#include "utils_glfw.hpp"
#endif

namespace dt {

std::vector<std::string> meta_data_types = {
    "PALM_ROI_LEFT",
    "PALM_ROI_TOP",
    "PALM_ROI_RIGHT",
    "PALM_ROI_BOTTOM",
    "PSENSOR_VALUE_LEFT_TOP",
    "PSENSOR_VALUE_RIGHT_TOP",
    "PSENSOR_VALUE_RIGHT_BOTTOM",
    "PSENSOR_VALUE_LEFT_BOTTOM",
    "LED_MODE_DATA",
};

void DisplayPointCloud2D(std::shared_ptr<Frame> frame) {
  if (frame == nullptr) {
    return;
  }

#if 0
  // Calculate the number of points in the point cloud
  int points_size = frame->GetDataSize() / sizeof(DtPoint);
  // Convert frame data to a PointXyz<float>* pointer
  DtPoint* points = reinterpret_cast<DtPoint*>(frame->GetData());

  int img_width = 248;
  int img_height = 180;
  cv::Mat img = cv::Mat::zeros(img_height, img_width, CV_8UC3);

  // Compute the boundary of all points (x, y coordinates)
  float min_x = (std::numeric_limits<float>::max)();
  float max_x = std::numeric_limits<float>::lowest();
  float min_y = (std::numeric_limits<float>::max)();
  float max_y = std::numeric_limits<float>::lowest();
  for (int i = 0; i < points_size; i++) {
    float x = points[i].x;
    float y = points[i].y;
    if (x < min_x)
      min_x = x;
    if (x > max_x)
      max_x = x;
    if (y < min_y)
      min_y = y;
    if (y > max_y)
      max_y = y;
  }

  // Prevent division by zero
  float range_x = (max_x - min_x);
  float range_y = (max_y - min_y);
  if (range_x < 1e-5)
    range_x = 1;
  if (range_y < 1e-5)
    range_y = 1;

  // Set margin spacing
  int margin = 10;
  // Calculate scaling factors based on image size to fit the point cloud into the image
  float scale_x = (img_width - 2 * margin) / range_x;
  float scale_y = (img_height - 2 * margin) / range_y;

  // Project and draw each point
  for (int i = 0; i < points_size; i++) {
    // Normalize and scale: map x and y coordinates to the image coordinate system
    int u = static_cast<int>((points[i].x - min_x) * scale_x) + margin;
    int v = static_cast<int>((points[i].y - min_y) * scale_y) + margin;
    // Optional: If y-axis needs to be flipped (OpenCV image coordinates increase from top to
    // bottom), apply: v = img_height - v;
    cv::circle(img, cv::Point(u, v), 1, cv::Scalar(0, 255, 0), -1);
  }

  // Display the image
  cv::imshow("PointCloud2D", img);
  cv::waitKey(1);  // Refresh window with 1ms delay

#endif
}

TestToolDevice::TestToolDevice(std::shared_ptr<Device> device) : device_(device) {
  Init();
}
TestToolDevice::~TestToolDevice() {
  UnInit();
}

void TestToolDevice::UnInit() {
  {
    std::unique_lock<std::mutex> lock(mutex_frame_);
    is_window_thread_running_.store(false);
    cv_frame_.notify_all();
  }

  if (window_thread_.joinable()) {
    window_thread_.join();
  }
  stream_is_running_ = false;

  device_.reset();
  print_title_ = "";
  is_print_fps_ = false;
  is_print_frame_info_ = false;
  is_start_pipeline_ = false;
  properties_.clear();
  id_option_map_.clear();
  pipeline_.reset();
  config_.reset();
  upgrade_event_triggered_ = false;
  point_cloud_.reset();
  enable_point_cloud_ = false;
}

void TestToolDevice::Reset(std::shared_ptr<Device> device) {
  {
    std::unique_lock<std::mutex> lock(mutex_frame_);
    is_window_thread_running_.store(false);
    cv_frame_.notify_all();
  }

  if (window_thread_.joinable()) {
    window_thread_.join();
  }

  device_ = device;
  Init();

  StartPipeline();
  StartStream();
}

std::string TestToolDevice::GetDeivceUid() {
  return device_info_->GetUid();
}

void TestToolDevice::PrintDeviceOptions() {
  while (1) {
    SetColorBoldYellowText("-------------------");
    SetColorBoldYellowText(print_title_);
    SetColorBoldYellowText("-------------------");
    SetBlueText("66  show print info again");
    SetBlueText("99  return main menu");
    SetBlueText("2:  get device info");
    SetBlueText("3:  get version info");
    SetBlueText("4:  set logger severity");
    SetBlueText("5:  set logger to file");
    SetBlueText("6:  get sensorlist info");
    SetBlueText("7:  get sensor by type");
    SetBlueText("8:  writeflash");
    SetBlueText("9:  readflash");
    SetBlueText("10:  device upgrade");
    SetBlueText("11:  set heartbeat");
    // SetBlueText("12:  stop heartbeat");
    SetBlueText("13:  get device status");
    SetBlueText("14:  get camera parameters");
    SetBlueText("15:  reboot");
    SetBlueText("16:  write license");
    SetBlueText("17:  read license");
    // SetBlueText("18:  get supported streamtype");
    SetBlueText("19:  option usage");
    SetBlueText("20:  print stream pipeline usage");
    // SetBlueText("21:  set ledmode stress test");
    // SetBlueText("22:  whole process stress test");
    SetBlueText("23:  stream process stress test");
    SetBlueText("24:  enable fps");
    // SetBlueText("25:  print imu data");
    // SetBlueText("26:  SetHardwareTimestamp");
    // SetBlueText("27:  GetHardwareTimestamp");
    SetBlueText("28:  SendFileToDevice");
    SetBlueText("29:  ReceiveFileFromDevice");
    SetBlueText("30:  Set/Get device serial number");

    std::cout << std::endl;
    std::cout << "input your choice: ";
    int selection = 0;
    std::cin >> selection;
    if (std::cin.fail()) {
      std::cout << "what are you doing, entered an unknown num" << std::endl;
      std::cin.clear();
      std::cin.ignore();
      continue;
    }

    // if (!device_is_open_.load()) {
    //   if (selection == 0 || selection == 66 || selection == 99 || selection == 20 ||
    //       selection == 22 || selection == 13) {
    //   } else {
    //     std::cout << std::endl;
    //     std::cout << std::endl;
    //     std::cout << "must open device first!!" << std::endl;
    //     std::cout << std::endl;
    //     std::cout << std::endl;
    //     continue;
    //   }
    // }
    switch (selection) {
      case 66:
        break;
      case 99:
        return;
      // case 0:
      //   OpenDevice();
      //   break;
      // case 1:
      //   CloseDevice();
      //   return;
      //   break;
      case 2:
        GetDeviceInfo();
        break;
      case 3:
        GetVersionInfo();
        break;
      case 4:
        SetLoggerSeverity();
        break;
      case 5:
        SetLoggerToFile();
        break;
      case 6:
        GetSensorList();
        break;
      case 7:
        GetSensorByType();
        break;
      case 8:
        WriteFlash();
        break;
      case 9:
        ReadFlash();
        break;
      case 10:
        UpdateFirmware();
        break;
      case 11:
        SetHeartbeat();
        break;
      // case 12:
      //   StopHeartbeat();
      //   break;
      case 13:
        GetDeviceStatus();
        break;
      case 14:
        GetCameraParameters();
        break;
      case 15:
        Reboot();
        break;
      case 16:
        WriteLicense();
        break;
      case 17:
        ReadLicense();
        break;
      // case 18:
      //   GetSupportedStreamType();
      //   break;
      case 19:
        PrintOptionUsage();
        break;
      case 20:
        PrintStreamPipelineUsage();
        break;
      // case 21:
      //   SetLedModeStressTest();
      //   break;
      // case 22:
      //   WholeProcessStressTest();
      //   break;
      case 23:
        StreamProcessStressTest();
        break;
      case 24:
        EnableFps();
        break;
      // case 25:
      //   PrintImuData();
      //   break;
      case 28: {
        std::cout << "SendFileToDevice" << std::endl;
        SendFileToDevice();
      } break;
      case 29: {
        std::cout << "ReceiveFileFromDevice" << std::endl;
        ReceiveFileFromDevice();
      } break;
      case 30: {
        SetDeviceSerialNumber();
        GetDeviceSerialNumber();
      } break;
      default:
        std::cout << "what are you doing, entered an unknown num" << std::endl;
        break;
    }
    std::cout << std::endl;
    std::cout << std::endl;
  }
}

void TestToolDevice::GetDeviceInfo() {
  std::cout << std::endl;
  std::cout << "----------------------" << print_title_ << " get device info"
            << "----------------------" << std::endl;
  std::cout << "device name: " << device_info_->GetName() << std::endl;
  std::cout << "pid: " << device_info_->GetPid() << std::endl;
  std::cout << "vid: " << device_info_->GetVid() << std::endl;
  std::cout << "serial number: " << device_info_->GetSerialNumber() << std::endl;
  std::cout << "port path: " << device_info_->GetUid() << std::endl;
  std::cout << "connection type: " << device_info_->GetConnectionType() << std::endl;
  std::cout << "supported min sdk version: " << device_info_->GetSupportedMinSdkVersion()
            << std::endl;
  std::cout << "firmware version: " << device_info_->GetFirmwareVersion() << std::endl;
  std::cout << "hardware version: " << device_info_->GetHardwareVersion() << std::endl;
}
void TestToolDevice::GetVersionInfo() {
  std::cout << std::endl;
  std::cout << "----------------------" << print_title_ << " get version info"
            << "----------------------" << std::endl;
  std::cout << "major version: " << Version::GetMajor() << std::endl;
  std::cout << "minor version: " << Version::GetMajor() << std::endl;
  std::cout << "patch version: " << Version::GetPatch() << std::endl;
  std::cout << "whole version: " << Version::GetVersion() << std::endl;
  std::cout << "version str: " << Version::GetVersionStr() << std::endl;
}
void TestToolDevice::SetLoggerSeverity() {
  std::cout << std::endl;
  std::cout << "----------------------" << print_title_ << " set log severity"
            << "----------------------" << std::endl;
  std::cout << "input severity: " << std::endl;
  std::cout << "debug: 0" << std::endl;
  std::cout << "information: 1" << std::endl;
  std::cout << "warning: 2" << std::endl;
  std::cout << "error: 3" << std::endl;
  std::cout << "fatal error: 4" << std::endl;
  std::cout << "off (close log): 5" << std::endl;
  int severity;
  std::cin >> severity;
  dt::Logger::SetLoggerSeverity((DtLogSeverity) severity);
}

void TestToolDevice::SetLoggerToFile() {
  try {
    std::cout << std::endl;
    std::cout << "----------------------" << print_title_ << " set log to file"
              << "----------------------" << std::endl;
    std::string log_path = "";
    int severity;
    std::cout << "set path for saving log: ";
    std::cin >> log_path;
    std::cout << "set log serverity: " << std::endl;
    std::cout << "debug: 0" << std::endl;
    std::cout << "information: 1" << std::endl;
    std::cout << "warning: 2" << std::endl;
    std::cout << "error: 3" << std::endl;
    std::cout << "fatal error: 4" << std::endl;
    std::cout << "off (close log): 5" << std::endl;
    std::cin >> severity;
    dt::Logger::SetLoggerToFile((DtLogSeverity) severity, log_path.c_str());
  } catch (dt::Error& e) {
    std::cerr << "function:" << e.GetFunction() << "\nargs:" << e.GetArgs()
              << "\nmessage:" << e.What() << "\ntype:" << e.GetExceptionType() << std::endl;
    std::cout << "\nPress any key to exit.";
    dt_utils::WaitForKeyPressed();
    exit(EXIT_FAILURE);
  }
}
void TestToolDevice::GetSensorList() {
  try {
    std::cout << std::endl;
    std::cout << "----------------------" << print_title_ << " get sensor list info"
              << "----------------------" << std::endl;
    std::shared_ptr<SensorList> sensor_list = device_->GetSensorList();
    int count = sensor_list->GetCount();
    std::cout << "sensor list count: " << count << std::endl;
    for (int i = 0; i < count; i++) {
      DtSensorType type{DtSensorType::kDtSensorUnknown};
      sensor_list->GetSensorType(i);
      std::cout << "sensor " << i << " type: " << type << std::endl;
    }
    std::cout
        << "you can use sensor type or index to get single sensor info(0: by index, 1: by type): "
        << std::endl;
    int type_or_index;
    std::cin >> type_or_index;
    std::shared_ptr<Sensor> sensor;
    if (type_or_index == 0) {
      std::cout << "input sensor index: " << std::endl;
      int index;
      std::cin >> index;
      sensor = sensor_list->GetSensor(index);
    } else {
      std::cout << "input sensor type: " << std::endl;
      std::cout << "SENSOR_IR = 1,        " << std::endl;
      std::cout << "SENSOR_COLOR = 2,     " << std::endl;
      std::cout << "SENSOR_DEPTH = 3,     " << std::endl;
      std::cout << "SENSOR_ACCEL = 4,     " << std::endl;
      std::cout << "SENSOR_GYRO = 5,      " << std::endl;
      std::cout << "SENSOR_IR_LEFT = 6,   " << std::endl;
      std::cout << "SENSOR_IR_RIGHT = 7,  " << std::endl;
      std::cout << "SENSOR_RAW_PHASE = 8, " << std::endl;
      int type;
      std::cin >> type;
      sensor = sensor_list->GetSensor((DtSensorType) type);
    }
    PrintSensorInfo(sensor);
  } catch (dt::Error& e) {
    std::cerr << "function:" << e.GetFunction() << "\nargs:" << e.GetArgs()
              << "\nmessage:" << e.What() << "\ntype:" << e.GetExceptionType() << std::endl;
    std::cout << "\nPress any key to exit.";
    dt_utils::WaitForKeyPressed();
    exit(EXIT_FAILURE);
  }
}
void TestToolDevice::GetSensorByType() {
  std::cout << std::endl;
  std::cout << "----------------------" << print_title_ << " get sensor by type"
            << "----------------------" << std::endl;
  std::cout << "SENSOR_IR = 1,        " << std::endl;
  std::cout << "SENSOR_COLOR = 2,     " << std::endl;
  std::cout << "SENSOR_DEPTH = 3,     " << std::endl;
  std::cout << "SENSOR_ACCEL = 4,     " << std::endl;
  std::cout << "SENSOR_GYRO = 5,      " << std::endl;
  std::cout << "SENSOR_IR_LEFT = 6,   " << std::endl;
  std::cout << "SENSOR_IR_RIGHT = 7,  " << std::endl;
  std::cout << "SENSOR_RAW_PHASE = 8, " << std::endl;
  std::cout << "input sensor type: ";
  int type;
  std::cin >> type;
  std::shared_ptr<Sensor> sensor = device_->GetSensor((DtSensorType) type);
  PrintSensorInfo(sensor);
}

void TestToolDevice::WriteFlash() {
  std::cout << std::endl;
  std::cout << "----------------------" << print_title_ << " write flash"
            << "----------------------" << std::endl;

  std::cout << "Input write address offset: eg: 0x1f00" << std::endl;
  std::string hex_offset;
  std::cin >> hex_offset;
  uint32_t offset = std::stoul(hex_offset, nullptr, 16);

  std::cout << "Input write data: " << std::endl;
  std::string data;
  std::cin >> data;

  try {
    device_->WriteFlash(
        offset,
        data.c_str(),
        data.size(),
        [](DtDataTranState state, uint8_t percent) {
          std::cout << "DeviceDataTransferCallback: state: " << static_cast<int>(state)
                    << " percent: " << static_cast<int>(percent) << std::endl;
        },
        false);

    std::cout << "write flash success" << std::endl;
  } catch (dt::Error& e) {
    std::cerr << "function:" << e.GetFunction() << "\nargs:" << e.GetArgs()
              << "\nmessage:" << e.What() << "\ntype:" << e.GetExceptionType() << std::endl;
    std::cout << "\nPress any key to exit.";
    dt_utils::WaitForKeyPressed();
  }
}

void TestToolDevice::ReadFlash() {
  std::cout << std::endl;
  std::cout << "----------------------" << print_title_ << " read flash"
            << "----------------------" << std::endl;

  // change to std::hex
  std::cout << "Input read address offset: eg: 0x1f00" << std::endl;
  std::string hex_offset;
  std::cin >> hex_offset;
  uint32_t offset = std::stoul(hex_offset, nullptr, 16);

  std::cout << "Input read data size: " << std::endl;
  uint32_t data_size;
  std::cin >> data_size;

  try {
    device_->ReadFlash(
        offset,
        data_size,
        [](DtDataTranState state, DtDataChunk* data_chunk) {
          std::cout << "DeviceDataTransferCallback: state: " << static_cast<int>(state)
                    << std::endl;
          if (data_chunk != nullptr) {
            std::string data(reinterpret_cast<char*>(data_chunk->data), data_chunk->size);
            std::cout << "data size: " << data_chunk->size << std::endl;
            std::cout << "data: " << data << std::endl;
          }
        },
        false);

    std::cout << "read flash success" << std::endl;
  } catch (dt::Error& e) {
    std::cerr << "function:" << e.GetFunction() << "\nargs:" << e.GetArgs()
              << "\nmessage:" << e.What() << "\ntype:" << e.GetExceptionType() << std::endl;
    std::cout << "\nPress any key to exit.";
    dt_utils::WaitForKeyPressed();
  }
}

void TestToolDevice::WriteEeprom() {
  std::cout << std::endl;
  std::cout << "----------------------" << print_title_ << " write Eeprom"
            << "----------------------" << std::endl;
  std::cout << "unsupported !" << std::endl;
}

void TestToolDevice::ReadEeprom() {
  std::cout << std::endl;
  std::cout << "----------------------" << print_title_ << " read Eeprom"
            << "----------------------" << std::endl;
  std::cout << "unsupported !" << std::endl;
}

void TestToolDevice::SendFileToDevice() {
  try {
    std::cout << std::endl;
    std::cout << "----------------------" << print_title_ << "Send file to device"
              << "----------------------" << std::endl;
    std::cout << "enter the host file path: ";
    std::string src_file_path;
    std::cin >> src_file_path;
    std::cout << "enter the device file path: ";
    std::string dst_file_path;
    std::cin >> dst_file_path;
    std::cout << std::endl;
    std::cout << "whether to use async transfer: [0]:no, [1]:yes" << std::endl;
    int async;
    std::cin >> async;
    TimePoint time_start = TIMESTART;
    Device::FileTransferCallback transfer_callback =
        [](DtFileTranState state, const char* message, uint8_t percent) {
          std::cout << "DeviceFileTransferCallback: state: " << state << " message: " << message
                    << " percent: " << static_cast<int>(percent) << std::endl;
        };
    device_->SendFileToDevice(src_file_path.c_str(),
                              dst_file_path.c_str(),
                              transfer_callback,
                              async);
    double elapsed_time = TIMEELAPSED(time_start);
    std::cout << "TestToolDevice::SendFileToDevice " << elapsed_time << std::endl;
  } catch (dt::Error& e) {
    std::cerr << "function:" << e.GetFunction() << "\nargs:" << e.GetArgs()
              << "\nmessage:" << e.What() << "\ntype:" << e.GetExceptionType() << std::endl;
    std::cout << "\nPress any key to exit.";
  }
}

void TestToolDevice::ReceiveFileFromDevice() {
  try {
    std::cout << std::endl;
    std::cout << "----------------------" << print_title_ << "Receive file from device"
              << "----------------------" << std::endl;
    std::cout << "enter the device file path: ";
    std::string src_file_path;
    std::cin >> src_file_path;
    std::cout << "enter the host file path: ";
    std::string dst_file_path;
    std::cin >> dst_file_path;
    std::cout << std::endl;
    std::cout << "whether to use async transfer: [0]:no, [1]:yes" << std::endl;
    int async;
    std::cin >> async;
    TimePoint time_start = TIMESTART;
    Device::FileTransferCallback transfer_callback =
        [](DtFileTranState state, const char* message, uint8_t percent) {
          std::cout << "DeviceFileTransferCallback: state: " << state << " message: " << message
                    << " percent: " << static_cast<int>(percent) << std::endl;
        };
    device_->ReceiveFileFromDevice(src_file_path.c_str(),
                                   dst_file_path.c_str(),
                                   transfer_callback,
                                   async);
    double elapsed_time = TIMEELAPSED(time_start);
    std::cout << "TestToolDevice::receiveFileFromDevice " << elapsed_time << std::endl;
  } catch (dt::Error& e) {
    std::cerr << "function:" << e.GetFunction() << "\nargs:" << e.GetArgs()
              << "\nmessage:" << e.What() << "\ntype:" << e.GetExceptionType() << std::endl;
    std::cout << "\nPress any key to exit.";
    dt_utils::WaitForKeyPressed();
    exit(EXIT_FAILURE);
  }
}

void TestToolDevice::UpdateFirmware() {
  try {
    std::cout << std::endl;
    std::cout << "----------------------" << print_title_ << " device upgrade"
              << "----------------------" << std::endl;
    std::cout << "[0]:stress test, [1]: interface test" << std::endl;
    int choice;
    std::cin >> choice;
    if (choice) {
      std::cout << "enter the path of update file: ";
      std::string update_file_path;
      std::cin >> update_file_path;
      std::cout << std::endl;
      std::cout << "whether to use async upgrade: [0]:no, [1]:yes" << std::endl;
      int async_upgrade;
      std::cin >> async_upgrade;
      TimePoint time_start = TIMESTART;
      Device::DeviceFwUpdateCallback upgrade_callback =
          [](DtFwUpdateState state, const char* message, uint8_t percent) {
            std::cout << "DeviceFwUpdateCallback: state: " << state << " message: " << message
                      << " percent: " << static_cast<int>(percent) << std::endl;
          };
      device_->UpdateFirmware(update_file_path.c_str(), upgrade_callback, async_upgrade);
      double elapsed_time = TIMEELAPSED(time_start);
      std::cout << "TestToolDevice::Upgrade " << elapsed_time << std::endl;
    } else {
      int number;
      std::cout << "input upgrade number: " << std::endl;
      std::cin >> number;
      std::string update_file_path1, update_file_path2;
      std::cout << "enter the path of update file 1: " << std::endl;
      std::cin >> update_file_path1;
      std::cout << "enter the path of update file 2: " << std::endl;
      std::cin >> update_file_path2;
      std::cout << std::endl;
      std::cout << "whether to use async upgrade: [0]:no, [1]:yes" << std::endl;
      int async_upgrade;
      std::cin >> async_upgrade;
      Device::DeviceFwUpdateCallback upgrade_callback =
          [&](DtFwUpdateState state, const char* message, uint8_t percent) {
            std::cout << "DeviceFwUpdateCallback: state: " << state << " message: " << message
                      << " percent: " << (int) percent << std::endl;
            if (state == DtFwUpdateState::kDtStatDone) {
              std::lock_guard<std::mutex> lock(mutex_);
              upgrade_event_triggered_ = true;
              cv_.notify_all();
              std::cout << "device upgrade done!" << std::endl;
            }
          };
      device_->UpdateFirmware(update_file_path1.c_str(), upgrade_callback, async_upgrade);
      std::cout << "start upgrade_thread!" << std::endl;
      std::future<void> upgrade_future_ = std::async(std::launch::async, [=]() {
        for (int i = 1; i < number; i++) {
          std::string file_name;
          if (i % 2) {
            file_name = update_file_path2;
          } else {
            file_name = update_file_path1;
          }
          std::cout << "file_name" << file_name << std::endl;
          std::unique_lock<std::mutex> lock(mutex_);
          cv_.wait(lock, [this]() { return upgrade_event_triggered_; });
          std::this_thread::sleep_for(std::chrono::seconds(10));
          device_->UpdateFirmware(file_name.c_str(), upgrade_callback, async_upgrade);
          upgrade_event_triggered_ = false;
          std::cout << "upgrade times: " << i + 1 << std::endl;
        }
        std::cout << "Upgrade succeed!" << std::endl;
      });
      upgrade_future_.get();
    }
  } catch (dt::Error& e) {
    std::cerr << "function:" << e.GetFunction() << "\nargs:" << e.GetArgs()
              << "\nmessage:" << e.What() << "\ntype:" << e.GetExceptionType() << std::endl;
    std::cout << "\nPress any key to exit.";
    dt_utils::WaitForKeyPressed();
    exit(EXIT_FAILURE);
  }
}

void TestToolDevice::SetHeartbeat() {
  try {
    std::cout << std::endl;
    std::cout << "----------------------" << print_title_ << " set hearbeat"
              << "----------------------" << std::endl;
    std::cout << "whether to open heartbeat: 0: close, 1: open";
    bool enable;
    std::cin >> enable;
    device_->EnableHeartbeat(enable);
  } catch (dt::Error& e) {
    std::cerr << "function:" << e.GetFunction() << "\nargs:" << e.GetArgs()
              << "\nmessage:" << e.What() << "\ntype:" << e.GetExceptionType() << std::endl;
    std::cout << "\nPress any key to exit.";
    dt_utils::WaitForKeyPressed();
    exit(EXIT_FAILURE);
  }
}

void TestToolDevice::GetDeviceStatus() {
  try {
    std::cout << std::endl;
    std::cout << "----------------------" << print_title_ << " get device status"
              << "----------------------" << std::endl;
    std::cout << "device state: " << device_->GetDeviceState() << std::endl;
  } catch (dt::Error& e) {
    std::cerr << "function:" << e.GetFunction() << "\nargs:" << e.GetArgs()
              << "\nmessage:" << e.What() << "\ntype:" << e.GetExceptionType() << std::endl;
    std::cout << "\nPress any key to exit.";
    dt_utils::WaitForKeyPressed();
    exit(EXIT_FAILURE);
  }
}

void TestToolDevice::GetCameraParameters() {
  try {
    std::cout << std::endl;
    std::cout << "----------------------" << print_title_ << " get camera parameter"
              << "----------------------" << std::endl;
    std::shared_ptr<CameraParamList> camera_list = device_->GetCalibrationCameraParamList();
    for (uint32_t i = 0; i < camera_list->GetCount(); ++i) {
      auto camera_param = camera_list->GetCameraParam(i);

      const DtCameraIntrinsic& depth = camera_param.depth_intrinsic;
      const DtCameraIntrinsic& rgb = camera_param.rgb_intrinsic;
      const DtCameraDistortion& depth_dist = camera_param.depth_distortion;
      const DtCameraDistortion& rgb_dist = camera_param.rgb_distortion;
      const DtD2CTransform& trans = camera_param.transform;

      std::cout << "------- Camera Param [" << i << "] -------\n";

      std::cout << "[Depth Intrinsic] fx: " << depth.fx << ", fy: " << depth.fy
                << ", cx: " << depth.cx << ", cy: " << depth.cy << ", width: " << depth.width
                << ", height: " << depth.height << "\n";

      std::cout << "[RGB Intrinsic] fx: " << rgb.fx << ", fy: " << rgb.fy << ", cx: " << rgb.cx
                << ", cy: " << rgb.cy << ", width: " << rgb.width << ", height: " << rgb.height
                << "\n";

      std::cout << "[Depth Distortion] k1: " << depth_dist.k1 << ", k2: " << depth_dist.k2
                << ", k3: " << depth_dist.k3 << ", k4: " << depth_dist.k4
                << ", k5: " << depth_dist.k5 << ", k6: " << depth_dist.k6
                << ", p1: " << depth_dist.p1 << ", p2: " << depth_dist.p2
                << ", model: " << static_cast<int>(depth_dist.model) << "\n";

      std::cout << "[RGB Distortion] k1: " << rgb_dist.k1 << ", k2: " << rgb_dist.k2
                << ", k3: " << rgb_dist.k3 << ", k4: " << rgb_dist.k4 << ", k5: " << rgb_dist.k5
                << ", k6: " << rgb_dist.k6 << ", p1: " << rgb_dist.p1 << ", p2: " << rgb_dist.p2
                << ", model: " << static_cast<int>(rgb_dist.model) << "\n";

      std::cout << "[Transform] rot: ";
      for (int j = 0; j < 9; ++j) {
        std::cout << trans.rot[j] << " ";
      }
      std::cout << "\n";

      std::cout << "            trans: ";
      for (int j = 0; j < 3; ++j) {
        std::cout << trans.trans[j] << " ";
      }
      std::cout << "\n";

      std::cout << "isMirrored: " << (camera_param.is_mirrored ? "true" : "false") << "\n";
      std::cout << "--------------------------------------\n\n";
    }
  } catch (dt::Error& e) {
    std::cerr << "function:" << e.GetFunction() << "\nargs:" << e.GetArgs()
              << "\nmessage:" << e.What() << "\ntype:" << e.GetExceptionType() << std::endl;
    std::cout << "\nPress any key to exit.";
    dt_utils::WaitForKeyPressed();
    exit(EXIT_FAILURE);
  }
}

void TestToolDevice::SetDeviceSerialNumber() try {
  std::cout << "input serial number : " << std::endl;
  std::string serial;
  std::cin >> serial;

  if (device_->IsOptionSupported(kDtOptionDeviceSerialNumberStruct, kDtPermissionWrite)) {
    DtSerialNumber serial_number;
    memset(&serial_number, 0, sizeof(DtSerialNumber));
    std::memcpy(serial_number.number_str, serial.c_str(), sizeof(serial_number.number_str));
    device_->SetStructuredData(kDtOptionDeviceSerialNumberStruct,
                               reinterpret_cast<const uint8_t*>(&serial_number),
                               sizeof(DtSerialNumber));

    std::cout << "set serial number success" << std::endl;
  }
} catch (dt::Error& e) {
  std::cerr << e << std::endl;
  std::cout << "\nPress any key to exit.";
  dt_utils::WaitForKeyPressed();
}

void TestToolDevice::GetDeviceSerialNumber() try {
  if (device_->IsOptionSupported(kDtOptionDeviceSerialNumberStruct, kDtPermissionRead)) {
    DtSerialNumber serial_number;
    uint32_t data_size = sizeof(DtSerialNumber);
    device_->GetStructuredData(kDtOptionDeviceSerialNumberStruct,
                               reinterpret_cast<uint8_t*>(&serial_number),
                               &data_size);

    std::cout << "serial number: " << serial_number.number_str << std::endl;
  }
} catch (dt::Error& e) {
  std::cerr << e << std::endl;
  std::cout << "\nPress any key to exit.";
  dt_utils::WaitForKeyPressed();
}

void TestToolDevice::DeviceAeParams() try {
  if (device_->IsOptionSupported(kDtOptionDepthAeParamStruct, kDtPermissionReadWrite)) {
    std::vector<float> ae_params;
    int ae_params_size = 100;
    ae_params.resize(ae_params_size);
    uint32_t data_size = ae_params_size * sizeof(float);
    device_->GetStructuredData(kDtOptionDepthAeParamStruct,
                               reinterpret_cast<uint8_t*>(ae_params.data()),
                               &data_size);

    for (size_t i = 0; i < data_size / sizeof(float); i++) {
      std::cout << "ae_params[" << i << "] = " << ae_params[i] << std::endl;
    }

    device_->SetStructuredData(kDtOptionDepthAeParamStruct,
                               reinterpret_cast<uint8_t*>(ae_params.data()),
                               data_size);
  }

} catch (dt::Error& e) {
  std::cerr << e << std::endl;
  std::cout << "\nPress any key to exit.";
  dt_utils::WaitForKeyPressed();
}

void TestToolDevice::DeviceExposureStruct() try {
  if (device_->IsOptionSupported(kDtOptionDepthExposureStruct, kDtPermissionReadWrite)) {
    std::vector<uint16_t> exposure_params;
    int exposure_params_size = 100;
    exposure_params.resize(exposure_params_size);
    uint32_t data_size = exposure_params_size * sizeof(uint16_t);
    device_->GetStructuredData(kDtOptionDepthExposureStruct,
                               reinterpret_cast<uint8_t*>(exposure_params.data()),
                               &data_size);

    for (size_t i = 0; i < data_size / sizeof(uint16_t); i++) {
      std::cout << "exposure_params[" << i << "] = " << exposure_params[i] << std::endl;
    }

    device_->SetStructuredData(kDtOptionDepthExposureStruct,
                               reinterpret_cast<uint8_t*>(exposure_params.data()),
                               data_size);
  }

} catch (dt::Error& e) {
  std::cerr << e << std::endl;
  std::cout << "\nPress any key to exit.";
  dt_utils::WaitForKeyPressed();
}

void TestToolDevice::Reboot() {
  try {
    std::cout << std::endl;
    std::cout << "----------------------" << print_title_ << " reboot"
              << "----------------------" << std::endl;
    long reboot_loop_times = 0;
    long total_reboot_loop_times = 0;
    long reboot_wait_times = 0;
    long current_time = 0;
    std::cout << "input reboot time:" << std::endl;
    std::cin >> reboot_loop_times;
    std::cout << "input reboot wait time:" << std::endl;
    std::cin >> reboot_wait_times;
    total_reboot_loop_times = reboot_loop_times;
    TimePoint time_start = TIMESTART;
    while (1) {
      TestToolDeviceManage::reboot_flag_ = false;
      std::cout << "reboot_loop_times: " << reboot_loop_times << std::endl;
      if (reboot_loop_times == -1) {
      } else {
        if (reboot_loop_times <= 0)
          break;
        reboot_loop_times--;
        current_time++;
        std::cout << "current reboot times: " << current_time << std::endl;
      }
      StopStream();
      device_->Reboot();
      // wait for device reboot complete
      while (1) {
        if (TestToolDeviceManage::reboot_flag_ == true) {
          std::this_thread::sleep_for(std::chrono::seconds(reboot_wait_times));
          break;
        } else {
          std::cout << "wait for device connect..." << std::endl;
          std::this_thread::sleep_for(std::chrono::milliseconds(500));
          continue;
        }
      }
    }
    if (reboot_loop_times == 0) {
      double elapsed_time = TIMEELAPSED(time_start);
      std::cout << "Reboot average time " << elapsed_time / total_reboot_loop_times << " ms"
                << std::endl;
    }
  } catch (dt::Error& e) {
    std::cerr << "function:" << e.GetFunction() << "\nargs:" << e.GetArgs()
              << "\nmessage:" << e.What() << "\ntype:" << e.GetExceptionType() << std::endl;
    std::cout << "\nPress any key to exit.";
    dt_utils::WaitForKeyPressed();
    exit(EXIT_FAILURE);
  }
}
void TestToolDevice::WriteLicense() {
  std::cout << std::endl;
  std::cout << "----------------------" << print_title_ << " write license"
            << "----------------------" << std::endl;
  std::cout << "unsupported !" << std::endl;

  // std::string license_path;
  // OpaqueData<uint8_t> file_content;
  // std::cout << "input License path: " << std::endl;
  // std::cin >> license_path;
  // int ret = ReadBinFile(license_path, file_content);
  // CHECK_SDK_RETURN_VALUE(ret);
  // ret = device_->WriteLicense(file_content);
  // CHECK_SDK_RETURN_VALUE(ret);
}
void TestToolDevice::ReadLicense() {
  std::cout << std::endl;
  std::cout << "----------------------" << print_title_ << " read license"
            << "----------------------" << std::endl;
  std::cout << "unsupported !" << std::endl;
  // OpaqueData<uint8_t> license_data;
  // int ret = device_->ReadLicense(license_data);
  // CHECK_SDK_RETURN_VALUE(ret);
  // std::cout << "license data size: " << license_data.data_len << std::endl;
  // if (license_data.data_len > 0) {
  //   std::string license_tmp(reinterpret_cast<const char*>(license_data.data.get()),
  //                           license_data.data_len);
  //   std::cout << "license data : " << license_tmp << std::endl;
  // }
  // return ret;
}

void TestToolDevice::GetOption() {
  try {
    std::cout << std::endl;
    std::cout << "----------------------" << print_title_ << " get option value"
              << "----------------------" << std::endl;
    std::cout << "input option index : " << std::endl;
    int option_idx;
    std::cin >> option_idx;

    if (static_cast<size_t>(option_idx) >= properties_.size()) {
      std::cout << "Your selection is out of range, please reselect: " << std::endl;
      return;
    }

    auto item = properties_.at(option_idx);
    DtOptionItem option_item = id_option_map_[item.id];
    if (option_item.permission == kDtPermissionRead ||
        option_item.permission == kDtPermissionReadWrite ||
        option_item.permission == kDtPermissionAny) {
      switch (option_item.type) {
        case kDtOptionBool: {
          bool bool_value = device_->GetBoolOption(item.id);
          std::cout << "value: " << bool_value << std::endl;
          break;
        }
        case kDtOptionInt: {
          int int_value = device_->GetIntOption(item.id);
          std::cout << "value: " << int_value << std::endl;
          break;
        }
        case kDtOptionFloat: {
          float float_value = device_->GetFloatOption(item.id);
          std::cout << "value: " << float_value << std::endl;
          break;
        }
        case kDtOptionStruct: {
          if (item.id == kDtOptionDepthAeParamStruct) {
            DeviceAeParams();
          } else if (item.id == kDtOptionDepthExposureStruct) {
            DeviceExposureStruct();
          } else if (item.id == kDtOptionDeviceSerialNumberStruct) {
            uint32_t size = 64;
            std::string serial_number(size, '\0');
            device_->GetStructuredData(item.id,
                                       reinterpret_cast<uint8_t*>(&serial_number[0]),
                                       &size);
            serial_number.resize(std::strlen(serial_number.c_str()));
            std::cout << serial_number.c_str();
          } else if (item.id == kDtOptionSensorFreqRangeStruct) {
            DtNebulaSensorFreq sensor_freq;
            uint32_t size = sizeof(sensor_freq);
            device_->GetStructuredData(item.id, reinterpret_cast<uint8_t*>(&sensor_freq), &size);
            std::cout << "mtof high freq: " << sensor_freq.mtof_high_freq << std::endl;
            std::cout << "mtof low freq: " << sensor_freq.mtof_low_freq << std::endl;
            std::cout << "stof high freq: " << sensor_freq.stof_high_freq << std::endl;
            std::cout << "stof low freq: " << sensor_freq.stof_low_freq << std::endl;
          } else if (item.id == kDtOptionMtofDepthCropRoiStruct) {
            uint8_t crop_roi[4];
            uint32_t size = sizeof(crop_roi);
            device_->GetStructuredData(item.id, reinterpret_cast<uint8_t*>(&crop_roi[0]), &size);
            std::cout << "mtof crop_roi[0]: " << static_cast<int>(crop_roi[0]) << std::endl;
            std::cout << "mtof crop_roi[1]: " << static_cast<int>(crop_roi[1]) << std::endl;
            std::cout << "mtof crop_roi[2]: " << static_cast<int>(crop_roi[2]) << std::endl;
            std::cout << "mtof crop_roi[3]: " << static_cast<int>(crop_roi[3]) << std::endl;
          } else if (item.id == kDtOptionStofDepthCropRoiStruct) {
            uint8_t crop_roi[4];
            uint32_t size = sizeof(crop_roi);
            device_->GetStructuredData(item.id, reinterpret_cast<uint8_t*>(&crop_roi[0]), &size);
            std::cout << "stof crop_roi[0]: " << static_cast<int>(crop_roi[0]) << std::endl;
            std::cout << "stof crop_roi[1]: " << static_cast<int>(crop_roi[1]) << std::endl;
            std::cout << "stof crop_roi[2]: " << static_cast<int>(crop_roi[2]) << std::endl;
            std::cout << "stof crop_roi[3]: " << static_cast<int>(crop_roi[3]) << std::endl;
          } else if (item.id == kDtOptionDeviceIpAddrConfigStruct) {
            GetNetIpConfig(item.id);
          }
          break;
        }
        default:
          break;
      }
    } else {
      std::cout << "permission is not supported" << std::endl;
    }
  } catch (dt::Error& e) {
    std::cerr << "function:" << e.GetFunction() << "\nargs:" << e.GetArgs()
              << "\nmessage:" << e.What() << "\ntype:" << e.GetExceptionType() << std::endl;
    std::cout << "\nPress any key to exit.";
    dt_utils::WaitForKeyPressed();
    exit(EXIT_FAILURE);
  }
}

void TestToolDevice::SetOption() {
  try {
    std::cout << std::endl;
    std::cout << "----------------------" << print_title_ << " set option value"
              << "----------------------" << std::endl;
    std::cout << "input option idx : " << std::endl;
    int option_idx;
    std::cin >> option_idx;

    if (static_cast<size_t>(option_idx) >= properties_.size()) {
      std::cout << "Your selection is out of range, please reselect: " << std::endl;
      return;
    }

    auto item = properties_.at(option_idx);
    DtOptionItem option_item = id_option_map_[item.id];
    if (option_item.permission == kDtPermissionWrite ||
        option_item.permission == kDtPermissionReadWrite ||
        option_item.permission == kDtPermissionAny) {
      switch (option_item.type) {
        case kDtOptionBool: {
          bool bool_value;
          std::cout << "input bool value : " << std::endl;
          std::cin >> bool_value;
          device_->SetBoolOption(item.id, bool_value);
          break;
        }
        case kDtOptionInt: {
          int int_value;
          std::cout << "input int value : " << std::endl;
          std::cin >> int_value;
          device_->SetIntOption(item.id, int_value);
          break;
        }
        case kDtOptionFloat: {
          float float_value;
          std::cout << "input float value : " << std::endl;
          std::cin >> float_value;
          device_->SetFloatOption(item.id, float_value);
          break;
        }
        case kDtOptionStruct: {
          switch (item.id) {
            case kDtOptionDeviceSerialNumberStruct: {
              std::cout << "input the serial number : ";
              std::string serial_number;
              std::cin >> serial_number;
              device_->SetStructuredData(item.id,
                                         reinterpret_cast<const uint8_t*>(serial_number.c_str()),
                                         serial_number.size());
            } break;
            case kDtOptionStofDepthCropRoiStruct:
            case kDtOptionMtofDepthCropRoiStruct:
            case kDtOptionDimPalmIrRecognizeRectBoundaryStruct:
            case kDtOptionDimPalmIrRegisterRectBoundaryStruct: {
              SetRoiStruct(item.id);
            } break;
            case kDtOptionDimPalmIrRegisterIntensityRangeStruct:
            case kDtOptionDimPalmIrRecognizeQualityRangeStruct:
            case kDtOptionDimPalmIrRegisterQualityRangeStruct:
            case kDtOptionDimPalmIrLivenessRangeStruct:
            case kDtOptionDimPalmIrRecognizeReliabilityRangeStruct:
            case kDtOptionDimPalmIrRegisterReliabilityRangeStruct:
            case kDtOptionDimPalmIrRecognizeIntensityRangeStruct: {
              SetIntensityRangeStruct(item.id);
            } break;
            case kDtOptionDeviceIpAddrConfigStruct: {
              SetNetIpConfig(item.id);
            } break;
            default:
              break;
          }
          default:
            break;
        }
      }
    } else {
      std::cout << "permission is not supported" << std::endl;
    }
  } catch (dt::Error& e) {
    std::cerr << "function:" << e.GetFunction() << "\nargs:" << e.GetArgs()
              << "\nmessage:" << e.What() << "\ntype:" << e.GetExceptionType() << std::endl;
    std::cout << "\nPress any key to exit.";
    dt_utils::WaitForKeyPressed();
    exit(EXIT_FAILURE);
  }
}

void TestToolDevice::IsOptionSupported() {
  std::cout << std::endl;
  std::cout << "----------------------" << print_title_ << " is option supported"
            << "----------------------" << std::endl;
  int count = properties_.size();
  std::cout << "input option id to check(option id's arrange (0-" << count - 1
            << ")): " << std::endl;
  int option_idx;
  std::cin >> option_idx;
  if (option_idx >= count) {
    std::cout << "Your selection is out of range, please reselect: " << std::endl;
    return;
  }

  auto option_item = properties_.at(option_idx);

  std::cout << "input permission to check: " << std::endl;
  std::cout << "0: PERMISSION_DENY, 1: PERMISSION_READ, 2: PERMISSION_WRITE, 3: "
               "PERMISSION_READ_WRITE, 255: PERMISSION_ANY"
            << std::endl;
  int permission;
  std::cin >> permission;
  bool is_supported = device_->IsOptionSupported(option_item.id, (DtPermissionType) permission);
  std::cout << "is option supported(0: false, 1: true): " << is_supported << std::endl;
}

std::string PermissionTypeToString(DtPermissionType permission) {
  switch (permission) {
    case kDtPermissionRead:
      return "R/_";
    case kDtPermissionWrite:
      return "_/W";
    case kDtPermissionReadWrite:
      return "R/W";

    default:
      break;
  }
  return "_/_";
}
void TestToolDevice::GetSupportedProperties() {
  std::cout << std::endl;
  std::cout << "----------------------" << print_title_ << " get supported option list"
            << "----------------------" << std::endl;
  // for (const auto& option : properties_) {
  //   std::cout << "option id: " << option.id << std::endl;
  //   std::cout << "option name: " << option.name << std::endl;
  //   std::cout << "option type: " << option.type << std::endl;
  //   std::cout << "option permission: " << option.permission <<
  //   std::endl; SetColorBoldYellowText("-------------------");
  // }

  std::cout << "size: " << properties_.size() << std::endl;
  if (properties_.empty()) {
    std::cout << "No supported option!" << std::endl;
  }
  SetColorBoldYellowText("-------------------");
  for (size_t i = 0; i < properties_.size(); i++) {
    auto option_item = properties_[i];
    std::string str_range = "";

    DtIntOptionRange int_range;
    DtFloatOptionRange float_range;
    switch (option_item.type) {
      case kDtOptionBool:
        str_range = "Bool value(min:0, max:1, step:1)";
        break;
      case kDtOptionInt: {
        try {
          int_range = device_->GetIntOptionRange(option_item.id);
          str_range = "Int value(min:" + std::to_string(int_range.min) +
                      ", max:" + std::to_string(int_range.max) +
                      ", step:" + std::to_string(int_range.step) + ")";
        } catch (...) { std::cout << "get int option range failed." << std::endl; }
      } break;
      case kDtOptionFloat:
        try {
          float_range = device_->GetFloatOptionRange(option_item.id);
          str_range = "Float value(min:" + std::to_string(float_range.min) +
                      ", max:" + std::to_string(float_range.max) +
                      ", step:" + std::to_string(float_range.step) + ")";
        } catch (...) { std::cout << "get float option range failed." << std::endl; }
        break;
      case kDtOptionStruct:
        str_range = "Struct value";
        break;
      default:
        break;
    }

    std::cout.setf(std::ios::right);
    std::cout.fill('0');
    std::cout.width(2);
    std::cout << i << ". ";
    std::cout << option_item.name << "(" << (int) option_item.id << ")";
    std::cout << ", permission=" << PermissionTypeToString(option_item.permission)
              << ", range=" << str_range << std::endl;
  }
  SetColorBoldYellowText("-------------------");
}
void TestToolDevice::StartSensorStream(std::shared_ptr<Sensor> sensor) {
  std::cout << std::endl;
  std::cout << "----------------------" << print_title_ << " start sensor stream"
            << "----------------------" << std::endl;
  auto stream_profile_list = sensor->GetStreamProfileList();
  SetColorBoldYellowText("-------------------");
  SetColorBoldYellowText("Test stream profile list interface ");
  SetColorBoldYellowText("-------------------");
  PrintStreamProfileList(stream_profile_list);
  std::cout << "please input stream profile index: " << std::endl;
  int stream_profile_index = 0;
  std::cin >> stream_profile_index;
  std::shared_ptr<dt_utils::FrameRateHelper>
      frame_rate_helper = std::make_shared<dt_utils::FrameRateHelper>();
  long cnt = 0;
  std::shared_ptr<StreamProfile> stream_profile = stream_profile_list->GetProfile(
      stream_profile_index);
  // if (stream_profile->is<VideoStreamProfile>()) {
  //   auto viedeo_stream_profile = stream_profile->as<VideoStreamProfile>();
  //   win->resize(viedeo_stream_profile->width(), viedeo_stream_profile->height());
  // }
  sensor->Start(stream_profile,
                [this, cnt, frame_rate_helper](std::shared_ptr<Frame> frame) mutable {
                  if (is_print_frame_info_.load()) {
                    PrintFrameInfo(frame);
                  }

                  frame_rate_helper->RecordTimestamp();
                  if (is_print_fps_.load() && 0 == cnt++ % 10) {
                    std::cout << "fps: " << frame_rate_helper->GetFrameRate() << std::endl;
                  }

                  {
                    std::unique_lock<std::mutex> lock(mutex_frame_);
                    recv_frame_queue_.push(frame);
                    cv_frame_.notify_all();
                  }
                });
}
void TestToolDevice::StopSensorStream(std::shared_ptr<Sensor> sensor) {
  try {
    std::cout << std::endl;
    std::cout << "----------------------" << print_title_ << " stop sensor stream"
              << "----------------------" << std::endl;
    sensor->Stop();
  } catch (dt::Error& e) {
    std::cerr << "function:" << e.GetFunction() << "\nargs:" << e.GetArgs()
              << "\nmessage:" << e.What() << "\ntype:" << e.GetExceptionType() << std::endl;
    std::cout << "\nPress any key to exit.";
    dt_utils::WaitForKeyPressed();
    exit(EXIT_FAILURE);
  }
}
void TestToolDevice::GetSensorStreamProfileList(std::shared_ptr<Sensor> sensor) {
  try {
    std::cout << std::endl;
    std::cout << "----------------------" << print_title_ << " get sensor stream profile list"
              << "----------------------" << std::endl;
    auto stream_profile_list = sensor->GetStreamProfileList();
    PrintStreamProfileList(stream_profile_list);
  } catch (dt::Error& e) {
    std::cerr << "function:" << e.GetFunction() << "\nargs:" << e.GetArgs()
              << "\nmessage:" << e.What() << "\ntype:" << e.GetExceptionType() << std::endl;
    std::cout << "\nPress any key to exit.";
    dt_utils::WaitForKeyPressed();
    exit(EXIT_FAILURE);
  }
}
void TestToolDevice::GetSensorType(std::shared_ptr<Sensor> sensor) {
  try {
    std::cout << std::endl;
    std::cout << "----------------------" << print_title_ << " get sensor type"
              << "----------------------" << std::endl;
    std::cout << "sensor type: " << sensor->GetType() << std::endl;
  } catch (dt::Error& e) {
    std::cerr << "function:" << e.GetFunction() << "\nargs:" << e.GetArgs()
              << "\nmessage:" << e.What() << "\ntype:" << e.GetExceptionType() << std::endl;
    std::cout << "\nPress any key to exit.";
    dt_utils::WaitForKeyPressed();
    exit(EXIT_FAILURE);
  }
}
// int TestToolDevice::SetLedModeStressTest() {
//   // std::cout << std::endl;
//   // std::cout << "----------------------" << print_title_ << " set led mode stress test"
//   //           << "----------------------" << std::endl;
//   // std::cout << "input number of light changes" << std::endl;
//   // int ret = 0;
//   // int number = 0;
//   // std::cin >> number;

//   // std::thread led_thread([=]() {
//   //   int led_marco = 0;
//   //   int led_max_marco = LedMode::LED_MAX_MODE;
//   //   bool is_supported = false;
//   //   int ret = device_->IsOptionSupported(is_supported, PROP_LED_MODE_INT,
//   PERMISSION_WRITE);
//   //   CHECK_SDK_RETURN_VALUE(ret);
//   //   if (!is_supported) {
//   //     LOG_F(ERROR, "unsported option: PROP_LED_MODE_INT");
//   //     return ret;
//   //   }
//   //   for (int i = 0; i < number; i++) {
//   //     int ret = device_->SetOption(PROP_LED_MODE_INT, (LedMode) led_marco);
//   //     if (ret) {
//   //       LOG_F(ERROR, "TestToolDevice::SetLedMode failure! ret: 0x%x", ret);
//   //       break;
//   //     }
//   //     std::cout << "led mode: " << led_marco << " times: " << i + 1 << std::endl;
//   //     led_marco = ++led_marco % led_max_marco;
//   //     std::this_thread::sleep_for(std::chrono::seconds(2));
//   //   }
//   //   return ret;
//   // });

//   // led_thread.detach();
//   // return ret;
// }
void TestToolDevice::EnableFps() {
  std::cout << "Enable fps print" << std::endl;
  is_print_fps_.store(!is_print_fps_.load());
}
void TestToolDevice::EnablePrintFrameInfo() {
  std::cout << "Enable print frame info" << std::endl;
  is_print_frame_info_.store(!is_print_frame_info_.load());
}
// void TestToolDevice::WholeProcessStressTest() {
//   std::cout << std::endl;
//   std::cout << "----------------------" << print_title_ << "whole process stress test"
//             << "----------------------" << std::endl;
//   long stream_loop_times = 0;
//   long frame_loop_times = 0;
//   int number = 0;
//   std::cout << "input number of whole process" << std::endl;
//   std::cin >> number;
//   std::cout << "enter the num of stream loops: " << std::endl;
//   std::cin >> stream_loop_times;
//   std::cout << "enter the num of frames loops: " << std::endl;
//   std::cin >> frame_loop_times;
//   std::cout << "config current stream profile" << std::endl;
//   ConfigCurrentStreamConfig();
//   TestDeviceWholeProcessThread(number, stream_loop_times, frame_loop_times);
// }
void TestToolDevice::StreamProcessStressTest() {
  std::cout << std::endl;
  std::cout << "----------------------" << print_title_ << "stream process stress test"
            << "----------------------" << std::endl;
  long stream_loop_times = 0;
  long frame_loop_times = 0;
  std::cout << "enter the num of stream loops: " << std::endl;
  std::cin >> stream_loop_times;
  std::cout << "enter the num of frames loops: " << std::endl;
  std::cin >> frame_loop_times;
  std::cout << "config current stream profile" << std::endl;
  ConfigCurrentStreamConfig();
  while (1) {
    std::unique_lock<std::mutex> lk(mutex_);
    if (stream_loop_times == -1) {
    } else {
      if (stream_loop_times <= 0)
        break;
      stream_loop_times--;
    }
    std::cout << "stream loop times: " << stream_loop_times << std::endl;
    StartPipelineWithConfig();
    stream_process_thread_ = std::make_shared<std::thread>(&TestToolDevice::StreamProcess,
                                                           this,
                                                           frame_loop_times);
    if (stream_process_thread_->joinable()) {
      stream_process_thread_->join();
    }
  };
}
void TestToolDevice::GetCurrentStreamProfile() {
  try {
    std::cout << std::endl;
    std::cout << "----------------------" << print_title_ << "get enabled streamprofile list"
              << "----------------------" << std::endl;
    std::shared_ptr<StreamProfileList> stream_profile_list = pipeline_->GetConfig()
                                                                 ->GetEnabledStreamProfileList();
    PrintStreamProfileList(stream_profile_list);
  } catch (dt::Error& e) {
    std::cerr << "function:" << e.GetFunction() << "\nargs:" << e.GetArgs()
              << "\nmessage:" << e.What() << "\ntype:" << e.GetExceptionType() << std::endl;
    std::cout << "\nPress any key to exit.";
    dt_utils::WaitForKeyPressed();
    exit(EXIT_FAILURE);
  }
}
void TestToolDevice::StartPipeline() {
  try {
    std::cout << std::endl;
    std::cout << "----------------------" << print_title_ << "start pipeline"
              << "----------------------" << std::endl;
    pipeline_->Start();
    is_start_pipeline_ = true;
  } catch (dt::Error& e) {
    std::cerr << "function:" << e.GetFunction() << "\nargs:" << e.GetArgs()
              << "\nmessage:" << e.What() << "\ntype:" << e.GetExceptionType() << std::endl;
    std::cout << "\nPress any key to exit.";
    dt_utils::WaitForKeyPressed();
    exit(EXIT_FAILURE);
  }
}
void TestToolDevice::StartPipelineWithConfig() {
  try {
    std::cout << std::endl;
    std::cout << "----------------------" << print_title_ << "start with config"
              << "----------------------" << std::endl;
    pipeline_->Start(config_);
    is_start_pipeline_ = true;
  } catch (dt::Error& e) {
    std::cerr << "function:" << e.GetFunction() << "\nargs:" << e.GetArgs()
              << "\nmessage:" << e.What() << "\ntype:" << e.GetExceptionType() << std::endl;
    std::cout << "\nPress any key to exit.";
    dt_utils::WaitForKeyPressed();
    exit(EXIT_FAILURE);
  }
}
void TestToolDevice::StartPipelineWithCallback() try {
  std::cout << std::endl;
  std::cout << "----------------------" << print_title_ << "start with callback"
            << "----------------------" << std::endl;
  std::shared_ptr<dt_utils::FrameRateHelper>
      frame_rate_helper = std::make_shared<dt_utils::FrameRateHelper>();
  long cnt = 0;
  pipeline_->Start(config_,
                   [this, cnt, frame_rate_helper](std::shared_ptr<FrameSet> frame_set) mutable {
                     if (frame_set) {
                       int count = frame_set->GetCount();
                       for (int i = 0; i < count; i++) {
                         std::shared_ptr<Frame> frame = frame_set->GetFrameByIndex(i);
                         if (is_print_frame_info_.load()) {
                           PrintFrameInfo(frame);
                         }
                         frame_rate_helper->RecordTimestamp();
                         if (is_print_fps_.load() && 0 == cnt++ % 10) {
                           std::cout << "fps: " << frame_rate_helper->GetFrameRate() << std::endl;
                         }
                       }
                     }
                     {
                       std::unique_lock<std::mutex> lock(mutex_frame_);
                       recv_frame_queue_.push(frame_set);
                       cv_frame_.notify_all();
                     }
                   });
} catch (dt::Error& e) { std::cerr << e << std::endl; }

void TestToolDevice::StartStream() {
  std::cout << std::endl;
  std::cout << "----------------------" << print_title_ << "start stream"
            << "----------------------" << std::endl;
  if (!is_start_pipeline_.load()) {
    std::cout << "pipeline is not stated, please start pipeline first" << std::endl;
    return;
  }
  stream_process_thread_ = std::make_shared<std::thread>(&TestToolDevice::StreamProcess, this, -1);
  stream_process_thread_->detach();
}
void TestToolDevice::StopStream() {
  if (!stream_is_running_) {
    return;
  }
  try {
    std::cout << std::endl;
    std::cout << "----------------------" << print_title_ << "stop stream"
              << "----------------------" << std::endl;
    pipeline_->Stop();
    stream_is_running_ = false;
  } catch (dt::Error& e) {
    std::cerr << "function:" << e.GetFunction() << "\nargs:" << e.GetArgs()
              << "\nmessage:" << e.What() << "\ntype:" << e.GetExceptionType() << std::endl;
    std::cout << "\nPress any key to exit.";
    dt_utils::WaitForKeyPressed();
    exit(EXIT_FAILURE);
  }
}
void TestToolDevice::GetStreamProfileListBySensorType() {
  std::cout << std::endl;
  std::cout << "----------------------" << print_title_ << "get stream profilelist by sensor type "
            << "----------------------" << std::endl;
  std::cout << "input sensor type: " << std::endl;
  std::cout << "SENSOR_IR = 1,        " << std::endl;
  std::cout << "SENSOR_COLOR = 2,     " << std::endl;
  std::cout << "SENSOR_DEPTH = 3,     " << std::endl;
  std::cout << "SENSOR_ACCEL = 4,     " << std::endl;
  std::cout << "SENSOR_GYRO = 5,      " << std::endl;
  std::cout << "SENSOR_IR_LEFT = 6,   " << std::endl;
  std::cout << "SENSOR_IR_RIGHT = 7,  " << std::endl;
  std::cout << "SENSOR_RAW_PHASE = 8, " << std::endl;
  int sensor_type;
  std::cin >> sensor_type;
  std::shared_ptr<StreamProfileList> stream_profile_list = pipeline_->GetStreamProfileList(
      (DtSensorType) sensor_type);
  PrintStreamProfileList(stream_profile_list);
}
void TestToolDevice::GetCameraParametersByPipeline() {
  std::cout << std::endl;
  std::cout << "----------------------" << print_title_ << " get camera parameter"
            << "----------------------" << std::endl;
  std::cout << "unsupported !";
}

void TestToolDevice::ConfigCurrentStreamConfig() {
  std::cout << std::endl;
  std::cout << "----------------------" << print_title_ << " config command"
            << "----------------------" << std::endl;
  while (1) {
    SetBlueText("66  show print info again");
    SetBlueText("99  return last menu");
    SetBlueText("1  get current stream profile list ");
    SetBlueText("2  enable video stream ");
    SetBlueText("3  disable stream");
    SetBlueText("4  disable all stream");
    SetBlueText("5  enable video stream with profile");
    std::cout << std::endl;
    std::cout << "input your choice: ";
    int selection = 0;
    std::cin >> selection;
    if (std::cin.fail()) {
      std::cout << "what are you doing, entered an unknown num" << std::endl;
      std::cin.clear();
      std::cin.ignore();
      continue;
    }
    switch (selection) {
      case 66:
        break;
      case 99:
        return;
      case 1:
        GetEnabledConfigStreamProfileList();
        break;
      case 2:
        EnableVideoStream();
        break;
      case 3:
        DisableConfigStream();
        break;
      case 4:
        DisableConfigAllStream();
        break;
      case 5:
        EnableVideoStreamWithProfile();
        break;
      default:
        break;
    }
  }
}
void TestToolDevice::GetEnabledConfigStreamProfileList() {
  try {
    std::cout << std::endl;
    std::cout << "----------------------" << print_title_ << " get enabled stream profile list"
              << "----------------------" << std::endl;
    std::shared_ptr<StreamProfileList> stream_profile_list = config_->GetEnabledStreamProfileList();
    PrintStreamProfileList(stream_profile_list);
  } catch (dt::Error& e) {
    std::cerr << "function:" << e.GetFunction() << "\nargs:" << e.GetArgs()
              << "\nmessage:" << e.What() << "\ntype:" << e.GetExceptionType() << std::endl;
    std::cout << "\nPress any key to exit.";
    dt_utils::WaitForKeyPressed();
    exit(EXIT_FAILURE);
  }
}
void TestToolDevice::DisableConfigAllStream() {
  try {
    std::cout << std::endl;
    std::cout << "----------------------" << print_title_ << " disable all stream"
              << "----------------------" << std::endl;
    config_->DisableAllStream();
  } catch (dt::Error& e) {
    std::cerr << "function:" << e.GetFunction() << "\nargs:" << e.GetArgs()
              << "\nmessage:" << e.What() << "\ntype:" << e.GetExceptionType() << std::endl;
    std::cout << "\nPress any key to exit.";
    dt_utils::WaitForKeyPressed();
    exit(EXIT_FAILURE);
  }
}
void TestToolDevice::DisableConfigStream() {
  try {
    std::cout << std::endl;
    std::cout << "----------------------" << print_title_ << " disable stream"
              << "----------------------" << std::endl;
    std::shared_ptr<StreamProfileList> stream_profile_list = config_->GetEnabledStreamProfileList();
    PrintStreamProfileList(stream_profile_list);
    std::cout << std::endl;
    std::cout << "input stream index: ";
    int stream_index = 0;
    std::cin >> stream_index;
    std::shared_ptr<StreamProfile> stream_profile = stream_profile_list->GetProfile(stream_index);
    std::cout << "stream type: " << stream_profile->GetType() << std::endl;
    config_->DisableStream(stream_profile->GetType());
  } catch (dt::Error& e) {
    std::cerr << "function:" << e.GetFunction() << "\nargs:" << e.GetArgs()
              << "\nmessage:" << e.What() << "\ntype:" << e.GetExceptionType() << std::endl;
    std::cout << "\nPress any key to exit.";
    dt_utils::WaitForKeyPressed();
    exit(EXIT_FAILURE);
  }
}
void TestToolDevice::EnableVideoStream() {
  std::cout << std::endl;
  std::cout << "----------------------" << print_title_ << " enabled video stream"
            << "----------------------" << std::endl;
  while (1) {
    std::cout << "input stream type you want to add: " << std::endl;
    SetBlueText("66  show print info again");
    SetBlueText("99  return last menu");
    SetBlueText("0: video stream (include ir,color,depth stream) ");
    SetBlueText("1: ir stream ");
    SetBlueText("2: color stream");
    SetBlueText("3: depth stream");
    SetBlueText("8: raw phase stream");
    SetBlueText("11: palm stream");
    std::cout << std::endl;
    std::cout << "input your choice: ";
    int selection = 0;
    std::cin >> selection;
    if (std::cin.fail()) {
      std::cout << "what are you doing, entered an unknown num" << std::endl;
      std::cin.clear();
      std::cin.ignore();
      continue;
    }
    switch (selection) {
      case 66:
        break;
      case 99:
        return;
      case 0:
      case 1:
      case 2:
      case 3:
      case 8:
        config_->EnableVideoStream((DtStreamType) selection);
        break;
      case 11:
        config_->EnablePalmStream();
        break;
      default:
        std::cout << "input error, please input again" << std::endl;
        break;
    }
  }
}

void TestToolDevice::EnableVideoStreamWithProfile() {
  std::cout << std::endl;
  std::cout << "----------------------" << print_title_
            << " enabled video stream with stream profile"
            << "----------------------" << std::endl;
  while (1) {
    std::cout << "input sensor type you want to add: " << std::endl;
    SetBlueText("66  show print info again");
    SetBlueText("99  return last menu");
    SetBlueText("0: ir sensor ");
    SetBlueText("1: color sensor");
    SetBlueText("2: depth sensor");
    std::cout << std::endl;
    std::cout << "input your choice: ";
    int selection = 0;
    std::cin >> selection;
    if (std::cin.fail()) {
      std::cout << "what are you doing, entered an unknown num" << std::endl;
      std::cin.clear();
      std::cin.ignore();
      continue;
    }
    switch (selection) {
      case 66:
        break;
      case 99:
        return;
      case 0:
      case 1:
      case 2: {
        std::shared_ptr<StreamProfileList> stream_profile_list = pipeline_->GetStreamProfileList(
            (DtSensorType) selection);
        PrintStreamProfileList(stream_profile_list);
        std::cout << std::endl;
        std::cout << "input stream profile index: ";
        int stream_profile_index = 0;
        std::cin >> stream_profile_index;
        std::shared_ptr<StreamProfile> stream_profile = stream_profile_list->GetProfile(
            stream_profile_index);
        config_->EnableStream(stream_profile);
      } break;
      default:
        std::cout << "input error, please input again" << std::endl;
        break;
    }
  }
}

void TestToolDevice::PrintFrameInfo(std::shared_ptr<Frame> frame) {
  auto index = frame->GetIndex();
  if ((index % 50) == 2) {
    auto time_stamp_us = frame->GetTimeStampUs();
    auto frame_system_time_stamp_us = frame->GetSystemTimeStampUs();
    auto frame_type = frame->GetType();
    auto frame_format = frame->GetFormat();
    auto frame_data_size = frame->GetDataSize();
    std::shared_ptr<DeviceInfo> device_info = frame->GetDevice()->GetDeviceInfo();
    auto device_name = device_info->GetName();
    auto sensor_type = frame->GetSensor()->GetType();
    auto stream_profile = frame->GetStreamProfile();
    auto stream_profile_format = stream_profile->GetFormat();
    auto stream_type = stream_profile->GetType();
    uint32_t fps = 0;
    uint32_t width = 0;
    uint32_t height = 0;
    int16_t intrinsic_rows = 0;
    int16_t intrinsic_cols = 0;
    if (stream_profile->Is<VideoStreamProfile>()) {
      auto video_stream_profile = stream_profile->As<VideoStreamProfile>();
      fps = video_stream_profile->GetFps();
      width = video_stream_profile->GetWidth();
      height = video_stream_profile->GetWidth();
      auto intrinsic = video_stream_profile->GetIntrinsic();
      intrinsic_rows = intrinsic.height;
      intrinsic_cols = intrinsic.width;
    }
    std::cout << std::endl;
    SetColorBoldYellowText("-------------------");
    SetColorBoldYellowText("Test Frame interface");
    SetColorBoldYellowText("-------------------");
    std::cout << "frame_time_stamp_us: " << time_stamp_us << std::endl;
    std::cout << "frame_system_time_stamp_us: " << frame_system_time_stamp_us << std::endl;
    std::cout << "index: " << index << std::endl;
    std::cout << "frame_type: " << frame_type << std::endl;
    std::cout << "frame_format: " << frame_format << std::endl;
    std::cout << "frame_data_size: " << frame_data_size << std::endl;
    SetColorBoldYellowText("-------------------");
    SetColorBoldYellowText("Test GetDevice interface");
    SetColorBoldYellowText("-------------------");
    std::cout << "device_name: " << device_name << std::endl;
    SetColorBoldYellowText("-------------------");
    SetColorBoldYellowText("Test GetSensor interface");
    SetColorBoldYellowText("-------------------");
    std::cout << "sensor_type: " << sensor_type << std::endl;
    SetColorBoldYellowText("-------------------");
    SetColorBoldYellowText("Test StreamProfile interface");
    SetColorBoldYellowText("-------------------");
    std::cout << "stream_profile_format: " << stream_profile_format << std::endl;
    std::cout << "fps: " << fps << std::endl;
    std::cout << "width: " << width << std::endl;
    std::cout << "height: " << height << std::endl;
    std::cout << "stream_type: " << stream_type << std::endl;
    std::cout << "intrinsic_rows: " << intrinsic_rows << std::endl;
    std::cout << "intrinsic_cols: " << intrinsic_cols << std::endl;
    SetColorBoldYellowText("-------------------");
    SetColorBoldYellowText("Test GetFrameMetadataValue interface ");
    SetColorBoldYellowText("-------------------");
    for (int meta_data_type = 1; meta_data_type < kDtFrameMetadataTypeCount; meta_data_type++) {
      if (frame->HasMetadata((DtFrameMetadataType) meta_data_type)) {
        std::cout << meta_data_types[meta_data_type - 1] << ": "
                  << frame->GetMetadataValue((DtFrameMetadataType) meta_data_type) << std::endl;
      } else {
        std::cout << meta_data_types[meta_data_type - 1] << ": "
                  << "unsupported" << std::endl;
      }
    }

    auto viedeo_frame = frame->As<VideoFrame>();
    auto depth_frame = frame->As<DepthFrame>();
    auto ir_frame = frame->As<IrFrame>();
    auto points_frame = frame->As<PointsFrame>();
    if (viedeo_frame) {
      SetColorBoldYellowText("-------------------");
      SetColorBoldYellowText("Test VideoFrame interface ");
      SetColorBoldYellowText("-------------------");
      std::cout << "width: " << viedeo_frame->GetWidth() << std::endl;
      std::cout << "height: " << viedeo_frame->GetHeight() << std::endl;
      std::cout << "pixel available bit size: "
                << static_cast<int>(viedeo_frame->GetPixelAvailableBitSize()) << std::endl;
    }
    if (depth_frame) {
      SetColorBoldYellowText("-------------------");
      SetColorBoldYellowText("Test DeothFrame interface ");
      SetColorBoldYellowText("-------------------");
      std::cout << "value scale: " << depth_frame->GetValueScale() << std::endl;
    }
    if (ir_frame) {
      SetColorBoldYellowText("-------------------");
      SetColorBoldYellowText("Test IrFrame interface ");
      SetColorBoldYellowText("-------------------");
      // std::cout << "data source: " << ir_frame->GetDataSource() << std::endl;
    }
    if (points_frame) {
      SetColorBoldYellowText("-------------------");
      SetColorBoldYellowText("Test PointsFrame interface ");
      SetColorBoldYellowText("-------------------");
      std::cout << "position value scale: " << points_frame->GetCoordinateValueScale() << std::endl;
    }
  }
}
void TestToolDevice::PrintSensorInfo(std::shared_ptr<Sensor> sensor) {
  std::cout << std::endl;
  std::cout << "----------------------" << print_title_ << " select sensor function"
            << "----------------------" << std::endl;
  while (1) {
    SetBlueText("66  show print info again");
    SetBlueText("99  return last menu");
    SetBlueText("1  get current sensor type ");
    SetBlueText("2  get profile list info ");
    SetBlueText("3  start stream");
    SetBlueText("4  stop stream");
    SetBlueText("5  print frame info");
    std::cout << std::endl;
    std::cout << "input your choice: ";
    int selection = 0;
    std::cin >> selection;
    if (std::cin.fail()) {
      std::cout << "what are you doing, entered an unknown num" << std::endl;
      std::cin.clear();
      std::cin.ignore();
      continue;
    }
    switch (selection) {
      case 66:
        break;
      case 99:
        return;
      case 1:
        GetSensorType(sensor);
        break;
      case 2:
        GetSensorStreamProfileList(sensor);
        break;
      case 3:
        StartSensorStream(sensor);
        break;
      case 4:
        StopSensorStream(sensor);
        break;
      case 5:
        EnablePrintFrameInfo();
        break;
      default:
        break;
    }
  }
}
void TestToolDevice::PrintOptionUsage() try {
  std::cout << std::endl;
  std::cout << "----------------------" << print_title_ << " option command"
            << "----------------------" << std::endl;
  while (1) {
    SetBlueText("66  show print info again");
    SetBlueText("99  return last menu");
    SetBlueText("1  print option list");
    SetBlueText("2  is option supported ");
    SetBlueText("3  get option value");
    SetBlueText("4  set option value");
    std::cout << std::endl;
    std::cout << "input your choice: ";
    int selection = 0;
    std::cin >> selection;
    if (std::cin.fail()) {
      std::cout << "what are you doing, entered an unknown num" << std::endl;
      std::cin.clear();
      std::cin.ignore();
      continue;
    }
    switch (selection) {
      case 66:
        break;
      case 99:
        return;
      case 1:
        GetSupportedProperties();
        break;
      case 2:
        IsOptionSupported();
        break;
      case 3:
        GetOption();
        break;
      case 4:
        SetOption();
        break;
      default:
        break;
    }
  }
} catch (const std::exception& e) { std::cout << "exception: " << e.what() << std::endl; }

void TestToolDevice::PrintStreamPipelineUsage() {
  std::cout << std::endl;
  std::cout << "----------------------" << print_title_ << " stream command"
            << "----------------------" << std::endl;
  point_cloud_ = std::make_shared<PointCloudFilter>();
  point_cloud_->SetCreatePointFormat(kDtFormatPoint);
  std::string name = point_cloud_->GetName();
  std::cout << "point cloud: " << name << std::endl;
  // point_cloud_->SetCallBack([this](std::shared_ptr<Frame> frame) {});
  std::cout << std::endl;
  std::cout << "----------------------" << print_title_ << " get device name(test for get device)"
            << "----------------------" << std::endl;
  std::shared_ptr<DeviceInfo> device_info = device_->GetDeviceInfo();
  std::cout << "device name: " << device_info->GetPid() << std::endl;

  while (1) {
    SetBlueText("66  show print info again");
    SetBlueText("99  return last menu");
    SetBlueText("1  get current stream profile");
    SetBlueText("2  start pipeline ");
    SetBlueText("3  start pipeline with config");
    SetBlueText("4  start stream with callback");
    SetBlueText("5  start stream");
    SetBlueText("6  stop stream");
    SetBlueText("7  get stream profilelist by sensor type");
    SetBlueText("8  get camera parameters");
    SetBlueText("9  enable print frame info");
    SetBlueText("10  config current stream config");
    SetBlueText("11:  enable point cloud");
    std::cout << std::endl;
    std::cout << "input your choice: ";
    int selection = 0;
    std::cin >> selection;
    if (std::cin.fail()) {
      std::cout << "what are you doing, entered an unknown num" << std::endl;
      std::cin.clear();
      std::cin.ignore();
      continue;
    }
    switch (selection) {
      case 66:
        break;
      case 99:
        return;
      case 1:
        GetCurrentStreamProfile();
        break;
      case 2:
        StartPipeline();
        break;
      case 3:
        StartPipelineWithConfig();
        break;
      case 4:
        StartPipelineWithCallback();
        break;
      case 5:
        StartStream();
        break;
      case 6:
        StopStream();
        break;
      case 7:
        GetStreamProfileListBySensorType();
        break;
      case 8:
        GetCameraParametersByPipeline();
        break;
      case 9:
        EnablePrintFrameInfo();
        break;
      case 10:
        ConfigCurrentStreamConfig();
        break;
      case 11:
        EnablePointCloud();
        break;
      default:
        break;
    }
  }
}
void TestToolDevice::PrintStreamProfileList(std::shared_ptr<StreamProfileList> profile_list) {
  int count = profile_list->GetCount();
  std::cout << "stream profile list count: " << count << std::endl;
  for (int i = 0; i < count; i++) {
    std::shared_ptr<StreamProfile> stream_profile = profile_list->GetProfile(i);
    std::cout << "stream profile index: " << i << std::endl;
    std::cout << "stream type: " << stream_profile->GetType() << std::endl;
    std::cout << "format: " << stream_profile->GetFormat() << std::endl;
    if (stream_profile->Is<VideoStreamProfile>()) {
      auto video_stream_profile = stream_profile->As<VideoStreamProfile>();
      std::cout << "fps: " << video_stream_profile->GetFps() << std::endl;
      std::cout << "width: " << video_stream_profile->GetWidth() << std::endl;
      std::cout << "height: " << video_stream_profile->GetHeight() << std::endl;
      try {
        std::cout << "intrinsic: rows " << video_stream_profile->GetIntrinsic().height
                  << " cols: " << video_stream_profile->GetIntrinsic().width << std::endl;
      } catch (const std::exception& e) { std::cout << "intrinsic: " << e.what() << std::endl; }
    }
    std::cout << std::endl;
  }
}
void TestToolDevice::PrintImuData() {
  // std::shared_ptr<Dt_smpl::FrameRateHelper> frame_rate_helper =
  // std::make_shared<Dt_smpl::FrameRateHelper>(); long cnt = 0;
  // CHECK_SDK_RETURN_VALUE(device_->RegisterImuDataHandler(
  //     [this, cnt, frame_rate_helper](const std::shared_ptr<ImuMeasurement>& imu_out_data)
  //     mutable
  //     {
  //       std::cout << "imu_out_data->timestamp: " << imu_out_data->timestamp << std::endl;
  //       std::cout << "imu_out_data->ax: " << imu_out_data->ax << std::endl;
  //       std::cout << "imu_out_data->ay: " << imu_out_data->ay << std::endl;
  //       std::cout << "imu_out_data->az: " << imu_out_data->az << std::endl;
  //       std::cout << "imu_out_data->wx: " << imu_out_data->wx << std::endl;
  //       std::cout << "imu_out_data->wy: " << imu_out_data->wy << std::endl;
  //       std::cout << "imu_out_data->wz: " << imu_out_data->wz << std::endl;
  //       std::cout << std::endl;
  //       frame_rate_helper->RecordTimestamp();
  //       if (is_print_fps_.load() && 0 == cnt++ % 10) {
  //         LOG_F(INFO, "fps: %f", frame_rate_helper->GetFrameRate());
  //       }
  //     }));
  // std::cout << std::endl;
  // return 0;
}
void TestToolDevice::TestLoopStreamThread(long frame_loop_times, long stream_loop_times) {
  (void) frame_loop_times;
  (void) stream_loop_times;
}
int TestToolDevice::TestDeviceWholeProcessThread(long device_loop_times,
                                                 long stream_loop_times,
                                                 long frame_loop_times) {
  (void) device_loop_times;
  (void) stream_loop_times;
  (void) frame_loop_times;
  return 0;
  // int ret = 0;
  // int current = 1;
  // int device_loop_times_temp = device_loop_times;
  // while (device_loop_times > 0) {
  //   std::cout << "current test progress: " << current << "/" << device_loop_times_temp <<
  //   std::endl; long stream_loop_times_temp = stream_loop_times;
  //   CHECK_SDK_RETURN_VALUE(OpenDevice());
  //   CHECK_SDK_RETURN_VALUE(pipeline_->Init(device_));
  //   while (1) {
  //     std::unique_lock<std::mutex> lk(mutex_);
  //     if (stream_loop_times_temp == -1) {
  //     } else {
  //       if (stream_loop_times_temp <= 0)
  //         break;
  //       stream_loop_times_temp--;
  //     }
  //     std::cout << "stream loop times: " << stream_loop_times_temp << std::endl;
  //     CHECK_SDK_RETURN_VALUE(StartPipelineWithConfig());
  //     stream_process_thread_ = std::make_shared<std::thread>(&TestToolDevice::StreamProcess,
  //                                                            this,
  //                                                            frame_loop_times);
  //     if (stream_process_thread_->joinable()) {
  //       stream_process_thread_->join();
  //     }
  //   }
  //   CHECK_SDK_RETURN_VALUE(pipeline_->Stop());
  //   CHECK_SDK_RETURN_VALUE(CloseDevice());
  //   current++;
  //   device_loop_times--;
  // }
  // return ret;
}
int TestToolDevice::StreamProcess(long frame_loop_times) {
  int ret = 0;
  stream_is_running_ = true;
  std::shared_ptr<dt_utils::FrameRateHelper>
      frame_rate_helper = std::make_shared<dt_utils::FrameRateHelper>();
  std::shared_ptr<FrameSet> frame_set;
  long cnt = 0;
  std::map<DtFrameType, std::shared_ptr<Frame>> frame_map;
  while (stream_is_running_.load()) {
    if (frame_loop_times == -1) {
    } else {
      if (frame_loop_times == 0)
        break;
      frame_loop_times--;
    }
    frame_set = pipeline_->WaitForFrames(1000);
    if (!frame_set) {
      continue;
    }
    if (frame_set) {
      auto count = frame_set->GetCount();
      if (enable_point_cloud_)
        point_cloud_->PushFrame(frame_set);
      for (uint32_t i = 0; i < count; i++) {
        auto frame = frame_set->GetFrameByIndex(i);
        if (is_print_frame_info_.load()) {
          PrintFrameInfo(frame);
        }
        frame_map[frame->GetType()] = frame;
      }
      frame_rate_helper->RecordTimestamp();
      if (is_print_fps_.load() && 0 == cnt++ % 10) {
        std::cout << "fps: " << frame_rate_helper->GetFrameRate() << std::endl;
      }
      std::vector<std::shared_ptr<const dt::Frame>> frames_for_render;
      for (auto& frame : frame_map) {
        frames_for_render.push_back(frame.second);
      }
      {
        std::unique_lock<std::mutex> lock(mutex_frame_);
        recv_frame_queue_.push(frame_set);
        cv_frame_.notify_all();
      }
    }
  }
  StopStream();
  std::cout << "thread end!" << std::endl;
  return ret;
}

void TestToolDevice::Init() {
  try {
    std::string sdk_version = std::to_string(Version::GetVersion());
    device_info_ = device_->GetDeviceInfo();
    print_title_ = " Usb port: " + std::string(device_info_->GetUid()) + " " +
                   device_info_->GetName() + " " + sdk_version + " ";

    pipeline_ = std::make_shared<Pipeline>(device_);
    config_ = std::make_shared<Config>();
    int count = device_->GetSupportedOptionCount();
    for (int i = 0; i < count; i++) {
      DtOptionItem option = device_->GetSupportedOption(i);
      properties_.push_back(option);
      id_option_map_[option.id] = option;
    }

    is_window_thread_running_.store(true);
    window_thread_ = std::thread(&TestToolDevice::ShowFrameProcess, this);
  } catch (dt::Error& e) {
    std::cerr << "function:" << e.GetFunction() << "\nargs:" << e.GetArgs()
              << "\nmessage:" << e.What() << "\ntype:" << e.GetExceptionType() << std::endl;
    std::cout << "\nPress any key to exit.";
    dt_utils::WaitForKeyPressed();
    exit(EXIT_FAILURE);
  }
}

void TestToolDevice::ShowFrameProcess() {
#ifdef BUILD_GRAPHICAL_EXAMPLES
  dt_utils::window app(1280, 720, "StreamWindow");

  while (app) {
    std::unique_lock<std::mutex> lk(mutex_frame_);
    cv_frame_.wait_for(lk, std::chrono::milliseconds(100), [this]() {
      return !recv_frame_queue_.empty() || !is_window_thread_running_;
    });

    if (!is_window_thread_running_) {
      // std::cout << "window thread end!" << std::endl;
      break;
    }
    if (!recv_frame_queue_.empty()) {
      auto frame = recv_frame_queue_.front();
      recv_frame_queue_.pop();
      lk.unlock();
      app.show(frame);
    }
  }
#endif
}

void TestToolDevice::EnablePointCloud() {
  enable_point_cloud_ = !enable_point_cloud_;
  point_cloud_->Enable(enable_point_cloud_);
}

void TestToolDevice::SetRoiStruct(DtOptionId option_id) try {
  if (device_->IsOptionSupported(option_id, kDtPermissionReadWrite)) {
    DtRegionOfInterest roi;
    std::cout << "Enter roi x:" << std::endl;
    std::cin >> roi.x;
    std::cout << "Enter roi y:" << std::endl;
    std::cin >> roi.y;
    std::cout << "Enter roi width:" << std::endl;
    std::cin >> roi.width;
    std::cout << "Enter roi height:" << std::endl;
    std::cin >> roi.height;

    device_->SetStructuredData(option_id, reinterpret_cast<uint8_t*>(&roi), sizeof(roi));
  } else {
    std::cout << option_id << " is not supported." << std::endl;
  }
} catch (dt::Error& e) {
  std::cerr << e << std::endl;
  std::cout << "\nPress any key to exit.";
  dt_utils::WaitForKeyPressed();
}

void TestToolDevice::SetIntensityRangeStruct(DtOptionId option_id) try {
  if (device_->IsOptionSupported(option_id, kDtPermissionReadWrite)) {
    DtIntensityRange range;
    std::cout << "Enter range max:" << std::endl;
    std::cin >> range.max;
    std::cout << "Enter range min:" << std::endl;
    std::cin >> range.min;
    device_->SetStructuredData(option_id, reinterpret_cast<uint8_t*>(&range), sizeof(range));
  } else {
    std::cout << option_id << " is not supported." << std::endl;
  }
} catch (dt::Error& e) {
  std::cerr << e << std::endl;
  std::cout << "\nPress any key to exit.";
  dt_utils::WaitForKeyPressed();
}

bool StringToIPv4(const std::string& ip_str, uint8_t address[4]) {
  std::vector<std::string> parts;
  std::stringstream ss(ip_str);
  std::string part;

  // 按'.'分割字符串
  while (std::getline(ss, part, '.')) {
    parts.push_back(part);
  }

  // 检查是否为4部分
  if (parts.size() != 4) {
    return false;
  }

  // 转换为数字并检查范围
  for (int i = 0; i < 4; ++i) {
    try {
      int num = std::stoi(parts[i]);
      if (num < 0 || num > 255) {
        return false;
      }
      address[i] = static_cast<uint8_t>(num);
    } catch (...) { return false; }
  }

  return true;
}

std::string Ipv4ToString(const uint8_t address[4]) {
  std::ostringstream oss;
  oss << static_cast<int>(address[0]) << "." << static_cast<int>(address[1]) << "."
      << static_cast<int>(address[2]) << "." << static_cast<int>(address[3]);
  return oss.str();
}

void TestToolDevice::SetNetIpConfig(DtOptionId option_id) try {
  if (device_->IsOptionSupported(option_id, kDtPermissionReadWrite)) {
    DtNetIpConfig ip_config;
    std::cout << "Set net ip config" << std::endl;
    std::cout << "Ip config use dhcp:" << std::endl;
    std::cin >> ip_config.dhcp;
    if (ip_config.dhcp) {
      device_->SetStructuredData(option_id,
                                 reinterpret_cast<uint8_t*>(&ip_config),
                                 sizeof(ip_config));
    } else {
      std::cout << "Enter ip:" << std::endl;
      std::string ip;
      std::cin >> ip;
      if (!StringToIPv4(ip, ip_config.address)) {
        std::cerr << "Invalid IP address format" << std::endl;
        return;
      }
      std::cout << "Enter mask:" << std::endl;
      std::string mask;
      std::cin >> mask;
      if (!StringToIPv4(mask, ip_config.mask)) {
        std::cerr << "Invalid IP address format" << std::endl;
        return;
      }
      std::cout << "Enter gateway:" << std::endl;
      std::string gateway;
      std::cin >> gateway;
      if (!StringToIPv4(gateway, ip_config.gateway)) {
        std::cerr << "Invalid IP address format" << std::endl;
        return;
      }
      device_->SetStructuredData(option_id,
                                 reinterpret_cast<uint8_t*>(&ip_config),
                                 sizeof(ip_config));
    }
  } else {
    std::cout << option_id << " is not supported." << std::endl;
  }
} catch (dt::Error& e) {
  std::cerr << e << std::endl;
  std::cout << "\nPress any key to exit.";
  dt_utils::WaitForKeyPressed();
}

void TestToolDevice::GetNetIpConfig(DtOptionId option_id) try {
  if (device_->IsOptionSupported(option_id, kDtPermissionRead)) {
    DtNetIpConfig ip_config;
    uint32_t data_size = sizeof(DtNetIpConfig);
    device_->GetStructuredData(option_id, reinterpret_cast<uint8_t*>(&ip_config), &data_size);
    std::cout << "Ip config use dhcp:" << ip_config.dhcp << std::endl;
    std::cout << "Ip config ip:" << Ipv4ToString(ip_config.address) << std::endl;
    std::cout << "Ip config mask:" << Ipv4ToString(ip_config.mask) << std::endl;
    std::cout << "Ip config gateway:" << Ipv4ToString(ip_config.gateway) << std::endl;
  } else {
    std::cout << option_id << " is not supported." << std::endl;
  }
} catch (dt::Error& e) {
  std::cerr << e << std::endl;
  std::cout << "\nPress any key to exit.";
  dt_utils::WaitForKeyPressed();
}

}  // namespace dt
