# C++ Sample：dt_hot_plugin

## Overview

Use SDK to handle the settings of device unplug callback and process the acquired code stream after unplugging

### Knowledge

Pipeline is a pipeline for processing data streams, providing multi-channel stream configuration, switching, frame aggregation, and frame synchronization functions.

Device is a class that can be used to get device information, parameters, and a list of contained sensors.

Sensor can be used to obtain different components of the camera and the stream of the component, for example, RGB, IR, Depth stream can be obtained through the RGB, IR, Depth sensor.

### Attention

*The GMSL devices do not support hot plugging.*

## code overview

1. Register device callback and execute relevant functions during device unplugging and unplugging

    ```cpp
        ctx.SetDeviceChangedCallback( []( std::shared_ptr< dt::DeviceList > removed_list, std::shared_ptr< dt::DeviceList > added_list ) {
                DeviceDisconnectCallback( removed_list );
                DeviceConnectCallback( added_list );
            } );
    ```

2. Trigger the callback function to print relevant information

    ```cpp
        void PrintDeviceList(const std::string &prompt, std::shared_ptr<dt::DeviceList> device_list) {
            auto count = device_list->GetCount();
            if(count == 0) {
                return;
            }
            std::cout << count << " device(s) " << prompt << ": " << std::endl;
            for(uint32_t i = 0; i < count; i++) {
                auto uid          = device_list->GetUid(i);
                auto vid          = device_list->GetVid(i);
                auto pid          = device_list->GetPid(i);
                auto serial_number = device_list->GetSerialNumber(i);
                auto connection   = device_list->GetConnectionType(i);
                std::cout << " - uid: " << uid << ", vid: 0x" << std::hex << std::setfill('0') << std::setw(4) << vid << ", pid: 0x" << pid
                            << ", serial number: " << serial_number << ", connection: " << connection << std::endl;
            }
            std::cout << std::endl;
        }
    ```

3. Restart your device

```cpp
    void RebootDevices(std::shared_ptr<dt::DeviceList> deviceList) {
        for(uint32_t i = 0; i < deviceList->GetCount(); i++) {
            // get device from device list
            auto device = deviceList->GetDevice(i);

            // reboot device
            device->Reboot();
        }
    }
```

## Run Sample

Press R to reboot the device
You can try to manually unplug and plug the device
Press the Esc key in the window to exit the program

### Result
