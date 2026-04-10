// Copyright (c) Deptrum Inc. All Rights Reserved.

#include <deptrum/deptrum.hpp>
#include "deptrum/hpp/dt_frame.hpp"
#include "deptrum/hpp/dt_utils.hpp"

void PrintUsage();
std::string InputWatcher();
void Transform2dto2d(std::shared_ptr<dt::Frame> color_frame,
                     std::shared_ptr<dt::Frame> depth_frame);
void Transform2dto3d(std::shared_ptr<dt::Frame> color_frame,
                     std::shared_ptr<dt::Frame> depth_frame);
void Transform3dto2d(std::shared_ptr<dt::Frame> color_frame,
                     std::shared_ptr<dt::Frame> depth_frame);
void Transform3dto3d(std::shared_ptr<dt::Frame> color_frame,
                     std::shared_ptr<dt::Frame> depth_frame);

int main(void) try {
  // Configure which streams to enable or disable for the Pipeline by creating a Config
  auto config = std::make_shared<dt::Config>();

  // enable depth and color streams with specified format
  config->EnableVideoStream(kDtStreamDepth, kDtWidthAny, kDtHeightAny, kDtFpsAny, kDtFormatY16);
  config->EnableVideoStream(kDtStreamColor, kDtWidthAny, kDtHeightAny, kDtFpsAny, kDtFormatRgb);

  // Create a pipeline with default device to manage stream
  auto pipe = std::make_shared<dt::Pipeline>();

  std::string test_type = "1";

  // Start the pipeline with config
  pipe->Start(config);
  while (1) {
    PrintUsage();
    test_type = InputWatcher();

    // Wait for a frameset from the pipeline
    auto frame_set = pipe->WaitForFrames(1000);
    if (frame_set == nullptr) {
      continue;
    }

    // Get the color frame and check its validity
    auto color_frame = frame_set->GetFrame(kDtFrameColor);

    // Get the depth frame and check its validity
    auto depth_frame = frame_set->GetFrame(kDtFrameDepth);

    if (test_type == "1") {
      Transform2dto2d(color_frame, depth_frame);
    } else if (test_type == "2") {
      Transform2dto3d(color_frame, depth_frame);
    } else if (test_type == "3") {
      Transform3dto3d(color_frame, depth_frame);
    } else if (test_type == "4") {
      Transform3dto2d(color_frame, depth_frame);
    } else {
      std::cout << "Invalid command" << std::endl;
    }
  }

  pipe->Stop();
  return 0;
} catch (dt::Error& e) {
  std::cerr << e << std::endl;
  exit(EXIT_FAILURE);
}

std::string InputWatcher() {
  while (true) {
    std::string cmd;
    std::cout << "\nInput command:  ";
    std::getline(std::cin, cmd);
    if (cmd == "quit" || cmd == "q") {
      exit(EXIT_SUCCESS);
    }
    return cmd;
  }
}
void PrintUsage() {
  std::cout << "Support commands:" << std::endl;
  std::cout << "    1 - transform 2d to 2d" << std::endl;
  std::cout << "    2 - transform 2d to 3d" << std::endl;
  std::cout << "    3 - transform 3d to 3d" << std::endl;
  std::cout << "    4 - transform 3d to 2d" << std::endl;

  std::cout << "--------------------------------" << std::endl;
  std::cout << "    quit / q- quit application" << std::endl;
}

void print_ruslt(std::string msg, DtPoint2f source_pixel, DtPoint2f target_pixel) {
  std::cout << msg << ":"
            << "(" << source_pixel.x << ", " << source_pixel.y << ") -> (" << target_pixel.x << ", "
            << target_pixel.y << ")" << std::endl;
}

void print_ruslt(std::string msg,
                 DtPoint2f source_pixel,
                 DtPoint3f target_pixel,
                 float depth_value) {
  std::cout << msg << ":"
            << "depth " << depth_value << " (" << source_pixel.x << ", " << source_pixel.y
            << ") -> (" << target_pixel.x << ", " << target_pixel.y << ", " << target_pixel.z << ")"
            << std::endl;
}

void print_ruslt(std::string msg, DtPoint3f source_pixel, DtPoint2f target_pixel) {
  std::cout << msg << ":"
            << "(" << source_pixel.x << ", " << source_pixel.y << ", " << source_pixel.z << ") -> ("
            << target_pixel.x << ", " << target_pixel.y << ")" << std::endl;
}

void print_ruslt(std::string msg, DtPoint3f source_pixel, DtPoint3f target_pixel) {
  std::cout << msg << ":"
            << "(" << source_pixel.x << ", " << source_pixel.y << ", " << source_pixel.z << ") -> ("
            << target_pixel.x << ", " << target_pixel.y << ", " << target_pixel.z << ")"
            << std::endl;
}

// test the transform from one 2D coordinate system to another
void Transform2dto2d(std::shared_ptr<dt::Frame> color_frame,
                     std::shared_ptr<dt::Frame> depth_frame) {
  // Get the width and height of the color and depth frames
  auto color_frame_width = color_frame->As<dt::VideoFrame>()->GetWidth();
  auto depth_frame_width = depth_frame->As<dt::VideoFrame>()->GetWidth();
  auto color_frame_height = color_frame->As<dt::VideoFrame>()->GetHeight();
  auto depth_frame_height = depth_frame->As<dt::VideoFrame>()->GetHeight();

  // Get the stream profiles for the color and depth frames
  auto color_profile = color_frame->GetStreamProfile();
  auto depth_profile = depth_frame->GetStreamProfile();
  auto extrinsic_d2_c = depth_profile->GetExtrinsicTo(color_profile);

  // Get the intrinsic and distortion parameters for the color and depth streams
  auto color_intrinsic = color_profile->As<dt::VideoStreamProfile>()->GetIntrinsic();
  auto color_distortion = color_profile->As<dt::VideoStreamProfile>()->GetDistortion();
  auto depth_intrinsic = depth_profile->As<dt::VideoStreamProfile>()->GetIntrinsic();
  auto depth_distortion = depth_profile->As<dt::VideoStreamProfile>()->GetDistortion();
  // Access the depth data from the frame
  uint16_t* p_depth_data = (uint16_t*) depth_frame->GetData();
  uint16_t convert_area_width = 3;
  uint16_t convert_area_height = 3;

  // Transform depth values to the color frame's coordinate system
  for (uint32_t i = depth_frame_height / 2; i < (depth_frame_height / 2 + convert_area_height);
       i++) {
    for (uint32_t j = depth_frame_width / 2; j < (depth_frame_width / 2 + convert_area_width);
         j++) {
      DtPoint2f source_pixel = {static_cast<float>(j), static_cast<float>(i)};
      DtPoint2f target_pixel = {};
      float depth_value = (float) p_depth_data[i * depth_frame_width + j];
      if (depth_value == 0) {
        std::cout
            << "The depth value is 0, so it's recommended to point the camera at a flat surface"
            << std::endl;
        continue;
      }

      // Demonstrate Depth 2D converted to Color 2D
      bool result = dt::CoordinateTransformHelper::Transform2dto2d(source_pixel,
                                                                   depth_value,
                                                                   depth_intrinsic,
                                                                   depth_distortion,
                                                                   color_intrinsic,
                                                                   color_distortion,
                                                                   extrinsic_d2_c,
                                                                   &target_pixel);

      // Check transform result and whether the target pixel is within the color frame
      if (!result || target_pixel.y < 0 || target_pixel.x < 0 ||
          target_pixel.y > color_frame_width || target_pixel.x > color_frame_width) {
        continue;
      }

      // Calculate the index position of the target pixel in the transform data buffer
      auto index = (((uint32_t) target_pixel.y * color_frame_width) + (uint32_t) target_pixel.x);
      if (index > color_frame_width * color_frame_height) {
        continue;
      }

      print_ruslt("depth to color: depth image coordinate transform to color image coordinate",
                  source_pixel,
                  target_pixel);
    }
  }
}

// test the transform from 2D to 3D coordinates
void Transform2dto3d(std::shared_ptr<dt::Frame> color_frame,
                     std::shared_ptr<dt::Frame> depth_frame) {
  // Get the width and height of the color and depth frames
  auto depth_frame_width = depth_frame->As<dt::VideoFrame>()->GetWidth();
  auto depth_frame_height = depth_frame->As<dt::VideoFrame>()->GetHeight();

  // Get the stream profiles for the color and depth frames
  auto color_profile = color_frame->GetStreamProfile();
  auto depth_profile = depth_frame->GetStreamProfile();
  auto extrinsic_d2_c = depth_profile->GetExtrinsicTo(color_profile);

  // Get the intrinsic and distortion parameters for the color and depth streams
  auto depth_intrinsic = depth_profile->As<dt::VideoStreamProfile>()->GetIntrinsic();
  // Access the depth data from the frame
  uint16_t* p_depth_data = (uint16_t*) depth_frame->GetData();
  uint16_t convert_area_width = 3;
  uint16_t convert_area_height = 3;

  // Transform depth values to the color frame's coordinate system
  for (uint32_t i = depth_frame_height / 2; i < (depth_frame_height / 2 + convert_area_height);
       i++) {
    for (uint32_t j = depth_frame_width / 2; j < (depth_frame_width / 2 + convert_area_width);
         j++) {
      // Get the coordinates of the current pixel
      DtPoint2f source_pixel = {static_cast<float>(j), static_cast<float>(i)};
      DtPoint3f target_pixel = {};
      // Get the depth value of the current pixel
      float depth_value = (float) p_depth_data[i * depth_frame_width + j];
      if (depth_value == 0) {
        std::cout
            << "The depth value is 0, so it's recommended to point the camera at a flat surface"
            << std::endl;
        continue;
      }

      // Perform the 2D to 3D transform
      bool result = dt::CoordinateTransformHelper::Transform2dto3d(source_pixel,
                                                                   depth_value,
                                                                   depth_intrinsic,
                                                                   extrinsic_d2_c,
                                                                   &target_pixel);
      if (!result) {
        continue;
      }

      print_ruslt("2d to 3D: pixel coordinates and depth transform to point in 3D space",
                  source_pixel,
                  target_pixel,
                  depth_value);
    }
  }
}

// test the transform from 3D coordinates to 3D coordinates
void Transform3dto3d(std::shared_ptr<dt::Frame> color_frame,
                     std::shared_ptr<dt::Frame> depth_frame) {
  // Get the width and height of the color and depth frames
  auto depth_frame_width = depth_frame->As<dt::VideoFrame>()->GetWidth();
  auto depth_frame_height = depth_frame->As<dt::VideoFrame>()->GetHeight();

  // Get the stream profiles for the color and depth frames
  auto color_profile = color_frame->GetStreamProfile();
  auto depth_profile = depth_frame->GetStreamProfile();
  auto extrinsic_d2_c = depth_profile->GetExtrinsicTo(color_profile);
  auto extrinsic_c2_d = color_profile->GetExtrinsicTo(depth_profile);

  // Get the intrinsic and distortion parameters for the color and depth streams
  auto depth_intrinsic = depth_profile->As<dt::VideoStreamProfile>()->GetIntrinsic();
  // Access the depth data from the frame
  uint16_t* p_depth_data = (uint16_t*) depth_frame->GetData();
  uint16_t convert_area_width = 3;
  uint16_t convert_area_height = 3;

  // Transform depth values to the color frame's coordinate system
  for (uint32_t i = depth_frame_height / 2; i < (depth_frame_height / 2 + convert_area_height);
       i++) {
    for (uint32_t j = depth_frame_width / 2; j < (depth_frame_width / 2 + convert_area_width);
         j++) {
      // Get the coordinates of the current pixel
      DtPoint2f source_pixel = {static_cast<float>(j), static_cast<float>(i)};
      DtPoint3f tmp_target_pixel = {};
      DtPoint3f target_pixel = {};
      // Get the depth value of the current pixel
      float depth_value = (float) p_depth_data[i * depth_frame_width + j];
      if (depth_value == 0) {
        std::cout
            << "The depth value is 0, so it's recommended to point the camera at a flat surface"
            << std::endl;
        continue;
      }

      // Perform the 2D to 3D transform
      bool result = dt::CoordinateTransformHelper::Transform2dto3d(source_pixel,
                                                                   depth_value,
                                                                   depth_intrinsic,
                                                                   extrinsic_d2_c,
                                                                   &tmp_target_pixel);
      if (!result) {
        continue;
      }
      print_ruslt("2d to 3D: pixel coordinates and depth transform to point in 3D space",
                  source_pixel,
                  tmp_target_pixel,
                  depth_value);

      // Perform the 3D to 3D transform
      result = dt::CoordinateTransformHelper::Transform3dto3d(tmp_target_pixel,
                                                              extrinsic_c2_d,
                                                              &target_pixel);
      if (!result) {
        continue;
      }
      print_ruslt("3d to 3D: transform 3D coordinates relative to one sensor to 3D coordinates "
                  "relative to another viewpoint",
                  tmp_target_pixel,
                  target_pixel);
    }
  }
}

// test the transform from 3D coordinates back to 2D coordinates
void Transform3dto2d(std::shared_ptr<dt::Frame> color_frame,
                     std::shared_ptr<dt::Frame> depth_frame) {
  // Get the width and height of the color and depth frames
  auto depth_frame_width = depth_frame->As<dt::VideoFrame>()->GetWidth();
  auto depth_frame_height = depth_frame->As<dt::VideoFrame>()->GetHeight();

  // Get the stream profiles for the color and depth frames
  auto color_profile = color_frame->GetStreamProfile();
  auto depth_profile = depth_frame->GetStreamProfile();
  auto extrinsic_d2_c = depth_profile->GetExtrinsicTo(color_profile);
  auto extrinsic_c2_d = color_profile->GetExtrinsicTo(depth_profile);

  // Get the intrinsic and distortion parameters for the color and depth streams
  auto depth_intrinsic = depth_profile->As<dt::VideoStreamProfile>()->GetIntrinsic();
  auto depth_distortion = depth_profile->As<dt::VideoStreamProfile>()->GetDistortion();
  // Access the depth data from the frame
  uint16_t* p_depth_data = (uint16_t*) depth_frame->GetData();
  uint16_t convert_area_width = 3;
  uint16_t convert_area_height = 3;

  // Transform depth values to the color frame's coordinate system
  for (uint32_t i = depth_frame_height / 2; i < (depth_frame_height / 2 + convert_area_height);
       i++) {
    for (uint32_t j = depth_frame_width / 2; j < (depth_frame_width / 2 + convert_area_width);
         j++) {
      // Get the coordinates of the current pixel
      DtPoint2f source_pixel = {static_cast<float>(j), static_cast<float>(i)};
      DtPoint3f tmp_target_pixel = {};
      DtPoint2f target_pixel = {};
      // Get the depth value of the current pixel
      float depth_value = (float) p_depth_data[i * depth_frame_width + j];
      if (depth_value == 0) {
        std::cout
            << "The depth value is 0, so it's recommended to point the camera at a flat surface"
            << std::endl;
        continue;
      }

      // Perform the 2D to 3D transform
      bool result = dt::CoordinateTransformHelper::Transform2dto3d(source_pixel,
                                                                   depth_value,
                                                                   depth_intrinsic,
                                                                   extrinsic_d2_c,
                                                                   &tmp_target_pixel);
      if (!result) {
        continue;
      }
      print_ruslt("depth 2d to 3D: pixel coordinates and depth transform to point in 3D space",
                  source_pixel,
                  tmp_target_pixel,
                  depth_value);

      // Perform the 3D to 2D transform
      result = dt::CoordinateTransformHelper::Transform3dto2d(tmp_target_pixel,
                                                              depth_intrinsic,
                                                              depth_distortion,
                                                              extrinsic_c2_d,
                                                              &target_pixel);
      if (!result) {
        continue;
      }
      print_ruslt("3d to depth 2d : point in 3D space transform to the corresponding pixel "
                  "coordinates in an image",
                  tmp_target_pixel,
                  target_pixel);
    }
  }
}
