// Copyright (c) Deptrum Inc. All Rights Reserved.

/**
 * @file dt_utils.hpp
 * @brief Utility classes for coordinate transformations and point cloud operations.
 */

#ifndef DEPTRUM_HPP_DT_UTILS_HPP_
#define DEPTRUM_HPP_DT_UTILS_HPP_

#include <memory>
#include "deptrum/h/dt_utils.h"
#include "dt_device.hpp"
#include "dt_frame.hpp"
#include "dt_types.hpp"

namespace dt {
class Device;

/**
 * @brief Point cloud processing utilities.
 */
class PointCloudHelper {
 public:
  /**
   * @brief Saves point cloud to PLY file format.
   *
   * @param[in] file_name Output file path.
   * @param[in] frame Point cloud frame containing data.
   * @param[in] save_binary Selects binary (true) or ASCII (false) format.
   * @param[in] use_mesh Enables mesh generation from point cloud.
   * @param[in] mesh_threshold Maximum distance for mesh face creation in millimeters.
   * @return bool True if save operation succeeded.
   */
  static bool SavePointcloudToPly(const char* file_name,
                                  std::shared_ptr<dt::Frame> frame,
                                  bool save_binary,
                                  bool use_mesh,
                                  float mesh_threshold = 50.0f) {
    dt_error* error = NULL;
    auto un_const_impl = const_cast<dt_frame*>(frame->GetImpl());
    bool result = dt_save_pointcloud_to_ply(file_name,
                                            un_const_impl,
                                            save_binary,
                                            use_mesh,
                                            mesh_threshold,
                                            &error);
    Error::Handle(&error, false);
    return result;
  }
};

/**
 * @brief Coordinate transform utilities.
 */
class CoordinateTransformHelper {
 public:
  /**
   * @brief Transforms 3D point between coordinate systems.
   *
   * @param[in] source_point3f Source 3D point in original coordinate system.
   * @param[in] extrinsic Transform matrix from source to target coordinate system.
   * @param[out] target_point3f Transformed 3D point in target coordinate system.
   * @return bool True if transform succeeded.
   */
  static bool Transform3dto3d(const DtPoint3f source_point3f,
                              DtExtrinsic extrinsic,
                              DtPoint3f* target_point3f) {
    dt_error* error = NULL;
    bool result = dt_transform_3d_to_3d(source_point3f, extrinsic, target_point3f, &error);
    Error::Handle(&error);
    return result;
  }

  /**
   * @brief Projects 2D pixel with depth to 3D point in target coordinate system.
   *
   * @param[in] source_point2f Source 2D pixel coordinates.
   * @param[in] source_depth_pixel_value Depth value at pixel in millimeters.
   * @param[in] source_intrinsic Intrinsic parameters of source camera.
   * @param[in] extrinsic Transform matrix from source to target coordinate system.
   * @param[out] target_point3f Resulting 3D point in target coordinate system.
   * @return bool True if transform succeeded.
   */
  static bool Transform2dto3d(const DtPoint2f source_point2f,
                              const float source_depth_pixel_value,
                              const DtCameraIntrinsic source_intrinsic,
                              DtExtrinsic extrinsic,
                              DtPoint3f* target_point3f) {
    dt_error* error = NULL;
    bool result = dt_transform_2d_to_3d(source_point2f,
                                        source_depth_pixel_value,
                                        source_intrinsic,
                                        extrinsic,
                                        target_point3f,
                                        &error);
    Error::Handle(&error);
    return result;
  }

  /**
   * @brief Projects 3D point to 2D pixel in target camera.
   *
   * @param[in] source_point3f Source 3D point in original coordinate system.
   * @param[in] target_intrinsic Intrinsic parameters of target camera.
   * @param[in] target_distortion Distortion coefficients of target camera.
   * @param[in] extrinsic Transform matrix from source to target coordinate system.
   * @param[out] target_point2f Resulting 2D pixel coordinates.
   * @return bool True if projection succeeded.
   */
  static bool Transform3dto2d(const DtPoint3f source_point3f,
                              const DtCameraIntrinsic target_intrinsic,
                              const DtCameraDistortion target_distortion,
                              DtExtrinsic extrinsic,
                              DtPoint2f* target_point2f) {
    dt_error* error = NULL;
    bool result = dt_transform_3d_to_2d(source_point3f,
                                        target_intrinsic,
                                        target_distortion,
                                        extrinsic,
                                        target_point2f,
                                        &error);
    Error::Handle(&error);
    return result;
  }

  /**
   * @brief Transforms 2D pixel between camera systems.
   *
   * @param[in] source_point2f Source 2D pixel coordinates.
   * @param[in] source_depth_pixel_value Depth value at pixel in millimeters.
   * @param[in] source_intrinsic Intrinsic parameters of source camera.
   * @param[in] source_distortion Distortion coefficients of source camera.
   * @param[in] target_intrinsic Intrinsic parameters of target camera.
   * @param[in] target_distortion Distortion coefficients of target camera.
   * @param[in] extrinsic Transform matrix between camera coordinate systems.
   * @param[out] target_point2f Resulting 2D pixel coordinates in target camera.
   * @return bool True if transform succeeded.
   */
  static bool Transform2dto2d(const DtPoint2f source_point2f,
                              const float source_depth_pixel_value,
                              const DtCameraIntrinsic source_intrinsic,
                              const DtCameraDistortion source_distortion,
                              const DtCameraIntrinsic target_intrinsic,
                              const DtCameraDistortion target_distortion,
                              DtExtrinsic extrinsic,
                              DtPoint2f* target_point2f) {
    dt_error* error = NULL;
    bool result = dt_transform_2d_to_2d(source_point2f,
                                        source_depth_pixel_value,
                                        source_intrinsic,
                                        source_distortion,
                                        target_intrinsic,
                                        target_distortion,
                                        extrinsic,
                                        target_point2f,
                                        &error);
    Error::Handle(&error);
    return result;
  }
};
/**
 * Default colormap table, 128 mapping values in total
 *
 * When using the color mapping table, three adjustment parameters need to be settled,
 * which are range_min, range_mid, range_max in millimeter (mm).
 * In order to make the foreground image have more colorful color changes,
 * the mapping function will map the last 100 color scales to range_min-range_mid
 * and the first 28 color scales to range_mid-range_max.
 */

class Visualizer {
 public:
  /**
   * Colormap the depth map on given colormap_table
   * Function referenced from Stellar Visualizer
   *
   * @param[out] colored_depth Colored depth map, memory needs to be applied externally.
   * @param[in] rows Rows of depth map.
   * @param[in] cols Columns of depth map.
   * @param[in] range_min Minimum mapping range of depth map.
   * @param[in] range_mid Middle value of mapping range of depth map.
   * @param[in] range_max Maximum mapping range of depth map.
   * @param[in] depth_map Depth map to be color mapped.
   * @return bool True if transformation succeeded.
   */
  static bool ColorizeDepth(uint8_t* colored_depth,
                            int rows,
                            int cols,
                            int range_min,
                            int range_mid,
                            int range_max,
                            const uint16_t* depth_map) {
    dt_error* error = NULL;
    bool result = dt_colorize_depth(colored_depth,
                                    rows,
                                    cols,
                                    range_min,
                                    range_mid,
                                    range_max,
                                    depth_map,
                                    &error);
    Error::Handle(&error);
    return result;
  }
};
}  // namespace dt

#endif  // DEPTRUM_HPP_DT_UTILS_HPP_
