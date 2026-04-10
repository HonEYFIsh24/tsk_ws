#ifndef SRC_TEST_TOOL_DEVICE_H_
#define SRC_TEST_TOOL_DEVICE_H_
#include <atomic>
#include "deptrum/deptrum.hpp"
// #include "test_types.h"
#include <condition_variable>
#include <mutex>
#include <queue>
#include <thread>

namespace dt {
class TestToolDevice {
 public:
  TestToolDevice(std::shared_ptr<Device> device);
  ~TestToolDevice();
  void UnInit();
  void Reset(std::shared_ptr<Device> device);
  std::string GetDeivceUid();

  void PrintDeviceOptions();
  void GetDeviceInfo();
  void GetVersionInfo();
  void SetLoggerSeverity();
  void SetLoggerToFile();
  void GetSensorList();
  void GetSensorByType();
  void WriteFlash();
  void ReadFlash();
  void WriteEeprom();
  void ReadEeprom();
  void SendFileToDevice();
  void ReceiveFileFromDevice();
  void UpdateFirmware();
  void SetHeartbeat();
  void GetDeviceStatus();
  void GetCameraParameters();
  void SetDeviceSerialNumber();
  void GetDeviceSerialNumber();
  void DeviceAeParams();
  void DeviceExposureStruct();
  void Reboot();
  void WriteLicense();
  void ReadLicense();
  void GetOption();
  void SetOption();
  void IsOptionSupported();
  void GetSupportedProperties();
  void StartSensorStream(std::shared_ptr<Sensor> sensor);
  void StopSensorStream(std::shared_ptr<Sensor> sensor);
  void GetSensorStreamProfileList(std::shared_ptr<Sensor> sensor);
  void GetSensorType(std::shared_ptr<Sensor> sensor);
  // int SetLedModeStressTest();
  void EnableFps();
  void EnablePrintFrameInfo();
  // void WholeProcessStressTest();
  void StreamProcessStressTest();

  void GetCurrentStreamProfile();
  void StartPipeline();
  void StartPipelineWithConfig();
  void StartPipelineWithCallback();
  void StartStream();
  void StopStream();
  void GetStreamProfileListBySensorType();
  void GetCameraParametersByPipeline();
  void ConfigCurrentStreamConfig();

  void GetEnabledConfigStreamProfileList();
  void DisableConfigAllStream();
  void DisableConfigStream();
  void EnableVideoStream();
  void EnableVideoStreamWithProfile();

  void PrintImuData();

  void EnablePointCloud();

 private:
  void SetRoiStruct(DtOptionId option_id);
  void SetIntensityRangeStruct(DtOptionId option_id);
  void SetNetIpConfig(DtOptionId option_id);
  void GetNetIpConfig(DtOptionId option_id);

 private:
  void PrintFrameInfo(std::shared_ptr<Frame> frame);
  void PrintSensorInfo(std::shared_ptr<Sensor> sensor);
  void PrintOptionUsage();
  void PrintStreamPipelineUsage();
  void PrintStreamProfileList(std::shared_ptr<StreamProfileList> profile_list);
  void TestLoopStreamThread(long frame_loop_times, long stream_loop_times);
  int TestDeviceWholeProcessThread(long device_loop_times,
                                   long stream_loop_times,
                                   long frame_loop_times);
  int StreamProcess(long frame_loop_times = -1);
  void Init();
  void ShowFrameProcess();

 private:
  std::shared_ptr<Device> device_;
  std::shared_ptr<DeviceInfo> device_info_;
  std::string print_title_{""};
  // std::atomic<bool> device_is_open_{false};
  std::atomic<bool> stream_is_running_{false};
  std::atomic<bool> is_print_fps_{false};
  std::atomic<bool> is_print_frame_info_{false};
  std::atomic<bool> is_start_pipeline_{false};
  std::vector<DtOptionItem> properties_;
  std::map<DtOptionId, DtOptionItem> id_option_map_;
  std::shared_ptr<Pipeline> pipeline_;
  std::shared_ptr<Config> config_;
  std::mutex mutex_;
  bool upgrade_event_triggered_ = false;
  std::shared_ptr<std::thread> stream_process_thread_;
  std::shared_ptr<PointCloudFilter> point_cloud_;
  std::atomic<bool> enable_point_cloud_{false};

  std::thread window_thread_;
  std::condition_variable cv_frame_;
  std::mutex mutex_frame_;
  std::queue<std::shared_ptr<Frame>> recv_frame_queue_;
  std::atomic<bool> is_window_thread_running_{true};
};
}  // namespace dt
#endif  // SRC_TEST_TOOL_DEVICE_MANAGE_H_