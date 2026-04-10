# C++ Sample: dt_multi_streams

## Overview

Use SDK to obtain multiple camera data streams and output them

### Knowledge

Pipeline is a pipeline for processing data streams, providing multi-channel stream configuration, switching, frame aggregation, and frame synchronization functions.

Device is a class that can be used to get device information, parameters, and a list of contained sensors.

Sensor can be used to obtain different components of the camera and the stream of the component, for example, RGB, IR, Depth stream can be obtained through the RGB, IR, Depth sensor.

Frameset is a combination of different types of Frames.

## code overview

1. Configure the output video stream in addition to imu data, such as depth, color, etc.

    ```cpp
        // Get sensor list from device.
        auto sensorList = device->GetSensorList();

        for(uint32_t i = 0; i < sensorList->GetCount(); i++) {
            // Get sensor type.
            auto sensorType = sensorList->GetSensorType(i);

            // exclude gyro and accel sensors.
            if(sensorType == kDtSensorGyro || sensorType == kDtSensorAccel) {
                continue;
            }

            // enable the stream.
            config->EnableStream(sensorType);
        }

        // Start the pipeline with config
        std::mutex                          frameMutex;
        std::shared_ptr<dt::FrameSet> renderFrameSet;
        pipe.Start(config, [&](std::shared_ptr<dt::FrameSet> frameSet) {
            std::lock_guard<std::mutex> lock(frameMutex);
            renderFrameSet = frameSet;
        });
    ```

2. Instantiate the pipeline, configure IMU related information and start streaming

    ```cpp
        // The IMU frame rate is much faster than the video, so it is advisable to use a separate pipeline to obtain IMU data.
        auto                                dev         = pipe.GetDevice();
        auto                                imuPipeline = std::make_shared<dt::Pipeline>(dev);
        std::mutex                          imuFrameMutex;
        std::shared_ptr<dt::FrameSet> renderImuFrameSet;

        std::shared_ptr<dt::Config> imuConfig = std::make_shared<dt::Config>();
        // enable gyro stream.
        imuConfig->EnableGyroStream();
        // enable accel stream.
        imuConfig->EnableAccelStream();
        // start the imu pipeline.
        imuPipeline->Start(imuConfig, [&](std::shared_ptr<dt::FrameSet> frameSet) {
            std::lock_guard<std::mutex> Lock_imu(imuFrameMutex);
            renderImuFrameSet = frameSet;
        });
    ```

## Run Sample

Press the Esc key in the window to exit the program.

### Result
