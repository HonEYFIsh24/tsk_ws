# C++ Sample：dt_device_playback

## Overview

This example demonstrates how to use the SDK to read and visualize data from a ROS bag file (.bag) with Deptrum camera streams.

### Knowledge

**Pipeline**: Manages data streams with multi-channel configuration, frame synchronization, and aggregation capabilities.  
**PlaybackDevice**: Reads sensor data from a ROS bag file and feeds it into the processing pipeline.

## code overview

1. Initialize Playback Device and Pipeline

    Create a playback device from a ROS bag file and configure the processing pipeline:

    ```cpp
        // Create a playback device with a Rosbag file
        std::shared_ptr<dt::PlaybackDevice> playback = std::make_shared<dt::PlaybackDevice>(filePath);
        // Create a pipeline with the playback device
        std::shared_ptr<dt::Pipeline> pipe = std::make_shared<dt::Pipeline>(playback);
    ```

2. Enable Recorded Streams
    Activate all sensor streams available in the bag file:

    ```cpp
        std::shared_ptr<dt::Config> config     = std::make_shared<dt::Config>();
        auto                        sensorList = playback->GetSensorList();
        for(uint32_t i = 0; i < sensorList->GetCount(); i++) {
            auto sensorType = sensorList->GetSensorType(i);

            config->EnableStream(sensorType);
        }
    ```

3. Start the Pipeline with the Config

    ```cpp
        pipe->Start(config);
    ```

4. Automatically restart playback when reaching file end:

    ```cpp
        playback->SetPlaybackStatusChangeCallback([&](DtPlaybackStatus status) {
            if(status == DT_PLAYBACK_STOPPED && !exited) {
                pipe->Stop();
                std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                pipe->Start(config);
            }
        });
    ```

## Run Sample

Press the 'Esc' key in the window to exit the program.

### Result


