// Copyright (c) Deptrum Inc. All Rights Reserved.

#include <deptrum/deptrum.hpp>
#include "utils.hpp"
#include "utils_glfw.hpp"

int main(void) try {
  // Create a pipeline with default device.
  dt::Pipeline pipe;

  // Configure which streams to enable or disable for the Pipeline by creating a Config.
  std::shared_ptr<dt::Config> config = std::make_shared<dt::Config>();

  // Enable color video stream.
  config->EnableVideoStream(kDtStreamColor);

  // Start the pipeline with config.
  pipe.Start(config);

  // Create a window for rendering and set the resolution of the window.
  dt_utils::window app(1280, 720, "Color");

  while (app) {
    // Wait for up to 100ms for a frameset in blocking mode.
    auto frame_set = pipe.WaitForFrames();
    if (frame_set == nullptr) {
      continue;
    }

    // get color frame from frameset.
    auto color_frame = frame_set->GetFrame(kDtFrameColor);
    // Render color_frame.
    app.show(color_frame);
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
