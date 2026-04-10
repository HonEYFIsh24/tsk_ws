// Copyright (c) Deptrum Inc. All Rights Reserved.

/**
 * @file dt_types.h
 * @brief Provides common SDK structs, enumerations, and constant definitions.
 */

#ifndef DEPTRUM_H_DT_TYPES_H_
#define DEPTRUM_H_DT_TYPES_H_

#include <stdbool.h>
#include <stdint.h>
#include "dt_export.h"

#pragma pack(push, 1)  // 1-byte struct alignment

#ifdef __cplusplus
extern "C" {
#endif

typedef struct dt_context_t dt_context;
typedef struct dt_device_t dt_device;
typedef struct dt_device_list_t dt_device_list;
typedef struct dt_config_t dt_config;
typedef struct dt_pipeline_t dt_pipeline;
typedef struct dt_sensor_t dt_sensor;
typedef struct dt_sensor_list_t dt_sensor_list;
typedef struct dt_stream_profile_t dt_stream_profile;
typedef struct dt_stream_profile_list_t dt_stream_profile_list;
typedef struct dt_frame_t dt_frame;
typedef struct dt_filter_t dt_filter;
typedef struct dt_filter_list_t dt_filter_list;
typedef struct dt_filter_config_schema_list_t dt_filter_config_schema_list;
typedef struct dt_camera_param_list_t dt_camera_param_list;
typedef struct dt_record_device_t dt_record_device;
typedef struct dt_playback_device_t dt_playback_device;

/**
 * \brief Read-only strings that can be queried from the device. Not all information attributes are
 * available on all camera types. This information is mainly available for camera debug and
 * troubleshooting and should not be used in applications.
 */
typedef enum {
  kDtCameraInfoName,                   /**< Device name */
  kDtCameraInfoProductId,              /**< Product ID as reported in the USB descriptor */
  kDtCameraInfoVendorId,               /**< Vendor ID as reported in the USB descriptor */
  kDtCameraInfoUniqueId,               /**< Unique ID */
  kDtCameraInfoSerialNumber,           /**< Device serial number */
  kDtCameraInfoFirmwareVersion,        /**< Firmware version */
  kDtCameraInfoConnectionType,         /**< Connection type, currently supports: "USB", "USB1.0",
         "USB1.1", "USB2.0",  "USB2.1", "USB3.0", "USB3.1", "USB3.2", "Ethernet" */
  kDtCameraInfoHardwareVersion,        /**< Hardware version */
  kDtCameraInfoAsicName,               /**< ASIC name */
  kDtCameraInfoSupportedMinSdkVersion, /**< Minimum SDK version required to support the device
                                        */
  kDtCameraInfoIpAddress,  /**< IP address for remote camera, such as "192.168.100.101" */
  kDtCameraInfoMacAddress, /**< MAC address for remote camera, such as "00:11:22:33:44:55" */
} DtCameraInfo,
    dt_camera_info;

/**
 * \brief  The breath mode of the camera.
 */
typedef enum {
  kDtBreathModeOff = 0,   /**< off */
  kDtBreathModeRed = 1,   /**< red */
  kDtBreathModeGreen = 2, /**< green */
  kDtBreathModeBlue = 3,  /**< blue */
  kDtBreathModeWhite = 4, /**< white */
} DtBreathMode,
    dt_breath_mode;

/**
 * @brief the mode of palm recognition.
 */
typedef enum {
  kDtDimPalmRecognition = 0, /**< palm recognition */
  kDtDimPalmRegister = 1,    /**< palm register */
} DtDimPalmMode,
    dt_dim_palm_mode;

/**
 * @brief Device status codes.
 */
typedef enum {
  kDtStatusOk = 0,    /**< Operation completed successfully */
  kDtStatusError = 1, /**< General operation failure */
} DtStatus,
    dt_status;

/**
 * @brief Log severity levels (higher values indicate stricter filtering).
 */
typedef enum {
  kDtLogSeverityDebug, /**< Debug-level messages */
  kDtLogSeverityInfo,  /**< Informational messages */
  kDtLogSeverityWarn,  /**< Warning conditions */
  kDtLogSeverityError, /**< Recoverable errors */
  kDtLogSeverityFatal, /**< Fatal errors causing termination */
  kDtLogSeverityOff    /**< Logging disabled */
} DtLogSeverity,
    dt_log_severity;

/**
 * @brief Sensor type identifiers.
 */
typedef enum {
  kDtSensorUnknown = -1,   /**< Unknown sensor type */
  kDtSensorIr = 0,         /**< Infrared sensor */
  kDtSensorColor = 1,      /**< Color (RGB) sensor */
  kDtSensorDepth = 2,      /**< Depth sensor */
  kDtSensorAccel = 3,      /**< Accelerometer */
  kDtSensorGyro = 4,       /**< Gyroscope */
  kDtSensorIrLeft = 5,     /**< Left IR sensor (stereo cameras) */
  kDtSensorIrRight = 6,    /**< Right IR sensor (stereo cameras) */
  kDtSensorRawPhase = 7,   /**< Raw phase sensor */
  kDtSensorColorLeft = 8,  /**< Left RGB sensor (stereo cameras) */
  kDtSensorColorRight = 9, /**< Right RGB sensor (stereo cameras) */
  kDtSensorPalm = 10,      /**< Palm sensor */
  kDtSensorTypeCount,      /**< Total sensor types (not a valid type) */
} DtSensorType,
    dt_sensor_type;

/**
 * @brief Stream type identifiers.
 */
typedef enum {
  kDtStreamUnknown = -1,    /**< Unknown stream type */
  kDtStreamVideo = 0,       /**< Generic video stream */
  kDtStreamIr = 1,          /**< Infrared stream */
  kDtStreamColor = 2,       /**< Color (RGB) stream */
  kDtStreamDepth = 3,       /**< Depth stream */
  kDtStreamAccel = 4,       /**< Accelerometer data stream */
  kDtStreamGyro = 5,        /**< Gyroscope data stream */
  kDtStreamIrLeft = 6,      /**< Left IR stream (stereo cameras) */
  kDtStreamIrRight = 7,     /**< Right IR stream (stereo cameras) */
  kDtStreamRawPhase = 8,    /**< Raw phase stream */
  kDtStreamColorLeft = 9,   /**< Left RGB stream (stereo cameras) */
  kDtStreamColorRight = 10, /**< Right RGB stream (stereo cameras) */
  kDtStreamPalm = 11,       /**< Palm stream */
  kDtStreamTypeCount,       /**< Total stream types (not a valid type) */
} DtStreamType,
    dt_stream_type;

/**
 * @brief Frame type identifiers.
 */
typedef enum {
  kDtFrameUnknown = -1,    /**< Unknown frame type */
  kDtFrameVideo = 0,       /**< Generic video frame */
  kDtFrameIr = 1,          /**< Infrared frame */
  kDtFrameColor = 2,       /**< Color (RGB) frame */
  kDtFrameDepth = 3,       /**< Depth frame */
  kDtFrameAccel = 4,       /**< Accelerometer data frame */
  kDtFrameSet = 5,         /**< Frame collection (multiple data frames) */
  kDtFramePoints = 6,      /**< Point cloud frame */
  kDtFrameGyro = 7,        /**< Gyroscope data frame */
  kDtFrameIrLeft = 8,      /**< Left IR frame (stereo cameras) */
  kDtFrameIrRight = 9,     /**< Right IR frame (stereo cameras) */
  kDtFrameRawPhase = 10,   /**< Raw phase frame */
  kDtFrameColorLeft = 11,  /**< Left RGB frame (stereo cameras) */
  kDtFrameColorRight = 12, /**< Right RGB frame (stereo cameras) */
  kDtFramePalm = 13,       /**< Palm frame */
  kDtFrameTypeCount,       /**< Total frame types (not a valid type) */
} DtFrameType,
    dt_frame_type;

/**
 * @brief Pixel format identifiers.
 */
typedef enum {
  kDtFormatUnknown = -1,    /**< Unknown pixel format */
  kDtFormatYuyv = 0,        /**< YUYV packed format */
  kDtFormatYuy2 = 1,        /**< YUY2 format (identical to YUYV) */
  kDtFormatUyvy = 2,        /**< UYVY packed format */
  kDtFormatNv12 = 3,        /**< NV12 semi-planar format */
  kDtFormatNv21 = 4,        /**< NV21 semi-planar format */
  kDtFormatMjpg = 5,        /**< MJPEG compressed format */
  kDtFormatH264 = 6,        /**< H.264 compressed format */
  kDtFormatH265 = 7,        /**< H.265 compressed format */
  kDtFormatY16 = 8,         /**< 16-bit grayscale */
  kDtFormatY8 = 9,          /**< 8-bit grayscale */
  kDtFormatY10 = 10,        /**< 10-bit grayscale (unpacked to Y16) */
  kDtFormatY11 = 11,        /**< 11-bit grayscale (unpacked to Y16) */
  kDtFormatY12 = 12,        /**< 12-bit grayscale (unpacked to Y16) */
  kDtFormatGray = 13,       /**< Grayscale (identical to Y8) */
  kDtFormatHevc = 14,       /**< HEVC compressed format (identical to H265) */
  kDtFormatI420 = 15,       /**< I420 planar format */
  kDtFormatAccel = 16,      /**< Accelerometer data format */
  kDtFormatGyro = 17,       /**< Gyroscope data format */
  kDtFormatPoint = 19,      /**< XYZ 3D coordinate points */
  kDtFormatRgbPoint = 20,   /**< XYZ+RGB 3D points */
  kDtFormatAcPoint = 21,    /**< XYZ+ intensity(Amplitude + confidence) 3D coordinate points */
  kDtFormatRle = 22,        /**< RLE compressed format (unpacked to Y16) */
  kDtFormatRgb = 23,        /**< RGB888 packed format */
  kDtFormatBgr = 24,        /**< BGR888 packed format */
  kDtFormatY14 = 25,        /**< 14-bit grayscale (unpacked to Y16) */
  kDtFormatBgra = 26,       /**< BGRA8888 packed format */
  kDtFormatCompressed = 27, /**< Generic compressed format */
  kDtFormatRvl = 28,        /**< RVL compressed format (unpacked to Y16) */
  kDtFormatZ16 = 29,        /**< Depth format (identical to Y16) */
  kDtFormatYv12 = 30,       /**< YV12 format (for right IR stream) */
  kDtFormatBa81 = 31,       /**< Bayer format (for right IR stream) */
  kDtFormatRgba = 32,       /**< RGBA8888 packed format */
  kDtFormatByr2 = 33,       /**< BYR2 packed format */
  kDtFormatRw16 = 34,       /**< RAW16 format */
  kDtFormatY32 = 35,        /**< 32-bit float grayscale */
  kDtFormatPalm = 36,       /**< Palm data format */

  // Aliases
  kDtFormatRgb888 = kDtFormatRgb,
  kDtFormatMjpeg = kDtFormatMjpg,
} DtFormat,
    dt_format;

// Check if the format is a fixed data size format
#define IS_FIXED_SIZE_FORMAT(format)                                                \
  (format != kDtFormatMjpg && format != kDtFormatH264 && format != kDtFormatH265 && \
   format != kDtFormatHevc && format != kDtFormatRle && format != kDtFormatRvl)

// Check if the format is a packed format, which means the data of pixels is not continuous or bytes
// aligned in memory
#define IS_PACKED_FORMAT(format)                                                 \
  (format == kDtFormatY10 || format == kDtFormatY11 || format == kDtFormatY12 || \
   format == kDtFormatY14 || format == kDtFormatRle)

/**
 * @brief Firmware update states.
 */
typedef enum {
  kDtStatDoneWithDuplicates = 6, /**< Update completed (some files ignored as duplicates) */
  kDtStatVerifySuccess = 5,      /**< Firmware image verification succeeded */
  kDtStatFileTransfer = 4,       /**< Firmware file transfer in progress */
  kDtStatDone = 3,               /**< Update completed successfully */
  kDtStatInProgress = 2,         /**< Update in progress */
  kDtStatStart = 1,              /**< Update initiated */
  kDtStatVerifyImage = 0,        /**< Verifying firmware image */
  kDtErrVerify = -1,             /**< Firmware verification failed */
  kDtErrProgram = -2,            /**< Programming operation failed */
  kDtErrErase = -3,              /**< Flash erase operation failed */
  kDtErrFlashType = -4,          /**< Unsupported flash type */
  kDtErrImageSize = -5,          /**< Invalid firmware image size */
  kDtErrOther = -6,              /**< General firmware update error */
  kDtErrTimeout = -7,            /**< Operation timed out */
  kDtErrMismatch = -8,           /**< Firmware/device mismatch */
  kDtErrInvalidCount = -9,       /**< Invalid firmware count */
} DtUpgradeState,
    DtFwUpdateState, dt_upgrade_state, dt_fw_update_state;

/**
 * @brief File transfer states
 */
typedef enum {
  kDtFileTranStatTransfer = 2,        /**< File transfer in progress */
  kDtFileTranStatDone = 1,            /**< File transfer completed */
  kDtFileTranStatPrepare = 0,         /**< Preparing for transfer */
  kDtFileTranErrDdr = -1,             /**< DDR access error */
  kDtFileTranErrNotEnoughSpace = -2,  /**< Insufficient storage space */
  kDtFileTranErrPathNotWritable = -3, /**< Destination path not writable */
  kDtFileTranErrMd5Error = -4,        /**< MD5 checksum mismatch */
  kDtFileTranErrWriteFlashError = -5, /**< Flash write error */
  kDtFileTranErrTimeout = -6          /**< Operation timed out */
} DtFileTranState,
    dt_file_tran_state;

/**
 * @brief Data transmission states.
 */
typedef enum {
  kDtDataTranStatVerifyDone = 4,   /**< Data verification completed */
  kDtDataTranStatStopped = 3,      /**< Transmission stopped */
  kDtDataTranStatDone = 2,         /**< Transmission completed */
  kDtDataTranStatVerifying = 1,    /**< Verifying data */
  kDtDataTranStatTransferring = 0, /**< Data transfer in progress */
  kDtDataTranErrBusy = -1,         /**< Resource busy */
  kDtDataTranErrUnsupported = -2,  /**< Operation not supported */
  kDtDataTranErrTranFailed = -3,   /**< Transmission failed */
  kDtDataTranErrVerifyFailed = -4, /**< Data verification failed */
  kDtDataTranErrOther = -5         /**< General transmission error */
} DtDataTranState,
    dt_data_tran_state;

/**
 * @brief Data block descriptor for chunked transfers.
 */
typedef struct {
  uint8_t* data;           /**< Pointer to data block */
  uint32_t size;           /**< Size of current block (bytes) */
  uint32_t offset;         /**< Offset relative to full data */
  uint32_t full_data_size; /**< Total expected data size (bytes) */
} DtDataChunk, dt_data_chunk;

/**
 * @brief Camera distortion models.
 */
typedef enum {
  kDtDistortionNone,                 /**< Rectilinear images (no distortion) */
  kDtDistortionModifiedBrownConrady, /**< Modified Brown-Conrady distortion model */
  kDtDistortionInverseBrownConrady,  /**< Inverse Brown-Conrady distortion model */
  kDtDistortionBrownConrady,         /**< Standard Brown-Conrady distortion model */
  kDtDistortionBrownConradyK6,       /**< Brown-Conrady model with k6 coefficient */
  kDtDistortionKannalaBrandt4,       /**< Kannala-Brandt distortion model */
} DtCameraDistortionModel,
    dt_camera_distortion_model;

/**
 * @brief Camera intrinsic parameters.
 */
typedef struct {
  float fx;       /**< Focal length (x-axis, pixels) */
  float fy;       /**< Focal length (y-axis, pixels) */
  float cx;       /**< Principal point (x-axis, pixels) */
  float cy;       /**< Principal point (y-axis, pixels) */
  int16_t width;  /**< Image width (pixels) */
  int16_t height; /**< Image height (pixels) */
} DtCameraIntrinsic, dt_camera_intrinsic;

/**
 * @brief Accelerometer intrinsic parameters.
 */
typedef struct {
  double noise_density;         /**< In-run bias instability (m/s²/√Hz) */
  double random_walk;           /**< Velocity random walk (m/s³/√Hz) */
  double reference_temp;        /**< Reference temperature (°C) */
  double bias[3];               /**< Bias offsets (x,y,z in m/s²) */
  double gravity[3];            /**< Gravity vector (x,y,z) */
  double scale_misalignment[9]; /**< Scale factors and misalignment matrix */
  double temp_slope[9];         /**< Temperature drift coefficients */
} DtAccelIntrinsic, dt_accel_intrinsic;

/**
 * @brief Gyroscope intrinsic parameters.
 */
typedef struct {
  double noise_density;         /**< Angular random walk (°/s/√Hz) */
  double random_walk;           /**< Rate random walk (°/s²/√Hz) */
  double reference_temp;        /**< Reference temperature (°C) */
  double bias[3];               /**< Bias offsets (x,y,z in °/s) */
  double scale_misalignment[9]; /**< Scale factors and misalignment matrix */
  double temp_slope[9];         /**< Temperature drift coefficients */
} DtGyroIntrinsic, dt_gyro_intrinsic;

/**
 * @brief Camera distortion parameters.
 */
typedef struct {
  float k1;                      /**< Radial distortion coefficient k₁ */
  float k2;                      /**< Radial distortion coefficient k₂ */
  float k3;                      /**< Radial distortion coefficient k₃ */
  float k4;                      /**< Radial distortion coefficient k₄ */
  float k5;                      /**< Radial distortion coefficient k₅ */
  float k6;                      /**< Radial distortion coefficient k₆ */
  float p1;                      /**< Tangential distortion coefficient p₁ */
  float p2;                      /**< Tangential distortion coefficient p₂ */
  DtCameraDistortionModel model; /**< Distortion model type */
} DtCameraDistortion, dt_camera_distortion;

/**
 * @brief 3D transformation parameters.
 */
typedef struct {
  float rot[9];   /**< Rotation matrix (row-major 3x3) */
  float trans[3]; /**< Translation vector (mm) */
} DtD2CTransform, dt_d2c_transform, DtTransform, dt_transform, DtExtrinsic, dt_extrinsic;

/**
 * @brief Complete camera parameters.
 */
typedef struct {
  DtCameraIntrinsic depth_intrinsic;   /**< Depth camera intrinsics */
  DtCameraIntrinsic rgb_intrinsic;     /**< RGB camera intrinsics */
  DtCameraDistortion depth_distortion; /**< Depth distortion coefficients */
  DtCameraDistortion rgb_distortion;   /**< RGB distortion coefficients */
  DtD2CTransform transform;            /**< Depth-to-color transformation */
  bool is_mirrored;                    /**< Indicates if images are mirrored horizontally */
} DtCameraParam, dt_camera_param;

/**
 * @brief Pixel format conversion types.
 */
typedef enum {
  kDtFormatYuyvToRgb = 0, /**< YUYV to RGB888 conversion */
  kDtFormatI420ToRgb,     /**< I420 to RGB888 conversion */
  kDtFormatNv21ToRgb,     /**< NV21 to RGB888 conversion */
  kDtFormatNv12ToRgb,     /**< NV12 to RGB888 conversion */
  kDtFormatMjpgToI420,    /**< MJPEG to I420 conversion */
  kDtFormatRgbToBgr,      /**< RGB888 to BGR888 conversion */
  kDtFormatMjpgToNv21,    /**< MJPEG to NV21 conversion */
  kDtFormatMjpgToRgb,     /**< MJPEG to RGB888 conversion */
  kDtFormatMjpgToBgr,     /**< MJPEG to BGR888 conversion */
  kDtFormatMjpgToBgra,    /**< MJPEG to BGRA8888 conversion */
  kDtFormatUyvyToRgb,     /**< UYVY to RGB888 conversion */
  kDtFormatBgrToRgb,      /**< BGR888 to RGB888 conversion */
  kDtFormatMjpgToNv12,    /**< MJPEG to NV12 conversion */
  kDtFormatYuyvToBgr,     /**< YUYV to BGR888 conversion */
  kDtFormatYuyvToRgba,    /**< YUYV to RGBA8888 conversion */
  kDtFormatYuyvToBgra,    /**< YUYV to BGRA8888 conversion */
  kDtFormatYuyvToY16,     /**< YUYV to 16-bit grayscale conversion */
  kDtFormatYuyvToY8,      /**< YUYV to 8-bit grayscale conversion */
  kDtFormatRgbaToRgb,     /**< RGBA8888 to RGB888 conversion */
  kDtFormatBgraToBgr,     /**< BGRA8888 to BGR888 conversion */
  kDtFormatY16ToRgb,      /**< 16-bit grayscale to RGB888 conversion */
  kDtFormatY8ToRgb,       /**< 8-bit grayscale to RGB888 conversion */
  kDtFormatNv12ToY8,      /**< NV12 to 8-bit grayscale conversion */
} DtConvertFormat,
    dt_convert_format;

/**
 * @brief IMU sample rates.
 */
typedef enum {
  kDtSampleRateUnknown = -1,
  kDtSampleRate15625Hz = 1,   /**< 1.5625 Hz */
  kDtSampleRate3125Hz = 3,    /**< 3.125 Hz */
  kDtSampleRate625Hz = 6,     /**< 6.25 Hz */
  kDtSampleRate125Hz = 12,    /**< 12.5 Hz */
  kDtSampleRate25Hz = 25,     /**< 25 Hz */
  kDtSampleRate50Hz = 50,     /**< 50 Hz */
  kDtSampleRate100Hz = 100,   /**< 100 Hz */
  kDtSampleRate200Hz = 200,   /**< 200 Hz */
  kDtSampleRate400Hz = 400,   /**< 400 Hz*/
  kDtSampleRate500Hz = 500,   /**< 500 Hz */
  kDtSampleRate800Hz = 800,   /**< 800 Hz*/
  kDtSampleRate1kHz = 1000,   /**< 1 kHz */
  kDtSampleRate2kHz = 2000,   /**< 2 kHz */
  kDtSampleRate4kHz = 4000,   /**< 4 kHz */
  kDtSampleRate8kHz = 8000,   /**< 8 kHz */
  kDtSampleRate16kHz = 16000, /**< 16 kHz */
  kDtSampleRate32kHz = 32000, /**< 32 kHz */
} DtImuSampleRate,
    dt_imu_sample_rate, DtGyroSampleRate, dt_gyro_sample_rate, DtAccelSampleRate,
    dt_accel_sample_rate, kDtSampleRate;

/**
 * @brief Gyroscope measurement ranges.
 */
typedef enum {
  kDtGyroFsUnknown = -1,
  kDtGyroFs16Dps = 16,     /**< ±16 degrees per second */
  kDtGyroFs31Dps = 31,     /**< ±31 degrees per second */
  kDtGyroFs62Dps = 62,     /**< ±62 degrees per second */
  kDtGyroFs125Dps = 125,   /**< ±125 degrees per second */
  kDtGyroFs250Dps = 250,   /**< ±250 degrees per second */
  kDtGyroFs400Dps = 400,   /**< ±400 degrees per second */
  kDtGyroFs500Dps = 500,   /**< ±500 degrees per second */
  kDtGyroFs800Dps = 800,   /**< ±800 degrees per second */
  kDtGyroFs1000Dps = 1000, /**< ±1000 degrees per second */
  kDtGyroFs2000Dps = 2000, /**< ±2000 degrees per second */
} DtGyroFullScaleRange,
    dt_gyro_full_scale_range;

/**
 * @brief Accelerometer measurement ranges.
 */
typedef enum {
  kDtAccelFsUnknown = -1,
  kDtAccelFs2g = 2,   /**< ±2g */
  kDtAccelFs3g = 3,   /**< ±3g */
  kDtAccelFs4g = 4,   /**< ±4g */
  kDtAccelFs6g = 6,   /**< ±6g */
  kDtAccelFs8g = 8,   /**< ±8g */
  kDtAccelFs12g = 12, /**< ±12g */
  kDtAccelFs16g = 16, /**< ±16g */
  kDtAccelFs24g = 24, /**< ±24g */
} DtAccelFullScaleRange,
    dt_accel_full_scale_range;

/**
 * @brief 3-axis floating point vector.
 */
typedef struct {
  float x; /**< X-axis component */
  float y; /**< Y-axis component */
  float z; /**< Z-axis component */
} DtAccelValue, DtGyroValue, DtFloat3D, dt_accel_value, dt_gyro_value, dt_float_3d;

/** @brief Device state bitmask */
typedef uint64_t DtDeviceState, dt_device_state;

/**
 * @brief Media type bitmask for recording/playback.
 */
typedef enum {
  kDtMediaColorStream = 1,     /**< Color stream data */
  kDtMediaDepthStream = 2,     /**< Depth stream data */
  kDtMediaIrStream = 4,        /**< Infrared stream data */
  kDtMediaGyroStream = 8,      /**< Gyroscope data */
  kDtMediaAccelStream = 16,    /**< Accelerometer data */
  kDtMediaCameraParam = 32,    /**< Camera parameters */
  kDtMediaDeviceInfo = 64,     /**< Device information */
  kDtMediaStreamInfo = 128,    /**< Stream configuration */
  kDtMediaIrLeftStream = 256,  /**< Left IR stream (stereo) */
  kDtMediaIrRightStream = 512, /**< Right IR stream (stereo) */

  /** @brief Bitmask for all media types */
  kDtMediaAll = kDtMediaColorStream | kDtMediaDepthStream | kDtMediaIrStream | kDtMediaGyroStream |
                kDtMediaAccelStream | kDtMediaCameraParam | kDtMediaDeviceInfo |
                kDtMediaStreamInfo | kDtMediaIrLeftStream | kDtMediaIrRightStream,
} DtMediaType,
    dt_media_type;

/**
 * @brief Media playback states.
 */
typedef enum {
  kDtMediaBegin = 0, /**< Playback started */
  kDtMediaPause,     /**< Playback paused */
  kDtMediaResume,    /**< Playback resumed */
  kDtMediaEnd,       /**< Playback ended */
} DtMediaState,
    dt_media_state;

/**
 * @brief 3D point in Cartesian coordinates.
 */
typedef struct {
  float x; /**< X-axis coordinate */
  float y; /**< Y-axis coordinate */
  float z; /**< Z-axis coordinate */
} DtPoint, dt_point, DtPoint3f, dt_point3f;

/**
 * @brief 2D point in Cartesian coordinates.
 */
typedef struct {
  float x; /**< X-axis coordinate */
  float y; /**< Y-axis coordinate */
} DtPoint2f, dt_point2f;

typedef struct {
  float x;             /**< X-axis coordinate */
  float y;             /**< Y-axis coordinate */
  float z;             /**< Z-axis coordinate */
  uint16_t amplitude;  /**< Amplitude */
  uint16_t confidence; /* Confidence */
} DtPointXyzAc, dt_point_xyz_ac;

/**
 * @brief Precomputed XY lookup tables for 3D reconstruction.
 */
typedef struct {
  float* x_table; /**< Precomputed X-coordinate table */
  float* y_table; /**< Precomputed Y-coordinate table */
  int width;      /**< Table width (pixels) */
  int height;     /**< Table height (pixels) */
} DtXyTables, dt_xy_tables;

/**
 * @brief 3D point with RGB color attributes.
 */
typedef struct {
  float x; /**< X-axis coordinate */
  float y; /**< Y-axis coordinate */
  float z; /**< Z-axis coordinate */
  float r; /**< Red component (0.0-1.0) */
  float g; /**< Green component (0.0-1.0) */
  float b; /**< Blue component (0.0-1.0) */
} DtColorPoint, dt_color_point;

/**
 * @brief Control protocol version descriptor.
 */
typedef struct {
  uint8_t major; /**< Major version number (API breaking changes) */
  uint8_t minor; /**< Minor version number (backward-compatible additions) */
  uint8_t patch; /**< Patch version (bug fixes) */
} DtProtocolVersion, dt_protocol_version;

/**
 * @brief IPv4 network configuration parameters.
 */
typedef struct {
  uint16_t dhcp;      /**< DHCP status (0: static, 1: DHCP) */
  uint8_t address[4]; /**< IPv4 address (big-endian order) */
  uint8_t mask[4];    /**< Subnet mask (big-endian order) */
  uint8_t gateway[4]; /**< Gateway address (big-endian order) */
} DtNetIpConfig, dt_net_ip_config;

/**
 * @brief Device communication interfaces.
 */
typedef enum {
  kDtCommUsb = 0x00, /**< USB connection */
  kDtCommNet = 0x01, /**< Ethernet connection */
} DtCommunicationType,
    dt_communication_type;

/**
 * @brief Image rotation options.
 */
typedef enum {
  kDtRotateDegree0 = 0,     /**< No rotation */
  kDtRotateDegree90 = 90,   /**< 90° clockwise rotation */
  kDtRotateDegree180 = 180, /**< 180° rotation */
  kDtRotateDegree270 = 270, /**< 270° clockwise rotation */
} dt_rotate_degree_type,
    DtRotateDegreeType;

/**
 * @brief 3D coordinate system conventions.
 */
typedef enum {
  kDtLeftHandCoordinateSystem = 0,  /**< Left-handed coordinate system (Z forward) */
  kDtRightHandCoordinateSystem = 1, /**< Right-handed coordinate system (Z backward) */
} kDtCoordinateSystemType,
    DtCoordinateSystemType, dt_coordinate_system_type;

/**
 * @brief Region of interest boundaries.
 */
typedef struct {
  uint32_t x;      /**< Top-left x-coordinate */
  uint32_t y;      /**< Top-left y-coordinate */
  uint32_t width;  /**< Region width */
  uint32_t height; /**< Region height */
} DtRegionOfInterest, dt_region_of_interest;

/**
 * @brief Intensity range.
 */
typedef struct {
  float min; /**< Minimum intensity value */
  float max; /**< Maximum intensity value */
} DtIntensityRange, dt_intensity_range;

/**
 * @brief Nebula sensor frequency range.
 */
typedef struct {
  float mtof_high_freq; /**< MTOF high frequency */
  float mtof_low_freq;  /**< MTOF low frequency */
  float stof_high_freq; /**< STOF high frequency */
  float stof_low_freq;  /**< STOF low frequency */
} DtNebulaSensorFreq, dt_nebula_sensor_freq;

/**
 * @brief Filter configuration value types.
 */
typedef enum {
  kDtFilterConfigValueTypeInvalid = -1, /**< Invalid type */
  kDtFilterConfigValueTypeInt = 0,      /**< Integer value */
  kDtFilterConfigValueTypeFloat = 1,    /**< Floating-point value */
  kDtFilterConfigValueTypeBoolean = 2,  /**< Boolean value */
} DtFilterConfigValueType,
    dt_filter_config_value_type;

/**
 * @brief Filter configuration schema item.
 */
typedef struct {
  const char* name;             /**< Configuration parameter name */
  DtFilterConfigValueType type; /**< Value data type */
  double min;                   /**< Minimum allowed value */
  double max;                   /**< Maximum allowed value */
  double step;                  /**< Value increment step */
  double def;                   /**< Default value */
  const char* desc;             /**< Human-readable description */
} DtFilterConfigSchemaItem, dt_filter_config_schema_item;

/**
 * @brief Device serial number container.
 */
typedef struct {
  char number_str[32]; /**< Null-terminated serial number string */
} DtDeviceSerialNumber, dt_device_serial_number, DtSerialNumber, dt_serial_number;

/**
 * @brief Frame metadata attributes.
 *
 * @note Units vary by device - use timestamp conversion functions for accuracy.
 */
typedef enum {
  kDtFrameMetadataTypeTimestamp = 0,              /**< Frame capture timestamp */
  kDtFrameMetadataTypeSensorTimestamp = 1,        /**< Mid-exposure sensor timestamp */
  kDtFrameMetadataTypeFrameNumber = 2,            /**< Sequential frame counter */
  kDtFrameMetadataTypeAutoExposure = 3,           /**< AE status (0: disabled) */
  kDtFrameMetadataTypeExposure = 4,               /**< Exposure time (device-specific units) */
  kDtFrameMetadataTypeGain = 5,                   /**< Sensor gain */
  kDtFrameMetadataTypeAutoWhiteBalance = 6,       /**< AWB status (0: disabled) */
  kDtFrameMetadataTypeWhiteBalance = 7,           /**< Color temperature */
  kDtFrameMetadataTypeBrightness = 8,             /**< Image brightness */
  kDtFrameMetadataTypeContrast = 9,               /**< Image contrast */
  kDtFrameMetadataTypeSaturation = 10,            /**< Color saturation */
  kDtFrameMetadataTypeSharpness = 11,             /**< Edge sharpness */
  kDtFrameMetadataTypeBacklightCompensation = 12, /**< Backlight compensation */
  kDtFrameMetadataTypeHue = 13,                   /**< Hue adjustment */
  kDtFrameMetadataTypeGamma = 14,                 /**< Gamma correction */
  kDtFrameMetadataTypeActualFrameRate = 15,       /**< Achieved frame rate (Fps) */
  kDtFrameMetadataTypeFrameRate = 16,             /**< Configured frame rate (Fps) */
  kDtFrameMetadataTypeAeRoiX = 17,                /**< AE region x */
  kDtFrameMetadataTypeAeRoiY = 18,                /**< AE region y */
  kDtFrameMetadataTypeAeRoiWidth = 19,            /**< AE region width */
  kDtFrameMetadataTypeAeRoiHeight = 20,           /**< AE region height */
  kDtFrameMetadataTypeLaserStatus = 21,           /**< Laser state (0: off, 1: on) */
  kDtFrameMetadataTypeTxTemperature = 22,         /**< Transmitter temperature (°C) */
  kDtFrameMetadataTypeTxDriverTemperature = 23,   /**< Driver IC temperature (°C) */
  kDtFrameMetadataTypeSocTemperature = 24,        /**< SoC temperature (°C) */
  kDtFrameMetadataTypeMainBoardTemperature = 25,  /**< Mainboard temperature (°C) */
  kDtFrameMetadataTypeMtofExposure = 26,          /**< MToF exposure time */
  kDtFrameMetadataTypeStofExposure = 27,          /**< SToF exposure time */
  kDtFrameMetadataTypeFrameType = 28,             /**< Nebula:Depth frame type (0: MToF, 1: SToF)
                                                      Pisca: 0:speckle 1:background */
  kDtFrameMetadataTypeFrameContextId = 29,        /**< Context ID (device-specific) */
  kDtFrameMetadataTypePalmWorkMode = 30, /**< Palm work mode (0: recognition; 1: register) */
  kDtFrameMetadataTypeHandType = 31,     /**< Hand type (0: left; 1: right) */
  kDtFrameMetadataTypeCount,             /**< Total metadata types (not a valid type) */
} dt_frame_metadata_type,
    DtFrameMetadataType;

/**
 * @brief Linux UVC backend implementations.
 */
typedef enum {
  kDtUvcBackendTypeAuto,   /**< Automatic backend selection */
  kDtUvcBackendTypeLibuvc, /**< libuvc-based implementation */
  kDtUvcBackendTypeV4l2,   /**< V4L2-based implementation */
} dt_uvc_backend_type,
    DtUvcBackendType;

/**
 * @brief Media playback states.
 */
typedef enum {
  kDtPlaybackUnknown, /**< State indeterminate */
  kDtPlaybackPlaying, /**< Actively playing */
  kDtPlaybackPaused,  /**< Playback paused */
  kDtPlaybackStopped, /**< Playback stopped */
} dt_playback_status,
    DtPlaybackStatus;
const char* dt_playback_status_to_string(dt_playback_status value);

/**
 * @brief SDK exception categories.
 */
typedef enum {
  kDtExceptionTypeUnknown,              /**< Unclassified error */
  kDtExceptionTypeStdException,         /**< Standard library exception */
  kDtExceptionTypeCameraDisconnected,   /**< Device disconnection */
  kDtExceptionTypePlatform,             /**< Platform adaptation error */
  kDtExceptionTypeInvalidValue,         /**< Invalid parameter value */
  kDtExceptionTypeWrongApiCallSequence, /**< API called out of sequence */
  kDtExceptionTypeNotImplemented,       /**< Unsupported functionality */
  kDtExceptionTypeIo,                   /**< I/O operation failure */
  kDtExceptionTypeMemory,               /**< Memory allocation error */
  kDtExceptionTypeUnsupportedOperation, /**< Operation not supported */
} DtExceptionType,
    dt_exception_type;

/**
 * @brief Detailed error information container.
 */
typedef struct dt_error {
  dt_status status;                 /**< Error status code */
  char message[256];                /**< Human-readable error description */
  char function[256];               /**< Function where error occurred */
  char args[256];                   /**< Arguments passed to function */
  dt_exception_type exception_type; /**< Error classification */
} dt_error;

/**
 * @brief File transfer progress callback.
 *
 * @param[in] state Current transfer state.
 * @param[in] message Status message.
 * @param[in] percent Completion percentage.
 * @param[in] user_data User-defined context.
 */
typedef void (*dt_file_tran_callback)(dt_file_tran_state state,
                                      const char* message,
                                      uint8_t percent,
                                      void* user_data);

/**
 * @brief Data write progress callback.
 *
 * @param[in] state Current transfer state.
 * @param[in] percent Completion percentage.
 * @param[in] user_data User-defined context.
 */
typedef void (*dt_set_data_callback)(dt_data_tran_state state, uint8_t percent, void* user_data);

/**
 * @brief Data read completion callback.
 *
 * @param[in] state Current transfer state.
 * @param[in] data_chunk Received data block.
 * @param[in] user_data User-defined context.
 */
typedef void (*dt_get_data_callback)(dt_data_tran_state state,
                                     dt_data_chunk* data_chunk,
                                     void* user_data);

/**
 * @brief Firmware update progress callback.
 *
 * @param[in] state Current update state.
 * @param[in] message Status message.
 * @param[in] percent Completion percentage.
 * @param[in] user_data User-defined context.
 */
typedef void (*dt_device_fw_update_callback)(dt_fw_update_state state,
                                             const char* message,
                                             uint8_t percent,
                                             void* user_data);

/**
 * @brief Device state change callback.
 *
 * @param[in] state New device state.
 * @param[in] message Status message.
 * @param[in] user_data User-defined context.
 */
typedef void (*dt_device_state_callback)(dt_device_state state,
                                         const char* message,
                                         void* user_data);

/**
 * @brief Media state change callback.
 *
 * @param[in] state New media state.
 * @param[in] user_data User-defined context.
 */
typedef void (*dt_media_state_callback)(dt_media_state state, void* user_data);

/**
 * @brief Device connection change callback.
 *
 * @param[in] removed List of disconnected devices.
 * @param[in] added List of connected devices.
 * @param[in] user_data User-defined context.
 */
typedef void (*dt_device_changed_callback)(dt_device_list* removed,
                                           dt_device_list* added,
                                           void* user_data);

/**
 * @brief Frame arrival callback.
 *
 * @param[in] frame Received frame object。
 * @param[in] user_data User-defined context.
 */
typedef void (*dt_frame_callback)(dt_frame* frame, void* user_data);
#define dt_filter_callback dt_frame_callback
#define dt_playback_callback dt_frame_callback

/**
 * @brief Frameset arrival callback.
 *
 * @param[in] frameset Received frameset object.
 * @param[in] user_data User-defined context.
 */
typedef void (*dt_frameset_callback)(dt_frame* frameset, void* user_data);

/**
 * @brief Custom frame buffer deallocation callback.
 *
 * @param[in] buffer Memory to deallocate.
 * @param[in] user_data User-defined context.
 */
typedef void(dt_frame_destroy_callback)(uint8_t* buffer, void* user_data);

/**
 * @brief Log message callback.
 *
 * @param[in] severity Message severity level.
 * @param[in] message Log content.
 * @param[in] user_data User-defined context.
 */
typedef void(dt_log_callback)(dt_log_severity severity, const char* message, void* user_data);

/**
 * @brief Playback state change callback.
 *
 * @param[in] status New playback state.

 * @param[in] user_data User-defined context.
 */
typedef void (*dt_playback_status_changed_callback)(dt_playback_status status, void* user_data);

/**
 * @brief Check if a sensor type is video-based.
 *
 * @param[in] sensor_type Sensor type to test.

 * @return True for video sensors.
 */
#define DT_IS_VIDEO_SENSOR_TYPE(sensor_type)                                   \
  ((sensor_type) == kDtSensorColor || (sensor_type) == kDtSensorDepth ||       \
   (sensor_type) == kDtSensorIr || (sensor_type) == kDtSensorIrLeft ||         \
   (sensor_type) == kDtSensorIrRight || (sensor_type) == kDtSensorColorLeft || \
   (sensor_type) == kDtSensorColorRight)

/**
 * @brief Check if a stream type is video-based.
 *
 * @param[in] stream_type Stream type to test.
 * @return True for video streams.
 */
#define DT_IS_VIDEO_STREAM_TYPE(stream_type)                               \
  ((stream_type) == kDtStreamColor || (stream_type) == kDtStreamDepth ||   \
   (stream_type) == kDtStreamIr || (stream_type) == kDtStreamIrLeft ||     \
   (stream_type) == kDtStreamIrRight || (stream_type) == kDtStreamVideo || \
   (stream_type) == kDtStreamColorRight || (stream_type) == kDtStreamColorLeft)

/**
 * @brief Check if a sensor type is IR-based.
 *
 * @param[in] sensor_type Sensor type to test.
 * @return True for IR sensors.
 */
#define DT_IS_IR_SENSOR(sensor_type)                                   \
  ((sensor_type) == kDtSensorIr || (sensor_type) == kDtSensorIrLeft || \
   (sensor_type) == kDtSensorIrRight)

/**
 * @brief Check if a stream type is IR-based.
 *
 * @param[in] stream_type Stream type to test
 * @return True for IR streams.
 */
#define DT_IS_IR_STREAM(stream_type)                                   \
  ((stream_type) == kDtStreamIr || (stream_type) == kDtStreamIrLeft || \
   (stream_type) == kDtStreamIrRight)

/**
 * @brief Check if a frame type is IR-based.
 *
 * @param[in] frame_type Frame type to test
 * @return True for IR frames.
 */
#define DT_IS_IR_FRAME(frame_type) \
  ((frame_type) == kDtFrameIr || (frame_type) == kDtFrameIrLeft || (frame_type) == kDtFrameIrRight)

const int32_t kDtWidthAny = 0;                  /**< Wildcard for any width */
const int32_t kDtHeightAny = 0;                 /**< Wildcard for any height */
const int32_t kDtFpsAny = 0;                    /**< Wildcard for any frame rate */
const DtFormat kDtFormatAny = kDtFormatUnknown; /**< Wildcard for any format */
const int32_t kDtProfileDefault = 0;            /**< Default profile index */
const int32_t kDtDefaultStrideBytes = 0;        /**< Auto-calculate stride */
const DtAccelFullScaleRange kDtAccelFullScaleRangeAny = kDtAccelFsUnknown;
const DtAccelSampleRate kDtAccelSampleRateAny = kDtSampleRateUnknown;
const DtGyroFullScaleRange kDtGyroFullScaleRangeAny = kDtGyroFsUnknown;
const DtGyroSampleRate kDtGyroSampleRateAny = kDtSampleRateUnknown;

#ifdef __cplusplus
}  // extern "C"
#endif

#pragma pack(pop)

#endif  // DEPTRUM_H_DT_TYPES_H_