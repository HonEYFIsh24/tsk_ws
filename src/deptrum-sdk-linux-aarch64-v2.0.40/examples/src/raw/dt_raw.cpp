// Copyright (c) Deptrum Inc. All Rights Reserved.

#include <deptrum/deptrum.hpp>
#include <thread>
#include "frame_rate_helper.h"
#include "utils.hpp"
#include "utils_glfw.hpp"

int main(void) try {
  // Initialize a pipeline using the default connected device.
  dt::Pipeline pipe;

  // Create a configuration object to specify which streams to enable.
  std::shared_ptr<dt::Config> config = std::make_shared<dt::Config>();

  // Utility for tracking and calculating frame rate (Fps).
  std::shared_ptr<dt_utils::FrameRateHelper>
      frame_rate_helper = std::make_shared<dt_utils::FrameRateHelper>();

  // Enable the Raw Phase video stream.
  config->EnableVideoStream(kDtStreamRawPhase);

  pipe.Start(config);
  // Create a rendering window with the given title, resolution, and layout mode.
  dt_utils::window app(1280, 720, "StreamWindow");

  long cnt = 0;
  while (app) {
    // Wait (up to 1000 ms) for a new frameset from the pipeline in blocking mode.
    auto frame_set = pipe.WaitForFrames(1000);
    if (frame_set == nullptr) {
      continue;
    }

    // Record a timestamp for frame rate measurement.
    frame_rate_helper->RecordTimestamp();

    // Every 30 frames, print the calculated frame rate to the console.
    if (cnt++ % 30 == 0) {
      std::cout << "fps: " << frame_rate_helper->GetFrameRate() << std::endl;
    }

    // Send the received frameset to the window for rendering.
    app.show(frame_set);
  }

  // Stop the pipeline once rendering is done.
  pipe.Stop();

  return 0;
} catch (dt::Error& e) {
  std::cerr << e << std::endl;
  std::cout << "\nPress any key to exit.";
  dt_utils::WaitForKeyPressed();
  exit(EXIT_FAILURE);
}
