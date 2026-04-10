# C++ Sample: dt_post_processing

## Overview

Use the SDK interface to  demonstrate post-processing operations, display post-processed images, and exit the program using the ESC_KEY key

### Knowledge

Pipeline is a pipeline for processing data streams, providing multi-channel stream configuration, switching, frame aggregation, and frame synchronization functions

Frameset is a combination of different types of Frames

win is used to display the frame data.

## Code overview

1. Get the device and sensor, and create the list of recommended filters for the sensor

    ```cpp
        auto device     = pipe.GetDevice();
        auto sensor     = device->GetSensor(kDtSensorDepth);
        auto filterList = sensor->CreateRecommendedFilters();
    ```

2. The filter operation.

    - Get the type of filter

    ```cpp
    filter->GetName()
    ```

    - Get the Config Schema Vec object

    ```cpp
    filter->GetConfigSchemaVec()
    ```

    - Enable the filter

    ```cpp
    filter->Enable(tokens[1] == "on");
    ```

    - Get the Config Value object by name.

    ```cpp
    filter->GetConfigValue(configSchema.name)
    ```

    - Get the Enable State of the filter.

    ```cpp
    filter->IsEnabled()
    ```

    - Set the filter config value by name.

    ```cpp
    filter->SetConfigValue(tokens[1], value);
    ```

3. Apply the recommended filters to the depth frame

    ```cpp
        auto processedFrame = depthFrame;
        // Apply the recommended filters to the depth frame
        for(auto &filter: filterList) {
            if(filter->IsEnabled()) {  // Only apply enabled filters
                processedFrame = filter->Process(processedFrame);
            }
        }
    ````

## Run Sample

Press the button according to the interface prompts

### Result
