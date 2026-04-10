// Copyright (c) Deptrum Inc. All Rights Reserved.

#pragma once

#include <string>
#include <vector>
#include "deptrum/deptrum.hpp"
#include "utils.h"

namespace deptrum_camera {
class JpegDecoder {
 public:
  JpegDecoder(int width, int height);

  virtual ~JpegDecoder();

  virtual bool Decode(const std::shared_ptr<dt::ColorFrame> &frame, uint8_t *dest) { return false; }

 protected:
  int width_ = 0;
  int height_ = 0;
};
}  // namespace deptrum_camera
