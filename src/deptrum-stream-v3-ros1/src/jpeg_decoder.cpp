// Copyright (c) Deptrum Inc. All Rights Reserved.
#include <deptrum_camera/jpeg_decoder.h>

namespace deptrum_camera {
JpegDecoder::JpegDecoder(int width, int height) : width_(width), height_(height) {}

JpegDecoder::~JpegDecoder() = default;

}  // namespace deptrum_camera