// Copyright (c) Deptrum Inc. All Rights Reserved.

#include <deptrum/deptrum.h>
#include <atomic>
#include <mutex>
#include <thread>
#include "utils.hpp"
#include "utils_glfw.hpp"

std::atomic<bool> is_paused{false};

void HandleKeyPress(std::shared_ptr<dt::RecordDevice> recorder, int key);

int main(void) try {
  std::cout << "Please enter the output filename (with .bag extension) and press Enter to start "
               "recording: ";
  std::string file_path;
  std::getline(std::cin, file_path);

  // Create a context, for getting devices and sensors
  std::shared_ptr<dt::Context> context = std::make_shared<dt::Context>();

  // Query device list
  auto device_list = context->QueryDeviceList();
  if (device_list->GetCount() < 1) {
    std::cout << "No device found! Please connect a supported device and retry this program."
              << std::endl;
    std::cout << "\nPress any key to exit.";
    dt_utils::WaitForKeyPressed();
    exit(EXIT_FAILURE);
  }

  // Acquire first available device
  auto device = device_list->GetDevice(0);

  // Create a pipeline the specified device
  auto pipe = std::make_shared<dt::Pipeline>(device);

  // Activate device clock synchronization
  device->TimerSyncWithHost();

  // Create a config and enable all streams
  std::shared_ptr<dt::Config> config = std::make_shared<dt::Config>();
  auto sensor_list = device->GetSensorList();
  for (uint32_t i = 0; i < sensor_list->GetCount(); i++) {
    auto sensor_type = sensor_list->GetSensorType(i);
    config->EnableStream(sensor_type);
  }

  std::mutex frame_mutex;
  std::shared_ptr<dt::FrameSet> render_frame_set;
  pipe->Start(config, [&](std::shared_ptr<dt::FrameSet> frame_set) {
    std::lock_guard<std::mutex> lock(frame_mutex);
    render_frame_set = frame_set;
  });

  // Initialize recording device with output file
  auto record_device = std::make_shared<dt::RecordDevice>(device, file_path);

  dt_utils::window app(1280, 720, "Record");
  dt_utils::window_key_listener key_listener(app);
  key_listener.set_key_pressed_callback(
      [record_device](int key) { HandleKeyPress(record_device, key); });

  std::cout << "Press 'S' to pause/resume recording." << std::endl;
  // set the callback function for the window to handle key press events

  while (app) {
    std::lock_guard<std::mutex> lock(frame_mutex);
    if (render_frame_set == nullptr) {
      continue;
    }
    app.show(render_frame_set);
  }

  pipe->Stop();

  // Flush and save recording file
  record_device = nullptr;

  return 0;
} catch (dt::Error& e) {
  std::cerr << "Function: " << e.GetFunction() << "\nArgs: " << e.GetArgs()
            << "\nMessage: " << e.What() << "\nException Type: " << e.GetExceptionType()
            << std::endl;
  std::cout << "\nPress any key to exit.";
  dt_utils::WaitForKeyPressed();
  exit(EXIT_FAILURE);
}

void HandleKeyPress(std::shared_ptr<dt::RecordDevice> recorder, int key) {
  if (key == 'S' || key == 's') {
    if (!is_paused) {
      recorder->Pause();
      is_paused.store(true);
      std::cout << "[PAUSED] Recording paused" << std::endl;
    } else {
      recorder->Resume();
      is_paused.store(false);
      std::cout << "[RESUMED] Recording resumed" << std::endl;
    }
  }
}
