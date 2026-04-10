# C++ Sample: dt_depth

## Overview

Use the SDK interface to obtain the raw stream of the camera and display it in the window

### Knowledge

RawFrame can obtain relevant information about the raw

## code overview

1. For Pisca you should use the following code to enable the raw stream

    ```cpp
        std::shared_ptr<dt::Config> config = std::make_shared<dt::Config>();

        //This is the default depth streamprofile that is enabled. If you want to modify it, you can do so in the configuration file.
        config->EnableVideoStream(kDtStreamRawPhase);

    ```
2. For Nebula you should use the following code to enable the raw stream

    ```cpp
        pipe.GetDevice()->SetBoolOption(kDtOptionCameraCalibBool, true);

        pipe.Start();
    ```

## Run Sample

Moving the camera can obtain the change in the distance across the center pixel
Press the Esc key in the window to exit the program.

### Result
