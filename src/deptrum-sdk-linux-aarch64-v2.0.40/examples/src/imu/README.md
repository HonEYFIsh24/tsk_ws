# C++ Sample: dt_imu

## Overview

Use the SDK interface to obtain the camera's internal imu data and output it

### Knowledge

AccelFrame measures the acceleration of x, y, and z in m/s^2
GyroFrame measures the angular velocity of x, y, and z in rad/s

Frameset is a combination of different types of Frames.imu data stream can be obtained through frameset

## code overview

1. Configure output imu related information and open stream.You must configure this before calling pipe.start().

    ```cpp

        // Configure which streams to enable or disable for the Pipeline by creating a Config.
        std::shared_ptr<dt::Config> config = std::make_shared<dt::Config>();

        // Enable Accel stream.
        config->EnableAccelStream();

        // Enable Gyro stream.
        config->EnableGyroStream();
    ```

2. Instantiate pipeline, configure output imu related information and open stream

    ```cpp
        auto accelFrameRaw    = frameSet->GetFrame(DT_FRAME_ACCEL);
        auto accelFrame       = accelFrameRaw->As<dt::AccelFrame>();
        auto accelIndex       = accelFrame->GetIndex();
        auto accelTimeStampUs = accelFrame->GetTimeStampUs();
        auto accelTemperature = accelFrame->GetTemperature();
        auto accelType        = accelFrame->GetType();
        if(accelIndex % 50 == 0) {  // print information every 50 frames.
            auto accelValue = accelFrame->GetValue();
            PrintImuValue(accelValue, accelIndex, accelTimeStampUs, accelTemperature, accelType, "m/s^2");
        }

        auto gyroFrameRaw    = frameSet->GetFrame(DT_FRAME_GYRO);
        auto gyroFrame       = gyroFrameRaw->As<dt::GyroFrame>();
        auto gyroIndex       = gyroFrame->GetIndex();
        auto gyroTimeStampUs = gyroFrame->GetTimeStampUs();
        auto gyroTemperature = gyroFrame->GetTemperature();
        auto gyroType        = gyroFrame->GetType();
        if(gyroIndex % 50 == 0) {  // print information every 50 frames.
            auto gyroValue = gyroFrame->GetValue();
            PrintImuValue(gyroValue, gyroIndex, gyroTimeStampUs, gyroTemperature, gyroType, "rad/s");
        }
    ```

## Run Sample

Press the Esc key in the window to exit the program.

### Result
