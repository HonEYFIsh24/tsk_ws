// Copyright (c) Deptrum Inc. All Rights Reserved.

#pragma once
#include "utils.h"
#include <opencv2/opencv.hpp>

#include "jpeg_decoder.h"
#include <NvJpegDecoder.h>
#include <NvUtils.h>
#include <NvV4l2Element.h>
#include <NvJpegDecoder.h>
#include <NvV4l2Element.h>

namespace deptrum_camera {
class JetsonNvJpegDecoder : public JpegDecoder {
 public:
  JetsonNvJpegDecoder(int width, int height);
  ~JetsonNvJpegDecoder() override;

  bool Decode(const std::shared_ptr<dt::ColorFrame>& frame, uint8_t* dest) override;

 private:
  NvJPEGDecoder* decoder_;
  int save_count_ = 0;
};
}  // namespace deptrum_camera