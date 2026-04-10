// Copyright (c) Deptrum Inc. All Rights Reserved.

#include <deptrum/deptrum.hpp>
#include <mutex>
#include "utils.hpp"
#include "utils_glfw.hpp"

int main(void) try {
  // Create a pipeline.
  dt::Pipeline pipe;

  // Configure which streams to enable or disable for the Pipeline by creating a Config.
  std::shared_ptr<dt::Config> config = std::make_shared<dt::Config>();

  // Get device from pipeline.
  auto device = pipe.GetDevice();

  // Get sensor_list from device.
  auto sensor_list = device->GetSensorList();

  for (uint32_t index = 0; index < sensor_list->GetCount(); index++) {
    // Query all supported infrared sensor type and enable the infrared stream.
    // For dual infrared device, enable the left and right infrared streams.
    // For single infrared device, enable the infrared stream.
    DtSensorType sensor_type = sensor_list->GetSensorType(index);

    // exclude non-video sensor type
    if (!dt::TypeHelper::IsVideoSensorType(sensor_type)) {
      continue;
    }

    // Enable the stream for the sensor type.
    config->EnableStream(sensor_type);
  }

  std::mutex frameset_mutex;
  std::shared_ptr<dt::FrameSet> frameset = nullptr;

  // Start the pipeline with callback.
  pipe.Start(config, [&](std::shared_ptr<dt::FrameSet> output) {
    std::lock_guard<std::mutex> lock(frameset_mutex);
    frameset = output;
  });

  // Create a window for rendering, and set the size of the window.
  dt_utils::window app(1280, 720, "Callback");
  while (app) {
    std::lock_guard<std::mutex> lock(frameset_mutex);

    if (frameset == nullptr) {
      continue;
    }

    // Rendering display
    app.show(frameset);
  }

  // Stop the Pipeline, no frame data will be generated
  pipe.Stop();

  return 0;
} catch (dt::Error& e) {
  std::cerr << e << std::endl;
  std::cout << "\nPress any key to exit.";
  dt_utils::WaitForKeyPressed();
  exit(EXIT_FAILURE);
}
