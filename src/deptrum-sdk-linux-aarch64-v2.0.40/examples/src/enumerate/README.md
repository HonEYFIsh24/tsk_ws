# C++ Sample: dt_enumerate

## Overview

Use the SDK interface to obtain camera-related information, including model, various sensors, and sensor-related configurations.

### Knowledge

Context is the environment context, the first object created during initialization, which can be used to perform some settings, including but not limited to device status change callbacks, log level settings, etc. Context can access multiple Devices.

Device is the device object, which can be used to obtain the device information, such as the model, serial number, and various sensors.One actual hardware device corresponds to one Device object.

## Code overview

1. Create a context

    ```cpp
        // Create a context.
        dt::Context context;
    ```

2. Check if there is a camera connected

    ```cpp
        // Query the list of connected devices.
        auto deviceList = context.QueryDeviceList();
        if(deviceList->GetCount() < 1) {
            std::cout << "No device found! Please connect a supported device and retry this program." << std::endl;
            return -1;
        }
    ```

3. Obtain and output relevant information of the access device

    ```cpp
        std::cout << "enumerated devices: " << std::endl;

        std::shared_ptr<dt::Device>     device     = nullptr;
        std::shared_ptr<dt::DeviceInfo> deviceInfo = nullptr;
        for(uint32_t index = 0; index < deviceList->GetCount(); index++) {
            // Get device from deviceList.
            device = deviceList->GetDevice(index);
            // Get device information from device
            deviceInfo = device->GetDeviceInfo();
            std::cout << " - " << index << ". name: " << deviceInfo->GetName() << " pid: " << deviceInfo->GetPid() << " SN: " << deviceInfo->GetSerialNumber()
                        << std::endl;
        }
    ```

4. Wait for keyboard input to select device

    ```cpp
        // select a device.
        int deviceSelected = dt_utils::GetInputOption();
        if(deviceSelected == -1) {
            break;
        }
    ```

5. Output device sensors and wait for keyboard input

     ```cpp
        // Enumerate sensors.
        void EnumerateSensors(std::shared_ptr<dt::Device> device) {
            while(true) {
                std::cout << "Sensor list: " << std::endl;
                // Get the list of sensors.
                auto sensorList = device->GetSensorList();
                for(uint32_t index = 0; index < sensorList->GetCount(); index++) {
                    // Get the sensor type.
                    auto sensorType = sensorList->GetSensorType(index);
                    std::cout << " - " << index << "."
                            << "sensor type: " << dt::TypeHelper::ConvertDtSensorTypeToString(sensorType) << std::endl;
                }

                std::cout << "Select a sensor to enumerate its streams(input sensor index or \'ESC\' to enumerate device): " << std::endl;

                // Select a sensor.
                int sensorSelected = dt_utils::GetInputOption();
                if(sensorSelected == -1) {
                    break;
                }

                // Get sensor from sensorList.
                auto sensor = sensorList->GetSensor(sensorSelected);
                EnumerateStreamProfiles(sensor);
            }
        }
    ```

6. Output information about the selected sensor

    ```cpp
    // Enumerate stream profiles.
    void EnumerateStreamProfiles(std::shared_ptr<dt::Sensor> sensor) {
        // Get the list of stream profiles.
        auto streamProfileList = sensor->GetStreamProfileList();
        // Get the sensor type.
        auto sensorType = sensor->GetType();
        for(uint32_t index = 0; index < streamProfileList->GetCount(); index++) {
            // Get the stream profile.
            auto profile = streamProfileList->GetProfile(index);
            if(sensorType == kDtSensorIr || sensorType == kDtSensorColor || sensorType == kDtSensorDepth || sensorType == kDtSensorIrLeft
            || sensorType == kDtSensorIrRight|| sensorType == kDtSensorColorLeft
            || sensorType == kDtSensorColorRight) {
                PrintStreamProfile(profile, index);
            }
            else if(sensorType == kDtSensorAccel) {
                PrintAccelProfile(profile, index);
            }
            else if(sensorType == kDtSensorGyro) {
                PrintGyroProfile(profile, index);
            }
            else {
                break;
            }
        }
    }
    ```

## Run Sample

In the window, enter the relevant information of the device sensor you want to view according to the prompts.
Press the Esc key in the window to exit the program.

### Result

