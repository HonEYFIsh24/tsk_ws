// Copyright (c) Deptrum Inc. All Rights Reserved.

#include <deptrum/h/dt_types.h>
#include <atomic>
#include <chrono>
#include <deptrum/deptrum.hpp>
#include <iomanip>
#include <thread>
#include "utils.hpp"

std::thread input_watch_thread;
std::atomic<bool> is_running(true);
void InputWatcher();
const char* MetadataTypeToString(DtFrameMetadataType type);
const char* FrameTypeToString(DtFrameType type);

int main() try {
  // Create a pipeline.
  dt::Pipeline pipe;

  // Configure which streams to enable or disable for the Pipeline by creating a Config.
  std::shared_ptr<dt::Config> config = std::make_shared<dt::Config>();

  // Enable color palm stream.
  config->EnablePalmStream();

  // Start the pipeline with config.
  pipe.Start(config);

  // Get key input
  input_watch_thread = std::thread(InputWatcher);
  input_watch_thread.detach();

  while (is_running) {
    // Wait for frame_set from the pipeline, the default timeout is 1000ms.
    auto frame_set = pipe.WaitForFrames();
    if (!frame_set) {
      continue;
    }

    // Get the count of frames in the frame_set
    auto frame_count = frame_set->GetCount();

    for (uint32_t i = 0; i < frame_count; i++) {
      // Get the frame from frame_set
      auto frame = frame_set->GetFrameByIndex(i);
      // Get the metadata of the frame, and print the metadata every 30 frames
      std::cout << std::endl;
      std::cout << "frame    type: " << FrameTypeToString(frame->GetType()) << std::endl;
      std::cout << "frame    size: " << frame->GetDataSize() << std::endl;

      if (frame->GetType() == kDtFramePalm) {
        const float* data = reinterpret_cast<const float*>(frame->GetData());
        std::cout << "frame    data: " << std::endl;
        for (uint32_t j = 0; j < frame->GetDataSize() / sizeof(float); j++) {
          std::cout << data[j] << " ";
        }
        std::cout << std::endl;
      }

      for (uint32_t j = 0; j < static_cast<uint32_t>(kDtFrameMetadataTypeCount); j++) {
        // If the frame has the metadata, get the metadata value
        if (frame->HasMetadata(static_cast<DtFrameMetadataType>(j))) {
          std::cout << "metadata type: " << std::left << std::setw(50)
                    << MetadataTypeToString(static_cast<DtFrameMetadataType>(j))
                    << " metadata value: "
                    << frame->GetMetadataValue(static_cast<DtFrameMetadataType>(j)) << std::endl;
        }
      }
    }
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

void InputWatcher() {
  char input = dt_utils::WaitForKeyPressed();
  if (input == ESC_KEY) {
    is_running = false;
  }
}

const char* MetadataTypeToString(DtFrameMetadataType type) {
  switch (type) {
    case kDtFrameMetadataTypePalmWorkMode:
      return "kDtFrameMetadataTypePalmWorkMode";
    case kDtFrameMetadataTypeHandType:
      return "kDtFrameMetadataTypeHandType";
    default:
      return "unknown metadata type";
  }
}

const char* FrameTypeToString(DtFrameType type) {
  switch (type) {
    case kDtFrameVideo:
      return "kDtFrameVideo";
    case kDtFrameIr:
      return "kDtFrameIr";
    case kDtFrameColor:
      return "kDtFrameColor";
    case kDtFrameDepth:
      return "kDtFrameDepth";
    case kDtFrameAccel:
      return "kDtFrameAccel";
    case kDtFrameGyro:
      return "kDtFrameGyro";
    case kDtFrameIrLeft:
      return "kDtFrameIrLeft";
    case kDtFrameIrRight:
      return "kDtFrameIrRight";
    case kDtFrameColorLeft:
      return "kDtFrameColorLeft";
    case kDtFrameColorRight:
      return "kDtFrameColorRight";
    case kDtFramePalm:
      return "kDtFramePalm";
    default:
      return "unknown frame type";
  }
}