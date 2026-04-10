# Depth with C

This is a depth guide to get depth stream and depth image by using the Deptrum SDK C API.

## Overview

### Knowledge

**Pipeline** is a pipeline for processing data streams, providing multi-channel stream configuration, switching, frame aggregation, and frame synchronization functions.

**Frameset** is a combination of different types of Frames.

**Depth_frame** is one of the frames in frameset, it contains the frames's  esolution information and depth data.

## Code Overview

### 1. Create instances

Create the pipeline instance using the default configuration and create a config instance to enable or disable the streams.

```c
// Used to return SDK interface error information.
dt_error *error = NULL;

// Create a pipeline object to manage the streams.
dt_pipeline *pipeline = dt_create_pipeline(&error);
check_dt_error(&error);

// Crete a config object to configure the pipeline streams.
dt_config *config = dt_create_config(&error);
check_dt_error(&error);
```

### 2. Start pipeline

enable the depth stream, and then start the pipeline.

```c
// enable depth stream with default stream profile.
dt_config_enable_stream(config, DT_STREAM_DEPTH, &error);
check_dt_error(&error);

// Start Pipeline with the configured streams.
dt_pipeline_start_with_config(pipeline, config, &error);
check_dt_error(&error);
```

### 3. Get  depth stream

Get the frameset from pipeline, and get the depth frame from frameset.

```c
// Wait for a frameset, timeout after 1000 milliseconds.
const dt_frame *frameset = dt_pipeline_wait_for_frames(pipeline, 1000, &error);
check_dt_error(&error);

// If no frameset is available with in the timeout, continue waiting.
if(frameset == NULL) {
    continue;
}

// Get the depth frame from frameset
const dt_frame *depth_frame = dt_frameset_get_depth_frame(frameset, &error);
check_dt_error(&error);
```

### 4. Print the distance of the center pixel

Print the distance of the center pixel every 30 frames to reduce output.

```c
if(index % 30 == 0) {
    // Get the width of the depth frame.
    uint32_t width = dt_video_frame_get_width(depth_frame, &error);
    check_dt_error(&error);

    // Get the height of the depth frame.
    uint32_t height = dt_video_frame_get_height(depth_frame, &error);
    check_dt_error(&error);

    // Get the scale of the depth frame.
    float scale = dt_depth_frame_get_value_scale(depth_frame, &error);
    check_dt_error(&error);

    // Get the data of the depth frame, cast to uint16_t* to access the pixels directly for Y16 format.
    uint16_t *data = (uint16_t *)dt_frame_get_data(depth_frame, &error);
    check_dt_error(&error);

    // pixel value multiplied by scale is the actual distance value in millimeters
    float center_distance = data[width * height / 2 + width / 2] * scale;

    // attention: if the distance is 0, it means that the depth camera cannot detect the object (may be out of detection range)
    printf("Facing an object at a distance of %.3f mm away.\n", center_distance);
}
```

### 5. Stop pipeline

```c
dt_pipeline_stop(pipeline, &error);
```

## Run Sample

If you are on Windows, you can switch to the directory `deptrum-dev/build/win_XX/bin` to find the `dt_depth_c.exe`.

If you are on linux, you can switch to the directory `deptrum-dev/build/linux_XX/bin` to find the `dt_depth_c`.

### Key introduction

Press 'ESC' key to stop the pipeline and exit the program.

### Result
