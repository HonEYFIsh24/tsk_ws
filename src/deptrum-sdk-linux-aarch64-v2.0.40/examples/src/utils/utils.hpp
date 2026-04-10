// Copyright (c) Deptrum Inc. All Rights Reserved.

#pragma once
#include <stdint.h>
#include <deptrum/deptrum.hpp>
#include <sstream>
#include "utils_types.h"

namespace dt_utils {
char WaitForKeyPressed(uint32_t timeout_ms = 0);

uint64_t GetNowTimesMs();

int GetInputOption();

template<typename T>
std::string ToString(const T a_value, const int n = 6) {
  std::ostringstream out;
  out.precision(n);
  out << std::fixed << a_value;
  return std::move(out).str();
}

void CheckDeviceMode(std::shared_ptr<dt::Device> device);

void CheckDepthMode(std::shared_ptr<dt::Device> device);
std::shared_ptr<dt::StreamProfile> SelectStreamProfileList(std::shared_ptr<dt::Device> device);

}  // namespace dt_utils
