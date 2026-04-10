// Copyright (c) Deptrum Inc. All Rights Reserved.

#include <deptrum/deptrum.hpp>
#include <fstream>
#include <iostream>
#include "deptrum/hpp/dt_utils.hpp"
#include "utils.hpp"
#include "utils_glfw.hpp"

using namespace dt_utils;

int main(void) try {
  // create pipeline to manage the streams
  auto pipeline = std::make_shared<dt::Pipeline>();
  pipeline->Start();

  // Create a point cloud Filter, which will be used to generate pointcloud frame from depth and
  // color frames.
  auto point_cloud = std::make_shared<dt::PointCloudFilter>();

  // Create a simple OpenGL window for rendering:
  window app(1280, 720, "Deptrum Pointcloud Example");
  // Construct an object to manage view state
  glfw_state app_state;
  // register callbacks to allow manipulation of the pointcloud
  register_glfw_callbacks(app, app_state);

  // set to create point cloud format
  while (app) {
    auto frameset = pipeline->WaitForFrames(1000);
    if (frameset) {
      auto depth_frame = frameset->GetDepthFrame();
      auto color_frame = frameset->GetColorFrame();
      if (depth_frame && color_frame) {
        point_cloud->SetCreatePointFormat(kDtFormatRgbPoint);
        // process the frameset to generate point cloud frame (pass into a single depth frame to
        // process is also valid)
        std::shared_ptr<dt::Frame> frame = point_cloud->Process(frameset);

        // Upload the color frame to OpenGL
        app_state.tex.upload(color_frame);

        // Draw the pointcloud
        auto point_cloud_frame = frame->As<dt::PointsFrame>();
        app.set_new_frame_flag(true);
        draw_pointcloud(app.width(), app.height(), app_state, point_cloud_frame);
      } else if (depth_frame) {
        point_cloud->SetCreatePointFormat(kDtFormatPoint);
        // process the frameset to generate point cloud frame (pass into a single depth frame to
        // process is also valid)
        std::shared_ptr<dt::Frame> frame = point_cloud->Process(frameset);

        // Draw the pointcloud
        auto point_cloud_frame = frame->As<dt::PointsFrame>();
        app.set_new_frame_flag(true);
        draw_pointcloud(app.width(), app.height(), app_state, point_cloud_frame);
      }
    }
  }
  // stop the pipeline
  pipeline->Stop();

  return 0;
} catch (dt::Error& e) {
  std::cerr << e << std::endl;
  std::cout << "\nPress any key to exit.";
  dt_utils::WaitForKeyPressed();
  exit(EXIT_FAILURE);
}
