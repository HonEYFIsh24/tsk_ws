// #Copyright(c) Shenzhen Guangjian Technology Co., Ltd..All Rights Reserved.

#include <deptrum/deptrum.hpp>
#include <iostream>
#include <opencv2/opencv.hpp>
#include "utils.hpp"

void SaveDepthFrame(const std::shared_ptr<dt::DepthFrame> depth_frame, uint32_t frame_index);
void SaveColorFrame(const std::shared_ptr<dt::ColorFrame> color_frame, uint32_t frame_index);

int main() try {
  // Create a pipeline.
  std::shared_ptr<dt::Pipeline> pipeline = std::make_shared<dt::Pipeline>();

  // Create a config and enable the depth and color streams.
  std::shared_ptr<dt::Config> config = std::make_shared<dt::Config>();
  config->EnableStream(kDtStreamColor);
  config->EnableStream(kDtStreamDepth);

  uint32_t frame_index = 0;
  // Create a format converter filter.
  auto format_converter = std::make_shared<dt::FormatConvertFilter>();

  // Start the pipeline.
  pipeline->Start(config);

  // Drop several frames
  for (int i = 0; i < 15; ++i) {
    auto lost = pipeline->WaitForFrames(1000);
  }

  while (true) {
    // Wait for frame_set from the pipeline.
    std::shared_ptr<dt::FrameSet> frame_set = pipeline->WaitForFrames(1000);

    if (!frame_set) {
      std::cout << "No frames received in 100ms..." << std::endl;
      continue;
    }

    if (++frame_index >= 5) {
      std::cout << "The demo is over, please press ESC to exit manually!" << std::endl;
      break;
    }

    // Get the depth and color frames.
    auto depth_frame = frame_set->GetFrame(kDtFrameDepth)->As<dt::DepthFrame>();
    auto color_frame = frame_set->GetFrame(kDtFrameColor)->As<dt::ColorFrame>();

    // Convert the color frame to RGB format.
    if (color_frame->GetFormat() != kDtFormatRgb) {
      if (color_frame->GetFormat() == kDtFormatMjpg) {
        format_converter->SetFormatConvertType(kDtFormatMjpgToRgb);
      } else if (color_frame->GetFormat() == kDtFormatUyvy) {
        format_converter->SetFormatConvertType(kDtFormatUyvyToRgb);
      } else if (color_frame->GetFormat() == kDtFormatYuyv) {
        format_converter->SetFormatConvertType(kDtFormatYuyvToRgb);
      } else {
        std::cout << "Color format is not support!" << std::endl;
        continue;
      }
      color_frame = format_converter->Process(color_frame)->As<dt::ColorFrame>();
    }
    // Processed the color frames to BGR format, use OpenCV to save to disk.
    format_converter->SetFormatConvertType(kDtFormatRgbToBgr);
    color_frame = format_converter->Process(color_frame)->As<dt::ColorFrame>();

    SaveDepthFrame(depth_frame, frame_index);
    SaveColorFrame(color_frame, frame_index);
  }

  // Stop the Pipeline, no frame data will be generated
  pipeline->Stop();

  std::cout << "Press any key to exit." << std::endl;
  dt_utils::WaitForKeyPressed();

  return 0;
} catch (dt::Error& e) {
  std::cerr << e << std::endl;
  std::cout << "\nPress any key to exit.";
  dt_utils::WaitForKeyPressed();
  exit(EXIT_FAILURE);
}

void SaveDepthFrame(const std::shared_ptr<dt::DepthFrame> depth_frame, const uint32_t frame_index) {
  std::vector<int> params;
  params.push_back(cv::IMWRITE_PNG_COMPRESSION);
  params.push_back(0);
  params.push_back(cv::IMWRITE_PNG_STRATEGY);
  params.push_back(cv::IMWRITE_PNG_STRATEGY_DEFAULT);
  std::string depth_name = "Depth_" + std::to_string(depth_frame->GetWidth()) + "x" +
                           std::to_string(depth_frame->GetHeight()) + "_" +
                           std::to_string(frame_index) + "_" +
                           std::to_string(depth_frame->GetTimeStampUs()) + "us.png";
  cv::Mat depth_mat(depth_frame->GetHeight(),
                    depth_frame->GetWidth(),
                    CV_16UC1,
                    depth_frame->GetData());
  cv::imwrite(depth_name, depth_mat, params);
  std::cout << "Depth saved:" << depth_name << std::endl;
}

void SaveColorFrame(const std::shared_ptr<dt::ColorFrame> color_frame, const uint32_t frame_index) {
  std::vector<int> params;
  params.push_back(cv::IMWRITE_PNG_COMPRESSION);
  params.push_back(0);
  params.push_back(cv::IMWRITE_PNG_STRATEGY);
  params.push_back(cv::IMWRITE_PNG_STRATEGY_DEFAULT);
  std::string color_name = "Color_" + std::to_string(color_frame->GetWidth()) + "x" +
                           std::to_string(color_frame->GetHeight()) + "_" +
                           std::to_string(frame_index) + "_" +
                           std::to_string(color_frame->GetTimeStampUs()) + "us.png";
  cv::Mat depth_mat(color_frame->GetHeight(),
                    color_frame->GetWidth(),
                    CV_8UC3,
                    color_frame->GetData());
  cv::imwrite(color_name, depth_mat, params);
  std::cout << "Color saved:" << color_name << std::endl;
}