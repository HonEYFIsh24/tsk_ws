// Copyright (c) Deptrum Inc. All Rights Reserved.

#include <deptrum/deptrum.h>
#include <atomic>
#include <cctype>
#include <mutex>
#include <thread>
#include "utils.hpp"
#include "utils_glfw.hpp"

bool GetRosbagPath(std::string& rosbag_path);

int main(void) try {
  std::atomic<bool> exited(false);
  std::string file_path;
  // Get valid .bag file path from user input
  GetRosbagPath(file_path);

  // Create a playback device with a Rosbag file
  std::shared_ptr<dt::PlaybackDevice> playback = std::make_shared<dt::PlaybackDevice>(file_path);
  // Create a pipeline with the playback device
  std::shared_ptr<dt::Pipeline> pipe = std::make_shared<dt::Pipeline>(playback);
  // Enable all recording streams from the playback device
  std::shared_ptr<dt::Config> config = std::make_shared<dt::Config>();

  // Set playback status change callback, when the playback stops, start the pipeline again with the
  // same config
  playback->SetPlaybackStatusChangeCallback([&](DtPlaybackStatus status) {
    if (status == kDtPlaybackStopped && !exited) {
      pipe->Stop();
      std::this_thread::sleep_for(std::chrono::milliseconds(1000));
      pipe->Start(config);
    }
  });

  auto sensor_list = playback->GetSensorList();
  for (uint32_t i = 0; i < sensor_list->GetCount(); i++) {
    auto sensor_type = sensor_list->GetSensorType(i);

    config->EnableStream(sensor_type);
  }

  // Start the pipeline with the config
  pipe->Start(config);

  dt_utils::window app(1280, 720, "Playback");

  while (app && !exited) {
    auto frame_set = pipe->WaitForFrames();
    if (frame_set == nullptr) {
      continue;
    }
    app.show(frame_set);
  }
  exited = true;

  pipe->Stop();
  return 0;
} catch (dt::Error& e) {
  std::cerr << e << std::endl;
  std::cout << "\nPress any key to exit.";
  dt_utils::WaitForKeyPressed();
  exit(EXIT_FAILURE);
}

bool GetRosbagPath(std::string& rosbag_path) {
  while (true) {
    std::cout << "Please input the path of the Rosbag file (.bag) to playback: \n";
    std::cout << "Path: ";
    std::string input;
    std::getline(std::cin, input);

    // Remove leading and trailing whitespaces
    input.erase(
        std::find_if(input.rbegin(), input.rend(), [](unsigned char ch) { return !isspace(ch); })
            .base(),
        input.end());

    // Remove leading and trailing quotes
    if (!input.empty() && input.front() == '\'' && input.back() == '\'') {
      input = input.substr(1, input.size() - 2);
    }

    if (!input.empty() && input.front() == '\"' && input.back() == '\"') {
      input = input.substr(1, input.size() - 2);
    }

    // Validate .bag extension
    if (input.size() > 4 && input.substr(input.size() - 4) == ".bag") {
      rosbag_path = input;
      std::cout << "Playback file confirmed: " << rosbag_path << "\n\n";
      return true;
    }

    std::cout << "Invalid file format. Please provide a .bag file.\n\n";
  }
}