// Copyright (c) Deptrum Inc. All Rights Reserved.

/**
 * @file dt_option.h
 * @brief Device control options and command definitions.
 */

#ifndef DEPTRUM_H_DT_OPTION_H_
#define DEPTRUM_H_DT_OPTION_H_

#include "dt_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Device control option identifiers.
 *
 * @note Options are listed in their original declaration order.
 */
typedef enum {
  /**
   * @brief Enable/disable auto exposure for depth camera.
   * @note Affects IR camera on compatible devices.
   */
  kDtOptionDepthAutoExposureBool = 1,

  /**
   * @brief Manual exposure adjustment for depth camera.
   * @note Affects IR camera on compatible devices.
   */
  kDtOptionDepthExposureInt = 2,

  /**
   * @brief Exposure adjustment for STOF depth frames.
   */
  kDtOptionStofDepthExposureInt = 3,

  /**
   * @brief Exposure adjustment for MTOF depth frames.
   */
  kDtOptionMtofDepthExposureInt = 4,

  /**
   * @brief Minimum depth threshold in millimeters.
   */
  kDtOptionMinDepthInt = 5,

  /**
   * @brief Maximum depth threshold in millimeters.
   */
  kDtOptionMaxDepthInt = 6,

  /**
   * @brief Light source control mode
   * @note 0 All off
   * @note 1 Laser on, LED off
   * @note 2 Laser off, LED on
   * @note 3 All on.
   */
  kDtOptionLightModeInt = 7,

  /**
   * @brief Auto-exposure ROI for depth sensor, @ref DtRegionOfInterest.
   * @note Also affects IR sensor which shares physical sensor.
   */
  kDtOptionDepthAeRoiStruct = 10,

  /**
   * @brief Auto-exposure ROI for STOF depth frames, @ref DtRegionOfInterest.
   */
  kDtOptionStofDepthAeRoiStruct = 11,

  /**
   * @brief Auto-exposure ROI for MTOF depth frames, @ref DtRegionOfInterest.
   */
  kDtOptionMtofDepthAeRoiStruct = 12,

  /**
   * @brief Depth auto-exposure algorithm parameters.
   */
  kDtOptionDepthAeParamStruct = 13,

  /**
   * @brief Extended RGB resolution setting
   * @note 0 Disabled
   * @note 1 1920x1080
   * @note 2 1280x720 (Nebula series only).
   */
  kDtOptionExtRgbInt = 15,

  /**
   * @brief Verification mode toggle.
   */
  kDtOptionVerifyBool = 16,

  /**
   * @brief Debug mode (internal use only)
   * @note Enables raw data transmission.
   */
  kDtOptionDebugModeBool = 17,

  /**
   * @brief Heartbeat monitoring toggle.
   */
  kDtOptionHeartbeatBool = 30,

  /**
   * @brief Speckle exposure adjustment.
   */
  kDtOptionSpeckleExposureInt = 31,

  /**
   * @brief White LED control.
   */
  kDtOptionWhiteLightBool = 50,

  /**
   * @brief Laser pulse interval (frames between activations).
   */
  kDtOptionLaserControlIntervalInt = 51,

  /**
   * @brief Laser trigger delay time (higher values increase delay).
   */
  kDtOptionTriggerLaserDelayTimeInt = 52,

  /**
   * @brief Configuration sequence identifier.
   */
  kDtOptionSequenceIdInt = 53,

  /**
   * @brief Surveillance mode
   * @note Requires stream restart to take effect.
   */
  kDtOptionSurveillanceModeBool = 54,

  /**
   * @brief High exposure frame value.
   */
  kDtOptionHighExposureInt = 60,

  /**
   * @brief Low exposure frame value.
   */
  kDtOptionLowExposureInt = 61,

  /**
   * @brief Background frame exposure.
   */
  kDtOptionBgExposureInt = 62,

  /**
   * @brief High exposure gain.
   */
  kDtOptionHighGainInt = 63,

  /**
   * @brief Low exposure gain.
   */
  kDtOptionLowGainInt = 64,

  /**
   * @brief Background frame gain.
   */
  kDtOptionBgGainInt = 65,

  /**
   * @brief Camera power state.
   */
  kDtOptionCameraPowerBool = 66,

  /**
   * @brief Laser power state.
   */
  kDtOptionLaserBool = 67,

  /**
   * @brief Laser current in milliamps (mA).
   */
  kDtOptionLaserCurrentInt = 68,

  /**
   * @brief IMU calibration mode.
   */
  kDtOptionImuCalibBool = 69,

  /**
   * @brief File runner mode.
   */
  kDtOptionFileRunnerBool = 70,

  /**
   * @brief Camera calibration mode.
   */
  kDtOptionCameraCalibBool = 80,

  /**
   * @brief STOF depth cropping configuration (Y-direction only)
   * @format uint8_t[4] = {x, y, width, height}
   * @details
   *   - Only cropping in Y direction is supported.
   *   - x must be 0.
   *   - width is fixed to 248.
   *   - y represents crop_up.
   *   - height = 180 - crop_up - crop_down.
   * @constraint crop_up + crop_down < 170
   * @constraint crop_up + crop_down must be even
   * @note For Nebula280 series, the ROI for MTOF and STOF must be set to the same values; cropping
   * STOF or MTOF individually is not supported.
   */
  kDtOptionStofDepthCropRoiStruct = 90,

  /**
   * @brief MTOF depth cropping configuration (Y-direction only)
   * @format uint8_t[4] = {x, y, width, height}
   * @details
   *   - Only cropping in Y direction is supported.
   *   - x must be 0.
   *   - width is fixed to 248.
   *   - y represents crop_up.
   *   - height = 180 - crop_up - crop_down.
   * @constraint crop_up + crop_down < 170
   * @constraint crop_up + crop_down must be even
   * @note For Nebula280 series, the ROI for MTOF and STOF must be set to the same values; cropping
   * STOF or MTOF individually is not supported.
   */
  kDtOptionMtofDepthCropRoiStruct = 91,

  /**
   * @brief Depth sensor operation mode
   * @value 0 Single MTOF AE
   * @value 1 Structured light + MTOF fusion(default)
   * @value 2 MTOF HDR
   * @value 3 MTOF + STOF + IR (Nebula280 only).
   */
  kDtOptionDepthModeInt = 92,

  /**
   * @brief Depth exposure parameters (internal use).
   */
  kDtOptionDepthExposureStruct = 93,

  /**
   * @brief AE algorithm parameters (internal use).
   */
  kDtOptionAeParametersStruct = 94,

  /**
   * @brief MTOF auto-exposure upper limit.
   */
  kDtOptionMtofAeUpperInt = 95,

  /**
   * @brief MTOF auto-exposure lower limit.
   */
  kDtOptionMtofAeLowerInt = 96,

  /**
   * @brief Sensor frequency range, @ref DtNebulaSensorFreq.
   */
  kDtOptionSensorFreqRangeStruct = 97,

  /**
   * @brief Laser pulse width control.
   */
  kDtOptionLaserPulseWidthInt = 200,

  /**
   * @brief Depth image mirroring.
   */
  kDtOptionDepthMirrorBool = 210,

  /**
   * @brief Depth image flipping.
   */
  kDtOptionDepthFlipBool = 211,

  /**
   * @brief IR image mirroring.
   */
  kDtOptionIrMirrorBool = 212,

  /**
   * @brief IR image flipping.
   */
  kDtOptionIrFlipBool = 213,

  /**
   * @brief Color image mirroring.
   */
  kDtOptionColorMirrorBool = 214,

  /**
   * @brief Color image flipping.
   */
  kDtOptionColorFlipBool = 215,

  /**
   * @brief Right IR image mirroring.
   */
  kDtOptionIrRightMirrorBool = 216,

  /**
   * @brief Right IR image flipping.
   */
  kDtOptionIrRightFlipBool = 217,

  /**
   * @brief Color sensor rotation
   * @note 0   0°
   * @note 90  90°
   * @note 180 180°
   * @note 270 270°.
   */
  kDtOptionColorRotateInt = 218,

  /**
   * @brief IR/Left IR sensor rotation
   * @note 0   0°
   * @note 90  90°
   * @note 180 180°
   * @note 270 270°.
   */
  kDtOptionIrRotateInt = 219,

  /**
   * @brief Right IR sensor rotation
   * @note 0   0°
   * @note 90  90°
   * @note 180 180°
   * @note 270 270°.
   */
  kDtOptionIrRightRotateInt = 220,

  /**
   * @brief Depth sensor rotation
   * @note 0   0°
   * @note 90  90°
   * @note 180 180°
   * @note 270 270°.
   */
  kDtOptionDepthRotateInt = 221,

  /**
   * @brief Timer reset enable.
   */
  kDtOptionTimerResetEnableBool = 250,

  /**
   * @brief Laser control mode
   * @note 0 Off
   * @note 1 On
   * @note 2 Auto.
   */
  kDtOptionLaserControlInt = 251,

  /**
   * @brief IR brightness adjustment.
   */
  kDtOptionIrBrightnessInt = 252,

  /**
   * @brief Slave device sync status (read-only).
   */
  kDtOptionSlaveDeviceSyncStatusBool = 253,

  /**
   * @brief Color AE maximum exposure limit.
   */
  kDtOptionColorAeMaxExposureInt = 254,

  /**
   * @brief IR AE maximum exposure limit.
   */
  kDtOptionIrAeMaxExposureInt = 255,

  /**
   * @brief Device serial number.
   */
  kDtOptionDeviceSerialNumberStruct = 300,

  /**
   * @brief Device time configuration.
   */
  kDtOptionDeviceTimeStruct = 301,

  /**
   * @brief Network IP configuration, @ref DtNetIpConfig.
   */
  kDtOptionDeviceIpAddrConfigStruct = 302,

  /**
   * @brief Color AE ROI configuration, @ref DtRegionOfInterest.
   */
  kDtOptionColorAeRoiStruct = 303,

  /**
   * @brief ASIC serial number.
   */
  kDtOptionAsicSerialNumberStruct = 304,

  /**
   * @brief Depth confidence threshold (0-100).
   */
  kDtOptionConfidenceThresholdInt = 305,

  /**
   * @brief Fusion with structured light depth
   */
  kDtOptionFusionWithSlDepthBool = 306,

  /**
   * @brief Color auto exposure.
   */
  kDtOptionColorAutoExposureBool = 500,

  /**
   * @brief Color exposure adjustment.
   */
  kDtOptionColorExposureInt = 501,

  /**
   * @brief Color gain adjustment.
   */
  kDtOptionColorGainInt = 502,

  /**
   * @brief Color auto white balance.
   */
  kDtOptionColorAutoWhiteBalanceBool = 503,

  /**
   * @brief Color white balance adjustment.
   */
  kDtOptionColorWhiteBalanceInt = 504,

  /**
   * @brief Color brightness adjustment.
   */
  kDtOptionColorBrightnessInt = 505,

  /**
   * @brief Color sharpness adjustment.
   */
  kDtOptionColorSharpnessInt = 506,

  /**
   * @brief Color shutter speed adjustment.
   */
  kDtOptionColorShutterInt = 507,

  /**
   * @brief Color saturation adjustment.
   */
  kDtOptionColorSaturationInt = 508,

  /**
   * @brief Color contrast adjustment.
   */
  kDtOptionColorContrastInt = 509,

  /**
   * @brief Color gamma adjustment.
   */
  kDtOptionColorGammaInt = 510,

  /**
   * @brief Color image rotation.
   */
  kDtOptionColorRollInt = 511,

  /**
   * @brief Color AE priority adjustment.
   */
  kDtOptionColorAutoExposurePriorityInt = 512,

  /**
   * @brief Color backlight compensation.
   */
  kDtOptionColorBacklightCompensationInt = 513,

  /**
   * @brief Color tint adjustment.
   */
  kDtOptionColorHueInt = 514,

  /**
   * @brief Power line frequency setting.
   */
  kDtOptionColorPowerLineFrequencyInt = 515,

  /**
   * @brief Color focus adjustment.
   */
  kDtOptionColorFocusInt = 516,

  /**
   * @brief Depth gain adjustment.
   * @note Affects IR camera on compatible devices.
   */
  kDtOptionDepthGainInt = 518,

  /**
   * @brief IR auto exposure.
   * @note Affects depth camera on compatible devices.
   */
  kDtOptionIrAutoExposureBool = 525,

  /**
   * @brief IR exposure adjustment.
   * @note Affects depth camera on compatible devices.
   */
  kDtOptionIrExposureInt = 526,

  /**
   * @brief IR gain adjustment.
   * @note Affects depth camera on compatible devices.
   */
  kDtOptionIrGainInt = 527,

  /**
   * @brief Ir current (uint: mA).
   */
  kDtOptionIrCurrentInt = 5000,

  /**
   * @brief White current (uint: mA).
   */
  kDtOptionWhiteCurrentInt = 5001,

  /**
   * @brief Breath mode, @ref DtBreathMode.
   */
  kDtOptionBreathModeInt = 5002,

  /**
   * @brief Dim palm mode, true: enable, false: disable.
   */
  kDtOptionDimPalmEnableBool = 5003,

  /**
   * @brief Dim palm mode, @ref DtDimPalmMode.
   */
  kDtOptionDimPalmModeInt = 5004,

  /**
   * @brief Transfer raw image in dim palm mode, true: enable, false: disable.
   */
  kDtOptionTransferRawImageInFeatReportBool = 5005,

  /**
   * @brief Dim palm rectangular boundary, @ref DtRegionOfInterest.
   */
  kDtOptionDimPalmIrRecognizeRectBoundaryStruct = 5006,

  /**
   * @brief Dim palm rectangular boundary, @ref DtRegionOfInterest.
   */
  kDtOptionDimPalmIrRegisterRectBoundaryStruct = 5007,

  /**
   * @brief Dim palm intensity range, @ref DtIntensityRange.
   */
  kDtOptionDimPalmIrRecognizeIntensityRangeStruct = 5008,

  /**
   * @brief Dim palm intensity range, @ref DtIntensityRange.
   */
  kDtOptionDimPalmIrRegisterIntensityRangeStruct = 5009,

  /**
   * @brief Dim palm quality range, @ref DtIntensityRange.
   */
  kDtOptionDimPalmIrRecognizeQualityRangeStruct = 5010,

  /**
   * @brief Dim palm quality range, @ref DtIntensityRange.
   */
  kDtOptionDimPalmIrRegisterQualityRangeStruct = 5011,

  /**
   * @brief Dim palm liveness range, @ref DtIntensityRange.
   */
  kDtOptionDimPalmIrLivenessRangeStruct = 5012,

  /**
   * @brief Dim palm reliability range, @ref DtIntensityRange.
   */
  kDtOptionDimPalmIrRecognizeReliabilityRangeStruct = 5013,

  /**
   * @brief Dim palm reliability range, @ref DtIntensityRange.
   */
  kDtOptionDimPalmIrRegisterReliabilityRangeStruct = 5014,

  /**
   * @brief Dim palm maximum motion threshold for Register.
   */
  kDtOptionDimPalmIrRegisterMaxMotionInt = 5015,
} DtOptionId,
    dt_option_id;

/**
 * @brief Integer option value constraints.
 */
typedef struct {
  int32_t cur;  /**< Current value */
  int32_t max;  /**< Maximum allowed value */
  int32_t min;  /**< Minimum allowed value */
  int32_t step; /**< Value increment step */
  int32_t def;  /**< Default value */
} DtIntOptionRange, dt_int_option_range;

/**
 * @brief Floating-point option value constraints.
 */
typedef struct {
  float cur;  /**< Current value */
  float max;  /**< Maximum allowed value */
  float min;  /**< Minimum allowed value */
  float step; /**< Value increment step */
  float def;  /**< Default value */
} DtFloatOptionRange, dt_float_option_range;

/**
 * @brief Unsigned 16-bit integer option constraints.
 */
typedef struct {
  uint16_t cur;  /**< Current value */
  uint16_t max;  /**< Maximum allowed value */
  uint16_t min;  /**< Minimum allowed value */
  uint16_t step; /**< Value increment step */
  uint16_t def;  /**< Default value */
} DtUint16OptionRange, dt_uint16_option_range;

/**
 * @brief Unsigned 8-bit integer option constraints.
 */
typedef struct {
  uint8_t cur;  /**< Current value */
  uint8_t max;  /**< Maximum allowed value */
  uint8_t min;  /**< Minimum allowed value */
  uint8_t step; /**< Value increment step */
  uint8_t def;  /**< Default value */
} DtUint8OptionRange, dt_uint8_option_range;

/**
 * @brief Boolean option constraints.
 */
typedef struct {
  bool cur;  /**< Current state */
  bool max;  /**< Maximum state (always true) */
  bool min;  /**< Minimum state (always false) */
  bool step; /**< Step size (not applicable) */
  bool def;  /**< Default state */
} DtBoolOptionRange, dt_bool_option_range;

/**
 * @brief Operation permission levels.
 */
typedef enum {
  kDtPermissionDeny = 0,      /**< No access */
  kDtPermissionRead = 1,      /**< Read-only access */
  kDtPermissionWrite = 2,     /**< Write-only access */
  kDtPermissionReadWrite = 3, /**< Read-write access */
  kDtPermissionAny = 255,     /**< Any permission level */
} DtPermissionType,
    dt_permission_type;

/**
 * @brief Option data types.
 */
typedef enum {
  kDtOptionBool = 0,   /**< Boolean option */
  kDtOptionInt = 1,    /**< Integer option */
  kDtOptionFloat = 2,  /**< Floating-point option */
  kDtOptionStruct = 3, /**< Structured data option */
} DtOptionType,
    dt_option_type;

/**
 * @brief Option descriptor.
 */
typedef struct {
  DtOptionId id;               /**< Unique option identifier */
  const char* name;            /**< Human-readable option name */
  DtOptionType type;           /**< Data type of option */
  DtPermissionType permission; /**< Access permissions */
} DtOptionItem, dt_option_item;

#ifdef __cplusplus
}
#endif

#endif  // DEPTRUM_H_DT_OPTION_H_