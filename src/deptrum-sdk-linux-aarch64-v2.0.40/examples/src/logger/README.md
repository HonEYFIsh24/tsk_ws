# C++ Sample: deptrum_loggger

## Overview
Use the SDK context class or modify the XML configuration file to set the SDK log level, including the log level output to the terminal and the log level output to the file.

### Knowledge
Context, which serves as the entry point to the underlying SDK. It is used to query device lists, handle device callbacks, and set the log level.

## Code Overview

1. Configure the log level output to the terminal

    ```cpp
    // Configure the log level output to the terminal.
    dt::SetLoggerToConsole(kDtLogSeverityError);
    ```

2. Configure the log level output to the file.
The default output path settings(Windows/Linux) is `${CWD}/Log/DeptrumSDK.log`, where `${CWD}` represents the Current Working Directory. For Android, the path is `/sdcard/deptrum/Log`.

    ```cpp
    // Configure the log level and path output to the file.
    // The first parameter is the log level, and the second parameter is the output path.
    // If the output path is empty, the existing settings will continue to be used (if the existing configuration is also empty, the log will not be output to the file).
    dt::SetLoggerToFile(DT_LOG_SEVERITY_DEBUG, "Log/Custom/");
    ```

3. Registering a log callback

    ```cpp
    // Register a log callback, you can get log information in the callback.
    // The first parameter is the log level, and the second parameter is the callback function.
    dt::SetLoggerToCallback(DT_LOG_SEVERITY_DEBUG, [](DtLogSeverity severity, const char *log_msg) {
        std::cout << "[CallbackMessage][Level:" << severity << "]" << log_msg;
    });
    ```
