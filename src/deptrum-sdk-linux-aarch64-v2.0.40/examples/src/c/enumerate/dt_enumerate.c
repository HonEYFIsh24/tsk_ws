// Copyright (c) Deptrum Inc. All Rights Reserved.

#include <deptrum/deptrum.h>
#include <stdio.h>
#include <stdlib.h>
#include "utils_c.h"

// helper function to check for errors and exit if there is one
void check_dt_error(dt_error** err) {
  if (*err) {
    const char* error_message = dt_error_get_message(*err);
    fprintf(stderr, "Error: %s\n", error_message);
    dt_delete_error(*err);
    exit(-1);
  }
  *err = NULL;
}

int select_index(const char* prompt,
                 int min_value,
                 int max_value) {  // todo: to support select index > 9
  int value = 0;
  printf("\n%s (Input device index or \'q\' to exit program): ", prompt);
  while (true) {
    char input;
    int ret = scanf("%c", &input);
    (void) ret;
    getchar();

    if (input == 'q' || input == 'Q') {
      value = -1;
      break;
    }
    if (input >= '0' && input <= '9' && input - '0' >= min_value && input - '0' <= max_value) {
      value = input - '0';
      break;
    }
    printf("Invalid input, please input a number between %d and %d or \'q\' to exit program: ",
           min_value,
           max_value);
  }
  return value;
}

// Enumerates stream information.
void enumerate_stream_info(dt_sensor* sensor) {
  dt_error* error = NULL;

  // Get sensor type.
  dt_sensor_type sensor_type = dt_sensor_get_type(sensor, &error);
  check_dt_error(&error);

  // Get stream profile list.
  dt_stream_profile_list* stream_profile_list = dt_sensor_get_stream_profile_list(sensor, &error);
  check_dt_error(&error);

  // Get stream profile count.
  uint32_t stream_profile_count = dt_stream_profile_list_get_count(stream_profile_list, &error);
  check_dt_error(&error);

  printf("Available stream profiles: \n");
  for (uint32_t index = 0; index < stream_profile_count; index++) {
    // Get stream profile.
    const dt_stream_profile* stream_profile = dt_stream_profile_list_get_profile(
        stream_profile_list,
        index,
        &error);
    check_dt_error(&error);

    // Print video stream profile information.
    if (sensor_type == kDtSensorIr || sensor_type == kDtSensorColor ||
        sensor_type == kDtSensorDepth || sensor_type == kDtSensorIrLeft ||
        sensor_type == kDtSensorIrRight || sensor_type == kDtSensorColorLeft ||
        sensor_type == kDtSensorColorRight) {
      dt_stream_type stream_type = dt_stream_profile_get_type(stream_profile, &error);
      check_dt_error(&error);
      const char* stream_type_str = dt_stream_type_to_string(stream_type);

      dt_format stream_format = dt_stream_profile_get_format(stream_profile, &error);
      check_dt_error(&error);
      const char* stream_format_str = dt_format_to_string(stream_format);

      uint32_t stream_width = dt_video_stream_profile_get_width(stream_profile, &error);
      check_dt_error(&error);

      uint32_t stream_height = dt_video_stream_profile_get_height(stream_profile, &error);
      check_dt_error(&error);

      uint32_t stream_fps = dt_video_stream_profile_get_fps(stream_profile, &error);
      check_dt_error(&error);

      printf("  %d - type: %4s, format: %4s, width: %4d, height: %4d, fps: %4d\n",
             index,
             stream_type_str,
             stream_format_str,
             stream_width,
             stream_height,
             stream_fps);
    } else if (sensor_type == kDtSensorAccel) {
      // Print acc stream profile information.
      dt_format stream_format = dt_stream_profile_get_format(stream_profile, &error);
      check_dt_error(&error);

      dt_accel_sample_rate acc_fps = dt_accel_stream_profile_get_sample_rate(stream_profile,
                                                                             &error);
      check_dt_error(&error);

      printf("  %d - type: %s, fps: %s\n",
             index,
             dt_format_to_string(stream_format),
             dt_imu_sample_rate_to_string(acc_fps));
    } else if (sensor_type == kDtSensorGyro) {
      // Print gyro stream profile information.
      dt_format stream_format = dt_stream_profile_get_format(stream_profile, &error);
      check_dt_error(&error);

      dt_gyro_sample_rate gyro_fps = dt_gyro_stream_profile_get_sample_rate(stream_profile, &error);
      check_dt_error(&error);

      printf("  %d - type: %s, fps: %s\n",
             index,
             dt_format_to_string(stream_format),
             dt_imu_sample_rate_to_string(gyro_fps));
    } else if (sensor_type == kDtSensorPalm) {
      // Print palm stream profile information.
      dt_format palm_format = dt_stream_profile_get_format(stream_profile, &error);
      check_dt_error(&error);

      printf("  %d - type: %s\n", index, dt_format_to_string(palm_format));
    }

    // destroy stream profile
    dt_delete_stream_profile(stream_profile);
  }

  // destroy stream profile list
  dt_delete_stream_profile_list(stream_profile_list);
}

// enumerate sensor list.
void enumerate_sensor_info(dt_device* device) {
  dt_error* error = NULL;

  // Get sensor list.
  dt_sensor_list* sensor_list = dt_device_get_sensor_list(device, &error);
  check_dt_error(&error);

  // Get sensor count.
  uint32_t sensor_count = dt_sensor_list_get_count(sensor_list, &error);
  check_dt_error(&error);

  while (true) {
    // Print sensor information.
    printf("Available sensors: \n");
    for (uint32_t index = 0; index < sensor_count; index++) {
      // Get device sensor.
      dt_sensor* sensor = dt_sensor_list_get_sensor(sensor_list, index, &error);
      check_dt_error(&error);

      // Get sensor type.
      dt_sensor_type sensor_type = dt_sensor_get_type(sensor, &error);
      check_dt_error(&error);
      const char* sensor_name = dt_sensor_type_to_string(sensor_type);

      // Print sensor information.
      printf("  %d - sensor name: %s\n", index, sensor_name);

      // destroy sensor
      dt_delete_sensor(sensor);
    }

    int index = select_index("Select a sensor to enumerate its stream profiles",
                             0,
                             sensor_count - 1);
    if (index >= 0) {
      // Get the selected sensor.
      dt_sensor* sensor = dt_sensor_list_get_sensor(sensor_list, index, &error);
      check_dt_error(&error);

      // Enumerate stream information of selected sensor.
      enumerate_stream_info(sensor);

      // destroy sensor
      dt_delete_sensor(sensor);
    } else {
      break;
    }
  }
  // destroy sensor list
  dt_delete_sensor_list(sensor_list);
}

// Enumerates device information.
void print_device_info(dt_device* device, int index) {
  dt_error* error = NULL;

  // Get device name.
  const char* dev_name = dt_device_get_device_info(device, kDtCameraInfoName, &error);
  check_dt_error(&error);

  // Get device pid.
  const char* dev_pid = dt_device_get_device_info(device, kDtCameraInfoProductId, &error);
  check_dt_error(&error);

  // Get device serial number.
  const char* dev_sn = dt_device_get_device_info(device, kDtCameraInfoSerialNumber, &error);
  check_dt_error(&error);

  // Get connection type.
  const char* conn_type = dt_device_get_device_info(device, kDtCameraInfoConnectionType, &error);
  check_dt_error(&error);

  printf("  %d - device name: %s, device pid: %s, device sn: %s, connection type: %s\n",
         index,
         dev_name,
         dev_pid,
         dev_sn,
         conn_type);
}

int main(void) {
  // Used to return SDK interface error information.
  dt_error* error = NULL;

  // Get deptrum version.
  int major_version = dt_get_major_version();
  int minor_version = dt_get_minor_version();
  int patch_version = dt_get_patch_version();
  printf("Deptrum SDK version: %d.%d.%d\n", major_version, minor_version, patch_version);

  // Create context.
  dt_context* ctx = dt_create_context(&error);
  check_dt_error(&error);

  // Get device list from context.
  dt_device_list* dev_list = dt_query_device_list(ctx, &error);
  check_dt_error(&error);

  // Get device count from device list.
  uint32_t dev_count = dt_device_list_get_count(dev_list, &error);
  check_dt_error(&error);

  // Check if any device is connected.
  if (dev_count == 0) {
    printf("No device found! Please connect a supported device and retry this program.\n");

    dt_delete_device_list(dev_list);

    dt_delete_context(ctx);

    printf("\nPress any key to exit.");
    dt_utils_wait_for_key_press(0);

    return -1;
  }

  while (true) {
    printf("Connected devices: \n");
    for (uint32_t index = 0; index < dev_count; index++) {
      // Get device from device list.
      dt_device* dev = dt_device_list_get_device(dev_list, index, &error);
      check_dt_error(&error);

      // print device information
      print_device_info(dev, index);

      // destroy device
      dt_delete_device(dev);
    }

    // Select a device.
    int device_index = select_index("Select a device to enumerate its sensors", 0, dev_count - 1);
    if (device_index >= 0) {
      // get device from device list
      dt_device* device = dt_device_list_get_device(dev_list, device_index, &error);
      check_dt_error(&error);

      // enumerate sensors of device
      enumerate_sensor_info(device);

      // destroy device
      dt_delete_device(device);
    } else {
      break;
    }
  }

  // destroy sensor list
  dt_delete_device_list(dev_list);

  // destroy context
  dt_delete_context(ctx);

  printf("\nProgram ended successfully. Press any key to exit.");
  dt_utils_wait_for_key_press(0);

  return 0;
}
