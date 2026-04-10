// Copyright (c) Deptrum Inc. All Rights Reserved.

#include <deptrum/deptrum.hpp>
#include <fstream>
#include <iostream>
#include "deptrum/hpp/dt_utils.hpp"
#include "utils.hpp"

#define KEY_ESC 27
#define KEY_R 82
#define KEY_r 114

int main(void) try {
  // create config to configure the pipeline streams
  auto config = std::make_shared<dt::Config>();

  // enable depth and color streams with specified format
  config->EnableVideoStream(kDtStreamDepth, kDtWidthAny, kDtHeightAny, kDtFpsAny, kDtFormatY16);
  config->EnableVideoStream(kDtStreamColor, kDtWidthAny, kDtHeightAny, kDtFpsAny, kDtFormatRgb);

  // create pipeline to manage the streams
  auto pipeline = std::make_shared<dt::Pipeline>();

  // Enable frame synchronization to ensure depth frame and color frame on output frameset are
  // synchronized.
  pipeline->EnableFrameSync();
  dt_utils::CheckDeviceMode(pipeline->GetDevice());
  dt_utils::CheckDepthMode(pipeline->GetDevice());

  // Start pipeline with config
  pipeline->Start();
  // pipeline->start(config);

  // Create a point cloud Filter, which will be used to generate pointcloud frame from depth and
  // color frames.
  auto point_cloud = std::make_shared<dt::PointCloudFilter>();

  // Create a Align Filter, which will be used to align depth frame and color frame.
  // auto align = std::make_shared<dt::Align>(kDtStreamColor);  // align depth frame to color frame

  // operation prompt
  std::cout << "Depth and Color stream are started!" << std::endl;
  std::cout << "Press R or r to create RGBD PointCloud and save to ply file! " << std::endl;
  std::cout << "Press D or d to create Depth PointCloud and save to ply file! " << std::endl;
  std::cout << "Press M or m to create RGBD PointCloud and save to Mesh ply file! " << std::endl;
  std::cout << "Press A or a to create intensity PointCloud and save to ply file! " << std::endl;
  std::cout << "Press ESC to exit! " << std::endl;

  while (true) {
    auto key = dt_utils::WaitForKeyPressed();
    if (key == 27) {
      break;
    }

    if (key == 'r' || key == 'R') {
      std::cout << "Save RGBD PointCloud to ply file, this will take some time..." << std::endl;

      std::shared_ptr<dt::FrameSet> frameset = nullptr;
      while (true) {
        frameset = pipeline->WaitForFrames(1000);
        if (frameset) {
          break;
        }
      }

      // align depth frame to color frame
      // auto aligned_frameset = align->process(frameset);

      // set to create RGBD point cloud format (will be effective only if color frame and depth
      // frame are contained in the frameset)
      point_cloud->SetCreatePointFormat(kDtFormatRgbPoint);

      // process the frameset to generate point cloud frame
      std::shared_ptr<dt::Frame> frame = point_cloud->Process(frameset);
      // std::shared_ptr<dt::Frame> frame = point_cloud->process(aligned_frameset);

      // save point cloud data to ply file
      if (frame != nullptr) {
        dt::PointCloudHelper::SavePointcloudToPly("RGBPoints.ply", frame, false, false, 50);
        std::cout << "RGBPoints.ply Saved" << std::endl;
      }
    } else if (key == 'd' || key == 'D') {
      std::cout << "Save Depth PointCloud to ply file, this will take some time..." << std::endl;

      std::shared_ptr<dt::FrameSet> frameset = nullptr;
      while (true) {
        frameset = pipeline->WaitForFrames(1000);
        if (frameset) {
          break;
        }
      }

      // set to create depth point cloud format
      // auto aligned_frameset = align->process(frameset);

      // set to create point cloud format
      point_cloud->SetCreatePointFormat(kDtFormatPoint);

      // process the frameset to generate point cloud frame (pass into a single depth frame to
      // process is also valid)
      std::shared_ptr<dt::Frame> frame = point_cloud->Process(frameset);
      // std::shared_ptr<dt::Frame> frame = point_cloud->process(aligned_frameset);

      // save point cloud data to ply file
      if (frame != nullptr) {
        dt::PointCloudHelper::SavePointcloudToPly("DepthPoints.ply", frame, false, false, 50);
        std::cout << "DepthPoints.ply Saved" << std::endl;
      }
    } else if (key == 'm' || key == 'M') {
      std::cout << "Save RGBD PointCloud(mesh) to ply file, this will take some time..."
                << std::endl;

      std::shared_ptr<dt::FrameSet> frameset = nullptr;
      while (true) {
        frameset = pipeline->WaitForFrames(1000);
        if (frameset) {
          break;
        }
      }

      // align depth frame to color frame
      // auto aligned_frameset = align->process(frameset);

      // set to create RGBD point cloud format (will be effective only if color frame and depth
      // frame are contained in the frameset)
      point_cloud->SetCreatePointFormat(kDtFormatRgbPoint);

      // process the frameset to generate point cloud frame
      std::shared_ptr<dt::Frame> frame = point_cloud->Process(frameset);
      if (frame != nullptr) {
        dt::PointCloudHelper::SavePointcloudToPly("ColorMeshPoints.ply", frame, false, true, 50);
        std::cout << "ColorMeshPoints.ply Saved" << std::endl;
      }
    } else if (key == 'a' || key == 'A') {
      std::cout << "Save AC PointCloud to ply file, this will take some time..." << std::endl;

      std::shared_ptr<dt::FrameSet> frameset = nullptr;
      while (true) {
        frameset = pipeline->WaitForFrames(1000);
        if (frameset) {
          break;
        }
      }

      // set to create depth point cloud format
      // auto aligned_frameset = align->process(frameset);

      // set to create point cloud format
      point_cloud->SetCreatePointFormat(kDtFormatAcPoint);

      // process the frameset to generate point cloud frame (pass into a single depth frame to
      // process is also valid)
      std::shared_ptr<dt::Frame> frame = point_cloud->Process(frameset);

      // save point cloud data to ply file
      if (frame != nullptr) {
        dt::PointCloudHelper::SavePointcloudToPly("AcDepthPoints.ply", frame, false, false, 50);
        std::cout << "AcDepthPoints.ply Saved" << std::endl;
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
