// Copyright (c) Deptrum Inc. All Rights Reserved.

#include <deptrum/deptrum.h>
#include <mutex>
#include <thread>
#include "utils.hpp"
#include "utils_glfw.hpp"

int main(void) try {
  // Create a pipeline with default device.
  dt::Pipeline pipe;

  // Configure which streams to enable or disable for the Pipeline by creating a Config.
  std::shared_ptr<dt::Config> config = std::make_shared<dt::Config>();

  // Enumerate and config all sensors.
  auto device = pipe.GetDevice();

  // Get sensor list from device.
  auto sensor_list = device->GetSensorList();

  for (uint32_t i = 0; i < sensor_list->GetCount(); i++) {
    // Get sensor type.
    auto sensor_type = sensor_list->GetSensorType(i);

    // exclude gyro and accel sensors.
    if (sensor_type == kDtSensorGyro || sensor_type == kDtSensorAccel) {
      continue;
    }

    // enable the stream.
    config->EnableStream(sensor_type);
  }

  // Start the pipeline with config
  std::mutex frame_mutex;
  std::shared_ptr<dt::FrameSet> render_frame_set;
  pipe.Start(config, [&](std::shared_ptr<dt::FrameSet> frame_set) {
    std::lock_guard<std::mutex> lock(frame_mutex);
    render_frame_set = frame_set;
  });

  // The IMU frame rate is much faster than the video, so it is advisable to use a separate pipeline
  // to obtain IMU data.
  auto dev = pipe.GetDevice();
  auto imu_pipeline = std::make_shared<dt::Pipeline>(dev);
  std::mutex imu_frame_mutex;
  std::shared_ptr<dt::FrameSet> render_imu_frame_set;

  std::shared_ptr<dt::Config> imu_config = std::make_shared<dt::Config>();
  // enable gyro stream.
  imu_config->EnableGyroStream();
  // enable accel stream.
  imu_config->EnableAccelStream();
  // start the imu pipeline.
  imu_pipeline->Start(imu_config, [&](std::shared_ptr<dt::FrameSet> frame_set) {
    std::lock_guard<std::mutex> Lock_imu(imu_frame_mutex);
    render_imu_frame_set = frame_set;
  });

  // Create a window for rendering and set the resolution of the window
  dt_utils::window app(1280, 720, "MultiStream");
  while (app) {
    std::lock_guard<std::mutex> Lock_imu(imu_frame_mutex);
    std::lock_guard<std::mutex> lock(frame_mutex);

    if (render_frame_set == nullptr || render_imu_frame_set == nullptr) {
      continue;
    }

    std::vector<std::shared_ptr<dt::Frame>> frames;
    frames.push_back(render_frame_set);
    frames.push_back(render_imu_frame_set);
    // Render camera and imu frameset.
    app.show(frames, 2);
  }

  // Stop the Pipeline, no frame data will be generated.
  pipe.Stop();

  // Stop the IMU Pipeline, no frame data will be generated.
  imu_pipeline->Stop();

  return 0;
} catch (dt::Error& e) {
  std::cerr << e << std::endl;
  std::cout << "\nPress any key to exit.";
  dt_utils::WaitForKeyPressed();
  exit(EXIT_FAILURE);
}
