// Copyright (c) Deptrum Inc. All Rights Reserved.

#include <deptrum/deptrum.hpp>
#include <thread>
#include "utils.hpp"
#include "utils_glfw.hpp"

int main(void) try {
  // Create a pipeline with default device.
  dt::Pipeline pipe;

  // By creating config to configure which streams to enable or disable for the pipeline, here the
  // depth stream will be enabled.
  std::shared_ptr<dt::Config> config = std::make_shared<dt::Config>();

  // This is the default depth streamprofile that is enabled. If you want to modify it, you can do
  // so in the configuration file.
  config->EnableVideoStream(kDtStreamDepth);

  // Start the pipeline with config.
  pipe.Start(config);

  // Create a window for rendering, and set the resolution of the window.
  dt_utils::window app(1280, 720, "Depth");

  while (app) {
    // Wait for up to 100ms for a frameset in blocking mode.
    auto frame_set = pipe.WaitForFrames(1000);
    if (frame_set == nullptr) {
      continue;
    }

    // Get the depth frame raw from frameset.
    auto depth_frame = frame_set->GetFrame(kDtFrameDepth);
    if (!depth_frame) {
      continue;
    }

    // Render frame in the window.
    app.show(depth_frame);
  }

  // Stop the pipeline
  pipe.Stop();

  return 0;
} catch (dt::Error& e) {
  std::cerr << e << std::endl;
  std::cout << "\nPress any key to exit.";
  dt_utils::WaitForKeyPressed();
  exit(EXIT_FAILURE);
}
