# Capture with C

This is a capture guide to start device streams using the Deptrum SDK C API.

## Overview

### Knowledge

**Pipeline** is a pipeline for processing data streams, providing multi-channel stream configuration, switching, frame aggregation, and frame synchronization functions.

**Frameset** is a combination of different types of Frames

## Code Overview

### 1. Create pipeline

```c
// Used to return SDK interface error information.
dt_error *error = NULL;

// Create a pipeline to manage the streams
dt_pipeline *pipe = dt_create_pipeline(&error);
```

### 2. Start pipeline

```c
// Start Pipeline with default configuration (At default, the pipeline will start with the color and depth streams)
dt_pipeline_start(pipe, &error);
```

### 3. Get frameset from pipeline

```c
// Wait for frameset from pipeline, with a timeout of 1000 milliseconds.
dt_frame *frameset = dt_pipeline_wait_for_frames(pipe, 1000, &error);
CHECK_DT_ERROR_EXIT(&error);

// If frameset is NULL, timeout occurred, continue to next iteration.
if(frameset == NULL) {
    continue;
}

// Get the color and depth frames from the frameset and calculate their frame rate.
calculate_and_print_frame_rate(frameset);

// delete frameset
dt_delete_frame(frameset, &error);
CHECK_DT_ERROR_EXIT(&error);
```

## Run Sample

If you are on Windows, you can switch to the directory `deptrum-dev/build/win_XX/bin` to find the `dt_quick_start_c.exe`.

If you are on linux, you can switch to the directory `deptrum-dev/build/linux_XX/bin` to find the `dt_quick_start_c`.

### Key introduction

Press 'ESC' key to stop the pipeline and exit the program.

### Result
