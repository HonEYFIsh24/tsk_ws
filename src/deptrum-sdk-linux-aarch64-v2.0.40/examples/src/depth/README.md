# C++ Sample: dt_depth

## Overview

Use the SDK interface to obtain the depth stream of the camera and display it in the window

### Knowledge

DepthFrame can obtain relevant information about the depth

## code overview

1. Configure the output color stream and open the video stream.You must configure this before calling pipe.start().

    ```cpp
        // By creating config to configure which streams to enable or disable for the pipeline, here the depth stream will be enabled.
        std::shared_ptr<dt::Config> config = std::make_shared<dt::Config>();

        //This is the default depth streamprofile that is enabled. If you want to modify it, you can do so in the configuration file.
        config->EnableVideoStream(DT_STREAM_DEPTH);

    ```

## Run Sample

Moving the camera can obtain the change in the distance across the center pixel
Press the Esc key in the window to exit the program.

### Result
