// Copyright (c) Deptrum Inc. All Rights Reserved.

#include <deptrum/deptrum.h>
#include <stdio.h>
#include <stdlib.h>
#include "utils_c.h"
#include "utils_types.h"

void calculate_and_print_frame_rate(const dt_frame* frameset) {
  // Initialize variables
  static float color_count = 0;
  static float depth_count = 0;
  static uint64_t color_timestamp_last = 0;
  static uint64_t depth_timestamp_last = 0;
  if (color_timestamp_last == 0) {
    color_timestamp_last = dt_utils_get_current_timestamp_ms();
  }
  if (depth_timestamp_last == 0) {
    depth_timestamp_last = dt_utils_get_current_timestamp_ms();
  }

  dt_error* error = NULL;

  // Get color frame from frameset.
  const dt_frame* color_frame = dt_frameset_get_frame(frameset, kDtFrameColor, &error);
  CHECK_DT_ERROR_EXIT(&error);

  // calculate and print color frame rate
  if (color_frame != NULL) {
    color_count++;
    // Get timestamp from color frame.
    uint64_t color_timestamp_current = dt_utils_get_current_timestamp_ms();
    uint64_t duration = color_timestamp_current - color_timestamp_last;
    if (duration > 1000) {  // calculate frame rate every second

      uint64_t index = dt_frame_get_index(color_frame, &error);
      CHECK_DT_ERROR_EXIT(&error);

      uint32_t width = dt_video_frame_get_width(color_frame, &error);
      CHECK_DT_ERROR_EXIT(&error);

      uint32_t height = dt_video_frame_get_height(color_frame, &error);
      CHECK_DT_ERROR_EXIT(&error);

      double frame_rate = color_count / (duration / 1000.0);

      printf("Color frame index: %d, width: %d, height: %d, frame rate: %.2f\n",
             (uint32_t) index,
             width,
             height,
             frame_rate);

      color_count = 0;
      color_timestamp_last = color_timestamp_current;
    }

    // delete color frame
    dt_delete_frame(color_frame);
  }

  // Get depth frame from frameset.
  const dt_frame* depth_frame = dt_frameset_get_frame(frameset, kDtFrameDepth, &error);
  CHECK_DT_ERROR_EXIT(&error);

  // calculate and print depth frame rate
  if (depth_frame != NULL) {
    depth_count++;
    // Get timestamp from depth frame.
    uint64_t depth_timestamp_current = dt_utils_get_current_timestamp_ms();
    uint64_t duration = depth_timestamp_current - depth_timestamp_last;
    if (duration > 1000) {  // 1 seconds
      uint64_t index = dt_frame_get_index(depth_frame, &error);
      CHECK_DT_ERROR_EXIT(&error);

      uint32_t width = dt_video_frame_get_width(depth_frame, &error);
      CHECK_DT_ERROR_EXIT(&error);

      uint32_t height = dt_video_frame_get_height(depth_frame, &error);
      CHECK_DT_ERROR_EXIT(&error);

      double frame_rate = depth_count / (duration / 1000.0);

      printf("Depth frame index: %d, width: %d, height: %d, frame rate: %.2f\n",
             (uint32_t) index,
             width,
             height,
             frame_rate);

      depth_count = 0;
      depth_timestamp_last = depth_timestamp_current;
    }

    // delete depth frame
    dt_delete_frame(depth_frame);
  }
}

int main(void) {
  // Used to return SDK interface error information.
  dt_error* error = NULL;

  // Create a pipeline to manage the streams
  dt_pipeline* pipe = dt_create_pipeline(&error);
  CHECK_DT_ERROR_EXIT(&error);

  // Start Pipeline with default configuration (At default, the pipeline will start with the color
  // and depth streams)
  dt_pipeline_start(pipe, &error);
  CHECK_DT_ERROR_EXIT(&error);

  printf("Streams have been started. Press 'ESC' key to stop the pipeline and exit the program.\n");

  // Main loop, continuously wait for frames and print their index and rate.
  while (true) {
    char key = dt_utils_wait_for_key_press(1);
    if (key == ESC_KEY) {
      break;
    }

    // Wait for frameset from pipeline, with a timeout of 1000 milliseconds.
    dt_frame* frameset = dt_pipeline_wait_for_frames(pipe, 1000, &error);
    CHECK_DT_ERROR_EXIT(&error);

    // If frameset is NULL, timeout occurred, continue to next iteration.
    if (frameset == NULL) {
      continue;
    }

    // Get the color and depth frames from the frameset and calculate their frame rate.
    calculate_and_print_frame_rate(frameset);

    // delete frameset
    dt_delete_frame(frameset);
  }

  // Stop Pipeline
  dt_delete_pipeline(pipe);

  return 0;
}
