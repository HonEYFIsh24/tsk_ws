// Copyright (c) Deptrum Inc. All Rights Reserved.

#include <deptrum/deptrum.hpp>
#include <mutex>
#include "frame_rate_helper.h"
#include "utils.hpp"

std::map<int, std::shared_ptr<const dt::Frame>> framesets;
std::mutex frameset_mutex;
const int max_device_count = 9;
std::shared_ptr<dt_utils::FrameRateHelper> frame_rate_helpers[max_device_count];
long frame_count[max_device_count] = {0};
void StartStream(std::map<int, std::shared_ptr<dt::Pipeline>>& pipes) {
  int i = 0;
  for (auto& item : pipes) {
    int device_index = item.first;
    auto& pipe = item.second;

    // config to enable depth and color streams
    std::shared_ptr<dt::Config> config = std::make_shared<dt::Config>();
    frame_rate_helpers[i] = std::make_shared<dt_utils::FrameRateHelper>();
    auto stream_profile = dt_utils::SelectStreamProfileList(pipe->GetDevice());
    auto video_stream_profile = stream_profile->As<dt::VideoStreamProfile>();
    config->EnableVideoStream(kDtStreamDepth,
                              video_stream_profile->GetWidth(),
                              video_stream_profile->GetHeight(),
                              video_stream_profile->GetFps(),
                              video_stream_profile->GetFormat());
    std::cout << "width: " << video_stream_profile->GetWidth()
              << " height: " << video_stream_profile->GetHeight()
              << " fps: " << video_stream_profile->GetFps()
              << " format: " << video_stream_profile->GetFormat() << std::endl;

    // start pipeline and pass the callback function to receive the frames
    pipe->Start(config, [pipe, device_index, i](std::shared_ptr<dt::FrameSet> frame_set) {
      std::lock_guard<std::mutex> lock(frameset_mutex);
      framesets[device_index] = frame_set;
      frame_rate_helpers[i]->RecordTimestamp();
      if (0 == frame_count[i]++ % 10) {
        std::cout << "device" << i << " uid: " << pipe->GetDevice()->GetDeviceInfo()->GetUid()
                  << " fps: " << frame_rate_helpers[i]->GetFrameRate() << std::endl;
      }
    });
    i++;
  }
}

void StopStream(std::map<int, std::shared_ptr<dt::Pipeline>>& pipes) {
  for (auto& item : pipes) {
    auto& pipe = item.second;
    // stop the pipeline
    pipe->Stop();
  }

  std::lock_guard<std::mutex> lock(frameset_mutex);
  framesets.clear();
}

int main() try {
  // Create a Context
  dt::Context ctx;

  // Query the list of connected devices
  auto dev_list = ctx.QueryDeviceList();

  // Get the number of connected devices
  int dev_count = dev_list->GetCount();

  // Create a pipeline for each device
  std::map<int, std::shared_ptr<dt::Pipeline>> pipes;
  for (int i = 0; i < dev_count; i++) {
    // Get the device from device list
    auto dev = dev_list->GetDevice(i);

    // Create a pipeline for the device
    auto pipe = std::make_shared<dt::Pipeline>(dev);

    // Add the pipeline to the map of pipelines
    pipes.insert({i, pipe});
    dt_utils::CheckDeviceMode(dev);
    dt_utils::CheckDepthMode(dev);
  }

  // Start the depth and color streams for all devices
  StartStream(pipes);

  // Main loop to show the frames, press `ESC` to exit
  char input;
  while (std::cin >> input) {
    if (input == 'q')
      break;
  }

  // // Stop all streams and clear the framesets
  // stop_stream(pipes);

  return 0;
} catch (dt::Error& e) {
  std::cerr << e << std::endl;
  std::cout << "\nPress any key to exit.";
  dt_utils::WaitForKeyPressed();
  exit(EXIT_FAILURE);
}
