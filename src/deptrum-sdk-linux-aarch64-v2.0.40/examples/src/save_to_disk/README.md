# C++ Sample: 4.misc.save_to_disk

## Overview

This sample demonstrates how to configure a pipeline to capture color and depth frames, perform necessary format conversions, and save the first 5 valid frames to disk in PNG format. The program discards initial frames to ensure stable stream capture and handles color format conversion for compatibility.

### Knowledge

Pipeline is a pipeline for processing data streams, providing multi-channel stream configuration, switching, frame aggregation, and frame synchronization functions.

Frameset is a combination of different types of Frames.

Metadata is used to describe the various properties and states of a frame.

## Code overview

1. Pipeline Configuration and Initialization.

    ```cpp
    // Create a pipeline.
    std::shared_ptr<dt::Pipeline> pipeline = std::make_shared<dt::Pipeline>();

    // Create a config and enable the depth and color streams.
    std::shared_ptr<dt::Config> config = std::make_shared<dt::Config>();
    config->EnableStream(DT_STREAM_COLOR);
    config->EnableStream(DT_STREAM_DEPTH);
    ```

2. Get frameSet from pipeline.

    ```cpp
    // Wait for frameSet from the pipeline, the default timeout is 1000ms.
    auto frameSet   = pipe.WaitForFrames();
    ```

3. Convert color images to RGB format and save them.

    ```cpp
    // Get the depth and color frames.
    auto depthFrame = frameSet->GetFrame(DT_FRAME_DEPTH)->As<dt::DepthFrame>();
    auto colorFrame = frameSet->GetFrame(DT_FRAME_COLOR)->As<dt::ColorFrame>();

    // Convert the color frame to RGB format.
    if(colorFrame->Format() != DT_FORMAT_RGB) {
        if(colorFrame->Format() == DT_FORMAT_MJPG) {
            formatConverter->SetFormatConvertType(FORMAT_MJPG_TO_RGB);
        }
        else if(colorFrame->Format() == DT_FORMAT_UYVY) {
            formatConverter->SetFormatConvertType(FORMAT_UYVY_TO_RGB);
        }
        else if(colorFrame->Format() == DT_FORMAT_YUYV) {
            formatConverter->SetFormatConvertType(FORMAT_YUYV_TO_RGB);
        }
        else {
            std::cout << "Color format is not support!" << std::endl;
            continue;
        }
        colorFrame = formatConverter->Process(colorFrame)->As<dt::ColorFrame>();
    }
    // Processed the color frames to BGR format, use OpenCV to save to disk.
    formatConverter->SetFormatConvertType(FORMAT_RGB_TO_BGR);
    colorFrame = formatConverter->Process(colorFrame)->As<dt::ColorFrame>();
    ```

4. Save the first 5 valid frames to disk in PNG format.

    ```cpp
    void SaveDepthFrame(const std::shared_ptr<dt::DepthFrame> depthFrame, const uint32_t frameIndex) {
        std::vector<int> params;
        params.push_back(cv::IMWRITE_PNG_COMPRESSION);
        params.push_back(0);
        params.push_back(cv::IMWRITE_PNG_STRATEGY);
        params.push_back(cv::IMWRITE_PNG_STRATEGY_DEFAULT);
        std::string depthName = "Depth_" + std::to_string(depthFrame->Width()) + "x" + std::to_string(depthFrame->Height()) + "_" + std::to_string(frameIndex) + "_"
                                + std::to_string(depthFrame->TimeStamp()) + "ms.png";
        cv::Mat DepthMat(depthFrame->Height(), depthFrame->Width(), CV_16UC1, depthFrame->data());
        cv::imwrite(depthName, depthMat, params);
        std::cout << "Depth saved:" << depthName << std::endl;
    }

    void SaveColorFrame(const std::shared_ptr<dt::ColorFrame> colorFrame, const uint32_t frameIndex) {
        std::vector<int> params;
        params.push_back(cv::IMWRITE_PNG_COMPRESSION);
        params.push_back(0);
        params.push_back(cv::IMWRITE_PNG_STRATEGY);
        params.push_back(cv::IMWRITE_PNG_STRATEGY_DEFAULT);
        std::string colorName = "Color_" + std::to_string(colorFrame->Width()) + "x" + std::to_string(colorFrame->Height()) + "_" + std::to_string(frameIndex) + "_"
                                + std::to_string(colorFrame->TimeStamp()) + "ms.png";
        cv::Mat DepthMat(colorFrame->Height(), colorFrame->Width(), CV_8UC3, colorFrame->data());
        cv::imwrite(colorName, depthMat, params);
        std::cout << "Depth saved:" << colorName << std::endl;
    }
    ```

## Run Sample

Press the Any key in the window to exit the program.