# C++ Sample: dt_coordinate_transform

## Overview

Use the SDK interface to  transform different coordinate systems.

### Knowledge

Pipeline is a pipeline for processing data streams, providing multi-channel stream configuration, switching, frame aggregation, and frame synchronization functions

Frameset is a combination of different types of Frames

## Code overview

1. Enable color stream

    ```cpp
        auto colorProfiles = pipe.GetStreamProfileList(DT_SENSOR_COLOR);
        if(colorProfiles) {
            colorProfile = colorProfiles->GetVideoStreamProfile(1280, kDtHeightAny, DT_FORMAT_RGB, 30);
        }
        config->EnableStream(colorProfile);
    ```

2. Enable depth stream

    ```cpp
        auto                                    depthProfiles = pipe.GetStreamProfileList(kDtSensorDepth);
        std::shared_ptr<dt::VideoStreamProfile> depthProfile  = nullptr;
        if(depthProfiles) {
        depthProfile = depthProfiles->GetVideoStreamProfile(640, kDtHeightAny, DT_FORMAT_Y16, 30);

        }
        config->EnableStream(depthProfile);
    ```

3. Get frame data

    ```cpp
        auto colorFrame = frameSet->ColorFrame();
        auto depthFrame = frameSet->DepthFrame();
    ```

4. Get get stream profile

    ```cpp
        auto colorProfile =  colorFrame->GetStreamProfile();
        auto depthProfile = depthFrame->GetStreamProfile();
    ```

5. Get the extrinsic parameters

    ```cpp
        auto extrinsicD2C = depthProfile->GetExtrinsicTo(colorProfile);
        auto extrinsicC2D = colorProfile->GetExtrinsicTo(depthProfile);
    ```

6. Get the intrinsic parameters

    ```cpp
        auto colorIntrinsic = colorProfile->As<dt::VideoStreamProfile>()->GetIntrinsic();
        auto colorDistortion = colorProfile->As<dt::VideoStreamProfile>()->GetDistortion();
    ```

7. Get the distortion parameters

    ```cpp
        auto depthIntrinsic = depthProfile->As<dt::VideoStreamProfile>()->GetIntrinsic();
        auto depthDistortion = depthProfile->As<dt::VideoStreamProfile>()->GetDistortion();
    ```

8. Processing

    ```cpp
        if(testType == "1") {
                Transform2dto2d(colorFrame, depthFrame);
            } else if (testType == "2") {
                Transform2dto3d(colorFrame, depthFrame);
            } else if (testType == "3") {
                Transform3dto3d(colorFrame, depthFrame);
            } else if (testType == "4") {
                Transform3dto2d(colorFrame, depthFrame);
            } else {
                std::cout << "Invalid command" << std::endl;
            }
    ```

## Run Sample

Press the Esc key to exit the program.
Press the 1 key - transform 2d to 2d
Press the 2 key - transform 2d to 3d
Press the 3 key - transform 3d to 3d
Press the 4 key - transform 3d to 2d

### Result
