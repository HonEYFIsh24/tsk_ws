// Copyright (c) Deptrum Inc. All Rights Reserved.

#include <deptrum/deptrum.hpp>
#include "utils.hpp"
#include "utils_glfw.hpp"

int main(void) try {
  // Create a pipeline.
  dt::Pipeline pipe;

  // Start the pipeline with default config.
  // Modify the default configuration by the configuration file: "deptrum_config.json"
  pipe.Start();

  // Create a window for showing the frames, and set the size of the window.
  dt_utils::window app(1280, 720, "Capture");

  while (app) {
    // Wait for frame_set from the pipeline, the default timeout is 1000ms.
    auto frame_set = pipe.WaitForFrames();
    if (!frame_set) {
      std::cout << "No frameSet received, exiting..." << std::endl;
      continue;
    }
    // std::cout << "frameSet: " << frame_set->frame_count() << std::endl;

    // Push the frames to the window for showing.
    app.show(frame_set);
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
