// Copyright (c) Deptrum Inc. All Rights Reserved.

/**
 * @file dt_utils.h
 * @brief Utility functions for coordinate transformations.
 */

#ifndef DEPTRUM_H_DT_UTILS_H_
#define DEPTRUM_H_DT_UTILS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "dt_types.h"

/**
 * @brief Saves point cloud data to PLY file format.
 *
 * @param[in] file_name Output file path for PLY file.
 * @param[in] frame Frame containing point cloud data.
 * @param[in] save_binary Use binary format (true) or ASCII format (false).
 * @param[in] use_mesh Generate mesh faces (true) or save as point cloud only (false).
 * @param[in] mesh_threshold Distance threshold for mesh generation (in millimeters).
 * @param[out] error Optional error object for capturing operation status.
 * @return bool True if save operation succeeded, false otherwise.
 */
DT_EXPORT bool dt_save_pointcloud_to_ply(const char* file_name,
                                         dt_frame* frame,
                                         bool save_binary,
                                         bool use_mesh,
                                         float mesh_threshold,
                                         dt_error** error);

/**
 * @brief Transforms a 3D point from source coordinate system to target coordinate system.
 *
 * @param[in] source_point3f 3D point in source coordinate system.
 * @param[in] extrinsic Transform matrix from source to target coordinate system.
 * @param[out] target_point3f Output transformed 3D point in target coordinate system.
 * @param[out] error Optional error object for capturing operation status.
 * @return bool True if transform succeeded, false otherwise.
 */
DT_EXPORT bool dt_transform_3d_to_3d(const DtPoint3f source_point3f,
                                     DtExtrinsic extrinsic,
                                     DtPoint3f* target_point3f,
                                     dt_error** error);

/**
 * @brief Projects a 2D pixel with depth value to 3D point in target coordinate system.
 *
 * @param[in] source_point2f 2D pixel coordinates in source camera.
 * @param[in] source_depth_pixel_value Depth value at pixel location (in millimeters).
 * @param[in] source_intrinsic Intrinsic parameters of source camera.
 * @param[in] extrinsic Transform from source to target coordinate system.
 * @param[out] target_point3f Output 3D point in target coordinate system.
 * @param[out] error Optional error object for capturing operation status.
 * @return bool True if projection succeeded, false otherwise.
 */
DT_EXPORT bool dt_transform_2d_to_3d(const DtPoint2f source_point2f,
                                     const float source_depth_pixel_value,
                                     const DtCameraIntrinsic source_intrinsic,
                                     DtExtrinsic extrinsic,
                                     DtPoint3f* target_point3f,
                                     dt_error** error);

/**
 * @brief Projects a 3D point to 2D pixel coordinates in target camera.
 *
 * @param[in] source_point3f 3D point in source coordinate system.
 * @param[in] target_intrinsic Intrinsic parameters of target camera.
 * @param[in] target_distortion Distortion parameters of target camera.
 * @param[in] extrinsic Transform from source to target coordinate system.
 * @param[out] target_point2f Output 2D pixel coordinates in target camera.
 * @param[out] error Optional error object for capturing operation status.
 * @return bool True if projection succeeded, false otherwise.
 */
DT_EXPORT bool dt_transform_3d_to_2d(const DtPoint3f source_point3f,
                                     const DtCameraIntrinsic target_intrinsic,
                                     const DtCameraDistortion target_distortion,
                                     DtExtrinsic extrinsic,
                                     DtPoint2f* target_point2f,
                                     dt_error** error);

/**
 * @brief Transforms a 2D pixel with depth to 2D pixel in target camera.
 *
 * @param[in] source_point2f 2D pixel coordinates in source camera.
 * @param[in] source_depth_pixel_value Depth value at pixel location (in millimeters).
 * @param[in] source_intrinsic Intrinsic parameters of source camera.
 * @param[in] source_distortion Distortion parameters of source camera.
 * @param[in] target_intrinsic Intrinsic parameters of target camera.
 * @param[in] target_distortion Distortion parameters of target camera.
 * @param[in] extrinsic Transform from source to target coordinate system.
 * @param[out] target_point2f Output 2D pixel coordinates in target camera.
 * @param[out] error Optional error object for capturing operation status.
 * @return bool True if transform succeeded, false otherwise.
 */
DT_EXPORT bool dt_transform_2d_to_2d(const DtPoint2f source_point2f,
                                     const float source_depth_pixel_value,
                                     const DtCameraIntrinsic source_intrinsic,
                                     const DtCameraDistortion source_distortion,
                                     const DtCameraIntrinsic target_intrinsic,
                                     const DtCameraDistortion target_distortion,
                                     DtExtrinsic extrinsic,
                                     DtPoint2f* target_point2f,
                                     dt_error** error);

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
 * @param[out] error Optional error object for capturing operation status.
 * @return bool True if transformation succeeded, false otherwise.
 */
DT_EXPORT bool dt_colorize_depth(uint8_t* colored_depth,
                                 int rows,
                                 int cols,
                                 int range_min,
                                 int range_mid,
                                 int range_max,
                                 const uint16_t* depth_map,
                                 dt_error** error);

#ifdef __cplusplus
}
#endif

#endif  // DEPTRUM_H_DT_UTILS_H_
