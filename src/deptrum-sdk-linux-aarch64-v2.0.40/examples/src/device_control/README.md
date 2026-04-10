# C++ Sample：dt_device_control

## Overview

The SDK can be used to modify camera-related parameters, including laser switch, laser level intensity, white balance switch, etc.

### Knowledge

Pipeline is a pipeline for processing data streams, providing multi-channel stream configuration, switching, frame aggregation, and frame synchronization functions.

Device is a class that can be used to get device information, parameters, and a list of contained sensors.

Sensor can be used to obtain different components of the camera and the stream of the component, for example, RGB, IR, Depth stream can be obtained through the RGB, IR, Depth sensor.

## code overview

1. Get camera related information and output

    ```cpp
        // select a device to operate
        std::shared_ptr<dt::Device> device = nullptr;
        if(deviceList->GetCount() > 0) {
            if(deviceList->GetCount() <= 1) {
                // If a single device is plugged in, the first one is selected by default
                device = deviceList->GetDevice(0);
            }
            else {
                device = SelectDevice(deviceList);
            }
            auto deviceInfo = device->GetDeviceInfo();
            std::cout << "\n------------------------------------------------------------------------\n";
            std::cout << "Current Device: "
                    << " name: " << deviceInfo->GetName() << ", vid: " << deviceInfo->GetVid()
                    << ", pid: " << deviceInfo->GetPid() << ", uid: " << deviceInfo->GetUid() << std::endl;
        }
        else {
            std::cout << "Device Not Found" << std::endl;
            isSelectDevice = false;
            break;
        }
    ```

2. Get the relevant parameters stored in the container and reorder them by id

    ```cpp
        // Get option list
        std::vector<DtOptionItem> GetOptionList(std::shared_ptr<dt::Device> device) {
        std::vector<DtOptionItem> optionVec;
        optionVec.clear();
        uint32_t size = device->GetSupportedOptionCount();
        for(uint32_t i = 0; i < size; i++) {
            DtOptionItem option_item = device->GetSupportedOption(i);
            if(IsPrimaryTypeOption(option_item) && option_item.permission != DT_PERMISSION_DENY) {
                optionVec.push_back(option_item);
            }
        }
        return optionVec;
        }
    ```

    ```cpp
        std::vector<DtOptionItem> optionList = GetOptionList(device);
        std::sort(optionList.begin(), optionList.end(), [](const DtOptionItem &a, const DtOptionItem &b) { return a.id < b.id; });
    ```

3. Use the get command to obtain camera-related option values

    ```cpp
       // get option value
        void GetOptionValue(std::shared_ptr<dt::Device> device, DtOptionItem optionItem) {
            try {
                bool  bool_ret  = false;
                int   int_ret   = 0;
                float float_ret = 0.0f;

                switch(optionItem.type) {
                case DT_BOOL_OPTION:
                    try {
                        bool_ret = device->GetBoolOption(optionItem.id);
                    }
                    catch(...) {
                        std::cout << "get bool option failed." << std::endl;
                    }
                    std::cout << "option name:" << optionItem.name << ",get bool value:" << bool_ret << std::endl;
                    break;
                case DT_INT_OPTION:
                    try {
                        int_ret = device->GetIntOption(optionItem.id);
                    }
                    catch(...) {
                        std::cout << "get int option failed." << std::endl;
                    }
                    std::cout << "option name:" << optionItem.name << ",get int value:" << int_ret << std::endl;
                    break;
                case DT_FLOAT_OPTION:
                    try {
                        float_ret = device->GetFloatOption(optionItem.id);
                    }
                    catch(...) {
                        std::cout << "get float option failed." << std::endl;
                    }
                    std::cout << "option name:" << optionItem.name << ",get float value:" << float_ret << std::endl;
                    break;
                default:
                    break;
                }
            }
            catch(...) {
                std::cout << "get option failed: " << optionItem.name << std::endl;
            }
        }
    ```

4. Use the set command to set camera-related option values

    ```cpp
        // set properties
        void SetOptionValue(std::shared_ptr<dt::Device> device, DtOptionItem optionItem, std::string strValue) {
            try {
                int   int_value   = 0;
                float float_value = 0.0f;
                int   bool_value  = 0;
                switch(optionItem.type) {
                case DT_BOOL_OPTION:
                    bool_value = std::atoi(strValue.c_str());
                    try {
                        device->SetBoolOption(optionItem.id, bool_value);
                    }
                    catch(...) {
                        std::cout << "set bool option fail." << std::endl;
                    }
                    std::cout << "option name:" << optionItem.name << ",set bool value:" << bool_value << std::endl;
                    break;
                case DT_INT_OPTION:
                    int_value = std::atoi(strValue.c_str());
                    try {
                        device->SetIntOption(optionItem.id, int_value);
                    }
                    catch(...) {
                        std::cout << "set int option fail." << std::endl;
                    }
                    std::cout << "option name:" << optionItem.name << ",set int value:" << int_value << std::endl;
                    break;
                case DT_FLOAT_OPTION:
                    float_value = static_cast<float>(std::atof(strValue.c_str())) ;
                    try {
                        device->SetFloatOption(optionItem.id, float_value);
                    }
                    catch(...) {
                        std::cout << "set float option fail." << std::endl;
                    }
                    std::cout << "option name:" << optionItem.name << ",set float value:" << float_value << std::endl;
                    break;
                default:
                    break;
                }
            }
            catch(...) {
                std::cout << "set option failed: " << optionItem.name << std::endl;
            }
        }
    ```

## Run Sample

Select the camera you want to operate. If it is a single device, skip the selection.
You can enter the command ? to get all the properties of the camera, including setting the maximum and minimum values, etc.
You can enter set to set command to setto set parameters, for example 6 set 0 (note the space)
You can enter the get command to set parameters, for example, 6 get (note the space)
Press the Esc key in the window to exit the program.

### Result